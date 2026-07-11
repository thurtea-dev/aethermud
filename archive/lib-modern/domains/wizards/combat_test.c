/* /domains/wizards/combat_test.c
   Combat testing zone. Combat is permitted here by design.
   Spawns SDC and MDC training dummies plus a restoration crystal. */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    /* "no attack" is intentionally absent -- combat is allowed here */
    set_short("Combat Testing Zone");
    set_long(
        "A reinforced chamber built for weapon and armor testing. The walls,\n"
        "floor, and ceiling are coated with impact-absorbing MDC plating. Two\n"
        "target stations stand at measured intervals, each sized for a humanoid\n"
        "combatant. A low pedestal near the entrance holds a faintly glowing\n"
        "gemstone. Both targets respawn after each reset. Wizard staff only.\n"
    );
    set_exits( ([ "north" : "/domains/wizards/lounge" ]) );
}

void reset() {
    object ob;
    ::reset();
    if(!present("training dummy", this_object())) {
        ob = clone_object("/domains/wizards/training_dummy");
        if(ob) ob->move(this_object());
    }
    if(!present("combat robot", this_object())) {
        ob = clone_object("/domains/wizards/mdc_training_dummy");
        if(ob) ob->move(this_object());
    }
    if(!present("restoration crystal", this_object())) {
        ob = clone_object("/domains/wizards/restore_crystal");
        if(ob) ob->move(this_object());
    }
}
