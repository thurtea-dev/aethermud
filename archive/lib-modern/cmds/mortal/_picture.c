/* /cmds/mortal/_picture.c
   Store and display an ASCII portrait on your character. */

#include <std.h>

inherit DAEMON;

#define MAX_LINES 15
#define MAX_WIDTH 72
#define MAX_CHARS 800

private string normalize_picture(string raw) {
    string *lines;
    string out;
    int i;
    int len;

    if(!raw || !sizeof(raw)) return "";
    lines = explode(raw, "\n");
    out = "";
    for(i = 0; i < sizeof(lines) && i < MAX_LINES; i++) {
        if(strlen(lines[i]) > MAX_WIDTH)
            lines[i] = lines[i][0..MAX_WIDTH - 1];
        if(sizeof(out))
            out += "\n";
        out += lines[i];
    }
    if(strlen(out) > MAX_CHARS)
        out = out[0..MAX_CHARS - 1];
    return out;
}

private void show_picture(object viewer, object subject) {
    string pic;
    string header;

    pic = (string)subject->getenv("player_picture");
    if(!pic || !sizeof(pic)) {
        write((string)subject->query_cap_name() + " has no picture set.\n");
        return;
    }
    header = "Picture of " + (string)subject->query_cap_name() + ":\n";
    write(header + pic + "\n");
}

void finish_picture(string line) {
    string body;
    string pic;

    if(!line || line == ".") {
        body = (string)this_player()->getenv("picture_edit_buf");
        if(!body || !sizeof(body)) {
            write("Picture unchanged.\n");
            this_player()->remove_env("picture_edit_buf");
            return;
        }
        pic = normalize_picture(body);
        this_player()->setenv("player_picture", pic);
        this_player()->remove_env("picture_edit_buf");
        write("Picture saved.\n");
        return;
    }
    if(lower_case(line) == "cancel") {
        this_player()->remove_env("picture_edit_buf");
        write("Picture edit cancelled.\n");
        return;
    }
    body = (string)this_player()->getenv("picture_edit_buf");
    if(!body || !sizeof(body))
        body = line;
    else
        body = body + "\n" + line;
    this_player()->setenv("picture_edit_buf", body);
    input_to("finish_picture");
}

int cmd_picture(string str) {
    object target;
    string arg;
    string sub;

    if(!str || !sizeof(str)) {
        show_picture(this_player(), this_player());
        return 1;
    }
    if(sscanf(str, "%s %s", arg, sub) == 2 && lower_case(arg) == "show") {
        target = present(sub, environment(this_player()));
        if(!target || !living(target)) {
            write("You don't see " + sub + " here.\n");
            return 1;
        }
        show_picture(this_player(), target);
        return 1;
    }
    if(lower_case(str) == "clear" || lower_case(str) == "remove") {
        this_player()->remove_env("player_picture");
        write("Your picture has been cleared.\n");
        return 1;
    }
    if(lower_case(str) == "set" || lower_case(str) == "edit") {
        this_player()->setenv("picture_edit_buf", "");
        write("Enter your ASCII picture, one line at a time.\n");
        write("Type . on a line by itself to finish, or cancel to abort.\n");
        write("Max " + MAX_LINES + " lines, " + MAX_WIDTH +
            " characters wide.\n");
        input_to("finish_picture");
        return 1;
    }
    write("Usage: picture [set|clear|show <player>]\n");
    return 1;
}

void help() {
    write(
        "Syntax: picture\n"
        "        picture set\n"
        "        picture clear\n"
        "        picture show <player>\n\n"
        "Store a small ASCII portrait on your character. Others can view it\n"
        "with picture show <name> when you are in the same room.\n\n"
        "Limits: " + MAX_LINES + " lines, " + MAX_WIDTH +
        " characters per line.\n\n"
        "See also: customize, describe, look\n"
    );
}
