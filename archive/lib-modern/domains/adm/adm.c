// /domains/adm/adm.c
// Administration domain master file.
// This domain is restricted to admin staff only.
// Contains: wiz_tools/ (staff objects distributed to wizards on promotion)

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_domain_name("Administration");
    set_property("light", 3);
    set_property("indoors", 1);
    set_short("Administration Domain");
    set_long(
        "The administration domain. Access is restricted to admin staff.\n"
        "This is not a navigable area of the game world.");
}
