// /domains/Lazlo/monsters/ley_line_instructor.c
// Ley line instructor in the Lazlo magic quarter. Flavor NPC only.
// Skills and languages are granted by RP wizards, not NPCs.

#include <std.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("ley line instructor");
    set_npc_name("Instructor");
    set_id( ({ "ley line instructor", "instructor", "mage", "teacher",
               "scholar", "academic" }) );
    set_short("a ley line instructor");
    set_long(
        "A senior mage in the ochre-and-gray academic robes of Lazlo's\n"
        "schools. Older, unhurried, with the precise movements of someone\n"
        "who has spent decades handling volatile energies. A teaching\n"
        "staff leans against the wall beside them.");
    set_level(12);
    set_max_hp(150);
    set_hp(150);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(500);
    set_body_type("human");

    add_response("hello", "Welcome to the magic quarter of Lazlo. This is a place of study and practice.");
    add_response("hi", "Welcome to the magic quarter of Lazlo. This is a place of study and practice.");
    add_response("bye", "Knowledge carries its own weight. Travel safely.");
    add_response("magic", "The ley lines here are strong. You can feel it if you pay attention.");
    add_response("ppe", "Those with magical training find their PPE recovers faster near a ley line nexus.");
    set_default_response("The instructor waits patiently.");
}

void catch_tell(string str) {
    string a, b;

    if(sscanf(str, "%s says: %s", a, b) != 2) {
        ::catch_tell(str);
        return;
    }
    b = lower_case(b);

    if(strsrch(b, "learn") != -1 || strsrch(b, "teach") != -1 ||
       strsrch(b, "language") != -1 || strsrch(b, "skill") != -1) {
        say_here("Skills and languages are granted by RP wizards. "
                 "Send a tell to an online RP wizard with the skills "
                 "you want and they will review your request.");
        return;
    }
    ::catch_tell(str);
}

void init() {
    ::init();
}
