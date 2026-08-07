/* /domains/LoneStar/areas/lone_star_training_ring.c
   Lone Star Dog Boy kennel wing: the psi-scent training ring.
   Where juveniles are trained to detect magic and psionics.
   Connects west to the kennel yard. */

#define MON_PATH "/domains/LoneStar/monsters/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_short("Lone Star: Training Ring");
    set_long(
        "A sand ring ringed by low bleachers, with a rack of scent boxes on\n"
        "a wheeled cart at the edge. The boxes are lead-lined and padlocked,\n"
        "each labeled with a hazard code rather than a name: confiscated\n"
        "talismans, ward stones, and rune fragments, kept only because a\n"
        "juvenile has to learn what magic smells like before it can be sent\n"
        "out to find it.\n\n"
        "The drill never varies: box opened, box closed, reward or\n"
        "correction. The sand is worn into a groove along the arc where it\n"
        "runs.\n\n"
        "The kennel yard is back west.");
    set_exits( ([
        "west" : "/domains/LoneStar/areas/lone_star_kennel_yard"
    ]) );
    set_listen("default",
        "A clicker, a whistle, and a low growl that rises whenever one of\n"
        "the boxes is unlatched.");
    set_smell("default",
        "Hot sand and, under it, the flat metallic tang that clings to\n"
        "anything the Coalition has confiscated from a ley line.");
    set_items( ([
        "boxes"     : "Lead-lined scent boxes, padlocked, labeled by hazard\n"
                      "code. Whatever is inside is meant to be smelled, not\n"
                      "seen.",
        "cart"      : "A wheeled rack holding the scent boxes. It is chained\n"
                      "and logged out like a weapon.",
        "sand"      : "Churned sand, worn into a groove along one arc where\n"
                      "the drill always runs.",
        "bleachers" : "Low bleachers for observers. Mostly empty. The drill\n"
                      "is not a performance.",
        "ring"      : "A packed sand circle about twenty meters across,\n"
                      "bounded by a painted rail."
    ]) );
}

void reset() {
    ::reset();
    if(!present("dog boy pup", this_object()))
        clone_object(MON_PATH + "ls_dog_boy_pup.c")->move(this_object());
}
