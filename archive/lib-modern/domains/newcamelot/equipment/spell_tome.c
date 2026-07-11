/* /domains/newcamelot/equipment/spell_tome.c
   Chained study tome in the Royal Academy library. Readable flavor. */

#include <std.h>

inherit "/std/Object";

void create() {
    ::create();
    set_name("spell tome");
    set_id( ({ "spell tome", "tome", "book" }) );
    set_short("a chained spell tome");
    set_long(
        "A heavy tome bound in scaled leather and chained to the reading\n"
        "table. It lies open to a chapter on ley-line harmonics. The ink\n"
        "moves slightly if you watch it too closely.");
    set_mass(4000);
    set_value(0);
    set_property("no_drop", 1);
    set_read(
        "LEY LINES OF THE BRITISH ISLES, CH. VII:\n"
        "The triangulation of Stonehenge, Glastonbury and New Camelot\n"
        "yields the strongest nexus in Europe. It is the opinion of this\n"
        "author that the city was sited by someone who knew precisely\n"
        "what they were doing, long before they announced themselves.\n"
        "The student is invited to consider who benefited.\n");
}
