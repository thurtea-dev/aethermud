/* /d/Praxis/vehicles/mountaineer/mtn_cab.c
   NG-V7 Mountaineer ATV: driver's cab. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("vehicle_interior", 1);
    set_short("NG-V7 Mountaineer: Driver's Cab");
    set_long(
        "The driver's cab of the Mountaineer. Two suspension seats face a\n"
        "wraparound windshield of thick, yellowed laminate. The dashboard\n"
        "is classic Northern Gun: rugged analog gauges, oversized switches\n"
        "built for gloved hands, and a radio rig bolted under the console.\n"
        "The ignition slot is empty; someone has taken the key.\n\n"
        "The main cabin is aft (south).");
    set_exits( ([
        "south" : "/domains/Praxis/vehicles/mountaineer/mtn_cabin"
    ]) );
    set_listen("default",
        "Faint static from the radio rig, left switched on.");
    set_smell("default",
        "Sun-baked vinyl and dust.");
    set_items( ([
        "seats" : "Suspension seats designed to soak up cross-country abuse.",
        "windshield" : "Laminated MDC-rated glass, pitted by years of grit.",
        "dashboard" : "Analog gauges: fuel, oil pressure, hull integrity.\n"
                      "All needles rest at zero with the engine off.",
        "radio" : "A vehicle radio rig. It hisses faint static on the local band.",
        "ignition" : "An empty key slot. Without the key, the Mountaineer is\n"
                     "going nowhere."
    ]) );
}

void reset() {
    ::reset();
}
