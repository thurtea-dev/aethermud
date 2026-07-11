// /daemon/faction_d.c
// RiftsMUD faction system.
//
// Two sub-systems:
//   1. Standing tracker: player reputation -1000..+1000 per faction.
//   2. Membership tracker: player membership in named factions.
//
// Standings factions: coalition, lazlo, new_camelot, black_market,
//                     splugorth, free_quebec, bandits
//
// Membership factions: coalition, ngr, triax, lazlo, tolkeen,
//                      splugorth, atlantean_nomads, aerihman*, sunaj*
//                      (* = secret, public=0)

#include <std.h>
#include <daemons.h>
#include <dirs.h>
#include <security.h>

inherit DAEMON;

#define SAVE_FILE (DIR_DAEMONS_SAVE + "/faction_d")

// ({ display_name, hostile_threshold, friendly_threshold })
#define FACTIONS ([ \
    "coalition":    ({ "Coalition States",  -100, 100 }), \
    "lazlo":        ({ "City of Lazlo",     -200, 100 }), \
    "new_camelot":  ({ "New Camelot",       -200, 100 }), \
    "black_market": ({ "Black Market",      -100,  50 }), \
    "splugorth":    ({ "Splugorth Empire",  -200, 200 }), \
    "free_quebec":  ({ "Free Quebec",       -200, 100 }), \
    "bandits":      ({ "Bandits",           -100,  50 }) \
])

// player_name -> mapping of faction -> int standing
mapping standings;

// faction_name -> ([ "display_name", "public", "description", "join_method",
//                     "members": ([ player_name: rank ]) ])
mapping faction_meta;

// player_name -> faction_name (primary faction membership)
mapping player_faction;

void save_data() { unguarded((: save_object, SAVE_FILE :)); }

private void _init_factions() {
    if(!mapp(faction_meta)) faction_meta = ([]);
    if(!mapp(player_faction)) player_faction = ([]);

    if(!faction_meta["coalition"])
        faction_meta["coalition"] = ([
            "display_name": "Coalition States",
            "public":       1,
            "description":  "The human supremacist Coalition States government and military.",
            "join_method":  "admin",
            "members":      ([])
        ]);
    if(!faction_meta["ngr"])
        faction_meta["ngr"] = ([
            "display_name": "New German Republic",
            "public":       1,
            "description":  "The militant NGR fighting off the Gargoyle Empire in Europe.",
            "join_method":  "admin",
            "members":      ([])
        ]);
    if(!faction_meta["triax"])
        faction_meta["triax"] = ([
            "display_name": "Triax Industries",
            "public":       1,
            "description":  "The NGR-allied megacorp supplying Europe with advanced weapons.",
            "join_method":  "admin",
            "members":      ([])
        ]);
    if(!faction_meta["lazlo"])
        faction_meta["lazlo"] = ([
            "display_name": "City of Lazlo",
            "public":       1,
            "description":  "The free, magic-tolerant city-state and ally to D-Bees.",
            "join_method":  "quest",
            "members":      ([])
        ]);
    if(!faction_meta["tolkeen"])
        faction_meta["tolkeen"] = ([
            "display_name": "Kingdom of Tolkeen",
            "public":       1,
            "description":  "The doomed magic kingdom that warred with the Coalition States.",
            "join_method":  "quest",
            "members":      ([])
        ]);
    if(!faction_meta["splugorth"])
        faction_meta["splugorth"] = ([
            "display_name": "Splugorth Empire",
            "public":       1,
            "description":  "The alien intelligence Splynncryth and its Atlantis empire.",
            "join_method":  "admin",
            "members":      ([])
        ]);
    if(!faction_meta["atlantean_nomads"])
        faction_meta["atlantean_nomads"] = ([
            "display_name": "Atlantean Nomads",
            "public":       1,
            "description":  "True Atlanteans wandering the megaverse to fight the Splugorth.",
            "join_method":  "quest",
            "members":      ([])
        ]);
    if(!faction_meta["aerihman"])
        faction_meta["aerihman"] = ([
            "display_name": "Aerihman",
            "public":       0,
            "description":  "Secret Atlantean intelligence organization.",
            "join_method":  "admin",
            "members":      ([])
        ]);
    if(!faction_meta["sunaj"])
        faction_meta["sunaj"] = ([
            "display_name": "Sunaj Assassins",
            "public":       0,
            "description":  "Shadowy Atlantean assassin cult, servants of the Sunaj.",
            "join_method":  "admin",
            "members":      ([])
        ]);
}

void create() {
    daemon::create();
    set_no_clean(1);
    standings      = ([]);
    faction_meta   = ([]);
    player_faction = ([]);
    if(file_size(SAVE_FILE + ".o") > 0)
        unguarded((: restore_object, SAVE_FILE :));
    _init_factions();
}

// ── Internal helpers ─────────────────────────────────────────────────────────

