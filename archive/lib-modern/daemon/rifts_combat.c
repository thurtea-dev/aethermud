/* /daemon/rifts_combat.c */
/* RiftsMUD  -  Palladium combat statistics and damage daemon */
/* Provides APM, initiative, strike, parry, dodge, damage bonuses, */
/* armor drain chain, autoparry, autododge, and death thresholds. */

#include <daemons.h>

#define ROUND_DURATION 15

/* Per-player melee round tracker: player -> ({ attacks_remaining, round_start_time }) */
private mapping rifts_round_data;

/* Forward declaration: can_do_attack() (defined below) calls this before its */
/* own definition is reached; this driver requires a prototype for that.     */
void combat_round(object player);

private void damage_trapped_net(object defender, int dmg) {
    object net;

    if(!defender || dmg < 1) return;
    net = defender->query_property("magic_net_obj");
    if(!net || !objectp(net)) return;
    catch(net->take_net_damage(dmg));
}

void try_entanglement_hit(object attacker, object defender, object weapon) {
    object net;

    if(!attacker || !defender || !weapon) return;
    if(!(int)weapon->query_property("entanglement")) return;
    if(defender->query_property("magic_net_obj")) return;
    if(environment(attacker) != environment(defender)) return;
    net = new("/std/obj/magic_net");
    if(!net) return;
    net->move(environment(defender));
    net->capture(defender, attacker);
}

private string id_name(object who, object viewer) {
    string n;

    if(!who) return "someone";
    if(function_exists("query_display_name", who)) {
        n = (string)who->query_display_name(viewer);
        if(n && sizeof(n)) return n;
    }
    return (string)who->query_cap_name();
}

private void tell_observers_combat(object env, object attacker, object defender,
    string msg) {
    object *obs;
    int k;
    string att_disp;
    string def_disp;
    string out;

    if(!env || !attacker || !defender) return;
    obs = all_inventory(env);
    for(k = 0; k < sizeof(obs); k++) {
        if(!obs[k] || obs[k] == attacker || obs[k] == defender) continue;
        att_disp = id_name(attacker, obs[k]);
        def_disp = id_name(defender, obs[k]);
        out = replace_string(msg, "$A", att_disp);
        out = replace_string(out, "$D", def_disp);
        tell_object(obs[k], out);
    }
}

private void tell_observers_subject(object env, object subject, object *skip,
    string msg) {
    object *obs;
    int k;
    int s;
    string shown;
    string out;

    if(!env || !subject || !msg) return;
    obs = all_inventory(env);
    for(k = 0; k < sizeof(obs); k++) {
        if(!obs[k] || obs[k] == subject) continue;
        if(skip) {
            for(s = 0; s < sizeof(skip); s++)
                if(obs[k] == skip[s]) break;
            if(s < sizeof(skip)) continue;
        }
        shown = id_name(subject, obs[k]);
        out = replace_string(msg, "$S", shown);
        tell_object(obs[k], out);
    }
}

/* ── Internal helpers ──────────────────────────────────────────────────────── */

/* PP attribute bonus to combat rolls (Palladium RPG standard). (approx) */
private int pp_combat_bonus(int pp) {
    if(pp >= 26) return 6;
    if(pp >= 21) return 5;
    if(pp >= 19) return 4;
    if(pp >= 18) return 3;
    if(pp >= 16) return 2;
    if(pp >= 13) return 1;
    return 0;
}

/* PS attribute damage bonus.  Supernatural PS doubles the raw bonus. (approx) */
private int ps_damage_bonus(int ps, int supernatural) {
    int bonus;
    if(ps >= 31) bonus = 7;
    else if(ps >= 30) bonus = 6;
    else if(ps >= 26) bonus = 5;
    else if(ps >= 21) bonus = 4;
    else if(ps >= 18) bonus = 3;
    else if(ps >= 16) bonus = 2;
    else bonus = 0;
    if(supernatural) return bonus * 2;
    return bonus;
}

/* APM base by OCC combat category. */
private int occ_base_apm(string occ) {
    if(!occ || occ == "") return 2;
    switch(occ) {
    /* Martial arts / peak combatants (approx) */
    case "cyber-knight":
    case "crazy":
    case "juicer":
    case "ninja juicer":
    case "delphi juicer":
    case "hyperion juicer":
    case "tattooed man":
    case "tattoo warrior":
        return 6; /* approx */

    /* Assassin / specialist (approx) */
    case "master assassin":
    case "city rat":
    case "forger":
    case "freelance spy":
    case "professional thief":
    case "smuggler":
    case "iss peacekeeper":
    case "iss specter":
        return 5; /* approx */

    /* Military / soldier (approx) */
    case "headhunter":
    case "bounty hunter":
    case "cs grunt":
    case "cs dead boy":
    case "cs ranger":
    case "cs military specialist":
    case "cs samas rpa pilot":
    case "cs technical officer":
    case "merc soldier":
    case "special forces (merc)":
    case "tribal warrior":
    case "wilderness scout":
    case "borg":
    case "glitter boy pilot":
    case "robot pilot":
    case "ntset protector":
    case "knight (europe)":
    case "royal knight":
    case "pirate (s.a.)":
    case "sailor (s.a.)":
        return 4; /* approx */

    /* Magic users (approx) */
    case "ley line walker":
    case "mystic":
    case "shifter":
    case "shaman":
    case "techno-wizard":
    case "ley line rifter":
    case "air warlock":
    case "nega-psychic":
        return 3; /* approx */

    /* Civilian / generalist (approx) */
    default:
        return 2; /* approx */
    }
}

/* Roll Rifts weapon damage from weapon properties (damage_num d damage_sides + damage_bonus).
   Returns 0 if no Rifts weapon is wielded or ranged weapon is out of ammo. */
private int roll_rifts_weapon_damage(object attacker) {
    object *wielded;
    object weap;
    int i, num, sides, bonus, damage, j;
    int ammo;

    wielded = (object *)attacker->query_wielded();
    if(!wielded || !sizeof(wielded)) return 0;
    for(i = 0; i < sizeof(wielded); i++) {
        weap = wielded[i];
        if(!weap) continue;

        /* Ranged weapon: check and consume one shot per attack action. */
        if((int)weap->query_property("ranged")) {
            ammo = (int)weap->query_property("current_ammo");
            if(ammo <= 0) {
                message("my_combat",
                    "Your " + (string)weap->query_short() +
                    " clicks empty. Type 'reload <weapon>' to reload.",
                    attacker);
                return 0;
            }
            weap->set_property("current_ammo", ammo - 1);
        }

        num   = (int)weap->query_property("damage_num");
        sides = (int)weap->query_property("damage_sides");
        if(num > 0 && sides > 0) {
            damage = 0;
            for(j = 0; j < num; j++) damage += random(sides) + 1;
            bonus = (int)weap->query_property("damage_bonus");
            damage += bonus;
            return damage > 0 ? damage : 1;
        }
    }
    return 0;
}

