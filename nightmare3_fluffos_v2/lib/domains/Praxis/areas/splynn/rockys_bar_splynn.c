// /d/Praxis/areas/splynn/rockys_bar_splynn.c
// Rocky's Bar, Splynn branch. South of the Dimensional Market.
// Sells the armor talisman (100 MDC, rechargeable, PPE-activated).

#include <std.h>
#include <rooms.h>
#include <daemons.h>
#include <clock.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"
#define EQ_PATH "/domains/Praxis/equipment/"
/* Matches flame_hilt.c's cooldown convention: defined identically at
   each armor_talisman spawn site since neither inherits the item file. */
#define ARMOR_TALISMAN_COOLDOWN (3 * DAY)

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Rocky's Bar, Splynn");
    set_long(
        "A cramped establishment carved into the lower level of the Splynn\n"
        "Dimensional Market. The decor is a chaotic mix of salvage from a\n"
        "hundred different worlds: a neon sign in three scripts, furniture\n"
        "that defies anatomical expectations, and a bar made from a crashed\n"
        "shuttle fuselage. Rocky somehow opened a second location here.\n\n"
        "The real draw is the merchandise. Rocky keeps a selection of unusual\n"
        "items that do not appear in any standard catalog. The armor talisman\n"
        "behind the bar is the most talked-about. Ask Rocky about 'wares'\n"
        "or say 'rumor' to catch up on dimensional market gossip.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/splynn/splynn_market",
        "south" : "/domains/Praxis/areas/splynn/splynn_south_road"
    ]) );
    set_listen("default",
        "A dozen overlapping conversations in as many languages, "
        "and a music box playing something that should not be mathematically possible.");
    set_smell("default",
        "Alien spirits, ozone from dimensional proximity, and something burning that is probably decorative.");
    set_items( ([
        "bar" : "Hammered from a crashed shuttle fuselage. Surprisingly smooth.",
        "talisman" : "A disc of worked bone etched with Atlantean script. Rocky keeps it behind the bar.",
        "shuttle" : "The fuselage serves as the bar surface. It looks like it landed hard.",
        "sign" : "Three scripts, one of which is Atlantean. All three say ROCKY'S.",
        "furniture": "Chairs and tables sized for at least four different body plans."
    ]) );
}

void reset() {
    int on_cooldown;

    ::reset();
    if(!present("rocky", this_object()))
        clone_object(MONSTER_PATH+"rocky_barkeep.c")->move(this_object());
    on_cooldown = 0;
    catch(on_cooldown = (int)UNIQUE_ITEMS_D->query_taken_within(
        "armor_talisman", ARMOR_TALISMAN_COOLDOWN));
    if(!on_cooldown && !present("armor talisman", this_object()))
        clone_object(EQ_PATH+"armor_talisman.c")->move(this_object());
}
