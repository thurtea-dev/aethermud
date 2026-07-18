// /domains/newcamelot/areas/forest_lake_far_shore.c
// Far bank of the forest lake. The flame hilt rests here.
// Respawn: seeded once on first reset, then once per game day (DAY = 24000s)
// after it leaves the room. Pattern follows hovertrain.c find_call_out guards.

#include <std.h>
#include <daemons.h>
#include <clock.h>

inherit ROOM;

private int __hilt_seeded;

void spawn_flame_hilt();
int hilt_present();
int do_swim(string str);

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("rest_allowed", 1);
    set_short("Far Shore of the Forest Lake");
    set_long(
        "The opposite bank of the lake. Thick roots grip muddy soil.\n"
        "Water drips from your clothes. Through the trees, faint lights\n"
        "suggest a distant settlement deeper in the forest. The lake\n"
        "stretches back to where you came from.\n\n"
        "Half-hidden in the roots, something catches the light."
    );
    set_exits( ([
        "north" : "/domains/newcamelot/areas/forest_lake"
    ]) );
    set_listen("default",
        "Wind in the high canopy. The distant settlement is very quiet.");
    set_smell("default",
        "Wet earth, bark, and the cold clean smell of the lake behind you.");
    set_items( ([
        "roots"      : "Thick gnarled roots from the ancient oaks. Something is tucked between them.",
        "lake"       : "The lake stretches north. You would need to swim back.",
        "lights"     : "A faint glow through the trees to the south. Far off.",
        "settlement" : "Too far to see clearly. It could be a village or a ruin.",
        "bank"       : "Muddy soil and root tangles. Wet from the lake."
    ]) );
    __hilt_seeded = 0;
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
                strsrch(lower_case(str), "back") == -1 &&
                strsrch(lower_case(str), "across") == -1))
        return 0;

    player = this_player();

    if((int)player->query_property("swim_as_fish_active")) {
        tell_object(player,
            "You dive back into the cold water and swim across easily.\n");
        tell_room(this_object(),
            (string)player->query_cap_name() +
            " dives into the lake and swims back.\n",
            ({ player }));
        player->move("/domains/newcamelot/areas/forest_lake");
        return 1;
    }

    swim_pct = (int)RIFTS_SKILLS_D->query_skill_percent(player, "swimming");

    if(swim_pct >= 40) {
        tell_object(player,
            "You dive back into the cold water and swim across.\n");
        tell_room(this_object(),
            (string)player->query_cap_name() +
            " dives back into the lake.\n",
            ({ player }));
        player->move("/domains/newcamelot/areas/forest_lake");
        return 1;
    }

    if(swim_pct >= 1) {
        roll = random(100) + 1;
        if(roll <= swim_pct) {
            tell_object(player,
                "You struggle through the cold water and haul yourself back.\n");
            player->move("/domains/newcamelot/areas/forest_lake");
        } else {
            player->add_hp( -(random(6) + 1) );
            tell_object(player,
                "You thrash in the water and drag yourself back to the far shore,\n"
                "coughing and exhausted. The current fought you.\n");
        }
        return 1;
    }

    tell_object(player,
        "You would need to know how to swim to cross the lake.\n");
    return 1;
}

int hilt_present() {
    object *inv;
    int i;

    inv = all_inventory(this_object());
    for(i = 0; i < sizeof(inv); i++) {
        if(strsrch(base_name(inv[i]), "flame_hilt") != -1 ||
           strsrch(base_name(inv[i]), "metal_hilt") != -1)
            return 1;
    }
    return 0;
}

void spawn_flame_hilt() {
    object hilt;

    if(hilt_present()) {
        __hilt_seeded = 1;
        return;
    }
    hilt = clone_object("/domains/Praxis/equipment/flame_hilt");
    if(hilt) hilt->move(this_object());
    __hilt_seeded = 1;
}

void reset() {
    ::reset();

    /* Already here: nothing to do. */
    if(hilt_present()) {
        __hilt_seeded = 1;
        return;
    }

    /* First load after boot: seed once so the shore is not empty. */
    if(!__hilt_seeded) {
        spawn_flame_hilt();
        return;
    }

    /* Taken: respawn once per game day (DAY from clock.h = 24000s / 6h40m).
       find_call_out guard matches hovertrain.c so reset() does not stack timers. */
    if(find_call_out("spawn_flame_hilt") == -1)
        call_out("spawn_flame_hilt", DAY);
}
