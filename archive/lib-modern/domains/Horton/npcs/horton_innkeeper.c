/* /domains/Horton/npcs/horton_innkeeper.c
   Horton innkeeper -- neutral, welcoming, has seen everything. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("innkeeper");
    set_npc_name("innkeeper");
    set_id( ({ "innkeeper", "barkeeper", "keeper", "host", "human", "woman",
               "horton innkeeper" }) );
    set_short("a broad-shouldered innkeeper");
    set_long(
        "She is broad-shouldered and calm, with hands that have worked hard\n"
        "for decades and eyes that have seen harder things. Her apron is clean.\n"
        "She carries nothing visible but you suspect that is not the whole story.\n"
        "She looks at you, assesses you in about half a second, and goes back to\n"
        "wiping the counter. No problem yet.");
    set_level(4);
    set_max_hp(70);
    set_hp(70);
    set_aggressive(0);
    set_moving(0);
    set_gender("female");
    set_race("human");
    set_alignment(150);
    set_body_type("human");

    add_response("hello",
        "The innkeeper says: 'Welcome to Horton. Room's available, food's adequate, "
        "trouble stays outside. Anything else you need?'");
    add_response("hi",
        "The innkeeper says: 'Welcome to Horton. Room's available, food's adequate, "
        "trouble stays outside. Anything else you need?'");
    add_response("room",
        "The innkeeper says: 'Fifty credits a night. Type rent room to get a key. "
        "We keep a bunk on for you if things go sideways out there.'");
    add_response("rent",
        "The innkeeper says: 'Fifty credits. Type rent room to set it up.'");
    add_response("price",
        "The innkeeper says: 'Room's fifty credits a night. Food costs extra, "
        "but it's real food.'");
    add_response("food",
        "The innkeeper says: 'Stew, bread, and preserved protein. Not fancy. "
        "Keeps you alive. Order from the bar.'");
    add_response("horton",
        "The innkeeper says: 'Good town. People take care of each other here. "
        "You will find that unusual. You will get used to it.'");
    add_response("forest",
        "The innkeeper says: 'North of town? People go in. Not all of them come back. "
        "Go with someone and go armed. Whatever is out there does not negotiate.'");
    add_response("danger",
        "The innkeeper says: 'North forest. East plains at night. West road after dark. "
        "Here in town you are fine. We look out for visitors.'");
    add_response("coalition",
        "The innkeeper says: 'CS stays out of Horton. We stay out of their business. "
        "That arrangement works for everyone.'");
    add_response("bye",
        "The innkeeper nods. 'Come back in one piece.'");
    set_default_response(
        "The innkeeper listens, wipes the counter once, and says: "
        "'I have heard stranger. What do you need?'");
}

void init() {
    ::init();
}
