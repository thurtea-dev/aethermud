/* /daemon/rifts.c */
/* AetherMUD  -  Palladium Rifts race and attribute daemon */
/* Handles attribute rolling, race data, and Rifts-specific stat storage. */

#include <daemons.h>

/* Prototypes for safe string lookups defined later in this file */
string query_occ_string(object pl);
string query_race_string(object who);

/* All playable Rifts races */
private string *rifts_races_list;

/* Admin-only races (not shown to players, applied on promotion) */
private string *admin_races_list;

/* Races eligible to choose an OCC (mirrors occ.c occ_eligible_races) */
private string *occ_races_list;

/* Idle position string shown in room listings, keyed by race. NPCs only;   */
/* see query_race_idle_string(). Races not listed here fall back to the    */
/* default "is standing around." set by the caller.                       */
private mapping race_idle_strings;

void create() {
    admin_races_list = ({ "godling" });

    rifts_races_list = ({
        "algor frost giant", "atlantean", "basilisk", "bearman", "bogie",
        "brodkil", "brownie", "burster", "changeling", "common faerie",
        "common pixie", "conservator", "coyle", "dog boy",
        "dragon hatchling", "dwarf", "elf", "equinoid",
        "fire dragon", "frost pixie", "gargoyle", "goblin",
        "great horned dragon", "green wood faerie", "gurgoyle",
        "hawrk-duhk", "hawrk-ka",
        "human", "ice dragon", "jotan", "kankoran", "kittani", "lyn-srial",
        "mind melter",
        "night-elves faerie", "nimro fire giant", "ogre", "orc",
        "pogtal", "cs psi-stalker", "rahu-man", "ratling",
        "secondary vampire", "silver bells faerie", "simvan",
        "thunder lizard dragon", "titan", "tree sprite", "troll",
        "water sprite", "werebear", "weretiger", "werewolf",
        "wild psi-stalker", "wild vampire", "wolfen",
        "mutant rat", "mutant dog", "mutant bear",
        "mutant cat", "mutant hawk", "mutant wolf"
    });

    race_idle_strings = ([
        "godling" : "stands here, watching.",

        "wolf" : "prowls nearby.",
        "animal" : "prowls nearby.",
        "dog" : "prowls nearby.",
        "reptile" : "lurks nearby.",

        "gargoyle" : "looms overhead.",
        "brodkil" : "looms overhead.",

        "deevil" : "lurks in the shadows.",
        "demon" : "lurks in the shadows.",
        "hound demon" : "lurks in the shadows.",

        "fire dragon" : "coils nearby, watching.",
        "ice dragon" : "coils nearby, watching.",
        "great horned dragon" : "coils nearby, watching.",
        "thunder lizard dragon" : "coils nearby, watching.",
        "dragon hatchling" : "coils nearby, watching.",

        "brownie" : "hovers nearby.",
        "common faerie" : "hovers nearby.",
        "common pixie" : "hovers nearby.",
        "frost pixie" : "hovers nearby.",
        "green wood faerie" : "hovers nearby.",
        "night-elves faerie" : "hovers nearby.",
        "silver bells faerie" : "hovers nearby.",
        "tree sprite" : "hovers nearby.",
        "water sprite" : "hovers nearby.",

        "monster" : "drifts nearby.",

        "golem" : "stands at ready.",

        "vampire" : "lingers nearby.",
        "secondary vampire" : "lingers nearby.",
        "wight" : "lingers nearby.",

        "splugorth minion" : "observes from a distance.",
        "kydian" : "observes from a distance.",
        "xiticix" : "observes from a distance."
    ]);

    occ_races_list = ({
        "algor frost giant", "atlantean", "bearman", "changeling", "coyle",
        "dwarf", "elf", "goblin", "human", "jotan", "kankoran",
        "kittani", "nimro fire giant", "ogre", "orc", "rahu-man", "ratling",
        "titan", "troll"
    });
}

int is_admin_race(string race) {
    if(!admin_races_list || !race || race == "") return 0;
    return member_array(lower_case(race), admin_races_list) != -1;
}

int is_rifts_race(string race) {
    if(!race || race == "") return 0;
    if(admin_races_list && member_array(lower_case(race), admin_races_list) != -1)
        return 1;
    if(!rifts_races_list) return 0;
    return member_array(lower_case(race), rifts_races_list) != -1;
}

/* Returns only player-accessible races (excludes admin races). */
string *query_rifts_races() {
    if(!rifts_races_list) return ({});
    return rifts_races_list + ({});
}

/* Returns all races including admin-only ones. */
string *query_all_rifts_races() {
    string *all;
    if(!rifts_races_list) all = ({});
    else all = rifts_races_list + ({});
    if(admin_races_list) all += admin_races_list;
    return all;
}

int query_occ_eligible(string race) {
    if(!occ_races_list || !race || race == "") return 0;
    return member_array(lower_case(race), occ_races_list) != -1;
}

/* Returns the room-listing idle string for an NPC race, or 0 if the race */
/* has no special entry (caller should fall back to the default string). */
string query_race_idle_string(string race) {
    if(!race_idle_strings || !race || race == "") return 0;
    return race_idle_strings[lower_case(race)];
}

/* Roll num dice with sides faces each. */
private int roll(int num, int sides) {
    int total, i;
    total = 0;
    for(i = 0; i < num; i++) total += random(sides) + 1;
    return total;
}

/* do_mutant_rolls  -  roll attributes for the 6 mutant animal RCCs. */
/* Called by do_rifts_rolls() default branch to keep that function small. */
private mapping do_mutant_rolls(string race) {
    int iq, me, ma, ps, pp, pe, pb, spd;
    switch(lower_case(race)) {
    case "mutant rat":
        iq=roll(3,6)+2; me=roll(3,6);   ma=roll(3,6);   ps=roll(3,6);
        pp=roll(3,6)+2; pe=roll(3,6);   pb=roll(1,6)+2; spd=roll(3,6)+4;
        break;
    case "mutant dog":
        iq=roll(3,6);   me=roll(3,6);   ma=roll(3,6);   ps=roll(3,6)+3;
        pp=roll(3,6)+2; pe=roll(3,6)+2; pb=roll(3,6);   spd=roll(3,6);
        break;
    case "mutant bear":
        iq=roll(3,6);   me=roll(3,6);   ma=roll(3,6);   ps=roll(3,6)+8;
        pp=roll(2,6)+2; pe=roll(3,6)+6; pb=roll(1,6)+1; spd=roll(2,6)+2;
        break;
    case "mutant cat":
        iq=roll(3,6);   me=roll(3,6);   ma=roll(3,6);   ps=roll(3,6);
        pp=roll(3,6)+4; pe=roll(3,6);   pb=roll(3,6)+2; spd=roll(3,6)+6;
        break;
    case "mutant hawk":
        iq=roll(3,6);   me=roll(3,6);   ma=roll(3,6);   ps=roll(2,6)+2;
        pp=roll(3,6)+4; pe=roll(3,6);   pb=roll(3,6)+2; spd=roll(3,6)+4;
        break;
    case "mutant wolf":
        iq=roll(3,6);   me=roll(3,6);   ma=roll(3,6);   ps=roll(3,6)+4;
        pp=roll(3,6)+3; pe=roll(3,6)+3; pb=roll(3,6);   spd=roll(3,6)+8;
        break;
    default:
        return 0;
    }
    return ([
        "IQ":iq, "ME":me, "MA":ma, "PS":ps,
        "PP":pp, "PE":pe, "PB":pb, "Spd":spd,
        "strength":ps, "constitution":pe, "intelligence":iq,
        "wisdom":me,   "dexterity":pp,  "charisma":pb
    ]);
}

