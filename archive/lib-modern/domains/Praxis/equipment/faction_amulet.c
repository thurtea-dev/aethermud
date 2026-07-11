/* /domains/Praxis/equipment/faction_amulet.c
   Faction membership amulet. Worn at the neck.
   Admin sets "faction_name" property to the faction display name before giving.
   Example: set_property("faction_name", "Splugorth Empire") */

#include <std.h>

inherit ARMOUR;

void create() {
    ::create();
    set_name("amulet");
    set_id( ({ "amulet", "clan amulet", "faction amulet", "disc", "medallion" }) );
    set_short("a clan amulet");
    set_long(
        "A flat metal disc on a woven cord. Both sides are engraved with a\n"
        "clan sigil and a name. The metal is cold to the touch and slightly\n"
        "heavier than it looks. It carries the faint dimensional resonance\n"
        "of Splynn work: functional as identification, not just decoration.\n"
        "Worn at the neck as proof of faction allegiance.\n"
    );
    set_armour_type("neck");
    set_property("rifts_slot", "neck");
    set_ac(0);
    set_mass(50);
    set_value(0);
}

string query_short() {
    string fname;
    fname = (string)query_property("faction_name");
    if(fname && sizeof(fname))
        return "a " + fname + " clan amulet";
    return "a clan amulet";
}

string query_long(object viewer) {
    string fname;
    fname = (string)query_property("faction_name");
    if(!fname || !sizeof(fname)) fname = "your faction";
    return
        "A flat metal disc on a woven cord. Both sides are engraved with\n"
        "the sigil of " + fname + ". The metal is cold to the touch and slightly\n"
        "heavier than it looks. It carries the faint dimensional resonance\n"
        "of Splynn work: functional as identification, not just decoration.\n"
        "Worn at the neck as proof of allegiance to " + fname + ".\n";
}
