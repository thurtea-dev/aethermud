/* /domains/Horton/npcs/horton_hermit.c
   Quiet hermit living near the quarry rim. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Horton hermit");
    set_npc_name("the hermit");
    set_id( ({ "horton hermit", "hermit", "old man", "man", "human" }) );
    set_short("an old hermit");
    set_long(
        "An old human in layered coats, sitting on a crate with a kettle\n"
        "and a battered shotgun across his knees. He watches the quarry\n"
        "rim more than he watches you.\n"
    );

    set_race("human");
    set_level(8);
    set_max_hp(80);
    set_hp(80);
    set_aggressive(0);
    set_gender("male");
    set_alignment(200);
    set_body_type("humanoid");

    add_response("hello", "The hermit nods. 'Stay off the quarry floor after dark.'");
    add_response("hi", "The hermit nods. 'Stay off the quarry floor after dark.'");
    add_response("quarry", "The hermit says, 'Something nests down there. Takes scrap. Takes people.'");
    add_response("horton", "The hermit says, 'Good town. Soft edges. The woods are not soft.'");
    add_response("lurker", "The hermit says, 'If you hear glass-clicking on stone, climb out fast.'");
    set_default_response("The hermit shrugs. 'Ask about the quarry if you are curious.'");
    set_property("position_str", "sits here with a kettle and a shotgun.");
}
