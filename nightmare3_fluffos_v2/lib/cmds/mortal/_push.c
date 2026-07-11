// /cmds/mortal/_push.c
// Push command.  Objects with is_pushable=1 can produce results.

#include <std.h>

inherit DAEMON;

int cmd_push(string str) {
    object ob;
    object room;
    string result;
    string push_msg;
    string item_file;
    string exit_dir;
    string exit_dest;
    object cloned;

    if(!str || str == "") {
        write("Push what?\n");
        return 1;
    }
    room = environment(this_player());
    if(!room) return 0;

    ob = present(str, room);
    if(!ob) ob = present(str, this_player());
    if(!ob) {
        write("You don't see that here.\n");
        return 1;
    }

    if(!(int)ob->query_property("is_pushable")) {
        write("You push it but nothing happens.\n");
        return 1;
    }

    push_msg = (string)ob->query_property("push_message");
    if(!push_msg || push_msg == "")
        push_msg = "You push it with all your strength.";

    result = (string)ob->query_property("push_result");

    if(result == "reveal_item") {
        if((int)ob->query_property("push_done")) {
            write("You already moved that.  Nothing more to find.\n");
            return 1;
        }
        write(push_msg + "\n");
        say(this_player()->query_cap_name() + " shoves a large rock aside.");
        item_file = (string)ob->query_property("push_item_file");
        if(item_file && strlen(item_file) > 0 && file_size(item_file + ".c") > 0) {
            cloned = clone_object(item_file);
            if(cloned) {
                cloned->move(room);
                write("Something falls from the hidden compartment.\n");
            }
        }
        ob->set_property("push_done", 1);
        return 1;
    }

    if(result == "reveal_exit") {
        if((int)ob->query_property("push_done")) {
            write("You already pushed that open.\n");
            return 1;
        }
        write(push_msg + "\n");
        say(this_player()->query_cap_name() + " pushes something heavy aside.");
        exit_dir  = (string)ob->query_property("push_exit_dir");
        exit_dest = (string)ob->query_property("push_exit_dest");
        if(exit_dir && exit_dest && strlen(exit_dir) > 0 && strlen(exit_dest) > 0)
            room->add_exit(exit_dir, exit_dest);
        ob->set_property("push_done", 1);
        return 1;
    }

    write(push_msg + "\n");
    return 1;
}

void help() {
    write(
        "Syntax: push <object>\n\n"
        "Push a moveable object.  Some objects reveal hidden items or passages.\n"
    );
}
