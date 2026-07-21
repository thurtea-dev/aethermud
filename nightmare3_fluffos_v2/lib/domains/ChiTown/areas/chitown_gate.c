// /domains/ChiTown/areas/chitown_gate.c
// Chi-Town main gate. Coalition starting room for new Americas characters.
// "waystation" exit (2026-07-21) returns to chitown_start -- north/south
// were already claimed by chitown_boulevard/chitown_burbs, so this follows
// the same named-exit pattern rifts_welcome.c uses for its "shelter" link.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Chi-Town, Main Gate");
    set_long(
        "A towering wall of reinforced ferrocrete and razor wire stretches as\n"
        "far as you can see in both directions. The gate itself is a massive\n"
        "slab of Coalition-gray metal studded with sensor arrays and weapon\n"
        "mounts. Guards in black Dead Boy armor watch every approach. A glowing\n"
        "CS skull emblem dominates the gate above you.\n\n"
        "Coalition propaganda banners line the walls. Automated turrets track\n"
        "movement. The air smells of exhaust and ozone from weapon systems on\n"
        "standby.");
    set_exits( ([
        "north" : "/domains/ChiTown/areas/chitown_boulevard",
        "south" : "/domains/ChiTown/areas/chitown_burbs",
        "waystation" : "/domains/ChiTown/areas/chitown_start"
    ]) );
    set_listen("default",
        "The hum of weapon systems, the tramp of boots, and Coalition anthems "
        "playing from speakers embedded in the wall.");
    set_smell("default",
        "Exhaust fumes, weapon lubricant, and the sharp tang of ozone from "
        "active energy systems.");
    set_items( ([
        "wall" : "Ferrocrete and razor wire, stretching as far as you can see.",
        "gate" : "A massive Coalition-gray metal slab studded with sensors and weapon mounts.",
        "banners" : "Coalition propaganda: THE STRONG SURVIVE. HUMANITY FIRST.",
        "turrets" : "Automated weapon mounts. They track movement.",
        "skull" : "The glowing CS skull emblem above the gate. It watches.",
        "guards" : "Dead Boys in full CA-4 armor. They watch everyone."
    ]) );
}

private void ensure_exits() {
    if(sizeof(query_exits()) > 0) return;
    set_exits( ([
        "north" : "/domains/ChiTown/areas/chitown_boulevard",
        "south" : "/domains/ChiTown/areas/chitown_burbs",
        "waystation" : "/domains/ChiTown/areas/chitown_start"
    ]) );
}

void init() {
    ensure_exits();
    ::init();
    if(!this_player() || !(int)this_player()->is_player()) return;
    if(WANTED_D->is_cs_enemy(this_player())) {
        call_out("spawn_guard_attack", 1);
    } else {
        write("A guard glances at you. \"Move along, citizen.\"\n");
        if((string)this_player()->getenv("cs_gate_visited") != ctime(time())[0..9]) {
            this_player()->setenv("cs_gate_visited", ctime(time())[0..9]);
            catch(FACTION_D->modify_standing(
                (string)this_player()->query_name(),
                "coalition", 5, "visited Chi-Town gate"));
        }
    }
}

void spawn_guard_attack() {
    object guard1;
    object guard2;

    if(!environment(this_player())) return;
    guard1 = clone_object("/domains/ChiTown/npcs/dead_boy_guard");
    guard2 = clone_object("/domains/ChiTown/npcs/dead_boy_guard");
    if(guard1) guard1->move(this_object());
    if(guard2) guard2->move(this_object());
    tell_room(this_object(),
        "An alarm sounds. Two Dead Boy guards charge toward you!\n", ({}));
    if(guard1 && this_player()) guard1->kill_ob(this_player(), 0);
    if(guard2 && this_player()) guard2->kill_ob(this_player(), 0);
}

void reset() {
    ::reset();
    if(!present("dead boy guard", this_object())) {
        catch(clone_object("/domains/ChiTown/npcs/dead_boy_guard")->move(this_object()));
        catch(clone_object("/domains/ChiTown/npcs/dead_boy_guard")->move(this_object()));
    }
    if(!present("coalition sergeant", this_object()))
        catch(clone_object("/domains/ChiTown/npcs/cs_sergeant")->move(this_object()));
    if(!present("cs rift liaison", this_object()))
        catch(clone_object("/domains/ChiTown/npcs/cs_rift_liaison")->move(this_object()));
}
