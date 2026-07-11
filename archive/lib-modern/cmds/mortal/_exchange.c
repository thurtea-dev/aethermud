// /cmds/mortal/_exchange.c
// Smuggler OCC: convert credits and black market credits at a level-based rate.
// Can perform exchanges as a paid service for others in the same room.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

#define SERVICE_FEE_PCT 3

private void show_menu(object pl) {
    int rate;

    rate = (int)RIFTS_D->query_smuggler_exchange_rate(pl);
    write("=== Smuggler Credit Exchange ===\n");
    write("Your rate: " + rate + "% (rises with level and Black Market skill)\n");
    write("Credits:       " + (int)pl->query_money("credits") + "\n");
    write("Black credits: " + (int)pl->query_money("black credits") + "\n");
    write("\nPersonal:\n");
    write("  exchange <amount> credits to black\n");
    write("  exchange <amount> black to credits\n");
    write("\nPaid service (same room, " + SERVICE_FEE_PCT + "% fee to you):\n");
    write("  exchange for <player> <amount> credits to black\n");
    write("  exchange for <player> <amount> black to credits\n");
    write("\n  exchange rate\n");
}

private int convert_credits_to_black(object customer, object smuggler,
    int amount, int service) {
    int rate;
    int payout;
    int fee;
    object env;

    rate = (int)RIFTS_D->query_smuggler_exchange_rate(smuggler);
    if(rate < 1) return 0;
    fee = 0;
    if(service) {
        fee = amount * SERVICE_FEE_PCT / 100;
        if(fee < 1) fee = 1;
    }
    if((int)customer->query_money("credits") < amount + fee) {
        write("Not enough credits" +
            (service ? " (includes " + fee + " credit service fee).\n" : ".\n"));
        return 1;
    }
    payout = amount * rate / 100;
    if(payout < 1) payout = 1;
    customer->add_money("credits", -(amount + fee));
    customer->add_money("black credits", payout);
    if(service && fee > 0)
        smuggler->add_money("credits", fee);
    env = environment(customer);
    if(env) {
        tell_room(env,
            capitalize((string)smuggler->query_display_name(customer)) +
            " runs a quiet credit exchange for " +
            capitalize((string)customer->query_display_name(customer)) + ".\n",
            ({ customer, smuggler }));
    }
    tell_object(customer,
        "You exchange " + amount + " credits for " + payout +
        " black credits at " + rate + "%.\n");
    if(service && fee > 0)
        tell_object(smuggler,
            "You earn " + fee + " credits for the exchange service.\n");
    else
        tell_object(smuggler,
            "You convert " + amount + " credits to " + payout +
            " black credits at " + rate + "%.\n");
    return 1;
}

private int convert_black_to_credits(object customer, object smuggler,
    int amount, int service) {
    int rate;
    int payout;
    int fee;
    object env;

    rate = (int)RIFTS_D->query_smuggler_exchange_rate(smuggler);
    if(rate < 1) return 0;
    rate -= 3;
    if(rate < 75) rate = 75;
    fee = 0;
    if(service) {
        fee = amount * SERVICE_FEE_PCT / 100;
        if(fee < 1) fee = 1;
    }
    if((int)customer->query_money("black credits") < amount) {
        write("Not enough black credits.\n");
        return 1;
    }
    if(service && (int)customer->query_money("credits") < fee) {
        write("Not enough credits for the " + fee + " credit service fee.\n");
        return 1;
    }
    payout = amount * rate / 100;
    if(payout < 1) payout = 1;
    customer->add_money("black credits", -amount);
    customer->add_money("credits", payout);
    if(service && fee > 0)
        customer->add_money("credits", -fee);
    if(service && fee > 0)
        smuggler->add_money("credits", fee);
    env = environment(customer);
    if(env) {
        tell_room(env,
            capitalize((string)smuggler->query_display_name(customer)) +
            " launders black credits for " +
            capitalize((string)customer->query_display_name(customer)) + ".\n",
            ({ customer, smuggler }));
    }
    tell_object(customer,
        "You exchange " + amount + " black credits for " + payout +
        " credits at " + rate + "%.\n");
    if(service && fee > 0)
        tell_object(smuggler,
            "You earn " + fee + " credits for the exchange service.\n");
    else
        tell_object(smuggler,
            "You convert " + amount + " black credits to " + payout +
            " credits at " + rate + "%.\n");
    return 1;
}

int cmd_exchange(string str) {
    object tp;
    object target;
    object env;
    string player_name;
    string rest;
    int amount;
    int service;

    tp = this_player();
    if(!tp) return 0;

    if(!(int)RIFTS_D->is_smuggler_occ(tp)) {
        notify_fail("Only a Smuggler can run credit exchange.\n");
        return 0;
    }

    if(!str || !strlen(str)) {
        show_menu(tp);
        return 1;
    }

    str = lower_case(str);
    if(str == "rate" || str == "menu") {
        show_menu(tp);
        return 1;
    }

    service = 0;
    target = tp;
    rest = str;

    if(strsrch(str, "for ") == 0) {
        rest = str[4..];
        if(sscanf(rest, "%s %s", player_name, rest) != 2) {
            notify_fail("Syntax: exchange for <player> <amount> credits to black\n");
            return 0;
        }
        env = environment(tp);
        target = present(player_name, env);
        if(!target || !living(target)) {
            notify_fail("You don't see " + player_name + " here.\n");
            return 0;
        }
        if(target == tp) {
            notify_fail("Use the personal exchange syntax without 'for'.\n");
            return 0;
        }
        service = 1;
    }

    if(sscanf(rest, "%d credits to black", amount) == 1)
        return convert_credits_to_black(target, tp, amount, service);
    if(sscanf(rest, "%d black to credits", amount) == 1)
        return convert_black_to_credits(target, tp, amount, service);

    notify_fail("Syntax: exchange <amount> credits to black\n"
        "        exchange for <player> <amount> credits to black\n"
        "        exchange rate\n");
    return 0;
}

void help() {
    write("Syntax: exchange [rate | menu | <amount> credits to black |\n"
        "        <amount> black to credits | for <player> ...]\n\n"
        "Smuggler OCC only. Converts between universal credits and black\n"
        "market credits. Rate improves with level and Black Market skill.\n"
        "Use 'for <player>' to exchange for someone else in the room (3% fee).\n");
}
