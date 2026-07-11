// /d/Praxis/areas/old_highway.c
// Old pre-Rifts highway north of the checkpoint ruins.
// Quest trigger: entering with rocky_rumor active auto-completes the objective.
// The supply crate from lost_supplies is found here.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("vehicle_accessible", 1);
    set_short("Old Highway, North of the Checkpoint");
    set_long(
        "The cracked asphalt stretches north and south between rusted guardrails\n"
        "that lean at angles, warped by years of ley line storms. Pre-Rifts\n"
        "highway signage is still bolted to a pole here. The letters are\n"
        "unreadable under layers of ash and bio-growth. A faint shimmer on the\n"
        "horizon north hints at energy discharge. Ley line country.\n\n"
        "Wedged against the guardrail on the east side is a battered supply crate,\n"
        "half-hidden under a canvas tarp. Scuff marks in the dirt suggest the\n"
        "caravan that carried it didn't make it past this point.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/test_zone",
        "north" : "/domains/Praxis/areas/highway_ruins",
        "east"  : "/domains/chitown/areas/illinois_road"
    ]) );
    set_listen("default",
        "Wind across flat ground. A distant crack of dimensional energy, like thunder without rain.");
    set_smell("default",
        "Ozone and old rubber. The faint metallic tang of a nearby ley line.");
    set_items( ([
        "guardrails" : "Steel guardrails twisted and leaning. The force that did this was not mechanical.",
        "sign" : "A highway sign still bolted to its post. Whatever it once said is gone under ash and growth.",
        "pole" : "Rusted steel highway post. Still standing after all these years.",
        "highway" : "Cracked asphalt, four lanes that once carried traffic, now empty in both directions.",
        "tarp" : "A heavy canvas tarp caught on the guardrail. A supply crate is wedged beneath it.",
        "shimmer" : "Northern ley line shimmer. Visible in daylight, which means it is very active.",
        "scuff marks": "Deep scrape marks in the asphalt. Something heavy was dragged here. Not long ago.",
        "scuff" : "Deep scrape marks in the asphalt. Something heavy was dragged here. Not long ago."
    ]) );

    new("/domains/Praxis/equipment/supply_crate")->move(this_object());
}

void init() {
    ::init();
    if(!this_player()) return;
    if(!(int)QUEST_D->is_quest_active(this_player(), "rocky_rumor")) return;
    call_out("trigger_rocky_rumor", 1, this_player());
}

void trigger_rocky_rumor(object player) {
    if(!player || !objectp(player)) return;
    if(environment(player) != this_object()) return;
    if(!(int)QUEST_D->is_quest_active(player, "rocky_rumor")) return;
    QUEST_D->update_objective(player, "rocky_rumor", "old_highway_reached", 1);
    tell_object(player,
        "You scan the empty highway. Bootprints in the ash lead north and don't come back.\n"
        "Whatever the Cyber-Knight was looking for, they went this way.\n");
    QUEST_D->complete_quest(player, "rocky_rumor");
}

void reset() {
    if(!present("supply crate", this_object()))
        new("/domains/Praxis/equipment/supply_crate")->move(this_object());
}
