/* /domains/NGR/npcs/ngr_civilian.c
   an NGR civilian. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();
    set_name("ngr civilian");
    set_npc_name("Civilian");
    set_id( ({ "ngr civilian", "civilian", "resident" }) );
    set_short("an NGR civilian");
    set_long(
        "A tired, well-fed woman in worker's coveralls with a Triax plant\n"
        "badge clipped to the pocket. She has the particular calm of\n"
        "someone whose city has never actually fallen, whatever the sirens\n"
        "said.");
    set_level(2);
    set_max_hp(40);
    set_hp(40);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(300);
    set_body_type("human");
    add_response("hello",
        "Civilian says: 'New face. Papers in order? The guards do check.'");
    add_response("gargoyles",
        "Civilian says: 'The wall holds. My grandfather built his section of it. It holds.'");
    add_response("bye",
        "Civilian says: 'Keep off the grass. They fine you.'");
    set_default_response("The civilian waters a windowbox with great seriousness.");
}
