// /d/Praxis/equipment/computer_pad.c
// Standard computer pad -- data device, no combat use

inherit "/std/object";

void create() {
    ::create();
    set_name("computer pad");
    set_id( ({ "computer pad", "comp pad", "pad", "datapad", "computer" }) );
    set_short("a computer pad");
    set_long(
        "A ruggedized portable computer pad with a scratch-resistant screen.\n"
        "Pre-loaded with technical manuals, schematics, and diagnostic software.\n"
        "Standard issue for technicians throughout the Rifts world.");
    set_mass(300);
    set_value(500);
}
