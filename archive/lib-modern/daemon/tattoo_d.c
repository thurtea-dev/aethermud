// /daemon/tattoo_d.c
// RiftsMUD  -  Atlantean Tattoo Magic system daemon.
// Tattoos are stored per-player as: setenv("tattoos", mapping(location -> name))
// All 16 body slots. Sunaj Assassin gets 20 slots.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private mapping __tattoos;

private void _define_tattoos();
private void _apply_passive_bonus(object player, string name, mapping def, int remove_flag);
private void _do_weapon(object player, string name, mapping def, string location, mapping active);
private void _do_armor(object player, string name, mapping def, string location, mapping active);
private void _do_power(object player, string name, mapping def, string location, mapping active, object target);

void create() {
    daemon::create();
    set_no_clean(1);
    __tattoos = ([]);
    _define_tattoos();
}

// ── Tattoo master list ────────────────────────────────────────────────────
// type: "weapon" | "armor" | "power" | "passive"
// subtype (power only): "light" | "sight" | "attack" | "stealth" | "heal"

private void _define_tattoos() {
    // WEAPON TATTOOS
    __tattoos["flaming sword"] = ([
        "type":       "weapon",
        "category":   "Weapons",
        "ppe":        10,
        "locations":  ({ "right forearm", "left forearm" }),
        "duration":   300,
        "weapon_file": "/obj/tattoo_weapons/flaming_sword",
        "desc":       "Summons a flaming sword (3d6 MDC fire, 5 min). PPE 10."
    ]);
    __tattoos["flaming arrow"] = ([
        "type":       "weapon",
        "category":   "Weapons",
        "ppe":        6,
        "locations":  ({ "right hand", "left hand" }),
        "duration":   0,
        "weapon_file": "/obj/tattoo_weapons/flaming_arrow",
        "desc":       "Creates one magical fire arrow (1d6 MDC). PPE 6."
    ]);
    __tattoos["dagger"] = ([
        "type":       "weapon",
        "category":   "Weapons",
        "ppe":        4,
        "locations":  ({ "right hand", "left hand" }),
        "duration":   600,
        "weapon_file": "/obj/tattoo_weapons/tattoo_dagger",
        "desc":       "Summons a magical dagger (1d6 MDC, 10 min). PPE 4."
    ]);
    __tattoos["spear"] = ([
        "type":       "weapon",
        "category":   "Weapons",
        "ppe":        8,
        "locations":  ({ "right forearm", "left forearm" }),
        "duration":   600,
        "weapon_file": "/obj/tattoo_weapons/tattoo_spear",
        "desc":       "Summons a magical spear (2d6 MDC, 10 min). PPE 8."
    ]);
    __tattoos["battle axe"] = ([
        "type":       "weapon",
        "category":   "Weapons",
        "ppe":        8,
        "locations":  ({ "right forearm", "left forearm" }),
        "duration":   600,
        "weapon_file": "/obj/tattoo_weapons/tattoo_battle_axe",
        "desc":       "Summons a magical battle axe (2d6 MDC, 10 min). PPE 8."
    ]);

    // ARMOR TATTOOS
    __tattoos["globe of invulnerability"] = ([
        "type":       "armor",
        "category":   "Armor",
        "ppe":        20,
        "locations":  ({ "chest" }),
        "duration":   120,
        "mdc_bonus":  50,
        "desc":       "+50 MDC force field (2 min). PPE 20."
    ]);
    __tattoos["armor of ithan"] = ([
        "type":       "armor",
        "category":   "Armor",
        "ppe":        10,
        "locations":  ({ "back" }),
        "duration":   300,
        "mdc_bonus":  30,
        "desc":       "+30 MDC magical field (5 min). PPE 10."
    ]);
    __tattoos["shadow armor"] = ([
        "type":       "armor",
        "category":   "Armor",
        "ppe":        10,
        "locations":  ({ "back" }),
        "duration":   300,
        "mdc_bonus":  25,
        "desc":       "+25 MDC shadow-force field (5 min). PPE 10. Sunaj."
    ]);
    __tattoos["flaming shield"] = ([
        "type":       "armor",
        "category":   "Armor",
        "ppe":        15,
        "locations":  ({ "right forearm", "left forearm", "chest" }),
        "duration":   300,
        "mdc_bonus":  40,
        "desc":       "+40 MDC shield of flame (5 min). PPE 15."
    ]);
    __tattoos["turtle"] = ([
        "type":       "armor",
        "category":   "Armor",
        "ppe":        8,
        "locations":  ({ "back", "chest" }),
        "duration":   600,
        "mdc_bonus":  20,
        "desc":       "+20 MDC protective shell (10 min). PPE 8."
    ]);

    // POWER TATTOOS
    __tattoos["globe of daylight"] = ([
        "type":       "power",
        "subtype":    "light",
        "category":   "Power",
        "ppe":        2,
        "locations":  ({ "face", "neck" }),
        "duration":   1800,
        "desc":       "Illuminates dark rooms (30 min). PPE 2."
    ]);
    __tattoos["eyes of the wolf"] = ([
        "type":       "power",
        "subtype":    "sight",
        "category":   "Power",
        "ppe":        4,
        "locations":  ({ "face" }),
        "duration":   3600,
        "desc":       "See in darkness and see invisible (1 hour). PPE 4."
    ]);
    __tattoos["fire bolt"] = ([
        "type":       "power",
        "subtype":    "attack",
        "category":   "Power",
        "ppe":        8,
        "locations":  ({ "right hand", "left hand" }),
        "duration":   0,
        "desc":       "Fire bolt 4d6 MDC at target. touch <loc> tattoo <target>. PPE 8."
    ]);
    __tattoos["shadow meld"] = ([
        "type":       "power",
        "subtype":    "stealth",
        "category":   "Power",
        "ppe":        6,
        "locations":  ({ "back", "left shoulder" }),
        "duration":   600,
        "desc":       "Invisible in shadow (10 min). PPE 6."
    ]);
    __tattoos["super healing"] = ([
        "type":       "power",
        "subtype":    "heal",
        "category":   "Power",
        "ppe":        15,
        "locations":  ({ "chest", "back" }),
        "duration":   0,
        "desc":       "Restore 3d6 MDC/HP instantly. No combat. PPE 15."
    ]);
    __tattoos["chain with broken link"] = ([
        "type":       "power",
        "subtype":    "strength",
        "category":   "Power",
        "ppe":        12,
        "locations":  ({ "right upper arm", "left upper arm" }),
        "duration":   600,
        "ps_bonus":   10,
        "desc":       "Supernatural strength: +10 PS (10 min). PPE 12."
    ]);
    __tattoos["eye of knowledge"] = ([
        "type":       "power",
        "subtype":    "knowledge",
        "category":   "Power",
        "ppe":        8,
        "locations":  ({ "face", "neck" }),
        "duration":   1800,
        "desc":       "Understand all spoken languages (30 min). PPE 8."
    ]);
    __tattoos["phoenix rising from flames"] = ([
        "type":       "power",
        "subtype":    "heal",
        "category":   "Power",
        "ppe":        25,
        "locations":  ({ "chest", "back" }),
        "duration":   0,
        "heal_num":   6,
        "desc":       "Restore 6d6 MDC/HP instantly. No combat. PPE 25."
    ]);

    // PASSIVE TATTOOS
    __tattoos["heart"] = ([
        "type":       "passive",
        "category":   "Passive",
        "ppe":        0,
        "locations":  ({ "chest", "back" }),
        "duration":   0,
        "bonus_hp":   10,
        "desc":       "+10 HP permanently."
    ]);
    __tattoos["skull"] = ([
        "type":       "passive",
        "category":   "Passive",
        "ppe":        0,
        "locations":  ({ "right shoulder", "left shoulder", "face" }),
        "duration":   0,
        "bonus_apm":  1,
        "desc":       "+1 APM in combat."
    ]);
    __tattoos["dragon"] = ([
        "type":       "passive",
        "category":   "Passive",
        "ppe":        0,
        "locations":  ({ "back", "chest" }),
        "duration":   0,
        "bonus_mdc":  20,
        "desc":       "+20 MDC permanently."
    ]);
    __tattoos["angel"] = ([
        "type":       "passive",
        "category":   "Passive",
        "ppe":        0,
        "locations":  ({ "back" }),
        "duration":   0,
        "bonus_save_magic": 2,
        "bonus_save_psi":   2,
        "desc":       "+2 save vs magic, +2 save vs psionics."
    ]);
    __tattoos["one hundred eyes"] = ([
        "type":       "passive",
        "category":   "Passive",
        "ppe":        0,
        "locations":  ({ "chest", "back" }),
        "duration":   0,
        "sunaj_only": 1,
        "desc":       "See all invisibles, detect ambush. Sunaj Assassin only."
    ]);
    __tattoos["heart pierced by a stake"] = ([
        "type":       "passive",
        "category":   "Passive",
        "ppe":        0,
        "locations":  ({ "chest" }),
        "duration":   0,
        "bonus_save_magic": 2,
        "desc":       "Atlantean birthright ward. +2 save vs magic, vampires cannot turn you."
    ]);
    __tattoos["serpent ring"] = ([
        "type":       "passive",
        "category":   "Passive",
        "ppe":        0,
        "locations":  ({ "right upper arm", "left upper arm", "neck" }),
        "duration":   0,
        "desc":       "A snake swallowing its own tail. Immune to poison and disease."
    ]);
}

