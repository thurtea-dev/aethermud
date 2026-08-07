// /domains/NewCamelot/npcs/camelot_mage.c
// Lazlo-affiliated mage in the Camelot Order tower. Sells spell scrolls.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

private mapping __scrolls;

void create() {
    ::create();

    set_name("camelot mage");
    set_npc_name("Mage");
    set_id( ({ "camelot mage", "mage", "wizard", "scholar", "sorcerer" }) );
    set_short("a Lazlo-affiliated mage");
    set_long(
        "A lean figure in deep blue robes covered in subtle magical notation.\n"
        "The mage's eyes have the slightly luminous quality of someone who has\n"
        "spent years working directly with ley line energy. Spell components\n"
        "are organized on the desk with careful precision.\n"
        "\nAsk about 'scrolls' or 'magic'. Entry requires a 100-credit donation.");
    set_level(12);
    set_max_hp(200);
    set_hp(200);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(600);
    set_body_type("human");

    add_response("hello", "");
    add_response("hi", "");
    add_response("scrolls", "");
    add_response("list", "");
    add_response("buy", "");
    add_response("magic", "");
    add_response("lazlo", "Mage says: 'The Lazlo scholars have been most generous. We share knowledge freely.'");
    add_response("donate", "Mage says: 'Thank you. Your contribution supports magical research.'");
    add_response("bye", "Mage says: 'Study well. Knowledge is the only defense against entropy.'");
    set_default_response("The mage glances up from the texts with measured interest.");

    __scrolls = ([
        "energy bolt" : ({ "energy bolt", 2000,
                             "A scroll of Energy Bolt. Teaches the spell upon reading." }),
        "armor of ithan": ({ "armor of ithan", 3000,
                             "A scroll of Armor of Ithan. Teaches the spell upon reading." }),
        "fire bolt" : ({ "fire bolt", 2500,
                             "A scroll of Fire Bolt. Teaches the spell upon reading." })
    ]);
}

void init() {
    ::init();
    add_action("cmd_list", "list");
    add_action("cmd_buy", "buy");
}

void catch_tell(string str) {
    string a;
    string b;
    object tp;

    if(sscanf(str, "%s says: %s", a, b) != 2) {
        ::catch_tell(str);
        return;
    }
    b = lower_case(b);
    tp = this_player();
    if(!tp) { ::catch_tell(str); return; }

    if(strsrch(b, "hello") != -1 || strsrch(b, "hi") != -1) {
        say_here("'Welcome to the tower. I sell spell scrolls and offer instruction. "
                 "Entry requires a 100-credit donation to the Order.'");
        return;
    }
    if(strsrch(b, "scroll") != -1 || strsrch(b, "list") != -1 ||
       strsrch(b, "magic") != -1) {
        call_out("show_scrolls", 1, tp);
        return;
    }
    ::catch_tell(str);
}

void show_scrolls(object tp) {
    string *ks;
    mixed scroll;
    int i;

    if(!tp || !objectp(tp)) return;
    say_here("I carry the following scrolls:");
    write(" Scroll Price\n");
    write(" -------------------- ----------\n");
    ks = keys(__scrolls);
    i = sizeof(ks);
    while(i--) {
        scroll = __scrolls[ks[i]];
        write(sprintf(" %-20s %d credits\n", capitalize(scroll[0]), scroll[1]));
    }
    write("Type: buy <scroll name>\n");
}

int cmd_list(string str) {
    show_scrolls(this_player());
    return 1;
}

int cmd_buy(string str) {
    string *ks;
    string s;
    mixed scroll;
    int i;
    int found;
    string key;
    string known;
    string spell_key;

    if(!str || !strlen(str)) {
        write("Buy which scroll? Type 'list' to see available scrolls.\n");
        return 1;
    }
    s = lower_case(str);
    ks = keys(__scrolls);
    i = sizeof(ks);
    found = 0;
    key = "";
    while(i--) {
        if(strsrch(s, ks[i]) != -1 || strsrch(ks[i], s) != -1) {
            key = ks[i];
            found = 1;
            break;
        }
    }
    if(!found) {
        say_here("I don't carry that scroll. Type 'list' for what I have.");
        return 1;
    }
    scroll = __scrolls[key];
    if((int)this_player()->query_money("credits") < scroll[1]) {
        say_here("That scroll costs " + scroll[1] + " credits. You don't have enough.");
        return 1;
    }
    known = (string)this_player()->getenv("known_spells");
    if(!known) known = "";
    spell_key = lower_case(scroll[0]);
    if(strsrch(known, spell_key) != -1) {
        say_here("You already know that spell. No need to purchase the scroll.");
        return 1;
    }
    this_player()->add_money("credits", -scroll[1]);
    if(!strlen(known))
        this_player()->setenv("known_spells", spell_key);
    else
        this_player()->setenv("known_spells", known + "," + spell_key);
    catch(FACTION_D->modify_standing(
        (string)this_player()->query_name(),
        "lazlo", 10, "purchased magical knowledge"));
    write("You pay " + scroll[1] + " credits. The mage hands you " + scroll[2] + "\n");
    write("You study the scroll. The knowledge settles into your mind.\n");
    write("Spell learned: " + capitalize(scroll[0]) + "\n");
    tell_room(environment(this_object()),
        "The mage hands " + this_player()->query_cap_name() + " a scroll.\n",
        ({ this_object(), this_player() }));
    return 1;
}
