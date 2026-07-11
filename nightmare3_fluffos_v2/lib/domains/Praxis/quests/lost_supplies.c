// /d/Praxis/quests/lost_supplies.c
// Quest definition: "Lost Supplies"
// Giver: Marta (general store). Reward: 500 XP, 300 credits.
// Registered by quest_d.c at startup. This file is reference/documentation.

mapping query_definition() {
    return ([
        "id" : "lost_supplies",
        "name" : "Lost Supplies",
        "giver" : "marta",
        "desc" : "A supply caravan from Lazlo went missing on the highway north. "
                           "Find the supply crate and bring it back to Marta.",
        "objectives" : ([ "supply_crate" : 1 ]),
        "reward_xp" : 500,
        "reward_credits" : 300,
        "reward_infamy" : 0
    ]);
}
