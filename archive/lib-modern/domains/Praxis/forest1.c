#include <std.h>

inherit ROOM;

#define MON_PATH "/domains/Praxis/monsters/"

void create() {
    ::create();
    set_property("light", 3);
    set_property("no castle", 1);
    set_short("North Forest near Praxis");
    set_long(
        "A vast forest north of Praxis. Ley-line runoff has warped some trees\n"
        "into twisted giants. A narrow path leads deeper north and back toward\n"
        "the city roads south.");
    set_exits(
              (["north" : "/domains/Praxis/forest2",
                "south" : "/domains/Praxis/n_centre2"]) );
    set_items(
        (["path" : "A worn trail through rift-touched woodland.",
          "forest" : "North Forest. Travelers report wolves and worse.",
          "trees" : "Some trunks bear scorch marks from dimensional storms."]) );
}

void reset() {
    int i;
    ::reset();
    if(!present("plains wolf", this_object())) {
        i = 1 + random(2);
        while(i-- > 0)
            clone_object(MON_PATH+"plains_wolf.c")->move(this_object());
    }
}

