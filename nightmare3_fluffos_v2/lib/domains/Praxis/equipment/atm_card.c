/* /domains/Praxis/equipment/atm_card.c
   ATM card issued by the Praxis Credit Exchange.
   Required to deposit, withdraw, or check balance at the bank terminal.
   Obtained by showing an ID card to the bank teller. */

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("ATM card");
    set_id( ({ "atm card", "atm", "bank card", "card" }) );
    set_short("an ATM card");
    set_long(
        "A slim polymer card issued by the Praxis Credit Exchange. The\n"
        "embedded chip links to your account. Required for deposits,\n"
        "withdrawals, and balance inquiries at the exchange terminal.\n");
    set_mass(10);
    set_value(0);
    set_property("atm_card", 1);
    set_property("no_drop", 0);
}
