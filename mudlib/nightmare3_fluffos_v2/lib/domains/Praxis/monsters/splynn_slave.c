// /domains/Praxis/monsters/splynn_slave.c
// Generic Splynn slave NPC for slave pens and market claim mechanic.

#include <std.h>

inherit "/std/rifts_npc";

void create() {
    string *races;
    string r;

    ::create();
    races = ({ "human", "elf", "dwarf", "ogre", "orc", "atlantean" });
    r = races[random(sizeof(races))];
    set_name("slave");
    set_id(({ "slave", "captive", r }));
    set_race(r);
    set_short("a " + r + " slave");
    set_long(
        "A captive from another world, collared and subdued by Splugorth "
        "technology. Shock manacles glint at the wrists.");
    set_level(2 + random(3));
    set_alignment(-20);
    set_property("is_slave", 1);
    set_property("owned_by", 0);
    set_property("no_attack", 1);
    set_property("passive", 1);
    set_property("aggressive", 0);
    set_hp(15 + random(10));
}
