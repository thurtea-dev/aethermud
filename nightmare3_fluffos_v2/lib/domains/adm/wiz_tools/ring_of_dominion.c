/* /domains/adm/wiz_tools/ring_of_dominion.c
   A plain gold ring that stands in for the full admin wiz-tool set.
   While worn by an admin (admin_wizp), it clones and hands over the
   same five tools /secure/daemon/wiztools.c gives a head arch/arch:
   staff_of_demotion, staff_of_dominion, staff_of_creation, rp_skill_tool,
   and tattoo_gun. Removing the ring strips only the copies it granted;
   tools the wearer already had before putting the ring on are left
   alone. Also answers askring <question> with a canned admin Q&A
   lookup while worn. No_drop/no_give: this is a one-off personal item,
   not something meant to circulate. */

#include <std.h>

inherit ARMOUR;

#define TOOL_DIR "/domains/adm/wiz_tools/"

private object find_carried_tool(object player, string name);
int on_ring_worn();
int on_ring_removed();
int cmd_askring(string str);
private string ring_answer(string question);

private string *tool_names = ({
    "staff_of_demotion", "staff_of_dominion", "staff_of_creation",
    "rp_skill_tool", "tattoo_gun"
});

void create() {
    ::create();
    set_name("gold ring");
    set_id( ({ "ring", "gold ring", "plain gold ring", "ring of dominion" }) );
    set_short("a plain gold ring");
    set_long(
        "A plain, unmarked band of gold. There is nothing about it that\n"
        "looks unusual, until it is worn.\n"
    );
    set_type("ring");
    set_property("rifts_slot", "admin_ring");
    set_ac(0);
    set_mass(1);
    set_value(0);
    set_property("no_drop", 1);
    set_property("no_give", 1);
    set_wear( (: this_object(), "on_ring_worn" :) );
    set_remove( (: this_object(), "on_ring_removed" :) );
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_askring", "askring");
}

private object find_carried_tool(object player, string name) {
    object *inv;
    int i;

    inv = all_inventory(player);
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i]) continue;
        if(strsrch(base_name(inv[i]), name) != -1) return inv[i];
    }
    return 0;
}

int on_ring_worn() {
    object player;
    object existing;
    object tool;
    int i;
    int granted;

    player = this_player();
    if(!admin_wizp(player)) {
        write("The ring stays cold and inert on your hand.\n");
        return 0;
    }
    granted = 0;
    for(i = 0; i < sizeof(tool_names); i++) {
        existing = find_carried_tool(player, tool_names[i]);
        if(existing) continue;
        tool = clone_object(TOOL_DIR + tool_names[i]);
        if(!tool) continue;
        tool->move(player);
        tool->set_property("ring_granted", 1);
        granted++;
    }
    if(granted)
        write("You slide the gold ring onto your finger. It warms against\n"
              "your skin, and the tools of your office take shape in your\n"
              "hands.\n");
    else
        write("You slide the gold ring onto your finger. It warms against\n"
              "your skin.\n");
    return 1;
}

int on_ring_removed() {
    object player;
    object *inv;
    int i;
    int stripped;

    player = query_worn();
    if(!player) player = this_player();
    inv = all_inventory(player);
    stripped = 0;
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i]) continue;
        if(!(int)inv[i]->query_property("ring_granted")) continue;
        inv[i]->remove();
        if(objectp(inv[i])) destruct(inv[i]);
        stripped++;
    }
    if(stripped)
        tell_object(player,
            "The ring cools, and the tools it granted fade from your\n"
            "hands.\n");
    return 1;
}

int cmd_askring(string str) {
    if(query_worn() != this_player()) {
        write("The ring is silent. It only answers while worn.\n");
        return 1;
    }
    if(!str || !sizeof(str)) {
        write("Ask it what? Try: askring <question>\n");
        return 1;
    }
    write(ring_answer(lower_case(str)));
    return 1;
}

private string ring_answer(string question) {
    if(strsrch(question, "promote") != -1 || strsrch(question, "demote") != -1)
        return "The staff of demotion handles this: type promote or demote\n"
               "to open its menu (promote/demote/setlevel/setrace/setocc/\n"
               "setstats/title/goto/summon/boot, plus daemon reload and\n"
               "shutdown). It requires admin_wizp.\n";
    if(strsrch(question, "build") != -1 || strsrch(question, "qcs") != -1 ||
       strsrch(question, "clone") != -1 || strsrch(question, "purge") != -1)
        return "The staff of creation handles this: build <verb> ... for QCS\n"
               "building (same as the qcs command), clone <path> to clone a\n"
               "file into your inventory, purge <id> to remove a non-living\n"
               "object from the room. review opens the apprentice coding\n"
               "queue.\n";
    if(strsrch(question, "domain") != -1)
        return "The staff of dominion handles this: type domain to create\n"
               "or list domains, set a description, reset loaded rooms, and\n"
               "review pending RP/domain apprentice submissions.\n";
    if(strsrch(question, "tattoo") != -1 || strsrch(question, "inscribe") != -1)
        return "The tattoo gun handles this: type inscribe to select a\n"
               "player and grant or remove an Atlantean/Tattooed Man tattoo.\n";
    if(strsrch(question, "skill") != -1 || strsrch(question, "language") != -1 ||
       strsrch(question, "rptool") != -1)
        return "The RP skill tool handles this: type tool or rptool to grant\n"
               "or remove a player's skill or language percentage, or view\n"
               "the skill request queue.\n";
    if(strsrch(question, "reboot") != -1 || strsrch(question, "warmboot") != -1)
        return "Warmboot reloads daemon and command files, but never\n"
               "std/user.c, living.c, room.c, armour.c, or combat.c for\n"
               "players already connected. For those, stop and start the\n"
               "driver from the shell: ./mud.sh stop && ./mud.sh start\n";
    if(strsrch(question, "save") != -1)
        return "Player saves live at /secure/save/users/<first letter>/\n"
               "<name>.o, never at /save/players/.\n";
    return "I don't have an answer for that. Try asking about: promote,\n"
           "demote, build, clone, purge, domain, tattoo, skill, warmboot,\n"
           "or save.\n";
}
