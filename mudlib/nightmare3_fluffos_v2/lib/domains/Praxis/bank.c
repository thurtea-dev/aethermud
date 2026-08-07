/* /domains/Praxis/bank.c
   Praxis Credit Exchange -- Rifts-themed credit bank.
   Players show their ID card to the teller to receive an ATM card and
   open an account. ATM card is required for deposit, withdraw, balance.
   Replaces the original NM3 fantasy bank. */

#include <std.h>
#include <daemons.h>
#include <bank.h>

#define BANK_ID "praxis"

inherit ROOM;

private int has_atm_card();
private int has_id_card();

void init() {
    ::init();
    add_action("do_show",    "show");
    add_action("do_read",    "read");
    add_action("deposit",    "deposit");
    add_action("withdraw",   "withdraw");
    add_action("balance",    "balance");
    add_action("do_close",   "close");
}

void create() {
    ::create();
    set_property("light",     3);
    set_property("indoors",   1);
    set_property("no magic",  1);
    set_property("no attack", 1);
    set_short("Praxis Credit Exchange");
    set_long(
        "A reinforced concrete room serving as Praxis's financial hub.\n"
        "A security counter runs along the north wall, behind which a\n"
        "teller manages accounts from a hardened terminal. An armored\n"
        "credit kiosk is mounted beside the counter for self-service\n"
        "transactions. A laminated sign lists available services.\n"
        "The exit is back south.\n");
    set_items(([
        "terminal" :
            "An armored credit terminal bolted to the wall. Slot your ATM "
            "card and use deposit, withdraw, or balance.",
        "kiosk" :
            "An armored credit terminal bolted to the wall. Slot your ATM "
            "card and use deposit, withdraw, or balance.",
        "teller" :
            "A professional teller in a security vest. She verifies ID "
            "cards and issues ATM cards for new accounts.",
        "counter" :
            "A reinforced counter with a security screen between you and "
            "the teller.",
        "sign" :
            "A laminated sign listing exchange services. Type 'read sign' "
            "for the full command list.",
        "exit" :
            "South, back to the alley." ]));
    set_exits(([ "south" : "/domains/Praxis/alley1" ]));
}

void reset() {
    object guard;
    object gun;
    object *inv;
    int i;

    ::reset();

    inv = all_inventory(this_object());
    guard = 0;
    for(i = 0; i < sizeof(inv); i++) {
        if(living(inv[i]) && (int)inv[i]->id("security officer")) {
            guard = inv[i];
            break;
        }
    }

    if(!guard) {
        guard = new(MONSTER);
        guard->set_name("security officer");
        guard->set_id(({ "security officer", "officer", "guard", "teller" }));
        guard->set_race("human");
        guard->set_gender("female");
        guard->set_short("Security officer");
        guard->set_long(
            "A uniformed security officer watching the exchange floor. She\n"
            "carries a CP-30 laser pistol and the practiced calm of someone\n"
            "who has broken up more than a few robbery attempts.\n");
        guard->set_level(10);
        guard->set_body_type("human");
        guard->set_hp(200 + random(50));
        guard->set_class("fighter");
        guard->set_skills("defense", 60);
        guard->set_skills("attack", 65);
        guard->set_emotes(15, ({
            "The security officer scans the room.",
            "The security officer adjusts her holster.",
            "The security officer nods at you professionally." }), 0);
        guard->set_wielding_limbs(({ "right hand" }));
        guard->move(this_object());

        gun = new(WEAPON);
        gun->set_name("CP-30 laser pistol");
        gun->set_id(({ "cp-30", "laser pistol", "pistol", "gun" }));
        gun->set_short("CP-30 laser pistol");
        gun->set_long("A Coalition-surplus CP-30 laser pistol, well maintained.\n");
        gun->set_wc(10);
        gun->set_type("energy pistol");
        gun->set_mass(400);
        gun->set_value(800);
        gun->set_property("mdc_weapon", 1);
        gun->set_property("damage_string", "2d6 MD");
        gun->move(guard);
        guard->force_me("wield cp-30 laser pistol in right hand");
    }
}

