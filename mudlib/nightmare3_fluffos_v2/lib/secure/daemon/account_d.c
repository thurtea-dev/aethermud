/* /secure/daemon/account_d.c
   Login accounts: one account name + password owns up to 5 characters.
   Character saves remain under /secure/save/users/. Account files live
   under /secure/save/login_accounts/<letter>/<account>.o. */

#include <std.h>
#include <security.h>
#include <daemons.h>
#include <dirs.h>
#include <config.h>

inherit DAEMON;

#define LOGIN_ACCOUNTS_DIR "/secure/save/login_accounts"
#define MAX_ACCOUNT_CHARS  5

string account_name;
string password;
string *characters;
string email;
string last_character;
int pending_approval;

/* Prototypes: callers above definitions need these on FluffOS 2.9. */
int is_staff_account(string name);
int owns_character(string acct, string charname);

void create() {
    ::create();
    set_no_clean(1);
    account_name = 0;
    password = 0;
    characters = ({});
    email = "";
    last_character = "";
    pending_approval = 0;
}

private string account_path(string name) {
    return LOGIN_ACCOUNTS_DIR + "/" + name[0..0] + "/" + name;
}

/* Privileged file probes. MANDATORY for every path under /secure/save/
   in this file -- do not "simplify" these back to bare efun calls.

   secure/cfg/read.cfg grants read on (/secure/save/) to ASSIST only, and
   write.cfg does the same for writes. master.c's check_access() walks the
   ENTIRE call stack (`stack = previous_object(-1) + ({ ob })`) and returns
   0 if any frame lacks the privilege, specifically so that calling through
   a privileged daemon cannot launder a caller's rights.

   Consequence, and the bug this fixes: reached from secure/std/login.c the
   whole stack is /secure/, so probes succeeded and account creation looked
   fine. Reached from cmds/adm/_accountapproval.c the stack also holds the
   invoking player object and a /cmds/ object, neither carrying ASSIST, so
   valid_read() denied and file_size() returned -1 -- indistinguishable
   from "file does not exist". account_exists() therefore reported 0 for
   accounts that were sitting on disk, and every admin-side lookup failed
   while the login side worked. Denials are silent to the caller but do
   land in /log/access as "Read access denied: ...".

   load_account()/save_account() were already correct because they wrap
   restore_object()/save_object() in unguarded(); only the bare probes
   were missed. */
private int acct_file_size(string p) {
    return (int)unguarded((: file_size, p :));
}

private string *acct_get_dir(string p) {
    return (string *)unguarded((: get_dir, p :));
}

private int acct_rm(string p) {
    return (int)unguarded((: rm, p :));
}

private void clear_slot() {
    account_name = 0;
    password = 0;
    characters = ({});
    email = "";
    last_character = "";
    pending_approval = 0;
}

/* Returns 1 only if both the base dir and the account's letter subdir
   exist afterward. mkdir()'s own return was previously discarded, and
   save_object() in FluffOS does not create missing parents (see the
   prepare_runtime_dirs() comment in mud.sh, which exists because this
   class of failure has bitten this lib before), so a failed mkdir here
   used to turn into a silently unwritten account file. */
private int ensure_dirs(string name) {
    string letter_dir;

    if(acct_file_size(LOGIN_ACCOUNTS_DIR) != -2)
        unguarded((: mkdir, LOGIN_ACCOUNTS_DIR :));
    if(acct_file_size(LOGIN_ACCOUNTS_DIR) != -2) {
        log_file("secure/account_d",
            sprintf("%s ensure_dirs: cannot create %s\n",
                ctime(time()), LOGIN_ACCOUNTS_DIR));
        return 0;
    }
    letter_dir = LOGIN_ACCOUNTS_DIR + "/" + name[0..0];
    if(acct_file_size(letter_dir) != -2)
        unguarded((: mkdir, letter_dir :));
    if(acct_file_size(letter_dir) != -2) {
        log_file("secure/account_d",
            sprintf("%s ensure_dirs: cannot create %s\n",
                ctime(time()), letter_dir));
        return 0;
    }
    return 1;
}

int account_exists(string name) {
    if(!name || !sizeof(name)) return 0;
    name = lower_case(name);
    return acct_file_size(account_path(name) + __SAVE_EXTENSION__) > -1;
}

