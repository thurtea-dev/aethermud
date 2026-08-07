/* /domains/Praxis/areas/splynn/ocean/ocean_rift_deep.c
   Ocean Rift, Deep Zone: home of the ocean leviathan. */

#include <std.h>
#include <rooms.h>
#include <daemons.h>

#define MON_PATH "/domains/Praxis/monsters/"
#define ROOM_ABOVE "/domains/Praxis/areas/splynn/ocean/ocean_rift_shallows"

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
        write("The pressure squeezes the last air from your chest. You\n"
              "kick upward in the dark, lungs burning. You need a\n"
              "rebreather, an oxygen implant, or the Swim as a Fish spell\n"
              "to survive this depth.\n");
        tell_room(this_object(), (string)this_player()->query_cap_name() +
            " kicks upward into the dark, out of air.\n",
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
    set_short("Ocean Rift, Deep Zone");
    set_long(
        "Complete darkness except for the faint blue glow of the rift tear\n"
        "below. The pressure at this depth is crushing. Water moves in slow\n"
        "spirals around a dimensional distortion that has no natural cause.\n\n"
        "Something large moves in the dark. You can hear it: displacement of\n"
        "water in rhythms too slow and too regular to be current.\n\n"
        "The rift floor is faintly visible below, lit by cold light from\n"
        "structures that should not exist this deep in the Atlantic. Non-aquatic\n"
        "beings require full environmental protection to survive this depth.\n"
    );
    set_exits( ([
        "up"   : "/domains/Praxis/areas/splynn/ocean/ocean_rift_shallows",
        "down" : "/domains/Praxis/areas/splynn/ocean/ocean_rift_floor"
    ]) );
    set_listen("default",
        "Slow rhythmic displacement in the dark. Something very large, breathing\n"
        "or moving through water in long, deliberate cycles.");
    set_smell("default",
        "The cold mineral smell of the deep Atlantic. Old water. Old darkness.");
    set_items( ([
        "darkness" : "Total. Only the blue rift glow below gives any reference.\n"
                     "Your eyes are useless here without light equipment.",
        "glow" : "Blue-white light from the rift tear below, illuminating the\n"
                 "edges of ancient stonework on the ocean floor.",
        "movement" : "Whatever it is, it is large. The displacement of water when\n"
                     "it moves is perceptible as pressure changes against your body."
    ]) );
}

void reset() {
    ::reset();
    if(!present("ocean leviathan", this_object()))
        clone_object(MON_PATH + "ocean_leviathan")->move(this_object());
}
