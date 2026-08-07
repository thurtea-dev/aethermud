// /domains/adm/wiz_tools/staff_of_dominion.c
// Staff of Dominion -- Domain Wizard tool for AetherMUD
// Menu-driven domain management for domain-level staff.
// The actual menu logic lives in dominion_menu_d.c (a shared, never-
// cloned daemon) so ring_of_dominion.c can drive the same menu without
// duplicating it. This file only owns identity, wear/carry lifecycle,
// and the "domain" verb.

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("staff of dominion");
    set_id( ({ "staff", "staff of dominion", "dominion staff",
               "staff of domains", "domains staff", "domain staff" }) );
    set_short("a staff of dominion");
    set_long(
        "A gleaming crystalline staff pulsing with territorial authority.\n"
        "It grants its wielder power over the domains of the world.\n"
        "For Domain and Admin wizards. Type: domain");
    set_mass(500);
    set_value(0);
    set_property("no_drop", 1);
    set_property("no_give", 1);
}

mixed *query_auto_load() {
    return ({ base_name(this_object()), ({}) });
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_use_staff", "domain");
}

int cmd_use_staff(string str) {
    return (int)"/domains/adm/wiz_tools/dominion_menu_d"->begin_menu(this_player());
}
