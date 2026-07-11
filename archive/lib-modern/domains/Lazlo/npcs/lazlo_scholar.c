// /domains/Lazlo/npcs/lazlo_scholar.c
// Lazlo scholar -- researcher in the Great Library.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Lazlo scholar");
    set_npc_name("Scholar");
    set_id( ({ "lazlo scholar", "scholar", "researcher", "academic" }) );
    set_short("a Lazlo scholar");
    set_long(
        "A bookish human in layered robes, carrying a stack of notes. One\n"
        "corner of a data-slate peeks out from under the papers. They move\n"
        "through the library with the comfortable unhurriedness of someone\n"
        "who spends most of their life here. Lazlo attracts the learned\n"
        "from across North America -- and beyond.");

    set_level(4);
    set_max_hp(50);
    set_hp(50);
    set_aggressive(0);
    set_moving(0);
    set_race("human");
    set_alignment(300);
    set_body_type("human");

    add_response("hello",
        "Scholar says: 'Welcome to Lazlo. The library is to the east. "
        "Mind the ley line shrine -- it pulses at midnight.'");
    add_response("hi",
        "Scholar says: 'Welcome to Lazlo. The library is to the east. "
        "Mind the ley line shrine -- it pulses at midnight.'");
    add_response("library",
        "Scholar says: 'Three levels of general reference, pre-Rifts science, "
        "and magical theory. Restricted access on level three. Ask the librarian.'");
    add_response("lazlo",
        "Scholar says: 'One of the few places on this earth where knowledge is "
        "more valuable than firepower. I rarely leave if I can help it.'");
    add_response("magic",
        "Scholar says: 'We have the largest collection of post-Rifts magical "
        "theory outside Atlantis. Perhaps the most complete, given that "
        "Atlantis is not open to outside researchers.'");
    add_response("coalition",
        "Scholar says: 'They burned the libraries in Chi-Town. Twice. "
        "Some things cannot be replaced.'");
    add_response("ley line",
        "Scholar says: 'The shrine pulses at midnight precisely. I have "
        "measured it for three years. The period has not varied by more "
        "than four seconds.'");
    add_response("help",
        "Scholar says: 'I am fairly absorbed in my work, but happy to answer questions.'");
    add_response("bye",
        "Scholar says: 'Good luck to you. Mind the exits -- the library closes at the third bell.'");
    set_default_response("The scholar glances up from their notes briefly, then returns to work.");
}
