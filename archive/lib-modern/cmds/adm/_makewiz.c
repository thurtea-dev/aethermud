/* /cmds/adm/_makewiz.c
   Promotes an online mortal player to wizard (creator) status.
   Required before setrole can assign a wizard role.
   Restricted to admin-level wizards. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_makewiz(string str) {
    string pname;
    string cur_pos;
    object target;
    mapping rolls;
    string *rflags;
    string *keys_arr;
    int gi, gtmp;

    if(!archp(this_player()) &&
       (string)this_player()->getenv("wiz_role") != "admin") {
        write("Access denied.\n");
        return 1;
    }
    if(!str || !strlen(str)) {
        write("Syntax: makewiz <player>\n");
        return 1;
    }
    pname = lower_case(str);
    target = find_player(pname);
    if(!target) {
        write("Player '" + capitalize(pname) + "' is not online.\n");
        return 1;
    }
    if(creatorp(target)) {
        cur_pos = (string)target->query_position();
        write(capitalize(pname) + " is already a wizard (position: " +
              cur_pos + ").\n");
        return 1;
    }
    target->set_position("creator");
    if(!creatorp(target)) {
        write("Promotion failed: set_position rejected. You must be arch to use makewiz.\n");
        return 1;
    }

    /* Apply Godling RCC on wizard promotion. */
    rolls = (mapping)RIFTS_D->do_rifts_rolls("godling");
    if(rolls) {
        keys_arr = keys(rolls);
        for(gi = 0, gtmp = sizeof(keys_arr); gi < gtmp; gi++)
            target->set_stats(keys_arr[gi], rolls[keys_arr[gi]]);
    }
    target->set_stats("MDC",          5000);
    target->set_stats("max_MDC",      5000);
    target->set_stats("SDC",          0);
    target->set_stats("max_SDC",      0);
    target->set_stats("is_MDC",       1);
    target->set_stats("rifts_hp",     1000);
    target->set_stats("max_rifts_hp", 1000);
    target->set_stats("PPE",          2000);
    target->set_stats("max_PPE",      2000);
    target->set_stats("ISP",          2000);
    target->set_stats("max_ISP",      2000);
    target->set_race("godling");
    target->set_class("godling");
    target->remove_env("awaiting_occ");
    target->remove_env("awaiting_alignment");
    target->setenv("rifts_occ",         "none");
    target->setenv("rifts_occ_flags",   "magic,psychic,borg,cybernetic");
    target->setenv("rifts_alignment",   "Principled");
    rflags = (string *)RIFTS_D->query_race_flags("godling");
    target->setenv("rifts_flags", implode(rflags, ","));
    target->setenv("always_known",        "1");
    target->setenv("speak_all_languages", "1");
    target->set_alignment(900);
    target->set_primary_start("/domains/Praxis/rifts_welcome");
    catch(RIFTS_START_D->grant_race_package(target));
    catch(LANGUAGE_D->grant_native_languages(target));
    target->setenv("active_language", "American");

    target->save_player((string)target->query_name());
    write("Promoted " + (string)target->query_cap_name() +
          " to wizard (creator) and applied Godling RCC.\n");
    tell_object(target,
        "%^YELLOW%^You have been promoted to wizard status by " +
        (string)this_player()->query_cap_name() +
        " and transformed into a Godling.%^RESET%^\n");
    log_file("adm/makewiz",
        ctime(time()) + " " + (string)this_player()->query_name() +
        " promoted " + pname + " to wizard (godling)\n");
    return 1;
}

void help() {
    write(
        "Syntax: makewiz <player>\n\n"
        "Promotes an online mortal to wizard (creator) status and applies the\n"
        "Godling RCC.  The player must be online.  Wizard status is required\n"
        "before setrole can assign an admin, domain, coding, or rp role.\n\n"
        "Typical sequence:\n"
        "  makewiz splynncryth\n"
        "  setrole splynncryth admin\n\n"
        "Restricted to admin-level wizards.  All promotions are logged.\n\n"
        "See also: setrole, godling\n"
    );
}
