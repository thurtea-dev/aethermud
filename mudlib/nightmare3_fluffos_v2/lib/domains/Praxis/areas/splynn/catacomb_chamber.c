// /d/Praxis/areas/splynn/catacomb_chamber.c
// Catacomb side chamber. The ghostly katana is found here.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Catacomb Side Chamber");
    set_long(
        "The hall opens into a wider chamber with a vaulted ceiling.\n"
        "Unlike the hall, this room has a faint phosphorescent glow coming\n"
        "from mineral veins in the stone. It is just enough light to see by.\n\n"
        "In the center of the chamber, a stone pedestal rises from the floor.\n"
        "A weapon rests on it, suspended in a faint shimmer of cold light.\n"
        "The blade is not quite solid: it flickers between present and absent,\n"
        "like something caught between dimensions. The hilt is wrapped in\n"
        "dark silk that has not decayed.\n\n"
        "The katana is real enough to pick up. You can feel the wrongness\n"
        "of it from three meters away. It passes through material things\n"
        "and reaches what is behind them instead.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/catacomb_hall",
        "south" : "/domains/Praxis/areas/splynn/catacomb_tomb"
    ]) );
    set_listen("default",
        "A faint resonance, as if the stone itself is vibrating at a\n"
        "frequency just below hearing. It changes pitch when you move.");
    set_smell("default",
        "Cold mineral stone and something faintly electric from the katana.");
    set_items( ([
        "pedestal" : "A stone pedestal carved from the chamber floor. The same\n"
                     "construction as the rest of the complex. Old. Deliberate.",
        "glow" : "Phosphorescent mineral veins in the stone provide just enough\n"
                     "light to see clearly. They have been glowing for a long time.",
        "shimmer" : "The cold light surrounding the katana. Not magical, exactly.\n"
                     "Dimensional. The weapon exists in more than one place at once.",
        "silk" : "Dark silk wrapped around the hilt. It has not decayed.\n"
                     "Whatever preserved it is still working.",
        "vault" : "The ceiling is vaulted stone with the same carved figures\n"
                     "as the hall, but here they are all oriented toward the pedestal."
    ]) );
}

void reset() {
    int taken;

    ::reset();
    taken = 0;
    catch(taken = (int)UNIQUE_ITEMS_D->query_taken("ghostly_katana"));
    if(!taken && !present("katana", this_object()))
        clone_object(EQ_PATH+"ghostly_katana.c")->move(this_object());
}
