/* /cmds/mortal/_forget.c
   Remove a player from your face list. */

#include <std.h>

inherit DAEMON;

private object find_player_target(string str) {
    object env;
    object ob;
    object *online;
    int i;

    if(!str || str == "") return 0;

    env = environment(this_player());
    if(env) {
        ob = present(str, env);
        if(ob && living(ob) && userp(ob)) return ob;
    }

    online = users();
    for(i = 0; i < sizeof(online); i++) {
        if(!online[i]) continue;
        if(lower_case((string)online[i]->query_name()) == lower_case(str))
            return online[i];
        if(lower_case((string)online[i]->query_cap_name()) == lower_case(str))
            return online[i];
    }
    return 0;
}

int cmd_forget(string str) {
    object target;
    string name;

    if(!str || str == "") {
        write("Forget whom? Type: forget <name>\n");
        return 1;
    }

    target = find_player_target(str);
    if(!target) {
        write("You don't know anyone by that name.\n");
        return 1;
    }
    if(target == this_player()) {
        write("You cannot forget yourself.\n");
        return 1;
    }

    name = (string)target->query_name();
    if(!(int)this_player()->knows_player(name)) {
        write("You do not recognize " +
              (string)target->query_cap_name() + ".\n");
        return 1;
    }

    this_player()->remove_from_face_list(name);
    write("You no longer recognize " +
          (string)target->query_cap_name() + ".\n");
    return 1;
}

void help() {
    write(
        "Syntax: forget <player>\n\n"
        "Removes a player from your recognized list.  They will again\n"
        "appear as their race description until you re-introduce or\n"
        "remember them.\n"
    );
}
