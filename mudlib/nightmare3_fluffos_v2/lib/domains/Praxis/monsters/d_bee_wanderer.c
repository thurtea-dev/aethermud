// /d/Praxis/monsters/d_bee_wanderer.c
// D-Bee wanderer -- a dimensional being passing through, neutral.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("D-Bee wanderer");
    set_npc_name("D-Bee");
    set_id( ({ "d-bee wanderer", "d-bee", "wanderer", "alien", "creature" }) );
    set_short("a D-Bee wanderer");
    set_long(
        "A humanoid of uncertain species. Roughly human height but wrong in\n"
        "the details: skin with a faint blue cast, fingers one joint too many,\n"
        "eyes that are solid amber without whites or pupils. It wears salvaged\n"
        "travelling gear and carries a pack. It does not appear hostile.\n"
        "On Rifts Earth, 'dimensional being' covers a very wide range.");

    set_level(3);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_moving(1);
    set_race("d-bee");
    set_alignment(0);
    set_body_type("humanoid");

    add_response("hello",
        "D-Bee says: 'Greetings, local. Your road markings are confusing but I managed.'");
    add_response("hi",
        "D-Bee says: 'Hello. I am passing through. Is Lazlo far from here?'");
    add_response("lazlo",
        "D-Bee says: 'I heard it is a place where my kind are not shot on sight. I am interested in this.'");
    add_response("coalition",
        "D-Bee says: 'I know of them. I avoid their territory. This seems wise.'");
    add_response("where",
        "D-Bee says: 'I came through a rift three days ago. I do not know the specific origin point.'");
    add_response("help",
        "D-Bee says: 'I need no help. I travel well alone. Thank you for asking.'");
    add_response("bye",
        "D-Bee says: 'Safe travels to you as well. Watch the roads after dark.'");
    set_default_response("The D-Bee looks at you with amber eyes and tilts its head slightly.");

    setenv("rifts_race", "d-bee");
}
