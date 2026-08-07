/* /cmds/mortal/_radio.c
   Player radio command. Delegates to /std/rifts_radio.c item logic.
   Requires a radio item in inventory (is_radio property). */

#include <std.h>

inherit DAEMON;

private object find_radio(object pl) {
    object *inv;
    object preferred;
    int i;

    if(!pl) return 0;
    inv = all_inventory(pl);
    preferred = 0;
    i = sizeof(inv);
    while(i--) {
        if(!(int)inv[i]->query_property("is_radio")) continue;
        if((int)inv[i]->query_radio_on()) return inv[i];
        if(!preferred) preferred = inv[i];
    }
    return preferred;
}

int cmd_radio(string str) {
    object radio;

    radio = find_radio(this_player());
    if(!radio) {
        write("You don't have a radio.\n");
        return 1;
    }
    if(!str) str = "";
    return (int)radio->cmd_radio(str);
}

void help() {
    write(
        "Syntax: radio [on|off|tune <freq>|scan|<message>]\n\n"
        "Controls your portable radio and broadcasts on your current frequency\n"
        "channel (1-100). Requires a radio in inventory. Only players tuned to\n"
        "the same frequency with their radio on will hear you.\n\n"
        "  radio on              turn your radio on\n"
        "  radio off             turn your radio off\n"
        "  radio tune <number>   change frequency\n"
        "  radio <message>       transmit on current channel\n"
        "  tune <number>         alternate syntax for changing frequency\n"
        "  transmit <message>    alternate syntax for broadcasting\n\n"
        "To stop receiving all radio: watchradio off\n"
        "To resume receiving:       watchradio on\n\n"
        "See also: watchradio, tune, transmit, ooc\n"
    );
}
