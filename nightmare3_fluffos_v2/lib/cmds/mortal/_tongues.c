/* /cmds/mortal/_tongues.c
   "tongues" casts the tongues spell when the player knows it (classic
   no-verb casting). Otherwise reports language comprehension.
   Replaces the old Nightmare command that listed the generic fantasy
   race and language table (Eltherian, Tlinghan, etc). */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_tongues(string str) {
    object me;

    me = this_player();
    if((int)RIFTS_D->player_knows_spell(me, "tongues")) {
        if(str && sizeof(str))
            return (int)"/cmds/mortal/_cast"->cmd_cast("tongues at " + str);
        return (int)"/cmds/mortal/_cast"->cmd_cast("tongues");
    }
    if((int)LANGUAGE_D->understands_all(me)) {
        write("You understand all languages.\n");
        return 1;
    }
    write("You do not know the tongues spell.\n"
          "Type 'languages' to see the languages you speak.\n");
    return 1;
}

int help() {
    write(
        "Syntax: tongues [at <target>]\n\n"
        "Casts the tongues spell if you know it, opening the target's\n"
        "mind to understand and speak all languages for a time.\n"
        "If you already understand all languages (godlings, or anyone\n"
        "under an active tongues effect), this reports that instead.\n\n"
        "See also: languages, language, cast, say\n"
    );
    return 1;
}
