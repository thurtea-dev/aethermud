// /domains/Lazlo/areas/lazlo_gate.c
// The eastern gate of Lazlo, opening toward the Lake of the Gifted.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Lazlo, Eastern Gate");
    set_long(
        "A wide archway of cut stone reinforced with alchemical bindings that\n"
        "glow faintly in the dark. This is the main gate of Lazlo, and it is\n"
        "never entirely unguarded. Two Cyber-Knights in full armor stand at\n"
        "the arch, watching the approaches without hostility.\n\n"
        "Beyond the gate, the Lake of the Gifted stretches east toward the\n"
        "horizon. The water is unnaturally clear. Ley line energy runs along\n"
        "the shoreline in visible threads.\n\n"
        "Back through the arch lies the arrival courtyard. The market quarter\n"
        "is north along the interior wall.");
    set_exits( ([
        "west"  : "/domains/Lazlo/lazlo_welcome",
        "north" : "/domains/Lazlo/areas/lazlo_square",
        "south" : "/domains/Praxis/areas/highway_ruins",
        "east"  : "/domains/Lazlo/areas/lazlo_shoreline"
    ]) );
    set_listen("default",
        "The wind off the lake, and the low murmur of the guards' conversation.");
    set_smell("default",
        "Fresh lake air, clean and cold.");
    set_items( ([
        "gate" : "Stone arch with alchemical reinforcement. The bindings glow softly.",
        "arch" : "See: gate",
        "guards" : "Two Cyber-Knights. Their armor is polished. They watch without menace.",
        "lake" : "The Lake of the Gifted. The water is impossibly clear. Ley line threads trace the shoreline.",
        "shoreline": "A narrow stone path runs along the lake's edge, worn smooth by use.",
        "bindings" : "Alchemical reinforcement. The glow is faint but steady. This gate is not coming down easily."
    ]) );
}

void reset() {
    object *obs;
    int count;
    int j;
    ::reset();
    obs   = all_inventory(this_object());
    count = 0;
    for(j = 0; j < sizeof(obs); j++) {
        if(living(obs[j]) &&
           strsrch((string)obs[j]->query_short(), "gate guard") != -1)
            count++;
    }
    while(count < 2) {
        clone_object("/domains/Lazlo/npcs/lazlo_gate_guard.c")->move(this_object());
        count++;
    }
}
