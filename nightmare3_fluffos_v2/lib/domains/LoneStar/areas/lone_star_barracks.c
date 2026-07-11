// /domains/LoneStar/areas/lone_star_barracks.c
// Lone Star Dog Boy barracks -- housing for on-site guard personnel.

#define NPC_PATH "/domains/LoneStar/npcs/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 0);
    set_short("Lone Star: Dog Boy Barracks");
    set_long(
        "The barracks is a long prefab building with rows of bunks down each\n"
        "wall.  The accommodations are functional: metal frames, thin mattresses,\n"
        "personal lockers bolted to the wall.  Canine-featured soldiers occupy\n"
        "the space between shifts.\n\n"
        "One wall has a hand-drawn mural: a pack of Dog Boys in Coalition armor\n"
        "charging across a battlefield, paws reaching forward.  Crudely drawn\n"
        "but rendered with obvious care.  Someone here is proud of what they are.");
    set_exits( ([
        "west" : "/domains/LoneStar/areas/lone_star_plaza"
    ]) );
    set_listen("default",
        "Low conversation in Dog Boy dialect.  The scratch of claws on the floor.  Someone snoring.");
    set_smell("default",
        "The distinct musk of canine physiology mixed with CS-issue soap.");
    set_items( ([
        "bunks"   : "Metal-frame bunks, two high.  Each has a thin mattress and a single standard-issue blanket.",
        "lockers" : "Personal lockers bolted to the wall.  Each has a name plate.  The names are a mix of numbers and simple words.",
        "mural"   : "A hand-drawn scene of Dog Boys charging into battle in full CS armor.  The perspective is off but the energy is real.",
        "mattress": "Thin, standard military issue.  The Dog Boys do not seem to mind.",
        "blanket" : "Coalition gray.  Standard issue."
    ]) );
}

void reset() {
    object *inv;
    int count, i;

    ::reset();
    count = 0;
    inv = all_inventory(this_object());
    for(i = 0; i < sizeof(inv); i++) {
        if(strsrch(base_name(inv[i]), "cs_dog_boy_guard") != -1)
            count++;
    }
    while(count < 2) {
        clone_object(NPC_PATH + "cs_dog_boy_guard.c")->move(this_object());
        count++;
    }
}
