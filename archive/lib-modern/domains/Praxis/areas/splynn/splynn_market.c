// /d/Praxis/areas/splynn/splynn_market.c
// The Splynn Dimensional Market: a terrifying hub of Splugorth commerce.
// The down exit to splynn_underground is hidden; revealed by the dig command.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("vehicle_accessible", 1);
    set_property("ex5_accessible", 1);
    set_property("ley_line_room", 1);
    set_property("is_ley_line", 1);
    set_property("ley_line", 1);
    set_short("Splynn Dimensional Market");
    set_long(
        "The Splynn Dimensional Market sprawls across a vast plaza of black\n"
        "obsidian-veined stone. Stalls of bone and worked metal are staffed\n"
        "by creatures that should not exist: tentacled merchants, floating\n"
        "eye-clusters, and reptilian Kittani in articulated power armor.\n"
        "Enslaved beings from a dozen worlds shuffle between stalls under the\n"
        "watchful gaze of winged Blind Warrior Women.\n\n"
        "A Kittani merchant runs a well-stocked stall on the eastern side.\n"
        "The atmosphere hums with dimensional energy. The ley lines here\n"
        "feed directly into the market's structure.\n\n"
        "Exits: west along the Long Road toward Praxis, north to Splynn Docks,\n"
        "east to the Slave Pits, northeast to the East Bazaar.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/long_road",
        "north" : "/domains/Praxis/areas/splynn/splynn_docks",
        "east" : "/domains/Praxis/areas/splynn/splynn_pits",
        "south" : "/domains/Praxis/areas/splynn/rockys_bar_splynn",
        "northeast" : "/domains/Praxis/areas/splynn/splynn_east_bazaar"
    ]) );
    set_listen("default",
        "A cacophony of alien languages, the clank of chains, dimensional nosave,\n"
        "and something that sounds like screaming but is probably just a vendor's call.");
    set_smell("default",
        "Ozone, exotic spices from other worlds, blood, and machinery oil.");
    set_items( ([
        "stalls" : "Stalls of bone and metal selling technology from multiple dimensions.",
        "kittani" : "Reptilian humanoids in power armor. They watch everything.",
        "slaves" : "Beings from a dozen worlds, collared and blank-eyed. Better not to linger.",
        "stone" : "The plaza floor is veined black obsidian, seamlessly fitted.",
        "energy" : "The air itself seems to vibrate with dimensional power.",
        "ley lines" : "The ley lines here pulse visibly as faint lines of blue light\n"
                      "running through the obsidian floor.",
        "ground" : "The stone plaza floor. In one corner the surface looks\n"
                      "different, softer, as if the obsidian has been patched\n"
                      "over something underneath. It might give way with enough effort.",
        "patch" : "A patch of stone that looks newer than the rest of the plaza floor.\n"
                      "The ground underneath sounds hollow when you tap it."
    ]) );
}

void reset() {
    ::reset();
    if(!present("kittani merchant", this_object()))
        clone_object("/domains/Praxis/monsters/kittani_merchant.c")->move(this_object());
    if(!present("moxim", this_object()))
        clone_object("/domains/Praxis/monsters/moxim.c")->move(this_object());
    if(!present("scarred slave", this_object()))
        clone_object("/domains/Praxis/areas/splynn/shadow_slave")->move(this_object());
}
