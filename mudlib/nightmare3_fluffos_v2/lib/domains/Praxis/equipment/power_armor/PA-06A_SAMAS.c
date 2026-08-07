// /d/Praxis/equipment/power_armor/PA-06A_SAMAS.c
// Coalition SAMAS (Strategic Armor Military Assault Suit) -- MDC power armor
// Main body: 200 MDC | Head: 90 | Arms: 85 ea | Legs: 100 ea | Wings: 90 ea

inherit "/std/armour";

void create() {
    ::create();
    set_name("SAMAS power armor");
    set_id( ({ "samas", "pa-06a", "coalition samas", "power armor", "armor",
               "samas suit", "coalition power armor" }) );
    set_short("a Coalition SAMAS power armor suit");
    set_long(
        "A sleek humanoid power armor suit in Coalition black. The SAMAS\n"
        "(Strategic Armor Military Assault Suit) is the backbone of Coalition\n"
        "air cavalry. Jet-assisted flight, radar, and targeting systems are\n"
        "built in. The skull-face helmet is standard Coalition intimidation design.\n"
        "MDC: Main body 200 | Head 90 | Arms 85 each | Legs 100 each | Wings 90 each.\n"
        "AR: 18. Flight speed: 300 mph. Radar range: 20 miles.");
    set_mass(11350);
    set_value(0);
    set_type("body armour");
    set_limbs( ({ "torso", "left arm", "right arm", "left leg", "right leg" }) );
    set_ac(12);
    set_property("rifts_slot", "armor");
    set_property("mdc_armor", 1);
    set_property("armor_mdc", 200);
    set_property("current_armor_mdc", 200);
    set_property("armor_ar", 18);
    set_property("power_armor", 1);
    set_property("environmental", 1);
    set_property("flight_capable", 1);
    set_property("flight_speed", 300);
    set_property("radar_range", 20);
    set_property("ps_bonus", 20);
    set_property("skill_penalty_prowl", -10);
    set_property("skill_penalty_climb", -10);
    set_property("skill_penalty_swim", -10);
}

private int wearer_is_samas_pilot(object who) {
    string occ;

    if(!who) return 0;
    occ = lower_case((string)who->getenv("rifts_occ"));
    if(!occ || !sizeof(occ))
        occ = lower_case((string)who->query_property("rifts_occ"));
    return (strsrch(occ, "samas") != -1);
}

int wear(string str) {
    int result;
    object who;
    result = ::wear(str);
    who = query_worn();
    if(!who) return result;
    if(!wearer_is_samas_pilot(who))
        tell_object(who,
            "You lack SAMAS pilot training. Flight systems respond sluggishly.\n");
    who->set_property("power_armor_ps", (int)query_property("ps_bonus"));
    who->set_property("flight_active", 1);
    tell_object(who,
        "The SAMAS power armor systems come online. "
        "Your strength increases dramatically. Flight systems ready.\n");
    return result;
}

void unwear() {
    object who;
    who = query_worn();
    ::unwear();
    if(!objectp(who)) return;
    who->set_property("power_armor_ps", 0);
    who->set_property("flight_active", 0);
    tell_object(who, "The SAMAS power armor powers down.\n");
}
