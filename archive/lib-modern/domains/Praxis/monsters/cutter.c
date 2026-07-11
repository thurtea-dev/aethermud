// /d/Praxis/monsters/cutter.c
// Cutter -- black market dealer in the back alley shop.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_npc";

private mapping __items;

void create() {
    ::create();

    set_name("Cutter");
    set_npc_name("Cutter");
    set_id( ({ "cutter", "dealer", "merchant", "woman", "vendor", "black market" }) );
    set_short("Cutter, black market dealer");
    set_long(
        "A lean woman in her late thirties, with a buzzed left side and a long\n"
        "braid on the right. Her left eye is biological; her right is a matte-\n"
        "black cybernetic that tracks everything in the room simultaneously.\n"
        "She has the calm of someone who has been in worse situations than this\n"
        "and made a living from the wreckage.\n\n"
        "She stocks items the legitimate vendors won't touch. Try 'list' or\n"
        "'buy <item>'. She does not negotiate on price.");
    set_level(8);
    set_max_hp(150);
    set_hp(150);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(-50);
    set_body_type("human");

    add_response("hello", "Cutter nods once. 'You know where you are. Browse the list.'");
    add_response("hi", "Cutter nods once. 'You know where you are. Browse the list.'");
    add_response("list", "Cutter says: Type 'list' to see what I carry.");
    add_response("buy", "Cutter says: Type 'buy <item>'. Credits only. No trades.");
    add_response("sell", "Cutter says: I buy, sometimes. Not today.");
    add_response("prices", "Cutter says: Type 'list'. The price is the price.");
    add_response("name", "Cutter says: Just Cutter.");
    add_response("work", "Cutter says: Not hiring.");
    add_response("faction", "Cutter says: I work for whoever pays. Same as everyone here.");
    add_response("coalition","Cutter says: I've sold to them and against them. Doesn't matter to me.");
    add_response("lazlo", "Cutter says: Good customers. Ask no awkward questions.");
    add_response("bye", "Cutter says: Watch the alley on your way out.");
    add_response("help", "Cutter says: 'list' shows my stock. 'buy <name>' to purchase.");
    set_default_response("Cutter watches you without expression. 'Was that a question?'");

    __items = ([
        "dead boy armor" : ({ EQ_PATH+"dead_boy_armor.c", 18000, "Dead Boy armor (CS surplus)" }),
        "dead boy" : ({ EQ_PATH+"dead_boy_armor.c", 18000, "Dead Boy armor (CS surplus)" }),
        "eba" : ({ EQ_PATH+"environmental_body_armor.c", 22000, "Environmental Body Armor" }),
        "environmental" : ({ EQ_PATH+"environmental_body_armor.c", 22000, "Environmental Body Armor" }),
        "exotic e-clip" : ({ EQ_PATH+"splynn/exotic_energy_clip.c", 800, "exotic e-clip (60 shots)" }),
        "exotic clip" : ({ EQ_PATH+"splynn/exotic_energy_clip.c", 800, "exotic e-clip (60 shots)" }),
        "exotic" : ({ EQ_PATH+"splynn/exotic_energy_clip.c", 800, "exotic e-clip (60 shots)" }),
        "e-clip" : ({ EQ_PATH+"e_clip.c", 350, "standard e-clip" }),
        "eclip" : ({ EQ_PATH+"e_clip.c", 350, "standard e-clip" }),
        "vibro-sword" : ({ EQ_PATH+"vibro_sword.c", 6500, "vibro-sword" }),
        "vibro sword" : ({ EQ_PATH+"vibro_sword.c", 6500, "vibro-sword" }),
        "cp-40" : ({ EQ_PATH+"cp40_laser.c", 14000, "CP-40 pulse laser rifle (unmarked)" }),
        "cp40" : ({ EQ_PATH+"cp40_laser.c", 14000, "CP-40 pulse laser rifle (unmarked)" }),
        "rope" : ({ EQ_PATH+"rope.c", 75, "50-foot rope" }),
        "first aid kit" : ({ EQ_PATH+"first_aid_kit.c", 200, "first aid kit" }),
        "first aid" : ({ EQ_PATH+"first_aid_kit.c", 200, "first aid kit" }),
        "rations" : ({ EQ_PATH+"rations.c", 50, "field rations" })
    ]);
}

