// /domains/tolkeen/areas/tolkeen_clinic.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("tolkeen", 1);
    set_property("no attack", 1);
    set_property("rest_allowed", 1);
    set_short("Tolkeen, Field Clinic");
    set_long(
        "Cots, bandages, and a ley-stabilized sterilizer. Healers here treat\n"
        "militia and refugees alike. Bloodstains are scrubbed but never gone.\n\n"
        "The mage quarter is east.");
    set_exits( ([
        "east" : "/domains/tolkeen/areas/tolkeen_mage_quarter"
    ]) );
    set_listen("default", "Quiet breathing. Occasional groans.");
    set_smell("default", "Antiseptic and smoke.");
}
