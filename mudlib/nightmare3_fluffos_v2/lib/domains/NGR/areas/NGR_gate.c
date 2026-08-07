/* /domains/NGR/areas/NGR_gate.c
   New German Republic - Main Gate.
   Entry point from the west road. Heavy guard presence. */

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/NGR/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("ngr_territory", 1);
    set_short("NGR Main Gate");
    set_long(
        "The main gate of the New German Republic is a monument to engineering\n"
        "discipline. Massive ferroconcrete pylons support an arch wide enough\n"
        "for power armor to pass four abreast. The gate itself is reinforced\n"
        "titanium-alloy plate, currently open but clearly capable of sealing\n"
        "within seconds. Automated rail gun emplacements watch every approach.\n\n"
        "Guards in grey NGR power armor stand at attention to either side of\n"
        "the gate. Their helmets are mirrored. You cannot see their eyes.\n"
        "An identification checkpoint station stands to the right: a desk,\n"
        "a terminal, and a guard who looks like he has never smiled.\n\n"
        "The road west leads back toward the European wilderness. Through the\n"
        "gate to the east is the NGR market district.");
    set_exits( ([
        "west"  : "/domains/NewCamelot/areas/europe_road",
        "east"  : "/domains/NGR/areas/NGR_market_district",
        "north" : "/domains/NGR/areas/ngr_checkpoint"
    ]) );
    set_listen("default",
        "The hum of power armor servos. The creak of the gate. Wind from the west\n"
        "carrying the smell of old forest.");
    set_smell("default",
        "Ozone from the rail gun emplacements. Machine oil. Clean air.\n"
        "The sterile smell of military discipline.");
    set_items( ([
        "gate"    : "Reinforced titanium-alloy plate, thick as your arm. "
                    "Currently open. The gate motor housing is visible above: "
                    "it could seal this arch in four seconds.",
        "pylons"  : "Ferroconcrete, reinforced with MDC polymer. "
                    "The NGR does not build anything it does not intend to last.",
        "guards"  : "NGR gate guards in full grey power armor. "
                    "They are watching you. They are always watching.",
        "checkpoint" : "An identification station. Documents required for entry. "
                    "Non-humans are processed separately, at the station to the left.",
        "emplacements" : "Rail gun turrets, automated, tracking every approach. "
                    "Capable of putting a hole through a Glitter Boy."
    ]) );
}

void reset() {
    ::reset();
    if(!present("ngr gate guard", this_object()))
        clone_object(MONSTER_PATH + "ngr_gate_guard.c")->move(this_object());
}
