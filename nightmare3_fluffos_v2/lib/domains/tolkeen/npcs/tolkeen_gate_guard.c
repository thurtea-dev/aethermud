// /domains/tolkeen/npcs/tolkeen_gate_guard.c

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Tolkeen gate guard");
    set_npc_name("Gate Guard");
    set_id( ({ "tolkeen gate guard", "gate guard", "guard", "militia" }) );
    set_short("a Tolkeen gate guard");
    set_long(
        "A militia guard in patchwork MDC armor etched with ward-runes.\n"
        "A techno-wizard charm hangs at the throat. Eyes stay on the road\n"
        "south, watching for Coalition gray.");
    set_level(6);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(200);
    set_body_type("human");

    set_stats("MDC", 90);
    set_stats("max_MDC", 90);
    set_property("mdc_creature", 1);

    add_response("hello",
        "Guard: 'Welcome to Tolkeen - if you are not Coalition. Keep your weapons peace-tied in the square.'");
    add_response("hi",
        "Guard: 'Welcome to Tolkeen - if you are not Coalition. Keep your weapons peace-tied in the square.'");
    add_response("coalition",
        "Guard spatters the gate stone. 'They want this city burned. We disagree.'");
    add_response("cs",
        "Guard spatters the gate stone. 'They want this city burned. We disagree.'");
    set_default_response("The guard watches the southern approach.");
}
