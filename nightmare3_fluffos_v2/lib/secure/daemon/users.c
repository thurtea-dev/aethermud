#include <std.h>
#include <objects.h>
#include <security.h>
#include <commands.h>
#include <daemons.h>
#include "users.h"

inherit DAEMON;
inherit OB_USER;

void create() {
    daemon::create();
    user::create();
}

void change_password(string who, string passwd) {
    string crypted;
    string acct;

    if(!((int)master()->valid_apply( ({ "law" }) ))) return;
    if(!stringp(who) || !stringp(passwd)) return;
    if(member_group(who, "assist") || member_group(who, "superuser")) return;
    if(!user_exists(who)) return;
    if(find_player(who)) return;
    crypted = crypt(passwd, 0);
    unguarded((: restore_object, DIR_USERS+"/"+who[0..0]+"/"+who :));
    this_object()->set_password(crypted);
    unguarded((: save_object, DIR_USERS+"/"+who[0..0]+"/"+who :));
    /* Keep login-account hash aligned with the character password. */
    acct = (string)ACCOUNT_D->account_for_character(who);
    if(!acct || !sizeof(acct)) acct = who;
    if((int)ACCOUNT_D->account_exists(acct))
        ACCOUNT_D->set_password(acct, crypted);
}

void xmote(string who, string pos, int lev) {
    if(!((int)master()->valid_apply( ({ "assist" }) ))) return;
    if(!stringp(who) || !user_exists(who)) return;
    if(member_group(who, "superuser")) return;
    if(find_player(who)) return;
    unguarded((: restore_object, DIR_USERS+"/"+who[0..0]+"/"+who :));
    if(pos) this_object()->set_position(pos);
    if(lev) this_object()->set_level(lev);
    unguarded((: save_object, DIR_USERS+"/"+who[0..0]+"/"+who :));
}

/* Full demote restore for an offline save. player_object() is
   login-only, so restore into this daemon (inherits OB_USER), apply
   staff_of_demotion restore, then save back. */
int offline_demote(string who) {
    object staff;
    string path;

    if(!((int)master()->valid_apply( ({ "assist" }) ))) return 0;
    if(!stringp(who) || !user_exists(who)) return 0;
    if(find_player(who)) return 0;
    path = DIR_USERS + "/" + who[0..0] + "/" + who;
    unguarded((: restore_object, path :));
    staff = load_object("/domains/adm/wiz_tools/staff_of_demotion");
    if(!staff) return 0;
    staff->apply_demote_restore(this_object());
    unguarded((: save_object, path :));
    return 1;
}

void suicide(string who) {
    if(!((int)master()->valid_apply(({ who })))) return;
    if(!((int)master()->valid_apply(({ (string)this_player(1)->query_name()}))))
      return;
    if(archp(this_player())) return;
    this_player()->remove();
    log_file("suicide", who+" at "+ctime(time())+"\n");
    delete_user(who);
}

static private void delete_user(string who) {
    string tmp;

    rename(sprintf("%s/%s/%s%s", DIR_USERS, who[0..0], who, __SAVE_EXTENSION__),
      sprintf("%s/rid/%s%s", DIR_USERS, who, __SAVE_EXTENSION__));
    if(unguarded((: file_exists, tmp = DIR_ACCOUNTS+"/"+who+__SAVE_EXTENSION__ :)))
      unguarded((: rm, tmp :));
}

void setup() { }

int remove() { return daemon::remove(); }

