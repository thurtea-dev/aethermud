// /d/Praxis/equipment/power_armor/GB_Glitter_Boy.c
// Glitter Boy power armor -- iconic pre-Rifts MDC suit
// Main body: 400 MDC | Head: 100 | Arms: 100 ea | Legs: 150 ea

inherit "/std/armour";

void create() {
    ::create();
    set_name("Glitter Boy power armor");
    set_id( ({ "glitter boy", "glitterboy", "gb", "power armor", "armor",
               "chromium armor", "glitter boy suit" }) );
    set_short("a Glitter Boy power armor suit");
    set_long(
        "Eight feet of polished chromium-alloy power armor. The Glitter Boy\n"
        "reflects light like a mirror, blinding in direct sun. The boom gun\n"
        "assembly folds across the right shoulder. Pre-Rifts American military\n"
        "hardware, one of the most powerful suits ever made.\n"
        "MDC: Main body 400 | Head 100 | Arms 100 each | Legs 150 each.\n"
        "AR: 20. Ground speed: 40 mph. Cannot fly. Cannot swim.\n"
        "Note: -5 APM without the Glitter Boy Pilot OCC.");
    set_mass(22700);
    set_value(0);
    set_type("body armour");
    set_limbs( ({ "torso", "left arm", "right arm", "left leg", "right leg" }) );
    set_ac(14);
    set_property("rifts_slot", "armor");
    set_property("mdc_armor", 1);
    set_property("armor_mdc", 400);
    set_property("current_armor_mdc", 400);
    set_property("armor_ar", 20);
    set_property("power_armor", 1);
    set_property("environmental", 1);
    set_property("flight_capable", 0);
    set_property("ps_bonus", 30);
    set_property("spd_bonus", 40);
    set_property("boom_gun_mount", 1);
    set_property("skill_penalty_physical", -20);
    set_property("no_swim", 1);
    set_property("unique_item", 1);
}

private int wearer_is_gb_pilot(object who) {
    string occ;

    if(!who) return 0;
    occ = lower_case((string)who->getenv("rifts_occ"));
    if(!occ || !sizeof(occ))
        occ = lower_case((string)who->query_property("rifts_occ"));
    return (strsrch(occ, "glitter boy") != -1);
}

int wear(string str) {
    int result;
    object who;
    int spd_bonus;
    result = ::wear(str);
    who = query_worn();
    if(!who) return result;
    if(!wearer_is_gb_pilot(who))
        tell_object(who,
            "You lack Glitter Boy Pilot training. The suit responds sluggishly.\n");
    who->set_property("power_armor_ps", (int)query_property("ps_bonus"));
    spd_bonus = (int)query_property("spd_bonus");
    if(spd_bonus) who->set_property("power_armor_spd", spd_bonus);
    tell_object(who,
        "The Glitter Boy power armor systems come online. "
        "Your strength and mobility increase dramatically. "
        "The chromium shell reflects light blindingly.\n");
    return result;
}

void unwear() {
    object who;
    who = query_worn();
    ::unwear();
    if(!objectp(who)) return;
    who->set_property("power_armor_ps", 0);
    who->set_property("power_armor_spd", 0);
    tell_object(who, "The Glitter Boy power armor powers down.\n");
}
