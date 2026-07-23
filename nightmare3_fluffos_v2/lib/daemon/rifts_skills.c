// /daemon/rifts_skills.c
// AetherMUD -- Palladium Rifts skill translation daemon.
//
// Responsibilities:
//   - Maps Palladium Rifts skill strings to Nightmare skill keys and percentages
//   - Applies an OCC's base_skills list to a player via add_skill()
//   - Provides the raw skill data table for query by other systems
//
// Design notes:
//   - Nightmare skills use integer levels (treated here as percentage 1-100).
//   - W.P. (Weapon Proficiency) skills are added as new skills; H2H types boost
//     the existing "melee" and "attack" skills that the combat engine reads.
//   - Skills already existing on the player are NOT overwritten (add_skill
//     returns 0 silently if the skill exists).
//   - Language, Literacy, and annotation-only entries are skipped gracefully.

#include <daemons.h>

// ── Skill data table ──────────────────────────────────────────────────────
//
// query_rifts_skill(key) -- returns a mapping or 0.
// Keys are normalized lowercase strings (see normalize_skill() below).
// Fields:
//   "start"    -- starting percentage / skill level
//   "max"      -- ceiling percentage (98 is Palladium's practical cap)
//   "per_level"-- improvement per level up
//   "category" -- grouping for display