// ── Internal helpers ──────────────────────────────────────────────────────

private string *all_locations() {
    return ({
        "right forearm", "left forearm", "right upper arm", "left upper arm",
        "right hand",    "left hand",    "chest",           "back",
        "right shoulder","left shoulder",
        "right leg",     "left leg",     "right calf",      "left calf",
        "neck",          "face"
    });
}

private mapping _get_tattoos(object player) {
    mixed t;
    if(!player) return ([]);
    t = player->getenv("tattoos");
    return (mapp(t) ? t : ([]));
}

private void _save_tattoos(object player, mapping t) {
    if(!player) return;
    player->setenv("tattoos", t);
}

private int _max_slots(object player) {
    string occ;
    occ = lower_case((string)player->getenv("rifts_occ"));
    if(occ == "sunaj assassin") return 20;
    /* Also honor a direct env override (set by _clan.c) */
    {
        int override;
        override = to_int((string)player->getenv("tattoo_slots"));
        if(override > 16) return override;
    }
    return 16;
}

// ── Public API ────────────────────────────────────────────────────────────

// Returns the full tattoo definition mapping, keyed by name.
mapping query_all_tattoos() { return __tattoos + ([]); }

// Returns one tattoo's definition, or 0 if unknown.
mapping query_tattoo_def(string name) {
    if(!name) return 0;
    return __tattoos[lower_case(name)];
}