private int load_account(string name) {
    int ok;

    name = lower_case(name);
    clear_slot();
    if(!account_exists(name)) return 0;
    ok = unguarded((: restore_object, account_path(name) :));
    return ok;
}

/* Returns 1 if the account file is actually on disk afterward. The old
   version returned void and discarded save_object()'s result, so every
   caller (create_account, set_pending_approval, add_character, ...) had
   no way to know a write had failed. */
private int save_account() {
    string path;

    if(!account_name || !sizeof(account_name)) return 0;
    if(!ensure_dirs(account_name)) return 0;
    path = account_path(account_name);
    unguarded((: save_object, path :));
    /* Verify rather than trust the return: confirms the bytes landed at
       the exact path account_exists() will later look for them. */
    if(acct_file_size(path + __SAVE_EXTENSION__) < 0) {
        log_file("secure/account_d",
            sprintf("%s save_account: write FAILED for %s\n",
                ctime(time()), path + __SAVE_EXTENSION__));
        return 0;
    }
    return 1;
}

/* Create a new account. pass must already be crypt()'d.
   Returns 1 only if the account file is verifiably on disk afterward.
   It previously returned 1 unconditionally, so a failed write reported
   success and login.c carried on building a character with no account
   behind it. */
int create_account(string name, string pass, string *chars) {
    int ok;

    if(!name || !sizeof(name) || !pass || !sizeof(pass)) return 0;
    name = lower_case(name);
    if(account_exists(name)) return 0;
    if(!chars) chars = ({});
    clear_slot();
    account_name = name;
    password = pass;
    characters = chars;
    email = "";
    last_character = sizeof(chars) ? chars[0] : "";
    ok = save_account();
    clear_slot();
    if(!ok) {
        log_file("secure/account_d",
            sprintf("%s create_account: FAILED to persist account '%s'\n",
                ctime(time()), name));
        return 0;
    }
    return 1;
}

/* Migrate: existing character .o becomes the first character on a new
   account of the same name. pass is the crypt hash from the character. */
int migrate_from_character(string name, string pass) {
    if(!name || !sizeof(name)) return 0;
    name = lower_case(name);
    if(account_exists(name)) return 1;
    if(!user_exists(name)) return 0;
    if(!pass || !sizeof(pass)) return 0;
    return create_account(name, pass, ({ name }));
}

int check_password(string name, string plain) {
    string pass;

    if(!name || !plain) return 0;
    name = lower_case(name);
    if(!load_account(name)) return 0;
    pass = password;
    clear_slot();
    if(!pass || !sizeof(pass)) return 0;
    return crypt(plain, pass) == pass;
}

string *query_characters(string name) {
    string *chars;

    if(!name || !sizeof(name)) return ({});
    name = lower_case(name);
    if(!load_account(name)) return ({});
    chars = characters ? characters + ({}) : ({});
    clear_slot();
    return chars;
}

int query_max_characters() { return MAX_ACCOUNT_CHARS; }

/* Staff accounts have no character cap. */
int at_character_limit(string acct) {
    string *chars;
    int staff;

    if(!acct || !sizeof(acct)) return 1;
    acct = lower_case(acct);
    staff = is_staff_account(acct);
    if(staff) return 0;
    chars = query_characters(acct);
    return sizeof(chars) >= MAX_ACCOUNT_CHARS;
}

int set_password(string name, string crypted_pass) {
    if(!name || !crypted_pass || !sizeof(crypted_pass)) return 0;
    name = lower_case(name);
    if(!load_account(name)) return 0;
    password = crypted_pass;
    save_account();
    clear_slot();
    return 1;
}

string query_password_hash(string name) {
    string pass;

    if(!name || !sizeof(name)) return 0;
    name = lower_case(name);
    if(!load_account(name)) return 0;
    pass = password;
    clear_slot();
    return pass;
}

