// /daemon/wanted_d.c
// RiftsMUD infamy and wanted status tracker.
//
// Infamy score: 0 = clean, 100+ = wanted, 500+ = kill on sight.
// Negative infamy = hero status (gained by killing evil beings).
// CS enemy: non-human race OR infamy >= 100.

#include <std.h>
#include <daemons.h>
#include <dirs.h>
#include <security.h>

inherit DAEMON;

#define SAVE_FILE (DIR_DAEMONS_SAVE + "/wanted_d")

// player_name -> infamy score (int, may be negative)
mapping infamy_scores;

// player_name -> array of ({ time, amount, reason }) log entries (last 20)
mapping infamy_log;

void save_data() {
    unguarded((: save_object, SAVE_FILE :));
}

void create() {
    daemon::create();
    set_no_clean(1);
    infamy_scores = ([]);
    infamy_log    = ([]);
    if(file_size(SAVE_FILE + ".o") > 0)
        unguarded((: restore_object, SAVE_FILE :));
}

// ── Infamy modification ──────────────────────────────────────────────────────

void add_infamy(string player_name, int amount, string reason) {
    mixed *log_entry;
    mixed *log_arr;
    int current;

    if(!player_name || !strlen(player_name)) return;
    player_name = lower_case(player_name);

    current = infamy_scores[player_name];
    if(!intp(current)) current = 0;
    current += amount;
    infamy_scores[player_name] = current;

    log_entry = ({ time(), amount, reason });
    log_arr   = infamy_log[player_name];
    if(!log_arr || !pointerp(log_arr)) log_arr = ({});
    log_arr += ({ log_entry });
    if(sizeof(log_arr) > 20) log_arr = log_arr[sizeof(log_arr)-20..];
    infamy_log[player_name] = log_arr;

    log_file("infamy",
        player_name + " [" + (amount >= 0 ? "+" : "") + amount + "=" + current +
        "] " + reason + " " + ctime(time()) + "\n");
    save_data();
}

void clear_infamy(string player_name) {
    if(!player_name || !strlen(player_name)) return;
    player_name = lower_case(player_name);
    map_delete(infamy_scores, player_name);
    map_delete(infamy_log,    player_name);
    log_file("infamy", player_name + " [cleared] " + ctime(time()) + "\n");
    save_data();
}

// ── Queries ──────────────────────────────────────────────────────────────────

int query_infamy(string player_name) {
    int val;

    if(!player_name || !strlen(player_name)) return 0;
    val = infamy_scores[lower_case(player_name)];
    if(!intp(val)) return 0;
    return val;
}

string query_wanted_status(string player_name) {
    int score;

    score = query_infamy(player_name);
    if(score <= -100) return "Hero of the People";
    if(score < 0)     return "clean (minor hero)";
    if(score < 100)   return "clean";
    if(score < 500)   return "wanted";
    return "kill on sight";
}

// Returns 1 if Coalition State forces are hostile to this player.
// Uses infamy, FACTION_D standings and membership, then race defaults.
int is_cs_enemy(object player) {
    string name;
    string race;
    string faction;

    if(!player || !objectp(player)) return 0;
    name = lower_case((string)player->query_name());

    if(query_infamy(name) >= 100) return 1;
    if((int)FACTION_D->is_cs_ally(player)) return 0;
    if((int)FACTION_D->is_hostile(name, "coalition")) return 1;

    faction = (string)FACTION_D->query_faction(name);
    if(faction == "splugorth" || faction == "tolkeen") return 1;

    race = (string)player->query_race();
    if(!race || !strlen(race)) return 0;
    race = lower_case(race);
    if(race == "human")          return 0;
    if(race == "dog boy")        return 0;
    if(race == "cs psi-stalker") return 0;
    if(race == "godling")        return 0;
    return 1;
}

// Returns the infamy log for a player (array of ({time, amount, reason}))
mixed *query_infamy_log(string player_name) {
    mixed *log_arr;

    if(!player_name || !strlen(player_name)) return ({});
    log_arr = infamy_log[lower_case(player_name)];
    if(!log_arr || !pointerp(log_arr)) return ({});
    return log_arr + ({});
}