private string standing_tier(int val) {
    if(val <= -500) return "Kill on Sight";
    if(val <= -200) return "Hostile";
    if(val <= -50)  return "Unfriendly";
    if(val <   50)  return "Neutral";
    if(val <  200)  return "Friendly";
    if(val <  500)  return "Honored";
    return "Exalted";
}

private mapping player_map(string name) {
    mapping m;

    m = standings[name];
    if(!mapp(m)) {
        m = ([]);
        standings[name] = m;
    }
    return m;
}

// ── Public API ───────────────────────────────────────────────────────────────

void modify_standing(string player_name, string faction, int amount, string reason) {
    mapping pmap;
    int cur;

    if(!player_name || !strlen(player_name)) return;
    if(!faction || !strlen(faction)) return;
    if(!FACTIONS[faction]) return;

    player_name = lower_case(player_name);
    pmap = player_map(player_name);
    cur = pmap[faction];
    if(!intp(cur)) cur = 0;
    cur += amount;
    if(cur >  1000) cur =  1000;
    if(cur < -1000) cur = -1000;
    pmap[faction] = cur;

    log_file("faction",
        player_name + " [" + faction + " " + (amount >= 0 ? "+" : "") + amount +
        "=" + cur + "] " + reason + " " + ctime(time()) + "\n");
    save_data();
}

int query_standing(string player_name, string faction) {
    mapping pmap;
    int val;

    if(!player_name || !strlen(player_name)) return 0;
    pmap = standings[lower_case(player_name)];
    if(!mapp(pmap)) return 0;
    val = pmap[faction];
    if(!intp(val)) return 0;
    return val;
}

string query_standing_name(string player_name, string faction) {
    return standing_tier(query_standing(player_name, faction));
}

int is_hostile(string player_name, string faction) {
    mixed *fdata;
    int threshold;

    fdata = FACTIONS[faction];
    if(!fdata || !pointerp(fdata)) return 0;
    threshold = fdata[1];
    return query_standing(player_name, faction) <= threshold;
}

int is_friendly(string player_name, string faction) {
    mixed *fdata;
    int threshold;

    fdata = FACTIONS[faction];
    if(!fdata || !pointerp(fdata)) return 0;
    threshold = fdata[2];
    return query_standing(player_name, faction) >= threshold;
}

/* Returns 1 if Coalition forces should treat the player as an ally.
   CS membership, allied membership, or friendly CS standing counts. */
int is_cs_ally(object player) {
    string name;
    string faction;

    if(!player || !objectp(player)) return 0;
    name = lower_case((string)player->query_name());
    faction = player_faction[name];
    if(!faction) faction = "";
    if(faction == "coalition" || faction == "ngr" || faction == "triax")
        return 1;
    if(is_friendly(name, "coalition")) return 1;
    return 0;
}

// Object-based standing query for use in commands: query_rep(player_ob, faction)
int query_rep(object player, string faction) {
    if(!player || !objectp(player)) return 0;
    return query_standing(lower_case((string)player->query_name()), faction);
}

// Returns a mapping of faction -> standing for display
mapping query_all_standings(string player_name) {
    mapping pmap;
    mapping out;
    string *fkeys;
    int i;

    player_name = lower_case(player_name);
    pmap  = standings[player_name];
    fkeys = keys(FACTIONS);
    out   = ([]);
    for(i = 0; i < sizeof(fkeys); i++) {
        int val;
        val = (mapp(pmap) && intp(pmap[fkeys[i]])) ? pmap[fkeys[i]] : 0;
        out[fkeys[i]] = val;
    }
    return out;
}

// Apply race-based default standings on first character creation.
// Call from rifts_start_d after race is set.
void apply_race_defaults(object player) {
    string race, name;

    if(!player || !objectp(player)) return;
    name = lower_case((string)player->query_name());
    race = lower_case((string)player->query_race());

    if(race == "human" || race == "dog boy" || race == "cs psi-stalker") {
        // Races the CS tolerates start neutral with CS
    } else {
        // D-Bees: CS hostile, Lazlo/New Camelot friendly
        modify_standing(name, "coalition",   -300, "D-Bee default");
        modify_standing(name, "lazlo",        150, "D-Bee welcome");
        modify_standing(name, "new_camelot",  100, "D-Bee welcome");
    }

    if(race == "dog boy" || race == "cs psi-stalker") {
        modify_standing(name, "coalition",    200, "CS-aligned race");
        modify_standing(name, "lazlo",       -100, "CS-aligned race");
    }

    if(race == "secondary vampire" || race == "wild vampire") {
        modify_standing(name, "coalition",   -500, "undead KOS");
        modify_standing(name, "lazlo",       -200, "undead danger");
        modify_standing(name, "new_camelot", -300, "undead enemy");
        modify_standing(name, "bandits",      100, "predatory kin");
    }

    if(race == "godling") {
        modify_standing(name, "coalition",   -200, "supernatural threat");
        modify_standing(name, "lazlo",        300, "divine ally");
        modify_standing(name, "new_camelot",  200, "divine ally");
    }
}

