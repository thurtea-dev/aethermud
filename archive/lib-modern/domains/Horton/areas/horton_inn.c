/* /domains/Horton/areas/horton_inn.c
   The Horton Inn -- rest point with respawn setting and room rental. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("rest_allowed", 1);
    set_property("no attack", 1);
    set_short("The Horton Inn");
    set_long(
        "Clean and well-kept. Wooden floors, actual curtains, a hearth burning\n"
        "low. The innkeeper has clearly seen everything and judges nothing.\n"
        "Travelers of every race and alignment have passed through here and\n"
        "none have been turned away.\n\n"
        "Cheap rooms through the east door. Adequate food available. A hand-lettered\n"
        "sign above the bar reads: No killing in here. Everything else, ask first.");
    set_exits( ([
        "west" : "/domains/Horton/areas/horton_main_street"
    ]) );
    set_listen("default",
        "Quiet conversation. A fire crackling. Mugs on wood.");
    set_smell("default",
        "Food, woodsmoke, and something being boiled in the back.");
    set_items( ([
        "hearth"    : "A stone hearth with a low fire. Warm.",
        "sign"      : "No killing in here. Everything else, ask first.",
        "bar"       : "A simple wooden bar. Clean.",
        "curtains"  : "Actual curtains. Not salvaged plastic sheeting.",
        "rooms"     : "Cheap and clean. East door.",
        "innkeeper" : "A broad-shouldered figure behind the bar."
    ]) );
}

void init() {
    ::init();
    add_action("cmd_rent", "rent");
    add_action("cmd_setrespawn", "setrespawn");
}

int cmd_rent(string str) {
    int credits;
    string room_file;

    if(!this_player()) return 0;
    if(str && lower_case(str) != "room") {
        write("Rent what? Type 'rent room' to rent a room for the night (50 credits).\n");
        return 1;
    }
    credits = (int)this_player()->query_money("credits");
    if(credits < 50) {
        write("The innkeeper says: 'A room for the night is 50 credits. Come back when you have it.'\n");
        return 1;
    }
    this_player()->add_money("credits", -50);
    room_file = file_name(this_object());
    this_player()->set_property("respawn_room", room_file);
    this_player()->save_player((string)this_player()->query_name());
    write("The innkeeper takes 50 credits and hands you a key.\n"
          "'Room's yours for the night. You know where to find us if you come back dead.'\n"
          "Respawn point set to the Horton Inn.\n");
    say((string)this_player()->query_cap_name() +
        " rents a room for the night.", this_player());
    return 1;
}

int cmd_setrespawn(string str) {
    string room_file;

    if(!this_player()) return 0;
    room_file = file_name(this_object());
    this_player()->set_property("respawn_room", room_file);
    this_player()->save_player((string)this_player()->query_name());
    write("Respawn point set to the Horton Inn.\n");
    return 1;
}

void reset() {
    object npc;
    if(!present("innkeeper", this_object())) {
        npc = clone_object("/domains/Horton/npcs/horton_innkeeper");
        npc->move(this_object());
    }
    ::reset();
}
