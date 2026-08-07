/* /obj/tattoo_weapons/tattoo_battle_axe.c
   Summoned magical battle axe from an Atlantean tattoo.
   Auto-destroys after 10 minutes (600 seconds). */

#include <std.h>

inherit "/std/weapon";

private object __owner;

void create() {
    ::create();
    set_name("magical battle axe");
    set_id(({ "axe", "battle axe", "magical battle axe", "tattoo axe" }));
    set_short("a magical battle axe");
    set_long(
        "A broad-bladed battle axe of solid magical force summoned from a\n"
        "tattoo on the wielder's arm. The edge shimmers with concentrated\n"
        "PPE and cleaves mega-damage material.");
    set_mass(200);
    set_wc(12);
    set_type("blade");
    set_property("tattoo_weapon",   1);
    set_property("summoned_weapon", 1);
    set_property("mdc_weapon",      1);
    set_property("no_drop",         1);
    set_property("damage_num",      2);
    set_property("damage_sides",    6);
    set_property("damage_bonus",    0);
    set_property("damage_string",   "2d6 MD");
}

void init_tattoo_owner(object player) {
    __owner = player;
    call_out("tattoo_timeout", 600);
    move(player);
    call_out("do_wield", 0, player);
}

private void do_wield(object player) {
    if(player && objectp(player))
        player->force_me("wield magical battle axe in right hand");
}

void tattoo_timeout() {
    if(__owner && objectp(__owner) && interactive(__owner))
        tell_object(__owner, "The magical battle axe fades back into your tattoo.\n");
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
