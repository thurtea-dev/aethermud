#include <std.h>

inherit ROOM;

void create() {
  ::create();
    set_property("light", 2);
    set_property("night light", 1);
    set_property("no castle", 1);
set_listen_string( "default", "The quiet chirp of grasshoppers hangs "
"in the air." );
    set_short( "the crossing of East Road and Sun Alley");
    set_long(
	"Sun Alley, named for its' beautiful view of the sunsets, shoots "
"off west from East Road. East Road continues, heading north and "
"south upon the eastern border of town. To the east, you can see "
"the Praxis cemetary, shaded by large trees. The air is very still "
"here, and a feeling of foreboding hangs in the air. An armored\n"
"all-terrain vehicle, a Northern Gun NG-V7 Mountaineer, is parked\n"
"on the roadside here. Type 'enter mountaineer' to board it.");
    set_items(
	(["alley" : "A bright and cheery alley which marks the northern "
 	    "boarder of Praxis.",
	  "road" : "East Road is a curvy road running north and south "
	    "on the east side of Praxis.",
	  "cemetary" : "The residents of Praxis get buried there. It "
	    "looks spooky.",
"trees" : "The trees are large drooping willow trees.",
	  "forest" : "A great forest the marks the northern end of Praxis.",
	  "mountaineer" : "A Northern Gun NG-V7 Mountaineer: an armored "
	    "all-terrain RV on six huge run-flat tires. Its boarding door "
	    "hangs open. Type 'enter mountaineer' to climb inside.",
	  "vehicle" : "The parked NG-V7 Mountaineer ATV. Type 'enter "
	    "mountaineer' to board it.",
	  "atv" : "The parked NG-V7 Mountaineer ATV. Type 'enter "
	    "mountaineer' to board it."])
    );
    set_exits(
	      (["north" : "/domains/Praxis/east_road3",
		"south" : "/domains/Praxis/east_road1",
		"east" : "/domains/Praxis/cemetary/grave_yard",
		"west"	 : "/domains/Praxis/sun2"]) );
}

void init() {
    ::init();
    add_action("cmd_enter_mountaineer", "enter");
}

int cmd_enter_mountaineer(string str) {
    object cabin;

    if(!str) return 0;
    str = lower_case(str);
    if(strsrch(str, "mountaineer") == -1 && strsrch(str, "atv") == -1 &&
       strsrch(str, "vehicle") == -1 && strsrch(str, "ng-v7") == -1) return 0;
    cabin = load_object("/domains/Praxis/vehicles/mountaineer/mtn_cabin");
    if(!cabin) {
        write("The Mountaineer cannot be entered right now.\n");
        return 1;
    }
    if(this_player()->move(cabin)) {
        write("You cannot enter the Mountaineer.\n");
        return 1;
    }
    write("You climb through the boarding door into the Mountaineer.\n");
    tell_room(this_object(),
        this_player()->query_cap_name() + " climbs into the Mountaineer ATV.\n",
        ({ this_player() }));
    this_player()->describe_current_room(1);
    return 1;
}



