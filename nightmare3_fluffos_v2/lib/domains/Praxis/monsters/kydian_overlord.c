/* /d/Praxis/monsters/kydian_overlord.c
   Kydian Overlord -- elite Splugorth enforcer.
   High MDC, psionic blast, commands lesser minions. Rare, dangerous. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    int mdc;

    ::create();

    mdc = (random(6)+1 + random(6)+1 + random(6)+1 + random(6)+1) * 10 + 80;

    set_name("Kydian Overlord");
    set_id( ({ "kydian overlord", "kydian", "overlord", "commander", "creature" }) );
    set_short("a Kydian Overlord");
    set_long(
        "The Kydian Overlord is massive: eight feet of dense bio-engineered muscle\n"
        "covered in layered natural armor plating that glistens like wet obsidian.\n"
        "Four upper limbs, each ending in bladed talons. It moves with the\n"
        "unhurried confidence of something that has never lost a fight it chose\n"
        "to have. A faint psionic pressure emanates from it -- not pain yet,\n"
        "just the awareness that it is reaching into your head.\n\n"
        "It serves the Splugorth directly and answers to no lesser authority.\n"
        "It has assessed you and found you to be a manageable problem.");

    set_race("kydian");
    set_class("monster");
    set_level(12);
    set_exp(100000);
    set_alignment(-700);
    set_aggressive(1);

    set_stats("IQ", 14);
    set_stats("ME", 18);
    set_stats("MA", 8);
    set_stats("PS", 28);
    set_stats("PP", 20);
    set_stats("PE", 22);
    set_stats("PB", 5);
    set_stats("Spd", 18);

    set_stats("MDC", mdc);
    set_stats("max_MDC", mdc);
    set_stats("rifts_hp", 80);
    set_stats("max_rifts_hp", 80);
    set_stats("SDC", 0);
    set_stats("max_SDC", 0);
    set_stats("ISP", 100);
    set_stats("max_ISP", 100);
    set_stats("is_MDC", 1);

    setenv("rifts_race", "kydian");
    setenv("rifts_flags", "psionic");

    set_hp(mdc + 80);
    set_mp(100);
    set_sp(100);

    set_property("mdc_creature", 1);
    set_property("AR", 15);
    set_property("psionic", 1);
    set_property("rifts_psionics", ({ "mind bolt", "pyrokinesis bolt" }));
    set_property("rifts_no_flee", 1);
}