/* do_rifts_rolls  -  roll Palladium 8 attributes for a race. */
/* */
/* Returns a mapping with: */
/* Rifts keys: IQ ME MA PS PP PE PB Spd */
/* D&D shadow keys: strength constitution intelligence wisdom dexterity charisma */
/* */
/* Shadow keys keep Nightmare's HP/encumbrance/SP formulas working without */
/* modifying living.c or body.c. constitution → max_hp, strength → encumbrance, etc. */
mapping do_rifts_rolls(string race) {
    int iq, me, ma, ps, pp, pe, pb, spd;

    if(!race || race == "") return ([]);
    switch(lower_case(race)) {

    /* ── Admin-only: Godling ──────────────────────────────────────────────── */

    case "godling":
        /* Fixed max-stat divine being.  Never rolled randomly. */
        iq = 30; me = 30; ma = 30; ps = 30;
        pp = 30; pe = 30; pb = 30; spd = 30;
        break;

    /* ── Humans and Near-Humans ───────────────────────────────────────────── */

    case "human":
        iq  = roll(3,6); me  = roll(3,6); ma  = roll(3,6); ps  = roll(3,6);
        pp  = roll(3,6); pe  = roll(3,6); pb  = roll(3,6); spd = roll(3,6);
        break;

    case "burster": /* psionic human  --  stats as human, powers added separately */
        iq  = roll(3,6); me  = roll(3,6)+2; ma  = roll(3,6); ps  = roll(3,6);
        pp  = roll(3,6); pe  = roll(3,6);   pb  = roll(3,6); spd = roll(3,6);
        break;

    case "mind melter": /* master psionic human */ /* approx */
        iq  = roll(3,6)+2; me = roll(3,6)+4; ma = roll(3,6)+2; ps = roll(3,6);
        pp  = roll(3,6);   pe = roll(3,6);   pb = roll(3,6);   spd= roll(3,6);
        break;

    /* ── Coalition Races ──────────────────────────────────────────────────── */

    case "dog boy":
        iq  = roll(3,6)+1;  /* Animal cunning */
        me  = roll(3,6);
        ma  = roll(3,6);
        ps  = roll(3,6)+6;  /* Supernatural Physical Strength */
        pp  = roll(3,6)+1;
        pe  = roll(3,6)+6;  /* Supernatural Physical Endurance */
        pb  = roll(3,6);
        spd = roll(4,6)+4;  /* Enhanced speed */
        break;

    case "cs psi-stalker": /* Coalition psionic bloodhound */ /* approx */
        iq  = roll(3,6);   me = roll(3,6)+4; ma = roll(3,6);   ps = roll(3,6)+4;
        pp  = roll(3,6)+4; pe = roll(3,6)+4; pb = roll(2,6)+2; spd= roll(3,6)+4;
        break;

    /* ── Dwarves, Elves, and Related ─────────────────────────────────────── */

    case "dwarf": /* approx */
        iq  = roll(3,6)+2; me = roll(3,6);   ma = roll(3,6);   ps = roll(4,6);
        pp  = roll(3,6);   pe = roll(4,6);   pb = roll(2,6);   spd= roll(2,6)+4;
        break;

    case "elf": /* approx */
        iq  = roll(3,6)+2; me = roll(3,6)+2; ma = roll(3,6)+2; ps = roll(3,6);
        pp  = roll(3,6)+3; pe = roll(3,6);   pb = roll(3,6)+6; spd= roll(3,6)+4;
        break;

    /* ── Giants ───────────────────────────────────────────────────────────── */

    case "algor frost giant": /* approx */
        iq  = roll(3,6);   me = roll(3,6)+1; ma = roll(2,6);   ps = roll(4,6)+10;
        pp  = roll(3,6);   pe = roll(4,6)+6; pb = roll(2,6);   spd= roll(3,6)+4;
        break;

    case "jotan": /* Norse giant */ /* approx */
        iq  = roll(2,6)+4; me = roll(2,6)+4; ma = roll(2,6)+4; ps = roll(5,6)+8;
        pp  = roll(2,6)+4; pe = roll(4,6)+8; pb = roll(2,6);   spd= roll(3,6)+6;
        break;

    case "nimro fire giant": /* approx */
        iq  = roll(3,6);   me = roll(3,6);   ma = roll(2,6)+2; ps = roll(5,6)+10;
        pp  = roll(3,6);   pe = roll(4,6)+8; pb = roll(2,6);   spd= roll(3,6)+6;
        break;

    case "titan": /* approx */
        iq  = roll(3,6)+2; me = roll(3,6)+2; ma = roll(3,6)+2; ps = roll(5,6)+10;
        pp  = roll(3,6);   pe = roll(4,6)+8; pb = roll(2,6)+2; spd= roll(3,6)+4;
        break;

    /* ── Goblinoids ───────────────────────────────────────────────────────── */

    case "goblin": /* approx */
        iq  = roll(2,6)+4; me = roll(2,6);   ma = roll(3,6);   ps = roll(3,6)+3;
        pp  = roll(3,6);   pe = roll(3,6);   pb = roll(1,6)+2; spd= roll(3,6)+4;
        break;

    case "ogre": /* approx */
        iq  = roll(2,6)+2; me = roll(2,6);   ma = roll(2,6);   ps = roll(4,6)+8;
        pp  = roll(2,6);   pe = roll(4,6)+4; pb = roll(1,6);   spd= roll(3,6);
        break;

    case "orc": /* approx */
        iq  = roll(2,6)+2; me = roll(2,6);   ma = roll(2,6)+2; ps = roll(3,6)+6;
        pp  = roll(3,6);   pe = roll(3,6)+6; pb = roll(1,6);   spd= roll(3,6);
        break;

    case "troll": /* approx */
        iq  = roll(2,6)+1; me = roll(2,6);   ma = roll(2,6);   ps = roll(4,6)+10;
        pp  = roll(2,6);   pe = roll(4,6)+6; pb = roll(1,6);   spd= roll(3,6)+2;
        break;

    /* ── Canine Races ─────────────────────────────────────────────────────── */

    case "coyle": /* intelligent fox-like humanoid */ /* approx */
        iq  = roll(3,6);   me = roll(2,6)+6; ma = roll(3,6);   ps = roll(3,6)+6;
        pp  = roll(3,6)+3; pe = roll(3,6)+6; pb = roll(2,6);   spd= roll(4,6)+6;
        break;

    case "kankoran": /* fox-like humanoid from WB3 */ /* approx */
        iq  = roll(3,6)+2; me = roll(3,6)+2; ma = roll(3,6)+4; ps = roll(3,6)+4;
        pp  = roll(3,6)+4; pe = roll(3,6)+4; pb = roll(3,6);   spd= roll(4,6)+4;
        break;

    case "bearman": /* bear-like humanoid, WB6 */ /* approx */
        iq  = roll(2,6)+4; me = roll(3,6);   ma = roll(3,6);   ps = roll(3,6)+10;
        pp  = roll(3,6);   pe = roll(3,6)+10; pb= roll(2,6);   spd= roll(2,6)+4;
        break;

    /* ── Ratling ──────────────────────────────────────────────────────────── */

    case "ratling": /* rodent humanoid */ /* approx */
        iq  = roll(3,6)+2; me = roll(2,6)+2; ma = roll(3,6);   ps = roll(3,6)+2;
        pp  = roll(3,6)+4; pe = roll(3,6);   pb = roll(1,6)+2; spd= roll(3,6)+6;
        break;

    /* ── Supernatural Humanoids ───────────────────────────────────────────── */

    case "rahu-man": /* Indian demigod-like being */ /* approx */
        iq  = roll(3,6)+2; me = roll(3,6)+4; ma = roll(4,6)+2; ps = roll(5,6)+6;
        pp  = roll(3,6)+4; pe = roll(4,6)+4; pb = roll(3,6)+2; spd= roll(4,6)+4;
        break;

    case "changeling": /* humanoid shapeshifter */ /* approx */
        iq  = roll(3,6);   me = roll(3,6);   ma = roll(3,6)+2; ps = roll(3,6);
        pp  = roll(3,6);   pe = roll(3,6);   pb = roll(4,6)+2; spd= roll(3,6);
        break;

    case "equinoid": /* horse-like humanoid */ /* approx */
        iq  = roll(2,6)+4; me = roll(3,6);   ma = roll(3,6);   ps = roll(4,6)+4;
        pp  = roll(3,6);   pe = roll(4,6)+4; pb = roll(3,6);   spd= roll(5,6)+10;
        break;

    case "pogtal": /* approx  --  ape-like crested humanoid */
        iq  = roll(2,6)+2; me = roll(2,6);   ma = roll(2,6);   ps = roll(4,6)+4;
        pp  = roll(3,6);   pe = roll(4,6);   pb = roll(1,6);   spd= roll(3,6);
        break;

    case "conservator": /* eco-mage humanoid */ /* approx */
        iq  = roll(3,6)+2; me = roll(3,6)+2; ma = roll(3,6)+2; ps = roll(3,6);
        pp  = roll(3,6);   pe = roll(3,6)+2; pb = roll(3,6);   spd= roll(3,6);
        break;

    case "basilisk": /* large reptilian creature */ /* approx */
        iq  = roll(1,6);   me = roll(2,6);   ma = roll(1,6);   ps = roll(4,6)+12;
        pp  = roll(2,6);   pe = roll(4,6)+10; pb= roll(1,6);   spd= roll(2,6);
        break;

    /* ── Faeries and Small Folk ───────────────────────────────────────────── */

    case "common faerie": /* approx */
        iq  = roll(2,6)+6; me = roll(3,6)+6; ma = roll(3,6)+6; ps = roll(2,6);
        pp  = roll(3,6)+6; pe = roll(2,6)+6; pb = roll(3,6)+6; spd= roll(4,6);
        break;

    case "common pixie": /* small winged faerie */ /* approx */
        iq  = roll(3,6)+4; me = roll(3,6)+2; ma = roll(3,6)+2; ps = roll(1,6);
        pp  = roll(4,6)+4; pe = roll(2,6)+2; pb = roll(4,6)+4; spd= roll(4,6)+6;
        break;

    case "frost pixie": /* cold-dwelling pixie */ /* approx */
        iq  = roll(3,6)+2; me = roll(3,6)+2; ma = roll(3,6);   ps = roll(1,6);
        pp  = roll(4,6)+4; pe = roll(2,6);   pb = roll(4,6)+2; spd= roll(4,6);
        break;

    case "green wood faerie": /* nature faerie */ /* approx */
        iq  = roll(3,6)+4; me = roll(3,6)+2; ma = roll(4,6)+4; ps = roll(1,6)+1;
        pp  = roll(3,6)+4; pe = roll(2,6);   pb = roll(4,6)+4; spd= roll(3,6)+4;
        break;

    case "night-elves faerie": /* dark faerie folk */ /* approx */
        iq  = roll(3,6)+6; me = roll(3,6)+6; ma = roll(3,6)+4; ps = roll(2,6)+2;
        pp  = roll(3,6)+4; pe = roll(2,6)+4; pb = roll(3,6)+6; spd= roll(3,6);
        break;

    case "silver bells faerie": /* musical faerie */ /* approx */
        iq  = roll(3,6)+4; me = roll(3,6)+4; ma = roll(4,6)+4; ps = roll(1,6);
        pp  = roll(3,6)+2; pe = roll(2,6)+2; pb = roll(4,6)+6; spd= roll(3,6);
        break;

    case "tree sprite": /* small nature spirit */ /* approx */
        iq  = roll(2,6)+4; me = roll(3,6)+6; ma = roll(4,6)+6; ps = roll(1,6)+1;
        pp  = roll(3,6);   pe = roll(2,6);   pb = roll(4,6);   spd= roll(2,6);
        break;

    case "water sprite": /* aquatic faerie */ /* approx */
        iq  = roll(2,6)+4; me = roll(3,6)+4; ma = roll(3,6)+4; ps = roll(1,6)+1;
        pp  = roll(3,6)+2; pe = roll(2,6)+2; pb = roll(3,6)+4; spd= roll(2,6)+4;
        break;

    case "bogie": /* small mischievous creature, Conversion Book */ /* approx */
        iq  = roll(2,6);   me = roll(2,6);   ma = roll(2,6);   ps = roll(3,6)+3;
        pp  = roll(3,6);   pe = roll(3,6);   pb = roll(1,6);   spd= roll(3,6);
        break;

    case "brownie": /* tiny helpful faerie */ /* approx */
        iq  = roll(3,6)+2; me = roll(3,6);   ma = roll(3,6)+2; ps = roll(2,6);
        pp  = roll(3,6)+2; pe = roll(2,6);   pb = roll(3,6)+2; spd= roll(3,6)+4;
        break;

    /* ── MDC Beings  -  Gargoyles ───────────────────────────────────────────── */

    case "gargoyle": /* MDC supernatural predator, Rifts core */ /* approx */
        iq  = roll(2,6)+2; me = roll(3,6);   ma = roll(2,6);   ps = roll(5,6)+10;
        pp  = roll(3,6)+2; pe = roll(5,6)+6; pb = roll(1,6);   spd= roll(3,6)+6;
        break;

    case "gurgoyle": /* MDC elite gargoyle variant, Rifts core */ /* approx */
        iq  = roll(2,6)+4; me = roll(3,6)+2; ma = roll(2,6)+2; ps = roll(5,6)+15;
        pp  = roll(3,6)+4; pe = roll(5,6)+10; pb= roll(1,6)+1; spd= roll(3,6)+8;
        break;

    /* ── MDC Beings  -  Were-Creatures ──────────────────────────────────────── */

    case "werewolf": /* MDC supernatural lycanthrope */ /* approx */
        iq  = roll(3,6);   me = roll(3,6);   ma = roll(3,6);   ps = roll(4,6)+8;
        pp  = roll(3,6)+4; pe = roll(4,6)+6; pb = roll(3,6);   spd= roll(4,6)+10;
        break;

    case "werebear": /* MDC supernatural lycanthrope */ /* approx */
        iq  = roll(3,6);   me = roll(3,6);   ma = roll(3,6);   ps = roll(5,6)+8;
        pp  = roll(3,6);   pe = roll(4,6)+8; pb = roll(3,6);   spd= roll(3,6)+4;
        break;

    case "weretiger": /* MDC supernatural lycanthrope */ /* approx */
        iq  = roll(3,6);   me = roll(3,6);   ma = roll(3,6);   ps = roll(4,6)+8;
        pp  = roll(3,6)+4; pe = roll(4,6)+6; pb = roll(3,6)+2; spd= roll(4,6)+4;
        break;

    /* ── MDC Beings  -  Vampires ────────────────────────────────────────────── */

    case "secondary vampire": /* MDC undead, Vampire Kingdoms */ /* approx */
        iq  = roll(3,6);   me = roll(3,6);   ma = roll(4,6);   ps = roll(4,6)+8;
        pp  = roll(3,6)+4; pe = roll(3,6)+8; pb = roll(4,6);   spd= roll(4,6)+4;
        break;

    case "wild vampire": /* MDC feral undead, Vampire Kingdoms */ /* approx */
        iq  = roll(3,6)+6; me = roll(3,6)+4; ma = roll(4,6)+4; ps = roll(4,6)+12;
        pp  = roll(3,6)+6; pe = roll(4,6)+10; pb= roll(4,6)+4; spd= roll(4,6)+8;
        break;

    /* ── MDC Beings  -  Dragons ─────────────────────────────────────────────── */

    case "fire dragon": /* young fire dragon, Rifts core */ /* approx */
        iq  = roll(3,6)+12; me = roll(3,6)+10; ma = roll(3,6)+8;  ps = roll(5,6)+20;
        pp  = roll(3,6)+6;  pe = roll(4,6)+20; pb = roll(3,6)+3;  spd= roll(4,6)+10;
        break;

    case "ice dragon": /* young ice dragon */ /* approx */
        iq  = roll(3,6)+12; me = roll(3,6)+10; ma = roll(3,6)+8;  ps = roll(5,6)+20;
        pp  = roll(3,6)+6;  pe = roll(4,6)+20; pb = roll(3,6);    spd= roll(4,6)+10;
        break;

    case "great horned dragon": /* Rifts core  --  most powerful common dragon */ /* approx */
        iq  = roll(3,6)+15; me = roll(3,6)+14; ma = roll(3,6)+12; ps = roll(6,6)+30;
        pp  = roll(4,6)+8;  pe = roll(5,6)+25; pb = roll(3,6)+3;  spd= roll(4,6)+10;
        break;

    case "thunder lizard dragon": /* massive reptilian dragon */ /* approx */
        iq  = roll(3,6)+8;  me = roll(3,6)+8;  ma = roll(3,6)+6;  ps = roll(6,6)+20;
        pp  = roll(3,6)+4;  pe = roll(5,6)+20; pb = roll(2,6);    spd= roll(4,6)+8;
        break;

    /* ── True Atlanteans (WB2: Atlantis) ────────────────────────────────────── */

    case "atlantean": /* True Atlantean: marked, dimensional traveller */
        iq  = roll(3,6);   me = roll(3,6);   ma = roll(3,6);   ps = roll(3,6)+6;
        pp  = roll(3,6);   pe = roll(3,6)+6; pb = roll(3,6)+6; spd= roll(3,6);
        break;

    /* ── Avian D-Bees (Splynn Dimensional Market area) ──────────────────────── */

    case "hawrk-duhk": /* large avian MDC D-Bee */ /* approx */
        iq  = roll(3,6);   me = roll(3,6);   ma = roll(2,6)+2; ps = roll(4,6)+6;
        pp  = roll(3,6)+4; pe = roll(3,6)+2; pb = roll(2,6);   spd= roll(3,6)+8;
        break;

    case "hawrk-ka": /* smaller agile avian MDC D-Bee */ /* approx */
        iq  = roll(3,6)+2; me = roll(3,6);   ma = roll(3,6);   ps = roll(3,6)+2;
        pp  = roll(4,6)+2; pe = roll(3,6);   pb = roll(2,6)+2; spd= roll(4,6)+6;
        break;

    /* ── Wild Psi-Stalker ───────────────────────────────────────────────────── */

    case "wild psi-stalker": /* unaffiliated psionic hunter */ /* approx */
        iq  = roll(3,6);   me = roll(3,6)+4; ma = roll(3,6)+2; ps = roll(3,6)+4;
        pp  = roll(3,6)+4; pe = roll(3,6)+4; pb = roll(2,6)+2; spd= roll(3,6)+4;
        break;

    /* ── Kittani (Splugorth-Aligned Alien Race) ─────────────────────────────── */

    case "kittani": /* insectoid alien humanoid, WB2: Atlantis */ /* approx */
        iq  = roll(3,6)+2; me = roll(3,6)+2; ma = roll(2,6)+2; ps = roll(3,6)+4;
        pp  = roll(3,6)+4; pe = roll(3,6)+4; pb = roll(2,6);   spd= roll(3,6)+4;
        break;

    /* ── New Rifts Races ────────────────────────────────────────────────────── */

    case "wolfen": /* large wolf-man humanoid, WB4 Africa */ /* approx */
        iq  = roll(3,6);   me = roll(3,6)+2; ma = roll(3,6);   ps = roll(3,6)+6;
        pp  = roll(3,6)+2; pe = roll(3,6)+2; pb = roll(3,6);   spd= roll(3,6)+4;
        break;

    case "dragon hatchling": /* young wyrmling, MDC being */ /* approx */
        iq  = roll(3,6)+6; me = roll(3,6)+4; ma = roll(3,6)+2; ps = roll(4,6)+8;
        pp  = roll(3,6)+2; pe = roll(4,6)+8; pb = roll(2,6)+4; spd= roll(3,6)+4;
        break;

    case "brodkil": /* demonic MDC bionic being, Rifts core */ /* approx */
        iq  = roll(1,6)+2; me = roll(2,6);   ma = roll(2,6);   ps = roll(5,6)+10;
        pp  = roll(3,6);   pe = roll(4,6)+8; pb = roll(1,6);   spd= roll(2,6)+6;
        break;

    case "simvan": /* monster-rider nomad humanoid, Rifts core */ /* approx */
        iq  = roll(3,6);   me = roll(3,6)+2; ma = roll(3,6);   ps = roll(3,6)+2;
        pp  = roll(3,6);   pe = roll(3,6)+2; pb = roll(2,6)+2; spd= roll(3,6);
        break;

    case "lyn-srial": /* felinoid winged being, Rifts WB */ /* approx */
        iq  = roll(3,6)+2; me = roll(3,6)+2; ma = roll(3,6)+2; ps = roll(3,6)+2;
        pp  = roll(3,6)+4; pe = roll(3,6)+2; pb = roll(3,6)+4; spd= roll(3,6)+4;
        break;

    default:
        return do_mutant_rolls(race);
    }

    return ([
        /* Palladium 8 attributes */
        "IQ": iq, "ME": me, "MA": ma, "PS": ps,
        "PP": pp, "PE": pe, "PB": pb, "Spd": spd,
        /* D&D shadow keys for Nightmare formula compatibility */
        "strength":     ps,
        "constitution": pe,
        "intelligence": iq,
        "wisdom":       me,
        "dexterity":    pp,
        "charisma":     pb
    ]);
}

