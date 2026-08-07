// /domains/Lazlo/areas/lazlo_square.c
// The central plaza of Lazlo.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Lazlo, Central Plaza");
    set_long(
        "The beating heart of Lazlo. The plaza is wide, clean, and paved\n"
        "with fitted stone that shows no cracks. A circular fountain at\n"
        "the center hums with a low magical resonance. Ley line energy\n"
        "threads through the water in visible currents of blue-white light.\n\n"
        "Scholars, mages, and citizens of a dozen species cross the plaza\n"
        "with unhurried purpose. Nobody stares at anyone else. The Great\n"
        "Library rises to the north. The market quarter is east. The gate\n"
        "approach is south. A side street leads west to the magic quarter.");
    set_exits( ([
        "north" : "/domains/Lazlo/areas/lazlo_library",
        "south" : "/domains/Lazlo/areas/lazlo_gate",
        "east" : "/domains/Lazlo/areas/lazlo_market",
        "west" : "/domains/Lazlo/areas/lazlo_magic_quarter"
    ]) );
    set_listen("default",
        "The hum of the fountain. Footsteps on stone. Quiet conversation "
        "in several languages.");
    set_smell("default",
        "Clean air, fresh water, and the faint ozone of active magic.");
    set_items( ([
        "fountain" : "A circular fountain with ley line energy running through the water. "
                      "The light pulses slowly.",
        "plaza" : "Wide, clean, well-maintained stone paving. "
                      "Not a single crack. Someone maintains this carefully.",
        "scholars" : "Researchers, mages, and students moving between buildings. "
                      "They range from human to Elf to things with no common name.",
        "library" : "The Great Library is north. Its stone facade is older than the Rifts.",
        "light" : "Threads of ley line energy running through the fountain water."
    ]) );
}

void reset() {
    ::reset();
}
