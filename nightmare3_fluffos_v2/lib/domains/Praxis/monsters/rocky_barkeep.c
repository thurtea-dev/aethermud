/* /d/Praxis/monsters/rocky_barkeep.c
   Rocky's Bar barkeep.
   Serves drinks, shares rumors, runs talisman mini-quest, triggers Dead Signal quest. */

#include <std.h>
#include <daemons.h>

#define DRINK_PRICE 50
#define DRINK_HEAL 5

inherit "/std/rifts_npc";

private string *__rumors;
private mapping __salvage_count;

void create() {
    ::create();

    set_name("Rocky");
    set_npc_name("Rocky");
    set_id( ({ "rocky", "barkeep", "bartender", "keeper", "bar" }) );
    set_short("Rocky, barkeep");
    set_long(
        "A heavyset man with chrome-plated forearms and a scar that runs\n"
        "from his left ear to his chin. He moves behind the bar with the\n"
        "practiced ease of someone who has broken up more than a few fights.\n"
        "A hand-painted sign behind him reads: ROCKY'S: NO RIFTS MAGIC\n"
        "INSIDE. VIOLATIONS WILL BE PROSECUTED.\n"
        "\nHe serves drinks. Try 'buy drink' or 'say rumor'.");
    set_level(6);
    set_max_hp(120);
    set_hp(120);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(100);
    set_body_type("human");

    add_response("hello", "Rocky says: What're you drinking?");
    add_response("hi", "Rocky says: What're you drinking?");
    set_repeat_greeting("Rocky says: Back again. You know where the bar is.");
    add_response("rocky", "Rocky says: That's me. Barkeep. Don't ask about the scar.");
    add_response("drink", "Rocky says: One drink is 50 credits. Type 'buy drink'.");
    add_response("buy", "Rocky says: Type 'buy drink', 50 credits each.");
    add_response("rumor", "");
    add_response("news", "");
    add_response("rifts", "Rocky says: Rifts Earth. Year 107 PA. Welcome to the apocalypse.");
    add_response("coalition", "Rocky says: CS patrols have been tighter lately. Stay clean at the checkpoint.");
    add_response("bye", "Rocky says: Watch the roads.");
    add_response("help", "Rocky says: 'buy drink', 50 credits. Say 'rumor' for gossip.");
    set_property("position_str", "polishes a glass behind the bar.");
    add_response("done", "");
    add_response("talisman", "Rocky says: Three salvage items. Prove you're worth it.");
    add_response("prove", "Rocky says: Bring me three salvage items and I'll consider it.");
    add_response("salvage", "Rocky says: Three salvage items. Give them to me here.");
    set_default_response("Rocky wipes the bar and waits.");

    __rumors = ({
        "Word is a caravan out of Chi-Town never made it to Lazlo. "
        "Fifty wagons. Just gone. Coalition says bandits. Nobody believes that.",

        "Saw a Dragon land about two miles east of here last week. "
        "Huge. Didn't attack. Just watched for an hour and flew north. "
        "Whatever it was looking for, I hope it found it.",

        "The CS has a new scanning post going up south of Monument Square. "
        "If you're D-Bee or carrying black-market tech, you'll want to know about it.",

        "A Ley Line Walker came through here saying the nexus point near "
        "the old stadium is getting stronger. Rifts opening more frequently. "
        "That's usually bad news for everyone nearby.",

        "Heard Lazlo is offering resettlement credits to skilled operators. "
        "Mechanics, pilots, medics. They're rebuilding something up there.",

        "There's a black market buyer working out of an alley off Market Street. "
        "Pays good for salvaged pre-Rifts tech. Pays even better if you don't ask questions.",

        "Rumor out of the harbor district: Atlantis is active again. "
        "Splugorth slavers running small raiding parties up the coast. "
        "If you see anything flying the Splugorth banner, don't engage. Just run.",

        "A Cyber-Knight came through here three nights ago. Sat right there. "
        "Wouldn't drink, wouldn't talk. Just watched the checkpoint for two hours and left north. "
        "Where there's a Cyber-Knight there's usually trouble about to happen.",

        "A CS patrol went dark south of the ruins two days ago. "
        "Six soldiers, full kit. High Command is calling it a radio malfunction. "
        "Locals who live out that way are calling it something else."
    });

    __salvage_count = ([]);
}

