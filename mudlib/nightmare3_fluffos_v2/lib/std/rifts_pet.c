// /std/rifts_pet.c
// Base pet object.  Follows player between rooms.  Has a hunger level.
// Override set_pet_name(), set_hunger_max(), etc. in subclasses.

#include <std.h>
#include <daemons.h>
#include <move.h>

inherit MONSTER;

#define HAWK_SAY_DELAY 2

void sync_hawk_position(int just_followed);
varargs void hawk_say_and_queue(string str, string callback, mixed extra);

private string __pet_name;
private int    __hunger;
private int    __hunger_max;
private object __owner;
private int    __stay_mode;

void create() {
    ::create();
    __pet_name   = "pet";
    __hunger     = 0;
    __hunger_max = 100;
    __owner      = 0;
    __stay_mode  = 0;
    set_aggressive(0);
    set_moving(0);
}

string query_pet_name()  { return __pet_name; }
int query_hunger()       { return __hunger; }
int query_hunger_max()   { return __hunger_max; }
object query_owner()     { return __owner; }
int query_stay_mode()    { return __stay_mode; }

void set_pet_name(string n)  { __pet_name  = n; }
void set_hunger(int v)       { __hunger    = v; }
void set_hunger_max(int v)   { __hunger_max = v; }
void set_owner(object o) {
    __owner = o;
    set_heart_beat(1);
}

void set_stay_mode(int v) {
    __stay_mode = v;
    sync_hawk_position(0);
}

int test_heart() {
    if(__owner && objectp(__owner)) return 1;
    return ::test_heart();
}

void sync_hawk_position(int just_followed) {
    string pos;

    if(!(int)query_property("is_hawk")) return;

    if(__stay_mode) {
        pos = "is perched nearby.";
    } else if(just_followed || (int)query_property("hawk_released")) {
        pos = "is circling overhead.";
    } else if((int)query_property("hawk_perched")) {
        pos = "is perched nearby.";
    } else {
        pos = "is circling overhead.";
    }

    set_property("default_position", pos);
    set_property("position_str", pos);
}

int move(mixed dest) {
    int result;

    result = ::move(dest);
    if(result == MOVE_OK && (int)query_property("is_hawk"))
        sync_hawk_position(0);
    return result;
}

varargs void hawk_say_and_queue(string str, string callback, mixed extra) {
    catch("/cmds/mortal/_say"->cmd_say(str));
    if(extra)
        call_out(callback, HAWK_SAY_DELAY, extra);
    else
        call_out(callback, HAWK_SAY_DELAY);
}

void eat_from_room() {
    object env;
    object *inv;
    int i;

    env = environment(this_object());
    if(env && living(env) && __owner)
        env = environment(__owner);
    if(!env) return;

    inv = all_inventory(env);
    i   = sizeof(inv);
    while(i--) {
        if((int)inv[i]->query_property("is_corpse")) {
            tell_room(env,
                (string)query_short() + " tears into " +
                (string)inv[i]->query_short() + " hungrily.\n");
            inv[i]->destruct();
            __hunger = 0;
            return;
        }
    }
}

/* Generic across every rifts_pet.c subclass: id(str) matches whatever
   the subclass's own set_id() list is (hawk/bird/falcon, familiar/
   sprite/spirit, ...), so this needs no per-subtype special-casing.
   Recall is handled by the standalone fallback command
   cmds/mortal/_call.c, since there is no live object left to bind an
   add_action to once this one destructs itself. That fallback only
   ever runs when no living pet's own add_action-bound "call" (e.g.
   trained_hawk.c's cmd_call) answers first. */
void init() {
    ::init();
    add_action("cmd_stable", "stable");
}

int cmd_stable(string str) {
    object owner;
    string ptype;
    string pname;

    owner = query_owner();
    if(!owner || owner != this_player()) return 0;
    if(!str || !id(str)) return 0;
    if((string)owner->getenv("stabled_pet") &&
       strlen((string)owner->getenv("stabled_pet"))) {
        tell_object(owner,
            "You already have a companion in storage. Call it back first.\n");
        return 1;
    }
    ptype = base_name(this_object());
    pname = query_pet_name();
    owner->setenv("stabled_pet", ptype);
    owner->setenv("stabled_pet_name", pname);
    owner->setenv("stabled_pet_ids", implode(query_id(), ","));
    tell_room(environment(this_object()),
        (string)owner->query_cap_name() + " stables " + pname + ". " +
        capitalize(pname) + " settles down and disappears from view.\n");
    destruct(this_object());
    return 1;
}

void follow_owner() {
    object owner;
    object owner_room;
    object my_room;

    if(__stay_mode) return;
    owner = __owner;
    if(!owner || !objectp(owner)) return;
    owner_room = environment(owner);
    my_room    = environment(this_object());
    if(!owner_room || !my_room) return;
    if(owner_room == my_room) return;
    move(owner_room);
    sync_hawk_position(1);
    tell_room(owner_room,
        capitalize((string)query_pet_name()) + " arrives, following " +
        (string)owner->query_cap_name() + ".\n",
        ({ this_object() }));
}
