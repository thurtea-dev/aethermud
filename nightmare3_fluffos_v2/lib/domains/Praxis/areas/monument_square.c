// /d/Praxis/areas/monument_square.c
// Monument Square: the primary hub of Praxis for returning Rifts players.
// Central trade plaza around a ruined pre-Rifts monument.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_property("vehicle_accessible", 1);
    set_short("Monument Square, Praxis hub");
    set_long(
        "Monument Square is the central gathering point of Praxis. The ruins\n"
        "of a pre-Rifts plaza have been cleared and rebuilt with salvaged\n"
        "materials. A monument to the fallen stands at the center. Two main\n"
        "roads intersect here. Vendors, travelers, and mercenaries pass\n"
        "through constantly. The smell of cook fires and machine oil hangs\n"
        "in the air. The hum of a nearby ley line is faint but constant.");
    set_exits( ([
        "north"     : "/domains/Praxis/rifts_welcome",
        "east"      : "/domains/Praxis/areas/market_street",
        "west"      : "/domains/Praxis/areas/rockys_bar",
        "south"     : "/domains/Praxis/areas/coalition_checkpoint",
        "northwest" : "/domains/Praxis/inn101"
    ]) );
    set_listen("default",
        "The overlapping noise of a hundred conversations, distant machinery, "
        "and the periodic crackle of broadcast poles.");
    set_smell("default",
        "Dust, cooking food, exhaust, and underneath it all the faint ozone of ley line proximity.");
    set_items( ([
        "monument" :
            "A cracked stone monument, pre-Rifts civic sculpture stripped of meaning.\n"
            "The original inscription is gone. PRAXIS is spray-painted over the base\n"
            "in yellow letters. Someone has left a small bouquet of salvaged wire\n"
            "flowers at its foot.",
        "bouquet" : "Wire twisted into rough flower shapes. Someone left these recently.",
        "square" : "The central plaza of Praxis, surrounded by improvised structures.",
        "structures" : "Every doorway and alcove holds a lean-to, stall, or sleeping spot.",
        "broadcast" :
            "A tall pole with a speaker array at the top. It's broadcasting a crackly\n"
            "automated message: 'Praxis welcomes all survivors. Report D-Bee aggression\n"
            "to Coalition checkpoint. Curfew begins at midnight. Mercs seeking work:\n"
            "ask Marta or Griz on Market Street east, or Rocky at the bar west.\n"
            "Type quests to track jobs you accept.'",
        "pole" : "A broadcast pole. The signal is automated and mostly static.",
        "crowd" : "Survivors and D-Bees moving through the square. Everybody minding their own business."
    ]) );
}

void reset() {
    ::reset();
    if(!present("hover cycle", this_object()))
        new("/domains/Praxis/equipment/hover_cycle")->move(this_object());
    if(!present("moxim", this_object()))
        clone_object("/domains/Praxis/monsters/moxim")->move(this_object());
    if(!present("falconer", this_object()))
        clone_object("/domains/Praxis/npcs/falconer")->move(this_object());
}
