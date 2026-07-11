// /domains/demon_plane/areas/demon_sliver_gate.c
// Entry to the demon-plane sliver from the Praxis ley tear.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("demon_plane", 1);
    set_property("off_world", 1);
    set_short("Demon-Plane Sliver, Rift Mouth");
    set_long(
        "You step out of the tear onto cracked basalt that was never part of\n"
        "Earth. The sky is a bruised purple streaked with green lightning.\n"
        "The ley scar behind you still flickers, your only obvious way home.\n"
        "Heat rolls off the stone. Something watches from the wastes ahead.");
    set_exits( ([
        "out" : "/domains/Praxis/areas/nexus_dimensional_tear",
        "north" : "/domains/demon_plane/areas/demon_sliver_wastes",
        "east" : "/domains/demon_plane/areas/demon_sliver_scar"
    ]) );
    set_listen("default", "Distant howls and the crackle of green lightning.");
    set_smell("default", "Sulfur, hot stone, and a copper tang in the air.");
}

void reset() {
    ::reset();
    if(!present("demon sliver stalker", this_object()))
        new("/domains/demon_plane/monsters/demon_sliver_stalker")->move(this_object());
}

void init() {
    ::init();
    if(!this_player()) return;
    if(!(int)QUEST_D->is_quest_active(this_player(), "chi_town_rift_report")) return;
    call_out("trigger_rift_entered", 1, this_player());
}

void trigger_rift_entered(object player) {
    if(!player || !objectp(player)) return;
    if(environment(player) != this_object()) return;
    if(!(int)QUEST_D->is_quest_active(player, "chi_town_rift_report")) return;
    if((int)QUEST_D->is_objective_complete(player, "chi_town_rift_report",
            "demon_sliver_entered")) return;
    QUEST_D->update_objective(player, "chi_town_rift_report",
        "demon_sliver_entered", 1);
    tell_object(player,
        "You confirm the CS liaison's worst fear: a live demon-plane sliver "
        "opens on the Praxis ley line. Get proof home.\n");
}