/* Find which account owns a character name. Scans login_accounts. */
string account_for_character(string charname) {
    string *letters;
    string *files;
    string *chars;
    string acct;
    string dir;
    int i, j, k;

    if(!charname || !sizeof(charname)) return 0;
    charname = lower_case(charname);
    if(account_exists(charname) &&
       owns_character(charname, charname))
        return charname;
    if(acct_file_size(LOGIN_ACCOUNTS_DIR) != -2) return 0;
    letters = acct_get_dir(LOGIN_ACCOUNTS_DIR + "/");
    if(!letters) return 0;
    for(i = 0; i < sizeof(letters); i++) {
        if(!letters[i] || sizeof(letters[i]) != 1) continue;
        dir = LOGIN_ACCOUNTS_DIR + "/" + letters[i];
        if(acct_file_size(dir) != -2) continue;
        files = acct_get_dir(dir + "/");
        if(!files) continue;
        for(j = 0; j < sizeof(files); j++) {
            if(strsrch(files[j], __SAVE_EXTENSION__) == -1) continue;
            acct = replace_string(files[j], __SAVE_EXTENSION__, "");
            chars = query_characters(acct);
            for(k = 0; k < sizeof(chars); k++) {
                if(chars[k] == charname) return acct;
            }
        }
    }
    return 0;
}

int add_character(string acct, string charname) {
    string *chars;
    int staff;

    if(!acct || !charname) return 0;
    acct = lower_case(acct);
    charname = lower_case(charname);
    staff = is_staff_account(acct);
    if(!load_account(acct)) return 0;
    chars = characters ? characters : ({});
    if(member_array(charname, chars) != -1) {
        clear_slot();
        return 0;
    }
    if(!staff && sizeof(chars) >= MAX_ACCOUNT_CHARS) {
        clear_slot();
        return 0;
    }
    characters = chars + ({ charname });
    if(!last_character || !sizeof(last_character))
        last_character = charname;
    save_account();
    clear_slot();
    return 1;
}

/* Deletes a character from an account: drops it from the account's
   character list (fixing up last_character if it was the one removed)
   and wipes its save file. Player-initiated (secure/std/login.c's
   character-select delete flow), so no postal/realm cleanup like
   cmds/adm/_playerwipe.c does for an admin wipe -- a fresh mortal
   character has neither. */
int remove_character(string acct, string charname) {
    string *chars;
    string path, bak;

    if(!acct || !charname) return 0;
    acct = lower_case(acct);
    charname = lower_case(charname);
    if(!load_account(acct)) return 0;
    chars = characters ? characters : ({});
    if(member_array(charname, chars) == -1) {
        clear_slot();
        return 0;
    }
    characters = chars - ({ charname });
    if(last_character == charname)
        last_character = sizeof(characters) ? characters[0] : "";
    save_account();
    clear_slot();

    path = DIR_USERS + "/" + charname[0..0] + "/" + charname + __SAVE_EXTENSION__;
    bak  = path + ".bak";
    if(acct_file_size(path) >= 0) acct_rm(path);
    if(acct_file_size(bak)  >= 0) acct_rm(bak);
    return 1;
}

void set_last_character(string acct, string charname) {
    if(!acct || !charname) return;
    acct = lower_case(acct);
    charname = lower_case(charname);
    if(!load_account(acct)) return;
    if(member_array(charname, characters ? characters : ({})) == -1) {
        clear_slot();
        return;
    }
    last_character = charname;
    save_account();
    clear_slot();
}

string query_last_character(string name) {
    string last;

    if(!name || !sizeof(name)) return "";
    name = lower_case(name);
    if(!load_account(name)) return "";
    last = last_character ? last_character : "";
    clear_slot();
    return last;
}

void set_account_email(string name, string em) {
    if(!name) return;
    name = lower_case(name);
    if(!load_account(name)) return;
    email = em ? em : "";
    save_account();
    clear_slot();
}

string query_account_email(string name) {
    string em;

    if(!name || !sizeof(name)) return "";
    name = lower_case(name);
    if(!load_account(name)) return "";
    em = email ? email : "";
    clear_slot();
    return em;
}

/* Only ever set true for a brand-new account created after this MUD
   already had an admin (secure/std/login.c confirm_password()). The
   very first account on a fresh install is never flagged, so it can
   still complete the first-admin bootstrap offer without a chicken-
   and-egg deadlock (nobody exists yet to approve it). Old accounts
   saved before this field existed restore with pending_approval == 0
   (the int default), so they are never retroactively locked out. */
void set_pending_approval(string name, int val) {
    int ok;

    if(!name || !sizeof(name)) return;
    name = lower_case(name);
    if(!load_account(name)) {
        /* Reached when the account file is missing, which is exactly the
           state that makes the login flow show "pending approval" and
           then leave nothing for accountapproval to find. Log it so the
           two halves can be correlated instead of guessed at. */
        log_file("secure/account_d",
            sprintf("%s set_pending_approval(%s, %d): no account file to "
                "update\n", ctime(time()), name, val));
        return;
    }
    pending_approval = val ? 1 : 0;
    ok = save_account();
    clear_slot();
    if(!ok)
        log_file("secure/account_d",
            sprintf("%s set_pending_approval: FAILED to persist flag %d "
                "for '%s'\n", ctime(time()), val, name));
}

