/* /domains/NewCamelot/npcs/camelot_blacksmith.c
   New Camelot blacksmith vendor. Sells Camelot-pattern MDC melee gear. */

#include <std.h>
#include <daemons.h>

#define EQ_PATH_NC "/domains/NewCamelot/equipment/"

inherit "/std/rifts_npc";

private mapping __items;

void create() {
    ::create();
    set_name("camelot blacksmith");
    set_npc_name("Blacksmith");
    set_id( ({ "camelot blacksmith", "blacksmith", "smith" }) );
    set_short("the New Camelot blacksmith");
    set_long(
        "A wiry woman with a cybernetic left arm that ends in an\n"
        "interchangeable tool mount, currently a hammer. Sweat and coal\n"
        "dust streak her face. She forges medieval steel around mega-\n"
        "damage cores, the New Camelot way.\n"
        "\nType 'list' to see her stock. 'buy <item>' to purchase.");
    set_level(6);
    set_max_hp(120);
    set_hp(120);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(300);
    set_body_type("human");
    add_response("hello",
        "Blacksmith says: 'Welcome to the forge. Mind the sparks.'");
    add_response("hi",
        "Blacksmith says: 'Welcome to the forge. Mind the sparks.'");
    add_response("bye",
        "Blacksmith says: 'Keep it out of the rain.'");
    set_default_response("The blacksmith turns a cooling blade, checking the edge.");

    __items = ([
        "camelot longsword" : ({ EQ_PATH_NC+"camelot_longsword.c", 9000, "camelot longsword" }),
        "longsword" : ({ EQ_PATH_NC+"camelot_longsword.c", 9000, "camelot longsword" }),
        "long sword" : ({ EQ_PATH_NC+"camelot_longsword.c", 9000, "camelot longsword" }),
        "chain mail" : ({ EQ_PATH_NC+"chain_mail.c", 1500, "chain mail" }),
        "chainmail" : ({ EQ_PATH_NC+"chain_mail.c", 1500, "chain mail" }),
        "short sword" : ({ EQ_PATH_NC+"short_sword.c", 800, "short sword" }),
        "shortsword" : ({ EQ_PATH_NC+"short_sword.c", 800, "short sword" }),
        "knight shield" : ({ EQ_PATH_NC+"knight_shield.c", 1200, "knight shield" }),
        "shield" : ({ EQ_PATH_NC+"knight_shield.c", 1200, "knight shield" })
    ]);
}

void init() {
    ::init();
    add_action("cmd_buy",  "buy");
    add_action("cmd_list", "list");
}

int cmd_list(string str) {
    string *ks;
    mixed item;
    string seen;
    int i;

    seen = "";
    write("Blacksmith says: 'Forge work, priced fair.'\n");
    write(" Item                             Price\n");
    write(" -------------------------------- ----------\n");
    ks = keys(__items);
    i = sizeof(ks);
    while(i--) {
        item = __items[ks[i]];
        if(strsrch(seen, item[2]) != -1) continue;
        seen += item[2];
        write(sprintf(" %-32s %d credits\n", item[2], item[1]));
    }
    return 1;
}

int cmd_buy(string str) {
    string *ks;
    string s;
    mixed item;
    object ob;
    int i;
    int found;
    string key;

    if(!str || !strlen(str)) {
        write("Buy what? Type 'list' to see available goods.\n");
        return 1;
    }
    s = lower_case(str);
    ks = keys(__items);
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
        write("Blacksmith says: 'Don't carry that. Type list for stock.'\n");
        return 1;
    }
    item = __items[key];
    if((int)this_player()->query_money("credits") < item[1]) {
        write("Blacksmith says: 'That's " + item[1] +
            " credits. Come back when you have it.'\n");
        return 1;
    }
    ob = clone_object(item[0]);
    if(!ob) {
        write("Blacksmith says: 'Out of stock right now.'\n");
        return 1;
    }
    this_player()->add_money("credits", -item[1]);
    if(ob->move(this_player())) {
        write("You cannot carry that.\n");
        ob->move(environment(this_object()));
        return 1;
    }
    write("You pay " + item[1] + " credits. The blacksmith hands you " +
        item[2] + ".\n");
    tell_room(environment(this_object()),
        "The blacksmith hands " + this_player()->query_cap_name() + " " +
        item[2] + ".\n", ({ this_object(), this_player() }));
    return 1;
}