mapping query_rifts_skill(string key) {
    switch(key) {

    // ── Weapon Proficiencies ──────────────────────────────────────────────
    case "wp energy pistol":
        return ([ "start": 10, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp energy rifle":
        return ([ "start": 10, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp heavy md weapons":
    case "wp heavy military weapons":
    case "wp heavy weapons":
        return ([ "start": 8,  "max": 98, "per_level": 4, "category": "weapons" ]);
    case "wp knife":
        return ([ "start": 15, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp sword":
    case "wp vibro-blade":
    case "wp vibroblade":
        return ([ "start": 10, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp blunt":
        return ([ "start": 10, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp chain":
        return ([ "start": 8,  "max": 98, "per_level": 4, "category": "weapons" ]);
    case "wp paired weapons":
        return ([ "start": 5,  "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp rifle":
    case "wp shotgun":
    case "wp rifle or shotgun":
        return ([ "start": 15, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp archery":
        return ([ "start": 15, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp two ancient":
    case "wp one ancient":
    case "wp three ancient":
        return ([ "start": 10, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp two modern":
    case "wp three modern weapons":
    case "wp three modern":
    case "wp two modern weapons":
        return ([ "start": 10, "max": 98, "per_level": 5, "category": "weapons" ]);

    // ── Pilot skills ──────────────────────────────────────────────────────
    case "pilot automobile":
        return ([ "start": 60, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot hovercraft":
        return ([ "start": 50, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot hovercycle":
        return ([ "start": 55, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot power armor":
    case "pilot power armour":
        return ([ "start": 60, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot robot":
    case "pilot robot combat elite":
    case "pilot glitter boy":
    case "pilot samas power armor":
        return ([ "start": 60, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot jet pack":
        return ([ "start": 42, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot boat":
    case "pilot boat 80%":
        return ([ "start": 56, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot submersible":
        return ([ "start": 40, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot ground vehicles":
    case "pilot_ground_vehicles":
        return ([ "start": 60, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot two vehicles of choice":
    case "pilot two vehicles":
        return ([ "start": 60, "max": 98, "per_level": 4, "category": "pilot" ]);

    // ── Communications / Technical ────────────────────────────────────────
    case "radio basic":
    case "radio: basic":
        return ([ "start": 45, "max": 98, "per_level": 5, "category": "technical" ]);
    case "computer operation":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "technical" ]);
    case "computer programming":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "technical" ]);
    case "basic electronics":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "technical" ]);
    case "advanced electronics":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "technical" ]);
    case "read sensory equipment":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "technical" ]);
    case "weapon systems":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "technical" ]);
    case "electronic countermeasures":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "technical" ]);
    case "mechanical engineer":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "technical" ]);
    case "robot mechanics":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "technical" ]);
    case "weapons engineer":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "technical" ]);

    // ── Medical ───────────────────────────────────────────────────────────
    case "first aid":
        return ([ "start": 45, "max": 98, "per_level": 5, "category": "medical" ]);
    case "paramedic":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "medical" ]);
    case "medical doctor":
        return ([ "start": 55, "max": 98, "per_level": 5, "category": "medical" ]);
    case "holistic medicine":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "medical" ]);
    case "biology":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "medical" ]);
    case "chemistry":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "technical" ]);
    case "pathology":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "medical" ]);

    // ── Survival / Wilderness ─────────────────────────────────────────────
    case "wilderness survival":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "survival" ]);
    case "land navigation":
        return ([ "start": 36, "max": 98, "per_level": 4, "category": "survival" ]);
    case "tracking":
    case "track animals":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "survival" ]);
    case "camouflage":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "survival" ]);
    case "hunting":
        return ([ "start": 0,  "max": 98, "per_level": 0, "category": "survival" ]); /* bonus skill, no % */
    case "falconry":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "survival" ]);
    case "fishing":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "survival" ]);
    case "swimming":
        return ([ "start": 50, "max": 98, "per_level": 5, "category": "physical" ]);
    case "climbing":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "physical" ]);
    case "running":
        return ([ "start": 0,  "max": 98, "per_level": 0, "category": "physical" ]); /* bonus, no % */

    // ── Espionage ─────────────────────────────────────────────────────────
    case "detect ambush":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "detect concealment":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "intelligence":
        return ([ "start": 32, "max": 98, "per_level": 4, "category": "espionage" ]);
    case "surveillance":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "pick locks":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "pick pockets":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "safe-cracking":
        return ([ "start": 20, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "forgery":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "disguise":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "impersonation":
        return ([ "start": 21, "max": 98, "per_level": 4, "category": "espionage" ]);
    case "streetwise":
        return ([ "start": 20, "max": 98, "per_level": 4, "category": "espionage" ]);
    case "gambling":
    case "gambling: basic":
        return ([ "start": 25, "max": 98, "per_level": 4, "category": "espionage" ]);
    case "concealment":
        return ([ "start": 20, "max": 98, "per_level": 4, "category": "espionage" ]);
    case "prowl":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "interrogation":
        return ([ "start": 30, "max": 98, "per_level": 4, "category": "espionage" ]);
    case "law enforcement":
        return ([ "start": 30, "max": 98, "per_level": 4, "category": "espionage" ]);
    case "cryptology":
        return ([ "start": 20, "max": 98, "per_level": 4, "category": "technical" ]);
    case "detect forgery":
        return ([ "start": 25, "max": 98, "per_level": 4, "category": "espionage" ]);
    case "wp revolver":
        return ([ "start": 15, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "boat building":
        return ([ "start": 20, "max": 98, "per_level": 4, "category": "technical" ]);
    case "rope making":
        return ([ "start": 25, "max": 98, "per_level": 4, "category": "technical" ]);
    case "vampiric lore":
        return ([ "start": 15, "max": 98, "per_level": 4, "category": "lore" ]);

    // ── Physical / Combat ─────────────────────────────────────────────────
    case "boxing":
        return ([ "start": 0,  "max": 98, "per_level": 0, "category": "physical" ]); /* bonus ability */
    case "gymnastics":
        return ([ "start": 50, "max": 98, "per_level": 5, "category": "physical" ]);
    case "acrobatics":
        return ([ "start": 50, "max": 98, "per_level": 5, "category": "physical" ]);
    case "body building":
        return ([ "start": 0,  "max": 98, "per_level": 0, "category": "physical" ]); /* attribute bonus */
    case "meditation":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "physical" ]);
    case "demolitions":
        return ([ "start": 36, "max": 98, "per_level": 4, "category": "espionage" ]);
    case "tactics":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "military" ]);
    case "military etiquette":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "military" ]);

    // ── Lore / Knowledge ──────────────────────────────────────────────────
    case "lore magic":
    case "lore: magic":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "lore" ]);
    case "lore demons and monsters":
    case "lore: demons & monsters":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "lore" ]);
    case "lore rifts and dimensions":
    case "lore: rifts & dimensions":
        return ([ "start": 20, "max": 98, "per_level": 5, "category": "lore" ]);
    case "lore psychic":
    case "lore: psychic":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "lore" ]);
    case "lore spirits":
    case "lore: spirits":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "lore" ]);
    case "research":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "technical" ]);
    case "navigation":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "pilot" ]);

    // ── General / Miscellaneous ───────────────────────────────────────────────
    case "salvage":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "technical" ]);
    case "athletics":
        return ([ "start": 0,  "max": 98, "per_level": 0, "category": "physical" ]); /* bonus ability */
    case "horsemanship":
    case "horsemanship: general":
    case "horsemanship: warhorse":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "physical" ]);
    case "seduction":
        return ([ "start": 22, "max": 98, "per_level": 3, "category": "rogue" ]);
    case "identify plants":
    case "identify plants and fruits":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "survival" ]);
    case "fly":
        return ([ "start": 50, "max": 98, "per_level": 5, "category": "physical" ]);
    case "beg":
        return ([ "start": 25, "max": 98, "per_level": 4, "category": "rogue" ]);
    case "geology":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "science" ]);
    case "parachuting":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "military" ]);
    case "sniping":
        return ([ "start": 45, "max": 98, "per_level": 5, "category": "military" ]);
    case "lore faerie":
    case "lore: faerie":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "lore" ]);
    case "detect psionics":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "psionic" ]);
    case "astronomy":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "science" ]);
    case "lore disease":
    case "lore: disease":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "lore" ]);
    case "underwater combat":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "military" ]);
    case "trap detection":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "rogue" ]);
    case "recognize gems":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "trade" ]);
    case "radio basic":
    case "radio: basic":
        return ([ "start": 40, "max": 98, "per_level": 4, "category": "technical" ]);
    case "forgery":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "pick locks":
    case "pick lock":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "computer hacking":
    case "hacking":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "technical" ]);

    // ── Electrical Skills ─────────────────────────────────────────────────────
    case "electrical engineer":
    case "electrical_engineer":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "electrical" ]);
    case "basic_electronics":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "electrical" ]);
    case "computer repair":
    case "computer_repair":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "electrical" ]);
    case "robot electronics":
    case "robot_electronics":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "electrical" ]);
    case "surveillance systems":
    case "surveillance_systems":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "electrical" ]);

    // ── Trade / Barter Skills ─────────────────────────────────────────────────
    case "barter":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "trade" ]);
    case "appraise goods":
    case "appraise_goods":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "trade" ]);
    case "black market":
    case "black_market":
        return ([ "start": 20, "max": 98, "per_level": 4, "category": "trade" ]);
    case "recognize weapon quality":
    case "recognize_weapon_quality":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "trade" ]);

    // ── Art / Creative Skills ─────────────────────────────────────────────────
    case "art":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "creative" ]);
    case "writing":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "creative" ]);
    case "singing":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "creative" ]);
    case "dancing":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "creative" ]);
    case "play instrument":
    case "play_instrument":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "creative" ]);
    case "ventriloquism":
        return ([ "start": 16, "max": 98, "per_level": 4, "category": "creative" ]);

    /* ── Weapon Proficiencies (expansion batch) ─────────────────────────── */
    case "wp automatic pistol":
    case "wp auto pistol":
        return ([ "start": 15, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp submachine gun":
    case "wp sub-machinegun":
        return ([ "start": 15, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp staff":
        return ([ "start": 10, "max": 98, "per_level": 5, "category": "weapons" ]);
    case "wp spear":
        return ([ "start": 10, "max": 98, "per_level": 5, "category": "weapons" ]);

    /* ── Pilot (expansion batch) ────────────────────────────────────────── */
    case "pilot motorcycle":
        return ([ "start": 60, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot truck":
        return ([ "start": 40, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot tanks and apcs":
    case "pilot tank":
        return ([ "start": 36, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot helicopter":
        return ([ "start": 35, "max": 98, "per_level": 4, "category": "pilot" ]);
    case "pilot airplane":
        return ([ "start": 50, "max": 98, "per_level": 4, "category": "pilot" ]);

    /* ── Mechanical / Technical (expansion batch) ───────────────────────── */
    case "automotive mechanics":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "technical" ]);
    case "aircraft mechanics":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "technical" ]);
    case "jury-rig":
    case "jury rig":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "technical" ]);
    case "general repair and maintenance":
    case "general repair":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "technical" ]);
    case "laser communications":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "technical" ]);
    case "photography":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "technical" ]);
    case "optic systems":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "electrical" ]);

    /* ── Military (expansion batch) ─────────────────────────────────────── */
    case "armorer":
    case "field armorer":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "military" ]);
    case "nbc warfare":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "military" ]);
    case "trap and mine detection":
    case "trap/mine detection":
        return ([ "start": 20, "max": 98, "per_level": 5, "category": "military" ]);
    case "forced march":
        return ([ "start": 0,  "max": 98, "per_level": 0, "category": "military" ]); /* endurance bonus, no % */

    /* ── Rogue / Espionage (expansion batch) ────────────────────────────── */
    case "locksmith":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "espionage" ]);
    case "escape artist":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "rogue" ]);
    case "palming":
        return ([ "start": 20, "max": 98, "per_level": 5, "category": "rogue" ]);
    case "cardsharp":
        return ([ "start": 24, "max": 98, "per_level": 4, "category": "rogue" ]);
    case "find contraband":
        return ([ "start": 26, "max": 98, "per_level": 4, "category": "espionage" ]);

    /* ── Physical (expansion batch) ─────────────────────────────────────── */
    case "scuba":
    case "swim: deep sea":
        return ([ "start": 50, "max": 98, "per_level": 5, "category": "physical" ]);
    case "wrestling":
        return ([ "start": 0,  "max": 98, "per_level": 0, "category": "physical" ]); /* bonus ability */

    /* ── Science (expansion batch) ──────────────────────────────────────── */
    case "botany":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "science" ]);
    case "anthropology":
        return ([ "start": 20, "max": 98, "per_level": 5, "category": "science" ]);
    case "archaeology":
        return ([ "start": 20, "max": 98, "per_level": 5, "category": "science" ]);

    /* ── Domestic (expansion batch) ─────────────────────────────────────── */
    case "cook":
    case "cooking":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "domestic" ]);
    case "sew":
    case "sewing":
        return ([ "start": 40, "max": 98, "per_level": 5, "category": "domestic" ]);
    case "preserve food":
        return ([ "start": 25, "max": 98, "per_level": 5, "category": "domestic" ]);

    /* ── Wilderness (expansion batch) ───────────────────────────────────── */
    case "skin and prepare animal hides":
    case "skin animals":
        return ([ "start": 30, "max": 98, "per_level": 5, "category": "survival" ]);
    case "spelunking":
        return ([ "start": 35, "max": 98, "per_level": 5, "category": "survival" ]);

    default:
        return 0;
    }
}

