// /domains/Praxis/areas/splynn/alvurron/alvurron_rift_chamber.c
// Controlled dimensional rift, deepest accessible point of Alvurron.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Alvurron, Rift Chamber");
    set_long(
        "A contained dimensional rift fills the far wall, held open by\n"
        "carved stone fittings and something that is not technology. The air\n"
        "crackles with PPE discharge and the witch-lamps along the walls dim\n"
        "and brighten with no pattern. Whatever the rift leads to, even the\n"
        "gargoyles of Alvurron treat it with visible caution. The passage up\n"
        "leads back to the inner district.");
    set_exits( ([
        "up" : "/domains/Praxis/areas/splynn/alvurron/alvurron_inner_district",
        "down" : "/domains/Praxis/areas/splynn/alvurron/alvurron_deep_rift"
    ]) );
    set_listen("default",
        "A low crackle, rising and falling, like static before a storm.");
    set_smell("default", "Ozone, sharp enough to taste.");
    set_items( ([
        "rift" : "A contained dimensional rift, held open by carved stone fittings\n"
                 "and something that is not technology. Looking into it too long\n"
                 "makes your eyes ache.",
        "fittings" : "Worked black stone fittings anchor the rift to this wall.\n"
                 "The carving is older than anything else in the city."
    ]) );
}
