/* /d/Praxis/areas/hydra_lair.c
   Wide cavern west of the nexus approach. A massive hydra guards it.
   Push the flat rock to reveal the obsidian blade. */

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
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_property("rest_allowed", 0);
    set_short("The Hydra Lair");
    set_long(
        "A wide cavern whose ceiling is lost in shadow. The smell of reptile\n"
        "musk is overwhelming. Bones litter the floor, some still wearing\n"
        "fragments of armor. The walls are slick with moisture.\n\n"
        "A large flat rock sits against the north wall, half-buried in debris.\n"
        "Something about it seems slightly out of place.");
    set_exits( ([
        "east" : "/domains/Praxis/areas/nexus_approach"
    ]) );
    set_listen("default",
        "A low rhythmic rumbling. Wet. The hydra breathing.");
    set_smell("default",
        "Reptile musk and old blood. Almost physical in its intensity.");
    set_items( ([
        "bones"  : "Bones of previous visitors. Some are fresh.",
        "walls"  : "Slick stone walls carved by moisture and time.",
        "rock"   : "A large flat rock pressed against the north wall. It looks moveable.\n"
                   "Try pushing it after the hydra is gone.",
        "debris" : "Rubble and bone chips covering the cavern floor."
    ]) );
}

int do_push(string str) {
    if(!str || strsrch(lower_case(str), "rock") == -1)
        return 0;

    if(present("hydra", this_object())) {
        write("The hydra blocks access to the rock.\n");
        return 1;
    }
    if((int)query_property("rock_pushed")) {
        write("The rock has already been moved aside.\n");
        return 1;
    }

    set_property("rock_pushed", 1);
    add_exit("north", "/domains/Praxis/areas/hydra_treasure");
    write("You strain against the rock. With a grinding scrape it shifts aside,\n"
          "revealing a dark passage to the north.\n");
    this_player()->tell_room_living(environment(this_object()),
        this_player(), 0,
        " pushes the rock aside, revealing a hidden passage north.\n");
    return 1;
}

void init() {
    ::init();
    add_action("do_push", "push");
}

void reset() {
    object hilt;
    int on_cooldown;
    ::reset();
    if(!present("hydra", this_object()))
        clone_object("/domains/Praxis/monsters/hydra")->move(this_object());
    if(!present("hilt", this_object()) && !random(3)) {
        /* World-wide dedup: another spawn site's hilt may already be
           loose. See forest_lake_far_shore.c for the pattern. */
        on_cooldown = 0;
        catch(on_cooldown = (int)UNIQUE_ITEMS_D->query_taken_within(
            "flame_hilt", FLAME_HILT_COOLDOWN));
        if(!on_cooldown) {
            hilt = clone_object(EQ_PATH + "flame_hilt");
            if(hilt) hilt->move(this_object());
        }
    }
}
