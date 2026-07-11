// /domains/Praxis/death/death_room.c
// The grey void -- Rifts resurrection chamber.
// Players arrive here when killed. Auto-resurrect fires after 2 minutes,
// or they can pray for a 30-second resurrect instead.

#include <std.h>
#include <daemons.h>

inherit ROOM;

void create() {
    room::create();
    set_properties(([ "light" : 3, "no magic" : 1, "no attack" : 1 ]));
    set_short("A Grey Void Between Worlds");
    set_long(
        "Silence. An absolute, oppressive silence broken only by distant\n"
        "echoes: screams, whispers, the crackle of a rift tearing open\n"
        "and sealing shut. The void is featureless: no sky, no ground,\n"
        "only a soft grey luminescence that seems to come from everywhere\n"
        "and nowhere.\n\n"
        "You feel the pull of the living world. A spectral presence drifts\n"
        "nearby, a guide of sorts, or perhaps simply the memory of one.\n"
        "It speaks without words: this is not your end. Return.\n\n"
        "The living world awaits. You will be drawn back automatically\n"
        "in two minutes. Type 'pray' to hasten your return.\n");
    set_listen("default",
        "Echoes of other deaths. Distant rifts tearing. Your own heartbeat, "
        "impossibly faint.");
    set_smell("default",
        "Ozone and something ancient: the smell of raw dimensional energy.");
    set_items(([
        "guide" : "A spectral shape, undefined. It radiates patience and faint sorrow.",
        "void" : "Grey in every direction. No horizon. No up or down.",
        "light" : "The grey luminescence has no source. It simply is.",
        "presence" : "Something watches over this space. Not malevolent. Waiting."
    ]));
}

void init() {
    ::init();
    if(!(int)this_player()->query_ghost()) return;
    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race())) return;
    // (Re)start the resurrect timer -- handles both fresh deaths and reconnections.
    this_player()->start_rifts_resurrect(120);
    message("my_action",
        "You will return to the living world in 2 minutes.\n"
        "Type 'pray' to hasten your resurrection to 30 seconds.\n",
        this_player());
}
