/* /daemon/addiction_d.c
   Tracks intoxication and withdrawal state for drug and alcohol addiction.
   All state is stored as player env vars -- the daemon holds no per-player data.

   Env vars used (all strings):
     addiction_type   -- "alcohol", "drug", or "" (none)
     addiction_intox  -- "1" while currently intoxicated, "0" otherwise
     addiction_drunk  -- "1" while totally drunk (25% of alcohol intox), "0" otherwise
     addiction_week   -- "0" when intoxicated, "1"-"6" for withdrawal week
     addiction_disp   -- 1-100 disposition roll result set at first intoxication;
                         cleared on sobriety; "0" means not yet rolled

   Mechanical values (Palladium RIFTS source):

   TOTALLY DRUNK (alcohol, drunk flag set):
     -4 initiative, -2 strike/parry/dodge, -12% all skills

   ALCOHOL INTOXICATED (not totally drunk) -- per disposition roll:
     1-10: +2 damage (not modeled; no damage key in combat modifier mapping)
     11-20: +1 initiative, -1 parry, -1 dodge
     21-30: +2 initiative
     31-40: -2% all skills
     41-50: +1 initiative
     51-60: -5% all skills
     61-70: -2% skills when SOBER (not applied while intoxicated)
     71-80: -5% all skills
     81-90: -2 initiative
     91-00: +5% all skills (and -10% when sober -- sober penalty not applied here)

   ALCOHOL WITHDRAWAL:
     Week 1: -5% skills, -2 initiative/strike/parry/dodge
     Week 2: -2% skills, -2 initiative, -1 strike/parry/dodge
     Weeks 3-6: -2 initiative, no skill penalty

   DRUG INTOXICATED (not totally drunk) -- per disposition roll:
     1-10: +1 initiative
     11-20: -5 parry, -5 dodge (-5 roll with punch not modeled)
     21-30: +1 initiative
     31-40: -2 strike, -4 parry, -4 dodge
     41-50: -4 parry, -4 dodge (50% friend/foe confusion = behavioral only)
     51-60: -6% all skills
     61-70: -4% skills when NOT on drug (not applied while intoxicated)
     71-80: -8% all skills
     81-85: -4 parry, -4 dodge (43% hallucination -- same as 41-50 when triggered)
     86-91: -2 initiative, -2% all skills
     92-00: +1 strike, +1 parry, +1 dodge, +6% all skills

   DRUG WITHDRAWAL (cold turkey):
     Week 1: -35% skills, -8 all combat bonuses
     Week 2: -20% skills, -4 all combat bonuses
     Week 3: -10% skills, -2 strike/parry/dodge
     Week 4: no mechanical penalties (psychological only) */

#include <std.h>

inherit "/secure/std/daemon";

void create() {
    /* No per-daemon state -- everything lives in player env vars. */
}

/* ── State setters ──────────────────────────────────────────────────────────── */

void set_intoxicated(object player, string type) {
    int disp;

    if(!player || !type || !sizeof(type)) return;
    disp = random(100) + 1;
    player->setenv("addiction_type",  type);
    player->setenv("addiction_intox", "1");
    player->setenv("addiction_drunk", "0");
    player->setenv("addiction_week",  "0");
    player->setenv("addiction_disp",  sprintf("%d", disp));
}

void set_totally_drunk(object player, int flag) {
    if(!player) return;
    player->setenv("addiction_drunk", flag ? "1" : "0");
}

void set_withdrawal(object player, string type, int week) {
    if(!player || !type || !sizeof(type)) return;
    player->setenv("addiction_type",  type);
    player->setenv("addiction_intox", "0");
    player->setenv("addiction_drunk", "0");
    player->setenv("addiction_week",  sprintf("%d", week));
    player->setenv("addiction_disp",  "0");
}

void clear_addiction(object player) {
    if(!player) return;
    player->setenv("addiction_type",  "");
    player->setenv("addiction_intox", "0");
    player->setenv("addiction_drunk", "0");
    player->setenv("addiction_week",  "0");
    player->setenv("addiction_disp",  "0");
}

/* ── Modifier queries ───────────────────────────────────────────────────────── */

/* Returns mapping of combat modifiers (positive or negative) to add to bonus totals.
   Keys: "initiative", "strike", "parry", "dodge".
   Missing keys default to 0 when accessed from a mapping in LPC. */