void init() {
    ::init();
    add_action("cmd_buy",   "buy");
    add_action("cmd_list",  "list");
    add_action("cmd_sell",  "sell");
    add_action("cmd_value", "value");
}

int cmd_list(string str) {
    string *ks;
    string seen;
    mixed item;
    int i;

    seen = "";
    write("Cutter says: What I carry:\n");
    write(" Item Price\n");
    write(" -------------------------------------- ----------\n");
    ks = keys(__items);
    i = sizeof(ks);
    while(i--) {
        item = __items[ks[i]];
        if(strsrch(seen, item[2]) != -1) continue;
        seen += item[2];
        write(sprintf(" %-38s %d credits\n", item[2], item[1]));
    }
    return 1;
}

int cmd_buy(string str) {
    string *ks, key, s;
    mixed item;
    object ob;
    int i, found;

    if(!str || !strlen(str)) {
        write("Buy what? Type 'list' to see available items.\n");
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
        tell_room(environment(this_object()),
            "Cutter says: I don't carry that. Type 'list'.\n",
            ({ this_object() }));
        return 1;
    }
    item = __items[key];
    if((int)this_player()->query_money("credits") < item[1]) {
        tell_room(environment(this_object()),
            "Cutter says: " + this_player()->query_cap_name() +
            ", you're short. That costs " + item[1] + " credits.\n",
            ({ this_object() }));
        return 1;
    }
    ob = clone_object(item[0]);
    if(!ob) {
        tell_room(environment(this_object()),
            "Cutter says: Out of that right now.\n",
            ({ this_object() }));
        return 1;
    }
    this_player()->add_money("credits", -item[1]);
    if(ob->move(this_player())) {
        write("You can't carry that.\n");
        ob->move(environment(this_object()));
        return 1;
    }
    write("You pay " + item[1] + " credits. Cutter slides " + item[2] + " across the counter.\n");
    tell_room(environment(this_object()),
        "Cutter slides " + item[2] + " across the counter to " +
        this_player()->query_cap_name() + ".\n",
        ({ this_object(), this_player() }));
    catch(FACTION_D->modify_standing(
        (string)this_player()->query_name(), "black_market", 5, "purchased from Cutter"));
    return 1;
}

int cmd_value(string str) {
    object ob;
    int val;
    int offer;

    if(!str || !strlen(str)) {
        write("Value what?\n");
        return 1;
    }
    ob = present(str, this_player());
    if(!ob) {
        tell_room(environment(this_object()),
            "Cutter says: Don't see that on you.\n",
            ({ this_object() }));
        return 1;
    }
    val = (int)ob->query_value();
    if(val <= 0) {
        tell_room(environment(this_object()),
            "Cutter says: Nothing. Worthless to me.\n",
            ({ this_object() }));
        return 1;
    }
    offer = val / 2;
    if(offer < 1) offer = 1;
    tell_room(environment(this_object()),
        "Cutter says: "+offer+" credits for "+
        (string)ob->query_short()+".\n",
        ({ this_object() }));
    return 1;
}

int cmd_sell(string str) {
    object ob;
    int val;
    int offer;

    if(!str || !strlen(str)) {
        write("Sell what?\n");
        return 1;
    }
    ob = present(str, this_player());
    if(!ob) {
        tell_room(environment(this_object()),
            "Cutter says: Don't see that on you.\n",
            ({ this_object() }));
        return 1;
    }
    val = (int)ob->query_value();
    if(val <= 0) {
        tell_room(environment(this_object()),
            "Cutter says: I don't buy junk.\n",
            ({ this_object() }));
        return 1;
    }
    offer = val / 2;
    if(offer < 1) offer = 1;
    ob->move(this_object());
    this_player()->add_money("credits", offer);
    write("Cutter pays you "+offer+" credits for "+
        (string)ob->query_short()+".\n");
    tell_room(environment(this_object()),
        this_player()->query_cap_name()+" sells "+
        (string)ob->query_short()+" to Cutter.\n",
        ({ this_object(), this_player() }));
    catch(FACTION_D->modify_standing(
        (string)this_player()->query_name(), "black_market", 2, "sold to Cutter"));
    return 1;
}
