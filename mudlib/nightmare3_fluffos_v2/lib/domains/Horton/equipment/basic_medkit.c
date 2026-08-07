/* /domains/Horton/equipment/basic_medkit.c
   Basic medical kit -- field-grade first aid. Restores 2d6 SDC. One use. */

#include <std.h>
#include <daemons.h>

inherit "/std/Object";

void create() {
    ::create();
    set_name("medkit");
    set_id( ({ "medkit", "basic medkit", "medical kit", "first aid", "kit" }) );
    set_short("a basic medkit");
    set_long(
        "A green plastic case stenciled FIRST AID. Inside: bandages, antiseptic\n"
        "spray, a suture kit, and a small bottle of painkillers. Standard field\n"
        "grade. It will close a wound but not regrow a limb.\n"
        "Type 'use medkit' to apply it.");
    set_weight(20);
    set_value(100);
}

void init() {
    ::init();
    if(environment(this_object()) == this_player())
        add_action("cmd_use", "use");
}

int cmd_use(string str) {
    int heal, cur_sdc, max_sdc;

    if(!str || (strsrch(lower_case(str), "medkit") == -1 &&
                strsrch(lower_case(str), "kit") == -1 &&
                strsrch(lower_case(str), "med") == -1))
        return 0;

    if(environment(this_object()) != this_player()) {
        write("You need to be carrying the medkit to use it.\n");
        return 1;
    }

    heal    = random(6) + random(6) + 2;
    cur_sdc = (int)this_player()->query_stats("SDC");
    max_sdc = (int)this_player()->query_stats("max_SDC");
    cur_sdc += heal;
    if(cur_sdc > max_sdc) cur_sdc = max_sdc;
    this_player()->set_stats("SDC", cur_sdc);

    write("You apply the medkit. Bandages and antiseptic spray. +" + heal + " SDC.\n");
    say((string)this_player()->query_cap_name() + " treats their wounds with a medkit.",
        this_player());
    remove();
    return 1;
}
