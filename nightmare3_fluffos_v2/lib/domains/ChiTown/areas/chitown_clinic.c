// /domains/ChiTown/areas/chitown_clinic.c
// Coalition medical clinic.

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/ChiTown/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Chi-Town, Medical Clinic");
    set_long(
        "A clean efficient medical facility. Bright lights and the smell of\n"
        "antiseptic. Beds are lined in neat rows, each with monitoring\n"
        "equipment. A Coalition medical insignia is mounted above the\n"
        "reception desk.\n\n"
        "A doctor in a white coat moves between stations. Say 'heal me' to\n"
        "receive treatment.");
    set_exits( ([
        "east" : "/domains/ChiTown/areas/chitown_boulevard",
        "west" : "/domains/ChiTown/areas/chitown_burbs"
    ]) );
    set_listen("default",
        "Medical monitors beeping. The quiet efficiency of trained personnel.");
    set_smell("default",
        "Strong antiseptic, medical-grade clean air, and faint ozone.");
    set_items( ([
        "beds" : "Medical beds with monitoring equipment attached.",
        "monitors" : "Vital signs displays. Everything is working.",
        "desk" : "Reception desk. A clipboard with patient forms.",
        "insignia" : "Coalition medical corps insignia. This is an official facility."
    ]) );
}

void reset() {
    ::reset();
    if(!present("chitown doctor", this_object()))
        clone_object(NPC_PATH+"chitown_doctor")->move(this_object());
}
