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

    /* Find target if specified; "me"/"self"/own name target the caster */
    target = 0;
    if(target_str) {
        if(target_str == "me" || target_str == "self" ||
           target_str == (string)this_player()->query_name())
            target = this_player();
        else
            target = present(target_str, environment(this_player()));
    }

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
   Accepts the classic RiftsMUD forms as well as the "at" form:
     windrush              (no target)
     windrush grunt        (classic: spell name then target)
     windrush at grunt
     energy bolt grunt     (multi-word spell names match longest-first)
   Returns 1 and casts if the player knows the spelled word; 0 to fall through. */
int try_spell_shortcut(string str) {
    string spell_part;
    string target_part;
    string spell_name;
    string *words;
    mapping sdata;
    int at_pos;
    int i;
    int n;

    if(!str || !sizeof(str)) return 0;
    if(!this_player()) return 0;
    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race())) return 0;
    if(!(int)RIFTS_D->player_has_magic_access(this_player())) return 0;

    at_pos = strsrch(str, " at ");
    if(at_pos > 0) {
        spell_part = lower_case(str[0..at_pos-1]);
        target_part = lower_case(str[at_pos+4..]);
        if(!sizeof(target_part)) target_part = 0;
    } else {
        spell_part = lower_case(str);
        target_part = 0;
    }

    spell_name = (string)RIFTS_SPELLS_D->normalize_spell_name(spell_part);
    if(spell_name == "magic net") return 0;
    sdata = (mapping)RIFTS_SPELLS_D->query_spell(spell_name);

    /* "windrush grunt": no " at " and the whole string is not a spell.
       Try progressively shorter word prefixes as the spell name and
       treat the remainder as the target. */
    if(!sdata && !target_part) {
        words = explode(spell_part, " ");
        n = sizeof(words);
        for(i = n - 1; i >= 1; i--) {
            spell_name = (string)RIFTS_SPELLS_D->normalize_spell_name(
              implode(words[0..i-1], " "));
            if(spell_name == "magic net") continue;
            sdata = (mapping)RIFTS_SPELLS_D->query_spell(spell_name);
            if(sdata) {
                target_part = implode(words[i..n-1], " ");
                break;
            }
        }
    }

    if(!sdata) return 0;
    if(!(int)RIFTS_D->player_knows_spell(this_player(), spell_name)) return 0;
    if(target_part)
        return cmd_cast(spell_name + " at " + target_part);
    return cmd_cast(spell_name);
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
