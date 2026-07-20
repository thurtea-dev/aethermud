/* /domains/Praxis/areas/splynn/ocean/ocean_rift_floor.c
   Ocean Rift Floor: Atlantean ruins at the base of the dimensional rift. */

#include <std.h>
#include <rooms.h>
#include <daemons.h>

#define ROOM_ABOVE "/domains/Praxis/areas/splynn/ocean/ocean_rift_deep"

inherit ROOM;

private int can_breathe_here(object who);

/* Underwater breathing gate; see ocean_rift_shallows.c. Guards this
   depth against teleport and summon arrivals as well. */
private int can_breathe_here(object who) {
    object *inv;
    mapping bionics;
    string *installed;
    int i;

    if(!who) return 0;
    if(creatorp(who)) return 1;
    if((int)who->query_property("swim_as_fish_active")) return 1;
    inv = all_inventory(who);
    for(i = 0; i < sizeof(inv); i++) {
        if((int)inv[i]->query_property("rebreather") &&
           objectp((object)inv[i]->query_worn()))
            return 1;
    }
    bionics = (mapping)BIONIC_D->query_bionics(who);
    if(mapp(bionics)) {
        installed = values(bionics);
        if(member_array("oxygen_storage_cell", installed) != -1) return 1;
        if(member_array("cyber_lung", installed) != -1) return 1;
    }
    return 0;
}

void init() {
    ::init();
    if(this_player() && userp(this_player()) &&
       !can_breathe_here(this_player())) {
        write("The crushing dark offers nothing to breathe. You push off\n"
              "the ancient stones and climb for your life. You need a\n"
              "rebreather, an oxygen implant, or the Swim as a Fish spell\n"
              "to survive this depth.\n");
        tell_room(this_object(), (string)this_player()->query_cap_name() +
            " pushes off the ruins and climbs toward the surface, out of air.\n",
            ({ this_player() }));
        this_player()->move_player(ROOM_ABOVE);
    }
}

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("water", 1);
    set_property("rest_allowed", 0);
    set_short("Ocean Rift Floor, Atlantean Ruins");
    set_long(
        "Crumbled stonework from a civilization that sank long before the Rifts.\n"
        "Columns carved with glyphs line what was once a great hall. The scale\n"
        "is wrong for human architecture: these columns were built for something\n"
        "larger. Much larger.\n\n"
        "The rift tears through the far wall, pulsing with cold light. The tear\n"
        "is old here, older than the current Rifts era. Whatever opened it did\n"
        "so before the coming of the Rifts, or time does not work normally at\n"
        "this depth.\n"
    );
    set_exits( ([
        "up" : "/domains/Praxis/areas/splynn/ocean/ocean_rift_deep"
    ]) );
    set_listen("default",
        "The low resonant pulse of the dimensional rift, much louder here.\n"
        "And silence otherwise. Everything living avoids this depth.");
    set_smell("default",
        "Ancient stone, salt-mineralized over thousands of years, and the\n"
        "acrid smell of a rift that has been open far too long.");
    set_items( ([
        "columns" : "Carved stone columns, each ten meters tall. The glyphs on them\n"
                    "are Atlantean: the written language of a civilization that sank\n"
                    "before recorded history. Some glyphs still glow faintly.",
        "glyphs" : "Atlantean script etched into the stone. You cannot read it\n"
                   "without Atlantean language skill. What you can see: the glyphs\n"
                   "repeat the same phrase in multiple registers, becoming more urgent.",
        "rift" : "A dimensional tear in the far wall of the ruins. Old. Larger\n"
                 "than the rift above. The light from it is cold and steady,\n"
                 "not the pulsing of new rift energy. This one has been open\n"
                 "for a very long time.",
        "stonework" : "Pre-Rifts construction, but not pre-Atlantis. These ruins\n"
                      "are older than anything that should exist here. The stone\n"
                      "is a type not found in any modern geological survey."
    ]) );
}
