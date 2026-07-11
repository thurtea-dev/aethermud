/* /domains/NGR/npcs/triax_rep.c
   Triax Industries sales representative. Showroom vendor. */

#include <std.h>
#include <daemons.h>

#define EQ_NGR "/domains/NGR/equipment/"
#define EQ_PR  "/domains/Praxis/equipment/"

inherit "/std/rifts_npc";

private mapping __items;

void create() {
    ::create();
    set_name("triax sales representative");
    set_npc_name("Sales Rep");
    set_id( ({ "triax sales representative", "sales representative",
        "representative", "sales rep", "rep" }) );
    set_short("a Triax sales representative");
    set_long(
        "A crisp-suited representative of Triax Industries with a\n"
        "salesman's smile and a soldier's haircut. A discreet pin on the\n"
        "lapel is the Triax gear-and-eagle. Every answer comes with a\n"
        "spec sheet.\n"
        "\nType 'list' for the catalog. 'buy <item>' to purchase.");
    set_level(4);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(200);
    set_body_type("human");
    add_response("hello",
        "Sales Rep says: 'Welcome to Triax. Protection is not a luxury on this continent.'");
    add_response("warranty",
        "Sales Rep says: 'Full warranty, excepting acts of war, acts of gods, and acts of gargoyles. So, the warranty is decorative.'");
    add_response("bye",
        "Sales Rep says: 'Triax thanks you. Stay armored.'");
    set_default_response("The sales rep straightens a spec card by two millimeters.");

    __items = ([
        "triax helmet" : ({ EQ_NGR+"triax_helmet.c", 4000, "Triax helmet" }),
        "helmet" : ({ EQ_NGR+"triax_helmet.c", 4000, "Triax helmet" }),
        "ngr body armor" : ({ EQ_PR+"ngr_body_armor.c", 18000, "NGR body armor" }),
        "body armor" : ({ EQ_PR+"ngr_body_armor.c", 18000, "NGR body armor" }),
        "armor" : ({ EQ_PR+"ngr_body_armor.c", 18000, "NGR body armor" }),
        "ngr rail gun" : ({ EQ_PR+"ngr_rail_gun.c", 35000, "NGR rail gun" }),
        "rail gun" : ({ EQ_PR+"ngr_rail_gun.c", 35000, "NGR rail gun" }),
        "railgun" : ({ EQ_PR+"ngr_rail_gun.c", 35000, "NGR rail gun" })
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
    write("Sales Rep says: 'The current catalog.'\n");
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
        write("Buy what? Type 'list' for the catalog.\n");
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
        write("Sales Rep says: 'Not in this quarter's catalog.'\n");
        return 1;
    }
    item = __items[key];
    if((int)this_player()->query_money("credits") < item[1]) {
        write("Sales Rep says: 'That model runs " + item[1] +
            " credits. Triax offers no financing to non-citizens.'\n");
        return 1;
    }
    ob = clone_object(item[0]);
    if(!ob) {
        write("Sales Rep says: 'On back order, I am afraid.'\n");
        return 1;
    }
    this_player()->add_money("credits", -item[1]);
    if(ob->move(this_player())) {
        write("You cannot carry that.\n");
        ob->move(environment(this_object()));
        return 1;
    }
    write("You pay " + item[1] + " credits. The rep hands over the " +
        item[2] + " with a receipt in triplicate.\n");
    tell_room(environment(this_object()),
        "The sales rep hands " + this_player()->query_cap_name() + " a " +
        item[2] + ".\n", ({ this_object(), this_player() }));
    return 1;
}
