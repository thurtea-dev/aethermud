// /domains/adm/vehicle_test/vt_ex5_pad.c
// EX-5 Behemoth parked in the wizard vehicle test zone.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("vehicle_accessible", 1);
    set_property("ex5_accessible", 1);
    set_short("Vehicle Test Zone: EX-5 Pad");
    set_long(
        "A reinforced permacrete pad marked for heavy vehicle testing.\n"
        "The EX-5 Behemoth stands dormant here, hull access open on the\n"
        "lower torso. Staff use only.\n\n"
        "Type 'enter ex-5' or 'enter behemoth' to board.\n"
        "The staging bay is west. The test loop continues south.");
    set_exits( ([
        "west"  : "/domains/adm/vehicle_test/vt_main",
        "south" : "/domains/adm/vehicle_test/vt_east"
    ]) );
    set_items( ([
        "ex-5" : "The EX-5 Behemoth. Type 'enter ex-5' to board.",
        "behemoth" : "The EX-5 Behemoth. Type 'enter ex-5' to board.",
        "pad" : "Reinforced permacrete rated for two hundred metric tons."
    ]) );
}

void init() {
    ::init();
    add_action("cmd_enter_ex5", "enter");
    add_action("cmd_unlock_ex5", "unlock");
}

int cmd_unlock_ex5(string str) {
    int pin;

    if(!str || sscanf(str, "%d", pin) != 1) return 0;
    if(pin < 0 || pin > 9999) {
        write("Syntax: unlock <4-digit pin>\n");
        return 1;
    }
    if((int)EX5_D->try_unlock_hull(pin)) {
        write("The EX-5 access hatch unlocks with a heavy clunk.\n");
        return 1;
    }
    write("Incorrect PIN.\n");
    return 1;
}

int cmd_enter_ex5(string str) {
    object hull;

    if(!str) return 0;
    str = lower_case(str);
    if(strsrch(str, "ex-5") == -1 && strsrch(str, "ex5") == -1 &&
       strsrch(str, "behemoth") == -1) return 0;
    if((int)EX5_D->query_hull_locked()) {
        write("The EX-5 access hatch is locked. Type 'unlock <pin>' from outside.\n");
        return 1;
    }
    hull = load_object("/domains/Praxis/vehicles/ex5/ex5_hull");
    if(!hull) {
        write("The EX-5 cannot be entered right now.\n");
        return 1;
    }
    catch(EX5_D->set_current_room("/domains/adm/vehicle_test/vt_ex5_pad"));
    if(this_player()->move(hull)) {
        write("You cannot enter the EX-5.\n");
        return 1;
    }
    write("You climb through the hull access hatch into the EX-5 Behemoth.\n");
    this_player()->describe_current_room(1);
    return 1;
}
