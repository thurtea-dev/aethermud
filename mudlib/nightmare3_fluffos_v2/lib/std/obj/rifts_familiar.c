// /std/obj/rifts_familiar.c
// Small magical familiar from call familiar.

#include <std.h>

inherit "/std/rifts_pet";

private object __owner;

void create() {
    ::create();
    __owner = 0;
    set_name("familiar");
    set_id(({ "familiar", "sprite", "spirit", "magical familiar" }));
    set_short("a magical familiar");
    set_long(
        "A small glowing spirit-creature bound to a mage. It watches alertly\n"
        "and follows its master everywhere.");
    set_mass(1);
    set_level(1);
    set_aggressive(0);
    set_moving(0);
}

void bind_to(object owner) {
    if(!owner) return;
    __owner = owner;
    set_owner(owner);
    if(environment(owner) && environment(this_object()) != environment(owner))
        move(environment(owner));
}

object query_bound_owner() { return __owner; }

void heart_beat() {
    object env;

    ::heart_beat();
    if(!__owner || !objectp(__owner)) return;
    env = environment(__owner);
    if(env && environment(this_object()) != env)
        move(env);
}
