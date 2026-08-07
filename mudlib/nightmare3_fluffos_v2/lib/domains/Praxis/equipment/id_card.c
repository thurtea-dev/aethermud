/* /domains/Praxis/equipment/id_card.c
   Universal Rifts ID card issued to every new character.
   Displays the holder's name, race, and OCC when examined. */

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("ID card");
    set_id( ({ "id card", "id", "identification card",
               "identification", "card" }) );
    set_short("an ID card");
    set_mass(10);
    set_value(0);
    set_property("no_drop", 1);
    set_property("id_card", 1);
    set_prevent_drop("You cannot drop your ID card.");
    set_prevent_get("You cannot take that.");
}

/* Persistent: skipped by the death->corpse transfer (std/user.c) and by
   normal save serialization (std/user/autosave.c pre_save()); recloned
   fresh via this recipe instead. No stored state needed -- query_long()
   reads owner info live. */
mixed *query_auto_load() {
    return ({ "/domains/Praxis/equipment/id_card", ({}) });
}

void init_arg(mixed args) {}

string query_long(string str) {
    object owner;
    string pname, race, occ;

    owner = environment(this_object());
    if(owner && (int)owner->is_player()) {
        pname = (string)owner->query_cap_name();
        race  = (string)owner->query_race();
        occ   = (string)owner->getenv("rifts_occ");
        if(!pname || !sizeof(pname)) pname = "Unknown";
        if(!race  || !sizeof(race))  race  = "Unknown";
        if(!occ   || !sizeof(occ))   occ   = "Civilian";
        return
            "A laminated identification card bearing a digital hologram.\n"
            "Name: " + pname + "\n"
            "Race: " + capitalize(race) + "\n"
            "OCC:  " + capitalize(occ) + "\n";
    }
    return
        "A laminated identification card. Rifts-standard format, "
        "issued to citizens and travelers.\n";
}
