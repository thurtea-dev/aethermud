// /d/Praxis/monsters/weapons_vendor.c
// Praxis Monument Square weapons dealer.
// Quest giver: cs_deserter. Trigger: "found him" / "found" to complete.

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();

    set_name("Griz");
    set_npc_name("Griz");
    set_id( ({ "griz", "weapons vendor", "vendor", "dealer", "merchant", "arms dealer" }) );
    set_short("Griz, weapons dealer");
    set_long(
        "A barrel-chested man with a mechanical right arm and a permanent "
        "squint from years of downrange work. His workbench is covered in\n"
        "disassembled energy weapons and tools. A hand-painted sign above\n"
        "him reads: GRIZ ARMS: NO CREDIT, NO PITY.\n"
        "\nHe sells weapons and repairs armor. Try 'list' or 'buy <item>'.\n"
        "Say 'repair' to ask about armor repair prices.");
    set_level(5);
    set_max_hp(100);
    set_hp(100);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(0);
    set_body_type("human");

    add_response("hello", "Griz nods once. 'Browse the list. Credits only.'");
    add_response("hi", "Griz nods once. 'Browse the list. Credits only.'");
    add_response("list", "Griz says: Type 'list' to see my stock.");
    add_response("buy", "Griz says: Type 'buy <item>' to purchase. Credits only.");
    add_response("sell", "Griz says: I don't buy. Try the black market.");
    add_response("repair", "Griz says: 'repair <armor name>'. MDC costs 100 credits per point. SDC runs 10 per point. Minimum charge applies.");
    add_response("fix", "Griz says: 'repair <armor name>'. MDC costs 100 credits per point. SDC runs 10 per point.");
    add_response("prices", "Griz says: Type 'list' for current prices.");
    add_response("work", "");
    add_response("bounty", "");
    add_response("quest", "");
    add_response("found", "");
    add_response("bye", "Griz grunts. 'Watch your six out there.'");
    add_response("help", "Griz says: 'list' shows my stock. 'buy <name>' buys it.");
    set_default_response("Griz shrugs and goes back to cleaning his rifle.");

    set_shop_sells(0);
    set_shop_items(([
        "cp-40" : ({ EQ_PATH+"cp40_laser.c", 12000, "CP-40 pulse laser rifle" }),
        "cp40" : ({ EQ_PATH+"cp40_laser.c", 12000, "CP-40 pulse laser rifle" }),
        "cp-30" : ({ EQ_PATH+"cp30_laser.c", 6000, "CP-30 heavy laser pistol" }),
        "cp30" : ({ EQ_PATH+"cp30_laser.c", 6000, "CP-30 heavy laser pistol" }),
        "c-18" : ({ EQ_PATH+"c18_pistol.c", 4500, "C-18 laser pistol" }),
        "c18" : ({ EQ_PATH+"c18_pistol.c", 4500, "C-18 laser pistol" }),
        "vibro-knife": ({ EQ_PATH+"vibro_knife.c", 2000, "vibro-knife" }),
        "vibro knife": ({ EQ_PATH+"vibro_knife.c", 2000, "vibro-knife" }),
        "vibro-sword": ({ EQ_PATH+"vibro_sword.c", 5000, "vibro-sword" }),
        "vibro sword": ({ EQ_PATH+"vibro_sword.c", 5000, "vibro-sword" }),
        "e-clip" : ({ EQ_PATH+"e_clip.c", 300, "standard e-clip (30 shots)" }),
        "eclip" : ({ EQ_PATH+"e_clip.c", 300, "standard e-clip (30 shots)" }),
        "clip" : ({ EQ_PATH+"e_clip.c", 300, "standard e-clip (30 shots)" }),
        "long e-clip" : ({ EQ_PATH+"ammo/e_clip_long.c", 500, "long e-clip (60 shots)" }),
        "long clip"   : ({ EQ_PATH+"ammo/e_clip_long.c", 500, "long e-clip (60 shots)" }),
        "frag grenade"  : ({ EQ_PATH+"grenades/frag_grenade.c",   50, "fragmentation grenade" }),
        "frag"          : ({ EQ_PATH+"grenades/frag_grenade.c",   50, "fragmentation grenade" }),
        "plasma grenade": ({ EQ_PATH+"grenades/plasma_grenade.c", 200, "plasma grenade" }),
        "plasma"        : ({ EQ_PATH+"grenades/plasma_grenade.c", 200, "plasma grenade" }),
        "smoke grenade" : ({ EQ_PATH+"grenades/smoke_grenade.c",  30, "smoke grenade" }),
        "smoke"         : ({ EQ_PATH+"grenades/smoke_grenade.c",  30, "smoke grenade" })
    ]));
}

void init() {
    ::init();
    add_action("do_repair", "repair");
}

