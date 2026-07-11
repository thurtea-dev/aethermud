// /cmds/mortal/_languages.c
// Show all Rifts languages this player knows and their proficiency.
// Replaces the old Nightmare language system (which was disabled anyway).

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_languages(string str) {
    mapping known;
    string *langs, active, bar;
    int i, pct;

    known  = (mapping)LANGUAGE_D->query_known(this_player());
    langs  = keys(known);
    active = (string)LANGUAGE_D->query_active(this_player());

    if(!sizeof(langs)) {
        write(
            "You have no language skills recorded.\n"
            "Your native language will be set on your next login\n"
            "or ask an admin to initialize it.\n"
        );
        return 1;
    }

    write("Languages you know:\n");
    write(sprintf("  %-22s  %-5s  %s\n", "Language", "Pct", "Proficiency"));
    write("  " + repeat_string("-", 50) + "\n");

    for(i = 0; i < sizeof(langs); i++) {
        pct = known[langs[i]];
        if(pct >= 98)       bar = "Fluent";
        else if(pct >= 80)  bar = "Very skilled";
        else if(pct >= 60)  bar = "Skilled";
        else if(pct >= 40)  bar = "Basic";
        else if(pct >= 20)  bar = "Rudimentary";
        else                bar = "Barely a word";
        write(sprintf("  %-22s  %-5d  %s%s\n",
            langs[i], pct, bar,
            (langs[i] == active ? " [active]" : "")
        ));
    }
    write("\nType 'language <name>' to switch your active speaking language.\n");
    return 1;
}

int help() {
    write(
        "Syntax: languages\n\n"
        "Lists all languages you know and your proficiency in each.\n"
        "The language marked [active] is what you speak when you use 'say'.\n\n"
        "See also: language, say, ooc\n"
    );
    return 1;
}
