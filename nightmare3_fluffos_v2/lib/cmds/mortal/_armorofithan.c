/* /cmds/mortal/_armorofithan.c
   Standalone command for casting Armor of Ithan.
   Delegates to cmd_cast so all PPE, spell-known, and combat
   checks run through the standard path.
   Supports:
     armorofithan            -- cast on self
     armorofithan <target>   -- cast on explicit target */

#include <std.h>
#include <daemons.h>

inherit "/cmds/mortal/_cast";

int cmd_armorofithan(string str) {
    if(str && strlen(str))
        return cmd_cast("armorofithan at " + str);
    return cmd_cast("armorofithan");
}

void help() {
    write(
        "Syntax: armorofithan [<target>]\n\n"
        "Casts Armor of Ithan on yourself or a named target.\n"
        "PPE cost: 10. Creates a 30 MDC magical field for 5 minutes.\n\n"
        "Examples:\n"
        "  armorofithan\n"
        "  armorofithan ally\n"
        "  cast armor of ithan\n\n"
        "See also: help armor_of_ithan, help spells, help cast\n"
    );
}
