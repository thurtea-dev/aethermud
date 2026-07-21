// /d/Praxis/rifts_welcome.c
// Arrival zone for newly created Rifts Earth characters.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_property("ley_line_room", 1);
    set_property("is_ley_line", 1);
    set_property("ley_line", 1);
    set_short("Rift Arrival Zone, outskirts of Praxis");
    set_long(
        "A scorched clearing marks the edge of Praxis. The air crackles "
        "with residual ley line energy. The rift that opened here has "
        "closed, but the ground still bears the burn. To the south, a "
        "makeshift wall of salvaged girders and Coalition scrap marks the "
        "boundary of Praxis proper. A battered sign has been nailed to a "
        "pole near a smoldering energy barrel.\n\n"
        "The sky overhead is a sickly amber, laced with the faint aurora "
        "of dimensional interference. In the distance, ruined skyscrapers "
        "pierce a horizon thick with smoke. Whatever world this was before "
        "the coming of the rifts, it is gone now.");
    set_exits(
        (["south" : "/domains/Praxis/areas/monument_square",
          "east" : "/domains/Praxis/east_road1",
          "west" : "/domains/Praxis/west_road1",
          "north" : "/domains/Praxis/areas/test_zone",
          "shelter" : "/domains/ChiTown/areas/chitown_start"]) );
    set_listen("default",
        "Wind whips through torn steel. Somewhere in the distance "
        "a ley line hums with barely contained power.");
    set_smell("default",
        "Ozone, scorched earth, and the metallic tang of rift energy.");
    set_items(
        (["sign" :
            "A hand-painted sign on a rusted pole reads:\n\n"
            " WELCOME TO RIFTS EARTH: PRAXIS REFUGE\n\n"
            " Basic commands:\n"
            " look examine your surroundings\n"
            " inventory see your gear\n"
            " score your character stats\n"
            " who other players online\n"
            " say <text> speak aloud\n"
            " tell <name> <text> private message\n"
            " help <topic> in-game help files\n"
            " north/south/east/west move\n"
            " kill <target> attack (caution: lethal world)\n"
            " quests track jobs you accept\n\n"
            " The settlement of Praxis lies to the SOUTH.\n"
            " Talk to the survivor nearby for more guidance.\n",
          "barrel" :
            "A steel barrel filled with burning scrap, improvised warmth "
            "at the edge of a very cold world.",
          "pole" : "A rusted metal pole driven into cracked asphalt.",
          "clearing" : "Blackened earth where the Rift opened. Nothing grows here yet.",
          "wall" :
            "A perimeter wall cobbled together from Coalition debris, "
            "salvaged I-beams, and reinforced concrete. Bullet scars "
            "and plasma burns mark its surface. This place has been "
            "defended before.",
          "sky" :
            "The amber sky shifts and shimmers. You can make out distant "
            "ley line energies arcing between clouds like slow lightning.",
          "ruins" :
            "The skeletal remains of a pre-Rifts city, too far away to "
            "detail but impossible to miss.",
          "survivor" : "She is watching the perimeter. Try <say help> to ask for guidance."
        ]) );
}

void reset() {
    ::reset();
    if(!present("survivor", this_object()))
        new("/domains/Praxis/obj/mon/rift_survivor")->move(this_object());
    if(!present("gamemaster", this_object()))
        clone_object("/domains/Praxis/npcs/chargen_guide")->move(this_object());
}
