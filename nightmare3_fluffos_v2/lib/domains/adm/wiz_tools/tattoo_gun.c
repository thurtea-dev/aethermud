// /domains/adm/wiz_tools/tattoo_gun.c
// Tattoo Gun -- RP Wizard tool for Atlantean/Tattooed Man tattoo management.
// Uses the current location-based TATTOO_D API.
// Usage: use gun / use tattoo gun
// The actual menu logic lives in tattoo_menu_d.c (a shared, never-cloned
// daemon) so ring_of_dominion.c can drive the same menu without
// duplicating it. This file only owns identity, wear/carry lifecycle,
// and the "inscribe" verb.

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("tattoo-gun");
    set_id( ({ "tattoo-gun", "tattoo gun", "gun", "tattoo" }) );
    set_short("a tattoo-gun");
    set_long(
        "A high-precision Atlantean tattoo device. It uses refined PPE to\n"
        "inscribe magical tattoos that grant real powers to True Atlanteans\n"
        "and Tattooed Men. For RP Wizard use only.\n"
        "Type: inscribe");
    set_mass(200);
    set_value(0);
    set_property("no_give", 1);
}

mixed *query_auto_load() {
    return ({ base_name(this_object()), ({}) });
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_use_gun", "inscribe");
}

int cmd_use_gun(string str) {
    return (int)"/domains/adm/wiz_tools/tattoo_menu_d"->begin_menu(this_player());
}
