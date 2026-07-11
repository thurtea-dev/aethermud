// /obj/tattoo_gun.c
// Atlantean Tattoo Gun  -  physical in-game inscription tool.
// The tattoo artist carrying this item can inscribe magical tattoos on players.
// Usage: inscribe <player>  (then follow the interactive menu)
// Requires: TATTOO_D  (tattoo daemon must be loaded)

#include <std.h>
#include <daemons.h>

inherit OBJECT;

private object __target_player;
private string *__location_list;
private int     __step;
// __step: 0=idle, 1=awaiting location, 2=awaiting tattoo, 3=awaiting confirm

private void show_location_menu();
private void show_tattoo_menu(string location);

void create() {
    ::create();
    __step  = 0;
    __location_list = ({
        "right forearm", "left forearm", "right upper arm", "left upper arm",
        "right hand",    "left hand",    "chest",           "back",
        "right shoulder","left shoulder",
        "right leg",     "left leg",     "right calf",      "left calf",
        "neck",          "face"
    });

    set_name("tattoo gun");
    set_id(({ "tattoo gun", "gun", "inscription tool", "tattoo device",
              "atlantean tattoo gun" }));
    set_short("an Atlantean tattoo gun");
    set_long(
        "A sleek device of alien design  -  part tool, part magical focus.\n"
        "Inscriptions cover the barrel in ancient Atlantean script.\n"
        "A PPE crystal glows softly in the grip.\n"
        "A skilled tattoo master uses this to inscribe magical tattoos.\n"
        "Type:  inscribe <player>  to begin a tattoo inscription session.");
    set_mass(200);
    set_value(50000);
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_inscribe", "inscribe");
}

// ── Main command: inscribe <player> ──────────────────────────────────────

int cmd_inscribe(string str) {
    object target;

    if(!str || !sizeof(str)) {
        write("Inscribe a tattoo on whom?\n"
              "Syntax:  inscribe <player>\n");
        return 1;
    }

    target = find_player(str);
    if(!target) target = present(str, environment(this_player()));
    if(!target || !living(target)) {
        write("You don't see '" + str + "' here.\n");
        return 1;
    }

    if(target == this_player()) {
        write("You cannot inscribe your own tattoos.\n");
        return 1;
    }

    if(__step != 0) {
        write("You are already in an inscription session.  "
              "Type 'cancel' to abort.\n");
        return 1;
    }

    __target_player = target;
    __step = 1;
    write("\n=== ATLANTEAN TATTOO GUN  ===\n"
          "Target: " + (string)target->query_cap_name() + "\n"
          "Select a body location to inscribe, or 'cancel' to abort.\n\n");
    show_location_menu();
    input_to("handle_location");
    return 1;
}

// ── Step 1: Location selection ────────────────────────────────────────────

private void show_location_menu() {
    mapping player_tattoos;
    string used, *locs, loc;
    int i, max_slots;

    if(!objectp(__target_player)) {
        write("Target player left.\n");
        __step = 0;
        return;
    }

    player_tattoos = (mapping)TATTOO_D->query_tattoos(__target_player);
    max_slots = (lower_case((string)__target_player->getenv("rifts_occ")) ==
                 "sunaj assassin") ? 20 : 16;

    write("\n--- Body Locations ---\n");
    write("Used slots: " + sizeof(keys(player_tattoos)) + " / " + max_slots + "\n\n");

    for(i = 0; i < sizeof(__location_list); i++) {
        loc  = __location_list[i];
        used = player_tattoos[loc];
        write("  " + (i + 1) + ".  " + loc +
              (used ? "   [" + used + "]" : "") + "\n");
    }
    write("\nEnter location number or name (or 'cancel'): ");
}

void handle_location(string str) {
    string loc;
    int idx;

    if(!str || !sizeof(str)) {
        write("Enter a location number or name.\n");
        show_location_menu();
        input_to("handle_location");
        return;
    }

    if(lower_case(str) == "cancel") {
        write("Inscription cancelled.\n");
        __step = 0;
        __target_player = 0;
        return;
    }

    idx = to_int(str);
    if(idx > 0 && idx <= sizeof(__location_list)) {
        loc = __location_list[idx - 1];
    } else {
        loc = lower_case(str);
        if(member_array(loc, __location_list) == -1) {
            write("Unknown location '" + str + "'.\n");
            show_location_menu();
            input_to("handle_location");
            return;
        }
    }

    // Check if slot is taken
    {
        mapping pt;
        string existing;
        pt       = (mapping)TATTOO_D->query_tattoos(__target_player);
        existing = pt[loc];
        if(existing) {
            write("That slot already has a " + existing + " tattoo.\n"
                  "Pick another location.\n");
            show_location_menu();
            input_to("handle_location");
            return;
        }
    }

    __step = 2;
    write("\nLocation: " + loc + "\n");
    show_tattoo_menu(loc);
    input_to("handle_tattoo_choice", loc);
}

