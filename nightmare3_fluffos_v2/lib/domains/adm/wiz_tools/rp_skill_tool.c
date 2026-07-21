// /domains/adm/wiz_tools/rp_skill_tool.c
// RP Wizard Skill Tool -- grants/removes skills and languages for players.
// Carried in inventory. Usable by creatorp() rank (rp_wiz and above).
// The actual menu logic lives in rp_skill_menu_d.c (a shared, never-
// cloned daemon) so ring_of_dominion.c can drive the same menu without
// duplicating it. This file only owns identity, wear/carry lifecycle,
// and the "tool"/"rptool" verbs.

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("rp skill tool");
    set_id( ({ "rp skill tool", "skill tool", "rp_skill_tool",
               "rp-wiz skill tool", "tool", "rp tool" }) );
    set_short("an RP-Wiz skill tool");
    set_long(
        "A sleek silver datapad imprinted with skill management software.\n"
        "Allows RP Wizards to grant, view, and remove skills and languages\n"
        "from players. For RP Wizard use only. Type: tool or rptool");
    set_mass(100);
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
    add_action("cmd_use_tool", "tool");
    add_action("cmd_use_tool", "rptool");
}

int cmd_use_tool(string str) {
    return (int)"/domains/adm/wiz_tools/rp_skill_menu_d"->begin_menu(this_player());
}
