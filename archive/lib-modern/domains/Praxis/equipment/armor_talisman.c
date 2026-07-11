// /domains/Praxis/equipment/armor_talisman.c
// Magical armor talisman earned from Rocky's Bar mini-quest.
// Absorbs incoming MDC from inventory. Rechargeable with PPE.

#include <std.h>
#include <daemons.h>

inherit OBJECT;

private int __talisman_mdc;

void create() {
    ::create();
    set_name("talisman");
    set_id( ({ "talisman", "armor talisman", "disc", "obsidian disc" }) );
    set_short("a glowing armor talisman");
    set_long(
        "A flat disc of polished obsidian etched with magical runes. It pulses\n"
        "faintly with stored PPE energy. The runes seem to shift when you look\n"
        "directly at them. It does not need to be worn. Its field absorbs MDC\n"
        "damage while carried. Type 'activate talisman' to recharge it with PPE.\n"
    );
    set_mass(50);
    set_property("is_talisman", 1);
    __talisman_mdc = 100;
}

int query_talisman_mdc() { return __talisman_mdc; }
void set_talisman_mdc(int v) { __talisman_mdc = (v < 0) ? 0 : v; }

int absorb_damage(object bearer, int dmg) {
    string race;
    int absorbed;

    race = lower_case((string)bearer->query_race());
    if(strsrch(race, "hawrk-duhk") != -1 || strsrch(race, "hawrk-ka") != -1)
        return 0;
    if(__talisman_mdc <= 0) return 0;
    absorbed = (__talisman_mdc >= dmg) ? dmg : __talisman_mdc;
    __talisman_mdc -= absorbed;
    tell_object(bearer, "The talisman absorbs " + absorbed + " MDC. " +
        __talisman_mdc + " MDC remaining.\n");
    return absorbed;
}

int cmd_activate(string str) {
    object env;
    int ppe;
    int max_ppe;
    string race;

    if(!str || strsrch(lower_case(str), "talisman") == -1) return 0;
    env = environment(this_object());
    if(!env || !living(env)) {
        write("You are not carrying the talisman.\n");
        return 1;
    }
    race = lower_case((string)env->query_race());
    if(strsrch(race, "hawrk-duhk") != -1 || strsrch(race, "hawrk-ka") != -1) {
        tell_object(env,
            "The talisman's runes flicker and go cold at your touch. "
            "It was not made for your kind.\n");
        return 1;
    }
    ppe = (int)env->query_stats("PPE");
    max_ppe = (int)env->query_stats("max_PPE");
    if(max_ppe <= 0) {
        tell_object(env, "You have no PPE to channel into the talisman.\n");
        return 1;
    }
    if(ppe < 20) {
        tell_object(env, "You need at least 20 PPE to recharge the talisman.\n");
        return 1;
    }
    env->set_stats("PPE", ppe - 20);
    __talisman_mdc = 100;
    tell_object(env,
        "You channel 20 PPE into the talisman. The runes blaze. Talisman MDC: 100.\n");
    return 1;
}

void init() {
    ::init();
    add_action("cmd_activate", "activate");
}
