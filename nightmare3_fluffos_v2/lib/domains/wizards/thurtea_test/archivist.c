/* /domains/wizards/thurtea_test/archivist.c
   Sela, testing archivist. Dialogue-pattern NPC with a full response
   set for testing conversation, default responses, and position text. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("Sela");
    set_npc_name("Sela");
    set_id( ({ "sela", "archivist", "testing archivist", "woman" }) );
    set_short("Sela, testing archivist");
    set_long(
        "A calm woman in an ink-stained coat, seated at a slate desk\n"
        "covered in bound test logs. She records the outcome of every\n"
        "trial run in this wing and answers questions about procedure.\n"
        "She responds to greetings and to questions about 'logs',\n"
        "'procedure', 'damage', 'armor', and 'spells'.\n");
    set_level(5);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(500);
    set_body_type("human");
    set_property("position_str", "sits at the slate desk, writing.");

    add_response("hello",
        "Sela says: Welcome back. The logs are current as of this morning.");
    add_response("hi",
        "Sela says: Welcome back. The logs are current as of this morning.");
    add_response("logs",
        "Sela says: Every trial in this wing is recorded: item worn, damage "
        "dealt, pools drained, anomalies noted. Ask about procedure if you "
        "want the standard sequence.");
    add_response("procedure",
        "Sela says: Wear the sample, verify the slot, take a measured hit in "
        "the arena, then check your pools. Armor first, barriers second, "
        "body last. Anything out of order is a defect.");
    add_response("damage",
        "Sela says: All damage lands on the pooled model now. Barrier "
        "fields drain first, then worn armor, then the body itself. Named "
        "limbs no longer take damage of their own.");
    add_response("armor",
        "Sela says: Protective suits cover the whole body. Clothing "
        "occupies cosmetic slots and stops nothing. The quartermaster "
        "stocks one sample of each.");
    add_response("spells",
        "Sela says: Armor of Ithan and its kin are depleting barriers. "
        "Cast one, take a hit, and watch the barrier absorb before your "
        "armor does.");
    add_response("bye",
        "Sela says: I will note the session closed.");
    set_default_response(
        "Sela says: That is not in my logs. Ask about logs, procedure, "
        "damage, armor, or spells.");
}
