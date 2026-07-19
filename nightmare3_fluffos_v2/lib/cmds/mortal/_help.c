/*    /cmds/mortal/_help.c
 *    from Nightmare IV
 *    the new help menu command interface
 *    created by Descartes of Borg 940420
 */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_help(string str) {
    string topic, category;
    int x;

    if(!str) {
        HELP_D->help("", 0, 0);
        return 1;
    }
    if((x = strsrch(str, "*")) != -1) {
        if(x < 2) topic = str;
        else {
            topic = str[0..x-2];
            category = str[x..strlen(str)-1];
	  }
      }
    else topic = str;
    HELP_D->help(topic, category);
    return 1;
  }

void help() {
    message("help", "Syntax: help [category or topic]\n\n"
      "With no arguments, lists the help categories (races, classes, "
      "skills, combat, communication, systems, commands, alignments).\n"
      "Type help <category> to list the topics in that category.\n"
      "Type help <topic> to read help on a specific topic. Topic names "
      "match loosely: help armor of ithan and help armorofithan "
      "both work.\n"
      "Type help index to see every topic in one flat list.\n\n"
      "Examples:\n\"help\"\n\"help combat\"\n\"help score\"\n"
      "\"help vagabond\"\n\n"
      "Staff may use: help staff, help creator, help admin",
      this_player());
  }
