// /daemon/occ.c
// RiftsMUD  -  Occupational Character Class daemon
// Stores OCC data, XP tables, skill lists, and attribute requirements.
// Called by setter.c during character creation and by score.c for display.

#include <daemons.h>

private string *occ_list;
private string *occ_eligible_races;

void create() {
    occ_list = ({
        // Original 12
        "vagabond", "wilderness scout", "city rat", "rogue scholar",
        "headhunter", "bounty hunter", "cs grunt", "cs ranger",
        "juicer", "body fixer", "cyber-knight", "ley line walker",
        // Aliases for common task-list names
        "mind melter", "mercenary", "coalition soldier",
        // Session 2 additions
        "mystic", "shifter", "shaman", "crazy",
        "cs dead boy", "borg", "robot pilot", "operator",
        "rogue scientist", "techno-wizard", "merc soldier",
        "tribal warrior", "ley line rifter",
        // Coalition / ISS
        "cs military specialist", "cs samas rpa pilot", "cs technical officer",
        "iss peacekeeper", "iss specter", "ntset protector",
        // Combat / Merc
        "special forces (merc)", "master assassin", "glitter boy pilot",
        "ninja juicer",
        // Rogue / Criminal
        "forger", "freelance spy", "professional thief", "smuggler",
        // Regional
        "knight (europe)", "royal knight", "pirate (s.a.)", "sailor (s.a.)",
        // Rare / Special
        "gifted gypsy", "maxi-man", "cyber-doc",
        // Help-file extras
        "delphi juicer", "hyperion juicer", "tattooed man",
        "air warlock", "nega-psychic",
        // Atlantean OCCs
        "atlantean nomad", "atlantean slave", "sunaj assassin",
        "tattoo warrior",
        // Additional OCCs
        "necromancer",
        // NGR OCCs
        "ngr soldier", "ngr mechanic",
        // Kittani OCCs (Kittani race only)
        "kittani warrior", "kittani field mechanic",
        // Western / specialty
        "gunfighter", "palmer", "palmer2"
    });

    occ_eligible_races = ({
        "algor frost giant", "atlantean", "bearman", "changeling", "coyle",
        "dwarf", "elf", "goblin", "human", "jotan", "kankoran",
        "kittani", "nimro fire giant", "ogre", "orc", "rahu-man", "ratling",
        "titan", "troll"
    });
}

int query_occ_eligible(string race) {
    if(!occ_eligible_races || !race || race == "") return 0;
    return member_array(lower_case(race), occ_eligible_races) != -1;
}

string *query_occs() {
    if(!occ_list) return ({});
    return occ_list + ({});
}

int query_occ_available(string occ_name, object player) {
    string race;
    int kills, faction;

    if(!occ_name || !player) return 0;
    race = lower_case((string)player->query_race());

    switch(lower_case(occ_name)) {
    case "atlantean nomad":
    case "atlantean slave":
        return (race == "atlantean");

    case "sunaj assassin":
        if(race != "atlantean") return 0;
        kills   = (int)player->getenv("rifts_pvp_kills");
        faction = (int)player->getenv("atlantean_faction");
        return (kills >= 10 && faction >= 200);

    case "tattoo warrior":
        return (race == "atlantean");

    case "kittani warrior":
        return (race == "kittani");

    case "kittani field mechanic":
        return (race == "kittani" || race == "atlantean" || race == "human");

    default:
        return 1;
    }
}

/* Forward declarations required because query_next_level_exp calls
   these before they are defined later in the file. */
private int *xp_civilian();
mapping query_occ(string occ_name);

/* Total XP required to advance from the player's current level to the next. */
int query_next_level_exp(object player) {
    string occ;
    int level;
    mapping occ_data;
    int *xp_table;
    int *default_xp;

    if(!player) return 999999;
    level = (int)player->query_level();
    if(level >= 15) return 999999;
    occ = (string)player->getenv("rifts_occ");
    occ_data = 0;
    if(occ && occ != "none" && occ != "")
        occ_data = query_occ(occ);
    default_xp = xp_civilian();
    if(occ_data && occ_data["xp_table"]) {
        xp_table = (int *)occ_data["xp_table"];
        return (level < sizeof(xp_table)) ? xp_table[level] : 999999;
    }
    return (level < sizeof(default_xp)) ? default_xp[level] : 999999;
}

// ── XP Tables ──────────────────────────────────────────────────────────────
// All tables are approximate per Palladium Rifts RPG.
// Index 0 = level 1 = 0 XP needed.  15 entries covers levels 1-15.

// Table A: Civilian/generalist (Vagabond, City Rat, CS Grunt, Juicer, Crazy)
private int *xp_civilian() { /* approx */
    return ({ 0, 2000, 4000, 8000, 16000, 24000, 48000, 80000,
              120000, 160000, 230000, 330000, 430000, 530000, 630000 });
}

// Table B: Scout/outdoorsman (Wilderness Scout, Tribal Warrior)
private int *xp_scout() { /* approx */
    return ({ 0, 1800, 3600, 7200, 14400, 21600, 40000, 70000,
              100000, 140000, 200000, 280000, 370000, 460000, 560000 });
}

// Table C: Scholar/academic (Rogue Scholar, Body Fixer, Operator, Rogue Scientist)
private int *xp_scholar() { /* approx */
    return ({ 0, 2500, 5000, 10000, 20000, 30000, 60000, 100000,
              150000, 210000, 300000, 410000, 530000, 660000, 800000 });
}

// Table D: Soldier/mercenary (Headhunter, Bounty Hunter, CS Ranger, CS Dead Boy, Merc Soldier, Robot Pilot)
private int *xp_soldier() { /* approx */
    return ({ 0, 2200, 4400, 8800, 18000, 27000, 54000, 90000,
              130000, 180000, 260000, 360000, 470000, 590000, 720000 });
}

// Table E: Cyber-Knight
private int *xp_cyber_knight() { /* approx */
    return ({ 0, 2800, 5600, 11200, 22000, 33000, 66000, 100000,
              150000, 200000, 280000, 390000, 500000, 620000, 750000 });
}

// Table F: Magic user  -  moderate (Ley Line Walker, Mystic, Shaman, Techno-Wizard)
private int *xp_ley_line() { /* approx */
    return ({ 0, 2200, 4400, 8800, 17600, 27000, 54000, 90000,
              130000, 180000, 260000, 360000, 480000, 600000, 730000 });
}

// Table G: Advanced magic (Shifter, Ley Line Rifter)  -  demanding table
private int *xp_magic_adv() { /* approx */
    return ({ 0, 3000, 6000, 12000, 24000, 36000, 72000, 110000,
              160000, 220000, 310000, 420000, 540000, 670000, 810000 });
}

// Table H: Full conversion cyborg (Borg)  -  powerful, slower advancement
private int *xp_borg() { /* approx */
    return ({ 0, 3000, 6000, 12000, 24000, 36000, 72000, 120000,
              180000, 250000, 350000, 470000, 600000, 740000, 880000 });
}

// ── query_occ ──────────────────────────────────────────────────────────────
//
// Returns a mapping with all OCC data, or 0 if not found.
//
// Fields:
//   description         -  one-line flavour string
//   xp_table            -  int array of 15 XP thresholds
//   hp_per_level        -  dice string added each level up
//   base_skills         -  skills granted automatically
//   occ_skills          -  skill categories player picks from
//   occ_skill_picks     -  how many occ_skills to choose
//   secondary_skills    -  how many secondary skills to choose
//   attribute_requirements  -  mapping of min attribute values (empty if none)
//   starting_equipment  -  descriptive placeholder
//   flags               -  string array: "magic", "psychic", "borg",
//                        "cybernetic", "juicer", "techno_wizard"
//   ppe_base            -  starting PPE (excludes ME bonus; 0 if not a mage)
//   ppe_per_level       -  PPE gained per level (0 if not a mage)
//   isp_base            -  starting ISP (excludes ME bonus; 0 if not psychic)
//   isp_per_level       -  ISP gained per level (0 if not psychic)