// Returns player's tattoos: mapping of location -> name.
mapping query_tattoos(object player) {
    return _get_tattoos(player);
}

// Returns the tattoo name at a specific location, or 0 if empty.
string query_tattoo_at(object player, string location) {
    mapping t;
    if(!player || !location) return 0;
    t = _get_tattoos(player);
    return t[lower_case(location)];
}

// Grant a tattoo at a body location. Returns 1 on success, 0 on failure.
int grant_tattoo(object player, string tattoo_name, string location) {
    mapping tattoos, def;
    string occ;
    int slots;

    if(!player || !tattoo_name || !location) return 0;
    tattoo_name = lower_case(tattoo_name);
    location    = lower_case(location);

    def = __tattoos[tattoo_name];
    if(!def) {
        if(this_player()) write("Unknown tattoo: " + tattoo_name + "\n");
        return 0;
    }

    if(member_array(location, all_locations()) == -1) {
        if(this_player()) write("Invalid body location: " + location + "\n");
        return 0;
    }

    if(member_array(location, (string *)def["locations"]) == -1) {
        if(this_player())
            write(capitalize(tattoo_name) + " cannot go on the " + location + ".\n"
                  "Valid locations: " +
                  implode((string *)def["locations"], ", ") + "\n");
        return 0;
    }

    tattoos = _get_tattoos(player);

    if(tattoos[location]) {
        if(this_player())
            write((string)player->query_cap_name() + " already has a " +
                  tattoos[location] + " tattoo on the " + location + ".\n");
        return 0;
    }

    slots = _max_slots(player);
    if(sizeof(keys(tattoos)) >= slots) {
        if(this_player())
            write((string)player->query_cap_name() + " has no free tattoo slots (" +
                  slots + " max).\n");
        return 0;
    }

    if((int)def["sunaj_only"]) {
        occ = lower_case((string)player->getenv("rifts_occ"));
        if(occ != "sunaj assassin" && this_player())
            write("WARNING: " + tattoo_name + " is Sunaj Assassin only. "
                  "Proceeding anyway as requested.\n");
    }

    tattoos[location] = tattoo_name;
    _save_tattoos(player, tattoos);

    // Apply passive bonus immediately on inscription
    if((string)def["type"] == "passive")
        _apply_passive_bonus(player, tattoo_name, def, 0);

    tell_object(player,
        "The Tattoo Master inscribes a " + tattoo_name + " tattoo on your " +
        location + ".\n");

    log_file("rp_wizard/tattoos",
        (this_player() ? (string)this_player()->query_name() : "system") +
        " granted '" + tattoo_name + "' on " + location + " to " +
        (string)player->query_name() + ": " + ctime(time()) + "\n");

    catch(player->save_player((string)player->query_name()));

    return 1;
}

