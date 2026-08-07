// /obj/tattoo_weapons/flaming_sword.c
// Summoned flaming sword from an Atlantean tattoo.
// Auto-destroys after 5 minutes (300 seconds).

#include <std.h>

inherit "/std/weapon";

private object __owner;

void create() {
    ::create();
    set_name("flaming sword");
    set_id(({ "sword", "flaming sword", "tattoo sword" }));
    set_short("a flaming sword");
    set_long(
        "A sword of pure magical flame conjured from a tattoo on the wielder's arm.\n"
        "The blade burns steady and hot, casting orange light on everything nearby.\n"
        "It will vanish when the tattoo's energy is spent.");
    set_mass(150);
    set_wc(14);
    set_type("blade");
    set_property("tattoo_weapon",    1);
    set_property("summoned_weapon",  1);
    set_property("mdc_weapon",       1);
    set_property("fire_damage",      1);
    set_property("no_drop",          1);
    set_property("damage_num",       3);
    set_property("damage_sides",     6);
    set_property("damage_bonus",     0);
    set_property("damage_string",    "3d6 MD fire");
}

void init_tattoo_owner(object player) {
    __owner = player;
    call_out("tattoo_timeout", 300);
    move(player);
    call_out("do_wield", 0, player);
}

private void do_wield(object player) {
    if(player && objectp(player))
        player->force_me("wield flaming sword in right hand");
}

void tattoo_timeout() {
    if(__owner && objectp(__owner) && interactive(__owner))
        tell_object(__owner, "The flaming sword fades back into your tattoo.\n");
    remove();
}

void remove() {
    remove_call_out("tattoo_timeout");
    if(__owner && objectp(__owner)) {
        catch(__owner->unwield(this_object()));
        catch(__owner->set_property("tattoo_sword_active", 0));
    }
    ::remove();
}
