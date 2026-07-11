// /daemon/language_d.c
// Rifts Language System daemon.
// Manages language knowledge stored as player env var "rifts_languages"
// in the format "American:98,Dragonese:60,Elvish:45".
// Active language stored as env var "active_language".

#include <daemons.h>

inherit "/secure/std/daemon";

private string *all_langs;

void create() {
    all_langs = ({
        "American", "Spanish", "French", "German", "Japanese", "Chinese",
        "Euro", "Dragonese", "Elvish", "Elven", "Dwarven", "Atlantean",
        "Trade Four", "Techno-Speak", "Demonic", "Faerie Speak",
        "Gobblely", "American Sign Language"
    });
}

// ── Internal helpers ─────────────────────────────────────────────────────────

// Parse "American:98,Dragonese:60" into ([ "American": 98, "Dragonese": 60 ])
private mapping parse_langs(string s) {
    mapping result;
    string *pairs, *pair;
    int i;

    result = ([]);
    if(!s || s == "") return result;
    pairs = explode(s, ",");
    for(i = 0; i < sizeof(pairs); i++) {
        pair = explode(pairs[i], ":");
        if(sizeof(pair) == 2)
            result[pair[0]] = to_int(pair[1]);
    }
    return result;
}

// Serialize ([ "American": 98 ]) back to "American:98"
private string serialize_langs(mapping m) {
    string *parts;
    string *ks;
    int i;

    ks = keys(m);
    parts = ({});
    for(i = 0; i < sizeof(ks); i++)
        parts += ({ ks[i] + ":" + m[ks[i]] });
    return implode(parts, ",");
}

// ── Race native language table ────────────────────────────────────────────────

// Returns mapping of native languages for a race: ([ "American": 98 ])
mapping race_native_languages(string race) {
    int i;
    mapping all;

    switch(lower_case(race)) {
    case "human":
    case "burster":
    case "mind melter":
    case "cs psi-stalker":
    case "wild psi-stalker":
    case "conservator":
        return ([ "American": 98 ]);

    case "elf":
        return ([ "Elven": 100, "Dragonese": 80, "American": 60 ]);
    case "dwarf":
        return ([ "Dwarven": 100, "American": 50 ]);

    case "atlantean":
        return ([ "Atlantean": 98, "Dragonese": 80, "American": 60 ]);

    case "great horned dragon":
    case "fire dragon":
    case "ice dragon":
    case "thunder lizard dragon":
    case "dragon":
        return ([ "Dragonese": 98, "American": 50 ]);

    case "gargoyle":
    case "gurgoyle":
        return ([ "Demonic": 80, "American": 40 ]);

    case "kankoran":
        return ([ "American": 80, "Gobblely": 50 ]);

    case "rahu-man":
        return ([ "Gobblely": 98, "American": 50 ]);

    case "dog boy":
        return ([ "American": 98 ]);

    case "hawrk-duhk":
    case "hawrk-ka":
        return ([ "Gobblely": 90, "Dragonese": 60, "American": 40 ]);

    case "coyle":
    case "bearman":
    case "ratling":
    case "orc":
    case "goblin":
    case "ogre":
    case "troll":
        return ([ "Gobblely": 80, "American": 40 ]);

    case "algor frost giant":
    case "jotan":
    case "nimro fire giant":
    case "titan":
        return ([ "Euro": 80, "American": 50 ]);

    case "secondary vampire":
    case "wild vampire":
        return ([ "Demonic": 90, "American": 60 ]);

    case "werewolf":
    case "werebear":
    case "weretiger":
        return ([ "American": 80 ]);

    case "common faerie":
    case "common pixie":
    case "frost pixie":
    case "green wood faerie":
    case "night-elves faerie":
    case "silver bells faerie":
    case "tree sprite":
    case "water sprite":
        return ([ "Faerie Speak": 100, "Elven": 80, "American": 30 ]);

    case "juicer":
    case "glitter boy":
    case "glitter boy pilot":
        return ([ "American": 98 ]);

    case "godling": {
        all = ([]);
        for(i = 0; i < sizeof(all_langs); i++)
            all[all_langs[i]] = 100;
        return all;
    }
    case "d-bee":
        return ([ "Gobblely": 100, "American": 25 ]);
    case "demon":
        return ([ "Demonic": 100, "American": 40 ]);
    case "faerie":
        return ([ "Faerie Speak": 100, "American": 30 ]);
    default:
        return ([ "American": 98 ]);
    }
}