/* Returns 1 for inherently MDC (Mega-Damage Capacity) beings. */
/* MDC beings use MDC instead of SDC as their primary damage pool. */
int is_mdc_race(string race) {
    if(!race || race == "") return 0;
    switch(lower_case(race)) {
    case "godling":
    case "fire dragon":
    case "ice dragon":
    case "great horned dragon":
    case "thunder lizard dragon":
    case "dragon hatchling":
    case "brodkil":
    case "gargoyle":
    case "gurgoyle":
    case "hawrk-duhk":
    case "hawrk-ka":
    case "secondary vampire":
    case "wild vampire":
    case "werewolf":
    case "werebear":
    case "weretiger":
    case "atlantean":
        return 1;
    default:
        return 0;
    }
}

/* Initial SDC at character creation (before OCC bonuses). */
/* Returns 0 for MDC races  -  use init_mdc() instead. */
int init_sdc(string race, int pe) {
    if(!race || race == "") return 0;
    switch(lower_case(race)) {
    case "human":
    case "burster":
    case "mind melter":
    case "cs psi-stalker":
    case "wild psi-stalker":
        return (roll(1,4) * 10) + pe;

    case "atlantean":
        return (roll(1,6) * 10) + pe;

    case "dog boy":       return roll(2,6) * 10;
    case "changeling":    return (roll(1,6) * 10) + pe; /* approx */
    case "dwarf":         return roll(2,6) * 10;
    case "elf":           return roll(2,6) * 10;
    case "goblin":        return roll(2,6) * 10;
    case "orc":           return roll(2,6) * 10;
    case "ratling":       return roll(2,6) * 10;
    case "coyle":         return roll(2,6) * 10; /* approx */
    case "kankoran":      return roll(3,6) * 10; /* approx */
    case "bearman":       return roll(3,6) * 10; /* approx */
    case "ogre":          return roll(3,6) * 10;
    case "troll":         return roll(3,6) * 10;
    case "algor frost giant": return roll(4,6) * 10; /* approx */
    case "jotan":         return roll(4,6) * 10; /* approx */
    case "nimro fire giant":  return roll(4,6) * 10; /* approx */
    case "titan":         return roll(4,6) * 10; /* approx */
    case "rahu-man":      return roll(4,6) * 10; /* approx */
    case "equinoid":      return roll(3,6) * 10; /* approx */
    case "pogtal":        return roll(3,6) * 10; /* approx */
    case "conservator":   return roll(2,6) * 10; /* approx */
    case "basilisk":      return roll(3,6) * 10; /* approx */
    case "wolfen":        return roll(2,6) * 10; /* approx */
    case "simvan":        return roll(2,6) * 10; /* approx */
    case "lyn-srial":     return roll(2,6) * 10; /* approx */
    case "mutant rat":    return roll(1,6) * 10 + 20;
    case "mutant dog":    return roll(2,6) * 10 + 10;
    case "mutant bear":   return roll(3,6) * 10 + 50;
    case "mutant cat":    return roll(1,6) * 10 + 15;
    case "mutant hawk":   return roll(1,6) * 10 + 10;
    case "mutant wolf":   return roll(2,6) * 10 + 20;
    case "bogie":         return roll(2,6) * 10; /* approx */
    case "brownie":       return roll(1,6) * 10; /* approx */
    case "common faerie": return roll(1,4) * 10;
    case "common pixie":  return roll(1,4) * 10; /* approx */
    case "frost pixie":   return roll(1,4) * 10; /* approx */
    case "green wood faerie": return roll(1,4) * 10; /* approx */
    case "night-elves faerie": return roll(1,4) * 10; /* approx */
    case "silver bells faerie": return roll(1,4) * 10; /* approx */
    case "tree sprite":   return roll(1,4) * 10; /* approx */
    case "water sprite":  return roll(1,4) * 10; /* approx */

    /* MDC races have no SDC */
    default:
        return 0;
    }
}

