// /secure/cmds/adm/_godling.c
// Admin command: apply the Godling race/RCC to a player.
// Usage: godling <playername>
// Requires arch or higher rank.

#include <std.h>
#include <daemons.h>
#include <privs.h>

inherit DAEMON;

int cmd_godling(string str) {
    object target;
    mapping rolls;
    string *rflags;

    if(!archp(previous_object())) {
        notify_fail("Only arches and above may use this command.\n");
        return 0;
    }
    if(!str || !strlen(str)) {
        notify_fail("Usage: godling <playername>\n");
        return 0;
    }
    target = find_player(lower_case(str));
    if(!target) {
        notify_fail("Player '" + str + "' is not online.\n");
        return 0;
    }

    // Apply godling stats
    rolls = (mapping)RIFTS_D->do_rifts_rolls("godling");
    if(rolls) {
        string *keys_arr;
        int i, tmp;
        keys_arr = keys(rolls);
        for(i = 0, tmp = sizeof(keys_arr); i < tmp; i++)
            target->set_stats(keys_arr[i], rolls[keys_arr[i]]);
    }

    // MDC pools
    target->set_stats("MDC",          5000);
    target->set_stats("max_MDC",      5000);
    target->set_stats("SDC",          0);
    target->set_stats("max_SDC",      0);
    target->set_stats("is_MDC",       1);
    target->set_stats("rifts_hp",     1000);
    target->set_stats("max_rifts_hp", 1000);

    // Psionic and magic pools
    target->set_stats("PPE",          2000);
    target->set_stats("max_PPE",      2000);
    target->set_stats("ISP",          2000);
    target->set_stats("max_ISP",      2000);

    // Race and class
    target->set_race("godling");
    target->set_class("godling");

    // Rifts env vars
    target->remove_env("awaiting_occ");
    target->remove_env("awaiting_alignment");
    target->setenv("rifts_occ",        "none");
    target->setenv("rifts_occ_flags",  "magic,psychic,borg,cybernetic");
    target->setenv("rifts_alignment",  "Principled");
    target->setenv("rifts_flags",      "");

    // Race ability flags
    rflags = (string *)RIFTS_D->query_race_flags("godling");
    target->setenv("rifts_flags", implode(rflags, ","));

    // Max level and exp
    target->set_level(15);
    target->set_exp(150000);
    target->set_alignment(900);

    // Fix primary_start
    target->set_primary_start("/domains/Praxis/rifts_welcome");

    catch(RIFTS_START_D->grant_race_package(target));
    catch(LANGUAGE_D->grant_native_languages(target));
    target->setenv("active_language", "American");

    write("Godling transformation applied to " + capitalize(str) + ".");
    message("info",
        "\nA wave of divine energy cascades through your being.  You have\n"
        "transcended mortal limits.  You are a Godling.",
        target);
    return 1;
}

void help() {
    write(
        "Syntax: godling <playername>\n\n"
        "Transforms the target player into a Godling, the admin race.\n"
        "Sets all Palladium attributes to 30, grants 5000 MDC, 1000 rifts HP,\n"
        "2000 PPE and ISP, and applies all divine ability flags.\n"
        "Requires arch rank.  Target must be online.\n"
    );
}
