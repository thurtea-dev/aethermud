/* /domains/Praxis/equipment/bandit_patch.c
   Proof of defeating a highway bandit. Turn in to Arbiter Kane for a life grant. */

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("bandit patch");
    set_id(({ "bandit patch", "patch", "raider patch", "scavenger patch",
              "proof" }));
    set_short("a scavenger raider patch");
    set_long(
        "A torn shoulder patch cut from a highway bandit's leathers. Faded\n"
        "red paint marks it as road-scavenger gear. Proof you survived a\n"
        "fight on the old highway.");
    set_mass(10);
    set_value(0);
}
