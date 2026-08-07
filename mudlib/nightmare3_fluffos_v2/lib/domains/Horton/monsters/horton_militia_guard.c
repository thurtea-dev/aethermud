/* /domains/Horton/monsters/horton_militia_guard.c
   Horton militia volunteer at the outskirts watchtower. Level 3, SDC 40. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("Horton militia guard");
    set_id( ({ "horton militia guard", "militia guard", "guard", "volunteer",
               "militia volunteer", "horton militia volunteer" }) );
    set_short("a Horton militia volunteer");
    set_long(
        "A local volunteer in mismatched body armor and a scavenged rifle.\n"
        "Not Coalition issue: patchwork plates, duct tape, and stubborn\n"
        "determination. They watch the treeline with the practiced stillness\n"
        "of someone who has seen what comes out of the forest at night.\n"
    );

    set_race("human");
    set_class("militia");
    set_level(3);
    set_alignment(100);
    set_aggressive(0);
    set_gender("neutral");
    set_body_type("humanoid");

    set_stats("IQ", 10);
    set_stats("ME", 10);
    set_stats("MA", 9);
    set_stats("PS", 12);
    set_stats("PP", 12);
    set_stats("PE", 12);
    set_stats("PB", 8);
    set_stats("Spd", 14);

    set_max_hp(25);
    set_hp(25);
    set_stats("SDC", 40);
    set_stats("max_SDC", 40);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 25);
    set_stats("max_rifts_hp", 25);
}
