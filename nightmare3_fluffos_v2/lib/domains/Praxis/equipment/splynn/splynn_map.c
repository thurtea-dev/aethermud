// /d/Praxis/equipment/splynn/splynn_map.c
// A map of the Splynn district -- examine to see area layout.

inherit "/std/Object";

void create() {
    ::create();
    set_name("Splynn map");
    set_id( ({ "splynn map", "map of splynn", "map", "district map" }) );
    set_short("a map of the Splynn district");
    set_long(
        "A hand-drawn map on reinforced parchment, overlaid with a printed\n"
        "satellite base. The Splynn Dimensional Market is at the center.\n"
        "Annotations in several languages mark key locations.\n\n"
        "Examine the map to see the district layout.");
    set_mass(50);
    set_value(200);
    set_property("readable", 1);
}

void init() {
    ::init();
    if(environment(this_object()) == this_player())
        add_action("cmd_examine_map", "examine");
}

int cmd_examine_map(string str) {
    if(!str || (strsrch(lower_case(str), "map") == -1 &&
                strsrch(lower_case(str), "splynn") == -1)) return 0;
    if(present(lower_case(str), this_player()) != this_object()) return 0;
    write(
        "\n"
        " SPLYNN DISTRICT MAP\n"
        " ===================\n"
        "\n"
        " [Splynn Docks]\n"
        " |\n"
        " north\n"
        " |\n"
        " [Long Road] --west-- [Splynn Market] --east-- [Slave Pits]\n"
        " |\n"
        " down (dig required)\n"
        " |\n"
        " [Buried Chamber]\n"
        " (EX-5 Behemoth)\n"
        "\n"
        " Long Road runs west to Praxis (weapons shop, market, monument square).\n"
        "\n"
    );
    return 1;
}