// ── Step 2: Tattoo selection ──────────────────────────────────────────────

private void show_tattoo_menu(string location) {
    mapping all_t;
    string *names, name, last_cat;
    mapping def;
    int i, n;

    all_t  = (mapping)TATTOO_D->query_all_tattoos();
    names  = sort_array(keys(all_t), 1);
    last_cat = "";
    n = 0;

    write("\n--- Tattoos valid for " + location + " ---\n");

    for(i = 0; i < sizeof(names); i++) {
        name = names[i];
        def  = all_t[name];
        if(member_array(location, (string *)def["locations"]) == -1) continue;
        n++;
        if((string)def["category"] != last_cat) {
            last_cat = (string)def["category"];
            write("\n  [ " + last_cat + " ]\n");
        }
        write("  " + n + ".  " + capitalize(name) + "\n"
              "      " + (string)def["desc"] + "\n");
    }
    if(n == 0) write("  (no tattoos valid for this location)\n");
    write("\nEnter tattoo name exactly (or 'back', 'cancel'): ");
}

void handle_tattoo_choice(string str, string location) {
    mapping def;
    string name_lc;

    if(!str || !sizeof(str)) {
        show_tattoo_menu(location);
        input_to("handle_tattoo_choice", location);
        return;
    }
    if(lower_case(str) == "cancel") {
        write("Inscription cancelled.\n");
        __step = 0;
        __target_player = 0;
        return;
    }
    if(lower_case(str) == "back") {
        __step = 1;
        show_location_menu();
        input_to("handle_location");
        return;
    }

    name_lc = lower_case(str);
    def = (mapping)TATTOO_D->query_tattoo_def(name_lc);
    if(!def) {
        write("Unknown tattoo '" + str + "'.\n");
        show_tattoo_menu(location);
        input_to("handle_tattoo_choice", location);
        return;
    }
    if(member_array(location, (string *)def["locations"]) == -1) {
        write(capitalize(name_lc) + " cannot be inscribed on the " +
              location + ".\n");
        show_tattoo_menu(location);
        input_to("handle_tattoo_choice", location);
        return;
    }

    __step = 3;
    write("\n--- Confirm Inscription ---\n"
          "Target:   " + (string)__target_player->query_cap_name() + "\n"
          "Location: " + location + "\n"
          "Tattoo:   " + capitalize(name_lc) + "\n"
          "  " + (string)def["desc"] + "\n\n"
          "Confirm? (yes / no): ");
    input_to("handle_confirm", location, name_lc);
}

// ── Step 3: Confirmation ──────────────────────────────────────────────────

void handle_confirm(string str, string location, string tattoo_name) {
    if(!str || !sizeof(str)) {
        write("Enter 'yes' or 'no': ");
        input_to("handle_confirm", location, tattoo_name);
        return;
    }
    if(lower_case(str) == "cancel" || str[0] == 'n' || str[0] == 'N') {
        write("Inscription cancelled.\n");
        __step = 0;
        __target_player = 0;
        return;
    }
    if(str[0] != 'y' && str[0] != 'Y') {
        write("Enter 'yes' or 'no': ");
        input_to("handle_confirm", location, tattoo_name);
        return;
    }

    if(!objectp(__target_player)) {
        write("The target player is no longer here.\n");
        __step = 0;
        __target_player = 0;
        return;
    }

    // Apply via TATTOO_D
    {
        int ok;
        ok = (int)TATTOO_D->grant_tattoo(__target_player, tattoo_name, location);
        if(!ok) {
            write("Inscription failed.  Check previous error.\n");
        }
    }

    __step = 0;
    __target_player = 0;
}

mixed *query_auto_load() {
    return ({ base_name(this_object()), ({}) });
}
