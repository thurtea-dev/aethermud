// /d/Praxis/equipment/cybernetics/cyber_eye_optic.c
// Cybernetic optic eye implant.

inherit "/std/Object";

void create() {
    ::create();
    set_name("cybernetic eye");
    set_id( ({ "cybernetic eye", "cyber eye", "optic implant", "eye implant",
               "bionic eye", "optic eye" }) );
    set_short("a cybernetic optic eye implant");
    set_long(
        "A prosthetic eye with multiple vision modes. Slightly larger than a\n"
        "natural eye, with a faint electronic glow. Models include nightvision\n"
        "(200ft), telescopic (5x magnification), and targeting enhancement.\n"
        "Installed by a qualified cyber-doc or body fixer.");
    set_mass(0);
    set_value(3000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "eye");
    set_property("bionic_name", "cyber_eye_optics");
    set_property("nightvision", 200);
    set_property("telescopic", 5);
}