/* Find the first worn armor of the given type ("mdc_armor" or "sdc_armor") on who. */
private object find_worn_armor(object who, string type) {
    object *inv;
    object ob;
    int i;

    inv = all_inventory(who);
    if(!inv) return 0;
    for(i = 0; i < sizeof(inv); i++) {
        ob = inv[i];
        if(!ob) continue;
        if((int)ob->query_property(type) && ob->query_worn() == who)
            return ob;
    }
    return 0;
}

/* Initialize current_armor_mdc/sdc on a piece of armor the first time it takes damage. */
private void init_armor_hp(object armor) {
    int max_mdc, max_sdc;
    max_mdc = (int)armor->query_property("armor_mdc");
    max_sdc = (int)armor->query_property("armor_sdc");
    if(max_mdc > 0 && !(int)armor->query_property("current_armor_mdc"))
        armor->set_property("current_armor_mdc", max_mdc);
    if(max_sdc > 0 && !(int)armor->query_property("current_armor_sdc"))
        armor->set_property("current_armor_sdc", max_sdc);
}

/* Destroy a worn armor piece: unwear it silently, rename it, drop it to the room. */
/* Called when MDC or SDC of the armor reaches 0 mid-combat. */
private void destroy_armor(object armor, object wearer, object room) {
    string *limbs, old_name;

    if(!armor || !objectp(armor)) return;
    old_name = (string)armor->query_name();

    /* Remove from the body limb tracking */
    limbs = (string *)armor->query_actual_limbs();
    if(limbs && objectp(wearer))
        wearer->remove_armour_from_limb(armor, limbs);
    armor->set_not_equipped();

    armor->set_short("ruined " + old_name);
    armor->set_name("ruined " + old_name);

    if(objectp(room))
        armor->move(room);
}

/* ── Public: APM and round tracking ───────────────────────────────────────── */

/* Returns attacks per melee round for a player. */
/* Base from OCC, +PP bonus, +level bonus from rifts_apm_bonus property. */
/* rifts_apm_bonus is incremented by _advance.c at levels 3/6/9/12/15. */
int query_apm(object player) {
    string occ, race;
    int pp, apm, pp_bonus, level_bonus;
    int lvl;

    if(!player) return 2;
    race = (string)player->query_race();
    if(!(int)RIFTS_D->is_rifts_race(race)) return 2;

    occ         = (string)player->getenv("rifts_occ");
    pp          = (int)player->query_stats("PP");
    level_bonus = (int)player->query_property("rifts_apm_bonus");

    /* Godling: base APM 4, +1 at PP 16-20, +2 at PP 21+. */
    if(lower_case(race) == "godling") {
        apm = 4;
        if(pp >= 21) pp_bonus = 2;
        else if(pp >= 16) pp_bonus = 1;
        else pp_bonus = 0;
        return apm + pp_bonus + level_bonus;
    }

    if(!occ || occ == "" || occ == "none") {
        if(!interactive(player)) {
            lvl = (int)player->query_level();
            if(lvl >= 12) return 4 + level_bonus;
            if(lvl >= 8) return 3 + level_bonus;
            return 2 + level_bonus;
        }
    }

    apm = occ_base_apm(occ);

    if(pp >= 18) pp_bonus = 2;
    else if(pp >= 16) pp_bonus = 1;
    else pp_bonus = 0;

    return apm + pp_bonus + level_bonus;
}

/* Public alias used by the combat hook. */
int query_total_apm(object player) { return query_apm(player); }

/* Returns 1 if this player still has attacks left this melee round. */
/* Resets the round automatically after ROUND_DURATION seconds. */
/* When a player and an NPC foe are involved, a fresh round is handed off  */
/* to combat_round() so the whole round resolves as two full-APM blocks    */
/* (player first, then foe) instead of one attack trickling in per         */
/* heartbeat tick. The single-attack request that triggered this check is  */
/* denied here since combat_round() just resolved it as part of the block. */
int can_do_attack(object attacker) {
    int now, apm;
    mixed *data;
    object foe, player;

    if(!attacker || !objectp(attacker)) return 0;
    if(!rifts_round_data) rifts_round_data = ([]);

    now  = time();
    data = rifts_round_data[attacker];

    if(!data || (now - data[1]) >= ROUND_DURATION) {
        foe = (object)attacker->query_current_attacker();
        player = (int)attacker->is_player() ? attacker :
                 ((foe && objectp(foe) && (int)foe->is_player()) ? foe : 0);
        if(player && objectp(player)) {
            data = rifts_round_data[player];
            if(!data || (now - data[1]) >= ROUND_DURATION)
                combat_round(player);
            return 0;
        }
        apm = query_apm(attacker);
        rifts_round_data[attacker] = ({ apm, now });
        return 1;
    }
    return data[0] > 0;
}

/* Resolves one full melee round for a player and their current NPC foe:   */
/* the player's entire APM block first (player has initiative by default), */
/* then the foe's entire APM block, then schedules the next round. Reuses  */
/* the existing continue_attack()/execute_attack() guard chain (death,     */
/* paralysis, room "no attack", wimpy) for every individual attack; only   */
/* the scheduling changes, not hit/miss/damage logic.                      */
/* Self-guards against firing twice for the same round boundary (the       */
/* natural per-tick path and the scheduled call_out can both land near     */
/* the same 15 second mark). */
void combat_round(object player) {
    mixed *existing;
    object foe;
    int apm, i, now;
    int player_apm, player_used, foe_apm, foe_used;
    string foe_name;

    if(!player || !objectp(player) || !interactive(player)) return;
    now = time();
    existing = rifts_round_data[player];
    if(existing && (now - existing[1]) < ROUND_DURATION - 1) return;

    foe = (object)player->query_current_attacker();
    if(!foe || !objectp(foe)) return;
    foe_name = (string)foe->query_name();

    apm = query_apm(player);
    player_apm = apm;
    rifts_round_data[player] = ({ apm, now });
    for(i = 0; i < apm; i++) {
        if(!objectp(player) || !interactive(player)) break;
        foe = (object)player->query_current_attacker();
        if(!foe || !objectp(foe)) break;
        player->continue_attack();
    }
    player_used = i;

    foe = (object)player->query_current_attacker();
    if(!foe || !objectp(foe) || !interactive(player)) {
        log_file("combat_round", sprintf("%s: %s vs %s: player_apm=%d player_used=%d foe=gone\n",
            ctime(time()), (string)player->query_name(), foe_name, player_apm, player_used));
        return;
    }

    foe_apm = 0;
    foe_used = 0;
    if((int)RIFTS_D->is_rifts_race((string)foe->query_race())) {
        /* Rifts-race foe: execute_attack() performs exactly one attack per */
        /* call, gated by can_do_attack(), so loop its full APM block.      */
        apm = query_apm(foe);
        foe_apm = apm;
        rifts_round_data[foe] = ({ apm, now });
        for(i = 0; i < apm; i++) {
            if(!objectp(foe)) break;
            if(!interactive(player) || !(object)foe->query_current_attacker()) break;
            foe->continue_attack();
        }
        foe_used = i;
    } else {
        /* Legacy (non-Rifts-race) NPC, e.g. Plains wolf: execute_attack()  */
        /* already resolves its own attack count internally per call, so a */
        /* single call is its full block; looping further would multiply   */
        /* its attacks instead of just grouping them. */
        foe->continue_attack();
        foe_apm = 1;
        foe_used = 1;
    }

    log_file("combat_round", sprintf("%s: %s vs %s: player_apm=%d player_used=%d foe_apm=%d foe_used=%d\n",
        ctime(time()), (string)player->query_name(), foe_name, player_apm, player_used, foe_apm, foe_used));

    if(!interactive(player) || !(object)player->query_current_attacker()) return;
    call_out("start_next_round", ROUND_DURATION, player);
}

