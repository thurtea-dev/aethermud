// /domains/tolkeen/areas/tolkeen_market.c

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set_property("tolkeen", 1);
    set_property("domain_stub", 1);
    set_short("Tolkeen, Ward Market");
    set_long(
        "Stalls sell spell components, MDC scrap, and Coalition trophies\n"
        "repurposed into tools. A techno-wizard watches the crowd from a\n"
        "raised platform, ready to dampen hostile magic.");
    set_exits( ([
        "west" : "/domains/tolkeen/areas/tolkeen_square"
    ]) );
}
