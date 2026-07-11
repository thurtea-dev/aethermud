/* /cmds/mortal/_cast.c
   Syntax: cast <spell name> [at <target>] */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_cast(string str) {
    string spell_name;
    string target_str;
    object target;
    mapping sdata;
    int ppe;
    int ppe_cost;
    int i;

    if(!str || !sizeof(str)) {
        write("Cast what spell?  Syntax: cast <spell> [at <target>]\n");
        return 1;
    }

    /* Check player is a Rifts character */
    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race())) {
        notify_fail("You don't have the ability to cast spells.\n");
        return 0;
    }

    if(!(int)RIFTS_D->player_has_magic_access(this_player())) {
        write("You do not know how to cast spells.\n");
        return 1;
    }

    /* Parse "spell name at target" -- split on first " at " */
    i = strsrch(str, " at ");
    if(i > 0) {
        spell_name = lower_case(str[0..i-1]);
        target_str = lower_case(str[i+4..]);
        if(!sizeof(target_str)) target_str = 0;
    } else {
        spell_name = lower_case(str);
        target_str = 0;
    }

    spell_name = (string)RIFTS_SPELLS_D->normalize_spell_name(spell_name);

    /* Validate spell exists in the definitions daemon */
    sdata = (mapping)RIFTS_SPELLS_D->query_spell(spell_name);
    if(!sdata) {
        write("You don't know the spell '" + spell_name + "'.\n");
        return 1;
    }

    if(!(int)RIFTS_D->player_knows_spell(this_player(), spell_name)) {
        write("You don't know the spell '" + spell_name + "'.\n");
        return 1;
    }

    /* Check PPE */
    ppe_cost = (int)sdata["ppe_cost"];
    ppe = (int)this_player()->query_stats("PPE");
    if(ppe < ppe_cost) {
        write("You don't have enough PPE to cast " + spell_name +
              ".  (Need " + ppe_cost + ", have " + ppe + ".)\n");
        return 1;
    }

    /* Find target if specified */
    target = 0;
    if(target_str)
        target = present(target_str, environment(this_player()));

    /* In combat: spend an APM */
    if(sizeof((object *)this_player()->query_attackers()) > 0) {
        if(!(int)RIFTS_COMBAT_D->can_do_attack(this_player())) return 1;
        RIFTS_COMBAT_D->use_rifts_attack(this_player());
    }

    /* Deduct PPE */
    this_player()->set_stats("PPE", ppe - ppe_cost);

    /* Apply effect */
    RIFTS_SPELLS_D->apply_spell_effect(spell_name, target);
    return 1;
}

/* Called from chat.c after all other command paths have failed.
   Receives the full input string (verb + optional " arg").
   Returns 1 and casts if the player knows the spelled word; 0 to fall through. */
int try_spell_shortcut(string str) {
    string spell_part;
    string spell_name;
    mapping sdata;
    int at_pos;

    if(!str || !sizeof(str)) return 0;
    at_pos = strsrch(str, " at ");
    if(at_pos > 0)
        spell_part = lower_case(str[0..at_pos-1]);
    else
        spell_part = lower_case(str);
    spell_name = (string)RIFTS_SPELLS_D->normalize_spell_name(spell_part);
    if(spell_name == "magic net") return 0;
    sdata = (mapping)RIFTS_SPELLS_D->query_spell(spell_name);
    if(!sdata) return 0;
    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race())) return 0;
    if(!(int)RIFTS_D->player_has_magic_access(this_player())) return 0;
    if(!(int)RIFTS_D->player_knows_spell(this_player(), spell_name)) return 0;
    return cmd_cast(str);
}

void help() {
    write(
        "Syntax: cast <spell name> [at <target>]\n\n"
        "Cast a magic spell.  Costs PPE.  In combat, costs 1 APM.\n\n"
        "  Examples:\n"
        "    cast globe of daylight\n"
        "    cast energy bolt at grunt\n"
        "    cast heal wounds\n"
        "    cast heal wounds at ally\n\n"
        "Type <spells> to see your known spells and PPE cost.\n"
    );
}
