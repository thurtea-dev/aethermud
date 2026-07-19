/* /domains/wizards/thurtea_test/test_hall.c
   Staff-only equipment testing hall off Thurtea's workroom.
   Mortals are ejected on entry; see init(). */

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_properties( ([
        "light" : 3,
        "indoors" : 1,
        "no teleport" : 1,
        "no castle" : 1
    ]) );
    set_short("Equipment testing hall");
    set_long(
        "A long hall with reinforced walls and steady white lighting. Racks\n"
        "of sample equipment line one side: weapons, armor, and clothing in\n"
        "every wear slot, each tagged for testing rather than sale. A supply\n"
        "counter stands at the far end, and a heavy door leads to a sparring\n"
        "arena. Warding sigils around the entrance keep this wing sealed to\n"
        "anyone below staff rank.");
    set_smell("default", "Gun oil and fresh fabric.");
    set_listen("default", "Muffled impacts carry through the arena door.");
    set_exits( ([
        "out"   : "/realms/thurtea/workroom",
        "arena" : "/domains/wizards/thurtea_test/test_arena"
    ]) );
}

void init() {
    ::init();
    /* Staff-only wing: bounce any mortal player however they arrived
       (walking, summon, admin move). NPCs and creators pass. */
    if(this_player() && userp(this_player()) &&
       !creatorp(this_player())) {
        write("A containment ward flares white. This wing is sealed to "
              "unauthorized visitors.\n");
        this_player()->move_player(ROOM_START);
    }
}

void reset() {
    ::reset();
    if(!present("quartermaster", this_object()))
        clone_object("/domains/wizards/thurtea_test/quartermaster")
            ->move(this_object());
    if(!present("archivist", this_object()))
        clone_object("/domains/wizards/thurtea_test/archivist")
            ->move(this_object());
}
