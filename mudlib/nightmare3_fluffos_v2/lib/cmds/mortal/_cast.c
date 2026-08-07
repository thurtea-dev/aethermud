/* /cmds/mortal/_cast.c
   Syntax: cast <spell name> [at <target>] */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

/* Spell effects whose fx_ handler in rifts_spells_d.c requires a resolved
   target and fails without one. Checked before PPE and APM are spent so a
   failed cast costs nothing. Keep in sync with the !target guards there.
   Deliberately absent: "net" (auto-targets a combat opponent), "windrush"
   (no target sweeps the whole room), and the self-defaulting effects
   (heal, swimasafish, tongues, restoration_spell, fly_eagle). */
#define NEED_TARGET_EFFECTS ({ \
    "damage", "mark", "blind", "fear", "burn", "exorcism", \
    "fireball", "firebolt", "horrific_illusion", "shadow_bolt", \
    "time_hole", "rift_to_oblivion", "constrain_being", "mend_wounds", \
    "cure_disorders", "cleanse", "luck_curse", "wisps_confusion", \
    "slow_movement", "breath_of_life", "words_truth", "detect_poison", \
    "spell_tk", "domination", "call_lightning", "freeze_spell", \
    "negate_mechanics", "energy_drain", "locate_spell", "negate_magic", \
    "amulet_spell", "life_drain", "create_scroll", "dim_envelope", \
    "soul_drain", "animate_object", "resurrection_spell", \
    "power_word_kill", "create_magic_weapon", "time_warp_age" })

/* Auto-target the caster's current combat opponent when no target was
   given and the effect requires one. Duplicated from _magicnet.c's
   find_combat_target() rather than shared, per instruction not to
   touch that file. */
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

    /* Target and range validation happens before any resources are
       spent: a failed cast costs nothing. */
    if(target_str && !target) {
        write("You don't see '" + target_str + "' here.\n");
        return 1;
    }
    if(!target &&
       member_array((string)sdata["effect"], NEED_TARGET_EFFECTS) != -1) {
        target = find_combat_target(this_player());
        if(!target) {
            write("Cast at whom?  Syntax: cast " + spell_name + " at <target>\n");
            return 1;
        }
    }

    /* In combat: spend an APM */
    if(sizeof((object *)this_player()->query_attackers()) > 0) {
        if(!(int)RIFTS_COMBAT_D->can_do_attack(this_player())) return 1;
        RIFTS_COMBAT_D->use_rifts_attack(this_player());
    }

    /* Deduct PPE */
    this_player()->set_stats("PPE", ppe - ppe_cost);

    /* Same prowl break as _kill.c: casting always abandons stealth. */
    if((int)this_player()->query_property("is_sneaking")) {
        this_player()->remove_property("is_sneaking");
        write("You abandon stealth as you attack!\n");
    }

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
