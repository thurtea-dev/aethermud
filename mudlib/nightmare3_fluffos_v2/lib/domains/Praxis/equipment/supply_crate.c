// /d/Praxis/equipment/supply_crate.c
// Quest item: battered supply crate from the Lazlo caravan.
// Cannot be dropped or sold. Picking it up marks the lost_supplies objective.

#include <std.h>
#include <daemons.h>

inherit "/std/Object";

void create() {
    ::create();
    set_name("supply crate");
    set_id( ({ "supply crate", "crate", "supplies", "caravan supplies" }) );
    set_short("a battered supply crate");
    set_long(
        "A pre-Rifts military surplus crate, built to survive hard travel. "
        "It's badly dented and one corner is scorched, but the Lazlo trading\n"
        "company seal is intact. Whatever is inside is still sealed. This\n"
        "crate belongs to someone. Marta at the general store would probably\n"
        "want to know you found it.");
    set_mass(1200);
    set_value(0);
    set_vendor_type("quest");
    set_property("quest_item", 1);
}

int move(mixed dest) {
    int result;
    object env;

    result = ::move(dest);
    if(!result) {
        env = environment(this_object());
        if(env && living(env)) {
            catch(QUEST_D->update_objective(env, "lost_supplies", "supply_crate", 1));
            tell_object(env,
                "You pick up the supply crate. The Lazlo trading company seal is still intact.\n");
        }
    }
    return result;
}

int drop() {
    write("The supply crate is a quest item. You feel obligated to return it.\n");
    return 1;
}
