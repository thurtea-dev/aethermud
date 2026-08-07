/* /cmds/mortal/_known.c
   List players you recognize (face list). */

#include <std.h>

inherit DAEMON;

int cmd_known(string str) {
    string *fl;
    object *online;
    string shown;
    int i;
    int j;

    fl = (string *)this_player()->query_face_list();
    if(!fl || !sizeof(fl)) {
        write("You do not recognize anyone yet.\n");
        return 1;
    }

    online = users();
    write("You recognize:\n");
    for(i = 0; i < sizeof(fl); i++) {
        shown = capitalize(fl[i]);
        for(j = 0; j < sizeof(online); j++) {
            if(!online[j]) continue;
            if((string)online[j]->query_name() == fl[i]) {
                shown = (string)online[j]->query_cap_name();
                break;
            }
        }
        write("  " + shown + "\n");
    }
    return 1;
}

void help() {
    write(
        "Syntax: known\n\n"
        "Lists every player whose name you recognize.  Recognition\n"
        "comes from introduce, remember, or being introduced to you.\n"
    );
}
