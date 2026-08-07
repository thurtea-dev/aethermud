// /d/Praxis/quests/cs_deserter.c
// Quest definition: "Coalition Deserter"
// Giver: Griz (weapons shop). Reward: 750 XP, 500 credits, +25 infamy.
// Registered by quest_d.c at startup. This file is reference/documentation.

mapping query_definition() {
    return ([
        "id" : "cs_deserter",
        "name" : "Coalition Deserter",
        "giver" : "griz",
        "desc" : "A CS soldier went AWOL and is hiding nearby. "
                           "Griz has a contact who pays well for this information. "
                           "Find the deserter, then return to Griz and say 'found him'.",
        "objectives" : ([ "deserter_found" : 1 ]),
        "reward_xp" : 750,
        "reward_credits" : 500,
        "reward_infamy" : 25
    ]);
}
