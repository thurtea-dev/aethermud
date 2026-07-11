// /domains/chitown/areas/chitown_hydra_lair.c
// Sewer cavern beneath Chi-Town. A hydra guards a hidden blade recess.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_short("Sewer Cavern, Chi-Town");
    set_long(
        "The sewer tunnel opens into a wide cavern beneath the burbs. Bones\n"
        "and rusted armor litter the floor. The smell of reptile musk is\n"
        "overwhelming.\n\n"
        "A large flat rock sits against the north wall, half-buried in debris.\n"
        "Something about it seems slightly out of place.\n\n"
        "The overflow chamber is west.");
    set_exits( ([
        "west" : "/domains/chitown/areas/chitown_sewer_overflow"
    ]) );
    set_listen("default",
        "A low rhythmic rumbling. Wet. Something large breathing.");
    set_smell("default",
        "Reptile musk and old blood.");
    set_items( ([
        "rock" : "A large flat rock pressed against the north wall. Try pushing it.",
        "bones" : "Bones of previous visitors.",
        "cavern" : "A natural widening of the old sewer system."
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
    add_exit("north", "/domains/chitown/areas/chitown_hydra_treasure");
    write("You strain against the rock. With a grinding scrape it shifts aside,\n"
          "revealing a dark passage to the north.\n");
    this_player()->tell_room_living(environment(this_object()),
        this_player(), 0, " pushes the rock aside, revealing a hidden passage north.\n");
    return 1;
}

void init() {
    ::init();
    add_action("do_push", "push");
}

void reset() {
    ::reset();
    if(!present("hydra", this_object()))
        clone_object("/domains/Praxis/monsters/hydra")->move(this_object());
}