// ── Skill normalization ────────────────────────────────────────────────────
//
// Converts a raw Palladium skill string into a normalized lookup key.
// Examples:
//   "W.P. Energy Rifle"       -> "wp energy rifle"
//   "Pilot Automobile 80%"    -> "pilot automobile"
//   "Hand-to-Hand: Commando"  -> (H2H -- handled separately, returns "")
//   "Language: Native 98%"    -> (skip -- returns "")
//   "First Aid"               -> "first aid"
//   "Lore: Magic"             -> "lore magic"

string normalize_skill(string raw) {
    string s;
    int i;

    if(!raw || !sizeof(raw)) return "";
    s = lower_case(raw);

    // Strip trailing percentage (e.g. " 60%", " 80%+", " 70%+")
    // Walk back from end, removing digits, %, +
    i = sizeof(s) - 1;
    while(i >= 0 && (s[i] == '+' || s[i] == '%' ||
          (s[i] >= '0' && s[i] <= '9'))) i--;
    if(i >= 0 && s[i] == ' ') s = s[0..i-1];

    // Skip flavor/annotation entries (not trackable skills)
    if(s[0..8] == "language:" || s[0..8] == "language ")
        return "";
    if(s[0..8] == "literacy:")
        return "";
    if(s[0..7] == "literacy")
        return "";
    if(s == "basic math" || s == "math: basic" || s == "math: advanced")
        return "";
    if(sizeof(s) > 3 && s[0..2] == "two") return ""; /* "Two additional languages..." */
    if(sizeof(s) > 11 && s[0..11] == "three additi") return "";
    if(sizeof(s) > 8 && s[0..8] == "history: ") return "";
    if(s == "literacy: all known languages") return "";

    // W.P. prefix: "w.p. energy rifle" -> "wp energy rifle"
    if(sizeof(s) > 4 && s[0..3] == "w.p.")
        s = "wp" + s[4..];

    // "Lore: X" -> "lore X"
    if(sizeof(s) > 6 && s[0..5] == "lore: ")
        s = "lore " + s[6..];

    // "Pilot: X" -> "pilot X"
    if(sizeof(s) > 7 && s[0..6] == "pilot: ")
        s = "pilot " + s[7..];

    // H2H -- signal to caller to handle as melee/attack boost, not a skill
    if(sizeof(s) > 13 && s[0..12] == "hand-to-hand:")
        return "";

    return s;
}

