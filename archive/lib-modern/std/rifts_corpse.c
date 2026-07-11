// /std/rifts_corpse.c
// Rifts player corpse -- dissolves after 10 minutes and drops items to floor.

#include <std.h>

inherit STORAGE;

string corpse_name;

void create() {
    ::create();
    corpse_name = "unknown";
    set_weight(100);
    set_prevent_put("The corpse will not fit.");
    set_max_encumbrance(999999999);
    set_id(({ "corpse", "rifts corpse", "body", "remains" }));
}

void set_name(string str) {
    if(!str || !strlen(str)) return;
    corpse_name = str;
    set_short("the corpse of " + corpse_name);
    set_long(
        "The fallen body of " + corpse_name + ".  Weapons and equipment lie\n"
        "with the dead.  The ley line energy in the air makes the corpse\n"
        "feel strangely charged.  It will not last long in this reality.\n");
    set_id(({ "corpse", "body", "remains",
              "corpse of " + lower_case(corpse_name),
              "body of "   + lower_case(corpse_name) }));
    set_property("is_corpse", 1);
    call_out("dissolve", 600);
}

string query_name()  { return corpse_name; }
string query_short() { return "the corpse of " + corpse_name; }

int can_put_and_get() { return 1; }

void dissolve() {
    object *inv;
    object room;
    int i;

    if(!(room = environment(this_object()))) {
        remove();
        return;
    }
    inv = all_inventory(this_object());
    for(i = 0; i < sizeof(inv); i++) {
        if(!objectp(inv[i])) continue;
        if(inv[i]->move(room)) {
            if(!inv[i]->remove())
                destruct(inv[i]);
        }
    }
    tell_room(room,
        "The corpse of " + corpse_name +
        " dissolves into cascading ley line energy,\n"
        "leaving equipment scattered on the ground.\n");
    remove();
}
