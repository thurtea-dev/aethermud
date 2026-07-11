// /daemon/quest_d.c
// AetherMUD quest tracking daemon.
// Registers quest definitions and tracks per-player quest progress.
// Player state stored in env vars: active_quests (mapping), completed_quests (array),
// quest_cooldowns (mapping, repeatable quests).

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private mapping __quests;

void define_quests();
private mapping get_active(object player);
private mapping get_cooldowns(object player);
private void persist_player(object player);
private void grant_lives(object player, int count);
private int query_cooldown_remaining(object player, string id);

void create() {
    daemon::create();
    set_no_clean(1);
    __quests = ([]);
    define_quests();
}

// ── Quest registry ────────────────────────────────────────────────────────────

private void define_quests() {
    __quests["lost_supplies"] = ([
        "name"           : "Lost Supplies",
        "giver"          : "marta",
        "start_location" : "Speak to Marta at the Monument Square merchant stalls.",
        "desc"           : "A supply caravan from Lazlo went missing on the highway north of town. "
                           "Marta thinks bandits hit them. Find the supply crate and bring it back.",
        "objectives"     : ([ "supply_crate" : 1 ]),
        "reward_xp"      : 500,
        "reward_credits" : 300,
        "reward_infamy"  : 0
    ]);
    __quests["cs_deserter"] = ([
        "name"           : "Coalition Deserter",
        "giver"          : "griz",
        "start_location" : "Griz can be found in the Praxis market district.",
        "desc"           : "A CS soldier went AWOL and is hiding somewhere near Monument Square. "
                           "Griz has a contact who pays well for this kind of information. "
                           "Find the deserter, then report back.",
        "objectives"     : ([ "deserter_found" : 1 ]),
        "reward_xp"      : 750,
        "reward_credits" : 500,
        "reward_infamy"  : 25
    ]);
    __quests["rocky_rumor"] = ([
        "name"           : "Following the Rumor",
        "giver"          : "rocky",
        "start_location" : "Talk to Rocky at his bar in Praxis.",
        "desc"           : "Rocky heard a Cyber-Knight was seen heading north on the old highway. "
                           "Whatever they were looking for might be worth finding.",
        "objectives"     : ([ "old_highway_reached" : 1 ]),
        "reward_xp"      : 1000,
        "reward_credits" : 0,
        "reward_infamy"  : 0
    ]);
    __quests["dead_signal"] = ([
        "name"           : "Dead Signal",
        "giver"          : "rocky",
        "start_location" : "Rocky's bar. Tune radio to freq 77 then go to the ley line nexus.",
        "desc"           : "Rocky picked up something strange on frequency 77.  "
                           "Tune your radio to 77 and transmit.  Then head to the ley line nexus and transmit again.",
        "objectives"     : ([ "freq_77_tuned" : 1, "contacted_nexus" : 1 ]),
        "reward_xp"      : 1500,
        "reward_credits" : 500,
        "reward_infamy"  : 0
    ]);
    __quests["stolen_goods"] = ([
        "name"           : "Stolen Goods",
        "giver"          : "cs_quartermaster",
        "start_location" : "The CS quartermaster is at the Chi-Town CS armory.",
        "desc"           : "A weapons shipment was hit by bandits east of Illinois Road.  "
                           "Eliminate the bandits and report back.",
        "objectives"     : ([ "bandits_killed" : 3 ]),
        "reward_xp"      : 2000,
        "reward_credits" : 800,
        "reward_infamy"  : 0
    ]);
    __quests["black_forest_nexus"] = ([
        "name"           : "The Black Forest Nexus",
        "giver"          : "pendragon_barkeep",
        "start_location" : "Pendragon Tavern, north along the Europe Road.",
        "desc"           : "The barkeep at the Pendragon Tavern wants something from the nexus "
                           "in the Black Forest north of Europe Road.  "
                           "Retrieve a nexus stone and bring it back.",
        "objectives"     : ([ "nexus_stone_found" : 1 ]),
        "reward_xp"      : 2500,
        "reward_credits" : 1000,
        "reward_infamy"  : 0
    ]);

    /* Secret quests -- never appear in public listings or info until started. */
    __quests["aerihman_introduction"] = ([
        "name"           : "Shadow Among Shadows",
        "secret"         : 1,
        "faction"        : "aerihman",
        "giver"          : "",
        "start_location" : "",
        "desc"           : "A name spoken in darkness. A warning from one who knows too much. "
                           "Seek the mark of the eclipse when the stars are right.",
        "objectives"     : ([ "heard_hint" : 1 ]),
        "reward_xp"      : 0,
        "reward_credits" : 0,
        "reward_infamy"  : 0
    ]);
    __quests["survival_proof"] = ([
        "name"           : "Proof of Survival",
        "giver"          : "arbiter kane",
        "start_location" : "Arbiter Kane at the Praxis hospital.",
        "desc"           : "Level fifteen minimum. Bring a highway bandit patch to earn an extra life.",
        "objectives"     : ([ "bandit_patch" : 1 ]),
        "repeatable"     : 1,
        "cooldown_secs"  : 86400,
        "level_min"      : 15,
        "reward_lives"   : 1,
        "reward_xp"      : 0,
        "reward_credits" : 0,
        "reward_infamy"  : 0
    ]);
    __quests["demon_sliver_shard"] = ([
        "name"           : "Rift Shard Sample",
        "giver"          : "ley researcher",
        "start_location" : "Ley researcher at the Praxis dimensional tear below the nexus.",
        "desc"           : "A ley researcher needs a stable rift shard amulet from the demon-plane sliver shrine. "
                           "Cross the tear, reach the Bone Corridor, and retrieve the altar sample.",
        "objectives"     : ([ "rift_shard_amulet" : 1 ]),
        "reward_xp"      : 3000,
        "reward_credits" : 1200,
        "reward_infamy"  : 0
    ]);
    __quests["praxis_bounty_bandit"] = ([
        "name"           : "Highway Bandit Bounty",
        "giver"          : "bounty board",
        "start_location" : "Monument Square bounty board in Praxis.",
        "desc"           : "Praxis pays for proof that highway bandits are being cleared from the old routes. "
                           "Accept the posting, defeat a bandit, and turn in a scavenger raider patch.",
        "objectives"     : ([ "bandit_patch_turned" : 1 ]),
        "repeatable"     : 1,
        "cooldown_secs"  : 3600,
        "reward_xp"      : 800,
        "reward_credits" : 400,
        "reward_infamy"  : -15
    ]);
    __quests["chi_town_rift_report"] = ([
        "name"           : "Praxis Tear Report",
        "giver"          : "cs rift liaison",
        "start_location" : "CS rift liaison at the Chi-Town main gate.",
        "desc"           : "Coalition intelligence wants confirmation of a demon-plane sliver on the Praxis ley line. "
                           "Locate the dimensional tear below the nexus, cross into the sliver, then report to Chi-Town.",
        "objectives"     : ([ "tear_located" : 1, "demon_sliver_entered" : 1 ]),
        "reward_xp"      : 2200,
        "reward_credits" : 600,
        "reward_infamy"  : 0
    ]);
}