// ── H2H melee/attack boosts ────────────────────────────────────────────────
//
// Returns melee and attack bonus amounts for a given H2H type.
// The caller applies these via add_skill_bonus() on the player.
// Returns ({ melee_bonus, attack_bonus }).

int *query_hth_bonus(string skill_raw) {
    string s;
    if(!skill_raw || skill_raw == "") return ({ 0, 0 });
    s = lower_case(skill_raw);
    if(strsrch(s, "commando") != -1) return ({ 8, 8 });
    if(strsrch(s, "martial arts") != -1) return ({ 7, 7 });
    if(strsrch(s, "expert") != -1) return ({ 5, 5 });
    if(strsrch(s, "basic") != -1) return ({ 2, 2 });
    return ({ 0, 0 });
}

// ── Apply OCC skills to a player ──────────────────────────────────────────
//
// Iterates base_skills from an OCC data mapping, normalizes each entry,
// and adds it to the player via add_skill() where a match is found.
// H2H entries boost the existing melee/attack skills.
// Returns the count of skills successfully added.

int apply_occ_skills(object player, string *skill_list) {
    int i, count;
    string raw, key;
    mapping sdata;
    int *hth;

    if(!player || !skill_list) return 0;
    count = 0;
    for(i = 0; i < sizeof(skill_list); i++) {
        raw = skill_list[i];
        if(!raw || !sizeof(raw)) continue;

        // H2H: detect and apply as melee/attack bonus
        if(strsrch(lower_case(raw), "hand-to-hand") != -1) {
            hth = query_hth_bonus(raw);
            if(hth[0] > 0) {
                player->add_skill_bonus("melee",  hth[0]);
                player->add_skill_bonus("attack", hth[1]);
                count++;
            }
            continue;
        }

        key = normalize_skill(raw);
        if(!key || !sizeof(key)) continue;

        sdata = query_rifts_skill(key);
        if(!sdata) continue; /* not in our table -- skip for now */

        // add_skill returns 0 if skill already exists (no overwrite)
        if((int)player->add_skill(key, sdata["start"], sdata["max"]))
            count++;
    }
    return count;
}

