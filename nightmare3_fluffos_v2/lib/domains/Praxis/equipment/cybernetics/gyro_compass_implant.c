// /d/Praxis/equipment/cybernetics/gyro_compass_implant.c
// Internal gyro-compass implant.

inherit "/std/Object";

void create() {
    ::create();
    set_name("gyro compass implant");
    set_id( ({ "gyro compass", "compass implant", "internal compass",
               "gyrocompass", "directional implant" }) );
    set_short("an internal gyro-compass implant");
    set_long(
        "A small internal device providing perfect directional sense.\n"
        "Always knows true north, elevation, and relative position. Never\n"
        "lost even underground or in total darkness. Skill bonuses:\n"
        "+20% navigation, +15% land navigation.");
    set_mass(0);
    set_value(1500);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "internal");
    set_property("bionic_name", "gyro_compass");
    set_property("skill_bonus_navigation", 20);
    set_property("skill_bonus_land_navigation", 15);
}
