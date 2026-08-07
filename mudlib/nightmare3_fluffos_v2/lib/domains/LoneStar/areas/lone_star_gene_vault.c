/* /domains/LoneStar/areas/lone_star_gene_vault.c
   Lone Star genetic sample vault, east of the research lab.
   Cold storage for the source material every Lone Star line is drawn from.
   Connects west to the lab, north to the observation booth. */

#define NPC_PATH "/domains/LoneStar/npcs/"

#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("no steal", 1);
    set_short("Lone Star: Gene Vault");
    set_long(
        "A narrow, freezing room walled floor to ceiling in numbered sample\n"
        "drawers. Frost creeps along the seams. This is the source library:\n"
        "every Dog Boy line, every psi-hound variant, every failed branch,\n"
        "reduced to a coded drawer and a tissue sample suspended in nitrogen.\n\n"
        "A reader terminal sits on a stand in the middle of the room, its\n"
        "index locked behind a clearance prompt. The drawers themselves are\n"
        "sealed magnetically and log every opening.\n\n"
        "The lab is back west. A short stair climbs north to the observation\n"
        "booth over the incubation gallery.");
    set_exits( ([
        "west"  : "/domains/LoneStar/areas/lone_star_lab",
        "north" : "/domains/LoneStar/areas/lone_star_observation"
    ]) );
    set_listen("default",
        "The steady tick of the cooling plant and the hiss of nitrogen\n"
        "topping off somewhere behind the drawer faces.");
    set_smell("default",
        "Cold so dry it has a smell of its own, and the sharp bite of\n"
        "coolant.");
    set_items( ([
        "drawers"  : "Numbered sample drawers, magnetically sealed. Each\n"
                     "opening is logged with a name and a time.",
        "frost"    : "Frost creeping along every drawer seam. Nothing in\n"
                     "here has been warm in decades.",
        "terminal" : "A reader terminal on a stand. The sample index is\n"
                     "behind a clearance prompt you cannot answer.",
        "samples"  : "Tissue held in nitrogen. Every line the complex runs\n"
                     "starts as one of these.",
        "index"    : "A locked catalog. The drawer codes mean nothing\n"
                     "without it, which is the point."
    ]) );
}

void reset() {
    ::reset();
    if(!present("Dog Boy guard", this_object()))
        clone_object(NPC_PATH + "cs_dog_boy_guard.c")->move(this_object());
}