/* Initial MDC for MDC beings at character creation. */
/* Returns 0 for SDC races  -  use init_sdc() instead. */
int init_mdc(string race, int pe) {
    if(!race || race == "") return 0;
    switch(lower_case(race)) {
    case "godling":   return 5000;
    case "atlantean": return (roll(1,6) * 10) + pe;  /* approx */
    case "gargoyle":          return (roll(3,6) * 10);           /* approx */
    case "gurgoyle":          return (roll(4,6) * 10);           /* approx */
    case "hawrk-duhk":        return (roll(3,6) * 10) + pe;      /* approx */
    case "hawrk-ka":          return (roll(2,6) * 10) + pe;      /* approx */
    case "werewolf":          return (roll(3,6) * 10) + 30;      /* approx */
    case "werebear":          return (roll(3,6) * 10) + 50;      /* approx */
    case "weretiger":         return (roll(3,6) * 10) + 40;      /* approx */
    case "secondary vampire": return (roll(3,6) * 10) + 40;      /* approx */
    case "wild vampire":      return (roll(4,6) * 10) + 60;      /* approx */
    case "dragon hatchling":  return (roll(3,6) * 10);           /* approx */
    case "brodkil":           return (roll(4,6) * 10) + 100;     /* approx */
    case "fire dragon":       return (roll(4,6) * 10) + 60;      /* approx */
    case "ice dragon":        return (roll(4,6) * 10) + 60;      /* approx */
    case "great horned dragon":     return (roll(6,6) * 10) + 100; /* approx */
    case "thunder lizard dragon":   return (roll(5,6) * 10) + 80;  /* approx */
    default:
        return 0;
    }
}

/* Base HP at level 1 (PE + one level roll). */
/* Full per-level HP rolls happen at level-up. */
int init_hp(string race, int pe) {
    if(!race || race == "") return pe + roll(1,6);
    switch(lower_case(race)) {
    case "godling":  return 1000;
    case "dog boy":
    case "cs psi-stalker":
    case "wild psi-stalker":
    case "coyle":
    case "kankoran":
    case "hawrk-duhk":
        return pe + roll(2,6);
    case "gargoyle":
    case "gurgoyle":
    case "werewolf":
    case "werebear":
    case "weretiger":
    case "secondary vampire":
    case "wild vampire":
    case "fire dragon":
    case "ice dragon":
    case "great horned dragon":
    case "thunder lizard dragon":
        return pe + roll(2,6); /* MDC beings still have HP as inner life force */
    default:
        return pe + roll(1,6);
    }
}

/* Race ability flags  -  presence on the player marks the ability exists. */
/* Effect code is implemented separately; these are data-only markers. */
string *query_race_flags(string race) {
    if(!race || race == "") return ({});
    switch(lower_case(race)) {
    case "godling":
        return ({
            "supernatural_ps", "supernatural_pe", "mdc_body",
            "master_psionic", "magic_ability", "divine_aura",
            "fire_immunity", "cold_immunity", "electricity_immunity",
            "flight", "nightvision", "regeneration", "immortal",
            "sense_rifts", "telemechanics", "telepathy", "telekinesis",
            "empathy", "mind_block", "faerie_magic", "shapeshift_humanoid",
            "enhanced_smell", "enhanced_hearing", "enhanced_vision"
        });

    case "dog boy":
    case "cs psi-stalker":
        return ({ "supernatural_ps", "supernatural_pe",
                  "enhanced_smell", "enhanced_hearing",
                  "enhanced_taste", "thermal_vision" });

    case "wild psi-stalker":
        return ({ "supernatural_ps", "supernatural_pe",
                  "enhanced_smell", "enhanced_hearing",
                  "enhanced_taste", "thermal_vision", "psi_sense" });

    case "atlantean":
        return ({ "sense_rifts", "ley_line_navigation",
                  "magic_tattoos", "dimension_sense",
                  "magic_resistance" });

    case "hawrk-duhk":
        return ({ "supernatural_ps", "supernatural_pe",
                  "mdc_body", "flight", "enhanced_vision",
                  "diving_attack" });

    case "hawrk-ka":
        return ({ "supernatural_ps", "supernatural_pe",
                  "mdc_body", "flight", "enhanced_vision",
                  "nightvision" });

    case "coyle":
    case "kankoran":
        return ({ "supernatural_ps", "supernatural_pe",
                  "enhanced_smell", "enhanced_hearing" });

    case "bearman":
        return ({ "supernatural_ps", "supernatural_pe",
                  "enhanced_smell", "hibernation" });

    case "burster":
        return ({ "psionic_pyrokinesis", "immune_to_fire_psionics" });

    case "mind melter":
        return ({ "master_psionic", "telepathy", "mind_block",
                  "telekinesis", "empathy" });

    case "gargoyle":
    case "gurgoyle":
        return ({ "supernatural_ps", "supernatural_pe",
                  "mdc_body", "nightvision", "flight" });

    case "troll":
        return ({ "supernatural_ps", "supernatural_pe", "mdc_body",
                  "regeneration", "giant_size" });

    case "ogre":
        return ({ "supernatural_ps", "supernatural_pe", "giant_size" });

    case "werewolf":
    case "werebear":
    case "weretiger":
        return ({ "supernatural_ps", "supernatural_pe", "mdc_body",
                  "regeneration", "silver_vulnerability",
                  "lycanthropy", "enhanced_smell" });

    case "secondary vampire":
    case "wild vampire":
        return ({ "supernatural_ps", "supernatural_pe", "mdc_body",
                  "undead", "regeneration", "sunlight_vulnerability",
                  "cannot_enter_uninvited", "wild_fire_vulnerability" });

    case "fire dragon":
        return ({ "supernatural_ps", "supernatural_pe", "mdc_body",
                  "fire_breath", "fire_immunity", "flight", "nightvision",
                  "telemechanics", "sense_rifts" });

    case "ice dragon":
        return ({ "supernatural_ps", "supernatural_pe", "mdc_body",
                  "ice_breath", "cold_immunity", "flight", "nightvision",
                  "telemechanics", "sense_rifts" });

    case "great horned dragon":
        return ({ "supernatural_ps", "supernatural_pe", "mdc_body",
                  "fire_breath", "acid_breath", "flight", "nightvision",
                  "psionics_master", "magic_ability", "sense_rifts" });

    case "thunder lizard dragon":
        return ({ "supernatural_ps", "supernatural_pe", "mdc_body",
                  "lightning_breath", "electricity_immunity",
                  "flight", "nightvision", "sense_rifts" });

    case "rahu-man":
        return ({ "supernatural_ps", "supernatural_pe",
                  "enhanced_vision", "enhanced_smell" });

    case "common faerie":
    case "common pixie":
    case "frost pixie":
    case "green wood faerie":
    case "night-elves faerie":
    case "silver bells faerie":
        return ({ "flight", "faerie_magic", "invisible_to_machines" });

    case "tree sprite":
    case "water sprite":
    case "brownie":
        return ({ "nature_magic", "diminutive" });

    case "wolfen":
        return ({ "supernatural_ps", "supernatural_pe",
                  "enhanced_smell", "enhanced_hearing" });

    case "dragon hatchling":
        return ({ "supernatural_ps", "supernatural_pe", "mdc_body",
                  "flight", "nightvision", "sense_rifts",
                  "fire_breath", "telemechanics" });

    case "brodkil":
        return ({ "supernatural_ps", "supernatural_pe", "mdc_body",
                  "bionic_integrated", "nightvision" });

    case "simvan":
        return ({ "monster_empathy", "wilderness_sense" });

    case "lyn-srial":
        return ({ "flight", "enhanced_vision", "nightvision",
                  "claw_attack" });

    case "equinoid":
        return ({ "supernatural_ps", "supernatural_pe", "hooves" });

    case "jotan":
    case "algor frost giant":
    case "nimro fire giant":
    case "titan":
        return ({ "supernatural_ps", "supernatural_pe", "giant_size" });

    case "changeling":
        return ({ "shapeshift_humanoid", "natural_mimic" });

    case "mutant rat":
        return ({ "nightvision", "enhanced_smell", "climbing", "bite_attack" });

    case "mutant dog":
        return ({ "enhanced_smell", "enhanced_hearing", "nightvision",
                  "bite_attack", "disease_resistance" });

    case "mutant bear":
        return ({ "enhanced_smell", "claws", "bite_attack", "hibernation",
                  "fear_factor" });

    case "mutant cat":
        return ({ "nightvision", "retractable_claws", "prowl_bonus",
                  "fall_resistance" });

    case "mutant hawk":
        return ({ "flight", "telescopic_vision", "talon_attack",
                  "flight_capable" });

    case "mutant wolf":
        return ({ "enhanced_smell", "nightvision", "bite_attack",
                  "pack_tactics", "howl" });

    default:
        return ({});
    }
}

