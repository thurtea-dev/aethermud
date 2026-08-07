/* /domains/ChiTown/areas/chitown_lower_market.c
   Underground commercial district beneath the Chi-Town vendor plaza. */

#include <std.h>
#include <rooms.h>

#define MOB_PATH "/domains/ChiTown/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Chi-Town, Lower Market");
    set_long(
        "The lower market runs beneath the main plaza, accessed by wide\n"
        "concrete ramps. Food stalls and scrap merchants line both sides.\n"
        "Coalition oversight is thinner here and the crowd is rougher.\n"
        "Gangers watch from the corner of the east passage.");
    set_exits( ([
        "up"   : "/domains/ChiTown/areas/chitown_market",
        "east" : "/domains/ChiTown/areas/chitown_maintenance_tunnel"
    ]) );
    set_listen("default",
        "Haggling voices, a generator humming, and the scrape of crates on concrete.");
    set_smell("default",
        "Cooking grease, machine oil, and unwashed crowds.");
    set_items( ([
        "stalls"     : "Food stalls and scrap merchants. Cash only, no questions.",
        "ramps"      : "Wide concrete ramps leading up to the plaza market.",
        "crowd"      : "Rougher than the plaza above. Fewer CS patrols come down here.",
        "gangers"    : "A knot of gangers watching the east passage."
    ]) );
}

void reset() {
    ::reset();
    if(!present("chi-town ganger", this_object()))
        clone_object(MOB_PATH + "chitown_ganger")->move(this_object());
}