void init() {
    ::init();
    add_action("cmd_buy", "buy");
    add_action("cmd_order", "order");
}

void catch_tell(string str) {
    string a, b;
    object tp;

    if(sscanf(str, "%s says, \"%s\"", a, b) != 2) return;
    b = lower_case(b);

    if(strsrch(b, "rumor") != -1 || strsrch(b, "news") != -1) {
        tp = this_player();
        call_out("tell_rumor", 1, tp);
        return;
    }

    if(strsrch(b, "done") != -1) {
        tp = this_player();
        call_out("check_dead_signal_done", 1, tp);
        return;
    }

    /* Faction-aware greeting */
    if(strsrch(b, "hello") != -1 || strsrch(b, "hi") != -1) {
        tp = this_player();
        if(!tp) { ::catch_tell(str); return; }
        if((int)FACTION_D->is_cs_ally(tp))
            say_here("Rocky says: Military. Bar's open. What'll it be?");
        else if((int)FACTION_D->query_rep(tp, "coalition") <= -100)
            say_here("Rocky says: Long way from safe territory. Sit down. I don't ask questions.");
        else
            say_here("Rocky says: What're you drinking?");
        return;
    }

    /* Faction-aware coalition response */
    if(strsrch(b, "coalition") != -1) {
        tp = this_player();
        if(!tp) { ::catch_tell(str); return; }
        if((int)FACTION_D->is_cs_ally(tp))
            say_here("Rocky keeps his voice even. 'Patrols have been heavy lately. You'd know the schedule better than me.'");
        else if((int)FACTION_D->query_rep(tp, "coalition") <= -100)
            say_here("Rocky leans in. 'Checkpoint scanner runs a full bio-scan every hour. Plan ahead before you head south.'");
        else
            say_here("Rocky says: CS patrols have been tighter lately. Stay clean at the checkpoint.");
        return;
    }

    ::catch_tell(str);
}

void tell_rumor(object tp) {
    string r;
    int idx;

    idx = random(sizeof(__rumors));
    r = __rumors[idx];
    tell_room(environment(this_object()),
        "Rocky leans on the bar and lowers his voice.\nRocky says: \""+r+"\"\n",
        ({ this_object() }));
    if(strsrch(r, "Cyber-Knight") != -1 && tp && objectp(tp)) {
        call_out("offer_rocky_rumor_quest", 2, tp);
    }
}

void offer_rocky_rumor_quest(object tp) {
    if(!tp || !objectp(tp)) return;
    if((int)QUEST_D->is_quest_complete(tp, "rocky_rumor") ||
       (int)QUEST_D->is_quest_active(tp, "rocky_rumor")) return;
    say_here("That Cyber-Knight? They went north. If you want to know what "
             "they were after, the old highway's where to start. Just a rumor.");
    catch(QUEST_D->start_quest(tp, "rocky_rumor"));
}

void check_dead_signal_done(object tp) {
    object radio;
    if(!tp || !objectp(tp)) return;
    if(!(int)QUEST_D->is_quest_active(tp, "dead_signal")) {
        say_here("Rocky says: Done with what?");
        return;
    }
    if(!(int)QUEST_D->is_objective_complete(tp, "dead_signal", "contacted_nexus")) {
        say_here("Rocky says: You haven't finished yet. Tune to frequency 77 and go to the nexus crossing.");
        return;
    }
    catch(QUEST_D->complete_quest(tp, "dead_signal"));
    if(!present("military radio", tp)) {
        radio = clone_object("/domains/Praxis/equipment/military_radio.c");
        if(radio) radio->move(tp);
    }
    tell_object(tp,
        "Rocky nods slowly. \"Told you it was worth knowing.\" "
        "He slides 500 credits across the bar and hands you a military radio.\n");
    tell_room(environment(this_object()),
        "Rocky nods and hands " + (string)tp->query_cap_name() + " something from under the bar.\n",
        ({ this_object(), tp }));
}

