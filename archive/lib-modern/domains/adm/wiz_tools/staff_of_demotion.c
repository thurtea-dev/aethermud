// /domains/adm/wiz_tools/staff_of_demotion.c
// Staff of Demotion -- Admin tool for RiftsMUD
// Menu-driven admin functions for archon-level staff.

#include <std.h>
#include <security.h>
#include <daemons.h>

inherit OBJECT;

private nosave string pending_action;
private nosave string pending_sub;
private nosave object pending_promote_ob;
private nosave string pending_promote_pos;

void do_player_action();
private void apply_promote_position();

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
            ob->set_position("player");
            ob->save_player((string)ob->query_name());
        }
        write("Demoted " + capitalize(pending_sub) + " to player (if online).");
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
    catch(log_file("/log/adm/staff_promotions", logline));
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
