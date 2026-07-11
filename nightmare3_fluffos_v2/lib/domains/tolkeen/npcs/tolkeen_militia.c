// /domains/tolkeen/npcs/tolkeen_militia.c

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Tolkeen militia");
    set_npc_name("Militia");
    set_id( ({ "tolkeen militia", "militia", "soldier", "guard" }) );
    set_short("a Tolkeen militia fighter");
    set_long(
        "A Tolkeen fighter in mismatched armor plates and a ley-charged\n"
        "rifle. Tired. Still standing.");
    set_level(5);
    set_max_hp(70);
    set_hp(70);
    set_aggressive(0);
    set_moving(0);
    set_race("human");
    set_alignment(150);
    set_body_type("human");

    set_stats("MDC", 70);
    set_stats("max_MDC", 70);
    set_property("mdc_creature", 1);

    add_response("hello", "Militia: 'Wall shifts rotate every six hours. You volunteering?'");
    set_default_response("The militia fighter checks a ward charm and moves on.");
}
