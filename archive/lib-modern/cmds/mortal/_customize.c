/* /cmds/mortal/_customize.c
   Menu-driven character appearance editor. */

#include <std.h>

inherit DAEMON;

private void show_menu(object player) {
    string desc;
    string pic_flag;

    desc = (string)player->query_description();
    if(!desc || !sizeof(desc)) desc = "(none)";
    if((string)player->getenv("player_picture") != "")
        pic_flag = "set";
    else
        pic_flag = "none";
    write(
        "\n=== Customize ===\n"
        " 1. Description  [" + desc + "]\n"
        " 2. Picture      [" + pic_flag + "]\n"
        " 3. Position     (use the face and position commands)\n"
        " 4. Show summary\n"
        " 5. Done\n"
        "Choice: "
    );
}

void customize_desc_done(string line) {
    if(!line || lower_case(line) == "cancel") {
        write("Description edit cancelled.\n");
        show_menu(this_player());
        input_to("customize_menu");
        return;
    }
    if(!sizeof(line)) {
        write("Description cannot be blank. Try again or type cancel.\n");
        input_to("customize_desc_done");
        return;
    }
    this_player()->set_description(line);
    write("Description set.\n");
    show_menu(this_player());
    input_to("customize_menu");
}

void customize_menu(string choice) {
    object player;
    string desc;
    string pos;

    player = this_player();
    if(!choice || !sizeof(choice) || choice == "5" || lower_case(choice) == "done") {
        write("Done customizing.\n");
        return;
    }
    switch(choice) {
    case "1":
        write("Enter a short description (shown after your name on look).\n");
        write("Example: is a scarred veteran in dusty armor.\n");
        write("Type cancel to return to the menu.\n");
        input_to("customize_desc_done");
        break;
    case "2":
        write("Launching picture editor. Type . on its own line when done.\n");
        (int)"/cmds/mortal/_picture"->cmd_picture("set");
        break;
    case "3":
        write("Use 'position <text>' or 'face <direction>' for room posture.\n");
        show_menu(player);
        input_to("customize_menu");
        break;
    case "4":
        desc = (string)player->query_description();
        pos = (string)player->query_property("position_str");
        write("\n--- Character summary ---\n");
        write("Name: " + (string)player->query_cap_name() + "\n");
        if(desc && sizeof(desc))
            write("Description: " + (string)player->query_cap_name() +
                " " + desc + "\n");
        else
            write("Description: (none)\n");
        if(pos && sizeof(pos))
            write("Position: " + pos + "\n");
        else
            write("Position: standing around\n");
        if((string)player->getenv("player_picture") != "")
            write("Picture: set (type picture to view)\n");
        else
            write("Picture: none\n");
        show_menu(player);
        input_to("customize_menu");
        break;
    default:
        write("Invalid choice.\n");
        show_menu(player);
        input_to("customize_menu");
        break;
    }
}

int cmd_customize(string str) {
    if(str && sizeof(str)) {
        write("Type customize with no arguments to open the menu.\n");
        return 1;
    }
    show_menu(this_player());
    input_to("customize_menu");
    return 1;
}

void help() {
    write(
        "Syntax: customize\n\n"
        "Opens a menu to edit your character appearance: description,\n"
        "ASCII picture, and pointers for position/facing commands.\n\n"
        "See also: describe, picture, position, face\n"
    );
}
