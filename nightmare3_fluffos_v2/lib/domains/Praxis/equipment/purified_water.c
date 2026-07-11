// /d/Praxis/equipment/purified_water.c
// Purified water -- restores 5 SDC when consumed, destroys on use.

inherit "/std/Object";

void create() {
    ::create();
    set_name("purified water");
    set_id( ({ "purified water", "water", "canteen", "drink" }) );
    set_short("purified water");
    set_long(
        "A sealed plastic bottle of purified water. Post-Rifts, clean\n"
        "water is worth more than it looks. Restores minor wounds when\n"
        "consumed. +5 SDC.");
    set_mass(200);
    set_value(20);
    set_vendor_type("food");
    set_property("single_use", 1);
}

void init() {
    ::init();
    if(environment(this_object()) == this_player())
        add_action("cmd_drink", "drink");
}

int cmd_drink(string str) {
    int cur_sdc, max_sdc, heal_amt;

    if(!str || (strsrch(lower_case(str), "water") == -1 &&
                strsrch(lower_case(str), "drink") == -1 &&
                strsrch(lower_case(str), "canteen") == -1)) return 0;
    if(present(lower_case(str), this_player()) != this_object()) return 0;
    if((object)this_player()->query_current_attacker()) {
        write("You cannot drink during combat!\n");
        return 1;
    }
    cur_sdc = (int)this_player()->query_stats("SDC");
    max_sdc = (int)this_player()->query_stats("max_SDC");
    heal_amt = 5;
    if(cur_sdc + heal_amt > max_sdc) heal_amt = max_sdc - cur_sdc;
    this_player()->set_stats("SDC", cur_sdc + heal_amt);
    write("You drink the water. " + heal_amt + " SDC restored.\n");
    this_object()->remove();
    if(this_object()) destruct(this_object());
    return 1;
}
