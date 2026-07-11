// /cmds/mortal/_assist.c
// Enter combat on the side of an ally who is already fighting.

#include <std.h>

inherit DAEMON;

int cmd_assist(string str) {
    object ally;
    object foe;
    object *obs;
    string att_name;
    string vic_name;
    int i;
    int ret;

    if(!str || str == "") {
        notify_fail("Assist whom?  Syntax: assist <player>\n");
        return 0;
    }
    if(this_player()->query_ghost()) {
        notify_fail("You're already dead.\n");
        return 0;
    }
    if(environment(this_player())->query_property("no attack")) {
        notify_fail("Greater powers prevent your malice.\n");
        return 0;
    }
    ally = present(lower_case(str), environment(this_player()));
    if(!ally || !living(ally)) {
        notify_fail("You don't see that here.\n");
        return 0;
    }
    if(!userp(ally)) {
        notify_fail("You can only assist other players.\n");
        return 0;
    }
    if(ally == this_player()) {
        notify_fail("You cannot assist yourself.\n");
        return 0;
    }
    foe = (object)ally->query_current_attacker();
    if(!foe || !living(foe)) {
        write((string)ally->query_cap_name() + " is not in combat.\n");
        return 1;
    }
    if(foe == this_player()) {
        write("You are already fighting them.\n");
        return 1;
    }
    ret = (int)this_player()->kill_ob(foe, 0);
    if(!ret) return 1;

    write("You move to assist " +
          (string)ally->query_display_name(this_player()) +
          " against " +
          (string)foe->query_display_name(this_player()) + "!\n");
    tell_object(foe,
        (string)this_player()->query_display_name(foe) +
        " joins the fight against you!\n");
    tell_object(ally,
        (string)this_player()->query_display_name(ally) +
        " assists you!\n");
    obs = all_inventory(environment(this_player()));
    for(i = 0; i < sizeof(obs); i++) {
        if(!obs[i] || obs[i] == this_player() || obs[i] == foe ||
           obs[i] == ally)
            continue;
        att_name = (string)this_player()->query_display_name(obs[i]);
        vic_name = (string)foe->query_display_name(obs[i]);
        tell_object(obs[i], att_name + " assists against " + vic_name + "!\n");
    }
    return ret;
}

void help() {
    write(
        "Syntax: assist <player>\n\n"
        "Join combat against whoever is attacking the named ally.\n"
        "You must be in the same room. The ally must already be fighting.\n\n"
        "See also: kill, party, flee\n"
    );
}
