// /d/Praxis/equipment/rations.c
// Field rations -- restores 10 SDC when eaten, destroys on use.

inherit "/std/Object";

void create() {
    ::create();
    set_name("field rations");
    set_id( ({ "field rations", "rations", "ration", "food", "meal" }) );
    set_short("field rations");
    set_long(
        "A vacuum-sealed foil pack of high-calorie survival rations. Tastes\n"
        "like compressed chalk, but it keeps you going. Eating these takes\n"
        "about five minutes and restores minor wounds. +10 SDC.");
    set_mass(150);
    set_value(50);
    set_vendor_type("food");
    set_property("single_use", 1);
}

void init() {
    ::init();
    if(environment(this_object()) == this_player())
        add_action("cmd_eat", "eat");
}

int cmd_eat(string str) {
    int cur_sdc, max_sdc, heal_amt;

    if(!str || (strsrch(lower_case(str), "ration") == -1 &&
                strsrch(lower_case(str), "food") == -1 &&
                strsrch(lower_case(str), "meal") == -1)) return 0;
    if(present(lower_case(str), this_player()) != this_object()) return 0;
    if((object)this_player()->query_current_attacker()) {
        write("You cannot eat during combat!\n");
        return 1;
    }
    cur_sdc = (int)this_player()->query_stats("SDC");
    max_sdc = (int)this_player()->query_stats("max_SDC");
    heal_amt = 10;
    if(cur_sdc + heal_amt > max_sdc) heal_amt = max_sdc - cur_sdc;
    this_player()->set_stats("SDC", cur_sdc + heal_amt);
    write("You eat the rations slowly. "+heal_amt+" SDC restored.\n");
    say(this_player()->query_cap_name()+" eats some field rations.\n", this_player());
    this_object()->remove();
    if(this_object()) destruct(this_object());
    return 1;
}
