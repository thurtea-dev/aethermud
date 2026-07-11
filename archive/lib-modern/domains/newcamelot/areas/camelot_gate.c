// /domains/newcamelot/areas/camelot_gate.c
// New Camelot main gate. Europe starting room for new characters.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("New Camelot, Great Gate");
    set_long(
        "Stone walls draped with heraldic banners rise on either side of the\n"
        "great gates of New Camelot. The banners show a white dragon on a\n"
        "blue field, the emblem of the Camelot Order. Two Royal Knight guards\n"
        "stand watch in gleaming full plate, lances upright.\n\n"
        "Beyond the gate, cobblestone streets lead to the main plaza. The\n"
        "spires of the wizard tower are visible above the rooftops.\n"
        "Magic-rich air hums faintly around the old stones.");
    set_exits( ([
        "north" : "/domains/newcamelot/areas/camelot_square",
        "south" : "/domains/newcamelot/areas/europe_road"
    ]) );
    set_listen("default",
        "Pennants snapping in the wind. Distant bells from the city interior.");
    set_smell("default",
        "Stone dust, horse, and the subtle electric tang of ambient magical energy.");
    set_items( ([
        "banners" : "Heraldic banners showing a white dragon on blue. New Camelot.",
        "gate" : "Great stone gates bound with iron. Open to travelers.",
        "guards" : "Two Royal Knights in gleaming full plate. Armed with lances.",
        "spires" : "The tower of the Camelot Order, visible above the roofline.",
        "stones" : "Old stones. Very old. They predate the Rifts by centuries."
    ]) );
}

void init() {
    ::init();
    call_out("check_entry", 1);
}

void check_entry() {
    int infamy;
    int coalition_rep;
    string name;

    if(!this_player() || !objectp(this_player())) return;
    name = (string)this_player()->query_name();
    infamy = (int)WANTED_D->query_infamy(name);
    coalition_rep = (int)FACTION_D->query_standing(name, "coalition");

    if(infamy > 300 || coalition_rep > 500) {
        tell_room(this_object(),
            "A Royal Knight steps forward. 'You are not welcome within these walls.'\n", ({}));
        call_out("spawn_knight_attack", 1);
        return;
    }
    write("A Royal Knight nods. 'Hail, traveler. Welcome to New Camelot.'\n");
    if((string)this_player()->getenv("nc_gate_visited") != ctime(time())[0..9]) {
        this_player()->setenv("nc_gate_visited", ctime(time())[0..9]);
        catch(FACTION_D->modify_standing(name, "new_camelot", 5, "visited New Camelot"));
    }
}

void spawn_knight_attack() {
    object k1;
    object k2;

    if(!this_player() || !environment(this_player())) return;
    k1 = clone_object("/domains/newcamelot/npcs/royal_knight_guard");
    k2 = clone_object("/domains/newcamelot/npcs/royal_knight_guard");
    if(k1) k1->move(this_object());
    if(k2) k2->move(this_object());
    tell_room(this_object(),
        "The Royal Knights lower their lances and advance!\n", ({}));
    if(k1 && this_player()) k1->kill_ob(this_player(), 0);
    if(k2 && this_player()) k2->kill_ob(this_player(), 0);
}

void reset() {
    ::reset();
    if(!present("royal knight guard", this_object())) {
        clone_object("/domains/newcamelot/npcs/royal_knight_guard")->move(this_object());
        clone_object("/domains/newcamelot/npcs/royal_knight_guard")->move(this_object());
    }
}
