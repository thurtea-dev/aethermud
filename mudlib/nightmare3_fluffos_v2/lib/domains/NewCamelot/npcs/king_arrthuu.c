/* /domains/NewCamelot/npcs/king_arrthuu.c
   King Arrr'thuu of New Camelot. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("King Arrr'thuu");
    set_npc_name("King Arrr'thuu");
    set_id( ({ "king arrr'thuu", "king arrrthuu", "king", "arrr'thuu", "arrrthuu" }) );
    set_short("King Arrr'thuu of New Camelot");
    set_long(
        "The King of New Camelot sits the white throne with effortless\n"
        "grace. He is tall, golden-haired, and impossibly composed; his eyes\n"
        "are an old, deep blue that has watched the realm rise from ruin.\n"
        "Something about his presence fills the room, warm as sunlight and\n"
        "just as hard to look at directly. If you stare too long, the warmth\n"
        "starts to feel like the surface of something else.");
    set_level(30);
    set_max_hp(2000);
    set_hp(2000);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(300);
    set_body_type("human");
    add_response("hello",
        "King Arrr'thuu says: 'Be welcome in New Camelot, traveler. The realm is open to all of good intent.'");
    add_response("camelot",
        "King Arrr'thuu says: 'We raised this city from the ruin of the old world. It will not fall while I sit this throne.'");
    add_response("mrrlyn",
        "King Arrr'thuu says: 'My counselor sees far. Farther than most would care to.'");
    add_response("dragons",
        "King Arrr'thuu says: 'The dragons are our sworn kin. Their pact is with me personally.'");
    add_response("bye",
        "King Arrr'thuu inclines his head a precise, regal degree.");

    set_property("position_str", "sits upon the white throne.");
    set_default_response("King Arrr'thuu watches the room with ancient, patient eyes.");
}
