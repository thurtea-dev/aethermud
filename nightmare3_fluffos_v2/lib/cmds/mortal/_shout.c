// /cmds/mortal/_shout.c
// Domain-wide shout command.  No language filtering.
// 5-second cooldown between shouts.

#include <std.h>

inherit DAEMON;

static string get_domain(object player) {
    string path;
    string *parts;

    if(!player) return "";
    if(!environment(player)) return "";
    path = file_name(environment(player));
    if(!path) return "";
    parts = explode(path, "/");
    if(sizeof(parts) >= 3 && parts[1] == "domains")
        return parts[2];
    return "";
}

int cmd_shout(string str) {
    string cap_name;
    string domain;
    string user_domain;
    object *all_users;
    int i;
    int last_shout;
    string last_str;

    if(!str || str == "") {
        notify_fail("Shout what?\n");
        return 0;
    }
    if(strlen(str) > 200) {
        notify_fail("Your shout is too long.\n");
        return 0;
    }
    if(this_player()->query_ghost()) {
        message("my_action", "You moan mournfully.", this_player());
        return 1;
    }

    last_str = (string)this_player()->getenv("last_shout_time");
    last_shout = last_str ? to_int(last_str) : 0;
    if((time() - last_shout) < 5) {
        notify_fail("You are still catching your breath from shouting.\n");
        return 0;
    }

    this_player()->setenv("last_shout_time", sprintf("%d", time()));

    cap_name = (string)this_player()->query_cap_name();
    domain   = get_domain(this_player());

    message("shout", "You shout, \"" + str + "\"", this_player());

    all_users = users();
    for(i = 0; i < sizeof(all_users); i++) {
        if(all_users[i] == this_player()) continue;
        if(!interactive(all_users[i])) continue;
        if((string)all_users[i]->query_blocked("shout")) continue;
        user_domain = get_domain(all_users[i]);
        if(domain == "" || user_domain != domain) continue;
        message("shout",
            (string)this_player()->query_display_name(all_users[i]) +
            " shouts, \"" + str + "\"", all_users[i]);
    }

    log_file("shouts", cap_name + " (shout/" + domain + "): " + str + "\n");
    return 1;
}

void help() {
    write(
        "Syntax: shout <message>\n\n"
        "Shouts a message to all players in the same domain.\n"
        "No language needed -- everyone understands.\n"
        "There is a 5-second cooldown between shouts.\n"
    );
}