/* ── Race data structures ─────────────────────────────────────────────────── */
/* */
/* query_race_data  -  returns a mapping with structured race information. */
/* Fields: */
/* abilities       -  string array of special abilities (human-readable) */
/* penalties       -  string array of weaknesses or restrictions */
/* innate_bonuses  -  mapping of stat modifiers already baked into rolls */
/* body_type       -  "humanoid", "giant", "dragon", "faerie", "supernatural" */
/* is_psychic      -  1 if race has innate psionic powers */
/* is_magical      -  1 if race has innate magic ability */
/* is_mdc          -  1 if race is an MDC being (mirrors is_mdc_race()) */
/* ppe_base        -  racial PPE available to any non-magic-user of this race */
/* */
mapping query_race_data(string race) {
    string *flags;
    if(!race || race == "") return 0;
    flags = query_race_flags(race);

    switch(lower_case(race)) {

    case "godling":
        return ([
            "abilities": ({
                "All attributes fixed at 30: divine maximum",
                "MDC body: 5000 MDC",
                "1000 rifts HP (inner life force)",
                "2000 starting PPE (magical power reserve)",
                "2000 starting ISP (psionic power reserve)",
                "Master psionic: all psionic categories",
                "Full magic ability: all spell levels",
                "Regeneration: rapid MDC recovery",
                "Immunity to fire, cold, and electricity",
                "Natural flight, nightvision, enhanced senses",
                "Sense Rifts and ley lines",
                "Telemechanics: understand any machine",
                "Divine aura: all aligned beings sense your nature",
                "Immortal: cannot die of old age"
            }),
            "penalties": ({}),
            "innate_bonuses": ([
                "IQ": 30, "ME": 30, "MA": 30, "PS": 30,
                "PP": 30, "PE": 30, "PB": 30, "Spd": 30
            ]),
            "body_type":  "divine",
            "is_psychic": 1,
            "is_magical": 1,
            "is_mdc":     1,
            "ppe_base":   2000
        ]);

    case "human":
        return ([
            "abilities":    ({ "Adaptable: bonus secondary skills", "No racial penalties" }),
            "penalties":    ({}),
            "innate_bonuses": ([]),
            "body_type":    "humanoid",
            "is_psychic":   0,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     6  /* approx */
        ]);

    case "burster":
        return ([
            "abilities":    ({ "Pyrokinesis: create and control fire psionically",
                               "Immunity to fire and heat from psionic origin",
                               "Enhanced ME attribute" }),
            "penalties":    ({ "Not OCC eligible: racial powers define role",
                               "Emotional instability under stress" }),
            "innate_bonuses": ([ "ME": 2 ]),
            "body_type":    "humanoid",
            "is_psychic":   1,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     6  /* approx */
        ]);

    case "mind melter":
        return ([
            "abilities":    ({ "Master psionic: access to all psionic categories",
                               "Telepathy", "Telekinesis", "Empathy",
                               "Mind Block auto-defense", "Enhanced IQ, ME, and MA" }),
            "penalties":    ({ "Not OCC eligible: psionic mastery is the path",
                               "Subject to psychic burnout if ISP depleted" }),
            "innate_bonuses": ([ "IQ": 2, "ME": 4, "MA": 2 ]),
            "body_type":    "humanoid",
            "is_psychic":   1,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     6  /* approx */
        ]);

    case "dog boy":
        return ([
            "abilities":    ({ "Supernatural Physical Strength",
                               "Supernatural Physical Endurance",
                               "Enhanced sense of smell: track by scent",
                               "Enhanced hearing: detect ambushes",
                               "Enhanced taste: detect poisons",
                               "Thermal vision",
                               "Psionically sensitive: sense magic and psionics" }),
            "penalties":    ({ "Not OCC eligible: Coalition military role",
                               "Coalition loyalty conditioning",
                               "Susceptible to sonic attacks" }),
            "innate_bonuses": ([ "IQ": 1, "PS": 6, "PE": 6, "PP": 1 ]),
            "body_type":    "humanoid",
            "is_psychic":   1,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     4  /* approx */
        ]);

    case "cs psi-stalker":
        return ([
            "abilities":    ({ "Psionic bloodhound: sense and track psionic/magic beings",
                               "Supernatural Physical Strength and Endurance",
                               "Feed on psionic energy (ISP/PPE drain from others)" }),
            "penalties":    ({ "Not OCC eligible: Coalition role defines abilities",
                               "Must feed on psychic energy regularly" }),
            "innate_bonuses": ([ "ME": 4, "PS": 4, "PE": 4, "PP": 4 ]),
            "body_type":    "humanoid",
            "is_psychic":   1,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     4  /* approx */
        ]);

    case "gargoyle":
    case "gurgoyle":
        return ([
            "abilities":    ({ "MDC body", "Supernatural Physical Strength and Endurance",
                               "Natural flight (wings)", "Nightvision 300 ft",
                               "Claws and teeth as weapons" }),
            "penalties":    ({ "Not OCC eligible", "Horrific appearance (low MA)",
                               "Subject to holy weapons and magic" }),
            "innate_bonuses": ([]),
            "body_type":    "supernatural",
            "is_psychic":   0,
            "is_magical":   0,
            "is_mdc":       1,
            "ppe_base":     20  /* approx */
        ]);

    case "werewolf":
    case "werebear":
    case "weretiger":
        return ([
            "abilities":    ({ "MDC body", "Supernatural Physical Strength and Endurance",
                               "Rapid regeneration: heal 2d6 MDC per melee round",
                               "Enhanced smell and hearing",
                               "Immunity to normal weapons (silver required)" }),
            "penalties":    ({ "Not OCC eligible",
                               "Vulnerable to silver: double damage",
                               "Lycanthropy infection risk to others" }),
            "innate_bonuses": ([]),
            "body_type":    "supernatural",
            "is_psychic":   0,
            "is_magical":   0,
            "is_mdc":       1,
            "ppe_base":     10  /* approx */
        ]);

    case "secondary vampire":
    case "wild vampire":
        return ([
            "abilities":    ({ "MDC body", "Supernatural Physical Strength and Endurance",
                               "Rapid regeneration", "Hypnotic gaze",
                               "Immunity to normal weapons" }),
            "penalties":    ({ "Not OCC eligible",
                               "Sunlight: 2d6 MDC per melee round exposure",
                               "Wild fire: double MDC damage",
                               "Cannot enter dwelling without invitation",
                               "Stake through heart: immediate death" }),
            "innate_bonuses": ([]),
            "body_type":    "supernatural",
            "is_psychic":   0,
            "is_magical":   0,
            "is_mdc":       1,
            "ppe_base":     25  /* approx */
        ]);

    case "fire dragon":
    case "ice dragon":
    case "great horned dragon":
    case "thunder lizard dragon":
        return ([
            "abilities":    ({ "MDC body", "Supernatural Physical Strength and Endurance",
                               "Natural flight", "Nightvision",
                               "Breath weapon (varies by type)",
                               "Sense Rifts and ley lines",
                               "Innate magic and/or psionic ability",
                               "Telemechanics: understand any machine" }),
            "penalties":    ({ "Not OCC eligible: ancient power defines the path",
                               "Dragon-sized: cannot enter most buildings",
                               "Subject to dragon hunters and Coalition aggression" }),
            "innate_bonuses": ([]),
            "body_type":    "dragon",
            "is_psychic":   1,
            "is_magical":   1,
            "is_mdc":       1,
            "ppe_base":     200  /* approx  --  dragons have enormous PPE */
        ]);

    case "common faerie":
    case "common pixie":
    case "frost pixie":
    case "green wood faerie":
    case "night-elves faerie":
    case "silver bells faerie":
    case "tree sprite":
    case "water sprite":
    case "brownie":
        return ([
            "abilities":    ({ "Natural flight (most faerie types)",
                               "Faerie magic: minor enchantments and illusions",
                               "Invisible to electronic detection",
                               "Communicate with animals and nature" }),
            "penalties":    ({ "Not OCC eligible",
                               "Very small: limited reach and carrying capacity",
                               "Vulnerable to cold iron",
                               "Low Physical Strength" }),
            "innate_bonuses": ([]),
            "body_type":    "faerie",
            "is_psychic":   0,
            "is_magical":   1,
            "is_mdc":       0,
            "ppe_base":     30  /* approx  --  faerie are magic-rich */
        ]);

    case "algor frost giant":
    case "jotan":
    case "nimro fire giant":
    case "titan":
        return ([
            "abilities":    ({ "Supernatural Physical Strength and Endurance",
                               "Giant size: massive reach and damage",
                               "Resilience to environmental extremes" }),
            "penalties":    ({ "Giant size: cannot use standard equipment",
                               "Conspicuous in human settlements",
                               "Require vast quantities of food and water" }),
            "innate_bonuses": ([]),
            "body_type":    "giant",
            "is_psychic":   0,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     8  /* approx */
        ]);

    case "mutant rat":
        return ([
            "abilities":    ({ "Nightvision 60 ft", "Climb 80%",
                               "Smell enemies within 200 ft",
                               "Bite attack: 1d4 SDC",
                               "Natural AR 8: tough hide",
                               "+2 IQ, +2 PP, +4 Spd" }),
            "penalties":    ({ "Not OCC eligible",
                               "-4 PB: humans react with fear or disgust" }),
            "innate_bonuses": ([ "IQ": 2, "PP": 2, "Spd": 4 ]),
            "body_type":    "humanoid",
            "is_psychic":   0,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     4
        ]);

    case "mutant dog":
        return ([
            "abilities":    ({ "Tracking by scent 90%",
                               "Nightvision 100 ft",
                               "+2 to initiative (keen hearing)",
                               "Bite attack: 2d4 SDC",
                               "Natural AR 9",
                               "+1 to save vs poisons and disease",
                               "+3 PS, +2 PP, +2 PE" }),
            "penalties":    ({ "Not OCC eligible" }),
            "innate_bonuses": ([ "PS": 3, "PP": 2, "PE": 2 ]),
            "body_type":    "humanoid",
            "is_psychic":   0,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     4
        ]);

    case "mutant bear":
        return ([
            "abilities":    ({ "Claws: 2d6 SDC",
                               "Bite: 2d6 SDC",
                               "Hibernation ability",
                               "Natural AR 12: thick hide",
                               "+8 PS, +6 PE" }),
            "penalties":    ({ "Not OCC eligible",
                               "-2 PP, -2 Spd, -4 PB",
                               "Fear factor vs lower-PS beings (future system)" }),
            "innate_bonuses": ([ "PS": 8, "PE": 6 ]),
            "body_type":    "humanoid",
            "is_psychic":   0,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     4
        ]);

    case "mutant cat":
        return ([
            "abilities":    ({ "Nightvision 200 ft",
                               "Prowl +20% racial bonus",
                               "Land on feet: no fall damage under 30 ft",
                               "Retractable claws: 1d6 SDC",
                               "+2 to dodge, +1 to initiative",
                               "+4 PP, +6 Spd" }),
            "penalties":    ({ "Not OCC eligible" }),
            "innate_bonuses": ([ "PP": 4, "Spd": 6 ]),
            "body_type":    "humanoid",
            "is_psychic":   0,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     4
        ]);

    case "mutant hawk":
        return ([
            "abilities":    ({ "Flight capable (wings, 60 mph)",
                               "Telescopic vision: see clearly at 1000 ft",
                               "Talon attack: 1d6 SDC",
                               "+4 PP, +2 PB" }),
            "penalties":    ({ "Not OCC eligible",
                               "Wings restrict armor: -20% to physical skills in most armor" }),
            "innate_bonuses": ([ "PP": 4, "PB": 2 ]),
            "body_type":    "humanoid",
            "is_psychic":   0,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     4
        ]);

    case "mutant wolf":
        return ([
            "abilities":    ({ "Tracking by scent 85%",
                               "Pack tactics: +1 to strike when allies present",
                               "Nightvision 150 ft",
                               "Bite attack: 2d6 SDC",
                               "Howl: rally nearby allies",
                               "Natural AR 10",
                               "+4 PS, +3 PP, +3 PE, +8 Spd" }),
            "penalties":    ({ "Not OCC eligible" }),
            "innate_bonuses": ([ "PS": 4, "PP": 3, "PE": 3, "Spd": 8 ]),
            "body_type":    "humanoid",
            "is_psychic":   0,
            "is_magical":   0,
            "is_mdc":       0,
            "ppe_base":     4
        ]);

    default:
        /* Generic fallback for races without a detailed data block */
        return ([
            "abilities":    query_race_flags(race),
            "penalties":    ({}),
            "innate_bonuses": ([]),
            "body_type":    "humanoid",
            "is_psychic":   (member_array("master_psionic", flags) != -1 ||
                             member_array("psionic_pyrokinesis", flags) != -1) ? 1 : 0,
            "is_magical":   (member_array("magic_ability", flags) != -1 ||
                             member_array("faerie_magic", flags) != -1) ? 1 : 0,
            "is_mdc":       is_mdc_race(race),
            "ppe_base":     6
        ]);
    }
}

