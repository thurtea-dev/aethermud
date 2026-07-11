/* /domains/newcamelot/npcs/dragon_knight.c
   a Dragon Knight of New Camelot. MDC creature. */

#include <std.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("dragon knight");
    set_id( ({ "dragon knight", "knight" }) );
    set_short("a Dragon Knight of New Camelot");
    set_long(
        "A knight of the Order in full mega-damage plate, the white dragon\n"
        "worked across the breastplate. Dragon Knights are sworn kin to the\n"
        "dragons of the realm and carry a measure of that fire in them. The\n"
        "knight moves like a closing portcullis: smooth, heavy, final.");
    set_race("human");
    set_level(12);
    set_exp(60000);
    set_alignment(800);
    set_aggressive(0);
    set_stats("MDC", 180);
    set_stats("max_MDC", 180);
    set_stats("PS", 26);
    set_hp(180);
    set_property("mdc_creature", 1);
}
