// /std/rifts_radio.c
// Base portable radio item for Rifts MUD.
// Subclasses set radio_type, default frequency, and item descriptions.

#include <std.h>
#include <daemons.h>

inherit OBJECT;

private int    __frequency;
private int    __radio_on;
private string __radio_type;

void create() {
    ::create();
    set_property("is_radio", 1);
    set_name("radio");
    set_id(({ "radio", "com", "comms", "communicator" }));
    set_short("a portable radio");
    set_long("A portable communications device for short-range radio contact.");
    set_mass(200);
    __frequency  = 14;
    __radio_on   = 0;
    __radio_type = "personal";
}

int query_frequency() { return __frequency; }
int query_radio_on()  { return __radio_on; }
string query_radio_type() { return __radio_type; }

void set_frequency(int f)     { __frequency  = f; }
void set_radio_on(int v)      { __radio_on   = v; }
void set_radio_type(string t) { __radio_type = t; }

void transmit_message(object sender, string msg) {
    object *online;
    object *inv;
    object room;
    string room_name;
    int freq, i, j;

    if(!sender || !__radio_on) return;
    freq     = __frequency;
    online   = users();
    i        = sizeof(online);
    while(i--) {
        inv = all_inventory(online[i]);
        j   = sizeof(inv);
        while(j--) {
            if(!(int)inv[j]->query_property("is_radio")) continue;
            if((int)inv[j]->query_frequency() != freq)   continue;
            if(!(int)inv[j]->query_radio_on())            continue;
            if((string)online[i]->getenv("radio_off") == "1") continue;
            message("radio",
                "[Radio " + freq + "] " +
                (string)sender->query_display_name(online[i]) + ": " + msg,
                online[i]);
            break;
        }
    }
    if(freq == 77 && objectp(sender)) {
        room = environment(sender);
        if(objectp(room)) {
            room_name = file_name(room);
            if((int)QUEST_D->is_quest_active(sender, "dead_signal")) {
                if(strsrch(room_name, "nexus_approach") != -1) {
                    if(!(int)QUEST_D->is_objective_complete(sender, "dead_signal", "contacted_nexus")) {
                        QUEST_D->update_objective(sender, "dead_signal", "freq_77_tuned", 1);
                        QUEST_D->update_objective(sender, "dead_signal", "contacted_nexus", 1);
                        message("radio",
                            "[Radio 77] Unknown: ...still here...the nexus remembers...",
                            sender);
                        tell_object(sender,
                            "The radio crackles.  A ghostly voice responds from the static.\n");
                    }
                } else {
                    if(!(int)QUEST_D->is_objective_complete(sender, "dead_signal", "freq_77_tuned"))
                        QUEST_D->update_objective(sender, "dead_signal", "freq_77_tuned", 1);
                }
            }
        }
    }
}

int cmd_tune(string str) {
    int new_freq;

    if(!str || !strlen(str)) {
        write("Radio is on frequency " + __frequency + ".  "
              "Type 'tune <number>' to change.\n");
        return 1;
    }
    new_freq = to_int(str);
    if(new_freq < 1 || new_freq > 100) {
        write("Frequency must be between 1 and 100.\n");
        return 1;
    }
    __frequency = new_freq;
    write("Radio tuned to frequency " + __frequency + ".\n");
    return 1;
}

int cmd_transmit(string str) {
    if(!str || !strlen(str)) {
        write("Transmit what?\n");
        return 1;
    }
    if(!__radio_on) {
        write("Your radio is off.  Type 'radio on' to turn it on.\n");
        return 1;
    }
    write("You transmit on channel " + __frequency + ": " + str + "\n");
    transmit_message(this_player(), str);
    return 1;
}

int cmd_radio(string str) {
    if(!str || !strlen(str)) {
        write("Radio is " + (__radio_on ? "ON" : "OFF") +
              ", frequency " + __frequency + ".\n");
        return 1;
    }
    if(str == "on") {
        __radio_on = 1;
        write("Radio on.  Frequency: " + __frequency + ".\n");
        return 1;
    }
    if(str == "off") {
        __radio_on = 0;
        write("Radio off.\n");
        return 1;
    }
    if(str == "scan") {
        if(__radio_type != "military") {
            write("Your radio does not have a scan function.\n");
            return 1;
        }
        write("Scanning all frequencies for active transmissions...\n");
        return 1;
    }
    // radio tune <freq> as a subcommand of radio
    if(strsrch(str, "tune ") == 0) {
        return cmd_tune(str[5..]);
    }
    if(str == "tune") {
        write("Tune to what frequency?  Syntax: radio tune <1-100>\n");
        return 1;
    }
    return cmd_transmit(str);
}

void init() {
    ::init();
    add_action("cmd_radio",    "radio");
    add_action("cmd_tune",     "tune");
    add_action("cmd_transmit", "transmit");
}
