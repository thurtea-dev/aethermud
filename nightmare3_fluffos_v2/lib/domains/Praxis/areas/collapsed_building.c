// /d/Praxis/areas/collapsed_building.c
// Ruined office building east of the nexus approach.
// Two wild vampires lair here. Pre-Rifts salvage is scattered throughout.

#include <std.h>
#include <rooms.h>
#include <daemons.h>
#include <clock.h>

/* "A few days" per the design brief: 3 game days. Defined identically
   at each flame_hilt spawn site -- see forest_lake_far_shore.c. */
#define FLAME_HILT_COOLDOWN (3 * DAY)

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Collapsed Office Building");
    set_long(
        "What was once a multi-story office building is now a horizontal ruin,\n"
        "the upper floors collapsed inward leaving a cavernous ground-level\n"
        "space. Energy-bleached vines have pushed through every crack. The\n"
        "ley line proximity has accelerated their growth into something that does\n"
        "not look entirely plant.\n\n"
        "Pre-Rifts detritus is everywhere: office furniture reduced to metal\n"
        "frames, scattered equipment housings, cabling hanging from collapsed\n"
        "ceiling panels. Someone has been sorting through it. Recently.\n\n"
        "The air smells of old blood and something colder. You are not alone here.");
    set_exits( ([
        "west" : "/domains/Praxis/areas/nexus_approach"
    ]) );
    set_listen("default",
        "Dripping water and something else, a slow rhythmic sound, like breathing, but wrong.");
    set_smell("default",
        "Old blood. Cold meat. The chemical smell of ley line energy seeping through the walls.");
    set_items( ([
        "furniture" : "Metal office chair frames and desk legs, the non-metal components long since rotted away.",
        "vines" : "Not entirely natural. The energy-bleached vines pulse faintly in low light. Do not touch them.",
        "equipment" : "Pre-Rifts computer housings, monitors stripped of screens, server racks. Salvageable material.",
        "cabling" : "Heavy cable runs hanging from a collapsed ceiling panel. Still attached to something up there.",
        "ceiling" : "Collapsed. The floors above came down in sequence, which is why anything on the ground survived.",
        "salvage" : "Pre-Rifts salvage scattered across the floor, sorted into rough piles by something with hands."
    ]) );

    // Initial spawn
    new("/domains/Praxis/monsters/wild_vampire")->move(this_object());
    new("/domains/Praxis/monsters/wild_vampire")->move(this_object());
    new("/domains/Praxis/equipment/salvage_item")->move(this_object());
    new("/domains/Praxis/equipment/salvage_item")->move(this_object());
    new("/domains/Praxis/equipment/salvage_item")->move(this_object());
}

void reset() {
    object *inv;
    object chest;
    int i, vcount;
    int on_cooldown;

    inv = all_inventory(this_object());
    i = sizeof(inv);
    vcount = 0;
    while(i--) {
        if(living(inv[i]) &&
           strsrch((string)inv[i]->query_short(), "vampire") != -1)
            vcount++;
    }
    while(vcount < 2) {
        new("/domains/Praxis/monsters/wild_vampire")->move(this_object());
        vcount++;
    }
    if(!present("salvage", this_object())) {
        new("/domains/Praxis/equipment/salvage_item")->move(this_object());
        new("/domains/Praxis/equipment/salvage_item")->move(this_object());
        new("/domains/Praxis/equipment/salvage_item")->move(this_object());
    }
    chest = present("chest", this_object());
    if(!chest) {
        chest = clone_object("/domains/Praxis/equipment/locked_chest");
        if(chest) chest->move(this_object());
    }
    /* locked_chest.c only clones its content list at pick-lock time, not
       at spawn time (see cmd_pick()), so re-set the content list on every
       reset() rather than once at chest creation -- otherwise a chest that
       spawned during a cooldown would keep an empty content list forever
       even after the cooldown clears. Only touch it while still locked:
       once picked, __content_files is already consumed/cleared and
       re-stocking a looted chest would defeat the dedup entirely. */
    if(chest && (int)chest->query_locked()) {
        on_cooldown = 0;
        catch(on_cooldown = (int)UNIQUE_ITEMS_D->query_taken_within(
            "flame_hilt", FLAME_HILT_COOLDOWN));
        chest->set_chest_contents(on_cooldown ? ({}) :
            ({ "/domains/Praxis/equipment/flame_hilt" }));
    }
}