private int has_atm_card() {
    object *inv;
    int i;

    inv = all_inventory(this_player());
    for(i = 0; i < sizeof(inv); i++) {
        if((int)inv[i]->query_property("atm_card"))
            return 1;
    }
    return 0;
}

private int has_id_card() {
    object *inv;
    int i;

    inv = all_inventory(this_player());
    for(i = 0; i < sizeof(inv); i++) {
        if((int)inv[i]->query_property("id_card"))
            return 1;
    }
    return 0;
}

int do_show(string str) {
    object atm;
    int x;

    if(!str || str != "id") return 0;

    if(!has_id_card()) {
        write("The teller says: I need to see a valid ID card first.\n");
        return 1;
    }
    if(has_atm_card()) {
        write("The teller says: You already have an ATM card on file.\n");
        return 1;
    }

    x = (int)BANK_D->open_account(
        (string)this_player()->query_name(), BANK_ID
    );
    /* ALREADY_EXISTS is acceptable -- account may exist without a card */

    atm = clone_object("/domains/Praxis/equipment/atm_card");
    if(atm) {
        atm->move(this_player());
        write(
            "The teller scans your ID card and issues you an ATM card.\n"
            "Your account is now open. Use 'balance', 'deposit <amount>',\n"
            "or 'withdraw <amount>' at the terminal.\n");
        say(
            (string)this_player()->query_cap_name() +
            " receives an ATM card from the teller.");
    }
    return 1;
}

private mixed parse_amount_currency(string str) {
    string s;
    string word;
    int amount;

    /* sscanf counts %d as matched even when a trailing literal fails,
       so "%d black" matches plain "50" too; capture the trailing word
       with %s and compare it explicitly instead. */
    if(!str || !strlen(str)) return 0;
    s = lower_case(str);
    if(sscanf(s, "%d %s", amount, word) == 2) {
        if(amount < 1) return 0;
        if(word == "black" || word == "black credits" ||
           word == "black credit")
            return ({ amount, "black credits" });
        if(word == "credits" || word == "credit")
            return ({ amount, "credits" });
        return 0;
    }
    if(sscanf(s, "black %d", amount) == 1) {
        if(amount < 1) return 0;
        return ({ amount, "black credits" });
    }
    if(sscanf(s, "credits %d", amount) == 1) {
        if(amount < 1) return 0;
        return ({ amount, "credits" });
    }
    amount = to_int(s);
    if(amount < 1) return 0;
    return ({ amount, "credits" });
}

int balance(string str) {
    mapping money;
    int bal;
    int black;

    if(!has_atm_card()) {
        write(
            "The terminal says: ATM card required.\n"
            "Show your ID card to the teller to obtain one.\n");
        return 1;
    }
    money = (mapping)BANK_D->query_balance(
        (string)this_player()->query_name(), BANK_ID
    );
    if(!money || money == ([])) {
        write(
            "The terminal says: No account found.\n"
            "Show your ID card to the teller to open one.\n");
        return 1;
    }
    bal = money["credits"];
    if(!bal) bal = 0;
    black = money["black credits"];
    if(!black) black = 0;
    write("Account balance: " + bal + " credits, " + black + " black credits.\n");
    say((string)this_player()->query_cap_name() + " checks account balance.");
    return 1;
}

