// /obj/tattoo_weapons/flaming_arrow.c
// One-use magical fire arrow summoned from an Atlantean tattoo.
// Does not persist - fires immediately when tattoo_d calls activate_tattoo.
// This object is handed to the player and auto-fires on get or force_fire.

#include <std.h>
#include <daemons.h>

inherit OBJECT;

private object __owner;
private int    __fired;

void create() {
    ::create();
    __fired = 0;
    set_name("flaming arrow");
    set_id(({ "arrow", "flaming arrow", "tattoo arrow", "fire arrow" }));
    set_short("a flaming arrow");
    set_long(
        "A single arrow of condensed magical flame, solid enough to hold but\n"
        "burning at its tip with a fierce orange fire.\n"
        "It will vanish the moment it is fired or after a few seconds.");
    set_mass(10);
    set_property("tattoo_weapon", 1);
    set_property("mdc_weapon",    1);
    set_property("fire_damage",   1);
    set_property("no_drop",       1);
    set_value(0);
}

void init_tattoo_owner(object player) {
    __owner = player;
    move(player);
    tell_object(player,
        "A flaming arrow materialises in your hand!  "
        "Type 'fire arrow at <target>' to use it.\n");
    call_out("auto_expire", 15);
}

// Called by _touch.c when a target is passed to a flaming arrow tattoo,
// or by the player directly via 'fire arrow at <target>'.
int fire_at(object target) {
    int damage, cur_mdc, cur_hp;

    if(__fired) {
        if(__owner) tell_object(__owner, "The arrow is spent.\n");
        return 0;
    }
    if(!target || !objectp(target) || !living(target)) {
        if(__owner) tell_object(__owner, "Fire it at whom?\n");
        return 0;
    }
    if(environment(target) != environment(__owner)) {
        if(__owner) tell_object(__owner, "Your target is not here.\n");
        return 0;
    }

    __fired = 1;
    remove_call_out("auto_expire");

    damage = random(6) + 1;

    if(__owner) {
        tell_object(__owner,
            "You loose the flaming arrow at " +
            (string)target->query_cap_name() + "!  " + damage + " MDC!\n");
    }
    tell_object(target,
        (__owner ? (string)__owner->query_cap_name() : "Someone") +
        " fires a magical flaming arrow at you!  " + damage + " MDC!\n");
    if(__owner)
        say((string)__owner->query_cap_name() +
            " looses a flaming arrow from a tattoo!",
            ({ __owner, target }));

    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        cur_mdc = (int)target->query_stats("MDC") - damage;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= -20) catch(target->die());
    } else {
        cur_hp = (int)target->query_hp() - damage;
        if(cur_hp < 0) cur_hp = 0;
        target->set_hp(cur_hp);
        if(cur_hp <= 0) catch(target->die());
    }

    call_out("remove", 0);
    return 1;
}

void auto_expire() {
    if(!__fired && __owner && objectp(__owner))
        tell_object(__owner, "The flaming arrow fizzles out unused.\n");
    call_out("remove", 0);
}

void remove() {
    remove_call_out("auto_expire");
    ::remove();
}

int query_damage() { return random(6) + 1; }
