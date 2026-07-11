/* /domains/Praxis/npcs/tattoo_master.c
   Neutral Atlantean tattoo master hiding in the Splynn catacombs.
   Informational NPC; not a shop. Tattoo grants are admin-only via addtattoo. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("tattoo master");
    set_id( ({ "tattoo master", "master", "atlantean", "man", "elder" }) );
    set_short("an Atlantean tattoo master");
    set_long(
        "An Atlantean elder of indeterminate age, bare-armed and bare-chested.\n"
        "Every visible surface of skin is covered in elaborate magical tattoos:\n"
        "swords, serpents, eyes, spheres, and geometric forms that seem to\n"
        "shift when not watched directly. His eyes are steady and measuring.\n"
        "He does not seem afraid of this place.\n"
    );
    set_gender("male");
    set_level(15);
    set_max_hp(200);
    set_hp(200);
    set_aggressive(0);
    set_property("no_steal",       1);
    set_property("tattoo_trainer", 1);
    set_property("immortal",       1);

    add_response("hello",
        "\"Tattoo magic is not ink. It is bound power, will, and pain.\"\n"
        "He does not look up from the pattern he is tracing on his forearm.\n");
    add_response("hi",
        "\"Tattoo magic is not ink. It is bound power, will, and pain.\"\n"
        "He does not look up from the pattern he is tracing on his forearm.\n");
    add_response("teach",
        "\"In time I may teach worthy Atlanteans new tattoos.\"\n"
        "He glances at your arms. Whatever he sees, he does not elaborate.\n");
    add_response("tattoos",
        "He extends one arm and points to three faded designs:\n"
        "  \"Weapon tattoos: a bound weapon that answers the call of PPE.\n"
        "   Protection tattoos: a force field inscribed into living skin.\n"
        "   Power tattoos: sight, light, healing, stealth -- each a gift\n"
        "   and a debt.\"\n"
        "He lowers his arm.\n"
        "  \"They do not run out. They do not rust. They do not break.\n"
        "   That is why the Splugorth want us.\"\n");
    add_response("tattoo",
        "He extends one arm and points to three faded designs:\n"
        "  \"Weapon tattoos: a bound weapon that answers the call of PPE.\n"
        "   Protection tattoos: a force field inscribed into living skin.\n"
        "   Power tattoos: sight, light, healing, stealth -- each a gift\n"
        "   and a debt.\"\n"
        "He lowers his arm.\n"
        "  \"They do not run out. They do not rust. They do not break.\n"
        "   That is why the Splugorth want us.\"\n");
    add_response("power",
        "\"PPE feeds every active tattoo. Weapon tattoos, armor tattoos, powers.\n"
        "Passive tattoos cost nothing -- they are permanent and always on.\n"
        "Manage your PPE and you manage your life.\"\n");
    add_response("ppe",
        "\"PPE feeds every active tattoo. Weapon tattoos, armor tattoos, powers.\n"
        "Passive tattoos cost nothing -- they are permanent and always on.\n"
        "Manage your PPE and you manage your life.\"\n");
    add_response("splugorth",
        "His expression goes very still.\n"
        "  \"We do not discuss them here. Not where their minions\n"
        "   still walk the street above us.\"\n");
    add_response("atlantis",
        "\"Atlantis was the first and will be the last. The Splugorth\n"
        "hold the land, not the memory. The memory lives in here.\"\n"
        "He touches the tattoos on his chest.\n");
    add_response("help",
        "\"Use 'tattoo list' to see what is inscribed on you.\n"
        "Use 'tattoo activate' with a name or number to invoke a tattoo.\n"
        "Use 'tattoo status' to see what is currently burning your PPE.\"\n");
}

/*
 * Override catch_tell to intercept clan-related keywords before the standard
 * response system handles them.  All other keywords fall through to the parent.
 * The aerihman check is intentionally conditional -- race and level gate it.
 * Do NOT add aerihman to add_response() or any public help text.
 */
void catch_tell(string str) {
    string a;
    string b;
    string race;
    object speaker;
    int lv;

    if(sscanf(str, "%s says: %s", a, b) != 2) {
        ::catch_tell(str);
        return;
    }
    b = lower_case(b);

    if(strsrch(b, "aerihman") != -1 ||
       strsrch(b, "shadow clan") != -1 ||
       strsrch(b, "the hidden clan") != -1) {

        speaker = find_living(lower_case(a));
        if(!speaker || !objectp(speaker)) {
            ::catch_tell(str);
            return;
        }

        race = lower_case((string)speaker->query_race());
        lv   = (int)speaker->query_level();

        if(strsrch(race, "atlantean") == -1) {
            /* Non-Atlantean: dismissal, no information */
            call_out("say_here", 1,
                "I do not know that name. You should leave.");
            return;
        }

        if(lv < 5) {
            /* Atlantean, too young: silence */
            return;
        }

        /* Atlantean, level 5+: first hint of the chain */
        call_out("say_here", 1,
            "That name is not spoken in the open. You have heard something\n"
            "you should not have. Come back when the stars are right --\n"
            "seek the mark of the eclipse.");
        speaker->set_property("heard_aerihman_hint", 1);
        catch(QUEST_D->start_quest(speaker, "aerihman_introduction"));
        return;
    }

    ::catch_tell(str);
}