int is_objective_complete(object player, string quest_id, string obj_key) {
    mapping aq, prog, objs, qdef;
    int needed;

    if(!player || !quest_id || !obj_key) return 0;
    aq = get_active(player);
    if(!aq[quest_id]) return 0;
    prog = (mapping)aq[quest_id];
    if(!mapp(prog)) return 0;
    qdef = __quests[quest_id];
    if(!qdef) return 0;
    objs = (mapping)qdef["objectives"];
    if(!objs) return 0;
    needed = (int)objs[obj_key];
    return ((int)prog[obj_key] >= needed);
}

void register_quest(string id, mapping def) {
    if(!id || !stringp(id) || !def || !mapp(def)) return;
    __quests[id] = def;
}

mapping query_quest(string id) {
    if(!id) return 0;
    return __quests[id];
}

string *query_quest_ids() {
    return keys(__quests);
}

mapping query_all_quests() {
    return __quests + ([]);
}

// ── Player state helpers ──────────────────────────────────────────────────────

private mapping get_active(object player) {
    mixed aq;

    aq = (mixed)player->getenv("active_quests");
    if(!aq || !mapp(aq)) return ([]);
    return aq;
}

private string *get_completed(object player) {
    mixed cq;

    cq = (mixed)player->getenv("completed_quests");
    if(!cq || !pointerp(cq)) return ({});
    return (string *)cq;
}

private mapping get_cooldowns(object player) {
    mixed cd;

    if(!player) return ([]);
    cd = (mixed)player->getenv("quest_cooldowns");
    if(!cd || !mapp(cd)) return ([]);
    return cd;
}

private void persist_player(object player) {
    if(!player || !interactive(player)) return;
    if((string)player->query_name())
        catch(player->save_player((string)player->query_name()));
}

