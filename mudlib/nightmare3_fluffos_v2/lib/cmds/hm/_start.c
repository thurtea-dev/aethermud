//	/bin/high_mortal/start.c
//	from the Nightmare mudlib
//	marks a new starting place for the high mortal
//	created 07 december 1992 by Descartes of Borg

#include <std.h>

inherit DAEMON;

int cmd_start(string str) {
    if(!str || str != "here") {
	notify_fail("Start where?\n");
	return 0;
    }
    return (int)this_player()->set_start_here();
}

void help() {
    write("Syntax: <start here>\n\n"+
	"Makes your current environment (where you are now) your site\n"+
	"of login.\n");
}
