// /domains/ChiTown/areas/burb_water_pump.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("rest_allowed", 1);
    set_short("Community Water Pump");
    set_long(
        "A rusted hand pump stands over a concrete basin, the only reliable\n"
        "clean water for several blocks. Plastic jugs and metal cans wait in\n"
        "a loose line. Someone has painted a faded blue circle on the basin\n"
        "rim - a local mark that this place is shared, not claimed.\n\n"
        "Preacher's square is north. East leads toward gang territory.");
    set_exits( ([
        "north" : "/domains/ChiTown/areas/burb_preacher_square",
        "east"  : "/domains/ChiTown/areas/burb_gang_corner"
    ]) );
    set_listen("default",
        "The clank of the pump handle and water hitting plastic.");
    set_smell("default",
        "Wet concrete and soap someone used to wash a child nearby.");
    set_items( ([
        "pump" : "Hand-operated, patched with wire and hope.",
        "basin" : "Stained concrete. The blue circle paint is peeling.",
        "jugs" : "A mix of pre-Rifts bottles and Coalition ration cans."
    ]) );
}
