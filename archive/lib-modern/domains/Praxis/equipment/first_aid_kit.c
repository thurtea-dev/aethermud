// /d/Praxis/equipment/first_aid_kit.c
// Standard first aid kit -- heals 1d6 SDC, single use.

inherit "/std/Object";

void create() {
    ::create();
    set_name("first aid kit");
    set_id( ({ "first aid kit", "first aid", "aid kit", "kit", "medkit" }) );
    set_short("a first aid kit");
    set_long(
        "A battered plastic case stenciled FIRST AID. Inside: bandages,\n"
        "antiseptic, and field dressings. Not much against mega-damage but\n"
        "enough to patch up ordinary wounds. Single use. Heals 1d6 SDC.");
    set_mass(250);
    set_value(200);
    set_vendor_type("medical");
    set_property("single_use", 1);
}

void init() {
    ::init();
    if(environment(this_object()) == this_player())
        add_action("cmd_use", "use");
}

int cmd_use(string str) {
    int heal_amt;
    int cur_sdc, max_sdc;

    if(!str || (strsrch(lower_case(str), "first aid") == -1 &&
                strsrch(lower_case(str), "aid kit") == -1 &&
                strsrch(lower_case(str), "kit") == -1 &&
                strsrch(lower_case(str), "medkit") == -1)) return 0;
    if(present(lower_case(str), this_player()) != this_object()) return 0;

    heal_amt = 1 + random(6);
    cur_sdc = (int)this_player()->query_stats("SDC");
    max_sdc = (int)this_player()->query_stats("max_SDC");
    if(cur_sdc >= max_sdc) {
        write("You are already at full health.\n");
        return 1;
    }
    if(cur_sdc + heal_amt > max_sdc) heal_amt = max_sdc - cur_sdc;
    this_player()->set_stats("SDC", cur_sdc + heal_amt);
    write("You apply the field dressings. "+heal_amt+" SDC restored.\n");
    say(this_player()->query_cap_name()+" patches up their wounds.\n", this_player());
    this_object()->remove();
    if(this_object()) destruct(this_object());
    return 1;
}
