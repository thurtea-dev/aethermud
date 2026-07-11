// /domains/adm/wiz_tools/tattoo_gun.c
// Tattoo Gun -- RP Wizard tool for Atlantean/Tattooed Man tattoo management.
// Uses the current location-based TATTOO_D API.
// Usage: use gun / use tattoo gun

#include <std.h>
#include <daemons.h>

inherit OBJECT;

static private object selected_player;
static private string *loc_list;     // locations of current tattoos (for remove)
static private string pending_tattoo; // tattoo name during grant flow

void do_view_tattoos();
void do_grant_tattoo_entry();
void do_remove_tattoo_entry();

void create() {
    ::create();
    set_name("tattoo-gun");
    set_id( ({ "tattoo-gun", "tattoo gun", "gun", "tattoo" }) );
    set_short("a tattoo-gun");
    set_long(
        "A high-precision Atlantean tattoo device. It uses refined PPE to\n"
        "inscribe magical tattoos that grant real powers to True Atlanteans\n"
        "and Tattooed Men. For RP Wizard use only.\n"
        "Type: inscribe");
    set_mass(200);
    set_value(0);
    set_property("no_give", 1);
}

mixed *query_auto_load() {
    return ({ base_name(this_object()), ({}) });
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_use_gun", "inscribe");
}

private void show_menu() {
    string pname;
    pname = (selected_player && objectp(selected_player))
          ? (string)selected_player->query_cap_name()
          : "(none)";
    write("\n=== TATTOO GUN === [Player: " + pname + "]");
    write(" 1. Select a player");
    write(" 2. View player tattoos");
    write(" 3. Grant a tattoo");
    write(" 4. Remove a tattoo");
    write(" 0. Exit");
    write("Choice: ");
}

int cmd_use_gun(string str) {
    if(!admin_wizp(this_player()) && !rp_wizp(this_player()) &&
       !domain_wizp(this_player())) {
        write("The gun does not respond to you.\n");
        return 1;
    }
    if(!creatorp(this_player())) {
        write("The gun does not respond to you.\n");
        return 1;
    }
    pending_tattoo = "";
    loc_list = ({});
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
    object ob;
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
    selected_player = ob;
    write("Selected: " + (string)ob->query_cap_name() + "\n");
    show_menu();
    input_to("handle_choice");
}

// ── Option 2: View tattoos ───────────────────────────────────────────────────

void do_view_tattoos() {
    mapping tattoos;
    string *locs;
    mapping def;
    int i;
    string name;

    if(!selected_player || !objectp(selected_player)) {
        write("No player selected. Use option 1 first.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    tattoos = (mapping)TATTOO_D->query_tattoos(selected_player);
    locs    = keys(tattoos);

    write("\nTattoos for " + (string)selected_player->query_cap_name() + ":");
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

// ── Option 3: Grant a tattoo ─────────────────────────────��───────────────────

void do_grant_tattoo_entry() {
    mapping all;
    string *names;
    mapping def;
    int i;

    if(!selected_player || !objectp(selected_player)) {
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
    mapping all;
    string *names;
    mapping def;
    int choice;
    string tattoo_name;
    int i;

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

    pending_tattoo = tattoo_name;

    write("Valid locations for " + tattoo_name + ": " +
          implode((string *)def["locations"], ", ") + "\n");
    write("Body location: ");
    input_to("pick_grant_location");
}

void pick_grant_location(string str) {
    string location;
    int ok;

    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        pending_tattoo = "";
        show_menu();
        input_to("handle_choice");
        return;
    }
    location = lower_case(str);

    if(!pending_tattoo || !sizeof(pending_tattoo)) {
        write("Error: no tattoo selected.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    ok = (int)TATTOO_D->grant_tattoo(selected_player, pending_tattoo, location);
    if(!ok) {
        write("Inscription failed. Check slot availability and location validity.\n");
    } else {
        write("Inscribed " + pending_tattoo + " on " + location + " for " +
              (string)selected_player->query_cap_name() + ".\n");
        tell_object(selected_player,
            "A " + pending_tattoo + " tattoo is inscribed on your " +
            location + ".\n");
    }

    pending_tattoo = "";
    show_menu();
    input_to("handle_choice");
}

// ── Option 4: Remove a tattoo ────────────────────────────────────────────────

void do_remove_tattoo_entry() {
    mapping tattoos;
    string *locs;
    mapping def;
    int i;
    string name;

    if(!selected_player || !objectp(selected_player)) {
        write("No player selected. Use option 1 first.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    tattoos = (mapping)TATTOO_D->query_tattoos(selected_player);
    locs    = keys(tattoos);

    if(!sizeof(locs)) {
        write((string)selected_player->query_cap_name() + " has no tattoos.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    loc_list = locs;
    write("Current tattoos for " + (string)selected_player->query_cap_name() + ":");
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
    int choice;
    string location;
    int ok;

    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    choice = to_int(str);
    if(choice >= 1 && choice <= sizeof(loc_list))
        location = loc_list[choice - 1];
    else
        location = lower_case(str);

    if(!location || !sizeof(location)) {
        write("Invalid.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }

    ok = (int)TATTOO_D->remove_tattoo(selected_player, location);
    if(!ok) {
        write((string)selected_player->query_cap_name() +
              " has no tattoo at " + location + ".\n");
    } else {
        write("Removed tattoo from " + location + " on " +
              (string)selected_player->query_cap_name() + ".\n");
    }
    show_menu();
    input_to("handle_choice");
}
