// /domains/demon_plane/equipment/rift_shard_amulet.c
// Quest loot from the demon-plane sliver shrine.

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("rift shard amulet");
    set_id(({ "rift shard amulet", "amulet", "shard amulet", "rift shard",
              "shard" }));
    set_short("a rift shard amulet");
    set_long(
        "A fist-sized crystal shard on a braided ley-wire chain. It pulses\n"
        "violet when near dimensional tears. Ley researchers pay well for\n"
        "stable samples from off-world rifts.");
    set_mass(80);
    set_value(2500);
    set_property("quest_item", "demon_sliver_shard");
}
