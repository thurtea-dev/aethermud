/* /domains/Praxis/equipment/alvurron_key.c
   Dimensional key found in the Alvurron inner district. No mechanics this pass. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("alvurron key");
    set_id( ({ "alvurron key", "key", "dimensional key" }) );
    set_short("an Alvurron dimensional key");
    set_long(
        "A device the size of a fist made of a dark material that is neither\n"
        "metal nor plastic. It hums at a frequency you can feel but not hear.\n"
        "Its purpose is not obvious but it is clearly valuable.");
    set_mass(20);
    set_value(5000);
}
