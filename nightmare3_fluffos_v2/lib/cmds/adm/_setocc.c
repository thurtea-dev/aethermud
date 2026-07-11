/* /cmds/adm/_setocc.c
   Set a player's OCC (Occupational Character Class). */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_setocc(string str) {
    string name, occ;
    object target;
    mapping occ_data;
    string *occ_flags;
    string race_flags;

    if(!creatorp(this_player())) {
        write("Access denied.\n");
        return 1;
    }
    if(!str || sscanf(str, "%s %s", name, occ) != 2) {
        write("Syntax: setocc <player> <occ_name>\n"
              "Example: setocc bob headhunter\n");
        return 1;
    }
    name   = lower_case(name);
    occ    = lower_case(occ);
    target = find_player(name);
    if(!target) {
        write("Player '" + capitalize(name) + "' is not online.\n");
        return 1;
    }

    occ_data = (mapping)OCC_D->query_occ(occ);
    if(!occ_data) {
        write("'" + occ + "' is not a valid OCC.\n");
        return 1;
    }

    target->setenv("rifts_occ", occ);
    target->set_class(occ);

    occ_flags = (string *)occ_data["flags"];
    if(!occ_flags) occ_flags = ({});
    target->setenv("rifts_occ_flags", implode(occ_flags, ","));

    race_flags = (string)target->getenv("rifts_flags");
    if(!race_flags) race_flags = "";
    if(sizeof(occ_flags)) {
        if(sizeof(race_flags))
            target->setenv("rifts_flags",
                race_flags + "," + implode(occ_flags, ","));
        else
            target->setenv("rifts_flags", implode(occ_flags, ","));
    }

    RIFTS_START_D->grant_starting_package(target);

    write("Set " + capitalize(name) + "'s OCC to: " + capitalize(occ) + "\n"
          "Starting package and skills applied.\n");
    tell_object(target,
        "%^YELLOW%^Your OCC has been set to " + capitalize(occ) +
        " by " + (string)this_player()->query_cap_name() +
        ".%^RESET%^\n");
    return 1;
}

void help() {
    write("Syntax: setocc <player> <occ_name>\n\n"
          "Sets a player's OCC and re-applies their starting package and skills.\n"
          "Player must be online. OCC must be a valid OCC in occ.c.\n"
          "Example: setocc bob headhunter\n"
          "Restricted to wizards.\n");
}
