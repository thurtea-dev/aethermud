// /domains/adm/wiz_tools/demotion_menu_d.c
// Shared Staff of Demotion menu logic. Extracted from staff_of_demotion.c
// so both the physical staff and ring_of_dominion.c can drive the same
// admin menu without duplicating it. Never cloned -- reached only via ->
// as an auto-loaded singleton, like /cmds/creator/_qcs.c.
//
// pending_* fields are per-player mappings, not bare statics: this daemon
// can be entered by more than one admin at once (a physical-staff holder
// and a ring wearer, or two ring wearers), so in-progress menu state must
// be keyed by the acting player or one admin's choices would leak into
// another's mid-session.

#include <std.h>
#include <security.h>
#include <daemons.h>

inherit DAEMON;

private mapping pending_action;
private mapping pending_sub;
private mapping pending_promote_ob;
private mapping pending_promote_pos;

void do_player_action();
private void apply_promote_position();
private void demote_restore(object ob);
void apply_demote_restore(object ob);
private void strip_wizard_package(object ob);
private void demote_force_quit(object ob);
private int is_mortal_start_room(string room);
private string pick_mortal_start(object ob);

void create() {
    ::create();
    pending_action = ([]);
    pending_sub = ([]);
    pending_promote_ob = ([]);
    pending_promote_pos = ([]);
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

int begin_menu(object player) {
    if(!admin_wizp(player)) {
        write("The staff refuses to respond to you.\n");
        return 1;
    }
    pending_action[player] = "";
    pending_sub[player] = "";
    show_menu();
    input_to("handle_choice");
    return 1;
}

void handle_choice(string str) {
    object player;
    int choice;

    player = this_player();
    if(!str || !sizeof(str)) { write("Invalid choice.\n"); show_menu(); input_to("handle_choice"); return; }
    choice = to_int(str);
    switch(choice) {
    case 0:
        write("Staff of Demotion closed.");
        return;
    case 1: pending_action[player] = "promote"; write("Player name: "); input_to("get_target"); return;
    case 2: pending_action[player] = "demote"; write("Player name: "); input_to("get_target"); return;
    case 3: pending_action[player] = "setlevel"; write("Player name: "); input_to("get_target"); return;
    case 4: pending_action[player] = "setrace"; write("Player name: "); input_to("get_target"); return;
    case 5: pending_action[player] = "setocc"; write("Player name: "); input_to("get_target"); return;
    case 6: pending_action[player] = "setstats"; write("Player name: "); input_to("get_target"); return;
    case 7: pending_action[player] = "title"; write("Player name: "); input_to("get_target"); return;
    case 8: pending_action[player] = "goto"; write("Player name: "); input_to("get_target"); return;
    case 9: pending_action[player] = "summon"; write("Player name: "); input_to("get_target"); return;
    case 10: pending_action[player] = "boot"; write("Player name: "); input_to("get_target"); return;
    case 11: pending_action[player] = "purge"; write("Player name: "); input_to("get_target"); return;
    case 12: pending_action[player] = "shutdown"; write("Shutdown reason: "); input_to("do_shutdown"); return;
    case 13: pending_action[player] = "reload"; write("Daemon (rifts/occ/rifts_skills/rifts_combat): "); input_to("do_reload"); return;
    default:
        write("Invalid option.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
}

void get_target(string name) {
    object player;

    player = this_player();
    if(!name || !sizeof(name)) { write("Cancelled.\n"); return; }
    pending_sub[player] = lower_case(name);
    switch(pending_action[player]) {
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
    object player;
    object ob;

    player = this_player();
    ob = find_player(pending_sub[player]);
    switch(pending_action[player]) {
    case "demote":
        if(ob) {
            if(!creatorp(ob) &&
               lower_case((string)ob->query_race()) != "wizard") {
                write(capitalize(pending_sub[player]) + " is not a wizard.");
                break;
            }
            demote_restore(ob);
            write("Demoted " + capitalize(pending_sub[player]) +
                " to mortal and restored their pre-wizard identity.");
        } else if(user_exists(pending_sub[player])) {
            /* player_object() is login-only; USERS_D restores the save
               into itself and calls apply_demote_restore(). */
            if((int)USERS_D->offline_demote(pending_sub[player]))
                write("Demoted offline player " + capitalize(pending_sub[player]) +
                    " to mortal and restored their pre-wizard identity.");
            else
                write("Unable to demote offline player " +
                    capitalize(pending_sub[player]) + ".");
        } else {
            write("No such player: " + capitalize(pending_sub[player]) + ".");
        }
        break;
    case "goto":
        if(!ob) { write("Player not found online.\n"); return; }
        player->move_player(file_name(environment(ob)));
        write("Teleported to " + capitalize(pending_sub[player]) + ".");
        break;
    case "summon":
        if(!ob) { write("Player not found online.\n"); return; }
        ob->move_player(file_name(environment(player)));
        write(capitalize(pending_sub[player]) + " summoned.");
        tell_object(ob, player->query_cap_name() + " summons you!");
        break;
    case "boot":
        if(!ob) { write("Player not found online.\n"); return; }
        tell_object(ob, "An admin has booted you from " + mud_name() + ".");
        ob->force_me("quit");
        write(capitalize(pending_sub[player]) + " has been booted.");
        break;
    case "purge":
        if(ob) { ob->remove(); destruct(ob); }
        write("WARNING: purge must be done via 'rid' command for safety.");
        write("Use: rid " + pending_sub[player]);
        break;
    }
}

private void apply_promote_position() {
    object player;

    player = this_player();
    if(pending_promote_ob[player])
        pending_promote_ob[player]->set_position(pending_promote_pos[player]);
}

/* Strip Wizard spells/psionics/skills granted by makewiz, then rebuild
   mortal race/OCC packages from the restored identity. */
private void strip_wizard_package(object ob) {
    string *wizard_skills;
    string *have;
    string val;
    int i;

    if(!ob) return;

    /* Restore pre-promotion spell/psi lists when snapshotted; otherwise
       clear the all-spells/all-psionics Wizard dump. */
    val = (string)ob->getenv("premote_spells");
    if(val && sizeof(val)) ob->setenv("known_spells", val);
    else ob->remove_env("known_spells");
    val = (string)ob->getenv("premote_psionics");
    if(val && sizeof(val)) ob->setenv("known_psionics", val);
    else ob->remove_env("known_psionics");

    wizard_skills = ({
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
    /* Wizard package grants these at 98. Only strip that tier so a
       mortal who already had the skill at a normal starting % keeps it. */
    have = (string *)ob->query_all_skills();
    if(have) {
        for(i = 0; i < sizeof(wizard_skills); i++) {
            if(member_array(wizard_skills[i], have) == -1) continue;
            if((int)ob->query_base_skill(wizard_skills[i]) < 98) continue;
            catch(ob->delete_skill(wizard_skills[i]));
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
    if(interactive(ob))
        tell_object(ob,
            "Your wizard status has been revoked. Please reconnect.\n");
    /* Explicit save so race, stats, and primary_start hit the .o file. */
    catch(ob->save_player((string)ob->query_name()));
    if(interactive(ob)) {
        sink = new(OBJECT);
        if(sink) {
            catch(exec(sink, ob));
            if(objectp(sink)) destruct(sink);
        }
    }
    if(objectp(ob)) catch(ob->remove());
}

/* Reject wizard workrooms, the wizard hallway, chargen, and rifts_welcome. */
private int is_mortal_start_room(string room) {
    if(!room || !sizeof(room)) return 0;
    if(file_size(room + ".c") <= 0) return 0;
    if(strsrch(room, "/realms/") == 0) return 0;
    if(strsrch(room, "/domains/wizards/") == 0) return 0;
    if(room == "/domains/Praxis/rifts_welcome") return 0;
    if(room == "/domains/Praxis/setter") return 0;
    return 1;
}

private string pick_mortal_start(object ob) {
    string start_room;

    start_room = (string)ob->getenv("premote_start");
    if(is_mortal_start_room(start_room)) return start_room;
    start_room = (string)ob->query_primary_start();
    if(is_mortal_start_room(start_room)) return start_room;
    start_room = (string)ob->getenv("start");
    if(is_mortal_start_room(start_room)) return start_room;
    if(environment(ob)) {
        start_room = base_name(environment(ob));
        if(is_mortal_start_room(start_room)) return start_room;
    }
    return "/domains/Praxis/areas/monument_square";
}

/* Full mortal restoration for a demoted wizard. Reads the premote_*
   env snapshot written by makewiz (see _makewiz.c for the fixed
   premote_stats order). Wizards promoted before the snapshot existed
   fall back to a fresh reroll for the restored race.
   Public entry used by USERS_D->offline_demote(). */
void apply_demote_restore(object ob) {
    demote_restore(ob);
}

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
    ob->remove_env("wiz_role");

    race = (string)ob->getenv("premote_race");
    if(!race || !sizeof(race) || lower_case(race) == "wizard")
        race = "human";
    cls = (string)ob->getenv("premote_class");
    if(!cls || !sizeof(cls) || lower_case(cls) == "wizard")
        cls = race;
    ob->set_race(race);
    ob->set_class(cls);
    /* Completed mortals must not re-enter chargen on next login. */
    ob->setenv("creation_step", "done");

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

    start_room = pick_mortal_start(ob);
    ob->set_primary_start(start_room);
    ob->setenv("start", start_room);

    strip_wizard_package(ob);

    /* Defense in depth: never leave a demoted player as Wizard. */
    if(lower_case((string)ob->query_race()) == "wizard") {
        ob->set_race("human");
        race = "human";
    }

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

    /* Save before quit so race/start_room survive reconnect. */
    catch(ob->save_player((string)ob->query_name()));

    catch(log_file("adm/staff_promotions",
        (string)(this_player() ? this_player()->query_name() : "system") +
        " demoted " +
        (string)ob->query_name() + " to mortal (" + race +
        ", start " + start_room + "): " +
        ctime(time()) + "\n"));
    /* Interactive players must reconnect; offline USERS_D restore
       must not destroy the users daemon. */
    if(interactive(ob))
        demote_force_quit(ob);
}

void do_promote(string pos) {
    object player;
    object ob;
    string logline;

    player = this_player();
    if(!pos || !sizeof(pos)) { write("Cancelled.\n"); return; }
    pos = lower_case(pos);
    if(pos != "creator" && pos != "arch" && pos != "rp_wiz" && pos != "domain_wiz") {
        write("Position must be 'arch', 'domain_wiz', 'creator', or 'rp_wiz'.\n");
        return;
    }
    ob = find_player(pending_sub[player]);
    if(ob) {
        ob->setenv("creation_step", "done");
        ob->remove_env("awaiting_occ");
        ob->remove_env("awaiting_alignment");
        ob->remove_env("awaiting_region");
        ob->remove_env("awaiting_elective_skills");
        ob->remove_env("stats_rolled");
        pending_promote_ob[player] = ob;
        pending_promote_pos[player] = pos;
        unguarded((: apply_promote_position :));
        pending_promote_ob[player] = 0;
        pending_promote_pos[player] = 0;
        if((int)ob->query_level() < 20)
            ob->set_level(20);
        ob->move_to_workroom();
        if(environment(ob))
            catch(ob->set_primary_start(base_name(environment(ob))));
        ob->save_player((string)ob->query_name());
    } else
        USERS_D->xmote(pending_sub[player], pos, 0);
    write(capitalize(pending_sub[player]) + " promoted to " + pos + ".");
    logline = player->query_name() + " promoted " + pending_sub[player] +
        " to " + pos + ": " + ctime(time()) + "\n";
    catch(log_file("adm/staff_promotions", logline));
}

void do_setlevel(string val) {
    object player;
    object ob;
    int lev;

    player = this_player();
    if(!val || !to_int(val)) { write("Cancelled.\n"); return; }
    lev = to_int(val);
    if(lev < 1) lev = 1;
    if(lev > 25) lev = 25;
    ob = find_player(pending_sub[player]);
    if(ob) ob->set_level(lev);
    else USERS_D->xmote(pending_sub[player], 0, lev);
    write(capitalize(pending_sub[player]) + " set to level " + lev + ".");
}

void do_setrace(string race) {
    object player;
    object ob;

    player = this_player();
    if(!race || !sizeof(race)) { write("Cancelled.\n"); return; }
    ob = find_player(pending_sub[player]);
    if(!ob) { write("Player not online.\n"); return; }
    ob->set_race(lower_case(race));
    write(capitalize(pending_sub[player]) + "'s race set to: " + race + ".");
}

void do_setocc(string occ) {
    object player;
    object ob;

    player = this_player();
    if(!occ || !sizeof(occ)) { write("Cancelled.\n"); return; }
    ob = find_player(pending_sub[player]);
    if(!ob) { write("Player not online.\n"); return; }
    ob->setenv("rifts_occ", lower_case(occ));
    write(capitalize(pending_sub[player]) + "'s OCC set to: " + occ + ".");
}

void do_setstats(string vals) {
    object player;
    object ob;
    string *parts;
    string *stat_names;
    int i;

    player = this_player();
    if(!vals || !sizeof(vals)) { write("Cancelled.\n"); return; }
    ob = find_player(pending_sub[player]);
    if(!ob) { write("Player not online.\n"); return; }
    parts = explode(vals, " ");
    stat_names = ({ "IQ", "ME", "MA", "PS", "PP", "PE", "PB", "Spd" });
    if(sizeof(parts) < 8) { write("Need 8 values.\n"); return; }
    for(i = 0; i < 8; i++) ob->set_stats(stat_names[i], to_int(parts[i]));
    write(capitalize(pending_sub[player]) + "'s stats updated.");
}

void do_title(string title) {
    object player;
    object ob;

    player = this_player();
    if(!title || !sizeof(title)) { write("Cancelled.\n"); return; }
    ob = find_player(pending_sub[player]);
    if(!ob) { write("Player not online.\n"); return; }
    if(lower_case(title) == "remove") title = capitalize(pending_sub[player]);
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
