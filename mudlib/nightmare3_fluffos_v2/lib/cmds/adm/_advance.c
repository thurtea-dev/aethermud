/* /cmds/adm/_advance.c
   Wizard tool: force-advance a player one or more levels, bypassing XP check. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private int roll_dice(string dice_str) {
    int num, sides, bonus, total, i;

    if(!dice_str || !sizeof(dice_str)) return 1;
    num = 0; sides = 0; bonus = 0;
    if(sscanf(dice_str, "%dd%d+%d", num, sides, bonus) == 3) {
        /* NdS+B */
    } else if(sscanf(dice_str, "%dd%d", num, sides) == 2) {
        bonus = 0;
    } else {
        return to_int(dice_str) > 0 ? to_int(dice_str) : 1;
    }
    if(num < 1)   num   = 1;
    if(sides < 1) sides = 6;
    total = 0;
    for(i = 0; i < num; i++) total += random(sides) + 1;
    return total + bonus;
}

int cmd_advance(string str) {
    string name;
    object target;
    int levels, i;
    int level, hp_gain, sdc_gain, ppe_gain, isp_gain;
    mapping occ_data;
    string occ, hp_dice;
    int cur_hp, max_hp, cur_sdc, max_sdc, max_ppe, max_isp;

    if(!creatorp(this_player())) {
        write("Access denied.\n");
        return 1;
    }
    if(!str) {
        write("Syntax: advance <player> [<levels>]\n");
        return 1;
    }
    levels = 1;
    if(sscanf(str, "%s %d", name, levels) != 2) {
        name = str;
        levels = 1;
    }
    name   = lower_case(name);
    target = find_player(name);
    if(!target) {
        write("Player '" + capitalize(name) + "' is not online.\n");
        return 1;
    }
    if(!(int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        write(capitalize(name) + " is not a Rifts character.\n");
        return 1;
    }
    if(levels < 1) levels = 1;
    if(levels > 5) levels = 5;

    occ      = (string)target->getenv("rifts_occ");
    occ_data = (occ && sizeof(occ)) ? (mapping)OCC_D->query_occ(occ) : 0;
    hp_dice  = occ_data ? (string)occ_data["hp_per_level"] : "1d6";
    ppe_gain = occ_data ? (int)occ_data["ppe_per_level"]   : 0;
    isp_gain = occ_data ? (int)occ_data["isp_per_level"]   : 0;
    if(!hp_dice || !sizeof(hp_dice)) hp_dice = "1d6";

    for(i = 0; i < levels; i++) {
        level = (int)target->query_level();
        if(level >= 15) {
            write(capitalize(name) + " is already at max level (15).\n");
            break;
        }
        target->set_level(level + 1);

        hp_gain = roll_dice(hp_dice);
        if(hp_gain < 1) hp_gain = 1;
        cur_hp = (int)target->query_stats("rifts_hp");
        max_hp = (int)target->query_stats("max_rifts_hp");
        target->set_stats("rifts_hp",     cur_hp + hp_gain);
        target->set_stats("max_rifts_hp", max_hp + hp_gain);

        sdc_gain = random(4) + 1;
        cur_sdc  = (int)target->query_stats("SDC");
        max_sdc  = (int)target->query_stats("max_SDC");
        if(max_sdc > 0) {
            target->set_stats("SDC",     cur_sdc + sdc_gain);
            target->set_stats("max_SDC", max_sdc + sdc_gain);
        }
        if(ppe_gain > 0) {
            max_ppe = (int)target->query_stats("max_PPE");
            target->set_stats("max_PPE", max_ppe + ppe_gain);
            target->set_stats("PPE", (int)target->query_stats("PPE") + ppe_gain);
        }
        if(isp_gain > 0) {
            max_isp = (int)target->query_stats("max_ISP");
            target->set_stats("max_ISP", max_isp + isp_gain);
            target->set_stats("ISP", (int)target->query_stats("ISP") + isp_gain);
        }
        write(sprintf("  %s -> level %d (HP +%d, SDC +%d)\n",
            capitalize(name), level + 1, hp_gain, sdc_gain));
        tell_object(target,
            "%^YELLOW%^" + (string)this_player()->query_cap_name() +
            " advances you to level " + (level + 1) + "!%^RESET%^\n");
    }
    return 1;
}

void help() {
    write("Syntax: advance <player> [<levels>]\n\n"
          "Force-advance a Rifts player one or more levels (max 5 at once),\n"
          "bypassing the XP requirement. HP, SDC, PPE, ISP are updated.\n"
          "Example: advance bob 2\n"
          "Restricted to wizards.\n");
}
