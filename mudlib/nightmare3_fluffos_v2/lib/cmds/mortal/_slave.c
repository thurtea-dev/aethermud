// /cmds/mortal/_slave.c
// Splynn slave ownership mechanic.
// "slave list" -- show slaves you own in this room/environment
// "slave free <name>" -- release a slave from your ownership
// "slave status" -- alias for list
// Only usable within the Splynn domain.

#include <std.h>

inherit DAEMON;

private int in_splynn() {
    object env;
    env = environment(this_player());
    if(!env) return 0;
    return (strsrch(base_name(env), "splynn") != -1);
}

private object *get_my_slaves() {
    object *here;
    object *mine;
    string pname;
    int i;

    pname = (string)this_player()->query_name();
    here  = all_inventory(environment(this_player()));
    mine  = ({});
    for(i = 0; i < sizeof(here); i++) {
        if((string)here[i]->query_property("owned_by") == pname)
            mine += ({ here[i] });
    }
    return mine;
}

int cmd_slave(string str) {
    object *slaves;
    object target;
    string pname;
    string cmd;
    string arg;
    int i;

    if(!in_splynn()) {
        write("The slave trade only operates within the Splynn Atlantis district.\n");
        return 1;
    }

    if(!str || str == "" || str == "list" || str == "status") {
        slaves = get_my_slaves();
        pname  = (string)this_player()->query_cap_name();
        if(!sizeof(slaves)) {
            write("You have no slaves here.\n");
            return 1;
        }
        write(pname + "'s slaves in this area:\n");
        for(i = 0; i < sizeof(slaves); i++) {
            write("  " + capitalize((string)slaves[i]->query_short()) + "\n");
        }
        return 1;
    }

    if(sscanf(str, "free %s", arg) == 1) {
        target = present(arg, environment(this_player()));
        pname  = (string)this_player()->query_name();
        if(!target) {
            write("You don't see that slave here.\n");
            return 1;
        }
        if((string)target->query_property("owned_by") != pname) {
            write("That is not your slave.\n");
            return 1;
        }
        target->set_property("owned_by", 0);
        target->set_property("following", 0);
        write("You release " + (string)target->query_short() + ".\n");
        tell_room(environment(this_player()),
            (string)this_player()->query_cap_name() +
            " releases " + (string)target->query_short() + " from bondage.\n",
            ({ this_player() }));
        return 1;
    }

    if(sscanf(str, "claim %s", arg) == 1) {
        target = present(arg, environment(this_player()));
        if(!target) {
            write("You don't see that here.\n");
            return 1;
        }
        if(!(int)target->query_property("is_slave")) {
            write("That is not a slave.\n");
            return 1;
        }
        if((string)target->query_property("owned_by")) {
            write("That slave is already owned.\n");
            return 1;
        }
        pname = (string)this_player()->query_name();
        target->set_property("owned_by", pname);
        write("You claim " + (string)target->query_short() +
              ". They will now follow you.\n");
        tell_room(environment(this_player()),
            (string)this_player()->query_cap_name() +
            " takes ownership of " + (string)target->query_short() + ".\n",
            ({ this_player() }));
        return 1;
    }

    write("Usage:\n"
          "  slave list          -- list your slaves here\n"
          "  slave claim <name>  -- claim ownership of a slave\n"
          "  slave free <name>   -- release a slave\n");
    return 1;
}

void help() {
    write(
        "Syntax: slave [list|claim <name>|free <name>]\n\n"
        "Manage slave ownership within the Splynn district.\n\n"
        "  slave list          Show slaves you own in this area\n"
        "  slave claim <name>  Claim a slave (must be for sale)\n"
        "  slave free <name>   Release a slave from your ownership\n\n"
        "See also: score, who\n"
    );
}