mapping query_occ(string occ_name) {
    if(!occ_name || occ_name == "") return 0;
    switch(lower_case(occ_name)) {

    // ── Original 12 OCCs ───────────────────────────────────────────────────

    case "vagabond":
        return ([
            "description":
                "Wanderer and survivor of the Rifts world, living by wit and adaptability.",
            "xp_table": xp_civilian(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 60%",
                "Basic Math", "Cook", "Dance or Play Musical Instrument",
                "Lore: One of Choice", "Wilderness Survival"
            }),
            "occ_skills": ({
                "Physical", "Rogue", "Wilderness", "Technical"
            }),
            "occ_skill_picks": 8,
            "secondary_skills": 8,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Rough travelling clothes, backpack, survival knife, canteen, "
                "one week of rations, and one ranged weapon of choice.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "wilderness scout":
        return ([
            "description":
                "Expert tracker, hunter, and outdoor survivalist beyond the city walls.",
            "xp_table": xp_scout(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy: Native 60%",
                "Pilot Automobile 50%", "Basic Math", "Biology", "Botany",
                "First Aid", "Fishing", "Falconry", "Hunting",
                "Identify Plants and Fruits", "Land Navigation",
                "Preserve Food", "Skin and Prepare Animals",
                "Track Animals", "Wilderness Survival",
                "W.P. Knife", "W.P. Rifle or Shotgun"
            }),
            "occ_skills": ({
                "Physical", "Rogue", "Wilderness"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 5,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Hunting rifle or shotgun, hunting knife, survival backpack, "
                "camouflage clothing, compass, fire-starting kit, and one week of rations.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "city rat":
        return ([
            "description":
                "Street-smart urban survivor skilled in theft, deception, and city life.",
            "xp_table": xp_civilian(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy: Native 60%",
                "Pilot Automobile 60%", "Basic Math",
                "Streetwise", "Gambling: Basic", "Prowl",
                "Pick Locks", "Find Contraband", "Concealment"
            }),
            "occ_skills": ({
                "Rogue", "Technical", "Physical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 6,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Street clothes, lock pick set, laser pistol or revolver, "
                "and a pocket full of contacts and ill-gotten credits.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "rogue scholar":
        return ([
            "description":
                "Self-taught intellectual dedicated to preserving pre-Rifts knowledge.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%",
                "Three additional languages (each 70%+)",
                "Literacy: All known languages",
                "Computer Operation", "Math: Advanced",
                "Research", "Lore: Two of Choice",
                "History: Pre-Rifts"
            }),
            "occ_skills": ({
                "Technical", "Science", "Medical", "Lore"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Laptop computer, collection of data discs, concealed sidearm, "
                "scholarly robes or traveller's clothes, and a journal.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "headhunter":
        return ([
            "description":
                "Elite mercenary soldier for hire, combat-hardened and heavily armed.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 80%",
                "Basic Math", "Demolitions", "First Aid",
                "Military Etiquette", "Pilot: Two Vehicles of Choice",
                "Radio: Basic", "W.P. One Ancient",
                "W.P. Three Modern Weapons"
            }),
            "occ_skills": ({
                "Espionage", "Military", "Physical", "Pilot", "Technical", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "PS": 10, "PE": 10 ]), /* approx */
            "starting_equipment":
                "Body armor, two modern ranged weapons, one melee weapon, "
                "combat knife, radio communicator, and mercenary ID papers.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "bounty hunter":
        return ([
            "description":
                "Pursuer of criminals and fugitives, blending combat skill with detection.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 80%",
                "Basic Math", "Detect Ambush", "Disguise",
                "First Aid", "Intelligence", "Interrogation",
                "Law: General", "Radio: Basic",
                "Tracking", "W.P. Two Ancient", "W.P. Two Modern"
            }),
            "occ_skills": ({
                "Espionage", "Military", "Rogue", "Technical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "PS": 9, "PE": 9 ]), /* approx */
            "starting_equipment":
                "Body armor, handcuffs or restraints, laser pistol, "
                "rifle, tracking equipment, and a current bounty list.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "gunfighter":
        return ([
            "description":
                "Fast-draw specialist and pistolero of the post-apocalyptic frontier.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 80%",
                "Basic Math", "Detect Ambush", "First Aid",
                "Land Navigation", "Radio: Basic",
                "W.P. Energy Pistol", "W.P. Revolver",
                "Hand-to-Hand: Expert"
            }),
            "occ_skills": ({
                "Physical", "Rogue", "W.P.", "Pilot"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 6,
            "attribute_requirements": ([ "PP": 12, "PE": 10 ]),
            "starting_equipment":
                "Twin C-18 laser pistols, vibro-knife, leather jacket, and trail gear.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "palmer":
        return ([
            "description":
                "Psychic investigator and wandering mystic-sleuth of the Rifts.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy: Native 98%",
                "Basic Math", "Detect Concealment", "Disguise",
                "First Aid", "Interrogation", "Land Navigation",
                "Lore: Psychic", "Radio: Basic", "Research",
                "Streetwise", "Tracking"
            }),
            "occ_skills": ({
                "Espionage", "Lore", "Physical", "Rogue", "Technical"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 10, "ME": 12, "MA": 9 ]),
            "starting_equipment":
                "Light armor, sidearm, notebook, and psychic focus items.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 20,
            "isp_per_level": 6
        ]);

    case "palmer2":
        return ([
            "description":
                "Master psychic investigator. An advanced Palmer who combines "
                "deep intuition, remote viewing, and field combat training.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy: Native 98%",
                "Basic Math", "Detect Concealment", "Disguise",
                "First Aid", "Intelligence", "Interrogation",
                "Land Navigation", "Lore: Psychic", "Radio: Basic",
                "Research", "Streetwise", "Tracking",
                "W.P. Energy Pistol", "Hand-to-Hand: Combat 3"
            }),
            "occ_skills": ({
                "Espionage", "Lore", "Physical", "Rogue", "Technical", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 12, "ME": 14, "MA": 10 ]),
            "starting_equipment":
                "Sidearm, field journal, light armor, psychic focus kit, "
                "and investigator credentials.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 35,
            "isp_per_level": 8
        ]);

    case "cs grunt":
        return ([
            "description":
                "Frontline soldier of the Coalition States, disciplined and loyal to humanity.",
            "xp_table": xp_civilian(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: American 98%", "Literacy: American 98%",
                "Pilot Automobile 80%", "Basic Math", "First Aid",
                "Military Etiquette", "Radio: Basic",
                "Recognize Weapon Quality", "Weapon Systems",
                "W.P. Energy Pistol", "W.P. Energy Rifle",
                "W.P. Heavy Military Weapons", "Hand-to-Hand: Basic"
            }),
            "occ_skills": ({
                "Military", "Physical", "Pilot", "Technical", "W.P."
            }),
            "occ_skill_picks": 4,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 6, "PS": 9, "PE": 9 ]), /* approx */
            "starting_equipment":
                "Coalition Dead Boy armor, C-20 laser pistol, C-12 heavy assault rifle, "
                "four grenades, and Coalition standard gear package.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "cs ranger":
        return ([
            "description":
                "Elite Coalition scout and survivalist operating deep behind enemy lines.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: American 98%", "Literacy: American 98%",
                "Pilot Automobile 80%", "Basic Math", "First Aid",
                "Hunting", "Land Navigation", "Military Etiquette",
                "Radio: Basic", "Wilderness Survival",
                "W.P. Energy Pistol", "W.P. Energy Rifle",
                "W.P. One Ancient", "Hand-to-Hand: Basic"
            }),
            "occ_skills": ({
                "Espionage", "Military", "Physical", "Pilot", "Technical", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 8, "PS": 10, "PE": 10 ]), /* approx */
            "starting_equipment":
                "Light Coalition scout armor, energy pistol, sniper rifle, "
                "survival backpack, camouflage field uniform, and Coalition ID.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "juicer":
        return ([
            "description":
                "Human enhanced by drug-delivery harnesses, granting incredible speed "
                "and strength at the cost of a shortened lifespan.",
            "xp_table": xp_civilian(),
            "hp_per_level": "2d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 80%",
                "Basic Math", "Boxing", "Running", "Gymnastics",
                "W.P. Two Ancient", "W.P. Two Modern",
                "Hand-to-Hand: Martial Arts"
            }),
            "occ_skills": ({
                "Espionage", "Physical", "Pilot", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "PS": 9, "PE": 9 ]), /* approx */
            "starting_equipment":
                "Juicer drug harness (fitted), light body armor, two ranged weapons, "
                "vibro-knife, and one year of drug supply.",
            "flags": ({ "juicer" }),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "body fixer":
        return ([
            "description":
                "Field medic and surgeon keeping warriors alive across the Rifts wastelands.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 90%",
                "Biology", "Chemistry", "Chemistry: Analytical",
                "Pathology", "Medical Doctor", "First Aid",
                "Pharmaceutical Chemistry", "Computer Operation",
                "Math: Advanced"
            }),
            "occ_skills": ({
                "Medical", "Science", "Technical", "Physical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 12, "ME": 8 ]), /* approx */
            "starting_equipment":
                "Surgical kit, portable medical scanner, drug and bandage kit, "
                "concealed sidearm, medical ID credentials, and doctor's bag.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "cyber-knight":
        return ([
            "description":
                "Noble warrior with psionic abilities and an innate psi-sword of "
                "pure mental energy, sworn to protect the innocent.",
            "xp_table": xp_cyber_knight(),
            "hp_per_level": "1d8",
            "base_skills": ({
                "Language: Native 98%", "Literacy 80%",
                "Horsemanship: General", "Body Building", "Running",
                "Meditation", "Lore: One of Choice",
                "W.P. One Ancient", "W.P. Two Modern",
                "Hand-to-Hand: Commando"
            }),
            "occ_skills": ({
                "Espionage", "Military", "Physical", "Technical", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 5,
            "attribute_requirements": ([ "PS": 12, "PE": 12, "MA": 10, "ME": 12 ]),
            "starting_equipment":
                "Light armor, horse or riding animal, one ranged weapon, "
                "and an innate psi-sword (no physical form, manifested psionically).",
            "flags": ({ "psychic" }),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 10, /* approx  --  minor psionic */
            "isp_per_level": 4
        ]);

    case "ley line walker":
        return ([
            "description":
                "Sorcerer attuned to the magic ley lines that crisscross Rifts Earth, "
                "channelling their power into spells and rifting.",
            "xp_table": xp_ley_line(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 80%",
                "Math: Basic", "Lore: Magic", "Lore: Two of Choice",
                "Holistic Medicine",
                "Two additional languages (each 70%+)",
                "Literacy: All known languages"
            }),
            "occ_skills": ({
                "Technical", "Science", "Lore", "Physical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 12, "ME": 14 ]),
            "starting_equipment":
                "Traveller's robes or armored jacket, spellbook, arcane focus crystal, "
                "concealed sidearm, and starting PPE pool.",
            "flags": ({ "magic" }),
            "ppe_base": 70, /* approx  --  ME bonus applied in setter.c */
            "ppe_per_level": 10,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    // ── Alias OCCs ────────────────────────────────────────────────────────────

    case "mercenary":
    case "merc soldier":
        return ([
            "description":
                "A professional soldier for hire with solid combat and survival training, "
                "adaptable and loyal to whoever is paying.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 75%",
                "Basic Math", "First Aid", "Military Etiquette",
                "Radio: Basic", "W.P. Two Ancient",
                "W.P. Three Modern", "Hand-to-Hand: Basic"
            }),
            "occ_skills": ({
                "Military", "Physical", "Espionage", "Pilot", "Technical", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 6,
            "attribute_requirements": ([ "PS": 9, "PE": 9 ]), /* approx */
            "starting_equipment":
                "Body armor, two ranged weapons, combat knife, radio, "
                "and mercenary contracts or unit affiliation.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "coalition soldier":
    case "cs dead boy":
        return ([
            "description":
                "Elite Coalition infantry wearing the iconic skull-faced Dead Boy armor, "
                "the professional backbone of the Coalition war machine.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: American 98%", "Literacy: American 98%",
                "Pilot Automobile 90%", "Basic Math",
                "First Aid", "Military Etiquette", "Radio: Basic",
                "Detect Ambush", "Tactics",
                "W.P. Energy Pistol", "W.P. Energy Rifle",
                "W.P. Heavy Military Weapons", "Hand-to-Hand: Expert"
            }),
            "occ_skills": ({
                "Military", "Physical", "Pilot", "Technical", "W.P.", "Espionage"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 8, "PS": 12, "PE": 12 ]), /* approx */
            "starting_equipment":
                "Coalition Dead Boy body armor (full), C-20 laser pistol, "
                "C-12 heavy assault rifle, four grenades, radio headset, "
                "and Coalition standard field kit.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "mind melter":
        return ([
            "description":
                "A powerful psychic mutant with master-level psionic abilities. "
                "Mind Melters are among the most feared and revered beings on Rifts Earth.",
            "xp_table": xp_ley_line(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "One extra language 50%",
                "Pilot Automobile 60%", "Math: Basic",
                "Athletics", "Meditation", "Detect Psionics"
            }),
            "occ_skills": ({
                "Physical", "Technical"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "ME": 14 ]),
            "starting_equipment":
                "Traveling clothes, no weapon (psionics are the weapon), "
                "and a deep knowledge of one's own mental power.",
            "flags": ({ "psychic" }),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 100, /* approx -- ME bonus applied in setter.c */
            "isp_per_level": 10
        ]);

    // ── New OCCs ───────────────────────────────────────────────────────────

    case "mystic":
        return ([
            "description":
                "A rare combination of magic-user and psychic, drawing on both PPE "
                "and ISP to sense threats, cast spells, and commune with spirits.",
            "xp_table": xp_ley_line(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 80%",
                "Math: Basic", "Lore: Magic", "Lore: Psychic",
                "Holistic Medicine", "Meditation", "See Aura",
                "See the Invisible", "Sense Evil"
            }),
            "occ_skills": ({
                "Technical", "Science", "Lore", "Physical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 10, "ME": 12 ]), /* approx */
            "starting_equipment":
                "Traveller's clothes, a focus crystal, journal of visions, "
                "and one concealed ranged weapon.",
            "flags": ({ "magic", "psychic" }),
            "ppe_base": 70, /* approx  --  ME bonus applied separately */
            "ppe_per_level": 8,
            "isp_base": 14, /* approx  --  ME bonus applied separately */
            "isp_per_level": 6
        ]);

    case "shifter":
        return ([
            "description":
                "A dangerous and powerful mage who opens Rifts to summon beings from "
                "other dimensions, broker deals with demons, and travel the megaverse.",
            "xp_table": xp_magic_adv(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 80%",
                "Math: Basic", "Lore: Magic", "Lore: Demons & Monsters",
                "Lore: Rifts & Dimensions", "Lore: Faerie",
                "Holistic Medicine", "Ley Line Rifting",
                "Literacy: All known languages"
            }),
            "occ_skills": ({
                "Technical", "Science", "Lore", "Physical"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 12, "ME": 14 ]), /* approx */
            "starting_equipment":
                "Grimoire of summoning, protective wards, travel clothing, "
                "ritual components kit, and a sidearm.",
            "flags": ({ "magic" }),
            "ppe_base": 105, /* approx  --  very high, ME bonus applied separately */
            "ppe_per_level": 10,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "shaman":
        return ([
            "description":
                "A tribal magic-user who calls upon nature spirits and ancestral power, "
                "blending wilderness survival with earthy spiritual magic.",
            "xp_table": xp_ley_line(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 50%",
                "Lore: Magic", "Lore: Spirits",
                "Holistic Medicine", "Wilderness Survival",
                "Animal Husbandry", "Identify Plants and Fruits",
                "Meditation"
            }),
            "occ_skills": ({
                "Wilderness", "Technical", "Lore", "Physical"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 5,
            "attribute_requirements": ([ "ME": 10 ]), /* approx */
            "starting_equipment":
                "Tribal garments, medicine bundle, ceremonial staff or weapon, "
                "survival pack, and ritual fetishes.",
            "flags": ({ "magic" }),
            "ppe_base": 35, /* approx  --  ME bonus applied separately */
            "ppe_per_level": 8,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "crazy":
        return ([
            "description":
                "A warrior subjected to Mind Over Matter (M.O.M.) cybernetic implants "
                "that grant psionic powers at the cost of sanity and impulse control.",
            "xp_table": xp_civilian(),
            "hp_per_level": "2d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 80%",
                "Basic Math", "Hand-to-Hand: Commando",
                "Running", "Boxing", "Swimming", "Gymnastics",
                "Meditation"
            }),
            "occ_skills": ({
                "Physical", "Espionage", "Military", "Pilot", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 9, "PS": 10, "PE": 10 ]), /* approx */
            "starting_equipment":
                "M.O.M. implants (fitted), light body armor, two ranged weapons, "
                "vibro-knife, and a personal sidearm.",
            "flags": ({ "psychic", "cybernetic" }),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 70, /* approx  --  ME bonus applied separately */
            "isp_per_level": 10
        ]);

    case "cs dead boy":
        return ([
            "description":
                "Elite Coalition infantry wearing the iconic skull-faced Dead Boy armor, "
                "the professional backbone of the Coalition war machine.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: American 98%", "Literacy: American 98%",
                "Pilot Automobile 90%", "Basic Math",
                "First Aid", "Military Etiquette", "Radio: Basic",
                "Detect Ambush", "Tactics",
                "W.P. Energy Pistol", "W.P. Energy Rifle",
                "W.P. Heavy Military Weapons", "Hand-to-Hand: Expert"
            }),
            "occ_skills": ({
                "Military", "Physical", "Pilot", "Technical", "W.P.", "Espionage"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 8, "PS": 12, "PE": 12 ]), /* approx */
            "starting_equipment":
                "Coalition Dead Boy body armor (full), C-20 laser pistol, "
                "C-12 heavy assault rifle, four grenades, radio headset, "
                "and Coalition standard field kit.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "borg":
        return ([
            "description":
                "A full-conversion cyborg: a human mind in a powerful mechanical body. "
                "Immense strength and resilience, but permanently transformed.",
            "xp_table": xp_borg(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 90%",
                "Basic Math", "Military Etiquette", "Radio: Basic",
                "Read Sensory Equipment", "Weapon Systems",
                "W.P. Energy Pistol", "W.P. Energy Rifle",
                "W.P. Heavy Military Weapons", "Hand-to-Hand: Commando"
            }),
            "occ_skills": ({
                "Military", "Physical", "Pilot", "Pilot Related", "Technical", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "PS": 14, "PE": 12 ]), /* approx */
            "starting_equipment":
                "Full conversion cyborg body (military or civilian chassis), "
                "two integrated ranged weapons, vibro-blade, and maintenance toolkit.",
            "flags": ({ "borg", "cybernetic" }),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "robot pilot":
        return ([
            "description":
                "Specialist operator of giant robots, power armor, and combat vehicles, "
                "combining technical skill with tactical precision.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Math: Advanced",
                "Pilot: Robot Combat Elite", "Pilot: Two Vehicles of Choice",
                "Computer Operation", "Read Sensory Equipment",
                "Radio: Basic", "Weapon Systems", "Military Etiquette",
                "Electronic Countermeasures"
            }),
            "occ_skills": ({
                "Pilot", "Pilot Related", "Military", "Technical", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 12, "PP": 9 ]), /* approx */
            "starting_equipment":
                "Assigned combat robot or power armor (model varies by background), "
                "personal sidearm, combat jumpsuit, and radio.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "operator":
        return ([
            "description":
                "Engineering genius and master mechanic who can build, repair, and "
                "modify almost any vehicle, weapon, or robot, for the right price.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 80%",
                "Math: Advanced", "Computer Operation",
                "Computer Programming", "Basic Electronics",
                "Advanced Electronics", "Mechanical Engineer",
                "Robot Mechanics", "Weapons Engineer"
            }),
            "occ_skills": ({
                "Technical", "Science", "Mechanical", "Electrical", "Pilot Related"
            }),
            "occ_skill_picks": 8,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 14 ]), /* approx */
            "starting_equipment":
                "Full tool kit, portable diagnostics computer, personal sidearm, "
                "work clothes, and a well-maintained vehicle.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "rogue scientist":
        return ([
            "description":
                "An independent scientist exploring the mysteries of Rifts Earth, "
                "rejecting Coalition control in pursuit of knowledge and discovery.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%",
                "Two additional languages 60%+",
                "Literacy: All known languages",
                "Math: Advanced", "Computer Operation",
                "Research", "Biology", "Chemistry",
                "Geology", "First Aid", "History: Pre-Rifts"
            }),
            "occ_skills": ({
                "Science", "Technical", "Medical", "Lore"
            }),
            "occ_skill_picks": 7,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 12 ]), /* approx */
            "starting_equipment":
                "Portable laboratory kit, specimen containers, laptop computer, "
                "data library discs, concealed sidearm, and field clothing.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "techno-wizard":
        return ([
            "description":
                "A technician who has unlocked the secret of fusing magic with machinery, "
                "creating devices that harness PPE to achieve technological miracles.",
            "xp_table": xp_ley_line(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 80%",
                "Math: Advanced", "Lore: Magic",
                "Computer Operation", "Basic Electronics",
                "Advanced Electronics", "Weapons Engineer",
                "Mechanical Engineer", "Alchemy: Basics"
            }),
            "occ_skills": ({
                "Technical", "Mechanical", "Electrical", "Lore"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 12, "ME": 10 ]), /* approx */
            "starting_equipment":
                "Personal workshop tools, one techno-wizard creation of choice, "
                "spellbook of known spells, sidearm, and armored work coat.",
            "flags": ({ "techno_wizard", "magic" }),
            "ppe_base": 35, /* approx  --  ME bonus applied separately */
            "ppe_per_level": 8,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "merc soldier":
        return ([
            "description":
                "A professional soldier for hire with solid combat and survival training, "
                "adaptable and loyal to whoever is paying.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 75%",
                "Basic Math", "First Aid", "Military Etiquette",
                "Radio: Basic", "W.P. Two Ancient",
                "W.P. Three Modern", "Hand-to-Hand: Basic"
            }),
            "occ_skills": ({
                "Military", "Physical", "Espionage", "Pilot", "Technical", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 6,
            "attribute_requirements": ([ "PS": 9, "PE": 9 ]), /* approx */
            "starting_equipment":
                "Body armor, two ranged weapons, combat knife, radio, "
                "and mercenary contracts or unit affiliation.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "tribal warrior":
        return ([
            "description":
                "A warrior of the wilderness tribes, schooled in survival, tracking, "
                "and combat far from the cities and technology of the Coalition.",
            "xp_table": xp_scout(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Wilderness Survival",
                "Hunting", "Land Navigation", "Track Animals",
                "Skin and Prepare Animals", "First Aid",
                "Climbing", "Swimming",
                "W.P. Two Ancient", "W.P. Rifle",
                "Hand-to-Hand: Basic"
            }),
            "occ_skills": ({
                "Wilderness", "Physical", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 6,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Tribal armor or leather, two ancient weapons, hunting rifle, "
                "survival kit, and tribal totem or fetish.",
            "flags": ({}),
            "ppe_base": 0,
            "ppe_per_level": 0,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    case "ley line rifter":
        return ([
            "description":
                "A sorcerer of enormous power who has mastered the art of opening, "
                "closing, and traversing dimensional Rifts, walking the most dangerous "
                "path of all magic users.",
            "xp_table": xp_magic_adv(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%",
                "Two additional languages 70%+",
                "Literacy: All known languages",
                "Math: Basic", "Lore: Magic",
                "Lore: Rifts & Dimensions", "Ley Line Rifting",
                "Holistic Medicine", "Wilderness Survival",
                "Meditation"
            }),
            "occ_skills": ({
                "Technical", "Science", "Lore", "Physical"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 12, "ME": 14 ]), /* approx */
            "starting_equipment":
                "Traveller's robes, dimensional compass, ley line map fragments, "
                "spellbook, arcane focus, and concealed sidearm.",
            "flags": ({ "magic" }),
            "ppe_base": 105, /* approx  --  ME bonus applied separately */
            "ppe_per_level": 10,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    // ── Coalition / ISS group ──────────────────────────────────────────────

    case "cs military specialist":
        return ([
            "description":
                "An elite Coalition specialist combining combat expertise with technical "
                "skills, the sharp end of the CS military beyond basic infantry.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: American 98%", "Literacy: American 98%",
                "Pilot Automobile 90%", "Basic Math", "First Aid",
                "Military Etiquette", "Radio: Basic", "Detect Ambush",
                "Tactics", "Electronic Countermeasures", "Weapon Systems",
                "W.P. Energy Pistol", "W.P. Energy Rifle",
                "W.P. Heavy Military Weapons", "Hand-to-Hand: Expert"
            }),
            "occ_skills": ({
                "Military", "Physical", "Pilot", "Technical", "W.P.", "Espionage"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 10, "PS": 12, "PE": 12 ]), /* approx */
            "starting_equipment":
                "Dead Boy armor, C-20 laser pistol, C-12 heavy assault rifle, "
                "four E-clips, radio headset, CS military kit.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "cs samas rpa pilot":
        return ([
            "description":
                "An elite Coalition pilot trained to operate the SAMAS flying power "
                "armor, the most mobile and feared unit in the CS air forces.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: American 98%", "Literacy: American 98%",
                "Math: Advanced", "Pilot: SAMAS Power Armor",
                "Pilot: Two Vehicles of Choice", "Computer Operation",
                "Read Sensory Equipment", "Radio: Basic",
                "Weapon Systems", "Military Etiquette",
                "Electronic Countermeasures"
            }),
            "occ_skills": ({
                "Pilot", "Pilot Related", "Military", "Technical", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 10, "PP": 11 ]), /* approx */
            "starting_equipment":
                "SAMAS power armor (CS military issue), C-20 laser pistol, "
                "TX-5 particle beam rifle, radio headset, CS support package.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "cs technical officer":
        return ([
            "description":
                "A Coalition officer specialising in advanced technology, cybernetics, "
                "and field engineering, the technical backbone of CS operations.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: American 98%", "Literacy: American 98%",
                "Math: Advanced", "Computer Operation",
                "Computer Programming", "Basic Electronics",
                "Advanced Electronics", "Pilot Automobile 80%",
                "Military Etiquette", "Radio: Basic", "W.P. Energy Pistol"
            }),
            "occ_skills": ({
                "Technical", "Science", "Electrical", "Mechanical", "Military"
            }),
            "occ_skill_picks": 7,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 14 ]), /* approx */
            "starting_equipment":
                "CS officer uniform, portable tech kit, laptop computer, "
                "concealed sidearm, CS technical badge, field engineering tools.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "iss peacekeeper":
        return ([
            "description":
                "A Coalition Internal Security Specialist enforcing the law in CS cities "
                "and territories, with powers of search, arrest, and summary judgment.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: American 98%", "Literacy: American 98%",
                "Pilot Automobile 80%", "Basic Math", "First Aid",
                "Military Etiquette", "Law: General", "Interrogation",
                "Detect Ambush", "Intelligence", "Radio: Basic",
                "W.P. Energy Pistol", "Hand-to-Hand: Expert"
            }),
            "occ_skills": ({
                "Espionage", "Military", "Physical", "Technical", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 10, "MA": 9, "PS": 10, "PE": 10 ]), /* approx */
            "starting_equipment":
                "ISS body armor, C-20 laser pistol, stun baton, handcuffs, "
                "radio, ISS badge and credentials, and vehicle access.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "iss specter":
        return ([
            "description":
                "A covert ISS operative working in the shadows, infiltrating underground "
                "movements and eliminating threats to Coalition security.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: American 98%", "Two languages 60%+",
                "Literacy: American 98%", "Pilot Automobile 80%",
                "Basic Math", "Disguise", "Impersonation", "Concealment",
                "Intelligence", "Interrogation", "Surveillance",
                "Tracking", "Pick Locks", "Streetwise",
                "W.P. Energy Pistol", "Hand-to-Hand: Commando"
            }),
            "occ_skills": ({
                "Espionage", "Military", "Rogue", "Technical", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 12, "MA": 12, "PP": 10 ]), /* approx */
            "starting_equipment":
                "Civilian clothing (multiple sets), disguise kit, ISS covert ID set, "
                "laser pistol, silenced sidearm, surveillance kit.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "ntset protector":
        return ([
            "description":
                "A specialist in dealing with supernatural threats, demons, and Rifts "
                "phenomena. NTSET operatives carry weapons blessed against the supernatural.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 80%",
                "Pilot Automobile 80%", "Basic Math",
                "Lore: Demons & Monsters", "Lore: Magic",
                "First Aid", "Tracking", "Detect Ambush",
                "Radio: Basic", "W.P. Energy Pistol", "W.P. Energy Rifle",
                "Hand-to-Hand: Expert"
            }),
            "occ_skills": ({
                "Espionage", "Military", "Physical", "Lore", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 10, "PS": 12, "PE": 12 ]), /* approx */
            "starting_equipment":
                "Mystically-warded body armor, two energy weapons, "
                "silver ammunition, anti-supernatural kit, radio, NTSET credentials.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    // ── Combat / Merc group ────────────────────────────────────────────────

    case "special forces (merc)":
        return ([
            "description":
                "The most highly trained mercenary soldiers available: elite, "
                "versatile, and expensive. Demolitions, tactics, and raw combat skill.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Two languages 60%+",
                "Pilot Automobile 90%", "Basic Math",
                "Demolitions", "Demolitions Disposal", "First Aid",
                "Detect Ambush", "Intelligence", "Climbing",
                "Swimming", "Wilderness Survival", "Military Etiquette",
                "Radio: Basic", "W.P. Two Ancient", "W.P. Three Modern",
                "Hand-to-Hand: Commando"
            }),
            "occ_skills": ({
                "Espionage", "Military", "Physical", "Pilot", "Technical", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 10, "PS": 13, "PE": 13 ]), /* approx */
            "starting_equipment":
                "Top-grade body armor, two high-power ranged weapons, "
                "combat knife, explosives kit, radio, and operator ID.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "master assassin":
        return ([
            "description":
                "The ultimate predator for hire, combining stealth, poisons, martial "
                "arts, and ruthless precision to eliminate any target without trace.",
            "xp_table": xp_cyber_knight(), /* approx - demanding table */
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Two languages 60%+",
                "Pilot Automobile 70%", "Basic Math",
                "Disguise", "Impersonation", "Concealment", "Prowl",
                "Pick Locks", "Surveillance", "Intelligence",
                "Tracking", "Poisons: Identify", "Forgery",
                "W.P. Two Ancient", "W.P. Two Modern",
                "Hand-to-Hand: Martial Arts"
            }),
            "occ_skills": ({
                "Espionage", "Rogue", "Physical", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 12, "PP": 12, "PE": 12 ]), /* approx */
            "starting_equipment":
                "Concealed armor, three exotic weapons, disguise kit, "
                "poison kit, and a list of current contracts.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "glitter boy pilot":
        return ([
            "description":
                "Operator of the legendary pre-Rifts Glitter Boy power armor. "
                "The boom gun is the single most powerful infantry weapon on Rifts Earth.",
            "xp_table": xp_borg(), /* approx - GB pilots advance slowly, suit is immensely powerful */
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 70%",
                "Math: Advanced", "Pilot: Glitter Boy",
                "Pilot: Two Vehicles of Choice",
                "Computer Operation", "Read Sensory Equipment",
                "Radio: Basic", "Weapon Systems", "Military Etiquette"
            }),
            "occ_skills": ({
                "Pilot", "Pilot Related", "Military", "Technical", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 10, "PP": 12 ]), /* approx */
            "starting_equipment":
                "Glitter Boy power armor (inherited or salvaged), personal sidearm, "
                "basic maintenance tools, and a supply of rail gun slugs.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "ninja juicer":
        return ([
            "description":
                "A Juicer who underwent martial training alongside drug enhancement, "
                "combining superhuman physical ability with the precision of a ninja.",
            "xp_table": xp_civilian(),
            "hp_per_level": "2d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 70%",
                "Basic Math", "Acrobatics", "Boxing", "Climbing",
                "Gymnastics", "Prowl", "Swimming",
                "W.P. Three Ancient", "Hand-to-Hand: Martial Arts"
            }),
            "occ_skills": ({
                "Physical", "Espionage", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 9, "PP": 13, "PE": 12 ]), /* approx */
            "starting_equipment":
                "Juicer harness (ninja-configured), light flexible armor, "
                "three ancient weapons, shuriken, grappling hook and line.",
            "flags": ({ "juicer" }),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    // ── Rogue / Criminal group ─────────────────────────────────────────────

    case "forger":
        return ([
            "description":
                "Expert creator of false identities, Coalition IDs, travel passes, "
                "and any document the right buyer needs. The foundation of the black market.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Two languages 60%+",
                "Literacy: All known languages", "Math: Basic",
                "Computer Operation", "Forgery", "Art",
                "Holography", "Photography", "Research"
            }),
            "occ_skills": ({
                "Technical", "Rogue", "Science"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 14, "PP": 10 ]), /* approx */
            "starting_equipment":
                "Portable forgery workstation, holographic printer, "
                "ID template library, false credentials for five identities, sidearm.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "freelance spy":
        return ([
            "description":
                "An independent intelligence professional selling information to the "
                "highest bidder, trained in infiltration, surveillance, and tradecraft.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Two languages 60%+",
                "Literacy 70%", "Pilot Automobile 80%", "Basic Math",
                "Disguise", "Impersonation", "Intelligence",
                "Surveillance", "Tracking", "Detect Ambush",
                "Find Contraband", "Concealment", "Streetwise",
                "W.P. Energy Pistol", "Hand-to-Hand: Basic"
            }),
            "occ_skills": ({
                "Espionage", "Rogue", "Technical", "Physical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 12, "MA": 12 ]), /* approx */
            "starting_equipment":
                "Civilian clothes (several sets), recording devices, "
                "concealed sidearm, transport, encrypted communicator, and client retainer.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "professional thief":
        return ([
            "description":
                "Expert at bypassing any security "
                "system, cracking any safe, and vanishing without trace.",
            "xp_table": xp_civilian(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 70%",
                "Pilot Automobile 70%", "Basic Math",
                "Acrobatics", "Climbing", "Concealment",
                "Pick Locks", "Pick Pockets", "Safe-cracking",
                "Prowl", "Streetwise", "Find Contraband", "Palming"
            }),
            "occ_skills": ({
                "Rogue", "Physical", "Technical"
            }),
            "occ_skill_picks": 7,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 12, "PP": 12 ]), /* approx */
            "starting_equipment":
                "Black form-fitting suit, lock pick set, electronic bypass tools, "
                "grappling hook, concealed sidearm, and a list of high-value targets.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "smuggler":
        return ([
            "description":
                "Runner of contraband past Coalition checkpoints and hostile territories. "
                "Trusted by those who pay, dangerous to those who do not.",
            "xp_table": xp_civilian(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Two languages 60%+",
                "Pilot Automobile 90%", "Pilot Boat or Hovercycle",
                "Basic Math", "Concealment", "Find Contraband",
                "Streetwise", "Detect Ambush", "Radio: Basic",
                "Barter", "W.P. Energy Pistol", "Hand-to-Hand: Basic"
            }),
            "occ_skills": ({
                "Rogue", "Pilot", "Technical", "Physical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 6,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Armored vehicle, personal sidearm, false cargo manifest, "
                "contraband contacts network, radio jammer, and a secret route map.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    // ── Regional group ─────────────────────────────────────────────────────

    case "knight (europe)":
        return ([
            "description":
                "A warrior of the old European tradition, schooled in chivalry, honor, "
                "and the knightly arts in the magical lands of post-Rifts Europe.",
            "xp_table": xp_cyber_knight(),
            "hp_per_level": "1d8",
            "base_skills": ({
                "Language: Native 98%", "Literacy 80%",
                "Horsemanship: Warhorse", "Body Building", "Climbing",
                "First Aid", "Heraldry", "Lore: One of Choice",
                "W.P. Two Ancient", "W.P. Two Modern",
                "Hand-to-Hand: Expert"
            }),
            "occ_skills": ({
                "Military", "Physical", "Technical", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 5,
            "attribute_requirements": ([ "PS": 12, "PE": 12, "MA": 10 ]), /* approx */
            "starting_equipment":
                "Full plate or chainmail (enchanted if noble lineage), "
                "two ancient weapons, one ranged weapon, horse, and knightly crest.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "royal knight":
        return ([
            "description":
                "A knight of noble birth or exceptional accomplishment who has earned "
                "a royal appointment, wielding political authority and martial excellence.",
            "xp_table": xp_cyber_knight(),
            "hp_per_level": "1d8",
            "base_skills": ({
                "Language: Native 98%", "Two languages 60%+",
                "Literacy 90%", "Horsemanship: Warhorse", "Body Building",
                "Climbing", "First Aid", "Heraldry", "Law: European",
                "Lore: Two of Choice", "Etiquette",
                "W.P. Two Ancient", "W.P. Two Modern",
                "Hand-to-Hand: Expert"
            }),
            "occ_skills": ({
                "Military", "Physical", "Technical", "W.P.", "Lore"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 10, "PS": 14, "PE": 14, "MA": 12 ]), /* approx */
            "starting_equipment":
                "Masterwork or enchanted full plate, signet ring of office, "
                "two ancient weapons, one ranged weapon, warhorse, and a land grant.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "pirate (s.a.)":
        return ([
            "description":
                "A free-sailing pirate of the South American coasts and rivers, "
                "combining naval combat with a raider's cunning.",
            "xp_table": xp_scout(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "One language 50%+",
                "Pilot Boat 80%", "Pilot Submersible",
                "Basic Math", "Navigation", "Swimming",
                "Climbing", "First Aid", "Gambling: Basic",
                "Concealment", "Streetwise",
                "W.P. Two Ancient", "W.P. Two Modern",
                "Hand-to-Hand: Basic"
            }),
            "occ_skills": ({
                "Rogue", "Pilot", "Physical", "Technical", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 5,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Leather or light armor, two ranged weapons, cutlass or vibro-blade, "
                "navigational charts, share in a vessel, and a crew's cut of plunder.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "sailor (s.a.)":
        return ([
            "description":
                "A skilled mariner of the South American waterways and coasts, expert "
                "navigator and trader in the dangerous seas of Rifts Earth.",
            "xp_table": xp_scout(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "One language 50%+",
                "Pilot Boat 90%", "Basic Math", "Navigation",
                "Astronomy", "Swimming", "Fishing",
                "First Aid", "Radio: Basic", "Lore: Sea Life",
                "W.P. Two Modern", "Hand-to-Hand: Basic"
            }),
            "occ_skills": ({
                "Pilot", "Physical", "Technical", "Wilderness"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 6,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Mariner's clothing, rain gear, personal sidearm, "
                "navigational tools, radio, and a berth on a vessel.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    // ── Rare / Special group ───────────────────────────────────────────────

    case "gifted gypsy":
        return ([
            "description":
                "A Gypsy with a genuine gift for reading people and a touch of real "
                "magic, wandering as fortune-teller, herbalist, and subtle enchanter.",
            "xp_table": xp_ley_line(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Two languages 60%+",
                "Literacy 70%", "Basic Math", "Holistic Medicine",
                "Lore: Magic", "Lore: One of Choice",
                "Gambling: Card Reading", "Disguise",
                "Streetwise", "Wilderness Survival", "Perform: Fortune Telling"
            }),
            "occ_skills": ({
                "Technical", "Lore", "Rogue", "Physical"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 6,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Colorful traveling clothes, fortune-telling kit, herbal medicine bag, "
                "concealed knife, transport, and a small PPE focus item.",
            "flags": ({ "magic" }),
            "ppe_base": 20, /* approx */
            "ppe_per_level": 6, /* approx */
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "maxi-man":
        return ([
            "description":
                "A subject of Mega-Damage nano-augmentation, Maxi-Men are near-superhuman "
                "soldiers with reinforced bones, enhanced reflexes, and internal weapons.",
            "xp_table": xp_borg(), /* approx - powerful class */
            "hp_per_level": "2d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 80%",
                "Basic Math", "Body Building", "Running", "Boxing",
                "Swimming", "W.P. Two Ancient", "W.P. Three Modern",
                "Hand-to-Hand: Commando"
            }),
            "occ_skills": ({
                "Physical", "Military", "Espionage", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 9, "PS": 14, "PE": 14 ]), /* approx */
            "starting_equipment":
                "Nano-augmentation treatment (permanent, integral weapons), "
                "light body armor, two ranged weapons, nano-repair capsules.",
            "flags": ({ "cybernetic" }),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "cyber-doc":
        return ([
            "description":
                "A surgeon specialising in cybernetic implants, bionics, and the "
                "merger of flesh with machine. Equally at home with scalpel and circuit.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 90%",
                "Biology", "Chemistry", "Chemistry: Analytical",
                "Medical Doctor", "Pathology", "Cybernetics: Basic",
                "Cybernetics: Advanced", "Computer Operation",
                "Math: Advanced", "First Aid"
            }),
            "occ_skills": ({
                "Medical", "Science", "Technical", "Physical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 14, "ME": 8, "PP": 10 ]), /* approx */
            "starting_equipment":
                "Cybersurgical kit, portable bionic workshop, medical scanner, "
                "diagnostic computer, personal sidearm, and medical credentials.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    // ── Help-file extras ───────────────────────────────────────────────────

    case "delphi juicer":
        return ([
            "description":
                "A rare Juicer variant whose harness includes psionic enhancers, "
                "granting limited clairvoyance alongside standard physical enhancements.",
            "xp_table": xp_civilian(),
            "hp_per_level": "2d6",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 70%",
                "Basic Math", "Boxing", "Running",
                "Meditation", "Sense Evil", "Sense Magic",
                "W.P. Two Ancient", "W.P. Two Modern",
                "Hand-to-Hand: Martial Arts"
            }),
            "occ_skills": ({
                "Physical", "Espionage", "Pilot", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "PS": 9, "PE": 9, "ME": 10 ]), /* approx */
            "starting_equipment":
                "Delphi Juicer harness (fitted), medium body armor, "
                "two weapons, and one year drug supply.",
            "flags": ({ "juicer", "psychic" }),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 30, /* approx */
            "isp_per_level": 8 /* approx */
        ]);

    case "hyperion juicer":
        return ([
            "description":
                "A next-generation Juicer running enhanced combat stimulants for even "
                "greater physical ability, at a steeper price in lifespan.",
            "xp_table": xp_civilian(),
            "hp_per_level": "2d6+2",
            "base_skills": ({
                "Language: Native 98%", "Pilot Automobile 80%",
                "Basic Math", "Boxing", "Running",
                "Gymnastics", "Acrobatics",
                "W.P. Two Ancient", "W.P. Three Modern",
                "Hand-to-Hand: Martial Arts"
            }),
            "occ_skills": ({
                "Physical", "Espionage", "Military", "Pilot", "W.P."
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 4,
            "attribute_requirements": ([ "PS": 12, "PE": 12 ]), /* approx */
            "starting_equipment":
                "Hyperion Juicer harness (military grade, fitted), medium body armor, "
                "three weapons, and one year enhanced drug supply.",
            "flags": ({ "juicer" }),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "tattooed man":
        return ([
            "description":
                "A warrior marked by magical tattoos that grant extraordinary abilities. "
                "Each tattoo is a living enchantment drawn with PPE.",
            "xp_table": xp_ley_line(),
            "hp_per_level": "1d8",
            "base_skills": ({
                "Language: Native 98%", "Literacy 70%",
                "Body Building", "Running", "Boxing",
                "Lore: Magic", "Wilderness Survival",
                "W.P. Two Ancient", "W.P. Two Modern",
                "Hand-to-Hand: Commando"
            }),
            "occ_skills": ({
                "Physical", "Military", "Lore", "W.P."
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "ME": 10, "PS": 12 ]), /* approx */
            "starting_equipment":
                "Magical tattoos (starting set), light armor, "
                "ancient weapon tied to a tattoo, one ranged weapon.",
            "flags": ({ "magic" }),
            "ppe_base": 20, /* approx */
            "ppe_per_level": 6, /* approx */
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "air warlock":
        return ([
            "description":
                "An elemental mage attuned to the power of air and lightning, "
                "commanding winds, calling storms, and hurling bolts of elemental energy.",
            "xp_table": xp_ley_line(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 80%",
                "Math: Basic", "Lore: Magic", "Lore: Weather and Storms",
                "Wilderness Survival", "Meditation", "Holistic Medicine",
                "Two additional languages 60%+"
            }),
            "occ_skills": ({
                "Technical", "Lore", "Physical"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 5,
            "attribute_requirements": ([ "ME": 12 ]), /* approx */
            "starting_equipment":
                "Light robes or travelling clothes, focus crystal attuned to air, "
                "spellbook of elemental invocations, and a concealed sidearm.",
            "flags": ({ "magic" }),
            "ppe_base": 35, /* approx */
            "ppe_per_level": 8, /* approx */
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "nega-psychic":
        return ([
            "description":
                "A rare human whose mind creates a field suppressing psionic and magical "
                "energies, invisible to supernatural senses, a nightmare for spellcasters.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 80%",
                "Pilot Automobile 60%", "Basic Math",
                "Detect Ambush", "Intelligence",
                "Lore: Magic", "Lore: Psychic",
                "Meditation", "Streetwise"
            }),
            "occ_skills": ({
                "Technical", "Espionage", "Physical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "ME": 14 ]), /* approx */
            "starting_equipment":
                "Normal clothing, personal sidearm, and a burning desire "
                "to be left alone by the supernatural.",
            "flags": ({ "nega_psychic" }),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    // ── Atlantean OCCs (WB2: Atlantis) ─────────────────────────────────────

    case "atlantean nomad":
        return ([
            "description":
                "A True Atlantean who wanders the megaverse trading, exploring, "
                "and fighting the forces of the Splugorth.  Versatile dimension-walker.",
            "xp_table": xp_scout(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Language: Dragonese 80%",
                "Literacy 80%", "Dimensional Navigation",
                "Lore: Rifts and Dimensions", "Lore: Magic",
                "Wilderness Survival", "Detect Ambush",
                "Intelligence", "Basic Math"
            }),
            "occ_skills": ({
                "Physical", "Technical", "Wilderness", "Rogue"
            }),
            "occ_skill_picks": 8,
            "secondary_skills": 6,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Enchanted Atlantean armor, a short sword with at least one "
                "magic tattoo weapon effect, dimensional compass, and survival gear.",
            "flags": ({ "magic_tattoos" }),
            "ppe_base": 20,
            "ppe_per_level": 5,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "atlantean slave":
        return ([
            "description":
                "A True Atlantean who escaped Splugorth captivity.  Psychically "
                "scarred but resourceful, with insider knowledge of Splugorth forces.",
            "xp_table": xp_civilian(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Language: Splugorthian 70%",
                "Basic Math", "Detect Ambush", "Streetwise",
                "Escape Artist", "Lore: Splugorth",
                "Lore: Magic", "First Aid", "Wilderness Survival"
            }),
            "occ_skills": ({
                "Physical", "Rogue", "Technical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 6,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Salvaged clothing, one weapon of choice, a concealed blade, "
                "and detailed knowledge of Splugorth slave markets.",
            "flags": ({ "magic_tattoos" }),
            "ppe_base": 15,
            "ppe_per_level": 4,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "tattoo warrior":
        return ([
            "description":
                "An Atlantean warrior who has mastered the art of tattoo magic "
                "in combat.  Tattoo Warriors use magical tattoos as primary "
                "weapons and armor, shunning most technology in favor of living "
                "magic permanently inscribed on their skin.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Language: Dragonese 80%",
                "Literacy 80%", "Lore: Magic", "Lore: Rifts and Dimensions",
                "Hand-to-Hand: Martial Arts",
                "W.P. Sword", "W.P. Knife",
                "Acrobatics", "Athletics"
            }),
            "occ_skills": ({
                "Physical", "Weapons", "Wilderness"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "PS": 10, "PP": 10 ]),
            "starting_equipment":
                "Vibro-sword, light MDC body armor (50 MDC), two energy clips, "
                "and four magical tattoos inscribed at character creation.",
            "flags": ({ "magic_tattoos" }),
            "ppe_base": 30,
            "ppe_per_level": 6,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "sunaj assassin":
        return ([
            "description":
                "A secret Atlantean order of killers who hunt practitioners of "
                "magic.  Sunaj conceal their Atlantean identity and operate in "
                "shadow.  Requires 10 kills and Atlantean faction 200 to unlock.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Language: Two of Choice 80%",
                "Literacy 80%", "Detect Ambush", "Intelligence",
                "Tracking", "Surveillance Systems", "Disguise",
                "Impersonation", "Prowl", "Detect Concealment"
            }),
            "occ_skills": ({
                "Espionage", "Physical", "Rogue", "Technical"
            }),
            "occ_skill_picks": 8,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 12, "ME": 12, "PP": 14 ]),
            "starting_equipment":
                "Atlantean combat armor in dark colors, concealed blade and "
                "two sidearms, forged identity documents, and a kill ledger.",
            "flags": ({ "magic_tattoos", "assassin_order" }),
            "ppe_base": 25,
            "ppe_per_level": 6,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "necromancer":
        return ([
            "description":
                "A dark mage who has mastered the magic of death and undeath.  "
                "Necromancers command skeletal warriors, communicate with spirits, "
                "draw power from death energy, and have a deep knowledge of "
                "anatomy and the dark arts.",
            "xp_table": xp_magic_adv(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Literacy 80%",
                "Math: Basic", "Lore: Magic", "Lore: Demons & Monsters",
                "Lore: Faerie", "Biology", "Pathology",
                "Holistic Medicine", "Chemistry: Analytical"
            }),
            "occ_skills": ({
                "Technical", "Science", "Lore", "Medical"
            }),
            "occ_skill_picks": 5,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 12, "ME": 12 ]),
            "starting_equipment":
                "Dark robes, ritual components, a grimoire of necromantic spells, "
                "one sidearm, and a collection of bones and focus materials.",
            "flags": ({ "magic" }),
            "ppe_base": 105,
            "ppe_per_level": 10,
            "isp_base": 0,
            "isp_per_level": 0
        ]);

    // ── NGR OCCs ───────────────────────────────────────────────────────────

    case "ngr soldier":
        return ([
            "description":
                "A soldier of the New German Republic, trained to fight Brodkil "
                "demons and Gargoyle invaders using a combination of advanced "
                "technology and brutal discipline.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Language: German 80%",
                "Literacy: Native 80%", "Basic Math",
                "Hand-to-Hand: Expert",
                "W.P. Energy Rifle", "W.P. Energy Pistol",
                "Military Etiquette", "Tactics",
                "Radio: Basic", "First Aid", "Land Navigation"
            }),
            "occ_skills": ({
                "Weapons", "Physical", "Military", "Technical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "PS": 10, "PE": 10 ]),
            "starting_equipment":
                "NGR infantry body armor, NGR rail gun, two energy clips, "
                "personal radio, combat knife, and standard field kit.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "ngr mechanic":
        return ([
            "description":
                "A technical specialist of the New German Republic, maintaining "
                "combat robots, vehicles, and weapon systems in the field.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Language: German 80%",
                "Literacy: Native 80%", "Basic Math",
                "Mechanical Engineer", "Robot Mechanics",
                "Basic Electronics", "Read Sensory Equipment",
                "Radio: Basic", "Computer Operation", "Pilot Automobile 60%"
            }),
            "occ_skills": ({
                "Technical", "Electrical", "Physical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 5,
            "attribute_requirements": ([ "IQ": 11 ]),
            "starting_equipment":
                "NGR field coveralls, basic tool kit, computer pad, personal radio, "
                "one sidearm, and standard maintenance supplies.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    // ── Kittani OCCs ───────────────────────────────────────────────────────

    case "kittani warrior":
        return ([
            "description":
                "An elite Kittani alien warrior in service to the Splugorth of "
                "Atlantis.  Kittani are insectoid humanoids renowned for their "
                "advanced technology and ruthless military discipline.",
            "xp_table": xp_soldier(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Language: Splugorthian 70%",
                "Literacy: Native 60%",
                "Hand-to-Hand: Martial Arts",
                "W.P. Energy Rifle", "W.P. Energy Pistol",
                "W.P. Sword", "Pilot Power Armor 60%",
                "Military Etiquette", "Tactics", "First Aid",
                "Radio: Basic"
            }),
            "occ_skills": ({
                "Weapons", "Physical", "Military", "Espionage"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 4,
            "attribute_requirements": ([]),
            "starting_equipment":
                "Kittani power armor, Kittani laser rifle, energy clips, "
                "combat blade, and Splugorth field rations.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    case "kittani field mechanic":
        return ([
            "description":
                "A Kittani technical specialist who maintains Splugorth technology "
                "and Kittani equipment in the field.  Highly valued for their "
                "ability to repair alien tech under combat conditions.",
            "xp_table": xp_scholar(),
            "hp_per_level": "1d6",
            "base_skills": ({
                "Language: Native 98%", "Language: Splugorthian 70%",
                "Literacy: Native 60%", "Basic Math",
                "Mechanical Engineer", "Robot Mechanics",
                "Basic Electronics", "Advanced Electronics",
                "Weapon Systems", "Computer Operation",
                "Pilot Automobile 60%"
            }),
            "occ_skills": ({
                "Technical", "Electrical", "Physical"
            }),
            "occ_skill_picks": 6,
            "secondary_skills": 4,
            "attribute_requirements": ([ "IQ": 12 ]),
            "starting_equipment":
                "Kittani field suit, advanced Kittani tool kit, computer pad, "
                "one sidearm with energy clips.",
            "flags": ({}),
            "ppe_base": 0, "ppe_per_level": 0,
            "isp_base": 0, "isp_per_level": 0
        ]);

    default:
        return 0;
    }
}
