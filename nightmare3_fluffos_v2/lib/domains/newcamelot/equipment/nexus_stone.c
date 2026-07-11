// /domains/newcamelot/equipment/nexus_stone.c
// A ley line nexus stone from the Black Forest.
// Quest item for black_forest_nexus. After quest complete, becomes a keep item.
// While carried: +1 PPE per regen tick.

#include <std.h>
#include <daemons.h>

inherit OBJECT;

varargs int move(mixed dest, int flags) {
    int result;
    object dest_ob;

    result = ::move(dest, flags);
    dest_ob = environment(this_object());
    if(dest_ob && living(dest_ob) && interactive(dest_ob)) {
        if((int)QUEST_D->is_quest_active(dest_ob, "black_forest_nexus")) {
            if(!(int)QUEST_D->is_objective_complete(dest_ob, "black_forest_nexus", "nexus_stone_found")) {
                QUEST_D->update_objective(dest_ob, "black_forest_nexus", "nexus_stone_found", 1);
                tell_object(dest_ob,
                    "[Quest] The Black Forest Nexus: Nexus stone obtained. Return to the Pendragon barkeep.\n");
            }
        }
    }
    return result;
}

void create() {
    ::create();
    set_name("stone");
    set_id( ({ "stone", "nexus stone", "smooth stone", "glowing stone" }) );
    set_short("a smooth nexus stone");
    set_long(
        "A smooth stone that thrums faintly in your hand. Ley line energy\n"
        "pulses through it in slow waves. While you carry it, you feel the\n"
        "flow of PPE around you more clearly. A quest item for the Pendragon\n"
        "barkeep in New Camelot. After the quest it is yours to keep.\n"
    );
    set_mass(100);
    set_property("is_quest_item", 1);
    set_property("nexus_stone_bonus_ppe", 1);
    set_value(0);
}
