// /d/Praxis/areas/splynn/preserve_nook.c
// Splynn Preserves hunting loop, R22: Rope Nook, a hidden alcove reached
// by rope beneath the High Catwalk.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Rope Nook");
    set_long(
        "A knotted rope hangs down from the catwalk above into a hollow\n"
        "formed where two of the bone-white trees grow close together.\n"
        "Whoever strung the catwalk also used this nook: a scattering of\n"
        "old brass casings and a rotted ration wrapper suggest a hunter\n"
        "waited here a long time for something that never came, or came\n"
        "and left no other trace.\n\n"
        "The only way out is up the rope, back to the catwalk.");
    set_exits( ([
        "up" : "/domains/Praxis/areas/splynn/preserve_catwalk"
    ]) );
    set_listen("default",
        "Creaking rope overhead, and the muffled sound of the high wind\n"
        "passing well above this hollow.");
    set_smell("default",
        "Rot and old brass, and beneath it the same cold reptilian scent\n"
        "that hangs around the lower blind.");
    set_items( ([
        "casings" : "A scattering of old brass casings, tarnished nearly\n"
                    "black. Whoever fired them has been gone a long time.",
        "wrapper"  : "A rotted ration wrapper, decades old by the look of\n"
                    "it.",
        "rope"     : "A knotted rope leading up to the catwalk above."
    ]) );
}

void reset() {
    ::reset();
    if(random(10) < 4 && !present("preserve beast", this_object()))
        clone_object(MONSTER_PATH+"preserve_beast.c")->move(this_object());
}
