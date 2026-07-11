// /d/Praxis/areas/coalition_checkpoint.c
// CS checkpoint at the south edge of Monument Square.
// D-Bees and high-infamy players are attacked on entry.

#include <std.h>
#include <rooms.h>
#include <daemons.h>

#define MONSTER_PATH "/domains/Praxis/monsters/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_short("Coalition Checkpoint, south gate");
    set_long(
        "A reinforced checkpoint booth straddles the only paved road south.\n"
        "Portable flood lights wash the area in harsh white light at all\n"
        "hours. Coalition skull-and-crossbones banners hang on either side\n"
        "of a vehicle barrier, a heavy steel arm that blocks the road.\n\n"
        "Two CS Dead Boy soldiers man the post. The skull faceplates of\n"
        "their helmets are expressionless. One has a CP-40 at low ready;\n"
        "the other stands with arms folded, watching.\n\n"
        "Humans and CS allies may pass south freely. Everyone else is\n"
        "considered a threat. The square is back to the north.");
    set_exits( ([
        "north" : "/domains/Praxis/areas/monument_square",
        "south" : "/domains/Praxis/areas/cs_territory"
    ]) );
    set_listen("default",
        "The hum of equipment inside the booth, boots on asphalt, the click of weapon mechanisms.");
    set_smell("default",
        "CS equipment lubricant, charged metal, and the ozone of active scanners.");
    set_items( ([
        "booth" : "A reinforced checkpoint booth with slit windows and a scanner array.",
        "banner" : "Coalition skull-and-crossbones on a black field. The universal CS hello.",
        "barrier" : "A heavy steel arm blocking the south road. Hydraulically operated from the booth.",
        "scanner" : "A sensor array mounted on the booth roof sweeps the area constantly.",
        "lights" : "Portable flood lights. Enough to see clearly even at night.",
        "road" : "The paved road south leads into Coalition territory."
    ]) );
}

void init() {
    ::init();
    if(!this_player() || this_player() == this_object()) return;
    if(this_player()->query_ghost()) return;
    if((int)WANTED_D->is_cs_enemy(this_player()))
        write("The checkpoint scanner flares red. A Dead Boy snaps his CP-40 up.\n");
    else
        write("The scanner marks you green. A guard mutters, 'Move along, citizen.'\n");
}

int is_guard(object ob) {
    return (int)ob->id("cs soldier");
}

void reset() {
    int guard_count;
    object *inv;
    int i;

    ::reset();
    inv = all_inventory(this_object());
    guard_count = 0;
    i = sizeof(inv);
    while(i--)
        if((int)inv[i]->id("cs soldier")) guard_count++;
    while(guard_count < 2) {
        clone_object(MONSTER_PATH+"coalition_dead_boy.c")->move(this_object());
        guard_count++;
    }
}