/* call_out target: starts the next round if the fight is still going. */
void start_next_round(object player) {
    if(!player || !objectp(player)) return;
    if(!(object)player->query_current_attacker()) return;
    combat_round(player);
}

/* Returns 1 if npc's current fight against a player is being driven by an  */
/* active combat_round() this round window. Legacy (non-Rifts-race) NPCs   */
/* like Plains wolf never reach can_do_attack() on their own heartbeat (its */
/* attacker gate in std/living/combat.c only checks the attacker's own      */
/* race), so their natural per-tick continue_attack() call is never gated  */
/* the way a Rifts attacker's is. std/monster.c heart_beat() calls this     */
/* first so it can skip that natural call while combat_round() already owns */
/* this NPC's attacks for the round; otherwise the NPC attacks once from    */
/* the daemon's block and again, unbounded, from its own heartbeat. */
int npc_round_active(object npc) {
    object player;
    mixed *data;

    if(!npc || !objectp(npc) || (int)npc->is_player()) return 0;
    player = (object)npc->query_current_attacker();
    if(!player || !objectp(player) || !(int)player->is_player()) return 0;
    data = rifts_round_data[player];
    if(!data) return 0;
    return (time() - data[1]) < ROUND_DURATION;
}

/* Deduct one attack from the current melee round for the given combatant. */
void use_rifts_attack(object combatant) {
    mixed *data;
    if(!rifts_round_data || !combatant || !objectp(combatant)) return;
    data = rifts_round_data[combatant];
    if(data && data[0] > 0) rifts_round_data[combatant][0]--;
}

/* Clean up destructed-object keys (call periodically if needed). */
void clean_round_data() {
    mixed *keys_arr;
    int i;
    if(!rifts_round_data) return;
    keys_arr = keys(rifts_round_data);
    for(i = 0; i < sizeof(keys_arr); i++) {
        if(!objectp(keys_arr[i]))
            map_delete(rifts_round_data, keys_arr[i]);
    }
}

/* ── Public: Combat stat queries ───────────────────────────────────────────── */

/* PP bonus to initiative roll (1d20 + this bonus at start of each round). */
int query_initiative_bonus(object player) {
    int pp;
    int bonus;
    mapping addmods;

    if(!player) return 0;
    pp      = (int)player->query_stats("PP");
    bonus   = pp_combat_bonus(pp);
    addmods = (mapping)ADDICTION_D->query_combat_modifiers(player);
    bonus  += (int)addmods["initiative"];
    return bonus;
}

/* Strike bonus  -  PP plus level-based combat training. (approx) */
/* Position modifier: +2 strike if offensive, +2 dodge/parry if defensive. */
private int position_strike_mod(object player) {
    string pos;
    pos = (string)player->query_property("combat_stance");
    if(!pos) return 0;
    if(pos == "offensive") return 2;
    if(pos == "defensive") return -2;
    return 0;
}

private int position_defense_mod(object player) {
    string pos;
    pos = (string)player->query_property("combat_stance");
    if(!pos) return 0;
    if(pos == "defensive") return 2;
    if(pos == "offensive") return -2;
    return 0;
}

int query_strike_bonus(object player) {
    int pp, level, bonus;
    string occ;
    mapping addmods;

    if(!player) return 0;
    pp    = (int)player->query_stats("PP");
    level = (int)player->query_level();
    occ   = (string)player->getenv("rifts_occ");
    bonus = pp_combat_bonus(pp);

    switch(occ_base_apm(occ)) {
    case 6: bonus += level / 2; break;
    case 5: bonus += level / 3; break;
    case 4: bonus += level / 3; break;
    case 3: bonus += level / 4; break;
    default: bonus += level / 5; break;
    }
    bonus  += position_strike_mod(player);
    addmods = (mapping)ADDICTION_D->query_combat_modifiers(player);
    bonus  += (int)addmods["strike"];
    return bonus;
}

/* Parry bonus  -  PP plus level-based training. */
int query_parry_bonus(object player) {
    int pp, level, bonus;
    string occ;
    mapping addmods;

    if(!player) return 0;
    pp    = (int)player->query_stats("PP");
    level = (int)player->query_level();
    occ   = (string)player->getenv("rifts_occ");
    bonus = pp_combat_bonus(pp);

    switch(occ_base_apm(occ)) {
    case 6: bonus += level / 2; break;
    case 5: bonus += level / 3; break;
    case 4: bonus += level / 3; break;
    case 3: bonus += level / 4; break;
    default: bonus += level / 5; break;
    }
    bonus  += position_defense_mod(player);
    addmods = (mapping)ADDICTION_D->query_combat_modifiers(player);
    bonus  += (int)addmods["parry"];
    return bonus;
}

/* Dodge bonus  -  same scale as parry. */
int query_dodge_bonus(object player) {
    return query_parry_bonus(player);
}

/* Damage bonus from PS.  Supernatural PS races double the raw bonus. */
int query_damage_bonus(object player) {
    int ps, supernatural;
    string race;
    string *flags;

    if(!player) return 0;
    ps   = (int)player->query_stats("PS");
    race = (string)player->query_race();

    flags = (string *)RIFTS_D->query_race_flags(race);
    supernatural = (flags && member_array("supernatural_ps", flags) != -1);

    return ps_damage_bonus(ps, supernatural);
}

/* ── Public: Weapon and MDC checks ────────────────────────────────────────── */

/* Returns 1 if the weapon object is MDC-capable. */
int is_mdc_weapon(object weapon) {
    return (int)RIFTS_D->is_mdc_weapon(weapon);
}

/* Returns 1 if the attacker can damage the defender. */
/* SDC attackers cannot harm MDC beings. */
int can_damage(object attacker, object defender) {
    string def_race;
    object *wielded;
    int i;

    if(!attacker || !defender) return 1;

    def_race = (string)defender->query_race();
    if(!(int)RIFTS_D->is_mdc_race(def_race)) return 1;

    /* Check worn MDC armor on attacker's hands; built-in MDC counts */
    if((int)defender->query_property("built_in_mdc")) return 1;

    wielded = (object *)attacker->query_wielded();
    if(wielded && sizeof(wielded)) {
        for(i = 0; i < sizeof(wielded); i++) {
            if(is_mdc_weapon(wielded[i])) return 1;
        }
    }
    /* Bare hands of MDC beings can still harm each other (approx) */
    if((int)RIFTS_D->is_mdc_race((string)attacker->query_race())) return 1;
    return 0;
}

