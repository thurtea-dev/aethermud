/* /cmds/adm/_makewiz.c
   Promotes an online mortal to Apprentice wizard.
   Temporary staff rank until setrole assigns admin/domain/coding/rp.
   Restricted to admin-level wizards. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_makewiz(string str) {
    string pname;
    string cur_pos;
    string psnap;
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
    /* Snapshot the mortal identity before promotion overwrites it, so
       the staff of demotion can restore it later. premote_stats holds
       19 space-separated values in this fixed order: IQ ME MA PS PP PE
       PB Spd MDC max_MDC SDC max_SDC is_MDC rifts_hp max_rifts_hp PPE
       max_PPE ISP max_ISP. Also stores premote_level. Keep in sync with
       demote_restore() in /domains/adm/wiz_tools/staff_of_demotion.c. */
    psnap = (string)target->query_race();
    target->setenv("premote_race", (psnap && sizeof(psnap)) ? psnap : "human");
    psnap = (string)target->query_class();
    if(psnap && sizeof(psnap)) target->setenv("premote_class", psnap);
    psnap = (string)target->getenv("rifts_occ");
    target->setenv("premote_occ", (psnap && sizeof(psnap)) ? psnap : "none");
    psnap = (string)target->getenv("rifts_occ_flags");
    if(psnap && sizeof(psnap)) target->setenv("premote_occ_flags", psnap);
    psnap = (string)target->getenv("rifts_flags");
    if(psnap && sizeof(psnap)) target->setenv("premote_flags", psnap);
    psnap = (string)target->getenv("rifts_alignment");
    if(psnap && sizeof(psnap)) target->setenv("premote_alignment", psnap);
    target->setenv("premote_align",
        sprintf("%d", (int)target->query_alignment()));
    psnap = (string)target->getenv("active_language");
    if(psnap && sizeof(psnap)) target->setenv("premote_language", psnap);
    psnap = (string)target->query_primary_start();
    if(psnap && sizeof(psnap)) target->setenv("premote_start", psnap);
    target->setenv("premote_level",
        sprintf("%d", (int)target->query_level()));
    target->setenv("premote_stats", sprintf(
        "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
        (int)target->query_stats("IQ"), (int)target->query_stats("ME"),
        (int)target->query_stats("MA"), (int)target->query_stats("PS"),
        (int)target->query_stats("PP"), (int)target->query_stats("PE"),
        (int)target->query_stats("PB"), (int)target->query_stats("Spd"),
        (int)target->query_stats("MDC"), (int)target->query_stats("max_MDC"),
        (int)target->query_stats("SDC"), (int)target->query_stats("max_SDC"),
        (int)target->query_stats("is_MDC"),
        (int)target->query_stats("rifts_hp"),
        (int)target->query_stats("max_rifts_hp"),
        (int)target->query_stats("PPE"), (int)target->query_stats("max_PPE"),
        (int)target->query_stats("ISP"), (int)target->query_stats("max_ISP")));

    target->set_position("apprentice");
    if(!creatorp(target) ||
       lower_case((string)target->query_position()) != "apprentice") {
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
    target->setenv("TITLE", "Apprentice $N");
    catch(target->set_cwd_home());

    target->save_player((string)target->query_name());
    write("Promoted " + (string)target->query_cap_name() +
          " to Apprentice wizard and applied Godling RCC.\n"
          "Use setrole when they are ready for a full staff role.\n");
    tell_object(target,
        "%^YELLOW%^You have been promoted to Apprentice wizard by " +
        (string)this_player()->query_cap_name() +
        " and transformed into a Godling.%^RESET%^\n"
        "You are temporary staff until you complete apprenticeship tasks\n"
        "and an admin assigns your role with setrole.\n");
    log_file("adm/makewiz",
        ctime(time()) + " " + (string)this_player()->query_name() +
        " promoted " + pname + " to apprentice (godling)\n");
    return 1;
}

void help() {
    write(
        "Syntax: makewiz <player>\n\n"
        "Promotes an online mortal to Apprentice wizard and applies the\n"
        "Godling RCC.  Apprentices are temporary staff: they get a workroom\n"
        "and basic wizard access, but no role staff tools until setrole\n"
        "assigns admin, domain, coding, or rp.\n\n"
        "Typical sequence:\n"
        "  makewiz splynncryth\n"
        "  (apprentice completes tasks)\n"
        "  setrole splynncryth coding\n\n"
        "Restricted to admin-level wizards.  All promotions are logged.\n\n"
        "See also: setrole, godling, staff\n"
    );
}
