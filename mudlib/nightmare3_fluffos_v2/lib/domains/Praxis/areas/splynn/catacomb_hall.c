// /d/Praxis/areas/splynn/catacomb_hall.c
// Catacomb main hall, deeper into the complex.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set_short("Catacomb Hall");
    set_long(
        "A long hall cut from the bedrock, the walls lined with sealed niches\n"
        "stacked floor to ceiling. Each niche is sealed with a carved stone\n"
        "panel. Figures are etched into some of the panels: humans, creatures,\n"
        "things that do not have neat categories.\n\n"
        "The hall runs north back toward the entrance and south toward a chamber\n"
        "where something faint and cold emanates, just at the edge of perception.\n"
        "Whatever this place was built to contain, it was built to last.\n\n"
        "A persistent feeling that you are being observed has not left you since\n"
        "you entered. Nothing is visible. That may not mean anything.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/catacomb_entrance",
        "south" : "/domains/Praxis/areas/splynn/catacomb_chamber"
    ]) );
    set_listen("default",
        "Nothing. The silence in here has weight. It presses on your ears.");
    set_smell("default",
        "Cold stone. Old dust. Nothing organic has been here in a long time.");
    set_items( ([
        "niches" : "Stone niches sealed with carved panels, stacked in rows\n"
                    "from floor to ceiling along both walls. The carving is intricate\n"
                    "and unfamiliar. This is not any tradition you recognize.",
        "panels" : "Carved stone panels sealing each niche. The figures on them\n"
                    "are not all human. Some have too many limbs. Some have none.",
        "figures" : "Etched into the stone: humans, creatures, beings without\n"
                    "clear anatomy. All of them in postures of supplication\n"
                    "toward something depicted above each niche entrance.",
        "hall" : "Long, low-ceilinged, perfectly straight. The construction\n"
                    "is older than anything above ground in Atlantis.",
        "cold" : "A cold that does not come from the rock. Something ahead\n"
                    "radiates it. It makes the air feel thin.",
        "script" : "There is script on the wall here - older and more deliberate\n"
                    "than the carved niches. Someone added this later. Type:\n"
                    " read script\n"
                    "to study it.",
        "writing" : "Script cut into the wall in a narrow column near the south end.\n"
                    "Type: read script",
        "glyphs" : "A column of unfamiliar glyphs along the south wall, different\n"
                    "from the niche carvings. Type: read script"
    ]) );
}

void init() {
    ::init();
    add_action("cmd_read_script", "read");
    add_action("cmd_read_script", "decipher");
}

int cmd_read_script(string str) {
    if(!str) return 0;
    str = lower_case(str);
    if(member_array(str, ({"script", "writing", "glyphs", "text",
                           "carving", "wall"})) == -1)
        return 0;
    write("The script is old and eroded, but deliberate. Someone wanted\n"
          "something recorded here without the authority of the niche-builders.\n\n"
          "You make out fragments:\n\n"
          " '...those who walk in shadow serve the order...'\n"
          " '...AERI... ...the first among them, the unnamed blade...'\n"
          " '...seek the rest where the dead carry knowledge...'\n\n"
          "A name cut short. AERI-something. You file it away.\n");
    this_player()->setenv("aerihman_clue_1", "1");
    return 1;
}

void reset() {
    ::reset();
}
