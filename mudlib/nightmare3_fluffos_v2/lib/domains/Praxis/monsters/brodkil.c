// /domains/Praxis/monsters/brodkil.c
// Brodkil demon warrior. MDC body, aggressive.

#include <std.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit MONSTER;

void create() {
    ::create();
    set_name("brodkil");
    set_id(({ "brodkil", "demon", "warrior" }));
    set_short("a brodkil demon warrior");
    set_long(
        "A hulking brodkil with red skin, bionic implants, and a cruel\n"
        "grin. Coalition intelligence classifies them as high-threat demons.");
    set_race("brodkil");
    set_level(9);
    set_exp(40000);
    set_alignment(-800);
    set_aggressive(1);
    set_stats("MDC", 100);
    set_stats("max_MDC", 100);
    set_stats("PS", 22);
    set_hp(180);
    new(EQ_PATH + "vibro_knife.c")->move(this_object());
    call_out("equip_gear", 0);
}

void equip_gear() {
    force_me("wield knife in right hand");
}
