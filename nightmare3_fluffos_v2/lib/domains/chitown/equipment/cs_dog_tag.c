// /domains/chitown/equipment/cs_dog_tag.c
// Coalition States dog tag, salvaged from a dead patrol officer.

inherit "/std/Object";

void create() {
    ::create();
    set_name("CS dog tag");
    set_id( ({ "dog tag", "tag", "dog tags", "coalition dog tag",
               "cs dog tag", "salvage" }) );
    set_short("a CS dog tag");
    set_long(
        "A stamped metal tag on a polymer cord. Coalition States serial\n"
        "number, blood type, and unit designation. The kind of thing you\n"
        "strip from the dead. Black market buyers pay well for proof of\n"
        "Coalition casualties.");
    set_mass(20);
    set_value(150);
    set_property("salvage", 1);
}
