// /cmds/mortal/_say.c
// Say command with Rifts language system.
// Wizards bypass language: everyone hears them directly.
// Mortals with an active language route through the language daemon.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_say(string str) {
    object *inv, ob;
    string lang, pname, guild_tag;
    int i, pct, tag_on;

    if(this_player()->query_ghost()) {
        message("talk", "You howl pitifully.", this_player());
        message("talk", "You hear a pitiful howl.",
            environment(this_player()), ({ this_player() }));
        return 1;
    }
    if(!str || str == "") {
        notify_fail("You mutter to yourself.\n");
        return 0;
    }
    if((int)this_player()->query_property("is_sneaking")) {
        this_player()->remove_property("is_sneaking");
        write("You give up your silent movements as you speak.\n");
    }

    tag_on    = (int)this_player()->query_property("guild_tag_on");
    guild_tag = (string)this_player()->query_property("guild_tag");

    if(creatorp(this_player())) {
        write("You say, \"" + str + "\"\n");
        this_player()->tell_room_living(environment(this_player()),
            this_player(), 0, " says, \"" + str + "\"\n");
        return 1;
    }

    lang = (string)LANGUAGE_D->query_active(this_player());

    // No active language: plain say with no language attribution.
    if(!lang || lang == "") {
        write("You say, \"" + str + "\"\n");
        this_player()->tell_room_living(environment(this_player()),
            this_player(), 0, " says, \"" + str + "\"\n");
        return 1;
    }

    // Active language: speaker hears themselves clearly.
    write("You say, \"" + str + "\" in " + lang + ".\n");

    // Each listener gets filtered output per their comprehension.
    inv = all_inventory(environment(this_player()));
    for(i = 0; i < sizeof(inv); i++) {
        ob = inv[i];
        if(!ob || ob == this_player()) continue;
        if(!living(ob)) continue;
        if(creatorp(ob) || (int)ob->query_property("tongues_active"))
            pct = 100;
        else
            pct = (int)LANGUAGE_D->can_understand(ob, lang);
        if(pct > 0) {
            pname = (string)this_player()->query_display_name(ob);
            if(tag_on && guild_tag && strlen(guild_tag) > 0)
                pname = "[" + guild_tag + "] " + pname;
            message("talk",
                pname + " says, \"" + str + "\" in " + lang + ".",
                ob);
        } else {
            pname = (string)this_player()->query_display_name(ob);
            if(tag_on && guild_tag && strlen(guild_tag) > 0)
                pname = "[" + guild_tag + "] " + pname;
            message("talk",
                pname + " says something in a language you do not understand.",
                ob);
        }
    }
    return 1;
}

void help() {
    write(
        "Syntax: say <message>\n\n"
        "Speaks aloud to everyone in the room in your active language.\n"
        "Listeners who do not know your language cannot understand you.\n"
        "Wizards always speak in plain text and understand all languages.\n\n"
        "See also: language, languages, tell, whisper, shout, ooc\n"
    );
}
