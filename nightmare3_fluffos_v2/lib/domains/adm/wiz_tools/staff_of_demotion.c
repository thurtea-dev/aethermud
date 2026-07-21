// /domains/adm/wiz_tools/staff_of_demotion.c
// Staff of Demotion -- Admin tool for AetherMUD
// Menu-driven admin functions for archon-level staff.
// The actual menu logic lives in demotion_menu_d.c (a shared, never-
// cloned daemon) so ring_of_dominion.c can drive the same menu without
// duplicating it. This file only owns identity, wear/carry lifecycle,
// and the "demote"/"promote" verbs. apply_demote_restore() stays as a
// thin forwarder: secure/daemon/users.c calls it by this file's path
// during offline demotion (USERS_D->offline_demote()), so the public
// entry point has to keep existing here even though the logic moved.

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("staff of demotion");
    set_id( ({ "staff", "staff of demotion", "demotion staff" }) );
    set_short("a staff of demotion");
    set_long(
        "A slender obsidian staff crackling with administrative authority.\n"
        "It hums with the power to shape the lives of those who dwell in\n"
        "this world. For Admin use only. Type: demote or promote");
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
    add_action("cmd_use_staff", "demote");
    add_action("cmd_use_staff", "promote");
}

int cmd_use_staff(string str) {
    return (int)"/domains/adm/wiz_tools/demotion_menu_d"->begin_menu(this_player());
}

// Public API used by secure/daemon/users.c's offline_demote(). Keep this
// name and signature stable even though the real logic now lives in
// demotion_menu_d.c.
void apply_demote_restore(object ob) {
    "/domains/adm/wiz_tools/demotion_menu_d"->apply_demote_restore(ob);
}
