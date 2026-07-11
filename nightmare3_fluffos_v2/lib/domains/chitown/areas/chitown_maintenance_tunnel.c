/* /domains/chitown/areas/chitown_maintenance_tunnel.c
   Service tunnels beneath Chi-Town, darker and more dangerous than the
   lower market above. */

#include <std.h>
#include <rooms.h>

#define MOB_PATH "/domains/chitown/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_short("Chi-Town Maintenance Tunnel");
    set_long(
        "Low concrete tunnels carry power conduits and water pipes beneath\n"
        "Chi-Town. The lighting is emergency red. Maintenance crews stopped\n"
        "coming down here after the rat problem. Something has nested in\n"
        "the east junction. A south branch smells of the black market.\n"
        "A west crawl looks too tight for official use. A service hatch\n"
        "climbs toward the residential blocks.");
    set_exits( ([
        "west"  : "/domains/chitown/areas/chitown_lower_market",
        "east"  : "/domains/chitown/areas/chitown_dead_end_junction",
        "south" : "/domains/chitown/areas/chitown_black_market",
        "crawl" : "/domains/chitown/areas/burb_smuggler_crawl",
        "southwest" : "/domains/chitown/areas/burb_fence_stall",
        "up"    : "/domains/chitown/areas/cs_service_hatch"
    ]) );
    set_listen("default",
        "Dripping pipes, a faint electrical hum, and something scratching in the dark.");
    set_smell("default",
        "Hot conduit insulation, standing water, and rust.");
    set_items( ([
        "conduits" : "Power conduits running the length of the tunnel, patched in places.",
        "pipes"    : "Water pipes, several leaking. The floor is damp.",
        "lighting" : "Emergency red lighting. The normal fixtures have not worked in years.",
        "junction" : "A side junction to the east. Something has nested in there."
    ]) );
}

void reset() {
    ::reset();
    if(!present("tunnel rat", this_object()))
        clone_object(MOB_PATH + "chitown_tunnel_rat")->move(this_object());
}
