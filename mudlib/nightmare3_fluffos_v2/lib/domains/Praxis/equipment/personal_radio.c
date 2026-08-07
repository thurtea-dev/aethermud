// /d/Praxis/equipment/personal_radio.c
// Standard-issue personal communicator. Default channel 14.
// Carried by headhunters, CS grunts, and wilderness scouts.

#include <std.h>

inherit "/std/rifts_radio";

void create() {
    ::create();
    set_name("personal radio");
    set_id(({ "personal radio", "radio", "com", "comms", "communicator",
              "personal communicator" }));
    set_short("a personal radio communicator");
    set_long(
        "A ruggedized civilian-grade radio communicator about the size of a large\n"
        "phone. Belt-clip on the back, stubby antenna on top. Frequency range\n"
        "1-100. Default channel is 14, the Praxis open frequency.\n\n"
        "Type 'radio on/off' to toggle, 'tune <freq>' to change channels,\n"
        "'transmit <message>' or 'radio <message>' to broadcast.");
    set_value(800);
    set_mass(150);
    set_radio_type("personal");
    set_frequency(14);
}
