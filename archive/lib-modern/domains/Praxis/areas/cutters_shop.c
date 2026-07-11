// /d/Praxis/areas/cutters_shop.c
// Cutter's black market shop, accessed via the back alley.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Cutter's, Black Market");
    set_long(
        "A low-ceilinged basement room converted into a shop by someone who\n"
        "clearly never bothered with permits. Wooden shelves run along three\n"
        "walls, packed with salvaged tech, unmarked crates, and equipment that\n"
        "probably still has CS serial numbers filed off. A bare bulb hangs\n"
        "from exposed wiring overhead, giving everything a yellow cast.\n\n"
        "The door back to the alley is east.");
    set_exits( ([
        "east" : "/domains/Praxis/areas/back_alley"
    ]) );
    set_listen("default",
        "The muffled noise of Market Street above, and the occasional creak of the building.");
    set_smell("default",
        "Machine oil, solder flux, and the faint metallic smell of energy cells.");
    set_items( ([
        "shelves" : "Packed with unmarked equipment. Most of it looks functional.",
        "crates" : "Heavy wooden crates stencilled with codes that don't mean anything standard.",
        "counter" : "A thick slab of reinforced composite. Bullet-scored on the customer side.",
        "bulb" : "A single bare bulb on exposed wiring. It flickers occasionally.",
        "wiring" : "Rough electrical work. Functional, probably.",
        "equipment": "Salvaged Coalition and Splugorth tech, stripped of identifying marks."
    ]) );
}

void reset() {
    ::reset();
    if(!present("cutter", this_object()))
        clone_object("/domains/Praxis/monsters/cutter")->move(this_object());
}