// Remove a tattoo at a body location. Deals 2d6 HP damage.
int remove_tattoo(object player, string location) {
    mapping tattoos, def;
    string name;
    int damage, cur_hp;

    if(!player || !location) return 0;
    location = lower_case(location);
    tattoos  = _get_tattoos(player);

    name = tattoos[location];
    if(!name) {
        if(this_player())
            write((string)player->query_cap_name() + " has no tattoo on the " +
                  location + ".\n");
        return 0;
    }

    def = __tattoos[name];
    if(def && (string)def["type"] == "passive")
        _apply_passive_bonus(player, name, def, 1);

    map_delete(tattoos, location);
    _save_tattoos(player, tattoos);

    // 2d6 HP pain damage
    damage  = random(6) + random(6) + 2;
    cur_hp  = (int)player->query_stats("rifts_hp");
    cur_hp -= damage;
    if(cur_hp < 1) cur_hp = 1;
    player->set_stats("rifts_hp", cur_hp);

    tell_object(player,
        "The Tattoo Master removes a tattoo. It hurts. (" + damage + " damage)\n");

    log_file("rp_wizard/tattoos",
        (this_player() ? (string)this_player()->query_name() : "system") +
        " removed '" + name + "' from " + location + " on " +
        (string)player->query_name() + ": " + ctime(time()) + "\n");

    return 1;
}

// Activate the tattoo at a body location. Returns 1 if activated, 0 if not.
int activate_tattoo(object player, string location, object target) {
    mapping tattoos, def, active;
    string name, type;
    int ppe, cost, expiry;
    object *atk;

    if(!player || !location) return 0;
    location = lower_case(location);
    tattoos  = _get_tattoos(player);
    name     = tattoos[location];

    if(!name) {
        tell_object(player, "You have no tattoo on your " + location + ".\n");
        return 0;
    }

    def = __tattoos[name];
    if(!def) {
        tell_object(player, "That tattoo has no defined effect.\n");
        return 0;
    }

    type = (string)def["type"];

    if(type == "passive") {
        tell_object(player, "That tattoo is always active.\n");
        return 1;
    }

    // Check if already active (for timed tattoos)
    active = player->getenv("tattoo_active");
    if(!mapp(active)) active = ([]);
    expiry = active[location] ? (int)active[location] : 0;
    if(expiry > time() && (int)def["duration"] > 0) {
        tell_object(player,
            "The " + name + " tattoo is already active for " +
            ((expiry - time()) / 60 + 1) + " more minute(s).\n");
        return 0;
    }

    // PPE check
    cost = (int)def["ppe"];
    ppe  = (int)player->query_stats("PPE");
    if(ppe < cost) {
        tell_object(player,
            "You need " + cost + " PPE to activate this tattoo. You have " +
            ppe + ".\n");
        return 0;
    }
    /* Attack-type power tattoos require active combat; auto-target first opponent. */
    if(type == "power" && (string)def["subtype"] == "attack") {
        atk = (object *)player->query_attackers();
        if(sizeof(atk) == 0) {
            tell_object(player,
                "You can only use that tattoo in combat.\n");
            return 0;
        }
        if(!target) target = atk[0];
    }
    player->set_stats("PPE", ppe - cost);

    switch(type) {
    case "weapon": _do_weapon(player, name, def, location, active); break;
    case "armor":  _do_armor(player,  name, def, location, active); break;
    case "power":  _do_power(player,  name, def, location, active, target); break;
    }
    return 1;
}

