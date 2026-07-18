// /domains/adm/wiz_tools/staff_of_demotion.c
// Staff of Demotion -- Admin tool for AetherMUD
// Menu-driven admin functions for archon-level staff.

#include <std.h>
#include <security.h>
#include <daemons.h>

inherit OBJECT;

static private string pending_action;
static private string pending_sub;
static private object pending_promote_ob;
static private string pending_promote_pos;

void do_player_action();
private void apply_promote_position();
private void demote_restore(object ob);
private void strip_godling_package(object ob);
private void demote_force_quit(object ob);

void create() {
    ::create();
    set_name("staff of demotion");
    set_id( ({ "staff", "staff of demotion", "demotion staff" }) );
    set_short("a staff of demotion");
    set_long(
        "A slender obsidian staff crackling with administrative authority.\n"
        "It hums with the power to shape the lives of those who dwell in\n"
        "this world. For Admin use only. Type: demote or promote");
    set_mass(500);
    set_value(0);
    set_property("no_drop", 1);
    set_property("no_give", 1);
    set_invis(1);
}

mixed *query_auto_load() {
    return ({ base_name(this_object()), ({}) });
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_use_staff", "demote");
    add_action("cmd_use_staff", "promote");
}

private void show_menu() {
    write("\n=== Staff of Demotion ===");
    write(" 1. Promote player to wizard rank");
    write(" 2. Demote wizard to mortal");
    write(" 3. Set player level");
    write(" 4. Set player race (Rifts)");
    write(" 5. Set player OCC");
    write(" 6. Set player stats (IQ/ME/MA/PS/PP/PE/PB/Spd)");
    write(" 7. Grant/remove title");
    write(" 8. Teleport to player");
    write(" 9. Summon player");
    write("10. Force-quit player (boot)");
    write("11. Purge player file");
    write("12. Shutdown MUD (with reason)");
    write("13. Reload daemon");
    write(" 0. Exit");
    write("Choice: ");
}

int cmd_use_staff(string str) {
    if(!admin_wizp(this_player())) {
        write("The staff refuses to respond to you.\n");
        return 1;
    }
    pending_action = "";
    pending_sub = "";
    show_menu();
    input_to("handle_choice");
    return 1;
}

