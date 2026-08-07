/* /domains/wizards/thurtea_test/test_arena.c
   Staff-only sparring arena: one legacy NM3-stat target and one
   Rifts MDC-stat target for damage-model testing. */

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
    set_short("Sparring arena");
    set_long(
        "A circular arena floored with scarred ferrocrete. Scorch marks and\n"
        "blade gouges record past testing sessions. Two practice targets\n"
        "wait here at all times: a salvaged sparring drone with ordinary\n"
        "plating, and a leashed rift crawler whose hide turns aside all but\n"
        "mega-damage weapons. Both are replaced whenever destroyed.");
    set_smell("default", "Ozone and scorched ferrocrete.");
    set_listen("default", "The rift crawler's claws click against the floor.");
    set_exits( ([
        "hall" : "/domains/wizards/thurtea_test/test_hall"
    ]) );
}

void init() {
    ::init();
    /* Staff-only wing: bounce any mortal player however they arrived. */
    if(this_player() && userp(this_player()) &&
       !creatorp(this_player())) {
        write("A containment ward flares white. This wing is sealed to "
              "unauthorized visitors.\n");
        this_player()->move_player(ROOM_START);
    }
}

void reset() {
    ::reset();
    if(!present("sparring drone", this_object()))
        clone_object("/domains/wizards/thurtea_test/sparring_drone")
            ->move(this_object());
    if(!present("rift crawler", this_object()))
        clone_object("/domains/wizards/thurtea_test/rift_crawler")
            ->move(this_object());
}