mapping query_combat_modifiers(object player) {
    string type;
    string intox_s;
    string drunk_s;
    string week_s;
    string disp_s;
    int intox;
    int drunk;
    int week;
    int disp;

    if(!player) return ([]);
    type    = (string)player->getenv("addiction_type");
    if(!type || !sizeof(type)) return ([]);

    intox_s = (string)player->getenv("addiction_intox");
    drunk_s = (string)player->getenv("addiction_drunk");
    week_s  = (string)player->getenv("addiction_week");

    intox = (intox_s && sizeof(intox_s)) ? to_int(intox_s) : 0;
    drunk = (drunk_s && sizeof(drunk_s)) ? to_int(drunk_s) : 0;
    week  = (week_s  && sizeof(week_s))  ? to_int(week_s)  : 0;

    /* Totally drunk: -4 initiative, -2 strike/parry/dodge */
    if(intox && drunk && type == "alcohol") {
        return ([ "initiative": -4,
                  "strike":     -2,
                  "parry":      -2,
                  "dodge":      -2 ]);
    }

    /* Alcohol intoxicated (not totally drunk): disposition modifier */
    if(intox && !drunk && type == "alcohol") {
        disp_s = (string)player->getenv("addiction_disp");
        disp = (disp_s && sizeof(disp_s)) ? to_int(disp_s) : 0;
        if(disp >= 11 && disp <= 20)
            return ([ "initiative": 1, "parry": -1, "dodge": -1 ]);
        if(disp >= 21 && disp <= 30)
            return ([ "initiative": 2 ]);
        if(disp >= 41 && disp <= 50)
            return ([ "initiative": 1 ]);
        if(disp >= 81 && disp <= 90)
            return ([ "initiative": -2 ]);
        return ([]);
    }

    /* Drug intoxicated: disposition modifier */
    if(intox && type == "drug") {
        disp_s = (string)player->getenv("addiction_disp");
        disp = (disp_s && sizeof(disp_s)) ? to_int(disp_s) : 0;
        if(disp >= 1 && disp <= 10)
            return ([ "initiative": 1 ]);
        if(disp >= 11 && disp <= 20)
            return ([ "parry": -5, "dodge": -5 ]);
        if(disp >= 21 && disp <= 30)
            return ([ "initiative": 1 ]);
        if(disp >= 31 && disp <= 40)
            return ([ "strike": -2, "parry": -4, "dodge": -4 ]);
        if(disp >= 41 && disp <= 50)
            return ([ "parry": -4, "dodge": -4 ]);
        if(disp >= 81 && disp <= 85)
            return ([ "parry": -4, "dodge": -4 ]);
        if(disp >= 86 && disp <= 91)
            return ([ "initiative": -2 ]);
        if(disp >= 92)
            return ([ "strike": 1, "parry": 1, "dodge": 1 ]);
        return ([]);
    }

    /* Alcohol withdrawal */
    if(!intox && type == "alcohol") {
        if(week == 1)
            return ([ "initiative": -2,
                      "strike":     -2,
                      "parry":      -2,
                      "dodge":      -2 ]);
        if(week == 2)
            return ([ "initiative": -2,
                      "strike":     -1,
                      "parry":      -1,
                      "dodge":      -1 ]);
        if(week >= 3 && week <= 6)
            return ([ "initiative": -2 ]);
    }

    /* Drug withdrawal */
    if(!intox && type == "drug") {
        if(week == 1)
            return ([ "initiative": -8,
                      "strike":     -8,
                      "parry":      -8,
                      "dodge":      -8 ]);
        if(week == 2)
            return ([ "initiative": -4,
                      "strike":     -4,
                      "parry":      -4,
                      "dodge":      -4 ]);
        if(week == 3)
            return ([ "strike":     -2,
                      "parry":      -2,
                      "dodge":      -2 ]);
    }

    return ([]);
}

/* Returns skill % modifier (positive = bonus, negative = penalty) to apply before roll.
   Caller adds this to the effective skill value before rolling d100. */
int query_skill_modifier(object player) {
    string type;
    string intox_s;
    string drunk_s;
    string week_s;
    string disp_s;
    int intox;
    int drunk;
    int week;
    int disp;

    if(!player) return 0;
    type = (string)player->getenv("addiction_type");
    if(!type || !sizeof(type)) return 0;

    intox_s = (string)player->getenv("addiction_intox");
    drunk_s = (string)player->getenv("addiction_drunk");
    week_s  = (string)player->getenv("addiction_week");

    intox = (intox_s && sizeof(intox_s)) ? to_int(intox_s) : 0;
    drunk = (drunk_s && sizeof(drunk_s)) ? to_int(drunk_s) : 0;
    week  = (week_s  && sizeof(week_s))  ? to_int(week_s)  : 0;

    /* Totally drunk: -12% all skills */
    if(intox && drunk && type == "alcohol") return -12;

    /* Alcohol intoxicated (not totally drunk): disposition modifier */
    if(intox && !drunk && type == "alcohol") {
        disp_s = (string)player->getenv("addiction_disp");
        disp = (disp_s && sizeof(disp_s)) ? to_int(disp_s) : 0;
        if(disp >= 31 && disp <= 40) return -2;
        if((disp >= 51 && disp <= 60) || (disp >= 71 && disp <= 80)) return -5;
        if(disp >= 91) return 5;
        return 0;
    }

    /* Drug intoxicated: disposition modifier */
    if(intox && type == "drug") {
        disp_s = (string)player->getenv("addiction_disp");
        disp = (disp_s && sizeof(disp_s)) ? to_int(disp_s) : 0;
        if(disp >= 51 && disp <= 60) return -6;
        if(disp >= 71 && disp <= 80) return -8;
        if(disp >= 86 && disp <= 91) return -2;
        if(disp >= 92) return 6;
        return 0;
    }

    /* Alcohol withdrawal */
    if(!intox && type == "alcohol") {
        if(week == 1) return -5;
        if(week == 2) return -2;
        return 0;
    }

    /* Drug withdrawal */
    if(!intox && type == "drug") {
        if(week == 1) return -35;
        if(week == 2) return -20;
        if(week == 3) return -10;
        return 0;
    }

    return 0;
}
