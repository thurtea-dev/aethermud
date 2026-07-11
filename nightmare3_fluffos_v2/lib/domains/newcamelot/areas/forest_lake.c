// /domains/newcamelot/areas/forest_lake.c
// A still lake at the end of the Black Forest path.

#include <std.h>
#include <daemons.h>

inherit ROOM;

int do_swim(string str);

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("rest_allowed", 1);
    set_short("A Forest Lake");
    set_long(
        "The forest path opens onto a quiet lake. Dark water stretches\n"
        "to the far shore, ringed by ancient oaks. The surface is still\n"
        "except for occasional ripples from beneath. Something glints on\n"
        "the far bank, catching the light between the roots.\n\n"
        "The water looks cold and deep. You could try to swim across."
    );
    set_exits( ([
        "north" : "/domains/newcamelot/areas/black_forest"
    ]) );
    set_listen("default",
        "The creak of oaks. Quiet lapping of water against the bank.");
    set_smell("default",
        "Cold water, wet earth, and the green smell of old forest.");
    set_items( ([
        "lake"  : "Dark water stretching to a far shore. Still but for small ripples.",
        "water" : "Cold and dark. Deep enough to be dangerous.",
        "shore" : "The far bank is visible but too distant to reach without swimming.",
        "oaks"  : "Ancient oaks ring the lake, their roots reaching into the water.",
        "bank"  : "The near bank is muddy and root-tangled."
    ]) );
}

void init() {
    ::init();
    add_action("do_swim", "swim");
}

int do_swim(string str) {
    object player;
    int swim_pct;
    int roll;

    if(!str || (strsrch(lower_case(str), "lake") == -1 &&
                strsrch(lower_case(str), "across") == -1))
        return 0;

    player = this_player();

    if((int)player->query_property("swim_as_fish_active")) {
        tell_object(player,
            "You plunge into the cold water and swim across effortlessly.\n");
        tell_room(this_object(),
            (string)player->query_cap_name() +
            " dives into the lake and swims across.\n",
            ({ player }));
        player->move("/domains/newcamelot/areas/forest_lake_far_shore");
        return 1;
    }

    swim_pct = (int)RIFTS_SKILLS_D->query_skill_percent(player, "swimming");

    if(swim_pct >= 40) {
        tell_object(player,
            "You plunge into the cold water and swim across.\n");
        tell_room(this_object(),
            (string)player->query_cap_name() +
            " dives into the lake and swims across.\n",
            ({ player }));
        player->move("/domains/newcamelot/areas/forest_lake_far_shore");
        return 1;
    }

    if(swim_pct >= 1) {
        roll = random(100) + 1;
        if(roll <= swim_pct) {
            tell_object(player,
                "You struggle but make it across, gasping and dripping.\n");
            tell_room(this_object(),
                (string)player->query_cap_name() +
                " struggles across the lake and reaches the far shore.\n",
                ({ player }));
            player->move("/domains/newcamelot/areas/forest_lake_far_shore");
        } else {
            player->add_hp( -(random(6) + 1) );
            tell_object(player,
                "You struggle in the water and barely make it back to shore,\n"
                "coughing up lake water. The current is stronger than it looked.\n");
            tell_room(this_object(),
                (string)player->query_cap_name() +
                " thrashes in the water and hauls back to the bank, exhausted.\n",
                ({ player }));
        }
        return 1;
    }

    tell_object(player,
        "You would need to know how to swim to cross this lake.\n");
    return 1;
}

void reset() {
    ::reset();
}
