// /cmds/mortal/_breath.c
// Racial breath weapon: breath [target]

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private object find_target(object caster, string str) {
    object *inv;
    object *atk;
    int i;

    if(str && strlen(str))
        return present(lower_case(str), environment(caster));
    atk = (object *)caster->query_attackers();
    if(atk && sizeof(atk)) return atk[0];
    inv = all_inventory(environment(caster));
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || !living(inv[i]) || inv[i] == caster) continue;
        atk = (object *)inv[i]->query_attackers();
        if(atk && member_array(caster, atk) != -1) return inv[i];
    }
    return 0;
}

int cmd_breath(string str) {
    object caster;
    object target;
    string btype;
    int dmg;
    string msg;

    caster = this_player();
    if(!caster) return 0;
    btype = (string)RIFTS_D->query_breath_type(caster);
    if(!btype || !strlen(btype)) {
        return notify_fail("You do not have a breath weapon.\n");
    }
    if((int)caster->query_property("breath_cooldown") > time()) {
        write("You need to catch your breath first.\n");
        return 1;
    }
    target = find_target(caster, str);
    if(!target) {
        write("Breath at whom?  Syntax: breath <target>\n");
        return 1;
    }
    if(environment(target) != environment(caster)) {
        write("They are not here.\n");
        return 1;
    }
    dmg = (random(6) + 1 + random(6) + 1) * 5;
    switch(btype) {
    case "fire":    msg = "A cone of dragonfire"; break;
    case "ice":     msg = "A blast of freezing breath"; break;
    case "acid":    msg = "A gout of corrosive acid"; break;
    default:        msg = "A crackling bolt of lightning"; break;
    }
    caster->set_property("breath_cooldown", time() + 30);
    write("You unleash your " + btype + " breath at " +
        (string)target->query_cap_name() + "!\n");
    tell_object(target, (string)caster->query_cap_name() +
        " breathes " + btype + " at you!\n");
    caster->tell_room_living(environment(caster), caster, target,
        " unleashes a " + btype + " breath attack!\n");
    caster->set_property("last_attack_element", btype);
    RIFTS_COMBAT_D->apply_rifts_damage(caster, target, dmg);
    caster->remove_property("last_attack_element");
    return 1;
}

void help() {
    write("Syntax: breath [target]\n\n"
        "Racial breath weapon for dragons and similar beings.\n"
        "30 second cooldown. Auto-targets your attacker in combat.\n");
}
