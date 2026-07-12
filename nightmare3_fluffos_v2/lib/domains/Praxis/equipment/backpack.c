/* /domains/Praxis/equipment/backpack.c
   Standard field backpack. Worn on the back, and holds gear:
   'put <item> in backpack', 'get/take <item> from backpack'.
   Container logic is local because ARMOUR and STORAGE cannot be
   safely multiple-inherited (duplicate Object state). */

#include <std.h>
#include <move.h>

inherit ARMOUR;

#define MAX_LOAD 8000

private int __Load;

void create() {
    ::create();
    set_name("backpack");
    set_id( ({ "backpack", "pack", "rucksack", "field pack" }) );
    set_short("a field backpack");
    set_long(
        "A sturdy canvas pack with padded shoulder straps and multiple compartments.\n"
        "Well-worn but serviceable. Worn on the back, freeing both hands.\n"
        "Put things in it with 'put <item> in pack' and retrieve them with\n"
        "'get <item> from pack'."
    );
    set_mass(600);
    set_value(120);
    set_type("backpack");
    set_limbs( ({ "torso" }) );
    set_ac(0);
    set_property("rifts_slot", "back");
    __Load = 0;
}

void init() {
    ::init();
    add_action("PackPut", "put");
    add_action("PackGet", "get");
    add_action("PackGet", "take");
}

int receive_objects(object ob) {
    if(!ob) return 0;
    if(living(ob)) return 0;
    return 1;
}

void add_encumbrance(int x) {
    __Load += x;
    if(__Load < 0) __Load = 0;
}

int query_encumbrance() { return __Load; }

static int PackPut(string str) {
    object ob;
    string what, where;

    if(!str) return 0;
    if(sscanf(lower_case(str), "%s in %s", what, where) != 2) return 0;
    if(present(where, environment(this_player())) != this_object() &&
       present(where, this_player()) != this_object()) return 0;
    if(!(ob = present(what, this_player()))) {
        notify_fail("You are not carrying that.\n");
        return 0;
    }
    if(ob == this_object()) {
        message("my_action", "You cannot change the laws of physics.",
            this_player());
        return 1;
    }
    if(!((int)ob->allow_put(this_player()))) return 1;
    if(!((int)ob->allow_drop(this_player()))) return 1;
    if((int)ob->move(this_object())) {
        message("my_action", "It does not fit in the pack.", this_player());
        return 1;
    }
    message("my_action", sprintf("You put %s into %s.",
        (string)ob->query_short(), (string)query_short()), this_player());
    message("other_action", sprintf("%s puts %s into %s.",
        (string)this_player()->query_cap_name(), (string)ob->query_short(),
        (string)query_short()), environment(this_player()),
        ({ this_player() }));
    return 1;
}

static int PackGet(string str) {
    object *things;
    object ob;
    string what, where;
    int i;

    if(!str) return 0;
    if(sscanf(lower_case(str), "%s from %s", what, where) != 2) return 0;
    if(present(where, environment(this_player())) != this_object() &&
       present(where, this_player()) != this_object()) return 0;
    if(what == "all") {
        things = all_inventory(this_object());
        i = sizeof(things);
    } else if(!(ob = present(what, this_object()))) {
        message("my_action", "There is no such thing in the pack.",
            this_player());
        return 1;
    } else {
        i = 1;
        things = ({ ob });
    }
    while(i--) {
        if(!((int)things[i]->allow_get(this_player()))) continue;
        if((int)things[i]->move(this_player())) {
            message("my_action", "You cannot carry that!", this_player());
            continue;
        }
        message("my_action", sprintf("You get %s from %s.",
            (string)things[i]->query_short(), (string)query_short()),
            this_player());
        message("other_action", sprintf("%s gets %s from %s.",
            (string)this_player()->query_cap_name(),
            (string)things[i]->query_short(), (string)query_short()),
            environment(this_player()), ({ this_player() }));
    }
    return 1;
}

varargs string query_long(string str) {
    string tmp;
    object *inv;
    int i;

    tmp = ::query_long(str);
    inv = all_inventory(this_object());
    if(!sizeof(inv)) return tmp + "\nThe pack is empty.";
    tmp += "\nThe pack contains:";
    for(i = 0; i < sizeof(inv); i++)
        if((string)inv[i]->query_short())
            tmp += "\n  " + (string)inv[i]->query_short();
    return tmp;
}
