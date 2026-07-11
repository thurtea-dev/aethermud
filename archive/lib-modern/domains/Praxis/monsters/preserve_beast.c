// /d/Praxis/monsters/preserve_beast.c
// Dimensional predator maintained in the Splynn Preserves.
// MDC creature from another dimension, aggressive, hunts intruders.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();

    set_name("preserve beast");
    set_id( ({ "preserve beast", "beast", "predator", "creature", "dimensional beast" }) );
    set_short("a preserve beast");
    set_long(
        "Something that evolved to hunt in an environment nothing like Earth.\n"
        "Six-limbed, low to the ground, with two sets of forward-facing eyes\n"
        "on a wedge-shaped head. The hide is naturally camouflaged: mottled\n"
        "grey-green that shifts as it moves. The claws on the forward limbs\n"
        "are each about thirty centimeters and gleam like polished carbon.\n\n"
        "It has been studying you since before you noticed it. The way it\n"
        "holds itself suggests it has decided you are prey.");

    set_race("dimensional beast");
    set_class("predator");
    set_level(6);
    set_exp(25000);
    set_alignment(-300);
    set_aggressive(1);

    set_stats("IQ", 4);
    set_stats("ME", 8);
    set_stats("MA", 5);
    set_stats("PS", 22);
    set_stats("PP", 19);
    set_stats("PE", 18);
    set_stats("PB", 4);
    set_stats("Spd", 22);

    set_stats("rifts_hp", 30);
    set_stats("max_rifts_hp", 30);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("MDC", 60);
    set_stats("max_MDC", 60);
    set_stats("is_MDC", 1);

    setenv("rifts_flags", "mdc_being");

    set_money("credits", 0);

    set_hp(100);
    set_mp(0);
    set_sp(80);

    set_property("damage_num", 2);
    set_property("damage_sides", 8);
    set_property("damage_bonus", 4);
    set_property("damage_string", "2d8+4 MDC (claws)");
    set_property("mdc_weapon", 1);
    set_property("is_MDC", 1);
    set_property("no_steal", 1);

    catch(clone_object("/domains/Praxis/equipment/splynn/alien_hide.c")->move(this_object()));
}
