/* /cmds/mortal/_call.c
   Recalls a stabled pet/companion (see std/rifts_pet.c cmd_stable()).
   Only fires when no living pet answers "call <name>" first -- a
   carried/present pet's own add_action-bound cmd_call (e.g.
   trained_hawk.c's "come back" call) always wins over this fallback,
   since add_action bindings are checked before the mudlib's directory
   command search reaches this file. */

#include <std.h>

inherit DAEMON;

int cmd_call(string str) {
    object owner;
    object pet;
    string ptype;
    string pname;
    string idstr;
    string *ids;

    if(!str || !strlen(str)) return 0;
    owner = this_player();
    ptype = (string)owner->getenv("stabled_pet");
    if(!ptype || !strlen(ptype)) return 0;

    idstr = (string)owner->getenv("stabled_pet_ids");
    ids = (idstr && strlen(idstr)) ? explode(idstr, ",") : ({});
    if(member_array(lower_case(str), ids) == -1) return 0;

    pname = (string)owner->getenv("stabled_pet_name");
    if(!pname || !strlen(pname)) pname = "pet";

    pet = clone_object(ptype);
    if(!pet) {
        write("Something goes wrong recalling your companion. Contact staff.\n");
        return 1;
    }
    /* bind_to() (where a subclass defines it, e.g. rifts_familiar.c)
       wires subclass-private owner state a plain set_owner() would
       miss; set_owner() alone is enough for subclasses with no
       override (e.g. trained_hawk.c, whose own heart_beat() relies
       only on the base class's follow_owner()). */
    if(function_exists("bind_to", pet)) pet->bind_to(owner);
    else pet->set_owner(owner);
    if(function_exists("set_pet_name", pet)) pet->set_pet_name(pname);
    if(environment(owner) && environment(pet) != environment(owner))
        pet->move(environment(owner));

    owner->remove_env("stabled_pet");
    owner->remove_env("stabled_pet_name");
    owner->remove_env("stabled_pet_ids");

    tell_room(environment(owner),
        capitalize(pname) + " reappears, called back by " +
        (string)owner->query_cap_name() + ".\n");
    return 1;
}

void help() {
    write(
        "Syntax: call <pet>\n\n"
        "Recalls a companion you previously stabled (see: stable).\n"
        "Only works when the companion is currently in storage --\n"
        "if it is already out and about, use its own commands instead\n"
        "(for the hawk: say hawk come, or call hawk).\n\n"
        "See also: stable\n"
    );
}
