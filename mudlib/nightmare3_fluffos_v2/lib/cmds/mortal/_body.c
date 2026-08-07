/*      /cmds/mortal/_body.c
        Body status: pooled health plus missing limbs.
        Rewritten 2026-07-19 for the whole_body damage model: limbs no
        longer take individual damage, so this reports the character's
        damage pools and any narratively severed limbs.  */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_body(string str) {
    object tp;
    string *missing;
    string race;
    int i;

    tp = this_player();
    if(tp->query_ghost()) {
	notify_fail("You have no body to check on!\n");
	return 0;
    }
    race = (string)tp->query_race();
    if((int)RIFTS_D->is_mdc_race(race) ||
       (int)tp->query_property("mdc_creature")) {
        write("MDC: "+(int)tp->query_stats("MDC")+"/"+
          (int)tp->query_stats("max_MDC"));
    }
    else if((int)tp->query_stats("max_rifts_hp") > 0) {
        write("Hit Points: "+(int)tp->query_stats("rifts_hp")+"/"+
          (int)tp->query_stats("max_rifts_hp")+"    SDC: "+
          (int)tp->query_stats("SDC")+"/"+(int)tp->query_stats("max_SDC"));
    }
    else {
        write("Hit Points: "+(int)tp->query_hp()+"/"+
          (int)tp->query_max_hp());
    }
    missing = (string *)tp->query_severed_limbs();
    if(missing && sizeof(missing)) {
        write("Missing limbs:");
        for(i=0; i<sizeof(missing); i++)
            write("  "+capitalize(missing[i]));
    }
    else write("All limbs intact.");
    return 1;
}

void help() {
   write("Syntax: body\n\n"+
	 "Shows the state of your body: your damage pools (MDC for MDC\n"+
	 "beings, otherwise hit points and SDC) and any limbs you are\n"+
	 "missing. Limbs no longer take damage of their own; lost limbs\n"+
	 "come from special events and can be restored by cybernetics\n"+
	 "or regenerative healing.\n"+
	 "See also: score, eq\n"
   );
}
