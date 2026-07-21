// /secure/cmds/adm/_wizard.c
// Admin command: apply the Wizard race/RCC to a player.
// Usage: wizard <playername>
// Requires arch or higher rank.

#include <std.h>
#include <daemons.h>
#include <privs.h>

inherit DAEMON;

int cmd_wizard(string str) {
    object target;
    mapping rolls;
    string *rflags;

    if(!archp(previous_object())) {
        notify_fail("Only arches and above may use this command.\n");
        return 0;
    }
    if(!str || !strlen(str)) {
        notify_fail("Usage: wizard <playername>\n");
        return 0;
    }
    target = find_player(lower_case(str));
    if(!target) {
        notify_fail("Player '" + str + "' is not online.\n");
        return 0;
    }

    // Apply wizard stats
    rolls = (mapping)RIFTS_D->do_rifts_rolls("wizard");
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
    target->set_race("wizard");
    target->set_class("wizard");

    // Rifts env vars
    target->remove_env("awaiting_occ");
    target->remove_env("awaiting_alignment");
    target->setenv("rifts_occ",        "none");
    target->setenv("rifts_occ_flags",  "magic,psychic,borg,cybernetic");
    target->setenv("rifts_alignment",  "Principled");
    target->setenv("rifts_flags",      "");

    // Race ability flags
    rflags = (string *)RIFTS_D->query_race_flags("wizard");
    target->setenv("rifts_flags", implode(rflags, ","));

    // Max level and exp
    target->set_level(15);
    target->set_exp(150000);
    target->set_alignment(900);

    // Fix primary_start
    target->set_primary_start("/domains/ChiTown/areas/chitown_start");

    catch(RIFTS_START_D->grant_race_package(target));
    catch(LANGUAGE_D->grant_native_languages(target));
    target->setenv("active_language", "American");

    write("Wizard transformation applied to " + capitalize(str) + ".");
    message("info",
        "\nArcane power cascades through your being.  You have transcended\n"
        "mortal limits.  You are a Wizard.",
        target);
    return 1;
}

void help() {
    write(
        "Syntax: wizard <playername>\n\n"
        "Transforms the target player into a Wizard, the admin race.\n"
        "Sets all Palladium attributes to 30, grants 5000 MDC, 1000 rifts HP,\n"
        "2000 PPE and ISP, and applies all archmage ability flags.\n"
        "Requires arch rank.  Target must be online.\n"
    );
}