// query_skill_categories -- returns array of category names for display.
string *query_skill_categories() {
    return ({ "weapons", "pilot", "technical", "medical",
              "survival", "physical", "espionage", "military", "lore",
              "rogue", "electrical", "trade", "creative", "science",
              "psionic", "domestic" });
}

// ── RP Wizard skill management API ────────────────────────────────────────

// Grant a skill to a player by name.  start_override = 0 uses table default.
int grant_skill(object player, string skill_name, int start_override) {
    string key;
    mapping sdata;
    int start;

    if(!player || !skill_name || !sizeof(skill_name)) return 0;
    key   = normalize_skill(skill_name);
    if(!key || !sizeof(key)) key = lower_case(skill_name);
    sdata = query_rifts_skill(key);
    start = (start_override > 0) ? start_override
          : (sdata ? (int)sdata["start"] : 30);
    return (int)player->add_skill(key, start, 98);
}

// Remove a skill from a player by setting it to 0.
int remove_skill(object player, string skill_name) {
    string key;
    if(!player || !skill_name) return 0;
    key = normalize_skill(skill_name);
    if(!key || !sizeof(key)) key = lower_case(skill_name);
    player->set_skill(key, 0);
    return 1;
}

// Set the percentage/level for a specific skill.
int set_skill_pct(object player, string skill_name, int pct) {
    string key;
    if(!player || !skill_name) return 0;
    key = normalize_skill(skill_name);
    if(!key || !sizeof(key)) key = lower_case(skill_name);
    if(pct < 1)  pct = 1;
    if(pct > 98) pct = 98;
    player->set_skill(key, pct);
    return 1;
}

