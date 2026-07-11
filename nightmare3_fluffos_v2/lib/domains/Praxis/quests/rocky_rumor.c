// /d/Praxis/quests/rocky_rumor.c
// Quest definition: "Following the Rumor"
// Giver: Rocky (Rocky's Bar). Reward: 1000 XP.
// Auto-completes when player reaches old_highway.c with the quest active.
// Registered by quest_d.c at startup. This file is reference/documentation.

mapping query_definition() {
    return ([
        "id" : "rocky_rumor",
        "name" : "Following the Rumor",
        "giver" : "rocky",
        "desc" : "Rocky heard a Cyber-Knight was seen heading north on the old highway. "
                           "Head north from the checkpoint ruins and see what you can find.",
        "objectives" : ([ "old_highway_reached" : 1 ]),
        "reward_xp" : 1000,
        "reward_credits" : 0,
        "reward_infamy" : 0
    ]);
}
