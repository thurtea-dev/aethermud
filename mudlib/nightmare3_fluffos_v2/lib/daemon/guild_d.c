/* /daemon/guild_d.c
   AetherMUD guild system daemon.
   Stores guild membership, tags, and bank accounts.
   Guilds persist via save_object. */

#include <std.h>
#include <daemons.h>
#include <dirs.h>

inherit DAEMON;

#define SAVE_FILE (DIR_DAEMONS_SAVE + "/guild_d")

/* guild_name -> ([
     "members": ([ player_name -> rank ]),
     "tag":     string (2-4 char tag),
     "bank":    int credits,
     "founder": string player_name
   ]) */
mapping guilds;

/* player_name -> guild_name */
mapping player_guild;

void save_data() { save_object(SAVE_FILE); }

void create() {
    daemon::create();
    set_no_clean(1);
    guilds       = ([]);
    player_guild = ([]);
    if(file_size(SAVE_FILE + ".o") > 0)
        restore_object(SAVE_FILE);
}

/* ── Public API ──────────────────────────────────────────────────────── */

/* Returns 1 on success, 0 if name or tag taken or founder already in guild. */
int create_guild(string name, string tag, object founder) {
    string pname;

    if(!name || !tag || !founder) return 0;
    name  = lower_case(name);
    tag   = upper_case(tag);
    pname = lower_case((string)founder->query_name());

    if(guilds[name]) return 0;
    if(player_guild[pname]) return 0;

    guilds[name] = ([
        "members": ([ pname: 1 ]),
        "tag":     tag,
        "bank":    0,
        "founder": pname
    ]);
    player_guild[pname] = name;
    save_data();
    return 1;
}

/* rank: 1=leader, 2=officer, 3=member */
int add_member(string guild_name, object player, int rank) {
    string pname;
    mapping guild;

    if(!guild_name || !player) return 0;
    guild_name = lower_case(guild_name);
    guild      = guilds[guild_name];
    if(!guild) return 0;
    pname = lower_case((string)player->query_name());
    if(player_guild[pname]) return 0;

    guild["members"][pname] = rank;
    player_guild[pname]     = guild_name;
    save_data();
    return 1;
}

int remove_member(string guild_name, object player) {
    string pname;
    mapping guild;
    mapping members;

    if(!guild_name || !player) return 0;
    guild_name = lower_case(guild_name);
    guild      = guilds[guild_name];
    if(!guild) return 0;
    pname   = lower_case((string)player->query_name());
    members = (mapping)guild["members"];
    if(!members || !members[pname]) return 0;

    map_delete(members, pname);
    map_delete(player_guild, pname);

    if(!sizeof(members)) {
        /* last member left: dissolve guild */
        map_delete(guilds, guild_name);
    }
    save_data();
    return 1;
}

/* Returns guild name for player, or "" if not in a guild. */
string query_guild(object player) {
    string pname, gname;

    if(!player) return "";
    pname = lower_case((string)player->query_name());
    gname = player_guild[pname];
    return gname ? gname : "";
}

/* Returns guild name for player by name string, or "" if not in a guild. */
string query_guild_by_name(string pname) {
    string gname;

    if(!pname) return "";
    pname = lower_case(pname);
    gname = player_guild[pname];
    return gname ? gname : "";
}

/* Returns full guild mapping or 0 if not found. */
mapping query_guild_data(string guild_name) {
    if(!guild_name) return 0;
    return guilds[lower_case(guild_name)];
}

/* Returns the guild tag for a guild name. */
string query_guild_tag(string guild_name) {
    mapping guild;

    if(!guild_name) return "";
    guild = guilds[lower_case(guild_name)];
    if(!guild) return "";
    return (string)guild["tag"];
}

/* Returns the player's rank in their guild (0 if not in one). */
int query_rank(object player) {
    string pname, gname;
    mapping guild, members;

    if(!player) return 0;
    pname = lower_case((string)player->query_name());
    gname = player_guild[pname];
    if(!gname) return 0;
    guild = guilds[gname];
    if(!guild) return 0;
    members = (mapping)guild["members"];
    if(!members) return 0;
    return (int)members[pname];
}

int query_guild_bank(string guild_name) {
    mapping guild;

    if(!guild_name) return 0;
    guild = guilds[lower_case(guild_name)];
    if(!guild) return 0;
    return (int)guild["bank"];
}

/* Returns 1 on success. */
int deposit_guild(string guild_name, int amount) {
    mapping guild;

    if(!guild_name || amount <= 0) return 0;
    guild = guilds[lower_case(guild_name)];
    if(!guild) return 0;
    guild["bank"] = (int)guild["bank"] + amount;
    save_data();
    return 1;
}

/* Returns 1 on success, 0 if insufficient funds or insufficient rank. */
int withdraw_guild(string guild_name, object player, int amount) {
    mapping guild;
    int rank, balance;

    if(!guild_name || !player || amount <= 0) return 0;
    rank = query_rank(player);
    if(rank < 1 || rank > 2) return 0; /* only rank 1-2 can withdraw */
    guild = guilds[lower_case(guild_name)];
    if(!guild) return 0;
    balance = (int)guild["bank"];
    if(balance < amount) return 0;
    guild["bank"] = balance - amount;
    save_data();
    return 1;
}

/* Returns a formatted member list string. */
string query_member_list(string guild_name) {
    mapping guild, members;
    string *names, buf, rank_str;
    int i, rank;

    if(!guild_name) return "";
    guild = guilds[lower_case(guild_name)];
    if(!guild) return "";
    members = (mapping)guild["members"];
    if(!members) return "(empty)";
    names = keys(members);
    buf   = "";
    for(i = 0; i < sizeof(names); i++) {
        rank = (int)members[names[i]];
        if(rank == 1)      rank_str = "[Leader]";
        else if(rank == 2) rank_str = "[Officer]";
        else               rank_str = "[Member]";
        buf += "  " + capitalize(names[i]) + " " + rank_str + "\n";
    }
    return buf;
}
