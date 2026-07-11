// /domains/Praxis/areas/nexus_dimensional_tear.c
// Unstable tear in the ley line nexus. Gateway to the demon-plane sliver.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("ley_line_nexus", 1);
    set_property("ley_line_room", 1);
    set_property("is_ley_line", 1);
    set_property("nexus", 1);
    set_short("Ley Nexus, Dimensional Tear");
    set_long(
        "The ley line here has torn a hole in reality. A vertical wound of\n"
        "violet-black energy hangs above the scorched highway, roughly two\n"
        "meters wide and tall enough to walk through. The air on this side\n"
        "still crackles with blue-white power, but the tear smells of sulfur\n"
        "and hot metal. A field researcher has set up a sensor tripod beside\n"
        "the crossing, taking readings with grim focus.\n\n"
        "The stable nexus crossing lies up. The tear itself leads somewhere\n"
        "that is not Rifts Earth.");
    set_exits( ([
        "up" : "/domains/Praxis/areas/nexus_approach",
        "in" : "/domains/demon_plane/areas/demon_sliver_gate"
    ]) );
    set_listen("default",
        "A deep subsonic roar from inside the tear, layered over the ley line hum.");
    set_smell("default", "Ozone mixed with sulfur and something like burnt blood.");
    set_items( ([
        "tear" : "A vertical wound of violet-black energy. You could step through.",
        "rift" : "Same as the tear: an open wound between dimensions.",
        "sensor" : "A tripod-mounted sensor array. Its display flickers with unreadable numbers.",
        "researcher" : "A ley line researcher taking readings. Ask about work or the tear."
    ]) );
}

void reset() {
    ::reset();
    if(!present("ley researcher", this_object()))
        new("/domains/demon_plane/npcs/ley_researcher")->move(this_object());
}

void init() {
    ::init();
    if(!this_player()) return;
    if(!(int)QUEST_D->is_quest_active(this_player(), "chi_town_rift_report")) return;
    call_out("trigger_rift_report", 1, this_player());
}

void trigger_rift_report(object player) {
    if(!player || !objectp(player)) return;
    if(environment(player) != this_object()) return;
    if(!(int)QUEST_D->is_quest_active(player, "chi_town_rift_report")) return;
    if((int)QUEST_D->is_objective_complete(player, "chi_town_rift_report",
            "tear_located")) return;
    QUEST_D->update_objective(player, "chi_town_rift_report", "tear_located", 1);
    tell_object(player,
        "You locate the dimensional tear the CS liaison described. "
        "Report back to Chi-Town when ready.\n");
}
