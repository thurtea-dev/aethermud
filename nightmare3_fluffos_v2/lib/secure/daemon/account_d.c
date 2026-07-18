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

void create() {
    ::create();
    set_no_clean(1);
    account_name = 0;
    password = 0;
    characters = ({});
    email = "";
    last_character = "";
}

private string account_path(string name) {
    return LOGIN_ACCOUNTS_DIR + "/" + name[0..0] + "/" + name;
}

private void clear_slot() {
    account_name = 0;
    password = 0;
    characters = ({});
    email = "";
    last_character = "";
}

private void ensure_dirs(string name) {
    if(file_size(LOGIN_ACCOUNTS_DIR) != -2)
        unguarded((: mkdir, LOGIN_ACCOUNTS_DIR :));
    if(file_size(LOGIN_ACCOUNTS_DIR + "/" + name[0..0]) != -2)
        unguarded((: mkdir, LOGIN_ACCOUNTS_DIR + "/" + name[0..0] :));
}

int account_exists(string name) {
    if(!name || !sizeof(name)) return 0;
    name = lower_case(name);
    return file_size(account_path(name) + __SAVE_EXTENSION__) > -1;
}

private int load_account(string name) {
    int ok;

    name = lower_case(name);
    clear_slot();
    if(!account_exists(name)) return 0;
    ok = unguarded((: restore_object, account_path(name) :));
    return ok;
}

private void save_account() {
    if(!account_name || !sizeof(account_name)) return;
    ensure_dirs(account_name);
    unguarded((: save_object, account_path(account_name) :));
}

/* Create a new account. pass must already be crypt()'d. */
int create_account(string name, string pass, string *chars) {
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
    save_account();
    clear_slot();
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

int add_character(string acct, string charname) {
    string *chars;

    if(!acct || !charname) return 0;
    acct = lower_case(acct);
    charname = lower_case(charname);
    if(!load_account(acct)) return 0;
    chars = characters ? characters : ({});
    if(member_array(charname, chars) != -1) {
        clear_slot();
        return 0;
    }
    if(sizeof(chars) >= MAX_ACCOUNT_CHARS) {
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

/* Staff accounts are exempt from the one-active-character lock. */
int is_staff_account(string name) {
    string *chars;
    string *lines;
    string pos;
    string path;
    string file;
    int i, j;

    if(!name || !sizeof(name)) return 0;
    name = lower_case(name);
    if(member_group(name, "SECURE") || member_group(name, "ASSIST") ||
       member_group(name, "superuser") || member_group(name, "assist"))
        return 1;
    chars = query_characters(name);
    for(i = 0; i < sizeof(chars); i++) {
        if(find_player(chars[i]) && creatorp(find_player(chars[i])))
            return 1;
        path = DIR_USERS + "/" + chars[i][0..0] + "/" + chars[i] +
               __SAVE_EXTENSION__;
        if(file_size(path) < 1) continue;
        file = unguarded((: read_file, path :));
        if(!file) continue;
        lines = explode(file, "\n");
        for(j = 0; j < sizeof(lines); j++) {
            if(sscanf(lines[j], "position \"%s\"", pos) != 1) continue;
            if(pos && member_array(pos, MORTAL_POSITIONS) == -1)
                return 1;
            break;
        }
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
