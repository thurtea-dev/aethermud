/* /domains/ChiTown/areas/chitown_dead_end_junction.c
   Dead end deeper in the maintenance tunnels. The collapsed ceiling is
   now a climbable gap into chitown_collapsed_nest.c, paying off
   chitown_maintenance_tunnel.c's own "something has nested in the east
   junction" line. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_short("Dead End Junction");
    set_long(
        "The tunnel ends at a collapsed section, the ceiling sagged and\n"
        "cracked. Scratch marks on the walls are too large to be human.\n"
        "Something wet drips from the junction overhead, through a gap in\n"
        "the collapse wide enough to climb.");
    set_exits( ([
        "west" : "/domains/ChiTown/areas/chitown_maintenance_tunnel",
        "up"   : "/domains/ChiTown/areas/chitown_collapsed_nest"
    ]) );
    set_listen("default",
        "A slow, heavy drip. Otherwise silence. The silence is worse.");
    set_smell("default",
        "Wet stone, old rust, and something faintly organic.");
    set_items( ([
        "ceiling"  : "Sagged and cracked, with a gap wide enough to climb through.",
        "scratches": "Deep gouges in the concrete, well above head height.",
        "drip"     : "Something wet falls from the junction overhead at irregular intervals."
    ]) );
}

void reset() { ::reset(); }
