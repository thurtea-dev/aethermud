// /domains/ChiTown/areas/bandit_camp.c
// Ruined gas station turned bandit camp east of Illinois Road.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Bandit Camp, Ruined Gas Station");
    set_long(
        "A ruined pre-Rifts gas station converted into a bandit camp. The\n"
        "canopy is collapsed on one side. A fire pit smolders in the center.\n"
        "Stolen CS gear is stacked carelessly against the far wall. Three\n"
        "armed raiders have made this their base. They don't look pleased\n"
        "to see you.\n"
    );
    set_exits( ([
        "west" : "/domains/ChiTown/areas/illinois_road"
    ]) );
    set_listen("default",
        "The crackle of a fire pit and the sound of someone loading a rifle.");
    set_smell("default",
        "Smoke, gun oil, and the unwashed smell of people living rough.");
    set_items( ([
        "fire" : "A smoldering fire pit. Recently fed.",
        "gear" : "Stolen Coalition supplies stacked against the wall. CS markings on all of it.",
        "canopy" : "The collapsed station canopy. Half of it lies flat on the ground.",
        "camp" : "A rough camp with bedrolls, salvage, and the evidence of several occupants."
    ]) );
}

void reset() {
    object *inv;
    int i, bcount;

    ::reset();
    inv = all_inventory(this_object());
    i = sizeof(inv);
    bcount = 0;
    while(i--) {
        if(living(inv[i]) &&
           strsrch((string)inv[i]->query_short(), "bandit") != -1)
            bcount++;
    }
    while(bcount < 3) {
        clone_object("/domains/ChiTown/monsters/bandit")->move(this_object());
        bcount++;
    }
}
