// /std/obj/dim_pocket_bag.c
// Extradimensional storage from the dimensional pocket spell.

#include <std.h>

inherit "/std/storage";

void create() {
    ::create();
    set_name("dimensional pocket");
    set_id(({ "dimensional pocket", "dim pocket", "pocket storage" }));
    set_short("");
    set_long("");
    set_mass(0);
    set_value(0);
    set_max_encumbrance(500000);
    set_property("dim_pocket_bag", 1);
    set_property("no_get", 1);
}

int id(string str) { return 0; }

int query_auto_load() { return 0; }

int allow_get(object who) { return 0; }

int allow_drop(object who) { return 0; }
