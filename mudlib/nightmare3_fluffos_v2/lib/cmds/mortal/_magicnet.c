// /cmds/mortal/_magicnet.c
// Shortcut for magic net: magicnet [target]

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private object find_combat_target(object caster) {
    object *inv;
    object *atk;
    int i;

    if(!caster) return 0;
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

int cmd_magicnet(string str) {
    object target;
    string cast_str;

    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race()))
        return notify_fail("You don't have the ability to cast spells.\n");
    if(!(int)RIFTS_D->player_knows_spell(this_player(), "magic net"))
        return notify_fail("You do not know the magic net spell.\n");

    if(str && strlen(str)) {
        cast_str = "magic net at " + str;
        return (int)"/cmds/mortal/_cast"->cmd_cast(cast_str);
    }

    target = find_combat_target(this_player());
    if(!target) {
        write("Magic net at whom?  Syntax: magicnet <target>\n");
        return 1;
    }
    cast_str = "magic net at " + (string)target->query_name();
    return (int)"/cmds/mortal/_cast"->cmd_cast(cast_str);
}

void help() {
    write("Syntax: magicnet [target]\n\n"
        "Cast magic net at a target. With no target in combat, hits your\n"
        "current attacker automatically. Also: cast magic net at <target>\n");
}
