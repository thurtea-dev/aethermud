/* /domains/Praxis/equipment/vehicles/ng_wastelander.c
   Northern Gun NG-150 Wastelander motorcycle. Parks on East Road.
   MDC 25, fuel 30 units, fits one rider.
   Auto-returns to east_road1 300 seconds after last rider dismounts. */

#include <std.h>

inherit "/std/rifts_vehicle";

#define HOME_ROOM "/domains/Praxis/east_road1"
#define ABANDON_DELAY 300

void create() {
    ::create();
    set_name("wastelander motorcycle");
    set_id(({ "wastelander", "wastelander motorcycle", "motorcycle",
        "ng-150", "bike", "vehicle" }));
    set_short("an NG-150 Wastelander motorcycle");
    set_long(
        "A Northern Gun NG-150 Wastelander, the workhorse motorcycle of the\n"
        "post-apocalypse. Fat off-road tires, a bored-out engine that burns\n"
        "almost anything, and light MDC plating over the tank and fairing.\n"
        "MDC armor rating 25. A fuel gauge is set into the handlebars.\n\n"
        "Type 'enter' or 'board motorcycle' to get on. 'exit' or 'leave' to\n"
        "dismount. 'drive <direction>' to move while mounted.");
    set_mass(4000);
    set_max_encumbrance(2500);
    set_mdc(25);
    set_max_mdc(25);
    set_fuel(30);
    set_max_fuel(30);
    set_speed(1);
    set_value(6000);
}

static int has_riders() {
    object *inv;
    int i;

    inv = all_inventory(this_object());
    for(i = 0; i < sizeof(inv); i++)
        if(living(inv[i])) return 1;
    return 0;
}

void auto_return() {
    object env;

    if(has_riders()) return;
    env = environment(this_object());
    if(env) tell_room(env,
        "The Wastelander rumbles off toward East Road on its own.\n");
    move(HOME_ROOM);
}

int cmd_enter(string str) {
    int res;

    remove_call_out("auto_return");
    res = ::cmd_enter(str);
    return res;
}

int cmd_exit(string str) {
    int res;

    res = ::cmd_exit(str);
    if(!has_riders())
        call_out("auto_return", ABANDON_DELAY);
    return res;
}
