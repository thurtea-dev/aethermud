/* /domains/Horton/areas/horton_station.c
   Horton Hovertrain Station -- arrival point from Chi-Town.
   Board the hovertrain with 'board train' (50 credits). */

#include <std.h>
#include <rooms.h>

#define HOVERTRAIN "/domains/Horton/areas/hovertrain"

inherit ROOM;

string query_long();
int cmd_board(string str);

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_short("Horton Hovertrain Station");
    set_long(
        "A small hovertrain platform on the edge of town. The concrete\n"
        "platform is clean and well-maintained, unusual for post-Rifts\n"
        "Earth. A row of wooden benches lines the eastern wall. A departure\n"
        "board mounted above the platform shows the next scheduled service.\n\n"
        "Horton lies to the south. The hovertrain to Chi-Town boards here.\n"
        "Type 'board train' to board (50 credits).");
    set_exits( ([
        "south" : "/domains/Horton/areas/horton_main_street"
    ]) );
    set_listen("default",
        "Wind across the platform. The creak of a bench. Distant forest sounds.");
    set_smell("default",
        "Ozone from the mag-lev rails and pine from the surrounding forest.");
    set_items( ([
        "platform"       : "Clean concrete. Someone maintains this place.",
        "benches"        : "Simple wooden benches along the east wall.",
        "departure board": "An electronic sign showing hovertrain schedule.",
        "rails"          : "Mag-lev rails running north into the distance.",
        "sky"            : "Amber and hazy. The forest presses in on all sides."
    ]) );
}

string query_long() {
    object train;
    int dep_time, secs_left, minutes;
    string board_info;

    train = find_object(HOVERTRAIN);
    board_info = "";
    if(train) {
        if((int)train->query_in_transit()) {
            board_info = "\nDeparture board: Hovertrain in transit -- arriving shortly.";
        } else if((int)train->query_at_horton()) {
            dep_time  = (int)train->query_next_departure();
            secs_left = dep_time - time();
            minutes   = (secs_left > 60) ? (secs_left / 60) : 0;
            if(minutes > 0)
                board_info = "\nDeparture board: Next departure in approximately " +
                             minutes + " minute(s). Type 'board train' (50 credits).";
            else
                board_info = "\nDeparture board: Hovertrain departs shortly. "
                             "Type 'board train' (50 credits).";
        } else {
            board_info = "\nDeparture board: Hovertrain at Chi-Town. "
                         "Next arrival in approximately 5 minutes.";
        }
    } else {
        board_info = "\nDeparture board: Schedule unavailable.";
    }
    return ::query_long() + board_info + "\n";
}

void init() {
    ::init();
    add_action("cmd_board", "board");
}

int cmd_board(string str) {
    object train;

    if(!str || (lower_case(str) != "train" && lower_case(str) != "hovertrain")) {
        notify_fail("Board what? Try 'board train'.\n");
        return 0;
    }
    train = find_object(HOVERTRAIN);
    if(!train)
        train = load_object(HOVERTRAIN);
    if(!train) {
        write("The hovertrain is not currently available.\n");
        return 1;
    }
    (int)train->board_player(this_player());
    return 1;
}

void reset() {
    ::reset();
    if(!find_object(HOVERTRAIN))
        catch(load_object(HOVERTRAIN));
}
