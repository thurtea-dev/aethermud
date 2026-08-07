/* /d/Praxis/areas/splynn/splynn_clan_hall.c
   The Splugorth Clan Hall: a fortified building in the Splynn back alley
   district. Players may inquire about faction membership here.
   Clan recruiter NPC handles information and admin-gated signup. */

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_short("Splynn Clan Hall");
    set_long(
        "A low fortified chamber carved from the same bone-and-obsidian\n"
        "construction as the rest of Splynn. The walls are hung with banners\n"
        "bearing clan sigils: a Kittani serpent crest, a stylized Splugorth\n"
        "eye, the tooth-wheel of the Brodkil clans. The air smells of wax\n"
        "and old stone.\n\n"
        "A single desk sits at the far end, manned by a Kittani administrator\n"
        "in formal armor. Clan membership here is a serious business.\n"
        "Membership grants an amulet engraved with your faction name,\n"
        "worn around the neck as proof of allegiance.\n\n"
        "Exit: west back to the back alley, east to guild row.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/splynn/splynn_back_alley",
        "east" : "/domains/Praxis/areas/splynn/splynn_guild_row"
    ]) );
    set_listen("default",
        "The scratch of a stylus, the distant sound of the alley, and\n"
        "a faint hum from the clan banners: each one resonates with\n"
        "a minor dimensional ward.");
    set_smell("default",
        "Wax, old stone, and the faint mineral smell of worked obsidian.");
    set_items( ([
        "banners" : "Clan sigil banners hanging from iron rods. Kittani serpent,\n"
                    "Splugorth eye, Brodkil tooth-wheel, and others less recognizable.\n"
                    "Each faction represented here has accepted Splugorth authority.",
        "desk"    : "A functional desk of dark wood with clan membership ledgers\n"
                    "stacked in neat rows. The administrator keeps these well ordered.",
        "sigils"  : "The clan sigils are not decorative. Each one carries a binding\n"
                    "ward: members of the opposing faction cannot enter this hall\n"
                    "without their allegiance being known.",
        "ledgers" : "Clan membership ledgers. Each player's name, race, and faction\n"
                    "status would be recorded here upon joining.",
        "amulet"  : "A Splugorth clan amulet is an engraved metal disc worn at the\n"
                    "neck. It identifies your faction to other members and grants\n"
                    "certain faction-specific access throughout Splynn.",
        "wards"   : "Dimensional wards on the room. You can feel them as a faint\n"
                    "pressure behind the eyes. They record who enters and leaves."
    ]) );
}

void reset() {
    ::reset();
    if(!present("kittani administrator", this_object()))
        clone_object(NPC_PATH+"clan_recruiter.c")->move(this_object());
}