// ── Activation handlers ───────────────────────────────────────────────────

private void _do_weapon(object player, string name, mapping def,
                         string location, mapping active) {
    string wfile;
    object weap;
    int dur;

    wfile = (string)def["weapon_file"];
    if(!wfile) {
        tell_object(player, "The tattoo glows but produces nothing.\n");
        return;
    }

    weap = clone_object(wfile);
    if(!weap) {
        tell_object(player, "The tattoo flickers but fails to manifest.\n");
        return;
    }

    /* init_tattoo_owner handles move, auto-wield, and weapon timeout */
    weap->init_tattoo_owner(player);
    player->set_property("tattoo_sword_active", 1);

    tell_object(player,
        "The " + name + " tattoo blazes and a weapon materializes!\n");
    say((string)player->query_cap_name() +
        "'s " + name + " tattoo flares and a weapon appears!",
        player);

    dur = (int)def["duration"];
    if(dur > 0) {
        active[location] = time() + dur;
        player->setenv("tattoo_active", active);
    }
}

private void _do_armor(object player, string name, mapping def,
                        string location, mapping active) {
    int bonus, dur;

    bonus = (int)def["mdc_bonus"];
    dur   = (int)def["duration"];

    player->set_stats("max_MDC",
        (int)player->query_stats("max_MDC") + bonus);
    player->set_stats("MDC",
        (int)player->query_stats("MDC") + bonus);
    player->setenv("tattoo_field_" + location, "" + bonus);
    player->set_property("tattoo_armor_mdc",
        (int)player->query_property("tattoo_armor_mdc") + bonus);

    active[location] = time() + dur;
    player->setenv("tattoo_active", active);

    tell_object(player,
        "The " + name + " tattoo activates.  +" + bonus + " MDC field!\n");
    say((string)player->query_cap_name() +
        " activates a " + name + " tattoo.  A magical field blazes up!",
        player);

    call_out("_expire_armor", dur, player, location, bonus);
}

void _expire_armor(object player, string location, int bonus) {
    int cur, maxv, cur_bonus;
    mapping active;

    if(!player || !objectp(player)) return;

    cur  = (int)player->query_stats("MDC") - bonus;
    maxv = (int)player->query_stats("max_MDC") - bonus;
    if(cur < 0) cur = 0;
    player->set_stats("MDC",     cur);
    player->set_stats("max_MDC", maxv);
    player->setenv("tattoo_field_" + location, "0");
    cur_bonus = (int)player->query_property("tattoo_armor_mdc") - bonus;
    if(cur_bonus < 0) cur_bonus = 0;
    player->set_property("tattoo_armor_mdc", cur_bonus);

    active = player->getenv("tattoo_active");
    if(mapp(active)) {
        map_delete(active, location);
        player->setenv("tattoo_active", active);
    }
    tell_object(player, "Your " + location + " tattoo field fades.\n");
}