/* query_race_abilities  -  human-readable ability list for a race. */
string *query_race_abilities(string race) {
    mapping d;
    d = query_race_data(race);
    if(!d) return ({});
    return (string *)d["abilities"];
}

/* query_race_penalties  -  human-readable penalty/restriction list for a race. */
string *query_race_penalties(string race) {
    mapping d;
    d = query_race_data(race);
    if(!d) return ({});
    return (string *)d["penalties"];
}

string query_race_description(string race) {
    if(!race || race == "") return "";
    switch(lower_case(race)) {
    case "godling":
        return
            "You are a Godling: a transcendent divine being beyond the limits\n"
            "of mortal existence.  Your attributes are fixed at their absolute maximum.\n"
            "You command the full spectrum of psionic and magical power, an MDC body\n"
            "of 5000, and immortality.  This state is not chosen: it is earned.\n"
            "Not OCC eligible.";

    case "human":
        return
            "You are a Human: adaptable survivors of the Rifts apocalypse.\n"
            "Humans are OCC eligible: you will choose an Occupational Character\n"
            "Class that defines your training and abilities.";

    case "dog boy":
        return
            "You are a Dog Boy: a Coalition States genetic creation fusing canine\n"
            "instincts with a near-human body.  You possess Supernatural Physical\n"
            "Strength and Endurance, and superior senses of smell, hearing, and taste.\n"
            "Dog Boys are NOT OCC eligible; your race defines your abilities.";

    case "algor frost giant":
        return
            "You are an Algor Frost Giant: a towering supernatural humanoid\n"
            "native to the frozen northern wastelands.  You are OCC eligible.";

    case "basilisk":
        return
            "You are a Basilisk: a massive reptilian predator with a petrifying\n"
            "gaze and supernatural resilience.  Not OCC eligible.";

    case "bearman":
        return
            "You are a Bearman of the Ice Wilderness: a powerful ursine humanoid\n"
            "with Supernatural Physical Strength and Endurance.  OCC eligible.";

    case "bogie":
        return
            "You are a Bogie: a squat and mischievous creature from the faerie\n"
            "world with a talent for tricks and minor magic.  Not OCC eligible.";

    case "brownie":
        return
            "You are a Brownie: a tiny helpful faerie folk known for industry\n"
            "and minor enchantments.  Not OCC eligible.";

    case "burster":
        return
            "You are a Burster: a psionic mutant human with the power of\n"
            "pyrokinesis: the ability to create, control, and project fire\n"
            "using the power of the mind.  Not OCC eligible.";

    case "changeling":
        return
            "You are a Changeling: a shapeshifting humanoid who can mimic the\n"
            "appearance of any person or creature of similar size.  OCC eligible.";

    case "common faerie":
        return
            "You are a Common Faerie: a winged magical being of the faerie\n"
            "world, attuned to nature and faerie magic.  Not OCC eligible.";

    case "common pixie":
        return
            "You are a Common Pixie: a tiny winged faerie with a talent for\n"
            "mischief and invisibility to machines.  Not OCC eligible.";

    case "conservator":
        return
            "You are a Conservator: a rare humanoid eco-mage being devoted\n"
            "to protecting nature from technology and demonic corruption.  Not OCC eligible.";

    case "coyle":
        return
            "You are a Coyle: a cunning fox-like humanoid with Supernatural\n"
            "Physical Strength and Endurance and keen senses.  OCC eligible.";

    case "cs psi-stalker":
        return
            "You are a CS Psi-Stalker: a psionic mutant human employed by the\n"
            "Coalition States to track and hunt supernatural beings.  Not OCC eligible.";

    case "dwarf":
        return
            "You are a Dwarf: a stout and hardy humanoid renowned for craftsmanship\n"
            "and resilience.  OCC eligible.";

    case "elf":
        return
            "You are an Elf: a tall, agile humanoid with natural grace and\n"
            "an affinity for magic and archery.  OCC eligible.";

    case "equinoid":
        return
            "You are an Equinoid: a powerful horse-like humanoid with Supernatural\n"
            "Physical Strength and remarkable speed.  Not OCC eligible.";

    case "fire dragon":
        return
            "You are a young Fire Dragon: a massive MDC supernatural being with\n"
            "fire breath, immunity to heat, and the ability to fly.  Not OCC eligible.";

    case "frost pixie":
        return
            "You are a Frost Pixie: a tiny cold-dwelling pixie from the northern\n"
            "regions with ice-aspected faerie magic.  Not OCC eligible.";

    case "gargoyle":
        return
            "You are a Gargoyle: an MDC supernatural predator with wings, claws,\n"
            "and immense physical power.  Not OCC eligible.";

    case "goblin":
        return
            "You are a Goblin: a small cunning humanoid skilled at survival\n"
            "in hostile environments.  OCC eligible.";

    case "great horned dragon":
        return
            "You are a young Great Horned Dragon: one of the most powerful\n"
            "supernatural beings on Rifts Earth, with fire breath, acid breath,\n"
            "master psionic ability, and magical power.  Not OCC eligible.";

    case "green wood faerie":
        return
            "You are a Green Wood Faerie: a nature faerie deeply connected to\n"
            "forests and plant life, wielding nature magic.  Not OCC eligible.";

    case "gurgoyle":
        return
            "You are a Gurgoyle: an elite MDC gargoyle of greater intelligence\n"
            "and power than the common variety.  Not OCC eligible.";

    case "ice dragon":
        return
            "You are a young Ice Dragon: an MDC supernatural being with ice\n"
            "breath, cold immunity, and the ability to fly.  Not OCC eligible.";

    case "jotan":
        return
            "You are a Jotan: a Norse giant of supernatural physical power,\n"
            "dwelling in the cold northern reaches.  OCC eligible.";

    case "kankoran":
        return
            "You are a Kankoran: a fox-like humanoid with Supernatural Physical\n"
            "Strength, keen senses, and natural cunning.  OCC eligible.";

    case "mind melter":
        return
            "You are a Mind Melter: a master psionic human with a vast array\n"
            "of mental powers including telepathy, telekinesis, and empathy.\n"
            "Not OCC eligible.";

    case "night-elves faerie":
        return
            "You are a Night-Elves Faerie: a tall, dark faerie folk of the\n"
            "twilight realm with exceptional psionic and magical senses.  Not OCC eligible.";

    case "nimro fire giant":
        return
            "You are a Nimro Fire Giant: a towering supernatural giant with\n"
            "affinity for fire and enormous physical power.  OCC eligible.";

    case "ogre":
        return
            "You are an Ogre: a massive, brutish humanoid with Supernatural\n"
            "Physical Strength and thick hide.  OCC eligible.";

    case "orc":
        return
            "You are an Orc: a tough grey-skinned humanoid survivor, fierce\n"
            "in battle and resilient under hardship.  OCC eligible.";

    case "pogtal":
        return
            "You are a Pogtal: a large crested ape-like humanoid of considerable\n"
            "physical power.  Not OCC eligible.";

    case "rahu-man":
        return
            "You are a Rahu-Man: a towering supernatural humanoid of Indian\n"
            "heritage with Supernatural Physical Strength and exceptional senses.\n"
            "OCC eligible.";

    case "ratling":
        return
            "You are a Ratling: an agile rat-like humanoid with sharp instincts\n"
            "and a talent for stealth and trade.  OCC eligible.";

    case "secondary vampire":
        return
            "You are a Secondary Vampire: an MDC undead being of supernatural\n"
            "physical power, vulnerable to sunlight and wild fire.  Not OCC eligible.";

    case "silver bells faerie":
        return
            "You are a Silver Bells Faerie: a tiny musical faerie whose bell-like\n"
            "song carries magical enchantment.  Not OCC eligible.";

    case "thunder lizard dragon":
        return
            "You are a young Thunder Lizard Dragon: a massive MDC reptilian\n"
            "dragon with lightning breath and electricity immunity.  Not OCC eligible.";

    case "titan":
        return
            "You are a Titan: an enormous supernatural humanoid of great wisdom\n"
            "and physical power.  OCC eligible.";

    case "tree sprite":
        return
            "You are a Tree Sprite: a tiny nature spirit bound to the forests,\n"
            "wielding subtle nature magic.  Not OCC eligible.";

    case "troll":
        return
            "You are a Troll: a huge supernatural humanoid with Supernatural\n"
            "Physical Strength, tough hide, and rapid regeneration.  OCC eligible.";

    case "water sprite":
        return
            "You are a Water Sprite: an aquatic faerie spirit tied to rivers,\n"
            "lakes, and the sea.  Not OCC eligible.";

    case "werebear":
        return
            "You are a Werebear: an MDC lycanthrope with the power to shift\n"
            "between human and bear forms.  Vulnerable to silver.  Not OCC eligible.";

    case "weretiger":
        return
            "You are a Weretiger: an MDC lycanthrope of exceptional stealth\n"
            "and predatory grace.  Vulnerable to silver.  Not OCC eligible.";

    case "werewolf":
        return
            "You are a Werewolf: an MDC lycanthrope of supernatural speed and\n"
            "ferocity.  Vulnerable to silver.  Not OCC eligible.";

    case "wild vampire":
        return
            "You are a Wild Vampire: an MDC feral undead of enormous power,\n"
            "more intelligent and dangerous than the secondary kind.\n"
            "Vulnerable to sunlight and wild fire.  Not OCC eligible.";

    case "atlantean":
        return
            "You are a True Atlantean: a proud ancient race whose civilization\n"
            "predates the Rifts.  Atlanteans are dimensional travellers marked\n"
            "by magic tattoos that grant them power.  You possess a natural\n"
            "resistance to magic and psionics, and an innate sense of ley lines\n"
            "and dimensional rifts.  Atlanteans are OCC eligible and may choose\n"
            "from Atlantean Nomad, Atlantean Slave, or, if you have proven\n"
            "yourself in blood, the secretive Sunaj Assassin.";

    case "hawrk-duhk":
        return
            "You are a Hawrk-Duhk: a large and powerful avian D-Bee from\n"
            "the dimensional realms beyond the Splynn markets.  Your kind are\n"
            "impressive physical specimens with natural MDC bodies, the ability\n"
            "to fly, and enhanced predator vision.  You are an RCC being.\n"
            "Not OCC eligible.";

    case "hawrk-ka":
        return
            "You are a Hawrk-Ka: a sleeker, faster variant of the avian D-Bee\n"
            "known for aerial agility and sharp nightvision.  Your natural MDC\n"
            "frame and flight capability make you a formidable scout and hunter.\n"
            "Not OCC eligible.";

    case "wild psi-stalker":
        return
            "You are a Wild Psi-Stalker: a psionic predator who hunts for\n"
            "Potential Psychic Energy rather than serving the Coalition.\n"
            "You possess Supernatural Physical Strength and Endurance, all\n"
            "the enhanced senses of your Coalition-trained kin, and an\n"
            "additional psionic awareness of other psychics and supernatural\n"
            "creatures.  Not OCC eligible.";

    case "mutant rat":
        return
            "You are a Mutant Rat: a quick-witted, agile rodent humanoid born\n"
            "from Rifts-era mutation.  You are nimble, clever, and possessed of\n"
            "exceptional nightvision and an acute sense of smell.  A natural\n"
            "climber and survivor, you thrive where others fail.\n"
            "Not OCC eligible.  Racial abilities define your path.";

    case "mutant dog":
        return
            "You are a Mutant Dog: a loyal and physically powerful canine humanoid\n"
            "with exceptional tracking ability and keen hearing.  You can follow\n"
            "a scent trail across terrain and detect ambushes long before others.\n"
            "Your bite is a natural weapon and you shrug off many toxins.\n"
            "Not OCC eligible.  Racial abilities define your path.";

    case "mutant bear":
        return
            "You are a Mutant Bear: a massively powerful ursine humanoid with\n"
            "crushing claws, a powerful bite, and thick hide rivaling light armor.\n"
            "You are one of the strongest mutant animal types, though your speed\n"
            "and dexterity are reduced by your bulk.  Your presence alone may\n"
            "intimidate lesser beings.\n"
            "Not OCC eligible.  Racial abilities define your path.";

    case "mutant cat":
        return
            "You are a Mutant Cat: a sleek, agile feline humanoid with unmatched\n"
            "reflexes and predator instincts.  You move without sound, land from\n"
            "great heights without harm, and can extend razor claws at will.\n"
            "Your nightvision is extraordinary even among mutant animals.\n"
            "Not OCC eligible.  Racial abilities define your path.";

    case "mutant hawk":
        return
            "You are a Mutant Hawk: a winged avian humanoid with broad wings\n"
            "capable of true flight and telescopic vision that can resolve detail\n"
            "at over a thousand feet.  Your talons are natural weapons.\n"
            "Wearing standard armor restricts your wings and physical performance.\n"
            "Not OCC eligible.  Racial abilities define your path.";

    case "mutant wolf":
        return
            "You are a Mutant Wolf: a fast, strong lupine humanoid built for\n"
            "endurance and group combat.  Your nose can track prey across great\n"
            "distances and your howl rallies nearby allies.  When fighting\n"
            "alongside others you fight with heightened coordination.\n"
            "Not OCC eligible.  Racial abilities define your path.";

    default:
        return "No description available.";
    }
}

