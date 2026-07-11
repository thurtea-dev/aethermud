/* /domains/Praxis/equipment/cybernetics/language_translator.c
   Language translator implant. */

inherit "/std/Object";

void create() {
    ::create();
    set_name("language translator");
    set_id( ({ "language translator", "translator implant", "translator" }) );
    set_short("a language translator implant");
    set_long(
        "A finger-length processor that pattern-matches speech in real time.\n"
        "Understands the six most common languages of Rifts Earth, including\n"
        "American, Spanish, Techno-can and Dragonese. Installed by cyber-doc.");
    set_mass(0);
    set_value(8000);
    set_vendor_type("cybernetic");
    set_property("cybernetic", 1);
    set_property("cyber_slot", "translator");
    set_property("bionic_name", "language_translator");
}