/* ── Public: Attack flavor messages ───────────────────────────────────────── */

/* Returns a short verb phrase describing the damage level. */
/* Used for "You <verb> the goblin." messages. */
string get_attack_message(object attacker, object defender, int damage) {
    string attname, defname;

    if(!attacker || !defender) return "strikes";
    attname = (string)attacker->query_cap_name();
    defname = (string)defender->query_cap_name();

    if(damage <= 0)    return "bounces harmlessly off";
    if(damage <= 5)    return "scratches";
    if(damage <= 15)   return "hits";
    if(damage <= 30)   return "strikes";
    if(damage <= 50)   return "claws viciously";
    return "devastates";
}

/* Same damage tiers as get_attack_message(), base verb form (no -s) for */
/* "You <verb> ..." messages where the subject is second person. */
string get_attack_verb_base(int damage) {
    if(damage <= 0)    return "bounce harmlessly off";
    if(damage <= 5)    return "scratch";
    if(damage <= 15)   return "hit";
    if(damage <= 30)   return "strike";
    if(damage <= 50)   return "claw viciously";
    return "devastate";
}

/* Full combat message string sent to both parties. */
/* Returns -1 to signal to combat.c that Rifts messages were sent. */
private int send_rifts_messages(object attacker, object defender, int damage) {
    string verb;
    string me_msg;
    string you_msg;

    verb = get_attack_message(attacker, defender, damage);

    if(damage <= 0) {
        me_msg = "Your attack " + verb + " " +
            id_name(defender, attacker) + ".";
        you_msg = id_name(attacker, defender) + "'s attack " + verb + " you.";
        message("my_combat", me_msg, attacker);
        message("my_combat", you_msg, defender);
        tell_observers_combat(environment(attacker), attacker, defender,
            "$A's attack " + verb + " $D.\n");
    } else {
        me_msg = sprintf("You %s %s.", get_attack_verb_base(damage),
            id_name(defender, attacker));
        you_msg = sprintf("%s %s you.", id_name(attacker, defender), verb);
        message("my_combat", me_msg, attacker);
        message("my_combat", you_msg, defender);
        tell_observers_combat(environment(attacker), attacker, defender,
            sprintf("$A %s $D.\n", verb));
    }
    return -1;
}

/* MDC-specific combat messages: qualitative verb only, no MDC numbers shown. */
private int send_mdc_hit_messages(object attacker, object defender, int damage) {
    string verb;

    verb = get_attack_message(attacker, defender, damage);

    if(damage <= 0) {
        message("my_combat",
            "Your attack " + verb + " " + id_name(defender, attacker) + ".",
            attacker);
        message("my_combat",
            id_name(attacker, defender) + "'s attack " + verb + " you.",
            defender);
        tell_observers_combat(environment(attacker), attacker, defender,
            "$A's attack " + verb + " $D.\n");
    } else {
        message("my_combat",
            sprintf("You %s %s.", get_attack_verb_base(damage),
                id_name(defender, attacker)),
            attacker);
        message("my_combat",
            sprintf("%s %s you.", id_name(attacker, defender), verb),
            defender);
        tell_observers_combat(environment(attacker), attacker, defender,
            sprintf("$A %s $D.\n", verb));
    }
    return -1;
}

/* ── Public: Autododge roll ────────────────────────────────────────────────── */

/* Roll an autododge check for the defender vs the attacker's strike. */
/* Returns 1 if the dodge succeeded, 0 if it failed. */
/* Does not consume offensive APM; autododge is a free reactive defense. */
int autododge_roll(object defender, int strike_roll) {
    int roll;

    if(!defender) return 0;

    roll = random(20) + 1 + query_dodge_bonus(defender);
    return (roll >= strike_roll);
}

