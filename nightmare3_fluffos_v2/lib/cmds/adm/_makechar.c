/* /cmds/adm/_makechar.c
   Build a fully-specified test/staff character without interactive chargen.
   Reuses RIFTS_D rolls/pools and RIFTS_START_D packages (same as chargen). */

#include <std.h>
#include <daemons.h>
#include <rooms.h>

inherit DAEMON;

private string resolve_zone(string zone);
private int apply_alignment(object target, string al_name);
private void apply_race_full(object target, string race);
private void apply_occ_meta(object target, string occ);
private void append_list_env(object target, string envkey, string *items);
private void grant_extra_skills(object target, string *skills);
private mapping parse_kv(string str);

/* Zone shortcuts match chargen landing rooms in setter finish_creation(). */
private string resolve_zone(string zone) {
    string z;
    string dest;

    if(!zone || !sizeof(zone)) return 0;
    z = lower_case(zone);
    if(z[0] == '/') {
        if(file_size(z + ".c") > 0) return z;
        return 0;
    }
    if(z == "americas" || z == "praxis")
        dest = "/domains/ChiTown/areas/chitown_start";
    else if(z == "atlantis" || z == "splynn")
        dest = "/domains/Praxis/areas/splynn/splynn_entry";
    else if(z == "europe" || z == "camelot" || z == "newcamelot")
        dest = "/domains/NewCamelot/areas/newcamelot_start";
    else if(z == "chitown" || z == "chi-town")
        dest = "/domains/ChiTown/areas/chitown_gate";
    else if(z == "lazlo")
        dest = "/domains/Lazlo/lazlo_welcome";
    else if(z == "tolkeen")
        dest = "/domains/tolkeen/areas/tolkeen_gate";
    else
        return 0;
    if(file_size(dest + ".c") <= 0) return 0;
    return dest;
}

private int apply_alignment(object target, string al_name) {
    string al;
    int al_val;
    string pretty;

    al = lower_case(al_name);
    if(al == "principled")        { pretty = "Principled";   al_val = 900; }
    else if(al == "scrupulous")   { pretty = "Scrupulous";   al_val = 600; }
    else if(al == "unprincipled") { pretty = "Unprincipled"; al_val = 200; }
    else if(al == "anarchist")    { pretty = "Anarchist";    al_val = -100; }
    else if(al == "miscreant")    { pretty = "Miscreant";    al_val = -500; }
    else if(al == "aberrant")     { pretty = "Aberrant";     al_val = -900; }
    else if(al == "diabolic")     { pretty = "Diabolic";     al_val = -1200; }
    else return 0;
    target->set_alignment(al_val);
    target->setenv("rifts_alignment", pretty);
    return 1;
}

/* Same rolls/pools path as cmds/adm/_setrcc.c (not setter.c). */
private void apply_race_full(object target, string race) {
    mapping rolls;
    string *kk;
    string *rflags;
    int i;
    int tmp;
    int pe;
    int hp;
    int sdc;
    int mdc;

    target->set_race(race);
    target->new_body();
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
}

private void apply_occ_meta(object target, string occ) {
    mapping occ_data;
    string *occ_flags;

    occ_data = (mapping)OCC_D->query_occ(occ);
    if(!occ_data) return;
    target->setenv("rifts_occ", occ);
    target->set_class(occ);
    occ_flags = (string *)occ_data["flags"];
    if(!occ_flags) occ_flags = ({});
    target->setenv("rifts_occ_flags", implode(occ_flags, ","));
}

private void append_list_env(object target, string envkey, string *items) {
    string existing;
    string *cur;
    string *add;
    string *out;
    int i;
    int n;
    string one;

    if(!items || !sizeof(items)) return;
    existing = (string)target->getenv(envkey);
    if(existing && sizeof(existing))
        cur = explode(existing, ";");
    else
        cur = ({});
    add = ({});
    for(i = 0, n = sizeof(items); i < n; i++) {
        one = lower_case(replace_string(items[i], "_", " "));
        while(sizeof(one) && one[0] == ' ') one = one[1..];
        if(!sizeof(one)) continue;
        if(member_array(one, cur) == -1 && member_array(one, add) == -1)
            add += ({ one });
    }
    out = cur + add;
    if(sizeof(out))
        target->setenv(envkey, implode(out, ";"));
}

