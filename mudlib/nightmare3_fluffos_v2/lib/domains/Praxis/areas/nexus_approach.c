// /d/Praxis/areas/nexus_approach.c
// Ley line crossing: air crackles with magical energy.
// MDC beings regenerate PPE 2x while here (tracked via room property).

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("ley_line_nexus", 1);
    set_property("ley_line_room", 1);
    set_property("is_ley_line", 1);
    set_property("ley_line", 1);
    set_property("nexus", 1);
    set_property("ppe_regen_bonus", 2);
    set_heart_beat(1);
    set_short("Ley Line Nexus: Highway Crossing");
    set_long(
        "The air here is alive. A visible ley line crosses the road, a ribbon\n"
        "of crackling blue-white energy about fifteen meters wide, running east to\n"
        "west across the highway and as far in both directions as you can see. The\n"
        "asphalt in the nexus zone is scorched glassy smooth. Nothing organic grows\n"
        "within thirty meters of the crossing.\n\n"
        "Magical beings feel the pull immediately. PPE flows here like water over\n"
        "stone. For those who can channel it, this place is a well that never runs\n"
        "dry. For those who cannot, it is merely unsettling.\n\n"
        "A ruined office building is visible to the east, half-consumed by\n"
        "energy-bleached vegetation.  To the north, a military road leads\n"
        "toward what looks like a fortified installation. A violet scar in the\n"
        "ley line opens downward: a dimensional tear you can descend to reach.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/highway_ruins",
        "east"  : "/domains/Praxis/areas/collapsed_building",
        "west"  : "/domains/Praxis/areas/hydra_lair",
        "north" : "/domains/LoneStar/areas/lone_star_gate",
        "down"  : "/domains/Praxis/areas/nexus_dimensional_tear"
    ]) );
    set_listen("default",
        "The ley line makes a sound like massed voices speaking a language you almost understand.");
    set_smell("default",
        "Pure ozone. The smell of raw magical energy made physical.");
    set_items( ([
        "ley line" : "A ribbon of blue-white dimensional energy crossing the highway. This is a power source of staggering magnitude.",
        "leyline" : "A ribbon of blue-white dimensional energy crossing the highway. This is a power source of staggering magnitude.",
        "crossing" : "The nexus point where the ley line intersects the highway. The asphalt is glassy smooth and warm to the touch.",
        "asphalt" : "Scorched glassy by years of ley line energy discharge. Warm underfoot.",
        "building" : "The ruined office building to the east. Something moves inside.",
        "energy" : "Blue-white light visible as a physical object: the ley line made manifest.",
        "tear" : "A faint violet distortion hangs over the highway to the south. "
                 "Descend the ley scar to reach it."
    ]) );
}

void heart_beat() {
    object *inv;
    string race;
    int ppe, max_ppe, bonus;
    int i;

    inv = all_inventory(this_object());
    i = sizeof(inv);
    while(i--) {
        if(!living(inv[i])) continue;
        race = lower_case((string)inv[i]->query_race());
        if(!race || !strlen(race)) continue;
        if(!(int)RIFTS_D->is_rifts_race(race)) continue;
        ppe = (int)inv[i]->query_stats("PPE");
        max_ppe = (int)inv[i]->query_stats("max_PPE");
        if(ppe < max_ppe) {
            bonus = 2;
            if(ppe + bonus > max_ppe) bonus = max_ppe - ppe;
            inv[i]->set_stats("PPE", ppe + bonus);
        }
    }
}

void reset() {
    ::reset();
}
