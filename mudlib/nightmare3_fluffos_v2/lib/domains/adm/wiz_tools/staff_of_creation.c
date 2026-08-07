/* /domains/adm/wiz_tools/staff_of_creation.c
   Staff of Creation - primary QCS building tool for coding wizards.
   Inherits tanstaafl_base which provides build/clone/purge commands.
   Also: review - apprentice coding queue, delegated to
   creation_review_menu_d.c (a shared, never-cloned daemon) so
   ring_of_dominion.c can drive the same review menu without
   duplicating it. */

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
        "For Coding Wizard use only. Type: build help\n"
        "Apprentice coding queue: review");
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
    add_action("cmd_review", "review");
}

int cmd_review(string str) {
    return (int)"/domains/adm/wiz_tools/creation_review_menu_d"->begin_menu(this_player());
}
