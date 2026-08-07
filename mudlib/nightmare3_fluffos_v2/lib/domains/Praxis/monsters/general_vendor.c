// /d/Praxis/monsters/general_vendor.c
// Praxis Monument Square general goods merchant.
// Quest giver: lost_supplies. Accepts supply crate return ("supplies" trigger).

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();

    set_name("Marta");
    set_npc_name("Marta");
    set_id( ({ "marta", "general vendor", "vendor", "merchant", "shopkeeper", "trader" }) );
    set_short("Marta, general goods merchant");
    set_long(
        "A middle-aged woman with sun-scarred skin and watchful eyes. She\n"
        "runs her stall from behind a reinforced counter covered in salvaged\n"
        "goods. A battered pre-Rifts cash register sits beside her.\n"
        "\nShe sells survival gear. Try 'list' or 'buy <item>'.");
    set_level(3);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(200);
    set_body_type("human");

    add_response("hello", "Marta says: Welcome. Got supplies for the road if you need them.");
    add_response("hi", "Marta says: Welcome. Got supplies for the road if you need them.");
    add_response("list", "Marta says: Type 'list' to see my stock.");
    add_response("buy", "Marta says: Type 'buy <item>' to purchase. Credits only.");
    add_response("bye", "Marta says: Stay alive out there. It's dangerous.");
    set_property("position_str", "stands behind the counter.");
    add_response("work", "");
    add_response("job", "");
    add_response("quest", "");
    add_response("supplies","");
    add_response("help", "Marta says: 'list' shows my goods. 'buy <name>' buys one.");
    set_default_response("Marta smiles politely and waits.");

    set_shop_items(([
        "leather jacket" : ({ EQ_PATH+"leather_jacket.c", 800, "leather jacket armor" }),
        "jacket" : ({ EQ_PATH+"leather_jacket.c", 800, "leather jacket armor" }),
        "first aid kit" : ({ EQ_PATH+"first_aid_kit.c", 200, "first aid kit" }),
        "aid kit" : ({ EQ_PATH+"first_aid_kit.c", 200, "first aid kit" }),
        "medkit" : ({ EQ_PATH+"first_aid_kit.c", 200, "first aid kit" }),
        "rations" : ({ EQ_PATH+"rations.c", 50, "field rations" }),
        "ration" : ({ EQ_PATH+"rations.c", 50, "field rations" }),
        "food" : ({ EQ_PATH+"rations.c", 50, "field rations" }),
        "rope" : ({ EQ_PATH+"rope.c", 100, "50 feet of rope" }),
        "shovel" : ({ EQ_PATH+"shovel.c", 150, "entrenching shovel" }),
        "spade" : ({ EQ_PATH+"shovel.c", 150, "entrenching shovel" }),
        "leather glove" : ({ EQ_PATH+"leather_glove.c", 100, "leather falconry glove" }),
        "glove" : ({ EQ_PATH+"leather_glove.c", 100, "leather falconry glove" })
    ]));
}

void catch_tell(string str) {
    string a, b;
    object tp;

    if(sscanf(str, "%s says: %s", a, b) != 2) return;
    b = lower_case(b);

    if(strsrch(b, "work") != -1 || strsrch(b, "job") != -1 ||
       strsrch(b, "quest") != -1) {
        tp = this_player();
        if(tp) call_out("offer_lost_supplies", 1, tp);
        return;
    }

    if(strsrch(b, "supplies") != -1 || strsrch(b, "crate") != -1) {
        tp = this_player();
        if(tp) call_out("try_complete_supplies", 1, tp);
        return;
    }

    /* Faction-aware greeting */
    if(strsrch(b, "hello") != -1 || strsrch(b, "hi") != -1) {
        tp = this_player();
        if(!tp) { ::catch_tell(str); return; }
        if((int)FACTION_D->query_rep(tp, "coalition") <= -100)
            say_here("Marta says: Welcome. I sell to everyone - credits are credits, no paperwork.");
        else
            say_here("Marta says: Welcome. Got supplies for the road if you need them.");
        return;
    }

    /* Faction-aware buy prompt */
    if(strsrch(b, "buy") != -1) {
        tp = this_player();
        if(!tp) { ::catch_tell(str); return; }
        if((int)FACTION_D->query_rep(tp, "coalition") <= -100)
            say_here("Marta says: Type 'buy <item>'. Quick transaction, no one has to know.");
        else
            say_here("Marta says: Type 'buy <item>' to purchase. Credits only.");
        return;
    }

    ::catch_tell(str);
}

void offer_lost_supplies(object tp) {
    if(!tp || !objectp(tp)) return;
    if((int)QUEST_D->is_quest_complete(tp, "lost_supplies")) {
        say_here("I don't have anything else for you right now. Come back later.");
        return;
    }
    if((int)QUEST_D->is_quest_active(tp, "lost_supplies")) {
        say_here("Still waiting on that Lazlo crate. Check the highway north of the old checkpoint.");
        return;
    }
    say_here("Actually, yes. A Lazlo supply crate went missing on the highway "
             "north of the old checkpoint. Bandits, I think. If you find it "
             "and bring it back, I'll pay. Say 'supplies' when you return.");
    catch(QUEST_D->start_quest(tp, "lost_supplies"));
}

void try_complete_supplies(object tp) {
    object crate;

    if(!tp || !objectp(tp)) return;
    if(!(int)QUEST_D->is_quest_active(tp, "lost_supplies")) {
        say_here("I'm not sure what you mean.");
        return;
    }
    crate = present("supply crate", tp);
    if(!crate) {
        say_here("I don't see any supply crate. The one from the Lazlo shipment?");
        return;
    }
    crate->remove();
    say_here("This is it! The Lazlo shipment seal is intact. Here's what I owe you.");
    catch(QUEST_D->update_objective(tp, "lost_supplies", "supply_crate", 1));
    catch(QUEST_D->complete_quest(tp, "lost_supplies"));
}
