/* /d/Praxis/monsters/clan_recruiter.c
   Kittani administrator at the Splynn Clan Hall.
   Handles faction membership inquiries.
   Actual join is admin-gated via FACTION_D->set_faction_member(). */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("administrator");
    set_npc_name("Kittani administrator");
    set_id( ({ "administrator", "kittani", "recruiter", "clerk", "kittani administrator" }) );
    set_short("a Kittani administrator");
    set_long(
        "A Kittani in formal segmented armor the color of dark copper.\n"
        "The scales on the exposed parts of their face are precise and\n"
        "unmarked: a clan official, not a warrior. A ledger is open on the\n"
        "desk and a stylus rests in three-fingered hands.\n\n"
        "They look at you with yellow vertical-slit eyes that evaluate\n"
        "without judgment. This is a bureaucrat in a dimension where\n"
        "bureaucracy is enforced with energy blades.\n\n"
        "Say 'factions' to hear which clans are represented.\n"
        "Say 'join' to ask about joining a faction.\n"
        "Say 'amulet' to ask about the membership amulet."
    );
    set_level(6);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("kittani");
    set_alignment(0);

    add_response("hello",       "The administrator nods. 'State your interest.'");
    add_response("hi",          "The administrator nods. 'State your interest.'");
    add_response("factions",
        "The administrator recites without looking up: 'Splugorth Empire,\n"
        "  Kittani Serpent Clan, and Brodkil War Council are registered here.\n"
        "  Membership is not open to all races. Ask about each specifically.'");
    add_response("splugorth",
        "Kittani administrator says: 'Splugorth Empire membership is by invitation\n"
        "  only. Speak with a Splugorth authority. Do not waste their time or yours.'");
    add_response("kittani",
        "Kittani administrator says: 'Kittani Serpent Clan accepts Kittani by birth\n"
        "  and exceptional non-Kittani by demonstrated service. Service means years.'");
    add_response("brodkil",
        "Kittani administrator says: 'Brodkil War Council accepts Brodkil and strong\n"
        "  D-Bees who prove worth in the pits. Pits are east of the market.'");
    add_response("join",
        "Kittani administrator says: 'Membership applications are handled by Splugorth\n"
        "  administration. I record decisions. I do not make them.\n"
        "  Speak with a Splugorth authority about formal acceptance.'");
    add_response("amulet",
        "The administrator opens a drawer and holds up a flat metal disc on a\n"
        "  neck cord: 'The clan amulet. Faction name and sigil engraved on both\n"
        "  sides. Granted on formal acceptance. Not sold. Not transferred.\n"
        "  Wear it at your neck or not at all.'");
    add_response("membership",
        "Kittani administrator says: 'Membership grants faction access,\n"
        "  standing recognition at faction-aligned vendors, and the amulet.\n"
        "  It also grants enemies of your faction a reason to kill you.'");
    set_default_response(
        "The administrator glances up. 'If you are not here to join\n"
        "  or inquire, the exit is west.'");
    set_property("position_str", "sits at the clan desk, reviewing ledgers.");
}
