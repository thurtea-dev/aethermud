#include <std.h>

inherit ROOM;

void init() {
    ::init();
    add_action("read", "read");
}

void create() {
    ::create();
    set_properties( ([ "light": 2, "indoors" : 1, "no castle" : 1 ]) );
    set_short("The Rusty Gyro");
    set_long(
        "The Rusty Gyro is Praxis's busiest drinking hole. Salvaged neon\n"
        "flickers over a permacrete floor sticky with spilled synth-ale.\n"
        "Mercenaries, smugglers, and CS deserters share benches under\n"
        "wanted posters for Rifts-era bounties. A laminated menu hangs\n"
        "behind the bar.\n\n"
        "Sun Alley leads north. The south square is east.");
       set_items(
	(["pub" : "The most hoppin' place in Praxis.",
	  "wall" : "The walls are covered with various pictures.",
	  ({ "posters", "poster", "wanted posters" }) :
	  "The wanted posters are very old and brittle. They mark "
	  "the faces of the rogues of this land.",
	  ({ "pictures" , "picture" }) :
	  "The pictures are of different parts of Rifts Earth, painted "
	  "by several of the villages' aspiring artists.",
	  "bar" : "The bar is actually Lars' Tavern. It is a very "
	  "clean and well run establishment.",
	  "counter" : "The wooden counter stands at the back of the "
	  "room and houses all of Lars' drinks.",
	  "room" : "You are in the main room of Lars' Tavern.",
	  "foo" : "bar",
	  "tavern" : "The tavern is the place that the people of "
	  "Praxis come to when they want to have a drink or just "
	  "be social.",
	  "floor" : "The floor is made out of solid oak.",
	  "list" : "It looks like a list of drinks. Read it to see "
	  "what's available."]) );
    set_exits(
	      (["north" : "/domains/Praxis/w_boc_la1",
		"east" : "/domains/Praxis/s_centre1"]) );
    set_properties( ([ "no castle": 1, "no attack" : 1 ]) );
    }

void reset() {
  ::reset();
    if(!present("list"))
      new("std/obj/player_list")->move(this_object());
    if(!present("lars"))
      new("/domains/Praxis/obj/mon/lars")->move(this_object());
}

int read(string str) {
    object ob;
    int i;

    ob = present("lars");
    if(!ob) {
        write("You cannot read the menu, as it is splattered with blood.");
	return 1;
    }
    message("Ninfo", "The following classic drinks are served at the Lars Pub!\n", this_player());
    message("Ninfo", "-----------------------------------------------------------\n", this_player());
    message("Ninfo", "A firebreather\t\t\t"+ (int)ob->get_price("firebreather")+" gold\n", this_player());
    message("Ninfo", "A special of the house\t\t"+(int)ob->get_price("special")+" gold\n", this_player());
    message("Ninfo", "A pale ale\t\t\t"+(int)ob->get_price("ale")+" gold\n", this_player());
    message("Ninfo", "-----------------------------------------------------------\n", this_player());
    message("Ninfo", "<buy drink_name> gets you a drink.\n", this_player());
    return 1;
}









