/* /d/Praxis/vehicles/mountaineer/mtn_cargo.c
   NG-V7 Mountaineer ATV: cargo hold. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("vehicle_interior", 1);
    set_short("NG-V7 Mountaineer: Cargo Hold");
    set_long(
        "The Mountaineer's cargo hold, a bare steel box lined with tie-down\n"
        "rails and ratchet straps. Scuffs and gouges on the deck mark years\n"
        "of crates, e-clips and salvage sliding around on bad roads. An\n"
        "empty weapon rack is welded to the forward bulkhead.\n\n"
        "The main cabin is west.");
    set_exits( ([
        "west" : "/domains/Praxis/vehicles/mountaineer/mtn_cabin"
    ]) );
    set_listen("default",
        "Ratchet straps swaying gently against the hull.");
    set_smell("default",
        "Diesel, rust, and burlap.");
    set_items( ([
        "rails" : "Tie-down rails running the length of the hold.",
        "straps" : "Ratchet straps, most still clipped to the rails.",
        "rack" : "A weapon rack sized for rifles. It is empty.",
        "deck" : "Gouged steel decking. Something heavy was dragged out of\n"
                 "here recently."
    ]) );
}

void reset() {
    ::reset();
}
