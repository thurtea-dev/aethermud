/* /domains/Praxis/equipment/magic_items/healing_charm.c
   Atlantean healing charm. Three charges of 2d6 healing; recharge with PPE.
   Self-contained: heals rifts_hp (or MDC for MDC races) via set_stats. */

#include <std.h>
#include <daemons.h>

inherit OBJECT;

#define MAX_CHARGES   3
#define RECHARGE_PPE 15

private int __charges;

void create() {
    ::create();
    set_name("healing charm");
    set_id( ({ "healing charm", "charm", "jade charm" }) );
    set_short("a jade healing charm");
    set_long(
        "A small jade carving of a coiled serpent on a leather thong. The\n"
        "stone is always warm to the touch. Rub it against a wound and the\n"
        "wound closes. It holds three charges of healing magic.\n"
        "Type 'rub charm' to heal (2d6). Type 'activate charm' to recharge\n"
        "it with " + RECHARGE_PPE + " PPE per charge.");
    set_mass(20);
    set_value(12000);
    set_property("magic_item", 1);
    __charges = MAX_CHARGES;
}

int query_charges() { return __charges; }

void init() {
    ::init();
    add_action("cmd_rub", "rub");
    add_action("cmd_recharge", "activate");
}

int cmd_rub(string str) {
    object who;
    int heal, cur, maxv;

    if(!str || !id(str)) return 0;
    who = this_player();
    if(environment(this_object()) != who) {
        notify_fail("You must be holding the charm to use it.\n");
        return 0;
    }
    if(__charges <= 0) {
        write("The charm is cold and inert. It needs to be recharged.");
        return 1;
    }
    __charges--;
    heal = random(6) + random(6) + 2;
    if((int)RIFTS_D->is_mdc_race((string)who->query_race())) {
        maxv = (int)who->query_stats("max_MDC");
        cur  = (int)who->query_stats("MDC") + heal;
        if(cur > maxv) cur = maxv;
        who->set_stats("MDC", cur);
    } else {
        maxv = (int)who->query_stats("max_rifts_hp");
        cur  = (int)who->query_stats("rifts_hp") + heal;
        if(cur > maxv) cur = maxv;
        who->set_stats("rifts_hp", cur);
    }
    write("The jade serpent glows warm and your wounds close. +" + heal +
        " restored. (" + __charges + " charge(s) left)");
    say((string)who->query_cap_name() + " rubs a jade charm and " +
        "looks healthier.");
    return 1;
}

int cmd_recharge(string str) {
    object who;
    int ppe;

    if(!str || !id(str)) return 0;
    who = this_player();
    if(environment(this_object()) != who) {
        notify_fail("You must be holding the charm to recharge it.\n");
        return 0;
    }
    if(__charges >= MAX_CHARGES) {
        write("The charm is fully charged.");
        return 1;
    }
    ppe = (int)who->query_stats("PPE");
    if(ppe < RECHARGE_PPE) {
        write("You need " + RECHARGE_PPE + " PPE to restore a charge. "
            "You have " + ppe + ".");
        return 1;
    }
    who->set_stats("PPE", ppe - RECHARGE_PPE);
    __charges++;
    write("You channel " + RECHARGE_PPE + " PPE into the charm. It pulses "
        "with green light. (" + __charges + "/" + MAX_CHARGES + " charges)");
    return 1;
}
