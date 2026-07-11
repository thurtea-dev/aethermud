/* /domains/NGR/areas/ngr_underground_bunker.c
   Hardened bunker below the NGR garrison.
   Spawns ngr_elite_soldier and triax_helmet loot on reset. */

#include <std.h>
#include <rooms.h>

#define NPC_PATH "/domains/NGR/npcs/"
#define EQ_PATH  "/domains/NGR/equipment/"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_property("ngr_territory", 1);
    set_short("NGR Hardened Bunker");
    set_long(
        "A hardened bunker below the garrison, accessed through a reinforced\n"
        "hatch and a short concrete stairwell. The ceiling is low, supported\n"
        "by MDC-rated ferroconcrete pillars. Emergency lighting strips run\n"
        "along the floor: the main lights are off. Equipment crates are stacked\n"
        "against the far wall, some open and emptied, some still sealed.\n\n"
        "This is the garrison's last-resort fallback position: hardened against\n"
        "direct artillery, stocked with reserve equipment, and guarded by\n"
        "soldiers in the heaviest armor the NGR fields. Whatever was worth\n"
        "storing here was worth protecting.\n"
    );
    set_exits( ([
        "up" : "/domains/NGR/areas/NGR_garrison"
    ]) );
    set_listen("default",
        "The hum of ventilation fans and the creak of ferroconcrete settling.\n"
        "Sealed space. The sounds of the garrison do not reach down here.");
    set_smell("default",
        "Concrete, machine oil, and the stale recycled air of a space that\n"
        "has been sealed and reopened many times.");
    set_items( ([
        "crates"   : "Military equipment crates, Triax-stenciled. Some are open\n"
                     "and empty. Some sealed crates remain: their contents are\n"
                     "classified, or were, before this facility went on reduced staff.",
        "pillars"  : "MDC-rated ferroconcrete pillars. The bunker structure is\n"
                     "rated to survive a direct hit from a TW artillery strike.",
        "hatch"    : "A reinforced access hatch above the stairwell. Heavy gauge\n"
                     "titanium-alloy, currently open. Closure takes four seconds.",
        "lighting" : "Emergency lighting strips along the floor, powered by an\n"
                     "independent fuel cell. The main lights have been off\n"
                     "for some time."
    ]) );
}

void reset() {
    ::reset();
    if(!present("NGR elite soldier", this_object()))
        clone_object(NPC_PATH + "ngr_elite_soldier")->move(this_object());
    if(!present("Triax PA-09 helmet", this_object()))
        clone_object(EQ_PATH + "triax_helmet")->move(this_object());
}
