/* /std/blue_rift_portal.c
   Intermittent blue-tinted rift to the ocean off Splynn. Spawned by
   the Chi-Town boulevard's reset() on a random chance; nothing else
   should clone it. The blue tint is the only visible sign that this
   rift differs from Moxim's ordinary work. Lasts 120 seconds. */

#include <std.h>

inherit "/std/rift_portal";

#define OCEAN_ROOM "/domains/Praxis/areas/splynn/ocean_rift"

void create() {
    ::create();
    set_name("blue rift");
    set_id( ({ "rift", "blue rift", "portal", "rift portal",
               "blue portal" }) );
    set_short("a rift glowing deep blue");
    set_long(
        "A tear in the air, taller than a man, its edges burning a deep\n"
        "oceanic blue instead of the white shimmer of ordinary rift work.\n"
        "Cold, damp air drifts out of it, carrying the smell of salt\n"
        "water. Through the distortion there is only slow moving darkness,\n"
        "like looking up at the surface from deep water.\n"
        "Step through to travel.");
    set_destination(OCEAN_ROOM);
    /* Parent schedules a 30 second collapse; this rift holds longer. */
    remove_call_out("self_destruct");
    call_out("self_destruct", 120);
}
