// /d/Praxis/equipment/military_radio.c
// Military-grade radio with scan capability. Default channel 1 (CS military net).

#include <std.h>

inherit "/std/rifts_radio";

void create() {
    ::create();
    set_name("military radio");
    set_id(({ "military radio", "radio", "mil radio", "com", "comms",
              "field radio", "military communicator" }));
    set_short("a military field radio");
    set_long(
        "A heavy-duty Coalition States field communicator. Black polymer housing\n"
        "with the CS skull emblem on the back. Encrypted channel capability,\n"
        "extended range, and a frequency scanner. Default channel is 1,\n"
        "the CS military coordination frequency.\n\n"
        "Type 'radio on/off' to toggle, 'tune <freq>' to change channels,\n"
        "'radio scan' to scan active frequencies, 'transmit <message>' to broadcast.");
    set_value(3500);
    set_mass(350);
    set_radio_type("military");
    set_frequency(1);
}