private void _do_power(object player, string name, mapping def,
                        string location, mapping active, object target) {
    string sub;
    int dur, heal, cur_mdc, max_mdc, cur_hp, max_hp, damage;
    int bonus, hnum, i;

    sub = (string)def["subtype"];
    dur = (int)def["duration"];

    switch(sub) {
    case "light":
        player->setenv("tattoo_has_light", "1");
        active[location] = time() + dur;
        player->setenv("tattoo_active", active);
        tell_object(player,
            "The globe of daylight tattoo activates.  Warm light radiates from you.\n");
        say((string)player->query_cap_name() +
            " glows with magical daylight!", player);
        call_out("_expire_power", dur, player, location, "tattoo_has_light", "");
        break;

    case "sight":
        player->setenv("tattoo_see_dark",  "1");
        player->setenv("tattoo_see_invis", "1");
        active[location] = time() + dur;
        player->setenv("tattoo_active", active);
        tell_object(player,
            "The eyes of the wolf tattoo activates.  "
            "Darkness thins and hidden things become visible.\n");
        call_out("_expire_sight", dur, player, location);
        break;

    case "attack":
        if(!target) {
            tell_object(player,
                "Fire bolt at whom?  Syntax: touch " + location +
                " tattoo <target>\n");
            return;
        }
        if(environment(target) != environment(player)) {
            tell_object(player, "Your target is not here.\n");
            return;
        }
        damage = random(6)+random(6)+random(6)+random(6)+4;
        tell_object(player,
            "You unleash a fire bolt from your " + name + " tattoo!  " +
            damage + " MDC!\n");
        tell_object(target,
            (string)player->query_cap_name() +
            " fires a bolt of magical flame at you!  " + damage + " MDC!\n");
        say((string)player->query_cap_name() +
            " fires a bolt of flame from a tattoo!",
            ({ player, target }));
        if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
            cur_mdc = (int)target->query_stats("MDC") - damage;
            target->set_stats("MDC", cur_mdc);
            if(cur_mdc <= -20) {
                tell_object(target, "The fire bolt destroys you!\n");
                catch(target->die());
            }
        } else {
            cur_hp = (int)target->query_hp() - damage;
            if(cur_hp < 0) {
                target->set_hp(0);
                catch(target->die());
            } else {
                target->set_hp(cur_hp);
            }
        }
        break;

    case "stealth":
        player->setenv("tattoo_shadow_meld", "1");
        player->set_invis(1);
        active[location] = time() + dur;
        player->setenv("tattoo_active", active);
        tell_object(player,
            "The shadow meld tattoo activates.  You slip into the shadows.\n");
        say((string)player->query_cap_name() +
            " blurs and vanishes into shadow!", player);
        call_out("_expire_power", dur, player, location, "tattoo_shadow_meld", "invis");
        break;

    case "strength":
        bonus = (int)def["ps_bonus"];
        if(bonus <= 0) bonus = 10;
        player->set_stats("PS", (int)player->query_stats("PS") + bonus);
        active[location] = time() + dur;
        player->setenv("tattoo_active", active);
        tell_object(player,
            "The " + name + " tattoo activates.  Supernatural strength "
            "floods your muscles!  (+" + bonus + " PS)\n");
        say((string)player->query_cap_name() +
            "'s muscles swell with supernatural strength!", player);
        call_out("_expire_strength", dur, player, location, bonus);
        break;

    case "knowledge":
        player->setenv("tattoo_all_languages", "1");
        active[location] = time() + dur;
        player->setenv("tattoo_active", active);
        tell_object(player,
            "The eye of knowledge tattoo opens.  "
            "Every tongue spoken around you becomes clear.\n");
        call_out("_expire_power", dur, player, location,
            "tattoo_all_languages", "");
        break;

    case "heal":
        if(sizeof((object *)player->query_attackers()) > 0) {
            tell_object(player,
                "You cannot use super healing while in combat.\n");
            return;
        }
        hnum = (int)def["heal_num"];
        if(hnum <= 0) hnum = 3;
        heal = hnum;
        for(i = 0; i < hnum; i++) heal += random(6);
        if((int)RIFTS_D->is_mdc_race((string)player->query_race())) {
            max_mdc = (int)player->query_stats("max_MDC");
            cur_mdc = (int)player->query_stats("MDC") + heal;
            if(cur_mdc > max_mdc) cur_mdc = max_mdc;
            player->set_stats("MDC", cur_mdc);
        } else {
            max_hp = (int)player->query_stats("max_rifts_hp");
            cur_hp = (int)player->query_stats("rifts_hp") + heal;
            if(cur_hp > max_hp) cur_hp = max_hp;
            player->set_stats("rifts_hp", cur_hp);
        }
        tell_object(player,
            "The " + name + " tattoo pulses.  +" + heal + " restored.\n");
        break;
    }
}

void _expire_strength(object player, string location, int bonus) {
    mapping active;
    int ps;

    if(!player || !objectp(player)) return;
    ps = (int)player->query_stats("PS") - bonus;
    if(ps < 1) ps = 1;
    player->set_stats("PS", ps);
    active = player->getenv("tattoo_active");
    if(mapp(active)) {
        map_delete(active, location);
        player->setenv("tattoo_active", active);
    }
    tell_object(player, "The supernatural strength drains from your muscles.\n");
}

