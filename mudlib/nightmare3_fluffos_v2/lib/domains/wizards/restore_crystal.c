/* /domains/wizards/restore_crystal.c
   Restoration crystal in the combat testing zone.
   Touch it to fully restore HP, MDC, ISP, and PPE. Wizard use only. */

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("restoration crystal");
    set_id( ({ "restoration crystal", "crystal", "restore crystal",
               "pedestal", "crystal pedestal" }) );
    set_short("a restoration crystal");
    set_long(
        "A large hexagonal crystal mounted on a reinforced pedestal. It glows\n"
        "with a steady blue-white light, radiating a faint warmth. Touch the\n"
        "crystal to restore all HP, MDC, ISP, and PPE to maximum.\n"
    );
    set_mass(2000);
    set_value(0);
    set_property("no_drop", 1);
    set_property("no_get", 1);
}

void init() {
    ::init();
    add_action("cmd_touch_crystal", "touch");
    add_action("cmd_touch_crystal", "use");
}

int cmd_touch_crystal(string str) {
    int max_hp;
    int max_mdc;
    int max_isp;
    int max_ppe;
    int max_rifts_hp;

    if(!str) return 0;
    str = lower_case(str);
    if(strsrch(str, "crystal") == -1 && strsrch(str, "pedestal") == -1)
        return 0;
    if(!creatorp(this_player())) {
        write("The crystal does not respond to you.\n");
        return 1;
    }
    max_hp = (int)this_player()->query_max_hp();
    if(max_hp > 0) this_player()->set_hp(max_hp);
    max_rifts_hp = (int)this_player()->query_stats("max_rifts_hp");
    if(max_rifts_hp > 0) this_player()->set_stats("rifts_hp", max_rifts_hp);
    max_mdc = (int)this_player()->query_stats("max_MDC");
    if(max_mdc > 0) this_player()->set_stats("MDC", max_mdc);
    max_isp = (int)this_player()->query_stats("max_ISP");
    if(max_isp > 0) this_player()->set_stats("ISP", max_isp);
    max_ppe = (int)this_player()->query_stats("max_PPE");
    if(max_ppe > 0) this_player()->set_stats("PPE", max_ppe);
    write("The crystal flares with healing light. All stats restored to maximum.\n");
    return 1;
}
