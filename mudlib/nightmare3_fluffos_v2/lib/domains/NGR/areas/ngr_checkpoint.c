/* /domains/NGR/areas/ngr_checkpoint.c
   NGR border checkpoint blockhouse north of the main gate.
   Spawns ngr_border_guard on reset. */

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/NGR/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("ngr_territory", 1);
    set_short("NGR Border Checkpoint");
    set_long(
        "A reinforced checkpoint blockhouse sits across the road. NGR soldiers\n"
        "in Triax-pattern armor check identity papers and scan for dimensional\n"
        "contamination. The border between NGR territory and the wilderness is\n"
        "enforced here with lethal authority.\n\n"
        "A terminal kiosk stands to one side for document registration. Above\n"
        "the blockhouse, a sensor array sweeps the approach road in steady\n"
        "arcs. Rail gun emplacements cover both the road north and the gate\n"
        "approach south. There is no casual way through this post.\n"
    );
    set_exits( ([
        "south" : "/domains/NGR/areas/NGR_gate",
        "east"  : "/domains/NGR/areas/ngr_patrol_road"
    ]) );
    set_listen("default",
        "The hum of the sensor array overhead. A document scanner beeping\n"
        "at intervals. The click of a rail gun safety being checked.");
    set_smell("default",
        "Machine oil, the ozone of electronics, and the sterile antiseptic\n"
        "used on the document scanners between each traveler.");
    set_items( ([
        "blockhouse" : "Reinforced ferroconcrete. Small windows with one-way glass.\n"
                       "The door is heavy gauge titanium-alloy, currently ajar.",
        "terminal"   : "A document registration kiosk. Scan your identification here.\n"
                       "All visitors must register. NGR policy is not negotiable.",
        "sensor"     : "A rotating sensor array on the blockhouse roof. It sweeps\n"
                       "for dimensional rift signatures, unauthorized weapons, and\n"
                       "biometric flags. It has already scanned you.",
        "emplacements" : "Automated rail gun emplacements covering both road approaches.\n"
                         "They are slaved to the sensor array. Response time: instant."
    ]) );
}

void reset() {
    ::reset();
    if(!present("NGR border guard", this_object()))
        clone_object(NPC_PATH + "ngr_border_guard")->move(this_object());
}
