/* /domains/PuertoAngel/areas/pa_main_street.c
   Puerto Angel Main Street -- empty by day, animated by night.
   Day  = time() % 86400 < 43200  (midnight-to-noon UTC as proxy)
   Night = time() % 86400 >= 43200 */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Puerto Angel Main Street");
    set_long(
        "Empty in daylight. Doors sealed, windows dark. The street itself is\n"
        "immaculate, swept cobblestones, intact storefronts, murals painted\n"
        "on old walls still bright. At night, they say, this street fills with\n"
        "vampires going about their business with unsettling normalcy.\n\n"
        "A cantina sits to the west. A residential district lies north.\n"
        "The outskirts are south.");
    set_exits( ([
        "south" : "/domains/PuertoAngel/areas/pa_outskirts",
        "west"  : "/domains/PuertoAngel/areas/pa_cantina",
        "north" : "/domains/PuertoAngel/areas/pa_residential"
    ]) );
    set_listen("default",
        "Your own footsteps on cobblestone. Nothing else moves.");
    set_smell("default",
        "Old wood, dried flowers, something sweet and slightly wrong.");
    set_items( ([
        "murals"     : "Painted on the old walls. Pre-Rifts scenes of fishing and "
                       "celebration. Maintained.",
        "doors"      : "Sealed. Not locked -- sealed. From the inside.",
        "storefronts": "Intact. Clean windows. No goods on display.",
        "cobblestones": "Swept clean. No dust. Someone sweeps this street regularly.",
        "cantina"    : "West. The one place in town that seems to be open."
    ]) );
}

varargs string query_long(string item) {
    int tod;

    if(item && strlen(item)) return ::query_long(item);

    tod = time() % 86400;
    if(tod >= 43200) {
        return
            "The street comes alive at night -- but not with life you recognize.\n"
            "Figures move along the cobblestones with eerie purpose, faster than\n"
            "they should, turning corners without looking. The storefronts are\n"
            "lit from within by something that is not quite candlelight.\n\n"
            "Nobody pays you attention. That may not remain true.\n\n"
            "A cantina glows to the west. Residential buildings lie north.\n"
            "The outskirts are south.";
    }
    return
        "Empty in daylight. Doors sealed, windows dark. The street itself is\n"
        "immaculate, swept cobblestones, intact storefronts, murals painted\n"
        "on old walls still bright. At night, they say, this street fills with\n"
        "vampires going about their business with unsettling normalcy.\n\n"
        "A cantina sits to the west. A residential district lies north.\n"
        "The outskirts are south.";
}

void reset() {
    object vamp;
    int tod;

    ::reset();
    tod = time() % 86400;
    if(tod < 43200) return;

    /* Night spawns */
    if(!present("secondary vampire", this_object())) {
        vamp = clone_object("/domains/PuertoAngel/npcs/pa_secondary_vampire");
        vamp->move(this_object());
    }
    if(random(2) == 0 && !present("wild vampire", this_object())) {
        vamp = clone_object("/domains/Praxis/monsters/wild_vampire");
        vamp->move(this_object());
    }
}
