/*  /cmds/mortal/_password.c
 *  Player-facing password change command.
 *  Implements the prompt flow directly (cannot delegate to _passwd.c
 *  because its security check requires previous_object to be the player).
 */

#include <std.h>
#include <daemons.h>
#include "/secure/include/flags.h"

inherit "/std/daemon";

private void sync_account_password(object who, string crypted) {
    string acct;

    if(!who || !crypted || !sizeof(crypted)) return;
    acct = (string)who->getenv("login_account");
    if(!acct || !sizeof(acct))
        acct = (string)ACCOUNT_D->account_for_character(
          (string)who->query_name());
    if(!acct || !sizeof(acct))
        acct = (string)who->query_name();
    if(acct && (int)ACCOUNT_D->account_exists(acct))
        ACCOUNT_D->set_password(acct, crypted);
}

int cmd_password(string str) {
    string nom;

    nom = (string)this_player()->query_name();
    if(!nom || nom == "guest") {
        notify_fail("Guests cannot change passwords.\n");
        return 0;
    }
    message("system", "Changing your password.", this_player());
    message("prompt", "Old password: ", this_player());
    input_to("check_old", I_NOECHO | I_NOESC);
    return 1;
}

static void check_old(string pass) {
    string stored;

    if(!pass || !strlen(pass)) {
        message("system", "Password change cancelled.", this_player());
        return;
    }
    stored = (string)this_player()->query_password();
    if(stored != crypt(pass, stored)) {
        message("system", "Incorrect password.", this_player());
        return;
    }
    message("prompt", "\nNew password: ", this_player());
    input_to("enter_new", I_NOECHO | I_NOESC);
}

static void enter_new(string pass) {
    if(!pass || strlen(pass) < 5) {
        message("system",
            "\nPasswords must be at least 5 characters long.",
            this_player());
        return;
    }
    message("prompt", "\nConfirm new password: ", this_player());
    input_to("confirm_new", I_NOECHO | I_NOESC, pass);
}

static void confirm_new(string pass2, string pass1) {
    string crypted;

    if(pass1 != pass2) {
        message("system", "\nPasswords do not match. Password not changed.", this_player());
        return;
    }
    crypted = crypt(pass2, 0);
    this_player()->set_password(crypted);
    sync_account_password(this_player(), crypted);
    message("system", "\nPassword changed successfully.", this_player());
}

void help() {
    message("help",
        "Syntax: password\n\n"
        "Changes your in-game password. You will be prompted for your current\n"
        "password first, then asked to enter and confirm a new one.\n"
        "Passwords must be at least 5 characters long.\n\n"
        "If you forget your password entirely, contact a wizard for assistance.\n"
        "Do not share your password with other players.",
        this_player());
}
