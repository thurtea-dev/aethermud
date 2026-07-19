/* /cmds/adm/_repairchar.c
   Admin diagnostic/repair for characters broken by chargen bugs.
   Online players only. Does not replace setocc/setrcc/grantrace. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private void dump_state(object target);
private int clear_chargen_flags(object target, int force_done);
private int do_reroll(object target);
private int do_setstat(object target, string stat, int val);
private int do_skills(object target);
private string *attr_names();

private string *attr_names() {
    return ({ "IQ", "ME", "MA", "PS", "PP", "PE", "PB", "Spd" });
}

private void dump_state(object target) {
    string *skills;
    string *anames;
    string step;
    string race;
    string occ;
    string align_name;
    string spells;
    string psi;
    int i;
    int n;
    int val;

    race = (string)target->query_race();
    occ = (string)target->getenv("rifts_occ");
    align_name = (string)target->getenv("rifts_alignment");
    step = (string)target->getenv("creation_step");
    spells = (string)target->getenv("known_spells");
    psi = (string)target->getenv("known_psionics");
    skills = (string *)RIFTS_SKILLS_D->query_player_skills(target);
    if(!race) race = "(none)";
    if(!occ) occ = "(none)";
    if(!align_name) align_name = "(none)";
    if(!step) step = "(unset)";
    if(!spells) spells = "";
    if(!psi) psi = "";
    if(!skills) skills = ({});

    write("=== REPAIRCHAR: " + (string)target->query_cap_name() + " ===\n");
    write(sprintf("  Race: %-20s OCC: %s\n", race, occ));
    write(sprintf("  Alignment: %-15s Level: %d\n",
        align_name, (int)target->query_level()));
    write("  creation_step: " + step + "\n");
    write("  chargen_complete: " +
          ((string)target->getenv("chargen_complete") == "1" ? "1" : "(unset)") +
          "\n");
    write("  stats_rolled: " +
          ((string)target->getenv("stats_rolled") == "1" ? "1" : "(unset)") +
          "  stats_accepted: " +
          ((string)target->getenv("stats_accepted") == "1" ? "1" : "(unset)") +
          "\n");
    write("  awaiting_region: " +
          ((string)target->getenv("awaiting_region") || "(unset)") +
          "  awaiting_alignment: " +
          ((string)target->getenv("awaiting_alignment") || "(unset)") +
          "\n");
    write("  awaiting_occ: " +
          ((string)target->getenv("awaiting_occ") || "(unset)") +
          "  awaiting_elective: " +
          ((string)target->getenv("awaiting_elective_skills") || "(unset)") +
          "  awaiting_secondary: " +
          ((string)target->getenv("awaiting_secondary_skills") || "(unset)") +
          "\n");
    write("  Attributes:\n");
    anames = attr_names();
    for(i = 0, n = sizeof(anames); i < n; i++) {
        val = (int)target->query_stats(anames[i]);
        write(sprintf("    %-4s %d%s\n", anames[i], val,
            (val < 1 ? "  <MISSING/ZERO>" : "")));
    }
    write(sprintf("  HP: %d/%d  SDC: %d/%d  MDC: %d/%d\n",
        (int)target->query_stats("rifts_hp"),
        (int)target->query_stats("max_rifts_hp"),
        (int)target->query_stats("SDC"),
        (int)target->query_stats("max_SDC"),
        (int)target->query_stats("MDC"),
        (int)target->query_stats("max_MDC")));
    write(sprintf("  PPE: %d/%d  ISP: %d/%d\n",
        (int)target->query_stats("PPE"),
        (int)target->query_stats("max_PPE"),
        (int)target->query_stats("ISP"),
        (int)target->query_stats("max_ISP")));
    write("  Skill count: " + sizeof(skills) + "\n");
    write("  known_spells: " + (sizeof(spells) ? spells : "(none)") + "\n");
    write("  known_psionics: " + (sizeof(psi) ? psi : "(none)") + "\n");
    write(sprintf("  Location: %s\n",
        environment(target) ?
        base_name(environment(target)) : "(nowhere)"));
}

private int clear_chargen_flags(object target, int force_done) {
    string race;

    target->remove_env("awaiting_region");
    target->remove_env("awaiting_alignment");
    target->remove_env("awaiting_occ");
    target->remove_env("awaiting_elective_skills");
    target->remove_env("awaiting_secondary_skills");
    target->remove_env("elective_picks_remaining");
    target->remove_env("elective_cats_allowed");
    target->remove_env("secondary_picks_remaining");
    target->remove_env("stats_rolled");
    target->remove_env("stats_accepted");
    target->remove_property("chargen_session_prompted");

    race = (string)target->query_race();
    if(force_done || (race && sizeof(race))) {
        target->setenv("creation_step", "done");
        target->setenv("chargen_complete", "1");
        write("Cleared chargen flags. creation_step=done, chargen_complete=1.\n");
        return 1;
    }
    target->setenv("creation_step", "region");
    target->remove_env("chargen_complete");
    write("No race set. Cleared awaiting flags; creation_step reset to region.\n");
    write("Move them to ROOM_SETTER (or have them reconnect) to resume chargen.\n");
    return 1;
}

private int do_reroll(object target) {
    string race;
    mapping rolls;
    string *kk;
    string *rflags;
    int i;
    int tmp;
    int pe;
    int hp;
    int sdc;
    int mdc;

    race = (string)target->query_race();
    if(!race || !sizeof(race)) {
        write("Target has no race. Set race first (setrcc).\n");
        return 1;
    }
    if(!(int)RIFTS_D->is_rifts_race(race) &&
       !(int)RIFTS_D->is_admin_race(race)) {
        write("Race '" + race + "' is not a Rifts/admin race.\n");
        return 1;
    }
    rolls = (mapping)RIFTS_D->do_rifts_rolls(race);
    kk = keys(rolls);
    tmp = sizeof(kk);
    for(i = 0; i < tmp; i++)
        target->set_stats(kk[i], rolls[kk[i]]);
    pe = (int)rolls["PE"];
    if((int)RIFTS_D->is_mdc_race(race)) {
        mdc = (int)RIFTS_D->init_mdc(race, pe);
        target->set_stats("MDC", mdc);
        target->set_stats("max_MDC", mdc);
        target->set_stats("SDC", pe * 2);
        target->set_stats("max_SDC", pe * 2);
        target->set_stats("is_MDC", 1);
        target->set_property("rifts_mdc_race", 1);
        target->setenv("rifts_mdc_race", "1");
    } else {
        sdc = (int)RIFTS_D->init_sdc(race, pe);
        target->set_stats("SDC", sdc);
        target->set_stats("max_SDC", sdc);
        target->set_stats("MDC", 0);
        target->set_stats("max_MDC", 0);
        target->set_stats("is_MDC", 0);
        target->remove_property("rifts_mdc_race");
        target->remove_env("rifts_mdc_race");
    }
    hp = (int)RIFTS_D->init_hp(race, pe);
    target->set_stats("rifts_hp", hp);
    target->set_stats("max_rifts_hp", hp);
    rflags = (string *)RIFTS_D->query_race_flags(race);
    if(rflags)
        target->setenv("rifts_flags", implode(rflags, ","));
    write("Re-rolled racial attributes and body pools for " + race + ".\n");
    dump_state(target);
    return 1;
}

private int do_setstat(object target, string stat, int val) {
    string *ok;
    string up;

    if(!stat || !sizeof(stat)) {
        write("Usage: repairchar <player> setstat <IQ|ME|MA|PS|PP|PE|PB|Spd> <value>\n");
        return 1;
    }
    up = stat;
    if(lower_case(stat) == "spd") up = "Spd";
    else up = upper_case(stat);
    ok = attr_names();
    if(member_array(up, ok) == -1) {
        write("Invalid attribute. Use: IQ ME MA PS PP PE PB Spd\n");
        return 1;
    }
    if(val < 1) val = 1;
    if(val > 60) val = 60;
    target->set_stats(up, val);
    if(up == "IQ") target->set_stats("intelligence", val);
    if(up == "ME") target->set_stats("wisdom", val);
    if(up == "PS") target->set_stats("strength", val);
    if(up == "PE") target->set_stats("constitution", val);
    if(up == "PP") target->set_stats("dexterity", val);
    if(up == "PB") target->set_stats("charisma", val);
    write("Set " + up + " to " + val + " on " +
          (string)target->query_cap_name() + ".\n");
    return 1;
}

private int do_skills(object target) {
    string occ;
    string *before;
    string *after;
    int n_before;
    int n_after;

    occ = (string)target->getenv("rifts_occ");
    if(!occ || !sizeof(occ)) occ = "none";
    before = (string *)RIFTS_SKILLS_D->query_player_skills(target);
    if(!before) before = ({});
    n_before = sizeof(before);
    catch(RIFTS_START_D->grant_occ_skills(target));
    after = (string *)RIFTS_SKILLS_D->query_player_skills(target);
    if(!after) after = ({});
    n_after = sizeof(after);
    write("Re-granted OCC skill package for OCC '" + occ + "'.\n");
    write("  Skills before: " + n_before + "  after: " + n_after +
          "  added: " + (n_after - n_before) + "\n");
    return 1;
}

int cmd_repairchar(string str) {
    string pname;
    string action;
    string rest;
    string astat;
    int aval;
    object target;

    if(!creatorp(this_player())) {
        notify_fail("Admin access required.\n");
        return 0;
    }
    if(!str || !strlen(str)) {
        write("Usage: repairchar <player> [dump|clear|finish|reroll|skills|"
              "setstat <STAT> <n>]\n");
        return 1;
    }
    if(sscanf(str, "%s %s", pname, rest) != 2) {
        pname = lower_case(str);
        rest = "dump";
    } else {
        pname = lower_case(pname);
        rest = lower_case(rest);
    }
    target = find_player(pname);
    if(!target || !objectp(target)) {
        write("Player '" + capitalize(pname) + "' is not online.\n");
        return 1;
    }
    if(sscanf(rest, "%s %s %d", action, astat, aval) == 3 &&
       action == "setstat") {
        do_setstat(target, astat, aval);
        catch(target->save_player(pname));
        log_file("adm/repairchar",
            ctime(time()) + " " + (string)this_player()->query_name() +
            " setstat " + astat + "=" + aval + " on " + pname + "\n");
        return 1;
    }
    action = rest;
    if(action == "dump" || action == "show" || action == "status") {
        dump_state(target);
        return 1;
    }
    if(action == "clear") {
        clear_chargen_flags(target, 0);
        catch(target->save_player(pname));
        log_file("adm/repairchar",
            ctime(time()) + " " + (string)this_player()->query_name() +
            " clear on " + pname + "\n");
        tell_object(target,
            "An administrator cleared stuck character-creation flags.\n");
        return 1;
    }
    if(action == "finish") {
        clear_chargen_flags(target, 1);
        catch(target->save_player(pname));
        log_file("adm/repairchar",
            ctime(time()) + " " + (string)this_player()->query_name() +
            " finish on " + pname + "\n");
        tell_object(target,
            "An administrator marked your character creation as complete.\n");
        return 1;
    }
    if(action == "reroll") {
        do_reroll(target);
        catch(target->save_player(pname));
        log_file("adm/repairchar",
            ctime(time()) + " " + (string)this_player()->query_name() +
            " reroll on " + pname + "\n");
        tell_object(target,
            "An administrator re-rolled your attributes.\n");
        return 1;
    }
    if(action == "skills") {
        do_skills(target);
        catch(target->save_player(pname));
        log_file("adm/repairchar",
            ctime(time()) + " " + (string)this_player()->query_name() +
            " skills on " + pname + "\n");
        tell_object(target,
            "An administrator re-granted your OCC skills.\n");
        return 1;
    }
    write("Unknown action '" + action + "'.\n");
    write("Usage: repairchar <player> [dump|clear|finish|reroll|skills|"
          "setstat <STAT> <n>]\n");
    return 1;
}

void help() {
    write(
        "Syntax:\n"
        "  repairchar <player>              dump chargen/stat state\n"
        "  repairchar <player> dump         same\n"
        "  repairchar <player> clear        clear stuck chargen flags\n"
        "                                   (done if race set, else region)\n"
        "  repairchar <player> finish       force creation_step=done\n"
        "  repairchar <player> reroll       re-roll racial attributes/pools\n"
        "  repairchar <player> skills       re-grant OCC skill package\n"
        "  repairchar <player> setstat <A> <n>\n"
        "                                   set one Palladium attribute\n\n"
        "Online players only. Admin (creatorp) only.\n"
        "For full OCC/race rebuilds use setocc / setrcc / grantrace.\n"
    );
}
