/* /domains/ChiTown/monsters/burb_dog_boy.c
   CS Dog Boy on checkpoint duty - aggressive to non-humans via default. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("Dog Boy patrol");
    set_id( ({ "dog boy patrol", "dog boy", "patrol", "mutant", "coalition" }) );
    set_short("a Dog Boy patrol");
    set_long(
        "A Coalition Dog Boy in light armor, snout working the air. A laser\n"
        "pistol rides the hip. The checkpoint is a reminder that the wall\n"
        "has a long nose.");

    set_race("dog boy");
    set_class("fighter");
    set_level(6);
    set_exp(20000);
    set_alignment(-100);
    set_aggressive(0);
    set_gender("male");
    set_body_type("human");

    set_stats("IQ", 10);
    set_stats("ME", 12);
    set_stats("MA", 8);
    set_stats("PS", 16);
    set_stats("PP", 15);
    set_stats("PE", 15);
    set_stats("PB", 8);
    set_stats("Spd", 18);

    set_max_hp(50);
    set_hp(50);
    set_stats("SDC", 70);
    set_stats("max_SDC", 70);
    set_stats("is_MDC", 0);
    set_stats("rifts_hp", 50);
    set_stats("max_rifts_hp", 50);
}
