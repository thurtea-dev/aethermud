// /domains/NewCamelot/areas/newcamelot_start.c
// New Camelot gatehouse interior. Europe starting room for new characters
// (moved from camelot_gate.c, 2026-07-21). Indoor per the start-room
// naming/consistency pass; carries the same faction/infamy gate-check and
// knight-ambush logic camelot_gate.c used to run, now here instead.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_short("New Camelot, Gatehouse Interior");
    set_long(
        "Torchlight flickers off close stone walls inside the gatehouse of\n"
        "New Camelot. Arrow slits look out over the road to the south; a\n"
        "heavy portcullis mechanism dominates the ceiling, currently raised.\n"
        "Two Royal Knight guards stand post beside the inner archway, lances\n"
        "at rest, watching new arrivals pass through.\n\n"
        "The archway to the east opens onto the old gate proper. North,\n"
        "beyond the portcullis, lies the plaza of New Camelot itself.\n"
        "Magic-rich air hums faintly around the old stones.");
    set_exits( ([
        "north" : "/domains/NewCamelot/areas/camelot_square",
        "south" : "/domains/NewCamelot/areas/europe_road",
        "east"  : "/domains/NewCamelot/areas/camelot_gate"
    ]) );
    set_listen("default",
        "Pennants snapping in the wind outside. Distant bells from the city interior.");
    set_smell("default",
        "Stone dust, horse, and the subtle electric tang of ambient magical energy.");
    set_items( ([
        "portcullis" : "A heavy iron portcullis, currently raised. It last dropped centuries ago.",
        "slits" : "Arrow slits facing the southern road. Standard siege defense.",
        "guards" : "Two Royal Knights in gleaming full plate. Armed with lances.",
        "archway" : "The old gate proper lies through the archway to the east.",
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
    k1 = clone_object("/domains/NewCamelot/npcs/royal_knight_guard");
    k2 = clone_object("/domains/NewCamelot/npcs/royal_knight_guard");
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
        clone_object("/domains/NewCamelot/npcs/royal_knight_guard")->move(this_object());
        clone_object("/domains/NewCamelot/npcs/royal_knight_guard")->move(this_object());
    }
    if(!present("gamemaster", this_object()))
        clone_object("/domains/NewCamelot/npcs/camelot_guide")->move(this_object());
}
