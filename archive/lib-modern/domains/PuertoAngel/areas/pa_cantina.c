/* /domains/PuertoAngel/areas/pa_cantina.c
   La Cantina -- open day and night, but busier after dark. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("La Cantina");
    set_long(
        "Open day and night. Vampires need somewhere to gather, and the barkeep\n"
        "has been here long enough to understand hospitality in all its forms.\n"
        "Dark wood, old tile, candles in every niche. The bar runs the length of\n"
        "the west wall. There are bottles behind it you would not recognize.\n\n"
        "The barkeep is ancient and civil. They serve blood-wine and information\n"
        "in equal measure. A secondary vampire or two occupies the corner tables.\n"
        "Nobody bothers you if you do not bother them.");
    set_exits( ([
        "east" : "/domains/PuertoAngel/areas/pa_main_street"
    ]) );
    set_listen("default",
        "Low conversation. Something old and Spanish on a record somewhere. "
        "The sound of liquid being poured.");
    set_smell("default",
        "Candle wax, old wood, copper, and something fermented that is not wine.");
    set_items( ([
        "bar"       : "Long, old, dark wood. Polished. The bottles behind it are unlabeled.",
        "bottles"   : "You do not ask what is in them. You do not.",
        "candles"   : "Dozens of them, in iron holders and cracked cups. Warm light.",
        "tables"    : "A few occupied by still, quiet figures who have not moved since you entered.",
        "barkeep"   : "Ancient. Civil. Old eyes that have seen the world before and after.",
        "blood-wine": "The specialty of the house. You are not sure you want to know."
    ]) );
}

varargs string query_long(string item) {
    int tod;

    if(item && strlen(item)) return ::query_long(item);

    tod = time() % 86400;
    if(tod >= 43200) {
        return
            "La Cantina is alive at night. Every table taken, every stool occupied.\n"
            "The conversations are quiet and fast -- too fast for human speech.\n"
            "The barkeep moves between groups with the practiced ease of decades.\n"
            "The record player cycles through the same songs without complaint.\n\n"
            "You are the only breathing thing in the room. This is noticed.\n"
            "Not acted upon -- just noticed. For now.\n\n"
            "East leads back to the main street.";
    }
    return
        "Open day and night. Vampires need somewhere to gather, and the barkeep\n"
        "has been here long enough to understand hospitality in all its forms.\n"
        "Dark wood, old tile, candles in every niche. The bar runs the length of\n"
        "the west wall. There are bottles behind it you would not recognize.\n\n"
        "The barkeep is ancient and civil. They serve blood-wine and information\n"
        "in equal measure. A secondary vampire or two occupies the corner tables.\n"
        "Nobody bothers you if you do not bother them.";
}

void reset() {
    object barkeep;
    object vamp;
    int tod;

    ::reset();
    tod = time() % 86400;

    if(!present("barkeep", this_object())) {
        barkeep = clone_object("/domains/PuertoAngel/npcs/pa_barkeep");
        barkeep->move(this_object());
    }
    if(!present("elena", this_object())) {
        barkeep = clone_object("/domains/PuertoAngel/npcs/pa_hunter");
        barkeep->move(this_object());
    }
    if(tod < 43200) {
        /* Daytime: barkeep alone, minimal atmosphere */
        return;
    }
    /* Night: busier */
    if(!present("secondary vampire", this_object())) {
        vamp = clone_object("/domains/PuertoAngel/npcs/pa_secondary_vampire");
        vamp->move(this_object());
    }
    if(random(2) == 0) {
        vamp = clone_object("/domains/PuertoAngel/npcs/pa_secondary_vampire");
        vamp->move(this_object());
    }
    if(random(3) == 0) {
        vamp = clone_object("/domains/PuertoAngel/npcs/pa_secondary_vampire");
        vamp->move(this_object());
    }
}