int is_pending_approval(string name) {
    int val;

    if(!name || !sizeof(name)) return 0;
    name = lower_case(name);
    if(!load_account(name)) return 0;
    val = pending_approval;
    clear_slot();
    return val;
}

/* Admin-facing: accounts still awaiting approval. Staff accounts are
   never listed, even if somehow flagged, since they are always exempt. */
string *list_pending_accounts() {
    string *letters;
    string *files;
    string *result;
    string acct;
    string dir;
    int i, j;

    result = ({});
    if(acct_file_size(LOGIN_ACCOUNTS_DIR) != -2) return result;
    letters = acct_get_dir(LOGIN_ACCOUNTS_DIR + "/");
    if(!letters) return result;
    for(i = 0; i < sizeof(letters); i++) {
        if(!letters[i] || sizeof(letters[i]) != 1) continue;
        dir = LOGIN_ACCOUNTS_DIR + "/" + letters[i];
        if(acct_file_size(dir) != -2) continue;
        files = acct_get_dir(dir + "/");
        if(!files) continue;
        for(j = 0; j < sizeof(files); j++) {
            if(strsrch(files[j], __SAVE_EXTENSION__) == -1) continue;
            acct = replace_string(files[j], __SAVE_EXTENSION__, "");
            if(!is_pending_approval(acct)) continue;
            if(is_staff_account(acct)) continue;
            result += ({ acct });
        }
    }
    return result;
}

int approve_account(string name) {
    if(!name || !sizeof(name)) return 0;
    name = lower_case(name);
    if(!account_exists(name)) return 0;
    set_pending_approval(name, 0);
    return 1;
}

/* Single-character version of the position-file scan below is_staff_account()
   already did per-character in a loop. Pulled out so login.c's delete-
   character flow can refuse to delete a wizard-positioned character
   without duplicating the file-scan logic. */
int is_staff_character(string charname) {
    string *lines;
    string pos;
    string path;
    string file;
    int j;

    if(!charname || !sizeof(charname)) return 0;
    charname = lower_case(charname);
    if(find_player(charname) && creatorp(find_player(charname))) return 1;
    path = DIR_USERS + "/" + charname[0..0] + "/" + charname +
           __SAVE_EXTENSION__;
    if(acct_file_size(path) < 1) return 0;
    file = unguarded((: read_file, path :));
    if(!file) return 0;
    lines = explode(file, "\n");
    for(j = 0; j < sizeof(lines); j++) {
        if(sscanf(lines[j], "position \"%s\"", pos) != 1) continue;
        if(pos && member_array(pos, MORTAL_POSITIONS) == -1) return 1;
        break;
    }
    return 0;
}

/* Staff accounts are exempt from the one-active-character lock. */
int is_staff_account(string name) {
    string *chars;
    int i;

    if(!name || !sizeof(name)) return 0;
    name = lower_case(name);
    if(member_group(name, "SECURE") || member_group(name, "ASSIST") ||
       member_group(name, "superuser") || member_group(name, "assist"))
        return 1;
    chars = query_characters(name);
    for(i = 0; i < sizeof(chars); i++) {
        if(is_staff_character(chars[i])) return 1;
    }
    return 0;
}

/* Returns the name of an interactive sibling, or 0 if none. */
string sibling_online(string acct, string except_name) {
    string *chars;
    object ob;
    int i;

    if(!acct) return 0;
    acct = lower_case(acct);
    if(except_name) except_name = lower_case(except_name);
    if(is_staff_account(acct)) return 0;
    chars = query_characters(acct);
    for(i = 0; i < sizeof(chars); i++) {
        if(except_name && chars[i] == except_name) continue;
        ob = find_player(chars[i]);
        if(ob && interactive(ob)) return chars[i];
    }
    return 0;
}

int owns_character(string acct, string charname) {
    string *chars;

    if(!acct || !charname) return 0;
    chars = query_characters(lower_case(acct));
    return member_array(lower_case(charname), chars) != -1;
}
