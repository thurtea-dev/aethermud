// /obj/tattoo_weapons/tattoo_dagger.c
// Summoned magical dagger from an Atlantean tattoo.
// Auto-destroys after 10 minutes (600 seconds).

#include <std.h>

inherit "/std/weapon";

private object __owner;

void create() {
    ::create();
    set_name("magical dagger");
    set_id(({ "dagger", "magical dagger", "tattoo dagger" }));
    set_short("a magical dagger");
    set_long(
        "A dagger of solid magical force summoned from a tattoo.\n"
        "The blade glows faintly with stored PPE energy and cuts through\n"
        "MDC materials as easily as ordinary matter.");
    set_mass(50);
    set_wc(8);
    set_type("blade");
    set_property("tattoo_weapon",   1);
    set_property("summoned_weapon", 1);
    set_property("mdc_weapon",      1);
    set_property("no_drop",         1);
    set_property("damage_num",      1);
    set_property("damage_sides",    6);
    set_property("damage_bonus",    0);
    set_property("damage_string",   "1d6 MD");
}

void init_tattoo_owner(object player) {
    __owner = player;
    call_out("tattoo_timeout", 600);
    move(player);
    call_out("do_wield", 0, player);
}

private void do_wield(object player) {
    if(player && objectp(player))
        player->force_me("wield magical dagger in right hand");
}

void tattoo_timeout() {
    if(__owner && objectp(__owner) && interactive(__owner))
        tell_object(__owner, "The magical dagger fades back into your tattoo.\n");
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
