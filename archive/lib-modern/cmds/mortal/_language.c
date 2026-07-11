// /cmds/mortal/_language.c
// Switch the active speaking language.
// Usage: language           -- show current active language
//        language <name>    -- switch to that language

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_language(string str) {
    string current, lang;
    mapping known;
    string *ks;
    int i;

    current = (string)LANGUAGE_D->query_active(this_player());

    if(!str || str == "") {
        write("You are currently speaking: " + current + "\n");
        write("Type 'languages' to see all languages you know.\n");
        return 1;
    }

    // Normalize: capitalize first letter of each word
    lang = capitalize(lower_case(str));

    // Try to match against known languages
    known = (mapping)LANGUAGE_D->query_known(this_player());
    ks = keys(known);

    // Exact match first
    for(i = 0; i < sizeof(ks); i++) {
        if(lower_case(ks[i]) == lower_case(lang)) {
            lang = ks[i];
            break;
        }
    }

    if(!(int)LANGUAGE_D->set_active(this_player(), lang)) {
        write("You do not know the language '" + lang + "'.\n");
        write("Type 'languages' to see languages you know.\n");
        return 1;
    }
    write("You are now speaking " + lang + ".\n");
    return 1;
}

void help() {
    write(
        "Syntax: language\n"
        "        language <name>\n\n"
        "Without an argument, shows your active speaking language.\n"
        "With a language name, switches to that language.\n\n"
        "You can only switch to languages you know.  Use 'languages'\n"
        "to see all languages available to you and your proficiency.\n\n"
        "See also: languages, say, ooc\n"
    );
}
