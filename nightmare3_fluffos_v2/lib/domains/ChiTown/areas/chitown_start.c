// /domains/ChiTown/areas/chitown_start.c
// Waystation between Praxis and Chi-Town. Americas starting room for new
// characters (2026-07-21) -- indoors, bridging the existing rifts_welcome
// arrival clearing (south, unchanged) and the Chi-Town city gate (north).
// Carries the chargen guide + survivor + orientation sign that used to
// live in rifts_welcome.c; rifts_welcome.c is untouched and keeps its own
// copies, since it must keep working as Atlantis's chargen fallback and
// the mudwide death-respawn default (see std/user.c).

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Waystation, Praxis-Chi-Town Corridor");
    set_long(
        "A reinforced shelter of scavenged plating and pre-Rifts ferrocrete,\n"
        "built into the corridor between the old Coalition road south to\n"
        "Praxis and the checkpoints of Chi-Town to the north. A steady\n"
        "trickle of new arrivals passes through here: refugees, mercs, and\n"
        "the newly rifted-in alike.\n\n"
        "A battered sign is nailed to a support beam near the door. A\n"
        "survivor watches the corridor from a folding chair.");
    set_exits( ([
        "south" : "/domains/Praxis/rifts_welcome",
        "north" : "/domains/ChiTown/areas/chitown_gate"
    ]) );
    set_listen("default",
        "Wind whistling through gaps in the plating. Distant traffic noise from both directions.");
    set_smell("default",
        "Machine oil, recycled air, and the ozone tang of a nearby ley line.");
    set_items( ([
        "sign" :
            "A hand-painted sign reads:\n\n"
            " WELCOME TO RIFTS EARTH\n\n"
            " Basic commands:\n"
            " look examine your surroundings\n"
            " inventory see your gear\n"
            " score your character stats\n"
            " who other players online\n"
            " say <text> speak aloud\n"
            " tell <name> <text> private message\n"
            " help <topic> in-game help files\n"
            " north/south move\n"
            " kill <target> attack (caution: lethal world)\n"
            " quests track jobs you accept\n\n"
            " Chi-Town lies to the NORTH. Praxis lies to the SOUTH.\n"
            " Talk to the survivor nearby for more guidance.\n",
          "plating" : "Scavenged plating and ferrocrete, bolted together in a hurry.",
          "beam" : "A support beam, also serving as a bulletin board.",
          "chair" : "A folding chair. Someone has been keeping watch here a while.",
          "survivor" : "She is watching the corridor. Try <say help> to ask for guidance."
        ]) );
}

void reset() {
    ::reset();
    if(!present("survivor", this_object()))
        new("/domains/Praxis/obj/mon/rift_survivor")->move(this_object());
    if(!present("gamemaster", this_object()))
        clone_object("/domains/Praxis/npcs/chargen_guide")->move(this_object());
}
