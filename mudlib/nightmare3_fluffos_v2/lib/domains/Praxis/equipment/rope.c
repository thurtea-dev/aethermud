// /d/Praxis/equipment/rope.c
// 50 feet of rope -- utility item.

inherit "/std/Object";

void create() {
    ::create();
    set_name("rope");
    set_id( ({ "rope", "50ft rope", "fifty foot rope", "climbing rope" }) );
    set_short("50 feet of rope");
    set_long(
        "Fifty feet of braided nylon climbing rope, pre-Rifts manufacture.\n"
        "Still in good shape. Someone scavenged it carefully. Useful for\n"
        "climbing, restraining, rigging, or a dozen other survival tasks.");
    set_mass(500);
    set_value(100);
    set_vendor_type("equipment");
}
