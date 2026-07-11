// /d/Praxis/equipment/splynn/alien_hide.c
// Preserve beast hide stripped after hunting. Salvage item worth 200 credits.
// Counts as salvage for Rocky's talisman quest.

inherit "/std/Object";

void create() {
    ::create();
    set_name("salvage");
    set_id( ({ "salvage", "alien hide", "hide", "beast hide", "dimensional hide",
               "preserve hide", "skin" }) );
    set_short("a preserve beast hide");
    set_long(
        "A section of hide stripped from a preserve beast after the hunt.\n"
        "The camouflage patterning is visible even in death: mottled grey-green\n"
        "that shifts slightly under different light. The material itself is\n"
        "tougher than it looks. There is a market for this: armor crafters,\n"
        "exotic goods dealers, and collectors who do not ask where things come from.\n"
        "Value: approximately 200 credits.");
    set_mass(600);
    set_value(200);
    set_vendor_type("salvage");
    set_property("alien_hide", 1);
}
