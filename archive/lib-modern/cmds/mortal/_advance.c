/* /cmds/mortal/_advance.c */
/* RiftsMUD - Rifts character level advancement command. */
/* Checks OCC XP table; advances level, rolls HP or MDC gain, adds PPE/ISP. */
/* Non-Rifts characters are directed to guild hall as before. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

/* Roll a dice string "NdS" or "NdS+B". Returns the sum. */
private int roll_dice(string dice_str) {
    int num, sides, bonus, total, i;

    if(!dice_str || !sizeof(dice_str)) return 1;
    num = 0; sides = 0; bonus = 0;
    if(sscanf(dice_str, "%dd%d+%d", num, sides, bonus) == 3) {
        /* NdS+B format parsed above */
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

/* Returns 1 if the player is an MDC-type being. */
/* Same multi-source check as _score.c to handle pre-fix and post-fix chars. */
private int is_mdc_being(object who) {
    string env_mdc;
    string race;

    if((int)who->query_stats("is_MDC") == 1) return 1;
    if((int)who->query_property("rifts_mdc_race") == 1) return 1;
    env_mdc = (string)who->getenv("rifts_mdc_race");
    if(env_mdc && env_mdc != "" && to_int(env_mdc) == 1) return 1;
    race = (string)who->query_race();
    if(race && (int)RIFTS_D->is_mdc_race(race)) return 1;
    return 0;
}

int cmd_advance(string str) {
    string race, occ, stat, pending, hp_dice, msg;
    int level, exp, next_exp, new_level;
    int hp_gain, sdc_gain, mdc_gain, ppe_gain, isp_gain;
    int cur_hp, max_hp, cur_sdc, max_sdc;
    int cur_mdc, max_mdc, max_ppe, max_isp;
    int cur_stat, apm_bonus, pending_slots, is_mdc;
    int cur_lives, lives_initted;
    mapping occ_data;

    if(creatorp(this_player())) {
        notify_fail("Creators do not advance this way.\n");
        return 0;
    }
    if(this_player()->query_ghost()) {
        notify_fail("The dead cannot advance.\n");
        return 0;
    }

    race = (string)this_player()->query_race();

    if(!(int)RIFTS_D->is_rifts_race(race)) {
        notify_fail("You must visit your guild hall to advance.\n");
        return 0;
    }

    /* Handle "advance pick <stat>" for even-level stat bonus choice */
    if(str && sscanf(str, "pick %s", stat) == 1) {
        pending = (string)this_player()->getenv("pending_stat_choice");
        if(!pending || pending != "1") {
            write("You don't have a stat bonus to assign right now.\n");
            return 1;
        }
        stat = lower_case(stat);
        if(stat != "ps" && stat != "pp" && stat != "pe" && stat != "spd") {
            write("Choose a valid stat to raise: PS, PP, PE, or Spd.\n");
            return 1;
        }
        if(stat == "ps")  stat = "PS";
        if(stat == "pp")  stat = "PP";
        if(stat == "pe")  stat = "PE";
        if(stat == "spd") stat = "Spd";
        cur_stat = (int)this_player()->query_stats(stat);
        this_player()->set_stats(stat, cur_stat + 1);
        this_player()->setenv("pending_stat_choice", "0");
        write("Your " + stat + " increases to " + (cur_stat + 1) + "!\n");
        return 1;
    }

    occ   = (string)this_player()->getenv("rifts_occ");
    level = (int)this_player()->query_level();
    exp   = (int)this_player()->query_exp();

    if(level >= 15) {
        write("You have reached the maximum level (15). You can advance no further.\n");
        return 1;
    }

    /* Look up OCC per-level gains */
    occ_data = 0;
    if(occ && occ != "none" && occ != "")
        occ_data = (mapping)OCC_D->query_occ(occ);

    next_exp = (int)OCC_D->query_next_level_exp(this_player());
    if(occ_data) {
        hp_dice  = (string)occ_data["hp_per_level"];
        ppe_gain = (int)occ_data["ppe_per_level"];
        isp_gain = (int)occ_data["isp_per_level"];
    } else {
        hp_dice  = "1d6";
        ppe_gain = 0;
        isp_gain = 0;
    }

    if(exp < next_exp) {
        write(sprintf(
            "You are not experienced enough to advance.\n"
            "  Level: %d    XP: %d    Next level: %d    Needed: %d\n",
            level, exp, next_exp, next_exp - exp));
        return 1;
    }

    /* Advance level */
    new_level = level + 1;
    this_player()->set_level(new_level);

    is_mdc = is_mdc_being(this_player());
    msg    = sprintf("\nYou advance to level %d!\n", new_level);

    if(is_mdc) {
        /* MDC beings gain flat +10 MDC per level instead of HP/SDC */
        mdc_gain = 10;
        cur_mdc  = (int)this_player()->query_stats("MDC");
        max_mdc  = (int)this_player()->query_stats("max_MDC");
        if(max_mdc == 0) {
            /* Pre-fix characters: MDC stored in SDC slot */
            cur_mdc = (int)this_player()->query_stats("SDC");
            max_mdc = (int)this_player()->query_stats("max_SDC");
            this_player()->set_stats("SDC",     cur_mdc + mdc_gain);
            this_player()->set_stats("max_SDC", max_mdc + mdc_gain);
        } else {
            this_player()->set_stats("MDC",     cur_mdc + mdc_gain);
            this_player()->set_stats("max_MDC", max_mdc + mdc_gain);
        }
        msg += sprintf("  MDC gain: +%d  (new max: %d)\n", mdc_gain, max_mdc + mdc_gain);
    } else {
        /* SDC beings gain HP from OCC dice, plus small SDC gain */
        if(!hp_dice || !sizeof(hp_dice)) hp_dice = "1d6";
        hp_gain = roll_dice(hp_dice);
        if(hp_gain < 1) hp_gain = 1;

        cur_hp = (int)this_player()->query_stats("rifts_hp");
        max_hp = (int)this_player()->query_stats("max_rifts_hp");
        this_player()->set_stats("rifts_hp",     cur_hp + hp_gain);
        this_player()->set_stats("max_rifts_hp", max_hp + hp_gain);

        sdc_gain = random(4) + 1;
        cur_sdc  = (int)this_player()->query_stats("SDC");
        max_sdc  = (int)this_player()->query_stats("max_SDC");
        if(max_sdc > 0) {
            this_player()->set_stats("SDC",     cur_sdc + sdc_gain);
            this_player()->set_stats("max_SDC", max_sdc + sdc_gain);
        }
        msg += sprintf("  HP gain: +%d  (new max: %d)\n",
                       hp_gain, (int)this_player()->query_stats("max_rifts_hp"));
        msg += sprintf("  SDC gain: +%d\n", sdc_gain);
    }

    /* PPE/ISP per level */
    if(ppe_gain > 0) {
        max_ppe = (int)this_player()->query_stats("max_PPE");
        this_player()->set_stats("max_PPE", max_ppe + ppe_gain);
        this_player()->set_stats("PPE", (int)this_player()->query_stats("PPE") + ppe_gain);
        msg += sprintf("  PPE gain: +%d\n", ppe_gain);
    }
    if(isp_gain > 0) {
        max_isp = (int)this_player()->query_stats("max_ISP");
        this_player()->set_stats("max_ISP", max_isp + isp_gain);
        this_player()->set_stats("ISP", (int)this_player()->query_stats("ISP") + isp_gain);
        msg += sprintf("  ISP gain: +%d\n", isp_gain);
    }

    /* APM bonus at H2H milestone levels (3, 6, 9, 12, 15) */
    if(new_level == 3 || new_level == 6 || new_level == 9 ||
       new_level == 12 || new_level == 15) {
        apm_bonus = (int)this_player()->query_property("rifts_apm_bonus");
        this_player()->set_property("rifts_apm_bonus", apm_bonus + 1);
        msg += "  APM gain: +1\n";
    }

    /* One pending skill slot per level */
    pending_slots = (int)this_player()->query_property("pending_skill_slots");
    this_player()->set_property("pending_skill_slots", pending_slots + 1);
    msg += "  Skill slot: +1 (type 'skills' to see pending slots)\n";

    /* +1 life per level (up to level 15). Initialize to 5 on first advance,
       crediting any past levels already gained. */
    lives_initted = (int)this_player()->query_property("rifts_lives_initted");
    if(!lives_initted) {
        cur_lives = 5 + (level > 1 ? level - 1 : 0);
        this_player()->set_property("rifts_lives_initted", 1);
    } else {
        cur_lives = (int)this_player()->query_property("rifts_lives");
    }
    if(new_level <= 15) {
        cur_lives++;
        msg += "  Life gained: +1 (lives remaining: " + cur_lives + ")\n";
    }
    this_player()->set_property("rifts_lives", cur_lives);

    write(msg);

    /* Even levels: grant a stat pick */
    if(new_level == 2 || new_level == 4 || new_level == 6 ||
       new_level == 8 || new_level == 10 || new_level == 12 ||
       new_level == 14) {
        this_player()->setenv("pending_stat_choice", "1");
        write("\nYou may raise one attribute.  Type: advance pick PS/PP/PE/Spd\n");
    }

    say(this_player()->query_cap_name() + " advances to level " + new_level + "!");
    catch(RIFTS_D->check_advance_notify(this_player()));
    return 1;
}

void help() {
    message("help",
        "Syntax: advance\n"
        "        advance pick <stat>\n\n"
        "If you have enough experience for the next level, this command\n"
        "will advance your Rifts character to the next level. You gain:\n"
        "  - HP per OCC dice roll (SDC beings) or +10 MDC (MDC beings)\n"
        "  - A small SDC gain each level (SDC beings only)\n"
        "  - PPE or ISP if your OCC grants them\n"
        "  - +1 life (lives protect you from full death; 0 lives means corpse/XP loss)\n"
        "  - +1 APM at levels 3, 6, 9, 12, and 15\n"
        "  - One pending skill slot each level\n\n"
        "At levels 2, 4, 6, 8, 10, 12, and 14, you may raise one attribute\n"
        "(PS, PP, PE, or Spd) by typing: advance pick <stat>\n\n"
        "Maximum level is 15.\n\n"
        "See also: experience, score, skills",
        this_player());
}
