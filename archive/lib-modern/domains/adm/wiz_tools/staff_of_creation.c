/* /domains/adm/wiz_tools/staff_of_creation.c
   Staff of Creation - primary QCS building tool for coding wizards.
   Inherits tanstaafl_base which provides build/clone/purge commands.
   To create other tanstaafl-enabled objects, inherit tanstaafl_base
   and include "tanstaafl" in your set_id() call. */

#include <std.h>

inherit "/domains/adm/wiz_tools/tanstaafl_base";

void create() {
    ::create();
    set_name("staff of creation");
    set_id(({ "staff", "staff of creation", "creation staff", "tanstaafl" }));
    set_short("a staff of creation");
    set_long(
        "A luminous white staff humming with creative potential. It allows\n"
        "its wielder to bring new rooms, monsters, and objects into being.\n"
        "For Coding Wizard use only. Type: build help");
    set_mass(500);
    set_value(0);
    set_property("no_drop", 1);
    set_property("no_give", 1);
    set_invis(1);
}

mixed *query_auto_load() {
    return ({ base_name(this_object()), ({}) });
}
