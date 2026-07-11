/* /domains/Horton/areas/hovertrain.c
   Horton <-> Chi-Town hovertrain. Runs every 5 minutes, costs 50 credits.
   Board via 'board train' at either station. Disembark with 'disembark'. */

#include <std.h>
#include <rooms.h>

#define HORTON_STATION  "/domains/Horton/areas/horton_station"
#define CHITOWN_BURBS   "/domains/chitown/areas/chitown_burbs"
#define TICKET_PRICE    50
#define CYCLE_TIME      300
#define TRANSIT_SECS    15
#define ARRIVE_SECS     5

inherit ROOM;

private int __at_horton;
private int __in_transit;
private int __next_dep_time;

void depart();
void arrive_announce();
void arrive();

void create() {
    ::create();
    set_property("no_clean", 1);
    set_property("vehicle_room", 1);
    set_property("light", 3);
    set_property("indoors", 1);
    set_short("Hovertrain");
    set_long(
        "A clean, pressurized hovertrain car. Padded bench seats run along\n"
        "both walls. Narrow windows show the flat Illinois plains streaming\n"
        "past at speed. The air smells of recycled coolness and new plastic.\n"
        "Soft lighting strips run the length of the ceiling.\n\n"
        "An automated chime sounds occasionally. The ride between stations\n"
        "takes about fifteen minutes.\n\n"
        "Type 'disembark' to exit at the next station stop.\n"
        "The train is moving. Wait for the stop announcement.");
    set_exits( ([]) );
    set_items( ([
        "seats"   : "Padded bench seating, bolted to the floor. Clean.",
        "windows" : "Small, reinforced. Illinois flats stretching to the horizon.",
        "doors"   : "Sealed. They open only at stations.",
        "lights"  : "Soft ceiling strips. Better than nothing."
    ]) );

    __at_horton     = 1;
    __in_transit    = 0;
    __next_dep_time = time() + CYCLE_TIME;
    call_out("depart", CYCLE_TIME);
}

void init() {
    ::init();
    add_action("cmd_disembark", "disembark");
    add_action("cmd_disembark", "exit");
}

/*
 * reset() restarts the departure call_out if a warmboot or other event
 * destroyed the scheduled call.  find_call_out() returns -1 when no such
 * call is pending; we reschedule only in that case to avoid double-firing.
 */
void reset() {
    ::reset();
    if(find_call_out("depart") == -1 && !__in_transit) {
        __next_dep_time = time() + CYCLE_TIME;
        call_out("depart", CYCLE_TIME);
    }
}

int cmd_disembark(string str) {
    string dest;

    if(__in_transit) {
        write("The train is moving. Wait for the next stop.\n");
        return 1;
    }
    dest = __at_horton ? HORTON_STATION : CHITOWN_BURBS;
    this_player()->move(dest);
    return 1;
}

/* Called from station rooms. */
int board_player(object player) {
    int credits;
    string player_loc;

    if(__in_transit) {
        tell_object(player,
            "The hovertrain is currently in transit between stations.\n"
            "It will arrive shortly.\n");
        return 0;
    }
    player_loc = file_name(environment(player));
    if(__at_horton) {
        if(strsrch(player_loc, "horton_station") == -1) {
            tell_object(player,
                "The hovertrain is currently at Horton Station.\n"
                "Board from there.\n");
            return 0;
        }
    } else {
        if(strsrch(player_loc, "chitown_burbs") == -1) {
            tell_object(player,
                "The hovertrain is currently at Chi-Town.\n"
                "Board from there.\n");
            return 0;
        }
    }
    /* Fallback: if departure was lost (e.g. after warmboot), restart it. */
    if(find_call_out("depart") == -1 && !__in_transit) {
        __next_dep_time = time() + CYCLE_TIME;
        call_out("depart", CYCLE_TIME);
        tell_object(player,
            "The automated system is resetting. Departure in 5 minutes.\n");
    }

    credits = (int)player->query_money("credits");
    if(credits < TICKET_PRICE) {
        tell_object(player,
            "The automated turnstile rejects your credit chip. (" +
            TICKET_PRICE + " credits required)\n");
        return 0;
    }
    player->add_money("credits", -TICKET_PRICE);
    tell_object(player,
        TICKET_PRICE + " credits deducted. The turnstile admits you.\n");
    player->move(this_object());
    tell_object(player,
        "You step aboard the hovertrain. The doors hiss shut behind you.\n");
    say((string)player->query_cap_name() +
        " boards the hovertrain.", player);
    return 1;
}

int query_at_horton()      { return __at_horton; }
int query_in_transit()     { return __in_transit; }
int query_next_departure() { return __next_dep_time; }

void depart() {
    object *riders;
    int i;

    riders = all_inventory(this_object());
    __in_transit = 1;
    for(i = 0; i < sizeof(riders); i++) {
        if(interactive(riders[i]))
            tell_object(riders[i],
                "The hovertrain accelerates smoothly, rising on its mag-lev field.\n");
    }
    call_out("arrive_announce", TRANSIT_SECS);
}

void arrive_announce() {
    object *riders;
    string dest_name;
    int i;

    riders    = all_inventory(this_object());
    dest_name = __at_horton ? "Chi-Town" : "Horton";
    for(i = 0; i < sizeof(riders); i++) {
        if(interactive(riders[i]))
            tell_object(riders[i],
                "The hovertrain decelerates as it approaches " +
                dest_name + " station.\n");
    }
    call_out("arrive", ARRIVE_SECS);
}

void arrive() {
    object *all_ob, *riders;
    string dest;
    int i;

    dest   = __at_horton ? CHITOWN_BURBS : HORTON_STATION;
    all_ob = all_inventory(this_object());
    riders = ({});
    for(i = 0; i < sizeof(all_ob); i++) {
        if(living(all_ob[i]) && interactive(all_ob[i]))
            riders += ({ all_ob[i] });
    }
    for(i = 0; i < sizeof(riders); i++) {
        tell_object(riders[i],
            "The hovertrain doors open. You have arrived. "
            "Type 'disembark' to step off.\n");
    }
    if(__at_horton)
        __at_horton = 0;
    else
        __at_horton = 1;
    __in_transit    = 0;
    __next_dep_time = time() + CYCLE_TIME;
    call_out("depart", CYCLE_TIME);
}
