/* /domains/Praxis/areas/splynn/ocean/ocean_rift_floor.c
   Ocean Rift Floor: Atlantean ruins at the base of the dimensional rift. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("water", 1);
    set_property("rest_allowed", 0);
    set_short("Ocean Rift Floor, Atlantean Ruins");
    set_long(
        "Crumbled stonework from a civilization that sank long before the Rifts.\n"
        "Columns carved with glyphs line what was once a great hall. The scale\n"
        "is wrong for human architecture: these columns were built for something\n"
        "larger. Much larger.\n\n"
        "The rift tears through the far wall, pulsing with cold light. The tear\n"
        "is old here, older than the current Rifts era. Whatever opened it did\n"
        "so before the coming of the Rifts, or time does not work normally at\n"
        "this depth.\n"
    );
    set_exits( ([
        "up" : "/domains/Praxis/areas/splynn/ocean/ocean_rift_deep"
    ]) );
    set_listen("default",
        "The low resonant pulse of the dimensional rift, much louder here.\n"
        "And silence otherwise. Everything living avoids this depth.");
    set_smell("default",
        "Ancient stone, salt-mineralized over thousands of years, and the\n"
        "acrid smell of a rift that has been open far too long.");
    set_items( ([
        "columns" : "Carved stone columns, each ten meters tall. The glyphs on them\n"
                    "are Atlantean: the written language of a civilization that sank\n"
                    "before recorded history. Some glyphs still glow faintly.",
        "glyphs" : "Atlantean script etched into the stone. You cannot read it\n"
                   "without Atlantean language skill. What you can see: the glyphs\n"
                   "repeat the same phrase in multiple registers, becoming more urgent.",
        "rift" : "A dimensional tear in the far wall of the ruins. Old. Larger\n"
                 "than the rift above. The light from it is cold and steady,\n"
                 "not the pulsing of new rift energy. This one has been open\n"
                 "for a very long time.",
        "stonework" : "Pre-Rifts construction, but not pre-Atlantis. These ruins\n"
                      "are older than anything that should exist here. The stone\n"
                      "is a type not found in any modern geological survey."
    ]) );
}
