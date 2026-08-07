// /domains/adm/wiz_tools/tattoo_menu_d.c
// Shared Tattoo Gun menu logic. Extracted from tattoo_gun.c so both the
// physical gun and ring_of_dominion.c can drive the same tattoo grant/
// remove menu without duplicating it. Never cloned -- reached only via ->
// as an auto-loaded singleton, like /cmds/creator/_qcs.c.
//
// selected_player/loc_list/pending_tattoo are per-player mappings, not
// bare statics: this daemon can be entered by more than one RP wizard at
// once, so in-progress menu state must be keyed by the acting player or
// one wizard's session would leak into another's.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private mapping selected_player;
private mapping loc_list;       // locations of current tattoos (for remove)
private mapping pending_tattoo; // tattoo name during grant flow

void do_view_tattoos();
void do_grant_tattoo_entry();
void do_remove_tattoo_entry();

void create() {
    ::create();
    selected_player = ([]);
    loc_list = ([]);
    pending_tattoo = ([]);
}

private void show_menu() {
    object player;
    string pname;

    player = this_player();
    pname = (selected_player[player] && objectp(selected_player[player]))
          ? (string)selected_player[player]->query_cap_name()
          : "(none)";
    write("\n=== TATTOO GUN === [Player: " + pname + "]");
    write(" 1. Select a player");
    write(" 2. View player tattoos");
    write(" 3. Grant a tattoo");
    write(" 4. Remove a tattoo");
    write(" 0. Exit");
    write("Choice: ");
}

int begin_menu(object player) {
    if(!admin_wizp(player) && !rp_wizp(player) &&
       !domain_wizp(player)) {
        write("The gun does not respond to you.\n");
        return 1;
    }
    if(!creatorp(player)) {
        write("The gun does not respond to you.\n");
        return 1;
    }
    pending_tattoo[player] = "";
    loc_list[player] = ({});
    show_menu();
    input_to("handle_choice");
    return 1;
}

