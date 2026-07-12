/* /cmds/mortal/_trade.c
   Player-to-player trade system.
   Syntax:
     trade <player>           -- initiate a trade
     trade accept             -- accept incoming trade request
     trade decline            -- decline incoming trade request
     trade offer <item>       -- add item to your trade window
     trade offer <n> credits  -- offer credits
     trade remove <item>      -- remove item from your offer
     trade confirm            -- lock in your side
     trade cancel             -- abort the trade */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private void do_cancel(string a_name, string b_name, int notify);

/* Trade sessions: player_name -> mapping */
private mapping __trades;

void create() {
    daemon::create();
    set_no_clean(1);
    __trades = ([]);
}

/* ── helpers ────────────────────────────────────────────────────────────── */

private void show_window(string a_name, string b_name) {
    object a, b;
    mapping a_data, b_data;
    mixed *a_items, *b_items;
    string buf, item_list;
    int i;

    a = find_player(a_name);
    b = find_player(b_name);
    a_data = __trades[a_name];
    b_data = __trades[b_name];
    if(!a_data || !b_data) return;
    a_items = (mixed *)a_data["items"];
    b_items = (mixed *)b_data["items"];
    if(!a_items) a_items = ({});
    if(!b_items) b_items = ({});

    buf = "=== TRADE: " + capitalize(a_name) + " <-> " + capitalize(b_name) + " ===\n";

    /* A's offers */
    buf += capitalize(a_name) + " offers: ";
    item_list = "";
    for(i = 0; i < sizeof(a_items); i++) {
        if(objectp(a_items[i]))
            item_list += (string)a_items[i]->query_short() + "  ";
    }
    if((int)a_data["credits"] > 0)
        item_list += a_data["credits"] + " credits  ";
    buf += (strlen(item_list) > 0) ? item_list : "(nothing)";
    buf += "\n";

    /* B's offers */
    buf += capitalize(b_name) + " offers: ";
    item_list = "";
    for(i = 0; i < sizeof(b_items); i++) {
        if(objectp(b_items[i]))
            item_list += (string)b_items[i]->query_short() + "  ";
    }
    if((int)b_data["credits"] > 0)
        item_list += b_data["credits"] + " credits  ";
    buf += (strlen(item_list) > 0) ? item_list : "(nothing)";
    buf += "\n";

    if((int)a_data["confirmed"]) buf += capitalize(a_name) + " has CONFIRMED.\n";
    if((int)b_data["confirmed"]) buf += capitalize(b_name) + " has CONFIRMED.\n";
    buf += "Commands: trade offer <item>  trade offer <n> credits  "
           "trade confirm  trade cancel\n";

    if(a) tell_object(a, buf);
    if(b) tell_object(b, buf);
}

private void reset_timeout(string a_name, string b_name) {
    int tid;

    tid = (int)__trades[a_name]["timeout"];
    if(tid) remove_call_out(tid);
    tid = call_out("trade_expire", 60, a_name, b_name);
    __trades[a_name]["timeout"] = tid;
    if(__trades[b_name]) __trades[b_name]["timeout"] = tid;
}

void trade_expire(string a_name, string b_name) {
    object a, b;

    a = find_player(a_name);
    b = find_player(b_name);
    if(a) tell_object(a, "Trade with " + capitalize(b_name) +
        " timed out and was cancelled.\n");
    if(b) tell_object(b, "Trade with " + capitalize(a_name) +
        " timed out and was cancelled.\n");
    do_cancel(a_name, b_name, 0);
}

private void do_cancel(string a_name, string b_name, int notify) {
    mapping a_data, b_data;
    mixed *a_items, *b_items;
    object a, b;
    int tid, i;

    a_data = __trades[a_name];
    b_data = __trades[b_name];

    if(a_data) {
        tid = (int)a_data["timeout"];
        if(tid) remove_call_out(tid);
        a_items = (mixed *)a_data["items"];
        if(a_items) {
            for(i = 0; i < sizeof(a_items); i++) {
                if(objectp(a_items[i]))
                    a_items[i]->set_property("trade_offered", 0);
            }
        }
        map_delete(__trades, a_name);
    }
    if(b_data) {
        b_items = (mixed *)b_data["items"];
        if(b_items) {
            for(i = 0; i < sizeof(b_items); i++) {
                if(objectp(b_items[i]))
                    b_items[i]->set_property("trade_offered", 0);
            }
        }
        map_delete(__trades, b_name);
    }
    if(notify) {
        a = find_player(a_name);
        b = find_player(b_name);
        if(a) tell_object(a, "Trade cancelled.\n");
        if(b) tell_object(b, "Trade cancelled.\n");
    }
}

/* ── main command ─────────────────────────────────────────────────────── */