int deposit(string str) {
    int amount;
    int x;
    string currency;
    mixed parsed;

    if(!has_atm_card()) {
        write("The terminal says: ATM card required.\n");
        return 1;
    }
    if(!str) {
        notify_fail("Syntax: deposit <amount> [black|credits]\n");
        return 0;
    }
    parsed = parse_amount_currency(str);
    if(!parsed || sizeof(parsed) < 2) {
        notify_fail("Syntax: deposit <amount> [black|credits]\n");
        return 0;
    }
    amount = parsed[0];
    currency = parsed[1];
    if((int)this_player()->query_money(currency) < amount) {
        notify_fail("The terminal says: Insufficient " + currency + " on hand.\n");
        return 0;
    }
    x = (int)BANK_D->deposit(
        (string)this_player()->query_name(), BANK_ID, amount, currency
    );
    if(x != TRANSACTION_OK) {
        switch(x) {
        case NO_ACCOUNT:
            notify_fail("The terminal says: No account found.\n");
            break;
        default:
            notify_fail("The terminal says: Transaction failed.\n");
        }
        return 0;
    }
    this_player()->add_money(currency, -amount);
    write("Deposited " + amount + " " + currency + ".\n");
    say((string)this_player()->query_cap_name() + " makes a deposit.");
    return 1;
}

int withdraw(string str) {
    int amount;
    int x;
    string currency;
    mixed parsed;

    if(!has_atm_card()) {
        write("The terminal says: ATM card required.\n");
        return 1;
    }
    if(!str) {
        notify_fail("Syntax: withdraw <amount> [black|credits]\n");
        return 0;
    }
    parsed = parse_amount_currency(str);
    if(!parsed || sizeof(parsed) < 2) {
        notify_fail("Syntax: withdraw <amount> [black|credits]\n");
        return 0;
    }
    amount = parsed[0];
    currency = parsed[1];
    x = (int)BANK_D->withdraw(
        (string)this_player()->query_name(), BANK_ID, amount, currency
    );
    if(x != TRANSACTION_OK) {
        switch(x) {
        case NO_ACCOUNT:
            notify_fail("The terminal says: No account found.\n");
            break;
        case INSF:
            notify_fail("The terminal says: Insufficient funds.\n");
            break;
        default:
            notify_fail("The terminal says: Transaction failed.\n");
        }
        return 0;
    }
    this_player()->add_money(currency, amount);
    write("Withdrew " + amount + " " + currency + ".\n");
    say((string)this_player()->query_cap_name() + " makes a withdrawal.");
    return 1;
}

int do_close(string str) {
    mapping money;
    int bal;
    int black;

    if(!str || str != "account") return 0;
    if(!has_atm_card()) {
        write("The teller says: ATM card required to close an account.\n");
        return 1;
    }
    money = (mapping)BANK_D->query_balance(
        (string)this_player()->query_name(), BANK_ID
    );
    if(!money || money == ([])) {
        write("The teller says: No account on file.\n");
        return 1;
    }
    bal = money["credits"];
    if(!bal) bal = 0;
    black = money["black credits"];
    if(!black) black = 0;
    if(bal > 0)
        this_player()->add_money("credits", bal);
    if(black > 0)
        this_player()->add_money("black credits", black);
    BANK_D->close_account((string)this_player()->query_name(), BANK_ID);
    if(bal > 0 || black > 0)
        write(
            "The teller closes your account and returns " +
            bal + " credits and " + black + " black credits.\n");
    else
        write("The teller closes your empty account.\n");
    say((string)this_player()->query_cap_name() + " closes their account.");
    return 1;
}

int do_read(string str) {
    if(!str || str != "sign")
        return notify_fail("Read what?\n");
    message("info",
        "Praxis Credit Exchange - Services\n\n"
        "  show id          Show your ID card to the teller to receive an\n"
        "                   ATM card and open an account.\n\n"
        "  balance          Check credits and black credits (ATM card required).\n\n"
        "  deposit <n>      Deposit n credits (default).\n"
        "  deposit <n> black  Deposit n black credits.\n\n"
        "  withdraw <n>     Withdraw n credits (default).\n"
        "  withdraw <n> black  Withdraw n black credits.\n"
        "                   (ATM card required)\n\n"
        "  close account    Return all credits and close your account.\n"
        "                   (ATM card required)\n",
        this_player()
    );
    return 1;
}
