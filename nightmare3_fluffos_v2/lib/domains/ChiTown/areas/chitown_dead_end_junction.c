/* /domains/ChiTown/areas/chitown_dead_end_junction.c
   Dead end deeper in the maintenance tunnels. Atmosphere only, no mob spawn. */

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
        "Something wet drips from the junction overhead.");
    set_exits( ([
        "west" : "/domains/ChiTown/areas/chitown_maintenance_tunnel"
    ]) );
    set_listen("default",
        "A slow, heavy drip. Otherwise silence. The silence is worse.");
    set_smell("default",
        "Wet stone, old rust, and something faintly organic.");
    set_items( ([
        "ceiling"  : "Sagged and cracked. It could come down further at any time.",
        "scratches": "Deep gouges in the concrete, well above head height.",
        "drip"     : "Something wet falls from the junction overhead at irregular intervals."
    ]) );
}

void reset() { ::reset(); }
