// /cmds/mortal/_wanted.c
// Check your own infamy score and wanted status.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_wanted(string str) {
    string name;
    int infamy;
    string wstatus;
    mixed *log_arr;
    int i;
    int log_size;
    mixed entry;

    name   = (string)this_player()->query_name();
    infamy = (int)WANTED_D->query_infamy(name);
    wstatus = (string)WANTED_D->query_wanted_status(name);

    write("=== WANTED STATUS ===\n");
    write("Name:   " + (string)this_player()->query_cap_name() + "\n");
    write("Infamy: " + infamy + "\n");
    write("Status: " + capitalize(wstatus) + "\n");

    if((int)WANTED_D->is_cs_enemy(this_player()))
        write("CS Threat: The Coalition States consider you an enemy.\n");
    else
        write("CS Threat: The Coalition States do not actively seek you.\n");

    write("\n");

    log_arr  = (mixed *)WANTED_D->query_infamy_log(name);
    log_size = sizeof(log_arr);
    if(log_size > 0) {
        write("Recent infamy history (last " + log_size + " events):\n");
        i = (log_size > 5) ? log_size - 5 : 0;
        for(; i < log_size; i++) {
            entry = log_arr[i];
            if(!pointerp(entry) || sizeof(entry) < 3) continue;
            write("  " + ctime((int)entry[0])[4..15] + "  " +
                ((int)entry[1] >= 0 ? "+" : "") + (int)entry[1] +
                "  " + (string)entry[2] + "\n");
        }
    } else {
        write("No infamy history on record.\n");
    }
    return 1;
}

void help() {
    write(
        "Syntax: wanted\n\n"
        "Displays your current infamy score and wanted status.\n\n"
        "Infamy thresholds:\n"
        "  < 0   : Hero (negative infamy, earned by killing evil)\n"
        "  0-99  : Clean\n"
        "  100+  : Wanted by Coalition authorities\n"
        "  500+  : Kill on Sight. CS forces attack on detection\n\n"
        "Infamy is gained by killing good-aligned players (+50),\n"
        "and reduced by killing evil beings (-10).\n\n"
        "See also: alignment, death, score\n"
    );
}