private void grant_lives(object player, int count) {
    int level;
    int lives_initted;
    int cur_lives;

    if(!player || count < 1) return;
    level = (int)player->query_level();
    lives_initted = (int)player->query_property("rifts_lives_initted");
    if(!lives_initted) {
        cur_lives = 5 + (level > 1 ? level - 1 : 0);
        player->set_property("rifts_lives_initted", 1);
    } else {
        cur_lives = (int)player->query_property("rifts_lives");
    }
    cur_lives += count;
    player->set_property("rifts_lives", cur_lives);
}

private int query_cooldown_remaining(object player, string id) {
    mapping qdef;
    mapping cds;
    int last;
    int legacy;
    int cooldown;
    int elapsed;

    if(!player || !id) return 0;
    qdef = __quests[id];
    if(!qdef || !(int)qdef["repeatable"]) return 0;
    cooldown = (int)qdef["cooldown_secs"];
    if(cooldown < 1) return 0;
    cds = get_cooldowns(player);
    last = (int)cds[id];
    if(id == "survival_proof") {
        legacy = to_int((string)player->getenv("life_quest_done"));
        if(legacy > last) last = legacy;
    }
    if(!last) return 0;
    elapsed = time() - last;
    if(elapsed >= cooldown) return 0;
    return cooldown - elapsed;
}

// ── Public API ────────────────────────────────────────────────────────────────

mapping query_active_quests(object player) {
    if(!player) return ([]);
    return get_active(player);
}

string *query_completed_quests(object player) {
    if(!player) return ({});
    return get_completed(player);
}

int is_quest_complete(object player, string id) {
    string *cq;

    if(!player || !id) return 0;
    cq = get_completed(player);
    return (member_array(id, cq) != -1);
}

int is_quest_active(object player, string id) {
    mapping aq;

    if(!player || !id) return 0;
    aq = get_active(player);
    return mapp(aq[id]);
}

int query_quest_cooldown(object player, string id) {
    return query_cooldown_remaining(player, id);
}

int query_quest_repeatable(string id) {
    mapping qdef;

    if(!id) return 0;
    qdef = __quests[id];
    if(!qdef) return 0;
    return (int)qdef["repeatable"];
}

int start_quest(object player, string id) {
    mapping aq, qdef, prog, objs;
    string *cq, *obj_keys;
    int i;
    int remaining;

    if(!player || !id) return 0;
    qdef = __quests[id];
    if(!qdef) return 0;
    aq = get_active(player);
    if(aq[id]) return 0;
    cq = get_completed(player);
    if(!(int)qdef["repeatable"] && member_array(id, cq) != -1) return 0;
    remaining = query_cooldown_remaining(player, id);
    if(remaining > 0) {
        tell_object(player,
            "\n[Quest] " + (string)qdef["name"] + " is on cooldown (" +
            ((remaining + 59) / 60) + " minutes remaining).\n");
        return 0;
    }
    objs = (mapping)qdef["objectives"];
    prog = ([]);
    if(objs) {
        obj_keys = keys(objs);
        i = sizeof(obj_keys);
        while(i--) prog[obj_keys[i]] = 0;
    }
    aq[id] = prog;
    player->setenv("active_quests", aq);
    tell_object(player,
        "\n[Quest] " + (string)qdef["name"] + " accepted.\n" +
        (string)qdef["desc"] + "\n");
    persist_player(player);
    return 1;
}

void update_objective(object player, string quest_id, string obj_key, int progress) {
    mapping aq, prog;

    if(!player || !quest_id || !obj_key) return;
    aq = get_active(player);
    if(!aq[quest_id]) return;
    prog = (mapping)aq[quest_id];
    if(!mapp(prog)) return;
    prog[obj_key] = progress;
    aq[quest_id] = prog;
    player->setenv("active_quests", aq);
    persist_player(player);
}

/* advance_quest -- mark a single step/objective as done (sets to required count). */
int advance_quest(object player, string quest_id, string step_id) {
    mapping aq;
    mapping prog;
    mapping qdef;
    mapping objs;
    int req;

    if(!player || !quest_id || !step_id) return 0;
    aq = get_active(player);
    if(!aq[quest_id]) return 0;
    prog = (mapping)aq[quest_id];
    if(!mapp(prog)) return 0;
    qdef = __quests[quest_id];
    if(!qdef) return 0;
    objs = (mapping)qdef["objectives"];
    if(!objs) return 0;
    req = (int)objs[step_id];
    if(req < 1) req = 1;
    prog[step_id] = req;
    aq[quest_id]  = prog;
    player->setenv("active_quests", aq);
    persist_player(player);
    return 1;
}