void handle_choice(string str) {
    int choice;

    if(!str || !sizeof(str)) { write("Invalid choice.\n"); show_menu(); input_to("handle_choice"); return; }
    choice = to_int(str);
    switch(choice) {
    case 0:
        write("Staff of Demotion closed.");
        return;
    case 1: pending_action = "promote"; write("Player name: "); input_to("get_target"); return;
    case 2: pending_action = "demote"; write("Player name: "); input_to("get_target"); return;
    case 3: pending_action = "setlevel"; write("Player name: "); input_to("get_target"); return;
    case 4: pending_action = "setrace"; write("Player name: "); input_to("get_target"); return;
    case 5: pending_action = "setocc"; write("Player name: "); input_to("get_target"); return;
    case 6: pending_action = "setstats"; write("Player name: "); input_to("get_target"); return;
    case 7: pending_action = "title"; write("Player name: "); input_to("get_target"); return;
    case 8: pending_action = "goto"; write("Player name: "); input_to("get_target"); return;
    case 9: pending_action = "summon"; write("Player name: "); input_to("get_target"); return;
    case 10: pending_action = "boot"; write("Player name: "); input_to("get_target"); return;
    case 11: pending_action = "purge"; write("Player name: "); input_to("get_target"); return;
    case 12: pending_action = "shutdown"; write("Shutdown reason: "); input_to("do_shutdown"); return;
    case 13: pending_action = "reload"; write("Daemon (rifts/occ/rifts_skills/rifts_combat): "); input_to("do_reload"); return;
    default:
        write("Invalid option.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
}

void get_target(string name) {
    if(!name || !sizeof(name)) { write("Cancelled.\n"); return; }
    pending_sub = lower_case(name);
    switch(pending_action) {
    case "promote":
        write("New position (arch/domain_wiz/creator/rp_wiz): ");
        input_to("do_promote");
        break;
    case "demote":
        do_player_action();
        break;
    case "setlevel":
        write("New level (1-25): ");
        input_to("do_setlevel");
        break;
    case "setrace":
        write("New race: ");
        input_to("do_setrace");
        break;
    case "setocc":
        write("New OCC (or 'none'): ");
        input_to("do_setocc");
        break;
    case "setstats":
        write("Stats as: IQ ME MA PS PP PE PB Spd (8 numbers space-separated): ");
        input_to("do_setstats");
        break;
    case "title":
        write("New title (use $N for name, or 'remove'): ");
        input_to("do_title");
        break;
    case "goto":
    case "summon":
    case "boot":
    case "purge":
        do_player_action();
        break;
    default:
        write("Unknown action.\n");
    }
}

void do_player_action() {
    object ob;
    ob = find_player(pending_sub);
    switch(pending_action) {
    case "demote":
        if(ob) {
            if(!creatorp(ob)) {
                write(capitalize(pending_sub) + " is not a wizard.");
                break;
            }
            demote_restore(ob);
            write("Demoted " + capitalize(pending_sub) +
                " to mortal and restored their pre-wizard identity.");
        } else if(user_exists(pending_sub)) {
            USERS_D->xmote(pending_sub, "player", 0);
            write("Stripped wizard position from offline player " +
                capitalize(pending_sub) + ". Race and stat restoration " +
                "needs them online: rerun demote once they log in.");
        } else {
            write("No such player: " + capitalize(pending_sub) + ".");
        }
        break;
    case "goto":
        if(!ob) { write("Player not found online.\n"); return; }
        this_player()->move_player(file_name(environment(ob)));
        write("Teleported to " + capitalize(pending_sub) + ".");
        break;
    case "summon":
        if(!ob) { write("Player not found online.\n"); return; }
        ob->move_player(file_name(environment(this_player())));
        write(capitalize(pending_sub) + " summoned.");
        tell_object(ob, this_player()->query_cap_name() + " summons you!");
        break;
    case "boot":
        if(!ob) { write("Player not found online.\n"); return; }
        tell_object(ob, "An admin has booted you from " + mud_name() + ".");
        ob->force_me("quit");
        write(capitalize(pending_sub) + " has been booted.");
        break;
    case "purge":
        if(ob) { ob->remove(); destruct(ob); }
        write("WARNING: purge must be done via 'rid' command for safety.");
        write("Use: rid " + pending_sub);
        break;
    }
}

private void apply_promote_position() {
    if(pending_promote_ob)
        pending_promote_ob->set_position(pending_promote_pos);
}

/* Strip Godling spells/psionics/skills granted by makewiz, then rebuild
   mortal race/OCC packages from the restored identity. */
private void strip_godling_package(object ob) {
    string *godling_skills;
    string *have;
    string val;
    int i;

    if(!ob) return;

    /* Restore pre-promotion spell/psi lists when snapshotted; otherwise
       clear the all-spells/all-psionics Godling dump. */
    val = (string)ob->getenv("premote_spells");
    if(val && sizeof(val)) ob->setenv("known_spells", val);
    else ob->remove_env("known_spells");
    val = (string)ob->getenv("premote_psionics");
    if(val && sizeof(val)) ob->setenv("known_psionics", val);
    else ob->remove_env("known_psionics");

    godling_skills = ({
        "wp energy pistol", "wp energy rifle", "wp heavy weapons",
        "wp knife", "wp sword", "wp vibroblade", "wp blunt", "wp chain",
        "wp paired weapons", "wp rifle", "wp archery",
        "pilot automobile", "pilot hovercraft", "pilot hovercycle",
        "pilot power armor", "pilot robot", "pilot glitter boy",
        "pilot jet pack", "pilot boat", "pilot submersible",
        "radio basic", "computer operation", "computer programming",
        "basic electronics", "advanced electronics",
        "read sensory equipment", "weapon systems",
        "electronic countermeasures", "mechanical engineer",
        "robot mechanics", "weapons engineer", "electrical engineer",
        "first aid", "paramedic", "medical doctor", "holistic medicine",
        "biology", "chemistry", "pathology", "wilderness survival",
        "land navigation", "tracking", "camouflage", "hunting",
        "fishing", "swimming", "climbing", "running", "navigation",
        "detect ambush", "detect concealment", "intelligence",
        "surveillance", "pick locks", "pick pockets", "safe-cracking",
        "forgery", "disguise", "impersonation", "streetwise", "gambling",
        "concealment", "prowl", "interrogation", "tactics",
        "military etiquette", "demolitions", "boxing", "gymnastics",
        "acrobatics", "body building", "meditation", "athletics",
        "lore magic", "lore demons and monsters",
        "lore rifts and dimensions", "lore psychic", "lore spirits",
        "research", "salvage", "horsemanship", "seduction",
        "identify plants", "barter", "appraise goods", "black market",
        "fly"
    });
    /* Godling package grants these at 98. Only strip that tier so a
       mortal who already had the skill at a normal starting % keeps it. */
    have = (string *)ob->query_all_skills();
    if(have) {
        for(i = 0; i < sizeof(godling_skills); i++) {
            if(member_array(godling_skills[i], have) == -1) continue;
            if((int)ob->query_base_skill(godling_skills[i]) < 98) continue;
            catch(ob->delete_skill(godling_skills[i]));
        }
    }

    /* Rebuild mortal packages for the restored race/OCC. */
    catch(RIFTS_START_D->grant_race_package(ob));
    catch(RIFTS_START_D->grant_occ_skills(ob));
}

/* Drop the network connection so the player must reconnect as a mortal. */
private void demote_force_quit(object ob) {
    object sink;

    if(!ob || !objectp(ob)) return;
    tell_object(ob,
        "Your wizard status has been revoked. Please reconnect.\n");
    catch(ob->save_player((string)ob->query_name()));
    sink = new(OBJECT);
    if(sink) {
        catch(exec(sink, ob));
        if(objectp(sink)) destruct(sink);
    }
    if(objectp(ob)) catch(ob->remove());
}

/* Full mortal restoration for a demoted wizard. Reads the premote_*
   env snapshot written by makewiz (see _makewiz.c for the fixed
   premote_stats order). Wizards promoted before the snapshot existed
   fall back to a fresh reroll for the restored race. */
private void demote_restore(object ob) {
    string race, cls, val, snap, start_room;
    string *parts;
    string *stat_names;
    string *rflags;
    mapping rolls;
    string *keys_arr;
    int i, pe, hp, sdc, mdc, lev;

    /* Position first: this also removes wiz_role, has_wiz_tools, and
       every wiz_tools object in inventory via WIZTOOLS_D. */
    ob->set_position("player");

    race = (string)ob->getenv("premote_race");
    if(!race || !sizeof(race)) race = "human";
    cls = (string)ob->getenv("premote_class");
    if(!cls || !sizeof(cls)) cls = race;
    ob->set_race(race);
    ob->set_class(cls);

    /* Restore pre-wizard level so setrole-promoted level-20 staff do not
       land as high mortals after demotion. */
    val = (string)ob->getenv("premote_level");
    if(val && sizeof(val)) {
        lev = to_int(val);
        if(lev < 1) lev = 1;
        if(lev > 25) lev = 25;
        ob->set_level(lev);
    } else if((int)ob->query_level() > 19) {
        ob->set_level(19);
    }

    snap = (string)ob->getenv("premote_stats");
    stat_names = ({ "IQ", "ME", "MA", "PS", "PP", "PE", "PB", "Spd",
        "MDC", "max_MDC", "SDC", "max_SDC", "is_MDC", "rifts_hp",
        "max_rifts_hp", "PPE", "max_PPE", "ISP", "max_ISP" });
    if(snap && sizeof(parts = explode(snap, " ")) >= 19) {
        for(i = 0; i < 19; i++)
            ob->set_stats(stat_names[i], to_int(parts[i]));
        /* Race body setup after stats so new_body() sees mortal attributes.
           Re-apply the snapshot afterward in case new_body tweaked pools. */
        catch(ob->new_body());
        for(i = 0; i < 19; i++)
            ob->set_stats(stat_names[i], to_int(parts[i]));
    } else {
        rolls = (mapping)RIFTS_D->do_rifts_rolls(race);
        if(rolls) {
            keys_arr = keys(rolls);
            for(i = 0; i < sizeof(keys_arr); i++)
                ob->set_stats(keys_arr[i], rolls[keys_arr[i]]);
        }
        pe = (int)ob->query_stats("PE");
        hp = (int)RIFTS_D->init_hp(race, pe);
        ob->set_stats("rifts_hp", hp);
        ob->set_stats("max_rifts_hp", hp);
        if((int)RIFTS_D->is_mdc_race(race)) {
            mdc = (int)RIFTS_D->init_mdc(race, pe);
            ob->set_stats("MDC", mdc);
            ob->set_stats("max_MDC", mdc);
            sdc = pe * 2;
            ob->set_stats("SDC", sdc);
            ob->set_stats("max_SDC", sdc);
            ob->set_stats("is_MDC", 1);
        } else {
            sdc = (int)RIFTS_D->init_sdc(race, pe);
            ob->set_stats("SDC", sdc);
            ob->set_stats("max_SDC", sdc);
            ob->set_stats("MDC", 0);
            ob->set_stats("max_MDC", 0);
            ob->set_stats("is_MDC", 0);
        }
        catch(ob->new_body());
    }

    if((int)RIFTS_D->is_mdc_race(race)) {
        ob->set_property("rifts_mdc_race", 1);
        ob->setenv("rifts_mdc_race", "1");
    } else {
        ob->remove_property("rifts_mdc_race");
        ob->remove_env("rifts_mdc_race");
    }

    val = (string)ob->getenv("premote_occ");
    ob->setenv("rifts_occ", (val && sizeof(val)) ? val : "none");
    val = (string)ob->getenv("premote_occ_flags");
    if(val && sizeof(val)) ob->setenv("rifts_occ_flags", val);
    else ob->remove_env("rifts_occ_flags");
    val = (string)ob->getenv("premote_flags");
    if(val && sizeof(val)) {
        ob->setenv("rifts_flags", val);
    } else {
        rflags = (string *)RIFTS_D->query_race_flags(race);
        if(rflags && sizeof(rflags))
            ob->setenv("rifts_flags", implode(rflags, ","));
        else ob->remove_env("rifts_flags");
    }
    val = (string)ob->getenv("premote_alignment");
    if(val && sizeof(val)) ob->setenv("rifts_alignment", val);
    val = (string)ob->getenv("premote_align");
    if(val && sizeof(val)) ob->set_alignment(to_int(val));
    else ob->set_alignment(0);
    val = (string)ob->getenv("premote_language");
    ob->setenv("active_language", (val && sizeof(val)) ? val : "American");

    ob->remove_env("speak_all_languages");
    ob->remove_env("always_known");
    ob->remove_env("TITLE");
    ob->remove_env("whotitle");

    start_room = (string)ob->getenv("premote_start");
    if(!start_room || !sizeof(start_room) ||
       file_size(start_room + ".c") <= 0)
        start_room = "/domains/Praxis/rifts_welcome";
    ob->set_primary_start(start_room);

    strip_godling_package(ob);

    ob->remove_env("premote_race");
    ob->remove_env("premote_class");
    ob->remove_env("premote_occ");
    ob->remove_env("premote_occ_flags");
    ob->remove_env("premote_flags");
    ob->remove_env("premote_alignment");
    ob->remove_env("premote_align");
    ob->remove_env("premote_language");
    ob->remove_env("premote_start");
    ob->remove_env("premote_level");
    ob->remove_env("premote_stats");
    ob->remove_env("premote_spells");
    ob->remove_env("premote_psionics");

    catch(log_file("adm/staff_promotions",
        (string)this_player()->query_name() + " demoted " +
        (string)ob->query_name() + " to mortal (" + race + "): " +
        ctime(time()) + "\n"));
    demote_force_quit(ob);
}

void do_promote(string pos) {
    object ob;
    string logline;

    if(!pos || !sizeof(pos)) { write("Cancelled.\n"); return; }
    pos = lower_case(pos);
    if(pos != "creator" && pos != "arch" && pos != "rp_wiz" && pos != "domain_wiz") {
        write("Position must be 'arch', 'domain_wiz', 'creator', or 'rp_wiz'.\n");
        return;
    }
    ob = find_player(pending_sub);
    if(ob) {
        ob->setenv("creation_step", "done");
        ob->remove_env("awaiting_occ");
        ob->remove_env("awaiting_alignment");
        ob->remove_env("awaiting_region");
        ob->remove_env("awaiting_elective_skills");
        ob->remove_env("stats_rolled");
        pending_promote_ob = ob;
        pending_promote_pos = pos;
        unguarded((: apply_promote_position :));
        pending_promote_ob = 0;
        pending_promote_pos = 0;
        if((int)ob->query_level() < 20)
            ob->set_level(20);
        ob->move_to_workroom();
        if(environment(ob))
            catch(ob->set_primary_start(base_name(environment(ob))));
        ob->save_player((string)ob->query_name());
    } else
        USERS_D->xmote(pending_sub, pos, 0);
    write(capitalize(pending_sub) + " promoted to " + pos + ".");
    logline = this_player()->query_name() + " promoted " + pending_sub +
        " to " + pos + ": " + ctime(time()) + "\n";
    catch(log_file("adm/staff_promotions", logline));
}

void do_setlevel(string val) {
    object ob;
    int lev;
    if(!val || !to_int(val)) { write("Cancelled.\n"); return; }
    lev = to_int(val);
    if(lev < 1) lev = 1;
    if(lev > 25) lev = 25;
    ob = find_player(pending_sub);
    if(ob) ob->set_level(lev);
    else USERS_D->xmote(pending_sub, 0, lev);
    write(capitalize(pending_sub) + " set to level " + lev + ".");
}

void do_setrace(string race) {
    object ob;
    if(!race || !sizeof(race)) { write("Cancelled.\n"); return; }
    ob = find_player(pending_sub);
    if(!ob) { write("Player not online.\n"); return; }
    ob->set_race(lower_case(race));
    write(capitalize(pending_sub) + "'s race set to: " + race + ".");
}

void do_setocc(string occ) {
    object ob;
    if(!occ || !sizeof(occ)) { write("Cancelled.\n"); return; }
    ob = find_player(pending_sub);
    if(!ob) { write("Player not online.\n"); return; }
    ob->setenv("rifts_occ", lower_case(occ));
    write(capitalize(pending_sub) + "'s OCC set to: " + occ + ".");
}

void do_setstats(string vals) {
    object ob;
    string *parts;
    string *stat_names;
    int i;

    if(!vals || !sizeof(vals)) { write("Cancelled.\n"); return; }
    ob = find_player(pending_sub);
    if(!ob) { write("Player not online.\n"); return; }
    parts = explode(vals, " ");
    stat_names = ({ "IQ", "ME", "MA", "PS", "PP", "PE", "PB", "Spd" });
    if(sizeof(parts) < 8) { write("Need 8 values.\n"); return; }
    for(i = 0; i < 8; i++) ob->set_stats(stat_names[i], to_int(parts[i]));
    write(capitalize(pending_sub) + "'s stats updated.");
}

void do_title(string title) {
    object ob;
    if(!title || !sizeof(title)) { write("Cancelled.\n"); return; }
    ob = find_player(pending_sub);
    if(!ob) { write("Player not online.\n"); return; }
    if(lower_case(title) == "remove") title = capitalize(pending_sub);
    ob->setenv("TITLE", title);
    write("Title set: " + title);
}

void do_shutdown(string reason) {
    if(!reason || !sizeof(reason)) { write("Cancelled.\n"); return; }
    shout("MUD shutting down: " + reason + "\n");
    catch(log_file("game_log", ctime(time()) + " Shutdown via staff by " +
        this_player()->query_name() + " (" + reason + ")\n"));
    shutdown();
}

void do_reload(string daemon_name) {
    string path;
    object ob;

    if(!daemon_name || !sizeof(daemon_name)) { write("Cancelled.\n"); return; }
    switch(lower_case(daemon_name)) {
    case "rifts": path = "/daemon/rifts"; break;
    case "occ": path = "/daemon/occ"; break;
    case "rifts_skills": path = "/daemon/rifts_skills"; break;
    case "rifts_combat": path = "/daemon/rifts_combat"; break;
    default:
        write("Unknown daemon. Try: rifts, occ, rifts_skills, rifts_combat");
        return;
    }
    ob = find_object(path);
    if(ob) {
        destruct(ob);
        write("Destructed " + path + ".");
    }
    call_other(path, "??");
    write("Reloaded " + path + ".");
}
