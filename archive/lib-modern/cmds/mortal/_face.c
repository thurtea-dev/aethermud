/* /cmds/mortal/_face.c
   Turn to face a direction or a person. Sets the position_str RP string. */

#include <std.h>

inherit DAEMON;

private string *valid_dirs;

void create() {
    ::create();
    valid_dirs = ({
        "north", "south", "east", "west", "up", "down",
        "northeast", "northwest", "southeast", "southwest",
        "in", "out"
    });
}

private int is_direction(string str) {
    return member_array(lower_case(str), valid_dirs) != -1;
}

int cmd_face(string str) {
    object target;
    string pos;
    string trimmed;

    if(!str || !sizeof(str)) {
        pos = (string)this_player()->query_property("position_str");
        if(!pos || !sizeof(pos))
            write("You are facing no particular direction.\n");
        else
            write("You " + pos + "\n");
        return 1;
    }
    trimmed = lower_case(str);
    if(trimmed == "clear" || trimmed == "none") {
        this_player()->set_property("position_str", 0);
        write("You stop holding a particular facing.\n");
        return 1;
    }
    if(is_direction(trimmed)) {
        this_player()->set_property("position_str", "faces " + trimmed + ".");
        write("You face " + trimmed + ".\n");
        return 1;
    }
    target = present(str, environment(this_player()));
    if(!target || !living(target)) {
        write("Face what? Try a direction (north, south, ...) or someone here.\n");
        return 1;
    }
    this_player()->set_property("position_str",
        "faces " + (string)target->query_cap_name() + ".");
    write("You face " + (string)target->query_cap_name() + ".\n");
    return 1;
}

void help() {
    write(
        "Syntax: face <direction>\n"
        "        face <person>\n"
        "        face clear\n"
        "        face\n\n"
        "Sets how you appear in the room list by changing your position\n"
        "string. Directions include north, south, east, west, up, down,\n"
        "and the diagonal and in/out variants.\n\n"
        "Type face alone to see your current facing. Type face clear to\n"
        "reset. Moving to another room also clears your position.\n\n"
        "See also: position, look\n"
    );
}
