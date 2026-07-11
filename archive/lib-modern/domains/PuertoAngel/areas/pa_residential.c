/* /domains/PuertoAngel/areas/pa_residential.c
   Puerto Angel Residential District -- vampire neighborhood, quiet by day. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_short("Residential District");
    set_long(
        "Once a neighborhood. Painted houses in pastel colors behind low walls.\n"
        "Gardens that were tended once and may still be, in a fashion. Now home\n"
        "to vampires who have kept the architecture and abandoned the schedule.\n\n"
        "Doors are locked by day. At night, shadows move between the houses with\n"
        "unnerving speed, not threatening, just present, going about whatever\n"
        "vampires go about at night in a town they have made their own.");
    set_exits( ([
        "south" : "/domains/PuertoAngel/areas/pa_main_street",
        "north" : "/domains/PuertoAngel/areas/pa_church"
    ]) );
    set_listen("default",
        "Silence. Absolute silence. Even the wind seems to avoid this street.");
    set_smell("default",
        "Gardenia and old earth. Something dried and metallic underneath.");
    set_items( ([
        "houses"  : "Pastel painted. Maintained. The shutters are sealed.",
        "gardens" : "Tended to a degree. The plants chosen carefully. No garlic.",
        "doors"   : "Locked. Iron-framed. Old wood.",
        "walls"   : "Low garden walls. Easy to step over. Not meant to keep people out.",
        "shadows" : "There is movement at the edge of your vision. Then there is not."
    ]) );
}

varargs string query_long(string item) {
    int tod;

    if(item && strlen(item)) return ::query_long(item);

    tod = time() % 86400;
    if(tod >= 43200) {
        return
            "The houses are open now -- windows unshuttered, front gates ajar.\n"
            "Figures move between them with unhurried confidence, carrying nothing,\n"
            "going nowhere you can identify. Children's toys sit in one yard,\n"
            "undisturbed. There are no children.\n\n"
            "The smell of gardenias is stronger at night. The metallic note\n"
            "underneath is stronger too.\n\n"
            "South leads back to main street.";
    }
    return
        "Once a neighborhood. Painted houses in pastel colors behind low walls.\n"
        "Gardens that were tended once and may still be, in a fashion. Now home\n"
        "to vampires who have kept the architecture and abandoned the schedule.\n\n"
        "Doors are locked by day. At night, shadows move between the houses with\n"
        "unnerving speed, not threatening, just present, going about whatever\n"
        "vampires go about at night in a town they have made their own.";
}

void reset() {
    object vamp;
    int tod;

    ::reset();
    tod = time() % 86400;
    if(tod < 43200) return;

    /* Night spawns */
    if(!present("wild vampire", this_object())) {
        vamp = clone_object("/domains/Praxis/monsters/wild_vampire");
        vamp->move(this_object());
    }
    if(random(2) == 0) {
        vamp = clone_object("/domains/Praxis/monsters/wild_vampire");
        vamp->move(this_object());
    }
    if(random(3) == 0 && !present("secondary vampire", this_object())) {
        vamp = clone_object("/domains/PuertoAngel/npcs/pa_secondary_vampire");
        vamp->move(this_object());
    }
}