void offer_dead_signal(object tp) {
    if(!tp || !objectp(tp)) return;
    if((int)QUEST_D->is_quest_complete(tp, "dead_signal") ||
       (int)QUEST_D->is_quest_active(tp, "dead_signal")) return;
    say_here("Hey. You look like someone who can keep a secret. I picked up something "
             "strange on frequency 77 last night. Could be nothing. Could be something "
             "worth knowing. Radio's free. Go tune it and see what answers back.");
    catch(QUEST_D->start_quest(tp, "dead_signal"));
}

int receive_object(object ob, object giver) {
    string ob_name;
    string giver_name;
    int count;
    object talisman;

    if(!ob || !giver || !living(giver)) return 0;
    ob_name = lower_case((string)ob->query_name());
    giver_name = (string)giver->query_name();

    if(strsrch(ob_name, "salvage") != -1) {
        if(!__salvage_count) __salvage_count = ([]);
        count = __salvage_count[giver_name] + 1;
        __salvage_count[giver_name] = count;
        destruct(ob);
        if(count < 3) {
            tell_object(giver,
                "Rocky takes the salvage and tucks it away. \"That's " +
                count + " of 3. Keep going.\"\n");
        } else {
            __salvage_count[giver_name] = 0;
            tell_object(giver,
                "Rocky nods approvingly. \"Good work. A deal's a deal.\"\n");
            tell_room(environment(this_object()),
                "Rocky reaches under the bar and places something in " +
                (string)giver->query_cap_name() + "'s hand.\n",
                ({ this_object(), giver }));
            talisman = clone_object("/domains/Praxis/equipment/armor_talisman.c");
            if(talisman) talisman->move(giver);
        }
        return 1;
    }
    tell_object(giver, "Rocky looks at it and shakes his head. \"I don't need that.\"\n");
    ob->move(giver);
    return 1;
}

int cmd_buy(string str) {
    int cur_sdc, max_sdc, heal_amt;

    if(!str || strsrch(lower_case(str), "drink") == -1) {
        write("Buy what? This is a bar. Try 'buy drink'.\n");
        return 1;
    }
    if((int)this_player()->query_money("credits") < DRINK_PRICE) {
        tell_room(environment(this_object()),
            "Rocky says: "+this_player()->query_cap_name()+
            ", you're short. "+DRINK_PRICE+" credits for a drink.\n",
            ({ this_object() }));
        return 1;
    }
    this_player()->add_money("credits", -DRINK_PRICE);
    cur_sdc = (int)this_player()->query_stats("SDC");
    max_sdc = (int)this_player()->query_stats("max_SDC");
    heal_amt = DRINK_HEAL;
    if(cur_sdc + heal_amt > max_sdc) heal_amt = max_sdc - cur_sdc;
    if(heal_amt > 0)
        this_player()->set_stats("SDC", cur_sdc + heal_amt);
    write("Rocky slides a glass across the bar. You drink. "+
        (heal_amt > 0 ? "+"+heal_amt+" SDC. " : "")+
        "Synthetic whiskey. Burns going down, burns coming back.\n");
    tell_room(environment(this_object()),
        "Rocky serves "+this_player()->query_cap_name()+" a drink.\n",
        ({ this_object(), this_player() }));
    call_out("offer_dead_signal", 2, this_player());
    if((int)FACTION_D->query_rep(this_player(), "coalition") <= -100)
        call_out("tip_hostile_player", 4, this_player());
    return 1;
}

void tip_hostile_player(object tp) {
    if(!tp || !objectp(tp)) return;
    say_here("Rocky mutters, not looking up: 'South gate scanner flags non-baseline bio-signs. Every hour. Just so you know.'");
}

int cmd_order(string str) { return cmd_buy(str); }