int player_has_magic_access(object who) {
    string occ;
    string flags;
    string race;
    mapping rdata;
    string known;

    if(!who) return 0;
    if(!(int)is_rifts_race((string)who->query_race())) return 0;
    race = query_race_string(who);
    if(race == "godling") return 1;
    rdata = query_race_data(race);
    if(rdata && (int)rdata["is_magical"]) return 1;
    flags = (string)who->getenv("rifts_occ_flags");
    occ = (string)who->getenv("rifts_occ");
    if(!stringp(occ)) occ = "";
    if(!stringp(flags)) flags = "";
    occ = lower_case(occ);
    flags = lower_case(flags);
    if(strsrch(flags, "magic") != -1) return 1;
    if(strsrch(occ, "ley line") != -1 ||
       strsrch(occ, "techno-wizard") != -1 ||
       strsrch(occ, "mystic") != -1 ||
       strsrch(occ, "shifter") != -1 ||
       strsrch(occ, "shaman") != -1 ||
       strsrch(occ, "warlock") != -1 ||
       strsrch(occ, "rifter") != -1) return 1;
    known = (string)who->getenv("known_spells");
    if(known && sizeof(known)) return 1;
    return 0;
}

int player_has_psi_access(object who) {
    string occ;
    string flags;
    string race;
    mapping rdata;
    string known;

    if(!who) return 0;
    if(!(int)is_rifts_race((string)who->query_race())) return 0;
    race = query_race_string(who);
    if(race == "godling") return 1;
    rdata = query_race_data(race);
    if(rdata && (int)rdata["is_psychic"]) return 1;
    flags = (string)who->getenv("rifts_occ_flags");
    occ = (string)who->getenv("rifts_occ");
    if(!stringp(occ)) occ = "";
    if(!stringp(flags)) flags = "";
    occ = lower_case(occ);
    flags = lower_case(flags);
    if(strsrch(flags, "psychic") != -1) return 1;
    if(strsrch(occ, "mind melter") != -1 ||
       strsrch(occ, "cyber-knight") != -1 ||
       strsrch(occ, "crazy") != -1 ||
       strsrch(occ, "psi-stalker") != -1 ||
       strsrch(occ, "burster") != -1 ||
       strsrch(occ, "zapper") != -1) return 1;
    known = (string)who->getenv("known_psionics");
    if(known && sizeof(known)) return 1;
    return 0;
}