private void grant_extra_skills(object target, string *skills) {
    int i;
    int n;
    string sk;

    if(!skills) return;
    for(i = 0, n = sizeof(skills); i < n; i++) {
        sk = lower_case(replace_string(skills[i], "_", " "));
        while(sizeof(sk) && sk[0] == ' ') sk = sk[1..];
        if(!sizeof(sk)) continue;
        catch(RIFTS_SKILLS_D->grant_skill(target, sk, 0));
    }
}

/* Parse trailing key=value tokens. Values may use ; for lists. */
private mapping parse_kv(string str) {
    mapping out;
    string *parts;
    string piece;
    string key;
    string val;
    int i;
    int n;

    out = ([]);
    if(!str || !sizeof(str)) return out;
    parts = explode(str, " ");
    for(i = 0, n = sizeof(parts); i < n; i++) {
        piece = parts[i];
        if(!piece || !sizeof(piece)) continue;
        if(sscanf(piece, "%s=%s", key, val) != 2) continue;
        key = lower_case(key);
        out[key] = val;
    }
    return out;
}

int cmd_makechar(string str) {
    string pname;
    string race;
    string occ;
    string align_name;
    string zone_arg;
    string rest;
    string gear;
    string dest;
    string *extra_skills;
    string *extra_spells;
    string *extra_psi;
    mapping kv;
    mapping occ_data;
    object target;
    int moved;

    if(!creatorp(this_player())) {
        notify_fail("Admin access required.\n");
        return 0;
    }
    if(!str || !strlen(str)) {
        write(
            "Usage: makechar <player> <race> <occ|none> <alignment> "
            "[zone=<z>] [gear=default|none] "
            "[skills=a;b] [spells=a;b] [psi=a;b]\n");
        return 1;
    }
    /* Required: player race occ alignment, then optional key=value */
    if(sscanf(str, "%s %s %s %s %s", pname, race, occ, align_name, rest) != 5) {
        if(sscanf(str, "%s %s %s %s", pname, race, occ, align_name) != 4) {
            write(
                "Usage: makechar <player> <race> <occ|none> <alignment> "
                "[zone=<z>] [gear=default|none] "
                "[skills=a;b] [spells=a;b] [psi=a;b]\n");
            return 1;
        }
        rest = "";
    }
    pname = lower_case(pname);
    race = lower_case(replace_string(race, "_", " "));
    occ = lower_case(replace_string(occ, "_", " "));
    align_name = lower_case(align_name);
    target = find_player(pname);
    if(!target || !objectp(target)) {
        write("Player '" + capitalize(pname) + "' is not online.\n");
        return 1;
    }
    if(!(int)RIFTS_D->is_rifts_race(race) &&
       !(int)RIFTS_D->is_admin_race(race)) {
        write("'" + race + "' is not a valid Rifts/admin race.\n");
        return 1;
    }
    if(occ != "none") {
        occ_data = (mapping)OCC_D->query_occ(occ);
        if(!occ_data) {
            write("'" + occ + "' is not a valid OCC. Use 'none' for RCC-only.\n");
            return 1;
        }
    }
    if(!apply_alignment(target, align_name)) {
        write("Invalid alignment. Use: principled, scrupulous, unprincipled,\n"
              "anarchist, miscreant, aberrant, diabolic.\n");
        return 1;
    }

    kv = parse_kv(rest);
    zone_arg = kv["zone"];
    gear = kv["gear"];
    if(!gear || !sizeof(gear)) gear = "default";
    gear = lower_case(gear);
    if(gear != "default" && gear != "none") {
        write("gear= must be 'default' or 'none'.\n");
        return 1;
    }
    if(zone_arg && sizeof(zone_arg)) {
        dest = resolve_zone(zone_arg);
        if(!dest) {
            write("Unknown or missing zone/room: " + zone_arg + "\n");
            write("Try: americas, atlantis, europe, chitown, lazlo, tolkeen,\n"
                  "or a full /domains/... path.\n");
            return 1;
        }
    } else {
        dest = 0;
    }
    if(kv["skills"])
        extra_skills = explode(replace_string(kv["skills"], ",", ";"), ";");
    else
        extra_skills = ({});
    if(kv["spells"])
        extra_spells = explode(replace_string(kv["spells"], ",", ";"), ";");
    else
        extra_spells = ({});
    if(kv["psi"] || kv["psionics"]) {
        if(kv["psi"])
            extra_psi = explode(replace_string(kv["psi"], ",", ";"), ";");
        else
            extra_psi = explode(replace_string(kv["psionics"], ",", ";"), ";");
    } else
        extra_psi = ({});

    /* Clear stuck chargen state so they are a finished character. */
    target->remove_env("awaiting_region");
    target->remove_env("awaiting_alignment");
    target->remove_env("awaiting_occ");
    target->remove_env("awaiting_elective_skills");
    target->remove_env("awaiting_secondary_skills");
    target->remove_env("stats_rolled");
    target->remove_env("stats_accepted");
    target->remove_property("chargen_session_prompted");

    apply_race_full(target, race);
    if(occ == "none") {
        target->setenv("rifts_occ", "none");
        target->setenv("rifts_occ_flags", "magic,psychic,borg,cybernetic");
        target->set_class("child");
    } else
        apply_occ_meta(target, occ);

    if(gear == "default")
        catch(RIFTS_START_D->grant_starting_package(target));
    else {
        /* No public API grants OCC spells without also giving equipment.
           gear=none: OCC skills + race spells/psi only; use spells= for
           OCC spell lists, or gear=default for the full package. */
        catch(RIFTS_START_D->grant_occ_skills(target));
        catch(RIFTS_START_D->grant_race_package(target));
        write("gear=none: OCC skills + race spells/psi only (no equipment).\n"
              "OCC spell packages require gear=default, or pass spells=.\n");
    }

    grant_extra_skills(target, extra_skills);
    append_list_env(target, "known_spells", extra_spells);
    append_list_env(target, "known_psionics", extra_psi);

    catch(LANGUAGE_D->grant_native_languages(target));
    target->setenv("creation_step", "done");
    target->setenv("chargen_complete", "1");
    if((int)RIFTS_D->is_rifts_race(race))
        target->setenv("TITLE", "");

    moved = 0;
    if(dest) {
        target->set_primary_start(dest);
        target->setenv("start", dest);
        if(zone_arg && zone_arg[0] != '/') {
            if(lower_case(zone_arg) == "praxis")
                target->setenv("rifts_region", "americas");
            else if(lower_case(zone_arg) == "splynn")
                target->setenv("rifts_region", "atlantis");
            else if(lower_case(zone_arg) == "camelot" ||
                    lower_case(zone_arg) == "newcamelot")
                target->setenv("rifts_region", "europe");
            else
                target->setenv("rifts_region", lower_case(zone_arg));
        }
        catch(target->move_player(dest));
        moved = 1;
    }

    catch(target->save_player(pname));
    write("=== MAKECHAR complete: " + (string)target->query_cap_name() +
          " ===\n");
    write("  Race: " + race + "  OCC: " + occ + "  Align: " +
          (string)target->getenv("rifts_alignment") + "\n");
    write("  Gear: " + gear +
          (gear == "default" ?
           " (OCC/RCC starting package via grant_starting_package)" :
           " (no equipment)") + "\n");
    write("  Extra skills: " + sizeof(extra_skills) +
          "  spells: " + sizeof(extra_spells) +
          "  psi: " + sizeof(extra_psi) + "\n");
    if(moved)
        write("  Moved to: " + dest + "\n");
    else
        write("  Zone: unchanged (omit zone= to leave in place).\n");
    tell_object(target,
        "An administrator rebuilt your character via makechar.\n");
    log_file("adm/makechar",
        ctime(time()) + " " + (string)this_player()->query_name() +
        " makechar " + pname + " race=" + race + " occ=" + occ +
        " align=" + align_name + " gear=" + gear +
        (dest ? (" dest=" + dest) : "") + "\n");
    return 1;
}

void help() {
    write(
        "Syntax:\n"
        "  makechar <player> <race> <occ|none> <alignment>\n"
        "           [zone=<name|/path>] [gear=default|none]\n"
        "           [skills=a;b] [spells=a;b] [psi=a;b]\n\n"
        "Required: player (online), race, OCC (or none), alignment.\n"
        "Optional:\n"
        "  zone=americas|atlantis|europe|chitown|lazlo|tolkeen|/domains/...\n"
        "       Default: leave player where they are.\n"
        "  gear=default  OCC/RCC starting package (equipment+skills+spells)\n"
        "                via RIFTS_START_D->grant_starting_package (default)\n"
        "  gear=none     No equipment; OCC skills + race spells/psi only.\n"
        "  skills=       Extra skills (; or , separated; spaces as _)\n"
        "  spells=       Extra known_spells entries\n"
        "  psi=          Extra known_psionics entries\n\n"
        "Example:\n"
        "  makechar bob human mystic unprincipled zone=atlantis\n"
        "  makechar bob titan none anarchist zone=europe gear=none "
        "skills=tracking;prowl\n\n"
        "Admin (creatorp) only. Online players only.\n"
    );
}
