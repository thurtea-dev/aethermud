// /domains/Praxis/equipment/power_armor/CS_Terror_Trooper.c
// Coalition Terror Trooper power armor.

inherit "/std/armour";

void create() {
    ::create();
    set_name("Terror Trooper armor");
    set_id(({ "terror trooper", "terror trooper armor", "cs terror trooper",
               "power armor", "armor", "tt armor" }));
    set_short("a CS Terror Trooper power armor suit");
    set_long(
        "Heavy Coalition assault power armor in skull-black plate. Built for\n"
        "urban shock operations. Integrated vibro-claws and targeting HUD.\n"
        "MDC: Main body 250 | Head 100 | Arms 90 each | Legs 110 each.\n"
        "AR: 19. Requires Pilot Power Armor skill.");
    set_mass(12000);
    set_value(0);
    set_type("body armour");
    set_limbs(({ "torso", "left arm", "right arm", "left leg", "right leg" }));
    set_ac(12);
    set_property("rifts_slot", "armor");
    set_property("mdc_armor", 1);
    set_property("armor_mdc", 250);
    set_property("current_armor_mdc", 250);
    set_property("armor_ar", 19);
    set_property("power_armor", 1);
    set_property("environmental", 1);
    set_property("ps_bonus", 15);
    set_property("skill_penalty_prowl", -15);
}

int wear(string str) {
    int result;
    object who;

    result = ::wear(str);
    who = query_worn();
    if(!who) return result;
    if((int)who->query_skill("pilot power armor") <= 0 &&
       (int)who->query_skill("pilot robot") <= 0)
        tell_object(who,
            "You lack power armor training. Systems respond sluggishly.\n");
    who->set_property("power_armor_ps", (int)query_property("ps_bonus"));
    tell_object(who, "Terror Trooper systems online.\n");
    return result;
}

void unwear() {
    object who;

    who = query_worn();
    ::unwear();
    if(!objectp(who)) return;
    who->set_property("power_armor_ps", 0);
    tell_object(who, "Terror Trooper armor powers down.\n");
}
