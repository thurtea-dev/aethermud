/* /cmds/mortal/_tattoo.c
   Atlantean tattoo magic command.
   Syntax:
     tattoo list              -- list tattoos by body location
     tattoo activate <loc>   -- activate the tattoo at a body location
     tattoo status           -- show active timed tattoos and time remaining
     tattoo help             -- extended help */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

void help();

private int is_tattoo_capable() {
    string race;
    string occ;

    race = lower_case((string)this_player()->query_race());
    occ  = (string)this_player()->getenv("rifts_occ");
    if(!occ) occ = "";
    else occ = lower_case(occ);

    if(strsrch(race, "atlantean") != -1) return 1;
    if(occ == "sunaj assassin") return 1;
    if(occ == "tattooed man") return 1;
    if(occ == "tattoo warrior") return 1;
    if((int)this_player()->query_property("tattoo_capable")) return 1;
    return 0;
}

int cmd_tattoo(string str) {
    string cmd;
    string arg;
    mapping tattoos;
    mapping active;
    string *locs;
    string name;
    string def_str;
    string loc;
    object target;
    int i;
    int expiry;
    int remaining;
    string time_str;
    mapping def;
    int sp;
    int num;
    string resolved;
    string arg_under;
    string tname;

    if(!str || str == "") {
        str = "list";
    }

    if(!is_tattoo_capable()) {
        write("Only Atlanteans and those with the tattoo_capable property have magical tattoos.\n");
        return 1;
    }

    /* split first word as cmd, remainder as arg */
    sp = strsrch(str, " ");
    if(sp == -1) {
        cmd = lower_case(str);
        arg = "";
    } else {
        cmd = lower_case(str[0..sp-1]);
        arg = lower_case(str[sp+1..]);
    }

    if(cmd == "list") {
        tattoos = (mapping)TATTOO_D->query_tattoos(this_player());
        if(!tattoos || !sizeof(keys(tattoos))) {
            write("You have no tattoos.\n");
            return 1;
        }
        locs = keys(tattoos);
        write("Tattoos Known:\n");
        for(i = 0; i < sizeof(locs); i++) {
            loc  = locs[i];
            name = tattoos[loc];
            def  = (mapping)TATTOO_D->query_tattoo_def(name);
            if(def)
                def_str = (string)def["desc"];
            else
                def_str = "";
            write(sprintf("  %2d. %-24s (%s)\n",
                i + 1, capitalize(name), loc));
            if(def_str && strlen(def_str) > 0)
                write("       " + def_str + "\n");
        }
        write("Use 'tattoo activate <name|number|location>' to activate.\n");
        return 1;
    }

    if(cmd == "status") {
        active = (mapping)this_player()->getenv("tattoo_active");
        if(!mapp(active) || !sizeof(keys(active))) {
            write("No tattoos are currently active.\n");
            return 1;
        }
        locs = keys(active);
        write("Active tattoos:\n");
        for(i = 0; i < sizeof(locs); i++) {
            expiry    = (int)active[locs[i]];
            remaining = expiry - time();
            if(remaining <= 0)
                time_str = "expiring";
            else if(remaining < 60)
                time_str = remaining + " seconds";
            else
                time_str = (remaining / 60) + " minutes";
            tattoos = (mapping)TATTOO_D->query_tattoos(this_player());
            name = tattoos ? tattoos[locs[i]] : "unknown";
            write("  " + capitalize(locs[i]) + " (" + capitalize(name) +
                  "): " + time_str + " remaining\n");
        }
        return 1;
    }

    if(cmd == "activate") {
        if(!arg || arg == "") {
            write("Activate which tattoo? Syntax: tattoo activate <name|number|location>\n");
            write("Use 'tattoo list' to see your tattoos.\n");
            return 1;
        }
        arg = lower_case(arg);
        resolved = "";
        tattoos  = (mapping)TATTOO_D->query_tattoos(this_player());
        if(!mapp(tattoos)) tattoos = ([]);
        locs = keys(tattoos);

        /* Numeric: tattoo activate 1 */
        num = to_int(arg);
        if(num > 0 && num <= sizeof(locs)) {
            resolved = locs[num - 1];
        }

        /* Exact location match */
        if(!resolved || resolved == "") {
            if(tattoos[arg]) resolved = arg;
        }

        /* Name match (display name -> location) */
        if(!resolved || resolved == "") {
            for(i = 0; i < sizeof(locs); i++) {
                if(lower_case((string)tattoos[locs[i]]) == arg) {
                    resolved = locs[i];
                    break;
                }
            }
        }

        /* Partial name / underscore match */
        if(!resolved || resolved == "") {
            arg_under = replace_string(arg, " ", "_");
            for(i = 0; i < sizeof(locs); i++) {
                tname = replace_string(lower_case((string)tattoos[locs[i]]), " ", "_");
                if(strsrch(tname, arg_under) == 0 || strsrch(arg_under, tname) == 0) {
                    resolved = locs[i];
                    break;
                }
            }
        }

        if(!resolved || resolved == "") {
            write("You have no tattoo matching '" + arg + "'.\n");
            write("Use 'tattoo list' to see your tattoos.\n");
            return 1;
        }

        target = 0;
        (int)TATTOO_D->activate_tattoo(this_player(), resolved, target);
        return 1;
    }

    if(cmd == "help") {
        help();
        return 1;
    }

    write("Unknown tattoo command. Use: tattoo list / tattoo activate <loc> / tattoo status\n");
    return 1;
}

void help() {
    write(
        "Syntax: tattoo [list | activate <name|number|location> | status | help]\n\n"
        "Atlantean magical tattoo system.\n\n"
        "  tattoo list              -- show all your tattoos with numbers\n"
        "  tattoo activate 1        -- activate by list number\n"
        "  tattoo activate flaming sword  -- activate by display name\n"
        "  tattoo activate right forearm  -- activate by body location\n"
        "  tattoo status            -- show currently active timed tattoos\n"
        "  tattoo help              -- show this help\n\n"
        "Tattoo types:\n"
        "  Weapon  -- summons a magical weapon for a duration, costs PPE\n"
        "  Armor   -- creates a magical force field, costs PPE\n"
        "  Power   -- various effects: light, sight, stealth, healing, attack\n"
        "  Passive -- always active, no activation needed\n\n"
        "Duplicate activation of an active timed tattoo is blocked.\n\n"
        "New tattoos are inscribed by Tattoo Masters (NPCs in Atlantean cities).\n"
        "Atlantean Nomads and Sunaj Assassins begin with starting tattoos.\n"
    );
}