void _expire_power(object player, string location, string env_key, string extra) {
    mapping active;
    if(!player || !objectp(player)) return;
    player->setenv(env_key, "0");
    if(extra == "invis") player->set_invis(0);
    active = player->getenv("tattoo_active");
    if(mapp(active)) {
        map_delete(active, location);
        player->setenv("tattoo_active", active);
    }
    tell_object(player, "Your " + location + " tattoo effect fades.\n");
}

void _expire_sight(object player, string location) {
    mapping active;
    if(!player || !objectp(player)) return;
    player->setenv("tattoo_see_dark",  "0");
    player->setenv("tattoo_see_invis", "0");
    active = player->getenv("tattoo_active");
    if(mapp(active)) {
        map_delete(active, location);
        player->setenv("tattoo_active", active);
    }
    tell_object(player, "The wolf-sight fades.  Darkness returns.\n");
}

// ── Passive bonus apply/remove ────────────────────────────────────────────
// remove_flag: 0 = apply, 1 = remove

private void _apply_passive_bonus(object player, string name, mapping def, int remove_flag) {
    int mult, tracked, delta;
    mult = remove_flag ? -1 : 1;

    switch(name) {
    case "heart": {
        int bonus;
        int max_hp, cur_hp;
        bonus  = (int)def["bonus_hp"] * mult;
        max_hp = (int)player->query_stats("max_rifts_hp") + bonus;
        cur_hp = (int)player->query_stats("rifts_hp") + bonus;
        if(cur_hp < 1) cur_hp = 1;
        player->set_stats("max_rifts_hp", max_hp);
        player->set_stats("rifts_hp", cur_hp);
        tracked = to_int((string)player->getenv("tattoo_hp_bonus"));
        player->setenv("tattoo_hp_bonus", "" + (tracked + bonus));
        break;
    }
    case "dragon": {
        int bonus;
        int max_mdc, cur_mdc;
        bonus   = (int)def["bonus_mdc"] * mult;
        max_mdc = (int)player->query_stats("max_MDC") + bonus;
        cur_mdc = (int)player->query_stats("MDC") + bonus;
        if(cur_mdc < 0) cur_mdc = 0;
        player->set_stats("max_MDC", max_mdc);
        player->set_stats("MDC", cur_mdc);
        tracked = to_int((string)player->getenv("tattoo_mdc_bonus"));
        player->setenv("tattoo_mdc_bonus", "" + (tracked + bonus));
        break;
    }
    case "skull": {
        int bonus;
        bonus   = (int)def["bonus_apm"] * mult;
        tracked = to_int((string)player->getenv("tattoo_apm_bonus"));
        player->setenv("tattoo_apm_bonus", "" + (tracked + bonus));
        break;
    }
    case "angel": {
        int sm, sp;
        sm = to_int((string)player->getenv("tattoo_save_magic_bonus"))
             + (int)def["bonus_save_magic"] * mult;
        sp = to_int((string)player->getenv("tattoo_save_psi_bonus"))
             + (int)def["bonus_save_psi"] * mult;
        player->setenv("tattoo_save_magic_bonus", "" + sm);
        player->setenv("tattoo_save_psi_bonus",   "" + sp);
        break;
    }
    case "one hundred eyes": {
        string v;
        v = remove_flag ? "0" : "1";
        player->setenv("tattoo_see_all_invis",  v);
        player->setenv("tattoo_detect_ambush",  v);
        break;
    }
    case "heart pierced by a stake": {
        int sm;
        sm = to_int((string)player->getenv("tattoo_save_magic_bonus"))
             + (int)def["bonus_save_magic"] * mult;
        player->setenv("tattoo_save_magic_bonus", "" + sm);
        break;
    }
    case "serpent ring": {
        string v;
        v = remove_flag ? "0" : "1";
        player->setenv("tattoo_poison_immune",  v);
        player->setenv("tattoo_disease_immune", v);
        break;
    }
    }
}

// ── Login hook ────────────────────────────────────────────────────────────