void handle_choice(string str) {
    int choice;

    if(!str || !sizeof(str)) {
        write("Invalid choice.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    choice = to_int(str);
    switch(choice) {
    case 0: write("Tattoo Gun closed.\n"); return;
    case 1: write("Player name: "); input_to("select_player"); return;
    case 2: do_view_tattoos(); return;
    case 3: do_grant_tattoo_entry(); return;
    case 4: do_remove_tattoo_entry(); return;
    default:
        write("Invalid option.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
}

// ── Option 1: Select player ──────────────────────────────────────────────────

void select_player(string name) {
    object player;
    object ob;

    player = this_player();
    if(!name || !sizeof(name)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    ob = find_player(lower_case(name));
    if(!ob) {
        write("Player '" + capitalize(name) + "' not found online.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    selected_player[player] = ob;
    write("Selected: " + (string)ob->query_cap_name() + "\n");
    show_menu();
    input_to("handle_choice");
}

// ── Option 2: View tattoos ───────────────────────────────────────────────────

void do_view_tattoos() {
    object player;
    mapping tattoos;
    string *locs;
    mapping def;
    int i;
    string name;

    player = this_player();
    if(!selected_player[player] || !objectp(selected_player[player])) {
        write("No player selected. Use option 1 first.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    tattoos = (mapping)TATTOO_D->query_tattoos(selected_player[player]);
    locs    = keys(tattoos);

    write("\nTattoos for " + (string)selected_player[player]->query_cap_name() + ":");
    if(!sizeof(locs)) {
        write(" (none)");
    } else {
        for(i = 0; i < sizeof(locs); i++) {
            name = tattoos[locs[i]];
            def  = (mapping)TATTOO_D->query_tattoo_def(name);
            write(sprintf("  %-20s : %-22s  [%s]",
                locs[i], name,
                def ? (string)def["type"] : "?"));
        }
    }
    write("");
    show_menu();
    input_to("handle_choice");
}

// ── Option 3: Grant a tattoo ─────────────────────────────────────────────────

void do_grant_tattoo_entry() {
    object player;
    mapping all;
    string *names;
    mapping def;
    int i;

    player = this_player();
    if(!selected_player[player] || !objectp(selected_player[player])) {
        write("No player selected. Use option 1 first.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    all   = (mapping)TATTOO_D->query_all_tattoos();
    names = sort_array(keys(all), 1);

    write("Available tattoos:");
    for(i = 0; i < sizeof(names); i++) {
        def = all[names[i]];
        write(sprintf(" %2d. %-22s %s [%s, PPE:%d]",
            i + 1,
            names[i],
            def ? (string)def["desc"] : "?",
            def ? (string)def["type"] : "?",
            def ? (int)def["ppe"]     : 0));
    }
    write("Tattoo to grant (number/name): ");
    input_to("pick_grant_tattoo");
}

void pick_grant_tattoo(string str) {
    object player;
    mapping all;
    string *names;
    mapping def;
    int choice;
    string tattoo_name;

    player = this_player();
    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    all   = (mapping)TATTOO_D->query_all_tattoos();
    names = sort_array(keys(all), 1);

    choice = to_int(str);
    if(choice >= 1 && choice <= sizeof(names))
        tattoo_name = names[choice - 1];
    else
        tattoo_name = lower_case(str);

    if(!tattoo_name || !sizeof(tattoo_name)) {
        write("Invalid.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    def = (mapping)TATTOO_D->query_tattoo_def(tattoo_name);
    if(!def) {
        write("Unknown tattoo: " + tattoo_name + "\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    pending_tattoo[player] = tattoo_name;

    write("Valid locations for " + tattoo_name + ": " +
          implode((string *)def["locations"], ", ") + "\n");
    write("Body location: ");
    input_to("pick_grant_location");
}

void pick_grant_location(string str) {
    object player;
    string location;
    int ok;

    player = this_player();
    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        pending_tattoo[player] = "";
        show_menu();
        input_to("handle_choice");
        return;
    }
    location = lower_case(str);

    if(!pending_tattoo[player] || !sizeof(pending_tattoo[player])) {
        write("Error: no tattoo selected.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    ok = (int)TATTOO_D->grant_tattoo(selected_player[player], pending_tattoo[player], location);
    if(!ok) {
        write("Inscription failed. Check slot availability and location validity.\n");
    } else {
        write("Inscribed " + pending_tattoo[player] + " on " + location + " for " +
              (string)selected_player[player]->query_cap_name() + ".\n");
        tell_object(selected_player[player],
            "A " + pending_tattoo[player] + " tattoo is inscribed on your " +
            location + ".\n");
    }

    pending_tattoo[player] = "";
    show_menu();
    input_to("handle_choice");
}

// ── Option 4: Remove a tattoo ────────────────────────────────────────────────

void do_remove_tattoo_entry() {
    object player;
    mapping tattoos;
    string *locs;
    mapping def;
    int i;
    string name;

    player = this_player();
    if(!selected_player[player] || !objectp(selected_player[player])) {
        write("No player selected. Use option 1 first.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    tattoos = (mapping)TATTOO_D->query_tattoos(selected_player[player]);
    locs    = keys(tattoos);

    if(!sizeof(locs)) {
        write((string)selected_player[player]->query_cap_name() + " has no tattoos.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    loc_list[player] = locs;
    write("Current tattoos for " + (string)selected_player[player]->query_cap_name() + ":");
    for(i = 0; i < sizeof(locs); i++) {
        name = tattoos[locs[i]];
        def  = (mapping)TATTOO_D->query_tattoo_def(name);
        write(sprintf(" %2d. %-20s : %-22s  [%s]",
            i + 1,
            locs[i],
            name,
            def ? (string)def["type"] : "?"));
    }
    write("Remove which (number = location #): ");
    input_to("pick_remove_tattoo");
}

void pick_remove_tattoo(string str) {
    object player;
    int choice;
    string location;
    int ok;

    player = this_player();
    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    choice = to_int(str);
    if(choice >= 1 && choice <= sizeof(loc_list[player]))
        location = loc_list[player][choice - 1];
    else
        location = lower_case(str);

    if(!location || !sizeof(location)) {
        write("Invalid.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    ok = (int)TATTOO_D->remove_tattoo(selected_player[player], location);
    if(!ok) {
        write((string)selected_player[player]->query_cap_name() +
              " has no tattoo at " + location + ".\n");
    } else {
        write("Removed tattoo from " + location + " on " +
              (string)selected_player[player]->query_cap_name() + ".\n");
    }
    show_menu();
    input_to("handle_choice");
}