int player_knows_spell(object who, string spell) {
    string known;
    string *list;
    string race;

    if(!who || !spell || !sizeof(spell)) return 0;
    spell = (string)RIFTS_SPELLS_D->normalize_spell_name(spell);
    race = query_race_string(who);
    if(race == "godling")
        return (mapping)RIFTS_SPELLS_D->query_spell(spell) != 0;
    known = (string)who->getenv("known_spells");
    if(!known || !sizeof(known)) return 0;
    list = explode(known, ";");
    return member_array(spell, list) != -1;
}

int player_knows_psionic(object who, string psi) {
    string known;
    string *list;
    string race;

    if(!who || !psi || !sizeof(psi)) return 0;
    psi = (string)RIFTS_PSIONICS_D->normalize_psionic_name(psi);
    race = query_race_string(who);
    if(race == "godling")
        return (mapping)RIFTS_PSIONICS_D->query_psionic(psi) != 0;
    known = (string)who->getenv("known_psionics");
    if(!known || !sizeof(known)) return 0;
    list = explode(known, ";");
    return member_array(psi, list) != -1;
}

/* Returns 1 if room carries any standard ley line environment tag. */
int room_on_ley_line(object room) {
    if(!room || !objectp(room)) return 0;
    if((int)room->query_property("ley_line")) return 1;
    if((int)room->query_property("ley_line_room")) return 1;
    if((int)room->query_property("is_ley_line")) return 1;
    if((int)room->query_property("ley_line_nexus")) return 1;
    return 0;
}

/* Returns 1 if weapon can deal MDC damage in Rifts combat. */
int is_mdc_weapon(object weapon) {
    string dmg;

    if(!weapon || !objectp(weapon)) return 0;
    if((int)weapon->query_property("mdc_weapon") == 1) return 1;
    if((int)weapon->query_property("sdc_weapon") == 1) return 0;
    dmg = (string)weapon->query_property("damage_string");
    if(!stringp(dmg) || !sizeof(dmg)) return 0;
    if(strsrch(lower_case(dmg), " md") != -1) return 1;
    return 0;
}

/* Returns 1 if armor uses the MDC pool (mdc_armor property). */
int is_mdc_armor(object armor) {
    if(!armor || !objectp(armor)) return 0;
    return (int)armor->query_property("mdc_armor") == 1;
}

/* Returns 1 if armor uses the SDC pool (sdc_armor property). */
int is_sdc_armor(object armor) {
    if(!armor || !objectp(armor)) return 0;
    return (int)armor->query_property("sdc_armor") == 1;
}

/* Notify Rifts players once when total XP reaches the next level threshold. */
void check_advance_notify(object who) {
    int level;
    int exp;
    int next_exp;
    int notified;

    if(!who || !interactive(who)) return;
    if(!(int)is_rifts_race((string)who->query_race())) return;
    if(creatorp(who)) return;
    level = (int)who->query_level();
    if(level >= 15) return;
    exp = (int)who->query_exp();
    next_exp = (int)OCC_D->query_next_level_exp(who);
    if(exp < next_exp) return;
    notified = (int)who->query_property("advance_notified_at_level");
    if(notified == level) return;
    who->set_property("advance_notified_at_level", level);
    tell_object(who,
        "You have enough experience to advance. Type 'advance' to level up.\n");
}

int is_vampire_race(object who) {
    string race;

    if(!who) return 0;
    race = query_race_string(who);
    if(!race || !sizeof(race)) return 0;
    return (strsrch(race, "vampire") != -1);
}

private int leaves_blood_pool(object victim) {
    string race;

    if(!victim) return 0;
    if((int)victim->query_property("no_blood")) return 0;
    if((int)victim->query_property("undead")) return 0;
    if((int)victim->query_ghost()) return 0;
    if(is_vampire_race(victim)) return 0;
    race = query_race_string(victim);
    if(race && strsrch(race, "construct") != -1) return 0;
    if(race && strsrch(race, "golem") != -1) return 0;
    return 1;
}

void drop_death_blood(object victim, object room) {
    object blood;

    if(!victim || !room) return;
    if(!leaves_blood_pool(victim)) return;
    blood = new("/std/obj/fresh_blood");
    if(!blood) return;
    catch(blood->set_source((string)victim->query_cap_name()));
    blood->move(room);
}

/* Safe lowercased OCC lookup. getenv/query_property return 0 for
   characters that have not picked an OCC yet (mid-chargen, wizards);
   passing that 0 to lower_case() is a runtime error. Returns "" when
   no OCC is set so strsrch/== checks in callers stay safe. */
string query_occ_string(object pl) {
    mixed occ;

    if(!pl) return "";
    occ = pl->getenv("rifts_occ");
    if(!stringp(occ) || !sizeof(occ))
        occ = pl->query_property("rifts_occ");
    if(!stringp(occ) || !sizeof(occ)) return "";
    return lower_case(occ);
}

/* Safe lowercased race lookup. query_race() returns 0 for monsters
   without set_race() and for users mid-chargen; passing that 0 to
   lower_case() is a runtime error. Returns "" when no race is set. */
string query_race_string(object who) {
    mixed race;

    if(!who) return "";
    race = who->query_race();
    if(!stringp(race) || !sizeof(race)) return "";
    return lower_case(race);
}

int query_rest_interval(object pl) {
    string occ;
    string race;

    if(!pl) return 600;
    occ = query_occ_string(pl);
    race = query_race_string(pl);
    if(occ && strsrch(occ, "juicer") != -1) return 90;
    if(race && strsrch(race, "dragon") != -1 &&
       (int)is_mdc_race(race)) return 2400;
    return 600;
}

int query_rest_mdc_gain(object pl) {
    string occ;
    int gain;

    if(!pl) return random(10) + 1;
    occ = query_occ_string(pl);
    gain = random(10) + 1;
    if(occ && strsrch(occ, "juicer") != -1)
        gain += random(6) + 1;
    return gain;
}

int query_rest_hp_gain(object pl) {
    string occ;
    int gain;

    if(!pl) return random(4) + 1;
    occ = query_occ_string(pl);
    if(occ && strsrch(occ, "juicer") != -1)
        return random(8) + 3;
    return random(4) + 1;
}

int query_rest_sdc_gain(object pl) {
    string occ;
    int gain;

    if(!pl) return random(6) + 1;
    occ = query_occ_string(pl);
    if(occ && strsrch(occ, "juicer") != -1)
        return random(10) + 4;
    return random(6) + 1;
}

int is_smuggler_occ(object pl) {
    string occ;

    if(!pl) return 0;
    occ = query_occ_string(pl);
    return (occ == "smuggler" || strsrch(occ, "smuggler") != -1);
}

int query_smuggler_exchange_rate(object pl) {
    int rate;
    int skill;
    int level;

    if(!pl || !is_smuggler_occ(pl)) return 0;
    level = (int)pl->query_level();
    skill = (int)RIFTS_SKILLS_D->query_skill_percent(pl, "black market");
    rate = 82 + level;
    if(skill > 0) rate += skill / 25;
    if(rate > 99) rate = 99;
    return rate;
}

int has_race_flag(object who, string flag) {
    string race;
    string *flags;

    if(!who || !flag || !strlen(flag)) return 0;
    if(!living(who)) return 0;
    race = query_race_string(who);
    if(!race || !strlen(race)) return 0;
    flags = query_race_flags(race);
    if(!flags || !sizeof(flags)) return 0;
    return member_array(lower_case(flag), flags) != -1;
}

string query_breath_type(object who) {
    if(has_race_flag(who, "fire_breath")) return "fire";
    if(has_race_flag(who, "ice_breath")) return "ice";
    if(has_race_flag(who, "acid_breath")) return "acid";
    if(has_race_flag(who, "lightning_breath")) return "lightning";
    return "";
}
