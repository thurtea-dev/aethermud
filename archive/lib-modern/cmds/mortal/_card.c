// /cmds/mortal/_card.c
// Business card system for player introductions.
// card set <text>     -- set your card message
// card                -- show your card in the room
// card show <player>  -- show your card to one player

#include <std.h>

inherit DAEMON;

#define MAX_CARD 240

private string card_text(object who) {
    string t;

    t = (string)who->query_property("business_card");
    if(!t || !sizeof(t)) return 0;
    return t;
}

int cmd_card(string str) {
    object who;
    object target;
    string arg;
    string text;

    who = this_player();
    if(!str || str == "") {
        text = card_text(who);
        if(!text) {
            write("You have not set a business card yet. Use: card set <message>\n");
            return 1;
        }
        message("say",
            (string)who->query_cap_name() + " offers a business card:\n  \"" +
            text + "\"\n",
            environment(who));
        return 1;
    }

    if(sscanf(str, "set %s", arg) == 1) {
        if(sizeof(arg) > MAX_CARD) {
            write("Your card may be at most " + MAX_CARD + " characters.\n");
            return 1;
        }
        who->set_property("business_card", arg);
        who->save_player((string)who->query_name());
        write("Business card updated.\n");
        return 1;
    }

    if(sscanf(str, "show %s", arg) == 1) {
        text = card_text(who);
        if(!text) {
            write("You have not set a business card yet. Use: card set <message>\n");
            return 1;
        }
        target = present(arg, environment(who));
        if(!target || !userp(target)) {
            write("You don't see that player here.\n");
            return 1;
        }
        write("You hand your business card to " +
              (string)target->query_cap_name() + ".\n");
        tell_object(target,
            (string)who->query_cap_name() + " hands you a business card:\n  \"" +
            text + "\"\n");
        tell_room(environment(who),
            (string)who->query_cap_name() + " hands a business card to " +
            (string)target->query_cap_name() + ".\n",
            ({ who, target }));
        return 1;
    }

    if(str == "clear") {
        who->remove_property("business_card");
        who->save_player((string)who->query_name());
        write("Business card cleared.\n");
        return 1;
    }

    write("Usage:\n"
          "  card              show your card in the room\n"
          "  card set <text>   set your card message\n"
          "  card show <name>  hand your card to someone\n"
          "  card clear        remove your card\n");
    return 1;
}

void help() {
    write(
        "Syntax: card [set <text>|show <player>|clear]\n\n"
        "Store and share a short business card for roleplay introductions.\n"
        "Cards persist on your character save.\n\n"
        "See also: introduce, who, biography\n"
    );
}