int cmd_trade(string str) {
    string cmd, arg, pname, partner_name;
    object partner, item_ob;
    mapping my_data, partner_data;
    mixed *my_items;
    int credits, my_credits, partner_credits, i, tid;
    object *inv;
    string amount_str;

    pname = lower_case((string)this_player()->query_name());

    if(!str || str == "") {
        /* show current trade if active */
        my_data = __trades[pname];
        if(!my_data) {
            write("You are not in a trade. Use 'trade <player>' to initiate.\n");
            return 1;
        }
        show_window(pname, (string)my_data["partner"]);
        return 1;
    }

    /* parse first word as cmd */
    {
        int sp;
        sp = strsrch(str, " ");
        if(sp == -1) { cmd = lower_case(str); arg = ""; }
        else { cmd = lower_case(str[0..sp-1]); arg = str[sp+1..]; }
    }

    /* ── trade <player> : initiate ─────────────────────────────────── */
    if(cmd != "accept" && cmd != "decline" && cmd != "offer" &&
       cmd != "remove" && cmd != "confirm" && cmd != "cancel") {
        /* treat the whole str as a player name */
        partner_name = lower_case(str);
        if(__trades[pname]) {
            write("You are already in a trade. Type 'trade cancel' first.\n");
            return 1;
        }
        partner = find_player(partner_name);
        if(!partner || !objectp(partner) || !interactive(partner)) {
            write("That player is not here.\n");
            return 1;
        }
        if(environment(partner) != environment(this_player())) {
            write("That player is not in the room.\n");
            return 1;
        }
        if(__trades[partner_name]) {
            write(capitalize(partner_name) + " is already in a trade.\n");
            return 1;
        }
        tid = call_out("trade_expire", 60, pname, partner_name);
        __trades[pname] = ([
            "partner":   partner_name,
            "state":     "pending",
            "items":     ({}),
            "credits":   0,
            "confirmed": 0,
            "timeout":   tid
        ]);
        __trades[partner_name] = ([
            "partner":   pname,
            "state":     "incoming",
            "items":     ({}),
            "credits":   0,
            "confirmed": 0,
            "timeout":   tid
        ]);
        tell_object(partner,
            capitalize(pname) + " wants to trade with you.\n"
            "Type 'trade accept' or 'trade decline'.\n");
        write("Trade request sent to " + capitalize(partner_name) + ".\n");
        return 1;
    }

    /* all other sub-commands require an active trade */
    my_data = __trades[pname];
    if(!my_data) {
        write("You are not in a trade.\n");
        return 1;
    }
    partner_name = (string)my_data["partner"];
    partner_data = __trades[partner_name];

    /* ── trade accept ─────────────────────────────────────────────── */
    if(cmd == "accept") {
        if((string)my_data["state"] != "incoming") {
            write("You have no pending trade request to accept.\n");
            return 1;
        }
        my_data["state"]           = "open";
        __trades[partner_name]["state"] = "open";
        reset_timeout(pname, partner_name);
        write("Trade accepted.\n");
        tell_object(find_player(partner_name),
            capitalize(pname) + " accepted your trade request.\n");
        show_window(partner_name, pname);
        return 1;
    }

    /* ── trade decline ────────────────────────────────────────────── */
    if(cmd == "decline") {
        if((string)my_data["state"] != "incoming") {
            write("You have no pending trade request to decline.\n");
            return 1;
        }
        partner = find_player(partner_name);
        if(partner)
            tell_object(partner, capitalize(pname) + " declined your trade request.\n");
        write("Trade declined.\n");
        do_cancel(pname, partner_name, 0);
        return 1;
    }

    /* ── trade cancel ─────────────────────────────────────────────── */
    if(cmd == "cancel") {
        do_cancel(pname, partner_name, 1);
        return 1;
    }

    /* remaining sub-commands require state == "open" */
    if((string)my_data["state"] != "open") {
        write("The trade is not yet open. Wait for the other player to accept.\n");
        return 1;
    }

    /* ── trade offer <item> or trade offer <n> credits ────────────── */
    if(cmd == "offer") {
        if(!arg || arg == "") {
            write("Offer what? Syntax: trade offer <item>  or  trade offer <n> credits\n");
            return 1;
        }
        /* check for credits offer */
        {
            int csp;
            csp = strsrch(lower_case(arg), " credits");
            if(csp != -1) {
                amount_str = arg[0..csp-1];
                credits    = to_int(amount_str);
                if(credits <= 0) {
                    write("Specify a positive credit amount.\n");
                    return 1;
                }
                if((int)this_player()->query_money("credits") < credits) {
                    write("You do not have that many credits.\n");
                    return 1;
                }
                my_data["credits"] = (int)my_data["credits"] + credits;
                reset_timeout(pname, partner_name);
                show_window(pname, partner_name);
                return 1;
            }
        }
        /* item offer */
        item_ob = present(arg, this_player());
        if(!item_ob) {
            write("You do not have '" + arg + "'.\n");
            return 1;
        }
        if((int)item_ob->query_property("trade_offered")) {
            write("That item is already in your trade offer.\n");
            return 1;
        }
        if((int)item_ob->query_auto_load()) {
            write("You cannot trade that item.\n");
            return 1;
        }
        my_items = (mixed *)my_data["items"];
        if(!my_items) my_items = ({});
        my_items += ({ item_ob });
        my_data["items"] = my_items;
        item_ob->set_property("trade_offered", 1);
        my_data["confirmed"] = 0;
        if(partner_data) partner_data["confirmed"] = 0;
        reset_timeout(pname, partner_name);
        show_window(pname, partner_name);
        return 1;
    }

    /* ── trade remove <item> ──────────────────────────────────────── */
    if(cmd == "remove") {
        my_items = (mixed *)my_data["items"];
        if(!my_items || !sizeof(my_items)) {
            write("You have no items in your trade offer.\n");
            return 1;
        }
        item_ob = present(arg, this_player());
        if(!item_ob) {
            write("You do not have that item.\n");
            return 1;
        }
        {
            mixed *new_items;
            new_items = ({});
            for(i = 0; i < sizeof(my_items); i++) {
                if(objectp(my_items[i]) && my_items[i] != item_ob)
                    new_items += ({ my_items[i] });
            }
            item_ob->set_property("trade_offered", 0);
            my_data["items"]     = new_items;
            my_data["confirmed"] = 0;
            if(partner_data) partner_data["confirmed"] = 0;
        }
        reset_timeout(pname, partner_name);
        show_window(pname, partner_name);
        return 1;
    }

    /* ── trade confirm ────────────────────────────────────────────── */
    if(cmd == "confirm") {
        my_data["confirmed"] = 1;
        reset_timeout(pname, partner_name);
        if(!(int)partner_data["confirmed"]) {
            write("Waiting for " + capitalize(partner_name) + " to confirm.\n");
            tell_object(find_player(partner_name),
                capitalize(pname) + " has confirmed. Type 'trade confirm' to complete.\n");
            return 1;
        }
        /* Both confirmed -- execute the trade */
        {
            mixed *a_items, *b_items;
            object pob, ppob;
            int a_credits, b_credits;

            pob    = find_player(pname);
            ppob   = find_player(partner_name);
            a_items   = (mixed *)my_data["items"];
            b_items   = (mixed *)partner_data["items"];
            a_credits = (int)my_data["credits"];
            b_credits = (int)partner_data["credits"];
            if(!a_items) a_items = ({});
            if(!b_items) b_items = ({});

            /* verify credit availability */
            if(pob && (int)pob->query_money("credits") < a_credits) {
                if(pob)  tell_object(pob,  "You no longer have enough credits to complete the trade.\n");
                if(ppob) tell_object(ppob, "Trade cancelled: partner lacks credits.\n");
                do_cancel(pname, partner_name, 0);
                return 1;
            }
            if(ppob && (int)ppob->query_money("credits") < b_credits) {
                if(pob)  tell_object(pob,  "Trade cancelled: partner lacks credits.\n");
                if(ppob) tell_object(ppob, "You no longer have enough credits to complete the trade.\n");
                do_cancel(pname, partner_name, 0);
                return 1;
            }

            /* move items */
            for(i = 0; i < sizeof(a_items); i++) {
                if(objectp(a_items[i])) {
                    a_items[i]->set_property("trade_offered", 0);
                    if(ppob) a_items[i]->move(ppob);
                }
            }
            for(i = 0; i < sizeof(b_items); i++) {
                if(objectp(b_items[i])) {
                    b_items[i]->set_property("trade_offered", 0);
                    if(pob) b_items[i]->move(pob);
                }
            }

            /* transfer credits */
            if(a_credits > 0 && pob && ppob) {
                pob->add_money("credits",  -a_credits);
                ppob->add_money("credits",  a_credits);
            }
            if(b_credits > 0 && pob && ppob) {
                ppob->add_money("credits", -b_credits);
                pob->add_money("credits",   b_credits);
            }

            if(pob)  tell_object(pob,  "Trade complete.\n");
            if(ppob) tell_object(ppob, "Trade complete.\n");
            do_cancel(pname, partner_name, 0);
        }
        return 1;
    }

    return 1;
}

void help() {
    write(
        "Syntax: trade <player>           -- initiate a trade\n"
        "        trade accept             -- accept incoming request\n"
        "        trade decline            -- decline incoming request\n"
        "        trade offer <item>       -- add item to your offer\n"
        "        trade offer <n> credits  -- offer credits\n"
        "        trade remove <item>      -- remove item from offer\n"
        "        trade confirm            -- confirm your side\n"
        "        trade cancel             -- cancel the trade\n\n"
        "Both players must type 'trade confirm' to complete the exchange.\n"
        "Trades auto-cancel after 60 seconds of inactivity.\n"
    );
}