/* ── Public: Apply Rifts damage chain ──────────────────────────────────────── */
/* */
/* Call this from living/combat.c instead of do_damage() for Rifts combatants. */
/* */
/* Returns: */
/* -1  : Rifts combat messages have been sent; caller should skip send_messages */
/* 0  : attack bounced/parried with no damage (messages sent) */
/* >0  : damage dealt (messages sent, caller should still skip send_messages */
/* since we already told both parties) */
/* */
/* Damage priority (Palladium rules): */
/* 1. Worn MDC armor MDC pool (if attacker has MDC weapon) */
/* 2. Worn SDC armor SDC pool (if attacker has SDC weapon) */
/* 3. Character's own SDC pool */
/* 4. Character's HP pool (rifts_hp) */
/* MDC beings without armor: MDC weapon hits MDC pool directly */
/* SDC weapon vs MDC being: 0 damage, bounce message */
/* */
int apply_rifts_damage(object attacker, object defender, int raw_damage) {
    int damage, strike, parry, is_mdc_def, is_mdc_atk, actual;
    int overflow;
    object armor;
    string def_race, att_race;
    int cur_mdc, cur_sdc, cur_hp;
    object *wielded;
    int has_mdc_weapon;
    int i;
    int wd;
    int armor_bypass;
    int penetrate_ar;
    int ar_bypass_half;

    if(!attacker || !defender || raw_damage < 0) return 0;

    {
        string elem;

        elem = lower_case((string)attacker->query_property("last_attack_element") || "");
        if(elem == "fire" && (int)defender->query_property("impervious_fire")) {
            message("other_action", (string)defender->query_cap_name() +
                " is untouched by the flames.", environment(defender));
            return 0;
        }
        if((elem == "cold" || elem == "ice") &&
           (int)defender->query_property("impervious_cold")) {
            message("other_action", (string)defender->query_cap_name() +
                " shrugs off the cold.", environment(defender));
            return 0;
        }
        if(elem == "lightning" && (int)defender->query_property("impervious_energy")) {
            message("other_action", (string)defender->query_cap_name() +
                " ignores the electrical discharge.", environment(defender));
            return 0;
        }
    }

    def_race = (string)defender->query_race();
    att_race = (string)attacker->query_race();
    is_mdc_def = (int)RIFTS_D->is_mdc_race(def_race);
    is_mdc_atk = (int)RIFTS_D->is_mdc_race(att_race);

    /* Check MDC immunity: SDC attacker cannot harm MDC defender */
    wielded = (object *)attacker->query_wielded();
    has_mdc_weapon = 0;
    if(wielded) {
        for(i = 0; i < sizeof(wielded); i++) {
            if(wielded[i] && (int)RIFTS_D->is_mdc_weapon(wielded[i])) {
                has_mdc_weapon = 1;
                break;
            }
        }
    }
    if(!has_mdc_weapon && (int)attacker->query_property("psi_sword_active") &&
       (!wielded || !sizeof(wielded)))
        has_mdc_weapon = 1;

    /* Check weapon special penetration properties */
    armor_bypass  = 0;
    penetrate_ar  = 0;
    ar_bypass_half = 0;
    if(wielded) {
        for(i = 0; i < sizeof(wielded); i++) {
            if(!wielded[i]) continue;
            if((int)wielded[i]->query_property("armor_bypass"))   armor_bypass  = 1;
            if((int)wielded[i]->query_property("penetrate_ar"))   penetrate_ar  = 1;
            if((int)wielded[i]->query_property("ar_bypass_half")) ar_bypass_half = 1;
        }
    }

    if(is_mdc_def && !has_mdc_weapon && !is_mdc_atk &&
       !(int)defender->query_property("built_in_mdc")) {
        /* SDC weapons deal damage to MDC being's SDC buffer; no MDC damage */
        cur_sdc = (int)defender->query_stats("SDC");
        if(cur_sdc > 0) {
            if(raw_damage >= cur_sdc) {
                cur_sdc = 0;
            } else {
                cur_sdc -= raw_damage;
            }
            defender->set_stats("SDC", cur_sdc);
            message("my_combat",
                "Your attack scratches the surface.",
                attacker);
            message("my_combat",
                id_name(attacker, defender) +
                "'s attack scratches your surface.",
                defender);
            tell_observers_combat(environment(attacker), attacker, defender,
                "$A scratches $D's surface.\n");
        } else {
            message("my_combat",
                "Your weapon bounces harmlessly off the MDC structure!",
                attacker);
            message("my_combat",
                id_name(attacker, defender) +
                "'s attack bounces off your MDC armor.",
                defender);
            tell_observers_combat(environment(attacker), attacker, defender,
                "$A's attack bounces harmlessly off $D.\n");
        }
        return -1;
    }

    /* Autoparry (free, default for Rifts characters). */
    /* Autododge overrides parry if toggled on and attacks remain. */
    /* Autoparry can be disabled via "autoparry off". */
    strike = random(20) + 1 + query_strike_bonus(attacker);
    if((int)RIFTS_D->is_rifts_race(def_race)) {
        if((string)defender->getenv("rifts_autododge") == "1") {
            if(autododge_roll(defender, strike)) {
                message("my_combat",
                    "You dodge " + id_name(attacker, defender) + "'s attack!",
                    defender);
                message("my_combat",
                    id_name(defender, attacker) + " dodges your attack!",
                    attacker);
                tell_observers_combat(environment(attacker), attacker, defender,
                    "$D dodges $A's attack!\n");
                return -1;
            }
        } else if((string)defender->getenv("rifts_autoparry_off") != "1") {
            /* Autoparry  -  costs no APM */
            parry = random(20) + 1 + query_parry_bonus(defender);
            if(parry >= strike) {
                message("my_combat",
                    "You parry " + id_name(attacker, defender) + "'s attack!",
                    defender);
                message("my_combat",
                    id_name(defender, attacker) + " parries your attack!",
                    attacker);
                tell_observers_combat(environment(attacker), attacker, defender,
                    "$D parries $A's attack!\n");
                return -1;
            }
        }
    }

    /* Add PS damage bonus and Rifts weapon damage to raw_damage */
    damage = raw_damage + query_damage_bonus(attacker);
    wd = roll_rifts_weapon_damage(attacker);
    if(wd > 0) damage = wd + query_damage_bonus(attacker);
    if(damage < 1) damage = 1;

    actual = 0;

    /* ── MDC defender ────────────────────────────────────────────────────── */

    if(is_mdc_def) {
        /* Check for worn MDC armor first (bypassed by armor_bypass weapons) */
        armor = armor_bypass ? 0 : find_worn_armor(defender, "mdc_armor");
        if(armor) {
            init_armor_hp(armor);
            cur_mdc = (int)armor->query_property("current_armor_mdc");
            if(cur_mdc > 0) {
                actual   = (damage <= cur_mdc) ? damage : cur_mdc;
                cur_mdc -= damage;
                if(cur_mdc < 0) cur_mdc = 0;
                armor->set_property("current_armor_mdc", cur_mdc);
                if(cur_mdc == 0) {
                    message("my_combat",
                        "Your " + (string)armor->query_name() + " is destroyed!",
                        defender);
                    tell_observers_subject(environment(defender), defender,
                        ({ defender }), "$S's armor is destroyed!\n");
                    destroy_armor(armor, defender, environment(defender));
                    armor = 0;
                }
                /* Overflow to character MDC (allow negative for death check) */
                if(actual < damage) {
                    overflow = damage - actual;
                    cur_mdc  = (int)defender->query_stats("MDC");
                    cur_mdc -= overflow;
                    actual  += overflow;
                    defender->set_stats("MDC", cur_mdc);
                }
            } else {
                /* Armor destroyed  -  hit MDC pool directly */
                cur_mdc = (int)defender->query_stats("MDC");
                actual  = damage;
                cur_mdc -= damage;
                defender->set_stats("MDC", cur_mdc);
            }
        } else {
            /* No armor  -  MDC weapon hits MDC pool directly */
            cur_mdc = (int)defender->query_stats("MDC");
            actual  = damage;
            cur_mdc -= damage;
            defender->set_stats("MDC", cur_mdc);
        }
        send_mdc_hit_messages(attacker, defender, actual);
        if(actual > 0) damage_trapped_net(defender, actual);
        cur_mdc = (int)defender->query_stats("MDC");
        if(cur_mdc <= -20) { /* approx: -20 MDC = dead */
            message("my_combat", "You are destroyed!", defender);
            tell_observers_subject(environment(defender), defender,
                ({ defender }), "$S is destroyed!\n");
            defender->cease_all_attacks();
            defender->die();
        } else if(cur_mdc <= 0) {
            message("my_combat", "You fall unconscious!", defender);
            tell_observers_subject(environment(defender), defender,
                ({ defender }), "$S collapses, unconscious!\n");
            defender->cease_all_attacks();
        }
        return -1;
    }

    /* ── SDC defender ────────────────────────────────────────────────────── */

    /* SDC beings wearing MDC armor: MDC armor protects regardless of race rating. */
    /* SDC weapons cannot breach MDC armor; MDC weapons drain the armor pool first. */
    {
        object mdc_arm;
        int mdc_cur;
        int mdc_overflow;
        int sdc_hit2;

        mdc_arm     = armor_bypass ? 0 : find_worn_armor(defender, "mdc_armor");
        mdc_overflow = 0;
        sdc_hit2    = 0;
        if(mdc_arm) {
            if(!has_mdc_weapon && !is_mdc_atk) {
                message("my_combat",
                    "Your attack bounces off " +
                    id_name(defender, attacker) + "'s MDC armor!", attacker);
                message("my_combat",
                    id_name(attacker, defender) +
                    "'s attack bounces off your MDC armor!", defender);
                tell_observers_combat(environment(attacker), attacker, defender,
                    "$A's attack bounces harmlessly off $D's MDC armor.\n");
                return -1;
            }
            init_armor_hp(mdc_arm);
            mdc_cur = (int)mdc_arm->query_property("current_armor_mdc");
            actual  = (damage <= mdc_cur) ? damage : mdc_cur;
            mdc_cur -= damage;
            if(mdc_cur < 0) mdc_cur = 0;
            mdc_arm->set_property("current_armor_mdc", mdc_cur);
            if(mdc_cur == 0) {
                message("my_combat",
                    "Your " + (string)mdc_arm->query_name() + " is destroyed!",
                    defender);
                tell_observers_subject(environment(defender), defender,
                    ({ defender }), "$S's armor is destroyed!\n");
                destroy_armor(mdc_arm, defender, environment(defender));
            }
            mdc_overflow = (actual < damage) ? (damage - actual) : 0;
            if(mdc_overflow > 0) {
                cur_sdc  = (int)defender->query_stats("SDC");
                if(cur_sdc > 0) {
                    sdc_hit2 = (mdc_overflow <= cur_sdc) ? mdc_overflow : cur_sdc;
                    actual  += sdc_hit2;
                    cur_sdc -= mdc_overflow;
                    if(cur_sdc < 0) cur_sdc = 0;
                    defender->set_stats("SDC", cur_sdc);
                    mdc_overflow -= sdc_hit2;
                }
                if(mdc_overflow > 0) {
                    cur_hp  = (int)defender->query_stats("rifts_hp");
                    actual += mdc_overflow;
                    cur_hp -= mdc_overflow;
                    defender->set_stats("rifts_hp", cur_hp);
                }
            }
            send_rifts_messages(attacker, defender, actual);
            if(actual > 0) damage_trapped_net(defender, actual);
            cur_hp = (int)defender->query_stats("rifts_hp");
            if(cur_hp <= -10) {
                message("my_combat", "You die!", defender);
                tell_observers_subject(environment(defender), defender,
                    ({ defender }), "$S dies!\n");
                defender->cease_all_attacks();
                defender->die();
            } else if(cur_hp <= 0) {
                message("my_combat", "You fall unconscious!", defender);
                tell_observers_subject(environment(defender), defender,
                    ({ defender }), "$S collapses, unconscious!\n");
                defender->cease_all_attacks();
            }
            return -1;
        }
    }

    /* Check for worn SDC armor (bypassed by armor_bypass weapons) */
    armor = armor_bypass ? 0 : find_worn_armor(defender, "sdc_armor");
    if(armor && !has_mdc_weapon) {
        /* SDC weapon hits SDC armor */
        int ar;
        int ar_roll;
        ar      = (int)armor->query_property("armor_ar");
        ar_roll = ar_bypass_half ? ar / 2 : ar;
        if(!penetrate_ar && ar_roll > 0 && random(20) + 1 < ar_roll) {
            /* Glancing blow  -  armor stopped it */
            message("my_combat",
                "Your attack glances off " +
                id_name(defender, attacker) + "'s armor.", attacker);
            message("my_combat",
                id_name(attacker, defender) +
                "'s attack glances off your armor.", defender);
            return -1;
        }
        init_armor_hp(armor);
        cur_sdc = (int)armor->query_property("current_armor_sdc");
        actual  = (damage <= cur_sdc) ? damage : cur_sdc;
        cur_sdc -= damage;
        if(cur_sdc < 0) cur_sdc = 0;
        armor->set_property("current_armor_sdc", cur_sdc);
        if(cur_sdc == 0) {
            message("my_combat",
                "Your " + (string)armor->query_name() + " is destroyed!",
                defender);
            tell_observers_subject(environment(defender), defender,
                ({ defender }), "$S's armor is destroyed!\n");
            destroy_armor(armor, defender, environment(defender));
            armor = 0;
        }
        /* Overflow to character SDC */
        if(actual < damage) {
            int leftover, sdc_hit, hp_hit;
            leftover = damage - actual;
            cur_sdc = (int)defender->query_stats("SDC");
            sdc_hit = (leftover <= cur_sdc) ? leftover : cur_sdc;
            actual   += sdc_hit;
            cur_sdc  -= leftover;
            if(cur_sdc < 0) cur_sdc = 0;
            defender->set_stats("SDC", cur_sdc);
            /* Overflow from SDC to rifts_hp */
            if(leftover > sdc_hit) {
                hp_hit = leftover - sdc_hit;
                cur_hp    = (int)defender->query_stats("rifts_hp");
                actual   += hp_hit;
                cur_hp   -= hp_hit;
                defender->set_stats("rifts_hp", cur_hp);
            }
        }
    } else if(has_mdc_weapon) {
        /* MDC weapon vs SDC armor: destroys SDC armor, hits SDC/HP directly */
        int hp_hit;
        hp_hit = 0;
        if(armor) {
            armor->set_property("current_armor_sdc", 0);
        }
        cur_sdc = (int)defender->query_stats("SDC");
        actual  = damage;
        cur_sdc -= damage;
        if(cur_sdc < 0) {
            hp_hit = -cur_sdc;
            cur_sdc = 0;
            cur_hp  = (int)defender->query_stats("rifts_hp");
            cur_hp -= hp_hit;
            defender->set_stats("rifts_hp", cur_hp);
        }
        defender->set_stats("SDC", cur_sdc);
    } else {
        /* No armor  -  hit SDC then HP */
        int sdc_hit, hp_over;
        sdc_hit = 0;
        hp_over = 0;
        cur_sdc = (int)defender->query_stats("SDC");
        actual  = damage;
        if(cur_sdc > 0) {
            sdc_hit = (damage <= cur_sdc) ? damage : cur_sdc;
            cur_sdc -= damage;
            if(cur_sdc < 0) {
                hp_over = -cur_sdc;
                cur_sdc = 0;
                cur_hp  = (int)defender->query_stats("rifts_hp");
                cur_hp -= hp_over;
                defender->set_stats("rifts_hp", cur_hp);
            }
            defender->set_stats("SDC", cur_sdc);
        } else {
            cur_hp  = (int)defender->query_stats("rifts_hp");
            cur_hp -= damage;
            defender->set_stats("rifts_hp", cur_hp);
        }
    }

    send_rifts_messages(attacker, defender, actual);
    if(actual > 0) damage_trapped_net(defender, actual);

    cur_hp = (int)defender->query_stats("rifts_hp");
    if(cur_hp <= -10) { /* approx: -10 HP = dead */
        message("my_combat", "You die!", defender);
        tell_observers_subject(environment(defender), defender,
            ({ defender }), "$S dies!\n");
        defender->cease_all_attacks();
        defender->die();
    } else if(cur_hp <= 0) {
        message("my_combat", "You fall unconscious!", defender);
        tell_observers_subject(environment(defender), defender,
            ({ defender }), "$S collapses, unconscious!\n");
        defender->cease_all_attacks();
    }
    return -1;
}

