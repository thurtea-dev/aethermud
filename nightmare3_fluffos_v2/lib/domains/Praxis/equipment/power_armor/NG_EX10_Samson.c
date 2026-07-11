// /d/Praxis/equipment/power_armor/NG_EX10_Samson.c
// Northern Gun Samson power armor -- commercial MDC suit
// Main body: 160 MDC | Head: 70 | Arms: 65 ea | Legs: 80 ea

inherit "/std/armour";

void create() {
    ::create();
    set_name("Northern Gun Samson power armor");
    set_id( ({ "samson", "ng samson", "ng-ex10", "ngex10", "northern gun samson",
               "power armor", "armor", "samson suit" }) );
    set_short("a Northern Gun Samson power armor");
    set_long(
        "A commercially available power armor from Northern Gun. Bulkier and\n"
        "less refined than military suits but available to civilians with\n"
        "enough credits. Olive drab finish, humanoid design, reliable.\n"
        "MDC: Main body 160 | Head 70 | Arms 65 each | Legs 80 each.\n"
        "AR: 17. Ground speed: 50 mph. Flight: not capable.\n"
        "Available for purchase -- one of the few power armors civilians can own.");
    set_mass(13600);
    set_value(180000);
    set_type("body armour");
    set_limbs( ({ "torso", "left arm", "right arm", "left leg", "right leg" }) );
    set_ac(11);
    set_property("rifts_slot", "armor");
    set_property("mdc_armor", 1);
    set_property("armor_mdc", 160);
    set_property("current_armor_mdc", 160);
    set_property("armor_ar", 17);
    set_property("power_armor", 1);
    set_property("environmental", 1);
    set_property("flight_capable", 0);
    set_property("ps_bonus", 15);
    set_property("spd_bonus", 20);
}

int wear(string str) {
    int result;
    object who;
    int spd_bonus;
    result = ::wear(str);
    who = query_worn();
    if(!who) return result;
    who->set_property("power_armor_ps", (int)query_property("ps_bonus"));
    spd_bonus = (int)query_property("spd_bonus");
    if(spd_bonus) who->add_spd(spd_bonus);
    tell_object(who,
        "The Samson power armor systems come online. "
        "Your strength and mobility increase dramatically.\n");
    return result;
}

void unwear() {
    object who;
    int spd_bonus;
    who = query_worn();
    spd_bonus = (int)query_property("spd_bonus");
    ::unwear();
    if(!objectp(who)) return;
    who->set_property("power_armor_ps", 0);
    if(spd_bonus) who->add_spd(-spd_bonus);
    tell_object(who, "The Samson power armor powers down.\n");
}
