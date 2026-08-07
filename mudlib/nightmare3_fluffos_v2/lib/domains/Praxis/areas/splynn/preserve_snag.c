// /d/Praxis/areas/splynn/preserve_snag.c
// Splynn Preserves hunting loop, R7: the trophy snag, boss room + loot cache.

#include <std.h>
#include <rooms.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"
#define LOOT_PATH "/domains/Praxis/equipment/splynn/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Trophy Snag");
    set_long(
        "A single dead tree stands here, a bone-white snag stripped of bark\n"
        "and hung with the spoils of the hunt. Skulls and hides and the\n"
        "salvaged plates of dead D-bees swing from its branches on wire, a\n"
        "slaver's trophy wall left to weather in the open. At its foot a\n"
        "cache chest of alien manufacture holds what the hunters strip from\n"
        "their kills before the beasts of the field can claim it.\n\n"
        "Whatever rules this ground considers the snag its own. The hive\n"
        "cleft drops away south; the ground climbs north toward a windblown\n"
        "ridge.");
    set_exits( ([
        "south" : "/domains/Praxis/areas/splynn/preserve_hive",
        "north" : "/domains/Praxis/areas/splynn/preserve_ridge"
    ]) );
    set_listen("default",
        "Trophies turning and knocking together on their wires, and a heavy,\n"
        "deliberate tread circling just beyond the snag.");
    set_smell("default",
        "Cured hide, old blood dried to rust, and the reptile musk of\n"
        "something that guards this place as its larder.");
    set_items( ([
        "snag"      : "A dead white tree hung with skulls, hides, and salvaged\n"
                      "armor plate. A hunter's trophy wall, left to the weather.",
        "trophies"  : "Kills from a dozen worlds strung up on wire: horned\n"
                      "skulls, scaled hides, the cracked plate of dead D-bees.",
        "cache"     : "A chest of alien make at the foot of the snag, where the\n"
                      "hunters stash what they strip from their kills. Search it."
    ]) );
}

void reset() {
    ::reset();
    if(random(100) < 15 && !present("kydian overlord", this_object()))
        clone_object(MONSTER_PATH+"kydian_overlord.c")->move(this_object());
    if(!present("alien hide", this_object()))
        clone_object(LOOT_PATH+"alien_hide.c")->move(this_object());
    if(random(100) < 25 && !present("net gun", this_object()))
        clone_object(LOOT_PATH+"net_gun.c")->move(this_object());
}
