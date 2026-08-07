// /d/Praxis/areas/test_zone.c
// Coalition checkpoint ruin - combat test zone for new Rifts characters.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Ruined Coalition Checkpoint, Highway 666");
    set_long(
        "The burned-out shell of a Coalition military checkpoint straddles "
        "a cracked stretch of pre-Rifts Interstate that locals call Highway "
        "666. Twisted rebar and shattered ferrocrete mark where a guard "
        "booth once stood. A faded Coalition skull-and-bones emblem clings "
        "to the remains of a blast wall, scorched by plasma fire.\n\n"
        "Scrub brush has pushed through the asphalt in the years since the "
        "post was abandoned, though spent e-clip casings still litter the "
        "ground. Something moves in the rubble to the east. It sounds "
        "like boots on gravel.\n\n"
        "The Rift arrival zone lies back to the south.");
    set_exits( ([
        "south" : "/domains/ChiTown/areas/chitown_start",
        "north" : "/domains/Praxis/areas/old_highway"
    ]) );
    set_listen("default",
        "Wind through ruined ferrocrete, and the distant hum of a ley line.");
    set_smell("default",
        "Ozone, old blood, and the faint chemical smell of discharged e-clips.");
    set_items(
        (["checkpoint" : "A Coalition forward checkpoint, long since abandoned.",
          "wall" : "Ferrocrete blast wall scarred by plasma and laser fire.",
          "emblem" : "A CS skull motif, faded but recognizable. These soldiers are gone now.",
          "rubble" : "Broken ferrocrete and twisted rebar. Good cover in a firefight.",
          "casings" : "Spent e-clip housings. Standard CS issue. The fight here was real.",
          "highway" : "Cracked Interstate asphalt, weeds splitting every joint. "
                         "Pre-Rifts engineering, abandoned post-Cataclysm."
        ]) );

    // Initial grunt spawn
    new("/domains/Praxis/monsters/coalition_grunt")->move(this_object());
}

// reset() is called by the driver periodically. Respawn the grunt if absent.
void reset() {
    if(!present("coalition grunt", this_object()))
        new("/domains/Praxis/monsters/coalition_grunt")->move(this_object());
}