/* Apply raw MDC damage from a vehicle-mounted weapon.                         */
/* Always treated as MDC weapon: bypasses has_mdc_weapon check.                */
/* MDC defender: drains worn MDC armor first, overflow to MDC pool.            */
/* SDC defender: drains MDC armor if worn, then SDC then rifts_hp.             */
/* weapon_desc: short name of the weapon for message strings ("particle beam").*/
/* Returns -1 (messages already sent). */
int apply_vehicle_mdc_damage(object defender, int raw_damage, string weapon_desc) {
    int cur_mdc;
    int cur_sdc;
    int cur_hp;
    int overflow;
    int actual;
    int hp_hit;
    object armor;
    object env;

    if(!defender || !objectp(defender) || raw_damage < 1) return 0;
    env      = environment(defender);
    actual   = raw_damage;
    overflow = 0;
    hp_hit   = 0;

    if(!weapon_desc || !strlen(weapon_desc)) weapon_desc = "vehicle weapon";

    if((int)RIFTS_D->is_mdc_race((string)defender->query_race()) ||
       (int)defender->query_property("mdc_creature")) {

        armor = find_worn_armor(defender, "mdc_armor");
        if(armor) {
            init_armor_hp(armor);
            cur_mdc = (int)armor->query_property("current_armor_mdc");
            actual  = (raw_damage <= cur_mdc) ? raw_damage : cur_mdc;
            cur_mdc -= raw_damage;
            if(cur_mdc < 0) cur_mdc = 0;
            armor->set_property("current_armor_mdc", cur_mdc);
            if(cur_mdc == 0) {
                message("my_combat",
                    "Your " + (string)armor->query_name() + " is destroyed!",
                    defender);
                tell_observers_subject(env, defender,
                    ({ defender }), "$S's armor is destroyed!\n");
                destroy_armor(armor, defender, env);
            }
            if(actual < raw_damage) {
                overflow = raw_damage - actual;
                cur_mdc  = (int)defender->query_stats("MDC");
                cur_mdc -= overflow;
                actual  += overflow;
                defender->set_stats("MDC", cur_mdc);
            }
        } else {
            cur_mdc = (int)defender->query_stats("MDC");
            cur_mdc -= raw_damage;
            defender->set_stats("MDC", cur_mdc);
        }
        message("my_combat",
            sprintf("The %s hits you hard.", weapon_desc),
            defender);
        cur_mdc = (int)defender->query_stats("MDC");
        if(cur_mdc <= -20) {
            message("my_combat", "You are destroyed!", defender);
            tell_observers_subject(env, defender,
                ({ defender }), "$S is destroyed!\n");
            defender->cease_all_attacks();
            defender->die();
        } else if(cur_mdc <= 0) {
            message("my_combat", "You fall unconscious!", defender);
            tell_observers_subject(env, defender,
                ({ defender }), "$S collapses, unconscious!\n");
            defender->cease_all_attacks();
        }
        return -1;
    }

    /* SDC defender: MDC weapon energy bypasses SDC armor rating. */
    /* If wearing MDC armor, drain it first. Otherwise SDC then HP. */
    armor = find_worn_armor(defender, "mdc_armor");
    if(armor) {
        init_armor_hp(armor);
        cur_mdc = (int)armor->query_property("current_armor_mdc");
        actual  = (raw_damage <= cur_mdc) ? raw_damage : cur_mdc;
        cur_mdc -= raw_damage;
        if(cur_mdc < 0) cur_mdc = 0;
        armor->set_property("current_armor_mdc", cur_mdc);
        if(cur_mdc == 0) {
            message("my_combat",
                "Your " + (string)armor->query_name() + " is destroyed!",
                defender);
            tell_observers_subject(env, defender,
                ({ defender }), "$S's armor is destroyed!\n");
            destroy_armor(armor, defender, env);
        }
        if(actual < raw_damage) {
            overflow = raw_damage - actual;
            cur_sdc  = (int)defender->query_stats("SDC");
            cur_sdc -= overflow;
            if(cur_sdc < 0) {
                hp_hit  = -cur_sdc;
                cur_sdc = 0;
                cur_hp  = (int)defender->query_stats("rifts_hp");
                cur_hp -= hp_hit;
                defender->set_stats("rifts_hp", cur_hp);
            }
            defender->set_stats("SDC", cur_sdc);
        }
    } else {
        cur_sdc = (int)defender->query_stats("SDC");
        cur_sdc -= raw_damage;
        if(cur_sdc < 0) {
            hp_hit  = -cur_sdc;
            cur_sdc = 0;
            cur_hp  = (int)defender->query_stats("rifts_hp");
            cur_hp -= hp_hit;
            defender->set_stats("rifts_hp", cur_hp);
        }
        defender->set_stats("SDC", cur_sdc);
    }
    message("my_combat",
        sprintf("The %s hits you hard.", weapon_desc),
        defender);
    cur_hp = (int)defender->query_stats("rifts_hp");
    if(cur_hp <= -10) {
        message("my_combat", "You die!", defender);
        tell_observers_subject(env, defender,
            ({ defender }), "$S dies!\n");
        defender->cease_all_attacks();
        defender->die();
    } else if(cur_hp <= 0) {
        message("my_combat", "You fall unconscious!", defender);
        tell_observers_subject(env, defender,
            ({ defender }), "$S collapses, unconscious!\n");
        defender->cease_all_attacks();
    }
    return -1;
}

