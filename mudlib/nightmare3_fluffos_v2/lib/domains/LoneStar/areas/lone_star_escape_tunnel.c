/* /domains/LoneStar/areas/lone_star_escape_tunnel.c
   Lone Star maintenance tunnel, breached from the perimeter.
   First leg of the escape route out of the complex.
   Connects south to the outer perimeter, north to the tunnel junction. */

#define LS_MON_PATH "/domains/LoneStar/monsters/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Lone Star: Breached Maintenance Tunnel");
    set_long(
        "A drainage and cable-access tunnel running under the perimeter\n"
        "fence, meant for maintenance crews rather than anyone trying to\n"
        "leave. A section of grating at the far end has been forced from\n"
        "the inside, bent outward, the metal still bright where it tore.\n"
        "Whoever came through here badly wanted out.\n\n"
        "The run continues north past the torn grating, into a stretch the\n"
        "Coalition's own plans record only as a drainage hazard. The\n"
        "perimeter road is back south.");
    set_exits( ([
        "south" : "/domains/LoneStar/areas/lone_star_perimeter",
        "north" : "/domains/LoneStar/areas/lone_star_tunnel_junction"
    ]) );
    set_listen("default",
        "Water dripping somewhere further down the tunnel. Distant traffic\n"
        "noise from the perimeter road, muffled by the concrete.");
    set_smell("default",
        "Standing water, rust, and a faint chemical trace that does not\n"
        "belong to any standard Coalition supply.");
    set_items( ([
        "grating" : "A section of maintenance grating forced outward from\n"
                    "the inside. The torn metal is still bright, not yet\n"
                    "rusted over.",
        "tunnel"  : "A cramped drainage and cable-access tunnel, built for\n"
                    "maintenance crews, not escape.",
        "cables"  : "Bundled cable runs along one wall, undisturbed."
    ]) );
}

void reset() {
    ::reset();
    if(random(2) == 0 && !present("mutant subject", this_object()))
        clone_object(LS_MON_PATH + "ls_mutant_subject.c")->move(this_object());
}
