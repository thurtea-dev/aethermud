/* /d/Praxis/equipment/splynn/psi_sword_hilt.c
   Splynn black-market psi-sword hilt. Inert crystalline hilt until a
   psychic wielder channels ISP into it: activation costs 10 ISP and
   projects a 2d6 MD blade of psionic force for 5 minutes per level.
   Unlike the Cyber-Knight psisword command (unarmed, psi_sword_active
   property), this is a wielded weapon and uses the standard mdc_weapon
   damage path in /daemon/rifts_combat.c. */

#include <std.h>
#include <move.h>

inherit "/std/weapon";

#define ACTIVATE_COST 10

private int __BladeActive;

private void set_inert_state() {
    __BladeActive = 0;
    set_short("a psi-sword hilt");
    set_wc(2);
    set_property("mdc_weapon", 0);
    set_property("sdc_weapon", 1);
    set_property("damage_num", 1);
    set_property("damage_sides", 4);
    set_property("damage_bonus", 0);
    set_property("damage_string", "1d4 SDC (inert)");
    set_long(
        "A crystalline hilt about thirty centimeters long, faceted and\n"
        "faintly luminescent. No blade extends from either end. Not yet.\n"
        "The hilt resonates with psychic energy. A person with sufficient\n"
        "ISP can activate it, projecting a blade of pure psionic force.\n"
        "Without ISP it is a very expensive stick.\n\n"
        "To ignite the blade: wield the hilt, then 'activate hilt'\n"
        "(costs " + ACTIVATE_COST + " ISP). 'deactivate hilt' extinguishes it.");
}

private void set_active_state() {
    __BladeActive = 1;
    set_short("a psi-sword (blade active)");
    set_wc(10);
    set_property("mdc_weapon", 1);
    set_property("sdc_weapon", 0);
    set_property("damage_num", 2);
    set_property("damage_sides", 6);
    set_property("damage_bonus", 0);
    set_property("damage_string", "2d6 MD (psi-blade)");
    set_long(
        "A blade of coherent psionic force hums from the crystalline hilt,\n"
        "shimmering like heat-haze given an edge. It cuts mega-damage\n"
        "material as easily as air. The blade will fade when its wielder's\n"
        "focus lapses, or on command ('deactivate hilt').");
}

void create() {
    ::create();
    set_name("psi-sword hilt");
    set_id( ({ "psi-sword hilt", "psi sword hilt", "hilt", "psi hilt",
        "psi sword", "psi-sword" }) );
    set_mass(300);
    set_value(5000);
    set_ac(0);
    set_type("blade");
    set_property("psi_sword", 1);
    set_property("requires_isp", 1);
    set_inert_state();
}

void init() {
    ::init();
    add_action("cmd_activate", "activate");
    add_action("cmd_deactivate", "deactivate");
    add_action("cmd_charge_hint", "charge");
}

int query_blade_active() { return __BladeActive; }

void blade_fade() {
    object holder;

    if(!__BladeActive) return;
    set_inert_state();
    holder = environment(this_object());
    if(objectp(holder) && living(holder))
        tell_object(holder,
            "The psi-blade flickers and fades back into the hilt.\n");
}

/* Players coming from the flame hilt try 'charge hilt with ppe'. Give a
   hint instead of "What?", but return 0 so an actual PPE-charged item in
   inventory (flame_hilt.c) still gets a chance to handle the verb. */
int cmd_charge_hint(string str) {
    notify_fail("The crystalline hilt does not respond to PPE. It draws on "
        "ISP: wield it and type 'activate hilt'.\n");
    return 0;
}

int cmd_activate(string str) {
    int isp;
    int level;
    int dur;
    object who;

    if(!str || !id(str)) {
        notify_fail("Activate what?\n");
        return 0;
    }
    who = this_player();
    if(environment(this_object()) != who) {
        notify_fail("You must be holding the hilt to activate it.\n");
        return 0;
    }
    if(__BladeActive) {
        write("The psi-blade is already active.");
        return 1;
    }
    if(!query_wielded()) {
        write("You must wield the hilt before channeling ISP into it.");
        return 1;
    }
    isp = (int)who->query_stats("ISP");
    if(isp < ACTIVATE_COST) {
        write("You strain to focus, but lack the ISP to ignite the blade. "
            "(Need " + ACTIVATE_COST + ", have " + isp + ".)");
        return 1;
    }
    who->set_stats("ISP", isp - ACTIVATE_COST);
    set_active_state();

    level = (int)who->query_level();
    if(level < 1) level = 1;
    dur = level * 300;
    call_out("blade_fade", dur);

    write("You pour your will into the hilt. A blade of psionic force "
        "snaps into being with a crystalline hum.\n"
        "Damage: 2d6 MD. Duration: " + (dur / 60) + " minutes.");
    say((string)who->query_cap_name() +
        "'s crystal hilt ignites into a shimmering blade of force.");
    return 1;
}

int cmd_deactivate(string str) {
    if(!str || !id(str)) {
        notify_fail("Deactivate what?\n");
        return 0;
    }
    if(environment(this_object()) != this_player()) {
        notify_fail("You must be holding the hilt to deactivate it.\n");
        return 0;
    }
    if(!__BladeActive) {
        write("The blade is not active.");
        return 1;
    }
    while(remove_call_out("blade_fade") != -1);
    set_inert_state();
    write("You release your focus. The psi-blade sinks back into the hilt.");
    say((string)this_player()->query_cap_name() +
        "'s blade of force dissolves into nothing.");
    return 1;
}

/* The blade cannot exist apart from a living holder: dropping or giving
   away the hilt breaks the psychic link and extinguishes the blade. */
int move(mixed dest) {
    int ret;

    ret = ::move(dest);
    if(ret == MOVE_OK && __BladeActive &&
       (!environment(this_object()) || !living(environment(this_object())))) {
        while(remove_call_out("blade_fade") != -1);
        set_inert_state();
    }
    return ret;
}