/* PPE and ISP regeneration; called from living.c heart_beat() once per hour. */
/* Active rate: 2/hour. Resting/meditating: 6/hour. Ley lines double PPE only. */
void do_rifts_regen(object player) {
    int max_ppe;
    int cur_ppe;
    int max_isp;
    int cur_isp;
    int is_rest;
    int base_regen;
    int ppe_regen;
    int isp_regen;
    object room;

    if(!player || !objectp(player)) return;
    max_ppe = (int)player->query_stats("max_PPE");
    max_isp = (int)player->query_stats("max_ISP");
    if(max_ppe <= 0 && max_isp <= 0) return;

    is_rest = ((string)player->getenv("resting") == "1" ||
               (int)player->query_property("meditating"));
    base_regen = is_rest ? 6 : 2;

    ppe_regen = base_regen;
    room = environment(player);
    if(room && (int)RIFTS_D->room_on_ley_line(room))
        ppe_regen *= 2;

    isp_regen = base_regen;

    if(max_ppe > 0) {
        cur_ppe = (int)player->query_stats("PPE");
        if(cur_ppe < max_ppe) {
            cur_ppe += ppe_regen;
            if(cur_ppe > max_ppe) cur_ppe = max_ppe;
            player->set_stats("PPE", cur_ppe);
        }
    }

    if(max_isp > 0) {
        cur_isp = (int)player->query_stats("ISP");
        if(cur_isp < max_isp) {
            cur_isp += isp_regen;
            if(cur_isp > max_isp) cur_isp = max_isp;
            player->set_stats("ISP", cur_isp);
        }
    }
}

/* ── NPC combat AI (minimal Rifts tactics) ─────────────────────────────────── */