// Called from user.c on login. Safe to call multiple times per session.
void apply_passive_tattoos(object player) {
    mapping tattoos;
    string *locs;
    string name, occ;
    mapping def;
    int i, login_t;
    int should_hp, should_mdc, should_apm, should_sm, should_sp;
    int have_hp,   have_mdc,   have_apm,   have_sm,   have_sp;
    int diff_hp, diff_mdc;
    int cur_hp, max_hp, cur_mdc, max_mdc;
    int ppe_base;

    if(!player) return;

    // Per-session guard: only apply once per login session.
    login_t = (int)player->query_login_time();
    if(login_t > 0 &&
       (string)player->getenv("tattoos_applied_session") == "" + login_t)
        return;

    tattoos = _get_tattoos(player);
    locs    = keys(tattoos);

    should_hp = should_mdc = should_apm = should_sm = should_sp = 0;

    for(i = 0; i < sizeof(locs); i++) {
        name = tattoos[locs[i]];
        def  = __tattoos[name];
        if(!def || (string)def["type"] != "passive") continue;
        if((int)def["bonus_hp"])          should_hp  += (int)def["bonus_hp"];
        if((int)def["bonus_mdc"])         should_mdc += (int)def["bonus_mdc"];
        if((int)def["bonus_apm"])         should_apm += (int)def["bonus_apm"];
        if((int)def["bonus_save_magic"])  should_sm  += (int)def["bonus_save_magic"];
        if((int)def["bonus_save_psi"])    should_sp  += (int)def["bonus_save_psi"];
    }

    have_hp  = to_int((string)player->getenv("tattoo_hp_bonus"));
    have_mdc = to_int((string)player->getenv("tattoo_mdc_bonus"));
    have_apm = to_int((string)player->getenv("tattoo_apm_bonus"));
    have_sm  = to_int((string)player->getenv("tattoo_save_magic_bonus"));
    have_sp  = to_int((string)player->getenv("tattoo_save_psi_bonus"));

    diff_hp  = should_hp  - have_hp;
    diff_mdc = should_mdc - have_mdc;

    if(diff_hp != 0) {
        max_hp = (int)player->query_stats("max_rifts_hp") + diff_hp;
        cur_hp = (int)player->query_stats("rifts_hp");
        if(diff_hp > 0) cur_hp += diff_hp;
        if(cur_hp < 1) cur_hp = 1;
        player->set_stats("max_rifts_hp", max_hp);
        player->set_stats("rifts_hp", cur_hp);
        player->setenv("tattoo_hp_bonus", "" + should_hp);
    }

    if(diff_mdc != 0) {
        max_mdc = (int)player->query_stats("max_MDC") + diff_mdc;
        cur_mdc = (int)player->query_stats("MDC");
        if(diff_mdc > 0) cur_mdc += diff_mdc;
        if(cur_mdc < 0) cur_mdc = 0;
        player->set_stats("max_MDC", max_mdc);
        player->set_stats("MDC", cur_mdc);
        player->setenv("tattoo_mdc_bonus", "" + should_mdc);
    }

    if(should_apm != have_apm)
        player->setenv("tattoo_apm_bonus", "" + should_apm);
    if(should_sm != have_sm)
        player->setenv("tattoo_save_magic_bonus", "" + should_sm);
    if(should_sp != have_sp)
        player->setenv("tattoo_save_psi_bonus", "" + should_sp);

    // Handle env-flag passives
    for(i = 0; i < sizeof(locs); i++) {
        name = tattoos[locs[i]];
        if(name == "one hundred eyes") {
            player->setenv("tattoo_see_all_invis", "1");
            player->setenv("tattoo_detect_ambush", "1");
        }
        if(name == "serpent ring") {
            player->setenv("tattoo_poison_immune",  "1");
            player->setenv("tattoo_disease_immune", "1");
        }
    }

    /* PPE backfill for characters created before OCC PPE was set. */
    /* Only fires when max_PPE is still 0 and the character has tattoos. */
    if((int)player->query_stats("max_PPE") <= 0 && sizeof(locs) > 0) {
        occ      = lower_case((string)player->getenv("rifts_occ"));
        ppe_base = 20;
        if(occ == "sunaj assassin")      ppe_base = 30;
        else if(occ == "tattoo warrior") ppe_base = 40;
        else if(occ == "tattooed man")   ppe_base = 30;
        player->set_stats("PPE",     ppe_base);
        player->set_stats("max_PPE", ppe_base);
    }

    if(login_t > 0)
        player->setenv("tattoos_applied_session", "" + login_t);
}
