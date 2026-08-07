/* /cmds/mortal/_stance.c
   Set combat stance. Affects strike/dodge bonuses in rifts_combat.c.
   Syntax: stance
           stance offensive
           stance defensive
           stance standard */

#include <std.h>

inherit DAEMON;

int cmd_stance(string str) {
    string pos, s;

    pos = (string)this_player()->query_property("combat_stance");
    if(!pos || !sizeof(pos)) pos = "standard";

    if(!str || !strlen(str)) {
        write("Current combat stance: " + capitalize(pos) + ".\n");
        if(pos == "offensive")
            write("  +2 to strike rolls, -2 to dodge/parry rolls.\n");
        else if(pos == "defensive")
            write("  +2 to dodge/parry rolls, -2 to strike rolls.\n");
        else
            write("  Balanced. No modifier.\n");
        return 1;
    }

    s = lower_case(str);
    if(s == "offensive" || s == "offense") {
        this_player()->set_property("combat_stance", "offensive");
        write("You shift to an aggressive combat stance. (+2 strike, -2 dodge)\n");
        this_player()->tell_room_living(environment(this_player()),
            this_player(), 0, " shifts to an aggressive combat stance.\n");
        return 1;
    }
    if(s == "defensive" || s == "defense") {
        this_player()->set_property("combat_stance", "defensive");
        write("You adopt a defensive posture. (+2 dodge/parry, -2 strike)\n");
        this_player()->tell_room_living(environment(this_player()),
            this_player(), 0, " adopts a defensive posture.\n");
        return 1;
    }
    if(s == "standard" || s == "normal" || s == "balanced") {
        this_player()->set_property("combat_stance", "standard");
        write("You settle into a balanced combat stance.\n");
        this_player()->tell_room_living(environment(this_player()),
            this_player(), 0, " settles into a balanced stance.\n");
        return 1;
    }
    write("Valid stances: offensive, defensive, standard\n");
    return 1;
}

void help() {
    message("help",
        "Syntax: stance                  -- show current stance\n"
        "        stance offensive        -- +2 strike, -2 dodge/parry\n"
        "        stance defensive        -- +2 dodge/parry, -2 strike\n"
        "        stance standard         -- balanced (default)\n\n"
        "Changes your combat stance. Persists until you change it.\n"
        "See also: combat, autododge, autoparry",
        this_player());
}

void init() {
    add_action("cmd_stance", "stance");
}