int do_repair(string str) {
    object tp, armor, env;
    object *inv;
    int max_mdc, cur_mdc, max_sdc, cur_sdc, missing, cost, i;

    tp = this_player();
    if(!tp) return 0;
    if(!str || !sizeof(str)) {
        say_here("Repair what? Name a piece of armor you're wearing or carrying.");
        return 1;
    }
    armor = 0;
    inv = all_inventory(tp);
    for(i = 0; i < sizeof(inv); i++) {
        if(inv[i] && (int)inv[i]->is_armour() && (int)inv[i]->id(str)) {
            armor = inv[i];
            break;
        }
    }
    if(!armor) {
        say_here("I don't see that on you.");
        return 1;
    }
    max_mdc = (int)armor->query_property("armor_mdc");
    cur_mdc = (int)armor->query_property("current_armor_mdc");
    max_sdc = (int)armor->query_property("armor_sdc");
    cur_sdc = (int)armor->query_property("current_armor_sdc");

    if(max_mdc > 0) {
        if(cur_mdc <= 0) cur_mdc = max_mdc;
        missing = max_mdc - cur_mdc;
        if(missing <= 0) {
            say_here("That armor is already at full MDC. No work needed.");
            return 1;
        }
        cost = missing * 100;
        if(cost < 200) cost = 200;
        if((int)tp->query_money("credits") < cost) {
            say_here("Repairing that will cost " + cost +
                " credits. You're short.");
            return 1;
        }
        tp->add_money("credits", -cost);
        armor->set_property("current_armor_mdc", max_mdc);
        write("Griz works on your " + (string)armor->query_name() +
            " and bills you " + cost + " credits. Back to " +
            max_mdc + " MDC.\n");
        env = environment(this_object());
        if(env) tell_room(env, "Griz repairs " +
            (string)tp->query_cap_name() + "'s " +
            (string)armor->query_name() + ".\n",
            ({ this_object(), tp }));
        if((int)FACTION_D->is_cs_ally(tp))
            say_here("CS gear takes a beating and comes back. Good armor.");
        else if((int)FACTION_D->query_rep(tp, "coalition") <= -100)
            say_here("Done. Don't tell anyone I touched it.");
        return 1;
    }
    if(max_sdc > 0) {
        if(cur_sdc <= 0) cur_sdc = max_sdc;
        missing = max_sdc - cur_sdc;
        if(missing <= 0) {
            say_here("That armor is at full SDC already. No work needed.");
            return 1;
        }
        cost = missing * 10;
        if(cost < 50) cost = 50;
        if((int)tp->query_money("credits") < cost) {
            say_here("Patching that up costs " + cost +
                " credits. You're short.");
            return 1;
        }
        tp->add_money("credits", -cost);
        armor->set_property("current_armor_sdc", max_sdc);
        write("Griz patches up your " + (string)armor->query_name() +
            " for " + cost + " credits. SDC restored to " +
            max_sdc + ".\n");
        env = environment(this_object());
        if(env) tell_room(env, "Griz repairs " +
            (string)tp->query_cap_name() + "'s " +
            (string)armor->query_name() + ".\n",
            ({ this_object(), tp }));
        if((int)FACTION_D->is_cs_ally(tp))
            say_here("CS materials. Easy work.");
        else if((int)FACTION_D->query_rep(tp, "coalition") <= -100)
            say_here("Done. We're square.");
        return 1;
    }
    say_here("I can't repair that. It's not MDC or SDC rated armor.");
    return 1;
}

void catch_tell(string str) {
    string a, b;
    object tp;

    if(sscanf(str, "%s says: %s", a, b) != 2) return;
    b = lower_case(b);

    if(strsrch(b, "work") != -1 || strsrch(b, "bounty") != -1 ||
       strsrch(b, "quest") != -1) {
        tp = this_player();
        if(tp) call_out("offer_cs_deserter", 1, tp);
        return;
    }

    if(strsrch(b, "found") != -1) {
        tp = this_player();
        if(tp) call_out("try_complete_deserter", 1, tp);
        return;
    }

    /* Faction-aware greeting -- overrides protected add_response */
    if(strsrch(b, "hello") != -1 || strsrch(b, "hi") != -1) {
        tp = this_player();
        if(!tp) { ::catch_tell(str); return; }
        if((int)FACTION_D->is_cs_ally(tp))
            say_here("Griz glances up. 'CS contract? Check the list. Got fresh stock.'");
        else if((int)FACTION_D->query_rep(tp, "coalition") <= -100)
            say_here("Griz watches you. 'Credits upfront. No exceptions, no trouble.'");
        else
            say_here("Griz nods once. 'Browse the list. Credits only.'");
        return;
    }

    ::catch_tell(str);
}

void offer_cs_deserter(object tp) {
    if(!tp || !objectp(tp)) return;
    if((int)QUEST_D->is_quest_complete(tp, "cs_deserter") ||
       (int)QUEST_D->is_quest_active(tp, "cs_deserter")) {
        say_here("I'm still waiting on you. Find that deserter.");
        return;
    }
    say_here("There's a CS soldier who went AWOL. Hiding around here somewhere. "
             "My contact at the checkpoint will pay for a location. Find him, "
             "come back and say 'found him'. Pay is good.");
    catch(QUEST_D->start_quest(tp, "cs_deserter"));
}

void try_complete_deserter(object tp) {
    mapping aq, prog;

    if(!tp || !objectp(tp)) return;
    if(!(int)QUEST_D->is_quest_active(tp, "cs_deserter")) {
        say_here("Found what? I don't know what you're talking about.");
        return;
    }
    aq = (mapping)QUEST_D->query_active_quests(tp);
    prog = (mapping)aq["cs_deserter"];
    if(!mapp(prog) || (int)prog["deserter_found"] < 1) {
        say_here("You haven't found him yet. Check the alleys off Market Street.");
        return;
    }
    say_here("Location confirmed. My contact handles the rest. Here's your pay. "
             "Don't ask what happens next.");
    catch(QUEST_D->complete_quest(tp, "cs_deserter"));
}
