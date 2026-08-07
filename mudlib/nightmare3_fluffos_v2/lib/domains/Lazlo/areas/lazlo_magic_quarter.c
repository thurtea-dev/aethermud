// /domains/Lazlo/areas/lazlo_magic_quarter.c
// The magic quarter of Lazlo. Ley line walkers practice openly.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("ley_line_room", 1);
    set_short("Lazlo, Magic Quarter");
    set_long(
        "A street where magic is not concealed. Ley line walkers channel\n"
        "energy openly on the corners. An Alchemist's workshop vents\n"
        "colored smoke through iron pipes. Students sit on steps with\n"
        "spell-formula books open across their knees, muttering memorization.\n\n"
        "The air feels different here. Slightly charged. PPE is thick and\n"
        "close to the surface. The central plaza is east. A narrow path\n"
        "leads north to the ley line shrine.\n\n"
        "A ley line instructor stands near the practice area. Say 'learn'\n"
        "to hear what languages they can teach.");
    set_exits( ([
        "east" : "/domains/Lazlo/areas/lazlo_square",
        "north" : "/domains/Lazlo/areas/lazlo_ley_shrine"
    ]) );
    set_listen("default",
        "The crackle of discharged PPE, quiet chanting, and pages turning.");
    set_smell("default",
        "Ozone, burnt components, and the clean electric smell of ley line energy.");
    set_items( ([
        "walkers" : "Ley line walkers practicing control exercises. "
                       "The energy around them is visible.",
        "workshop" : "An Alchemist's workshop, sealed except for the exhaust pipes.",
        "students" : "A cluster of young mages on the steps with spell books. "
                       "They barely notice you.",
        "smoke" : "Colored smoke venting from the Alchemist's workshop. "
                       "It doesn't smell dangerous.",
        "instructor" : "A senior mage in Lazlo academic robes. They teach languages."
    ]) );
}

void reset() {
    ::reset();
    if(!present("ley line instructor", this_object()))
        clone_object("/domains/Lazlo/monsters/ley_line_instructor")->move(this_object());
    if(!present("mage-vendor", this_object()))
        clone_object("/domains/Lazlo/npcs/lazlo_mage_vendor.c")->move(this_object());
}
