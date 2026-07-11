/* /domains/newcamelot/areas/camelot_garden.c
   The walled court garden behind the New Camelot plaza.
   A central fountain of magical water. Animated thornbushes line the paths.
   Entry from camelot_square via the garden gate. */

#include <std.h>
#include <rooms.h>

#define BUSH_PATH "/domains/Praxis/monsters/magic_bush"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("rest_allowed", 1);
    set_property("no attack", 0);
    set_short("New Camelot, Court Garden");
    set_long(
        "A walled garden enclosed by pale stone. Flagstone paths wind between\n"
        "beds of herbs and small trees brought from across the world, tended\n"
        "by the Order's groundskeepers. The air is noticeably warmer here\n"
        "than in the plaza outside, protected from the wind by the walls.\n\n"
        "At the center of the garden stands a stone fountain. The water is\n"
        "clear and faintly luminous, catching the light in ways that suggest\n"
        "ley line influence. Drinking from it feels faintly restorative.\n\n"
        "Thornbushes with silver-shimmering leaves line the outer paths.\n"
        "They are animated: the Order's groundskeepers cultivated them as\n"
        "a living perimeter barrier. They are not aggressive unless threatened.\n\n"
        "Exit: south back to the main plaza.");
    set_exits( ([
        "south" : "/domains/newcamelot/areas/camelot_square",
        "north" : "/domains/newcamelot/areas/castle_gatehouse"
    ]) );
    set_listen("default",
        "The quiet splash of the fountain, birdsong, and the soft rustling\n"
        "of the thornbushes even when the air is still.");
    set_smell("default",
        "Herbs, fresh water, and a faint magical warmth, like sun on stone.");
    set_items( ([
        "fountain" : "A stone fountain at the center of the garden. The water\n"
                     "is clear and faintly luminous. Drinking from it leaves\n"
                     "a sense of clarity. Type 'drink fountain' to drink.",
        "water"    : "Clear water in the fountain. It glows slightly. The ley\n"
                     "lines beneath the garden feed it.",
        "paths"    : "Flagstone paths winding between garden beds and the fountain.\n"
                     "Worn smooth by decades of foot traffic.",
        "walls"    : "Pale stone walls enclosing the garden. They keep out the\n"
                     "wind but not the light.",
        "herbs"    : "Herb beds maintained by the Order's groundskeepers.\n"
                     "Some of the species are not native to Earth.",
        "trees"    : "Small trees from multiple continents. Some look like they\n"
                     "should not be alive in this climate. Magic helps.",
        "gate"     : "The garden gate to the south, back to the main plaza.",
        "thornbushes" : "Animated silver-leafed thornbushes lining the outer paths.\n"
                     "They sway even without wind. The thorns are hard as iron.\n"
                     "Leave them alone and they leave you alone.",
        "leaves"   : "Silver-sheened leaves that shimmer faintly. A side effect\n"
                     "of the ley line energy that saturates the garden soil."
    ]) );
}

void init() {
    ::init();
    add_action("cmd_drink_fountain", "drink");
}

int cmd_drink_fountain(string str) {
    int hp;
    int max_hp;

    if(!str) return 0;
    str = lower_case(str);
    if(strsrch(str, "fountain") == -1 && strsrch(str, "water") == -1) return 0;
    hp = (int)this_player()->query_stats("rifts_hp");
    max_hp = (int)this_player()->query_stats("max_rifts_hp");
    if(hp >= max_hp) {
        write("You drink from the fountain. The water is cool and clear,\n"
              "tasting faintly of minerals. You feel no different.\n");
        return 1;
    }
    this_player()->set_stats("rifts_hp", hp + 5 > max_hp ? max_hp : hp + 5);
    write("You cup water from the fountain and drink. It is cool and luminous\n"
          "on your tongue. A small warmth spreads through you. (5 HP restored)\n");
    return 1;
}

void reset() {
    ::reset();
    if(!present("magic bush", this_object()))
        clone_object(BUSH_PATH)->move(this_object());
    if(sizeof(all_inventory(this_object())) < 3)
        clone_object(BUSH_PATH)->move(this_object());
}