// Improve a skill by one slot (per_level amount from the skill table).
// Returns new percentage on success, 0 on failure.
int improve_skill(object player, string skill_name) {
    string key;
    mapping sdata;
    int cur;
    int gain;
    int max_pct;
    int new_pct;

    if(!player || !skill_name || !sizeof(skill_name)) return 0;
    key = normalize_skill(skill_name);
    if(!key || !sizeof(key)) key = lower_case(skill_name);
    cur = (int)player->query_skill(key);
    if(cur <= 0) return 0;
    sdata = query_rifts_skill(key);
    if(!sdata) return 0;
    gain = (int)sdata["per_level"];
    if(gain <= 0) return 0;
    max_pct = (int)sdata["max"];
    if(max_pct <= 0) max_pct = 98;
    new_pct = cur + gain;
    if(new_pct > max_pct) new_pct = max_pct;
    if(new_pct == cur) return 0;
    player->set_skill(key, new_pct);
    return new_pct;
}

// Query current percentage for a skill.
int query_skill_pct(object player, string skill_name) {
    string key;
    if(!player || !skill_name) return 0;
    key = normalize_skill(skill_name);
    if(!key || !sizeof(key)) key = lower_case(skill_name);
    return (int)player->query_skill(key);
}

// Return array of all skill names the player currently has.
string *query_player_skills(object player) {
    if(!player) return ({});
    return (string *)player->query_all_skills();
}

