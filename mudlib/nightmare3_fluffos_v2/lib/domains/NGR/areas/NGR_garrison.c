/* /domains/NGR/areas/NGR_garrison.c
   New German Republic -- Garrison district, south of the market.
   Military housing, motor pool, and patrol assembly area.
   Gargoyle scouts press the north wall at night; soldiers engage them routinely.
   Connects north to the market district. */

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"
#define NPC_PATH     "/domains/NGR/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("ngr_territory", 1);
    set_short("NGR Garrison District");
    set_long(
        "The garrison district occupies a walled sector south of the market.\n"
        "Motor pools of Triax-built APCs and power armor maintenance frames\n"
        "fill the western side. The eastern block is troop housing, barracks\n"
        "buildings with the same uniform grey prefab architecture as everything\n"
        "else inside NGR territory.\n\n"
        "Soldiers move between buildings on scheduled rotations. A duty board\n"
        "near the main entrance lists today's patrol assignments and threat\n"
        "assessment: GARGOYLE SCOUTS: ELEVATED. The NGR garrison handles the\n"
        "gargoyle threat at the northern border with practiced routine.\n"
        "The creatures test the walls regularly and are repelled with rail guns\n"
        "and power armor.\n\n"
        "The market district is north.");
    set_exits( ([
        "north" : "/domains/NGR/areas/NGR_market_district",
        "down"  : "/domains/NGR/areas/ngr_underground_bunker"
    ]) );
    set_listen("default",
        "The clank of power armor maintenance. Boots on pavement, regular and\n"
        "precise. A distant rail gun test-fire, single shot, then silence.");
    set_smell("default",
        "Machine oil, weapons solvent, and the specific smell of power armor\n"
        "cooling systems: warm polymer and ozone.");
    set_items( ([
        "barracks"  : "Grey prefab troop housing. Numbered, not named. Efficient.",
        "motor pool": "APCs and power armor frames in maintenance bays. "
                      "Triax construction, all in working order.",
        "frames"    : "Power armor maintenance frames: open scaffolding that holds "
                      "a suit in position while technicians work on it.",
        "duty board": "Today's assignments. Patrol rotations, threat status. "
                      "GARGOYLE SCOUTS: ELEVATED is written in bold lettering.",
        "apcs"      : "Armored Personnel Carriers in NGR grey. Triax model TX-APC-2. "
                      "Capable of carrying a full squad in powered armor.",
        "soldiers"  : "NGR garrison troops moving between buildings. On schedule, "
                      "in order, without conversation beyond what duty requires.",
        "wall"      : "A perimeter wall, reinforced. Rail gun emplacements at the "
                      "corners. The gargoyle problem is not hypothetical."
    ]) );
}

void reset() {
    ::reset();
    if(!present("ngr gate guard", this_object()))
        clone_object(NPC_PATH + "ngr_gate_guard.c")->move(this_object());
    if(!present("gargoyle", this_object()) && !random(3))
        clone_object(MONSTER_PATH + "gargoyle.c")->move(this_object());
}
