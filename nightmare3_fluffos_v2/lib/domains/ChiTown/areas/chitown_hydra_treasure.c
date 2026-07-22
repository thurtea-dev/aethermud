// /domains/ChiTown/areas/chitown_hydra_treasure.c
// Hidden recess north of the Chi-Town hydra lair.

#include <std.h>
#include <rooms.h>
#include <daemons.h>
#include <clock.h>

#define EQ_PATH "/domains/Praxis/equipment/"
/* "A few days" per the design brief: 3 game days. Defined identically
   at each flame_hilt spawn site -- see forest_lake_far_shore.c. */
#define FLAME_HILT_COOLDOWN (3 * DAY)

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Hidden Recess, Chi-Town Sewers");
    set_long(
        "A narrow recess behind the displaced rock. Cold stale air and the\n"
        "drip of distant water. Pre-Rifts masonry lines the walls.\n\n"
        "Metal glints in the shadows - more than one shape.");
    set_exits( ([
        "south" : "/domains/ChiTown/areas/chitown_hydra_lair"
    ]) );
}

void reset() {
    int on_cooldown;
    ::reset();
    if(!present("obsidian blade", this_object()))
        clone_object(EQ_PATH + "obsidian_blade.c")->move(this_object());
    /* Memories: metal hilt charged with PPE for a flame blade.
       World-wide dedup: another spawn site's hilt may already be
       loose. See forest_lake_far_shore.c for the pattern. */
    if(!present("flame hilt", this_object())) {
        on_cooldown = 0;
        catch(on_cooldown = (int)UNIQUE_ITEMS_D->query_taken_within(
            "flame_hilt", FLAME_HILT_COOLDOWN));
        if(!on_cooldown)
            clone_object(EQ_PATH + "flame_hilt.c")->move(this_object());
    }
}