// Return all known skill names in a given category.
string *query_skills_by_category(string cat) {
    string *all_skills;
    string *result;
    mapping sdata;
    int i;

    all_skills = ({
        "wp energy pistol", "wp energy rifle", "wp heavy weapons",
        "wp knife", "wp sword", "wp vibroblade", "wp blunt",
        "wp rifle", "wp archery",
        "pilot automobile", "pilot hovercraft", "pilot hovercycle",
        "pilot power armor", "pilot robot", "pilot glitter boy",
        "pilot jet pack", "pilot boat", "pilot submersible",
        "radio basic", "computer operation", "computer programming",
        "basic electronics", "advanced electronics", "read sensory equipment",
        "weapon systems", "electronic countermeasures",
        "mechanical engineer", "robot mechanics", "weapons engineer",
        "first aid", "paramedic", "medical doctor", "pathology", "biology",
        "wilderness survival", "land navigation", "tracking", "track animals",
        "camouflage", "hunting", "fishing", "swimming", "climbing", "running",
        "detect ambush", "detect concealment", "intelligence", "surveillance",
        "pick locks", "pick pockets", "forgery", "disguise",
        "impersonation", "streetwise", "gambling",
        "prowl", "interrogation", "demolitions",
        "boxing", "gymnastics", "acrobatics", "meditation",
        "tactics", "military etiquette",
        "lore magic", "lore rifts and dimensions", "lore psychic",
        "lore demons and monsters", "research", "navigation",
        "salvage", "concealment", "beg", "athletics", "horsemanship",
        "seduction", "identify plants", "fly",
        "electrical engineer", "computer repair", "robot electronics",
        "surveillance systems", "basic_electronics",
        "barter", "appraise goods", "black market", "recognize weapon quality",
        "art", "writing", "singing", "dancing", "play instrument", "ventriloquism",
        "geology", "parachuting", "sniping", "lore faerie", "detect psionics",
        "astronomy", "lore disease", "underwater combat",
        "trap detection", "recognize gems", "radio basic",
        "forgery", "pick locks", "computer hacking",
        "law enforcement", "cryptology", "detect forgery", "wp revolver",
        "boat building", "rope making", "vampiric lore",
        "wp automatic pistol", "wp submachine gun", "wp staff", "wp spear",
        "pilot motorcycle", "pilot truck", "pilot tanks and apcs",
        "pilot helicopter", "pilot airplane",
        "automotive mechanics", "aircraft mechanics", "jury-rig",
        "general repair and maintenance", "laser communications",
        "photography", "optic systems",
        "armorer", "nbc warfare", "trap and mine detection", "forced march",
        "locksmith", "escape artist", "palming", "cardsharp",
        "find contraband", "scuba", "wrestling",
        "botany", "anthropology", "archaeology",
        "cook", "sew", "preserve food",
        "skin and prepare animal hides", "spelunking",

        /* Array-drift fix (2026-07-23): these 12 have a valid switch
           case above but were never added here, making them invisible
           to query_skills_by_category() -- and, via daemon/help.c's
           is_skills_topic(), to the "help skills" category listing --
           even though query_rifts_skill() resolves them fine directly.
           One representative alias per skill, matching this array's
           existing convention (see "wp vibroblade" above, which is
           likewise the sole array entry for a 3-alias switch block). */
        "wp chain", "wp paired weapons", "wp two ancient", "wp two modern",
        "pilot ground vehicles", "pilot two vehicles of choice",
        "holistic medicine", "chemistry", "falconry", "safe-cracking",
        "body building", "lore spirits"
    });
    result = ({});
    cat    = lower_case(cat);
    i      = sizeof(all_skills);
    while(i--) {
        string skill_cat;

        sdata = query_rifts_skill(all_skills[i]);
        if(!sdata) continue;
        skill_cat = lower_case((string)sdata["category"]);
        if(skill_cat == cat) {
            result += ({ all_skills[i] });
            continue;
        }
        /* Chargen (setter.c map_occ_pick_category / normalize_skill_category)
           folds "science" OCC picks into "technical" and "rogue" OCC picks
           into "espionage" so players compare against one category name.
           Mirror that alias here too, or science/rogue-tagged skills (e.g.
           archaeology, astronomy, escape artist) never show up to browse. */
        if(cat == "technical" && skill_cat == "science") result += ({ all_skills[i] });
        else if(cat == "espionage" && skill_cat == "rogue") result += ({ all_skills[i] });
    }
    return result;
}