// ── Public: Query and modify player languages ─────────────────────────────────

// Returns mapping of languages this player knows: ([ "American": 98 ])
mapping query_known(object who) {
    string s;
    if(!who) return ([]);
    s = (string)who->getenv("rifts_languages");
    if(!s) return ([]);
    return parse_langs(s);
}

// Returns the comprehension percentage for a specific language (0-100).
// Players or NPCs with no language data understand everything (backward compat).
int can_understand(object who, string lang) {
    string lang_env;
    mapping known;

    if(!who || !lang) return 0;
    lang_env = (string)who->getenv("rifts_languages");
    // No language data: treat as omnilingual (non-Rifts chars and NPCs)
    if(!lang_env || lang_env == "") return 100;
    known = parse_langs(lang_env);
    if(!known[lang]) return 0;
    return known[lang];
}

// Returns the active language name for this player.
// Returns "" if the player has no language data (non-Rifts backward compat).
string query_active(object who) {
    string lang, lang_env;
    mapping known;
    string *ks;

    if(!who) return "";
    lang = (string)who->getenv("active_language");
    if(lang && lang != "") return lang;
    lang_env = (string)who->getenv("rifts_languages");
    if(!lang_env || lang_env == "") return "";
    known = parse_langs(lang_env);
    ks = keys(known);
    if(sizeof(ks)) return ks[0];
    return "";
}

// Grant a language at a given proficiency (1-100). Does not downgrade.
void grant_language(object who, string lang, int pct) {
    mapping known;
    int existing;

    if(!who || !lang) return;
    known = query_known(who);
    existing = known[lang];
    if(pct <= existing) return;
    known[lang] = pct;
    who->setenv("rifts_languages", serialize_langs(known));
}

// Grant all native languages for the player's race.
// Also sets active_language if not already set.
void grant_native_languages(object who) {
    mapping native;
    string race, *langs, active;
    int i;

    if(!who) return;
    race = (string)who->query_race();
    if(!race) race = "human";
    native = race_native_languages(race);
    langs = keys(native);
    for(i = 0; i < sizeof(langs); i++)
        grant_language(who, langs[i], native[langs[i]]);
    active = (string)who->getenv("active_language");
    if(!active || active == "") {
        if(sizeof(langs) > 0) who->setenv("active_language", langs[0]);
        else who->setenv("active_language", "American");
    }
}

// Switch the player's active speaking language.
// Returns 1 on success, 0 if they don't know that language.
int set_active(object who, string lang) {
    mapping known;

    if(!who || !lang) return 0;
    known = query_known(who);
    if(!known[lang]) return 0;
    who->setenv("active_language", lang);
    return 1;
}

// Teach a language at a given proficiency. Alias for grant_language.
// Used by shop NPCs: LANGUAGE_D->teach_language(player, lang, pct)
void teach_language(object who, string lang, int pct) {
    grant_language(who, lang, pct);
}

// Returns all language names.
string *query_all_languages() {
    return all_langs + ({});
}

// ── Public: Garbling and formatting ─────────────────────────────────────────

// Garble msg based on comprehension percentage (0-100).
// Returns garbled string, or 0 if pct is 0 (caller shows "says something in X").
string format_for_listener(string msg, int pct) {
    string *words;
    int i;

    if(pct >= 100) return msg;
    if(pct <= 0) return 0;

    words = explode(msg, " ");
    for(i = 0; i < sizeof(words); i++) {
        if(random(100) >= pct)
            words[i] = "---";
    }
    return implode(words, " ");
}
