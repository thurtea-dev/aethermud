// /d/Praxis/areas/splynn/preserve_hive.c
// Splynn Preserves hunting loop, R6: a Xiticix hive spur, insectoid warriors.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Xiticix Spur");
    set_long(
        "A spur of hive matter erupts from the preserve floor here, grey and\n"
        "chitinous and ridged like the inside of a wasp's nest grown to the\n"
        "size of a barn. A breeding pair of Xiticix must have been rifted in\n"
        "and never cleared, and the insectoids have built. The walls pulse\n"
        "faintly, warm to stand near, and the air hums with wingbeats you\n"
        "cannot see the source of.\n\n"
        "Guardian warriors patrol the chittering galleries. The walkway back\n"
        "to the blind runs west; a cleft climbs north out of the hive stink\n"
        "toward higher ground.");
    set_exits( ([
        "west"  : "/domains/Praxis/areas/splynn/preserve_blind",
        "north" : "/domains/Praxis/areas/splynn/preserve_snag"
    ]) );
    set_listen("default",
        "A layered hum of wings, the wet clicking of mandibles, and the deep\n"
        "resonant pulse of the hive itself breathing around you.");
    set_smell("default",
        "Sharp formic acid, chewed pulp, and the sickly warmth of a nest\n"
        "that has never once been cold.");
    set_items( ([
        "hive"       : "Grey chitinous matter chewed and cemented into ridged\n"
                       "walls. It is warm, and it pulses, and it is very much alive.",
        "galleries"  : "Tunnels bored through the hive spur, wide enough for a\n"
                       "warrior to rush down at you without warning.",
        "walls"      : "Nest walls that flex and pulse with the hive's slow\n"
                       "breathing. Cutting them would bring the whole spur down on you."
    ]) );
}

void reset() {
    ::reset();
    if(!present("xiticix warrior", this_object()))
        clone_object(MONSTER_PATH+"xiticix_warrior.c")->move(this_object());
    if(random(10) < 4 && !present("splugorth minion", this_object()))
        clone_object(MONSTER_PATH+"splugorth_minion.c")->move(this_object());
}
