/* /domains/wizards/rift_opener.c
   Techno-Wizard rift opener in the Wizard's Lounge.
   Lets wizard staff jump to key world locations for testing.
   Command: use rift  (also: activate rift) */

#include <std.h>

inherit OBJECT;

private string *dest_names;
private string *dest_paths;

void create() {
    ::create();
    set_name("rift opener");
    set_id( ({ "rift opener", "rift", "opener",
               "techno-wizard rift opener", "dimensional rift opener" }) );
    set_short("a Techno-Wizard rift opener");
    set_long(
        "A compact Techno-Wizard device the size of a travel case. Brass dials\n"
        "and crystal readouts cover its surface. It can open a temporary rift to\n"
        "any registered waypoint across the world. Wizard staff only.\n"
        "Type 'use rift' to activate.\n"
    );
    set_mass(800);
    set_value(0);
    set_property("no_drop", 1);
    set_property("no_get", 1);
    dest_names = ({
        "Monument Square, Praxis hub",
        "Splynn Rift Arrival Plaza",
        "Rocky's Bar, Praxis",
        "Chi-Town Gate",
        "New Camelot Gate",
        "City of Lazlo",
        "Wizard Hallway"
    });
    dest_paths = ({
        "/domains/Praxis/areas/monument_square",
        "/domains/Praxis/areas/splynn/splynn_entry",
        "/domains/Praxis/areas/riffys_bar",
        "/domains/ChiTown/areas/chitown_gate",
        "/domains/NewCamelot/areas/newcamelot_start",
        "/domains/Lazlo/lazlo_welcome",
        "/domains/wizards/hallway"
    });
}

void init() {
    ::init();
    add_action("cmd_use_rift", "use");
    add_action("cmd_use_rift", "activate");
}

private void show_menu() {
    int i;
    write("\n=== Rift Opener ===");
    for(i = 0; i < sizeof(dest_names); i++) {
        if(file_size(dest_paths[i] + ".c") > 0)
            write(sprintf(" %d. %s", i + 1, dest_names[i]));
        else
            write(sprintf(" %d. %s [unavailable]", i + 1, dest_names[i]));
    }
    write(" 0. Cancel");
    write("Destination: ");
}

int cmd_use_rift(string str) {
    if(!str || strsrch(lower_case(str), "rift") == -1) return 0;
    if(!creatorp(this_player())) {
        write("The device does not respond to you.\n");
        return 1;
    }
    show_menu();
    input_to("rift_destination");
    return 1;
}

void rift_destination(string str) {
    int choice;
    string dest;

    if(!str || str == "0" || !sizeof(str)) {
        write("Rift closed.\n");
        return;
    }
    choice = to_int(str) - 1;
    if(choice < 0 || choice >= sizeof(dest_paths)) {
        write("Invalid choice. Rift closed.\n");
        return;
    }
    dest = dest_paths[choice];
    if(file_size(dest + ".c") <= 0) {
        write("That destination is not yet built. No rift forms.\n");
        return;
    }
    write("A rift shimmers open before you...\n");
    this_player()->move_player(dest);
}