string *query_faction_list() {
    return keys(FACTIONS);
}

string query_faction_display_name(string faction) {
    mixed *fdata;

    fdata = FACTIONS[faction];
    if(!fdata || !pointerp(fdata)) return faction;
    return fdata[0];
}

// ── Faction membership API ────────────────────────────────────────────────

/* create_faction: register a new faction if it does not already exist.
   Returns 1 on success, 0 if name already taken. */
int create_faction(string name, string display_name, int public_flag,
                   string description, string join_method) {
    if(!name || !strlen(name)) return 0;
    name = lower_case(name);
    if(faction_meta[name]) return 0;
    faction_meta[name] = ([
        "display_name": display_name,
        "public":       public_flag,
        "description":  description,
        "join_method":  join_method,
        "members":      ([])
    ]);
    save_data();
    return 1;
}

/* add_member: add player to faction at given rank. Rank is a string.
   Returns 1 on success, 0 on failure. */
int add_member(string faction, string player_name, string rank) {
    mapping meta;
    mapping members;

    if(!faction || !player_name) return 0;
    faction     = lower_case(faction);
    player_name = lower_case(player_name);
    meta = faction_meta[faction];
    if(!mapp(meta)) return 0;
    members = (mapping)meta["members"];
    if(!mapp(members)) members = ([]);
    members[player_name]      = rank;
    meta["members"]           = members;
    player_faction[player_name] = faction;
    save_data();
    return 1;
}

/* remove_member: remove player from their faction.
   Returns 1 on success, 0 if not found. */
int remove_member(string faction, string player_name) {
    mapping meta;
    mapping members;

    if(!faction || !player_name) return 0;
    faction     = lower_case(faction);
    player_name = lower_case(player_name);
    meta = faction_meta[faction];
    if(!mapp(meta)) return 0;
    members = (mapping)meta["members"];
    if(!mapp(members) || !members[player_name]) return 0;
    map_delete(members, player_name);
    map_delete(player_faction, player_name);
    save_data();
    return 1;
}

/* query_faction: returns faction name for player, or "" if none. */
string query_faction(string player_name) {
    string f;
    if(!player_name) return "";
    f = player_faction[lower_case(player_name)];
    return f ? f : "";
}

/* query_rank: returns rank string for player in their faction, or "". */
string query_rank(string player_name) {
    string faction;
    mapping meta, members;
    string rank;

    if(!player_name) return "";
    player_name = lower_case(player_name);
    faction = player_faction[player_name];
    if(!faction) return "";
    meta = faction_meta[faction];
    if(!mapp(meta)) return "";
    members = (mapping)meta["members"];
    if(!mapp(members)) return "";
    rank = (string)members[player_name];
    return rank ? rank : "";
}

/* query_faction_data: returns full faction metadata mapping or 0. */
mapping query_faction_data(string faction) {
    if(!faction) return 0;
    return faction_meta[lower_case(faction)];
}

/* query_members: returns mapping of player_name -> rank for the faction. */
mapping query_members(string faction) {
    mapping meta, members;
    if(!faction) return ([]);
    meta = faction_meta[lower_case(faction)];
    if(!mapp(meta)) return ([]);
    members = (mapping)meta["members"];
    return mapp(members) ? members + ([]) : ([]);
}

/* set_rank: update a player's rank within their faction.
   Returns 1 on success. */
int set_rank(string faction, string player_name, string rank) {
    mapping meta, members;

    if(!faction || !player_name) return 0;
    faction     = lower_case(faction);
    player_name = lower_case(player_name);
    meta = faction_meta[faction];
    if(!mapp(meta)) return 0;
    members = (mapping)meta["members"];
    if(!mapp(members) || !members[player_name]) return 0;
    members[player_name] = rank;
    save_data();
    return 1;
}

/* query_faction_is_public: returns 1 if faction is publicly visible. */
int query_faction_is_public(string faction) {
    mapping meta;
    if(!faction) return 0;
    meta = faction_meta[lower_case(faction)];
    if(!mapp(meta)) return 0;
    return (int)meta["public"];
}

/* query_faction_member_display: returns faction display name for score.
   Returns "" for secret factions or players not in any faction. */
string query_faction_member_display(string player_name) {
    string faction;
    mapping meta;

    if(!player_name) return "";
    faction = player_faction[lower_case(player_name)];
    if(!faction) return "";
    meta = faction_meta[faction];
    if(!mapp(meta)) return "";
    if(!(int)meta["public"]) return "";
    return (string)meta["display_name"];
}

string *query_all_faction_names() {
    return keys(faction_meta);
}
