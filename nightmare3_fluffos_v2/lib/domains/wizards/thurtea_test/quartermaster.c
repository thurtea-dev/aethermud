/* /domains/wizards/thurtea_test/quartermaster.c
   Renn, testing quartermaster. Sells one representative item from each
   major equipment category and one item per cosmetic wear slot, at
   token prices, for staff wear/remove/damage testing.
   Commands: list, buy <item> */

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"
#define TW_PATH "/domains/wizards/thurtea_test/"

inherit "/std/rifts_vendor";

void create() {
    ::create();
    set_name("Renn");
    set_npc_name("Renn");
    set_id( ({ "renn", "quartermaster", "testing quartermaster",
               "vendor", "man" }) );
    set_short("Renn, testing quartermaster");
    set_long(
        "A wiry older man in a pressed supply-corps uniform with no unit\n"
        "insignia. He keeps the sample racks in strict order: one weapon,\n"
        "one suit of armor, one implant, and one garment for every wear\n"
        "slot on the standard body chart. Each tag lists a token price.\n"
        "Type 'list' to see the stock. Type 'buy <item>' to draw a sample.\n");
    set_level(5);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(300);
    set_body_type("human");
    set_property("position_str", "stands behind the supply counter.");

    add_response("hello", "Renn says: State your test and I will pull the samples.");
    add_response("hi", "Renn says: State your test and I will pull the samples.");
    add_response("stock", "Renn says: Type 'list' for the full inventory.");
    add_response("armor", "Renn says: The Dead Boy suit is the whole-body protective sample. The clothing covers the cosmetic slots.");
    add_response("slots", "Renn says: Cap, amulet, shirt, backpack, belt, trousers, gloves, boots, ring. One sample each.");
    add_response("cybernetics", "Renn says: The optic eye is the implant sample. Installation is a separate procedure.");
    add_response("bye", "Renn says: Log your results.");
    set_default_response("Renn straightens a tag on the nearest rack.");
    set_shop_sells(0);

    set_shop_items( ([
        /* Weapon sample */
        "rifle"         : ({ EQ_PATH+"c12_laser_rifle.c", 50,
                             "C-12 heavy assault laser rifle (weapon)" }),
        /* Whole-body protective armor sample (appearance override) */
        "dead boy"      : ({ EQ_PATH+"dead_boy_armor.c", 50,
                             "Coalition Dead Boy armor (protective, whole body)" }),
        /* Cybernetics sample */
        "optic eye"     : ({ EQ_PATH+"cybernetics/cyber_eye_optic.c", 50,
                             "cybernetic optic eye (implant)" }),
        /* Cosmetic slot samples, one per slot */
        "cap"           : ({ TW_PATH+"field_cap.c", 10,
                             "gray field cap (head slot)" }),
        "amulet"        : ({ EQ_PATH+"faction_amulet.c", 10,
                             "clan amulet (neck slot)" }),
        "shirt"         : ({ TW_PATH+"canvas_shirt.c", 10,
                             "canvas work shirt (shirt slot)" }),
        "backpack"      : ({ EQ_PATH+"backpack.c", 10,
                             "travel backpack (back slot)" }),
        "belt"          : ({ TW_PATH+"utility_belt.c", 10,
                             "utility belt (belt slot)" }),
        "trousers"      : ({ TW_PATH+"canvas_trousers.c", 10,
                             "canvas trousers (legs slot)" }),
        "gloves"        : ({ TW_PATH+"work_gloves.c", 10,
                             "work gloves (hands slot)" }),
        "boots"         : ({ TW_PATH+"travel_boots.c", 10,
                             "travel boots (feet slot)" }),
        "ring"          : ({ TW_PATH+"signet_ring.c", 10,
                             "steel signet ring (ring slot)" }),
        /* Environmental gear sample */
        "rebreather"    : ({ EQ_PATH+"rebreather.c", 10,
                             "rebreather mask (underwater access)" })
    ]) );
}
