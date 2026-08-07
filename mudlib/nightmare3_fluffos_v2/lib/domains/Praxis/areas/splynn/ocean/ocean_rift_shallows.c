/* /domains/Praxis/areas/splynn/ocean/ocean_rift_shallows.c
   Ocean Rift, Shallows: first underwater zone below the Splynn ocean rift. */

#include <std.h>
#include <rooms.h>
#include <daemons.h>

#define MON_PATH "/domains/Praxis/monsters/"
#define ROOM_ABOVE "/domains/Praxis/areas/splynn/ocean_rift"

inherit ROOM;

private int can_breathe_here(object who);

/* Underwater breathing gate, same check family as the New Camelot
   forest lake crossing: worn rebreather, Swim as a Fish, or an
   installed oxygen cell / cyber lung. Creators pass. */
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
        write("The water closes over your head and your lungs find only\n"
              "salt water. You fight your way back toward the light,\n"
              "gasping. You need a rebreather, an oxygen implant, or the\n"
              "Swim as a Fish spell to survive down here.\n");
        tell_room(this_object(), (string)this_player()->query_cap_name() +
            " thrashes back toward the surface, out of air.\n",
            ({ this_player() }));
        this_player()->move_player(ROOM_ABOVE);
    }
}

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("water", 1);
    set_property("rest_allowed", 0);
    set_short("Ocean Rift, Shallows");
    set_long(
        "Fractured light filters down through water stained with rift energy,\n"
        "blues and golds that do not come from the sun. The seafloor is not\n"
        "visible from here. Strange currents pull at you from below, patient\n"
        "and cold.\n\n"
        "Non-aquatic beings require a breathing apparatus or the Swim as a\n"
        "Fish spell to survive extended time in these waters.\n"
    );
    set_exits( ([
        "up"   : "/domains/Praxis/areas/splynn/ocean_rift",
        "down" : "/domains/Praxis/areas/splynn/ocean/ocean_rift_deep"
    ]) );
    set_listen("default",
        "The pressure of water and the low hum of the dimensional rift below.");
    set_smell("default",
        "Salt, rift ozone, and the mineral smell of deep ocean.");
    set_items( ([
        "water" : "Rift-stained seawater, its colors not entirely natural. Moving\n"
                  "through it requires effort against the slow current pulling down.",
        "light" : "Filtered Atlantic sunlight mixed with rift energy. The blue and\n"
                  "gold tones are not from any natural source.",
        "current" : "A slow persistent pull from below. Something is drawing water\n"
                    "down toward the rift's deepest point."
    ]) );
}

void reset() {
    ::reset();
    if(!present("ocean creature", this_object()))
        clone_object(MON_PATH + "ocean_creature")->move(this_object());
}
