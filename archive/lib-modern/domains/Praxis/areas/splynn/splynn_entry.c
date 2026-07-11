// /d/Praxis/areas/splynn/splynn_entry.c
// Rift arrival point on the shores of Atlantis.
// Moxim portals from Praxis deposit travelers here.

#include <std.h>
#include <rooms.h>

#define MOXIM_PATH "/domains/Praxis/monsters/moxim"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("vehicle_accessible", 0);
    set_short("Splynn Rift Arrival Plaza");
    set_long(
        "The rift spits you out onto a broad stone plaza overlooking the\n"
        "Atlantic coast. The air tastes of ozone and sea salt, and the\n"
        "dimensional residue from the transit shimmers at the edges of your\n"
        "vision for a few seconds before fading.\n\n"
        "Ahead to the south, the Splynn Dimensional Market sprawls across\n"
        "a vast obsidian plaza. The noise, smell, and alien geometry of it\n"
        "hit you all at once. This is Atlantis now: a Splugorth territory\n"
        "carved from the ruins of the old world. Move carefully.\n\n"
        "A carved stone marker reads: SPLYNN DIMENSIONAL MARKET.\n"
        "Arrivals are advised to declare all weapons at the checkpoint.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/splynn/splynn_market",
        "west" : "/domains/Praxis/areas/splynn/splynn_market"
    ]) );
    set_listen("default",
        "The distant crash of the Atlantic and the ambient hum of the market.\n"
        "Something large passed overhead a moment ago.");
    set_smell("default",
        "Sea salt, ozone from dimensional transit, and the distinct smell\n"
        "of Splugorth territory: something organic and unpleasant underneath everything.");
    set_items( ([
        "marker" : "Stone marker carved in three scripts. The top script is Atlantean.\n"
                       "All three say the same thing: SPLYNN DIMENSIONAL MARKET.",
        "plaza" : "Broad flat stone, worn smooth by decades of arrivals.\n"
                       "The edges glow faintly where rift energy has scorched them.",
        "coast" : "The Atlantic, visible to the north and east. Vast and grey.\n"
                       "Splugorth barges are visible offshore.",
        "checkpoint" : "A Kittani checkpoint post, currently unoccupied.\n"
                       "Someone has been here recently. The sensors are still warm.",
        "rift" : "The rift has closed behind you. The air still shimmers slightly\n"
                       "where it was. There is no going back that way."
    ]) );
}

void reset() {
    ::reset();
    if(!present("moxim", this_object()))
        clone_object(MOXIM_PATH)->move(this_object());
}
