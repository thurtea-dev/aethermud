// /d/Praxis/equipment/hover_cycle.c
// Pre-Rifts-era hover cycle -- parks in Monument Square.
// MDC 40, fuel 20 units, fits one rider.
// Auto-returns to monument_square 300 seconds after last rider dismounts.

#include <std.h>

inherit "/std/rifts_vehicle";

#define HOME_ROOM "/domains/Praxis/areas/monument_square"
#define ABANDON_DELAY 300

void create() {
    ::create();
    set_name("hover cycle");
    set_id(({ "hover cycle", "hovercycle", "cycle", "bike", "vehicle" }));
    set_short("a hover cycle");
    set_long(
        "A single-rider hover cycle, pre-Rifts manufacture but clearly maintained\n"
        "by someone who knows what they're doing. The repulsor skids have been\n"
        "patched with welded salvage plate. MDC armor rating 40. A fuel gauge\n"
        "on the handlebars shows current reserves.\n\n"
        "Type 'enter' or 'board cycle' to get on. 'exit' or 'leave' to dismount.\n"
        "'drive <direction>' to move while mounted.");
    set_mass(5000);
    set_max_encumbrance(3000);
    set_mdc(40);
    set_max_mdc(40);
    set_fuel(20);
    set_max_fuel(20);
    set_speed(1);
    set_value(8000);
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
        "The hover cycle drifts back toward town on autopilot.\n");
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