private int query_rifts_health_pct(object who) {
    int cur_mdc;
    int max_mdc;
    int cur_hp;
    int max_hp;
    int cur_sdc;
    int max_sdc;

    if(!who || !objectp(who)) return 100;

    if((int)RIFTS_D->is_mdc_race((string)who->query_race()) ||
       (int)who->query_property("mdc_creature")) {
        cur_mdc = (int)who->query_stats("MDC");
        max_mdc = (int)who->query_stats("max_MDC");
        if(max_mdc > 0) return (cur_mdc * 100) / max_mdc;
    }

    cur_sdc = (int)who->query_stats("SDC");
    max_sdc = (int)who->query_stats("max_SDC");
    if(max_sdc > 0) return (cur_sdc * 100) / max_sdc;

    cur_hp = (int)who->query_stats("rifts_hp");
    max_hp = (int)who->query_stats("max_rifts_hp");
    if(max_hp > 0) return (cur_hp * 100) / max_hp;

    if((int)who->query_max_hp() > 0)
        return ((int)who->query_hp() * 100) / (int)who->query_max_hp();
    return 100;
}

private void npc_try_equip_ranged(object npc) {
    object *inv;
    object ob;
    object *wielded;
    int i;

    if((int)npc->query_property("rifts_no_ranged")) return;
    wielded = (object *)npc->query_wielded();
    if(wielded && sizeof(wielded) && wielded[0] &&
       (int)wielded[0]->query_property("ranged")) return;

    inv = all_inventory(npc);
    if(!inv) return;
    for(i = 0; i < sizeof(inv); i++) {
        ob = inv[i];
        if(!ob || !(int)ob->is_weapon()) continue;
        if(!(int)ob->query_property("ranged")) continue;
        if((int)ob->query_property("ammo_max") > 0 &&
           (int)ob->query_property("current_ammo") <= 0) continue;
        npc->force_me("wield " + (string)ob->query_name() + " in right hand");
        return;
    }
}

private void npc_try_psionic(object npc, object foe) {
    string *psis;
    string psi;
    mapping pdata;
    int isp;
    int cost;
    int damage;
    int last;
    int idx;

    if(!npc || !foe || !(int)npc->query_property("psionic")) return;
    last = (int)npc->query_property("rifts_psi_last");
    if(last > 0 && time() - last < 30) return;
    if(!(int)can_do_attack(npc)) return;

    psis = (string *)npc->query_property("rifts_psionics");
    if(!psis || !pointerp(psis) || !sizeof(psis))
        psis = ({ "mind bolt" });

    idx = random(sizeof(psis));
    psi = (string)RIFTS_PSIONICS_D->normalize_psionic_name(psis[idx]);
    pdata = (mapping)RIFTS_PSIONICS_D->query_psionic(psi);
    if(!mapp(pdata)) return;

    cost = (int)pdata["isp_cost"];
    isp  = (int)npc->query_stats("ISP");
    if(isp < cost) return;

    use_rifts_attack(npc);
    npc->set_stats("ISP", isp - cost);
    npc->set_property("rifts_psi_last", time());

    if((string)pdata["effect"] == "mind_bolt_heavy")
        damage = random(6) + 1 + random(6) + 1 + random(6) + 1 +
                 random(6) + 1 + random(6) + 1 + random(6) + 1;
    else if((string)pdata["effect"] == "pyro_bolt" ||
            (string)pdata["effect"] == "pyro_blast")
        damage = random(6) + 1 + random(6) + 1 + random(6) + 1;
    else
        damage = random(6) + 1 + random(6) + 1 + random(6) + 1;

    tell_observers_combat(environment(npc), npc, foe,
        "$A unleashes " + psi + " at $D!\n");
    apply_rifts_damage(npc, foe, damage);
}

/* ── Alignment shift on NPC death ──────────────────────────────────────────── */
/* Called from std/monster.c::die() when an NPC is killed by a player.         */
/* Shifts the player's alignment slightly based on the slain NPC's moral stance.*/
/* Palladium thresholds (matching score.c):                                     */
/*   Clearly good NPC: alignment > 400 (Scrupulous/Principled)                 */
/*     -> killer loses 5 to 15 alignment (wanton killing of innocents)          */
/*   Clearly evil NPC: alignment <= -300 (Miscreant/Aberrant/Diabolic)         */
/*     -> killer gains 5 to 10 alignment (ridding the world of evil)            */
/*   Neutral/Anarchist/Unprincipled: no shift                                   */
/* The rifts_alignment env var label is synced to match the new numeric value.  */

void npc_died(object npc, object killer) {
    int npc_al;
    int shift;
    int al;
    string new_name;

    if(!npc || !killer) return;
    if(!interactive(killer)) return;

    npc_al = (int)npc->query_alignment();
    shift  = 0;

    if(npc_al > 400) {
        /* Killed a clearly good being */
        shift = -(5 + random(11));
        killer->add_alignment(shift);
        tell_object(killer, "You feel a twinge of remorse.\n");
    } else if(npc_al <= -300) {
        /* Killed a Miscreant, Aberrant, or Diabolic being */
        shift = 5 + random(6);
        killer->add_alignment(shift);
        tell_object(killer, "Ridding the world of that felt right.\n");
    }

    if(!shift) return;

    /* Keep the rifts_alignment env var label in sync with the new value. */
    al = (int)killer->query_alignment();
    if(al > 800)        new_name = "Principled";
    else if(al > 400)   new_name = "Scrupulous";
    else if(al > 0)     new_name = "Unprincipled";
    else if(al > -300)  new_name = "Anarchist";
    else if(al > -700)  new_name = "Miscreant";
    else if(al > -1100) new_name = "Aberrant";
    else                new_name = "Diabolic";
    killer->setenv("rifts_alignment", new_name);
}

void npc_combat_tick(object npc) {
    object foe;
    int flee_pct;
    int health_pct;
    mixed aggr;

    if(!npc || !objectp(npc)) return;
    if(interactive(npc)) return;
    if(!(int)RIFTS_D->is_rifts_race((string)npc->query_race())) return;

    foe = (object)npc->query_current_attacker();
    if(!foe || !objectp(foe)) return;
    if(environment(foe) != environment(npc)) return;

    if((int)npc->query_property("rifts_prefers_ranged"))
        npc_try_equip_ranged(npc);

    if(!(int)npc->query_property("rifts_no_flee")) {
        flee_pct = (int)npc->query_property("rifts_flee_pct");
        if(flee_pct <= 0) {
            aggr = npc->query_aggressive();
            if(intp(aggr) && aggr > 0) flee_pct = 0;
            else flee_pct = 20;
        }
        health_pct = query_rifts_health_pct(npc);
        if(flee_pct > 0 && health_pct < flee_pct && !random(3)) {
            message("other_combat",
                npc->query_cap_name() + " breaks off and retreats!",
                environment(npc), ({ npc, foe }));
            npc->cease_all_attacks();
            npc->run_away();
            return;
        }
    }

    if((int)npc->query_property("psionic") && !random(4))
        npc_try_psionic(npc, foe);
}