/* query_quest_status -- returns mapping with "status" and "steps_done" keys. */
mapping query_quest_status(object player, string quest_id) {
    mapping aq;
    mapping prog;
    string *cq;
    mapping result;

    result = ([ "status" : "not_started", "steps_done" : ({}) ]);
    if(!player || !quest_id) return result;
    cq = get_completed(player);
    if(member_array(quest_id, cq) != -1) {
        result["status"] = "complete";
        return result;
    }
    aq = get_active(player);
    if(!aq[quest_id]) return result;
    result["status"] = "in_progress";
    prog = (mapping)aq[quest_id];
    if(mapp(prog))
        result["steps_done"] = keys(prog);
    return result;
}

/* grant_rewards -- award XP/credits without marking quest complete. */
void grant_rewards(object player, string quest_id) {
    mapping qdef;
    int xp;
    int credits;
    int infamy;
    int lives;

    if(!player || !quest_id) return;
    if((int)is_quest_complete(player, quest_id)) return;
    qdef = __quests[quest_id];
    if(!qdef) return;
    xp      = (int)qdef["reward_xp"];
    credits = (int)qdef["reward_credits"];
    infamy  = (int)qdef["reward_infamy"];
    lives   = (int)qdef["reward_lives"];
    if(xp > 0)      player->add_exp(xp);
    if(credits > 0) player->add_money("credits", credits);
    if(infamy != 0) catch(WANTED_D->add_infamy(player->query_name(), infamy, "Quest: " + quest_id));
    if(lives > 0)   grant_lives(player, lives);
}

/* query_quest_secret -- returns 1 if quest is flagged secret. */
int query_quest_secret(string quest_id) {
    mapping qdef;
    if(!quest_id) return 0;
    qdef = __quests[quest_id];
    if(!qdef) return 0;
    return (int)qdef["secret"];
}

int complete_quest(object player, string id) {
    mapping aq, prog, objs, qdef, cds;
    string *cq, *obj_keys, reward_msg;
    int i, xp, credits, infamy, lives, all_done;
    int level_min;
    int repeatable;

    if(!player || !id) return 0;
    qdef = __quests[id];
    if(!qdef) return 0;
    repeatable = (int)qdef["repeatable"];
    if(!repeatable) {
        cq = get_completed(player);
        if(member_array(id, cq) != -1) return 0;
    }
    aq = get_active(player);
    if(!aq[id]) return 0;
    prog    = (mapping)aq[id];
    objs    = (mapping)qdef["objectives"];
    all_done = 1;
    if(objs) {
        obj_keys = keys(objs);
        i = sizeof(obj_keys);
        while(i--) {
            if(!mapp(prog) || (int)prog[obj_keys[i]] < (int)objs[obj_keys[i]]) {
                all_done = 0;
                break;
            }
        }
    }
    if(!all_done) return 0;
    level_min = (int)qdef["level_min"];
    if(level_min > 0 && (int)player->query_level() < level_min) return 0;
    map_delete(aq, id);
    player->setenv("active_quests", aq);
    if(repeatable) {
        cds = get_cooldowns(player);
        cds[id] = time();
        player->setenv("quest_cooldowns", cds);
        if(id == "survival_proof")
            player->setenv("life_quest_done", sprintf("%d", time()));
    } else {
        cq = get_completed(player);
        cq += ({ id });
        player->setenv("completed_quests", cq);
    }
    xp      = (int)qdef["reward_xp"];
    credits = (int)qdef["reward_credits"];
    infamy  = (int)qdef["reward_infamy"];
    lives   = (int)qdef["reward_lives"];
    if(xp > 0)      player->add_exp(xp);
    if(credits > 0) player->add_money("credits", credits);
    if(infamy != 0) catch(WANTED_D->add_infamy(player->query_name(), infamy, "Quest: "+id));
    if(lives > 0)   grant_lives(player, lives);
    reward_msg = "\n[Quest Complete] " + (string)qdef["name"] + "\n";
    if(xp > 0)      reward_msg += "  XP:      +" + xp + "\n";
    if(credits > 0) reward_msg += "  Credits: +" + credits + "\n";
    if(lives > 0)   reward_msg += "  Lives:   +" + lives +
                        " (now " + (int)player->query_property("rifts_lives") + ")\n";
    if(infamy > 0)  reward_msg += "  Infamy:  +" + infamy + " (cooperating with the Coalition)\n";
    tell_object(player, reward_msg);
    persist_player(player);
    return 1;
}
