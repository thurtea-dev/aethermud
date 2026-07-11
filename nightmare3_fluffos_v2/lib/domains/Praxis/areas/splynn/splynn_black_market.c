// /d/Praxis/areas/splynn/splynn_black_market.c
// Hidden black market chamber behind the pillar passage.

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/Praxis/npcs/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no steal", 1);
    set_short("Black Market Chamber, Splynn");
    set_long(
        "A low-ceilinged chamber lit by chemical lanterns, the walls lined\n"
        "with shelves of goods that do not appear in any legitimate catalog.\n"
        "Pre-Rifts electronics. Weapons without serial numbers. Armor with\n"
        "faction markings sanded off. Several crates are stacked against the\n"
        "far wall, stenciled in three languages, all of them saying FRAGILE.\n\n"
        "A vendor sits behind a folding table watching you with practiced\n"
        "neutrality. This is the kind of place where everybody here knows\n"
        "not to use names. Type 'list' to see available merchandise.");
    set_exits( ([
        "east" : "/domains/Praxis/areas/splynn/splynn_back_alley"
    ]) );
    set_listen("default",
        "The chemical lanterns hiss softly. No other sound gets in.");
    set_smell("default",
        "Chemical fuel, old metal, and the distinctive smell of stored weapons\n"
        "that have not been fired recently.");
    set_items( ([
        "shelves" : "Shelves packed with goods that cannot be described in detail\n"
                      "without creating a liability. The vendor knows what is here.",
        "crates" : "Three large crates stenciled FRAGILE in Atlantean, American, and\n"
                      "what might be Japanese. Do not ask about the crates.",
        "lanterns" : "Chemical lanterns burning slow and steady. No electric light\n"
                      "in here. Better for discretion.",
        "table" : "Folding table with a battered terminal on one end and a locked\n"
                      "case on the other. The vendor runs the whole operation from here.",
        "weapons" : "Arrayed on the shelves: energy pistols, vibro-blades, and at\n"
                      "least one thing that looks like a net launcher. No tags."
    ]) );
}

void reset() {
    ::reset();
    if(!present("vendor", this_object()))
        clone_object(NPC_PATH+"black_market_vendor.c")->move(this_object());
}
