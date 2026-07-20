/* /daemon/rifts_spells_d.c */
/* Palladium Rifts spell definitions and effect dispatch. */

#include <daemons.h>

/* ── Spell data ────────────────────────────────────────────────────────────── */
/* Returns mapping with keys: ppe_cost, duration(s), range, effect, desc. */
/* Returns 0 if spell is unknown. */

string normalize_spell_name(string name) {
    string n;

    if(!name) return "";
    n = lower_case(name);
    switch(n) {
    case "invis":
    case "invisibility":
        return "invisibility simple";
    case "armorofithan":
    case "armor_of_ithan":
    case "ithan":
        return "armor of ithan";
    case "goe":
        return "globe of daylight";
    case "magicnet":
        return "magic net";
    case "impervious fire":
    case "impervious to fire":
        return "impervious to fire";
    case "impervious cold":
    case "impervious to cold":
        return "impervious to cold";
    case "breath of life":
        return "breath of life";
    case "heavy darkness":
        return "heavy darkness";
    case "words of truth":
        return "words of truth";
    case "see invisible":
    case "see the invisible spell":
        return "see invisible";
    case "animate dead":
        return "animate dead";
    case "constrain being":
        return "constrain being";
    case "astral projection":
        return "astral projection";
    case "detect concealment":
        return "detect concealment";
    case "mend wounds":
        return "mend wounds";
    case "sense danger":
        return "sense danger";
    case "mystic alarm":
        return "mystic alarm";
    case "slow movement":
        return "slow movement";
    case "detect poison":
        return "detect poison";
    default:
        return n;
    }
}

string query_spell_alias_hint(string name) {
    if(!name) return "";
    switch(lower_case(name)) {
    case "invisibility simple": return "invis";
    case "armor of ithan":      return "ithan";
    case "globe of daylight":   return "goe";
    default:                    return "";
    }
}

mapping query_spell(string name) {
    name = normalize_spell_name(name);
    switch(name) {
    case "globe of daylight":
        return ([ "ppe_cost":2, "duration":240, "range":"room",
                  "effect":"light", "desc":"Illuminates the area for 4 minutes." ]);
    case "sense evil":
        return ([ "ppe_cost":2, "duration":0, "range":"room",
                  "effect":"sense_evil", "desc":"Detects evil presences in the room." ]);
    case "sense magic":
        return ([ "ppe_cost":4, "duration":0, "range":"room",
                  "effect":"sense_magic", "desc":"Detects magical objects and beings." ]);
    case "thunderclap":
        return ([ "ppe_cost":4, "duration":0, "range":"room",
                  "effect":"stun", "desc":"Stuns all enemies in the room.  Save negates." ]);
    case "energy bolt":
        return ([ "ppe_cost":5, "duration":0, "range":"single",
                  "effect":"damage", "desc":"Hurls a bolt of magical energy.  1d6 MDC per level." ]);
    case "heal wounds":
        return ([ "ppe_cost":6, "duration":0, "range":"touch",
                  "effect":"heal", "desc":"Heals 1d8 SDC or 2d6 HP." ]);
    case "light target":
        return ([ "ppe_cost":4, "duration":180, "range":"single",
                  "effect":"mark", "desc":"Marks target with magical light.  Cannot hide." ]);
    case "dimensional pocket":
        return ([ "ppe_cost":20, "duration":604800, "range":"self",
                  "effect":"pocket", "desc":"Creates extradimensional storage for 1 week." ]);
    case "call familiar":
        return ([ "ppe_cost":30, "duration":0, "range":"self",
                  "effect":"familiar", "desc":"Summons a small magical creature companion." ]);
    case "windrush":
        return ([ "ppe_cost":8, "duration":0, "range":"room",
                  "effect":"windrush", "desc":"A gust hurls victims into an adjacent room and scatters their gear. No target sweeps everyone present. Save negates." ]);
    case "swimasafish":
        return ([ "ppe_cost":6, "duration":600, "range":"self",
                  "effect":"swimasafish", "desc":"Allows breathing underwater and movement through water rooms for 10 minutes." ]);

    /* ── Level 1 spells ────────────────────────────────────────────────────── */
    case "cloud of smoke":
        return ([ "ppe_cost":2, "duration":120, "range":"room",
                  "effect":"smoke",
                  "desc":"Fills room with obscuring smoke. All combat rolls -2 strike." ]);
    case "blinding flash":
        return ([ "ppe_cost":1, "duration":0, "range":"single",
                  "effect":"blind",
                  "desc":"Blinds target 1d4 rounds. -10 strike/parry/dodge. Save vs magic negates." ]);
    case "levitation":
        return ([ "ppe_cost":5, "duration":180, "range":"self",
                  "effect":"levitate",
                  "desc":"Caster floats. Cannot be tripped or knocked down." ]);
    case "magic net":
        return ([ "ppe_cost":7, "duration":120, "range":"single",
                  "effect":"net",
                  "desc":"Snares target in a magic net (30 MDC). Cannot flee but can fight. Save vs magic (14+) negates." ]);
    case "chameleon":
        return ([ "ppe_cost":6, "duration":600, "range":"self",
                  "effect":"chameleon",
                  "desc":"Camouflage self. +20% prowl for 10 minutes." ]);
    case "fear":
        return ([ "ppe_cost":5, "duration":60, "range":"single",
                  "effect":"fear",
                  "desc":"Frightens target. Save vs magic (12+) negates." ]);
    case "see aura":
        return ([ "ppe_cost":4, "duration":120, "range":"self",
                  "effect":"see_aura",
                  "desc":"See auras and alignments in the room for 2 minutes." ]);
    case "impervious to fire":
        return ([ "ppe_cost":12, "duration":120, "range":"self",
                  "effect":"impervious_fire",
                  "desc":"Immune to fire and heat for 2 minutes." ]);
    case "impervious to cold":
        return ([ "ppe_cost":12, "duration":120, "range":"self",
                  "effect":"impervious_cold",
                  "desc":"Immune to cold and ice for 2 minutes." ]);
    case "breath of life":
        return ([ "ppe_cost":10, "duration":0, "range":"touch",
                  "effect":"breath_of_life",
                  "desc":"Revive unconscious ally for 2d6 HP or SDC." ]);
    case "exorcism":
        return ([ "ppe_cost":15, "duration":0, "range":"single",
                  "effect":"exorcism",
                  "desc":"Banish spirits or harm undead. Save vs magic." ]);
    case "heavy darkness":
        return ([ "ppe_cost":6, "duration":300, "range":"room",
                  "effect":"heavy_darkness",
                  "desc":"Magical darkness fills the room for 5 minutes." ]);
    case "words of truth":
        return ([ "ppe_cost":8, "duration":120, "range":"single",
                  "effect":"words_truth",
                  "desc":"Target cannot lie for 2 minutes." ]);
    case "see invisible":
        return ([ "ppe_cost":4, "duration":120, "range":"self",
                  "effect":"see_invisible_spell",
                  "desc":"See invisible beings for 2 minutes." ]);
    case "animate dead":
        return ([ "ppe_cost":12, "duration":0, "range":"room",
                  "effect":"animate_dead",
                  "desc":"Raise one mindless undead servant from a corpse." ]);
    case "constrain being":
        return ([ "ppe_cost":10, "duration":60, "range":"single",
                  "effect":"constrain_being",
                  "desc":"Magically bind target. Cannot flee. Save vs magic (14+)." ]);
    case "astral projection":
        return ([ "ppe_cost":15, "duration":120, "range":"self",
                  "effect":"astral_projection",
                  "desc":"Project your spirit. Body stays behind for 2 minutes." ]);
    case "detect concealment":
        return ([ "ppe_cost":6, "duration":0, "range":"room",
                  "effect":"detect_concealment",
                  "desc":"Reveal invisible or camouflaged beings in the room." ]);
    case "mend wounds":
        return ([ "ppe_cost":8, "duration":0, "range":"touch",
                  "effect":"mend_wounds",
                  "desc":"Heal 2d6 HP or SDC on a living target." ]);
    case "sense danger":
        return ([ "ppe_cost":4, "duration":300, "range":"self",
                  "effect":"sense_danger",
                  "desc":"Danger sense warns of ambush for 5 minutes." ]);
    case "mystic alarm":
        return ([ "ppe_cost":10, "duration":3600, "range":"room",
                  "effect":"mystic_alarm",
                  "desc":"Magical alarm triggers if intruders enter the room." ]);
    case "slow movement":
        return ([ "ppe_cost":8, "duration":60, "range":"single",
                  "effect":"slow_movement",
                  "desc":"Target moves at half speed. Save vs magic (14+)." ]);
    case "detect poison":
        return ([ "ppe_cost":4, "duration":0, "range":"touch",
                  "effect":"detect_poison",
                  "desc":"Detect poison or toxin in a target or object." ]);
    case "death trance":
        return ([ "ppe_cost":6, "duration":600, "range":"self",
                  "effect":"death_trance",
                  "desc":"Appear dead to casual inspection for 10 minutes." ]);
    case "globe of silence":
        return ([ "ppe_cost":8, "duration":120, "range":"room",
                  "effect":"globe_silence",
                  "desc":"Magical silence fills the room for 2 minutes." ]);
    case "burn":
        return ([ "ppe_cost":5, "duration":0, "range":"single",
                  "effect":"burn",
                  "desc":"Small flame scorches one target for 2d4 damage." ]);
    case "cure disorders":
        return ([ "ppe_cost":10, "duration":0, "range":"touch",
                  "effect":"cure_disorders",
                  "desc":"Remove fear, domination, and minor mental afflictions." ]);
    case "charismatic aura":
        return ([ "ppe_cost":6, "duration":300, "range":"self",
                  "effect":"charismatic_aura",
                  "desc":"+2 to MA-based social rolls for 5 minutes." ]);
    case "repel animals":
        return ([ "ppe_cost":5, "duration":300, "range":"room",
                  "effect":"repel_animals",
                  "desc":"Animals flee the room for 5 minutes." ]);
    case "cleanse":
        return ([ "ppe_cost":8, "duration":0, "range":"touch",
                  "effect":"cleanse",
                  "desc":"Remove poison, disease, and minor curses from a target." ]);
    case "luck curse":
        return ([ "ppe_cost":10, "duration":300, "range":"single",
                  "effect":"luck_curse",
                  "desc":"Curse target with bad luck for 5 minutes." ]);
    case "wall of light":
        return ([ "ppe_cost":12, "duration":120, "range":"room",
                  "effect":"wall_of_light",
                  "desc":"Brilliant light fills the room, hindering evil beings." ]);
    case "wisps of confusion":
        return ([ "ppe_cost":8, "duration":60, "range":"single",
                  "effect":"wisps_confusion",
                  "desc":"Swirling lights confuse target. Save vs magic." ]);

    /* ── Level 2 spells ────────────────────────────────────────────────────── */
    case "fire ball":
        return ([ "ppe_cost":10, "duration":0, "range":"single",
                  "effect":"fireball",
                  "desc":"4d6 MDC fire damage. No save." ]);
    case "carpet of adhesion":
        return ([ "ppe_cost":10, "duration":120, "range":"room",
                  "effect":"adhesion",
                  "desc":"Floor becomes sticky. Save vs magic or stuck. -2 APM while trapped." ]);
    case "invisibility simple":
        return ([ "ppe_cost":6, "duration":180, "range":"self",
                  "effect":"invisible",
                  "desc":"Caster invisible. Broken by combat or loud action." ]);

    /* ── Level 3 spells ────────────────────────────────────────────────────── */
    case "tongues":
        return ([ "ppe_cost":10, "duration":300, "range":"touch",
                  "effect":"tongues",
                  "desc":"Target understands and speaks all languages for 5 min per level." ]);
    case "fire bolt":
        return ([ "ppe_cost":7, "duration":0, "range":"single",
                  "effect":"firebolt",
                  "desc":"5d6 MDC fire. Superior fire damage spell." ]);
    case "armor of ithan":
        return ([ "ppe_cost":10, "duration":300, "range":"self",
                  "effect":"ithan_armor",
                  "desc":"30 MDC magical field protects the caster for 5 minutes." ]);
    case "circle of flame":
        return ([ "ppe_cost":20, "duration":120, "range":"room",
                  "effect":"flame_circle",
                  "desc":"Ring of fire around caster. 2d6 MDC per round to anyone entering." ]);
    case "telekinesis":
        return ([ "ppe_cost":8, "duration":120, "range":"single",
                  "effect":"spell_tk",
                  "desc":"Move objects with mind. Lift PS 20 equivalent. 2 minutes." ]);

    /* ── Level 4 spells ────────────────────────────────────────────────────────── */
    case "energy field":
        return ([ "ppe_cost":10, "duration":120, "range":"self",
                  "effect":"energy_field",
                  "desc":"Invisible 100 MDC force barrier surrounds caster for 2 minutes." ]);
    case "ignite fire":
        return ([ "ppe_cost":6, "duration":0, "range":"room",
                  "effect":"ignite_fire",
                  "desc":"Instantly ignite any flammable material within 60 feet." ]);
    case "multiple image":
        return ([ "ppe_cost":7, "duration":240, "range":"self",
                  "effect":"multiple_image",
                  "desc":"1d4+1 illusory duplicates confuse attackers for 4 minutes." ]);
    case "shadow meld":
        return ([ "ppe_cost":6, "duration":120, "range":"self",
                  "effect":"shadow_meld",
                  "desc":"Merge with shadows in dim light. Near-invisible for 2 minutes." ]);
    case "mask of deceit":
        return ([ "ppe_cost":15, "duration":600, "range":"self",
                  "effect":"mask_deceit",
                  "desc":"Appear as any specific person for 10 minutes." ]);

    /* ── Level 5 spells ────────────────────────────────────────────────────────── */
    case "domination":
        return ([ "ppe_cost":10, "duration":60, "range":"single",
                  "effect":"domination",
                  "desc":"Completely control one target for 1 minute. Save 16 ME negates." ]);
    case "escape":
        return ([ "ppe_cost":8, "duration":0, "range":"self",
                  "effect":"escape_spell",
                  "desc":"Instantly teleport 60 feet to an adjacent area." ]);
    case "fly":
        return ([ "ppe_cost":15, "duration":300, "range":"self",
                  "effect":"fly_spell",
                  "desc":"Fly at 50 mph for 5 minutes." ]);
    case "horrific illusion":
        return ([ "ppe_cost":12, "duration":240, "range":"single",
                  "effect":"horrific_illusion",
                  "desc":"Target sees terrifying vision. Save 14 ME or flee in panic." ]);
    case "impervious to energy":
        return ([ "ppe_cost":20, "duration":120, "range":"self",
                  "effect":"impervious_energy",
                  "desc":"Immune to energy attacks for 2 minutes." ]);
    case "metamorphosis animal":
        return ([ "ppe_cost":20, "duration":600, "range":"self",
                  "effect":"metamorphosis",
                  "desc":"Transform into any animal up to twice caster's size for 10 minutes." ]);

    /* ── Level 6 spells ────────────────────────────────────────────────────────── */
    case "call lightning":
        return ([ "ppe_cost":15, "duration":0, "range":"single",
                  "effect":"call_lightning",
                  "desc":"1d6x10 MDC lightning. Requires outdoor/open sky." ]);
    case "freeze":
        return ([ "ppe_cost":20, "duration":60, "range":"single",
                  "effect":"freeze_spell",
                  "desc":"2d6 MDC cold damage and entomb in ice. Save 16 PE avoids freeze." ]);
    case "negate mechanics":
        return ([ "ppe_cost":20, "duration":60, "range":"single",
                  "effect":"negate_mechanics",
                  "desc":"Disable one machine or vehicle for 1 minute." ]);
    case "restoration":
        return ([ "ppe_cost":30, "duration":0, "range":"touch",
                  "effect":"restoration_spell",
                  "desc":"Powerful healing: 2d6 HP/SDC or caster-level MDC." ]);
    case "shadow bolt":
        return ([ "ppe_cost":15, "duration":0, "range":"single",
                  "effect":"shadow_bolt",
                  "desc":"3d6 MDC bolt of shadow energy." ]);
    case "time slip":
        return ([ "ppe_cost":20, "duration":30, "range":"self",
                  "effect":"time_slip",
                  "desc":"Phase out of normal time. Impervious to physical attacks for 2 melee rounds." ]);

    /* ── Level 7 spells ────────────────────────────────────────────────────────── */
    case "cloud of fear":
        return ([ "ppe_cost":24, "duration":240, "range":"room",
                  "effect":"cloud_of_fear",
                  "desc":"Panic up to 2d4 targets in room. Save 14 ME or flee screaming." ]);
    case "dispel magic barrier":
        return ([ "ppe_cost":20, "duration":0, "range":"single",
                  "effect":"dispel_barrier",
                  "desc":"Permanently remove one magic field, ward, or barrier." ]);
    case "invincible armor":
        return ([ "ppe_cost":25, "duration":300, "range":"self",
                  "effect":"invincible_armor",
                  "desc":"200 MDC magical force armor for 5 minutes." ]);
    case "mystic portal":
        return ([ "ppe_cost":60, "duration":120, "range":"touch",
                  "effect":"mystic_portal",
                  "desc":"Open doorway between two points up to 1 mile per level for 2 minutes." ]);
    case "protection circle simple":
        return ([ "ppe_cost":30, "duration":600, "range":"self",
                  "effect":"protection_circle",
                  "desc":"Supernatural beings cannot enter 10-foot circle for 10 minutes." ]);

    /* ── Level 8 spells ────────────────────────────────────────────────────────── */
    case "energy drain":
        return ([ "ppe_cost":50, "duration":0, "range":"touch",
                  "effect":"energy_drain",
                  "desc":"Drain 2d6 PPE from target and absorb it. Save 15 PE negates." ]);
    case "eyes of thoth":
        return ([ "ppe_cost":8, "duration":3600, "range":"self",
                  "effect":"eyes_of_thoth",
                  "desc":"Read any language, alien script, or magical text for 1 hour." ]);
    case "locate":
        return ([ "ppe_cost":30, "duration":0, "range":"self",
                  "effect":"locate_spell",
                  "desc":"Locate a specific known person in the same region." ]);
    case "mass confusion":
        return ([ "ppe_cost":30, "duration":240, "range":"room",
                  "effect":"mass_confusion",
                  "desc":"Confuse all in area. Save 15 IQ or wander aimlessly for 4 minutes." ]);
    case "negate magic":
        return ([ "ppe_cost":30, "duration":0, "range":"single",
                  "effect":"negate_magic",
                  "desc":"Cancel one active spell or magic effect on a target." ]);
    case "rift walk":
        return ([ "ppe_cost":80, "duration":0, "range":"self",
                  "effect":"rift_walk",
                  "desc":"Step through ley line rift to any known location. Requires nearby ley line." ]);

    /* ── Level 9 spells ────────────────────────────────────────────────────────── */
    case "metamorphosis superior":
        return ([ "ppe_cost":30, "duration":600, "range":"self",
                  "effect":"meta_superior",
                  "desc":"Transform into any living creature up to 10x caster size. Retains psionics. 10 min/level." ]);
    case "create zombie":
        return ([ "ppe_cost":50, "duration":0, "range":"touch",
                  "effect":"create_zombie",
                  "desc":"Animate a corpse as a mindless zombie servant. Limit: 1 per 2 levels." ]);
    case "amulet":
        return ([ "ppe_cost":75, "duration":31536000, "range":"touch",
                  "effect":"amulet_spell",
                  "desc":"Enchant an object with a protective property. +2 to saves. Lasts 1 year." ]);
    case "summon fog":
        return ([ "ppe_cost":30, "duration":600, "range":"room",
                  "effect":"summon_fog",
                  "desc":"Thick magical fog, zero visibility beyond 6 feet. 10 min/level." ]);
    case "teleport":
        return ([ "ppe_cost":40, "duration":0, "range":"self",
                  "effect":"teleport_spell",
                  "desc":"Teleport to recall point. 10% failure at level 1, -1% per level (min 1%)." ]);
    case "energy disruption":
        return ([ "ppe_cost":30, "duration":0, "range":"room",
                  "effect":"energy_disruption",
                  "desc":"Disrupt all electronic/energy systems in area for 1d6 melees." ]);

    /* ── Level 10 spells ────────────────────────────────────────────────────────── */
    case "life drain":
        return ([ "ppe_cost":50, "duration":0, "range":"touch",
                  "effect":"life_drain",
                  "desc":"Drain 4d6 HP from target. Caster gains half as HP. Save 16 PE negates." ]);
    case "circle of protection superior":
        return ([ "ppe_cost":65, "duration":1800, "range":"self",
                  "effect":"circle_superior",
                  "desc":"Complete 20-foot barrier. No supernatural entity enters/exits. Blocks outside magic. 30 min/level." ]);
    case "create magic scroll":
        return ([ "ppe_cost":40, "duration":0, "range":"touch",
                  "effect":"create_scroll",
                  "desc":"Encode a known spell into parchment for one-time use by any character." ]);
    case "fly as eagle":
        return ([ "ppe_cost":25, "duration":1800, "range":"self",
                  "effect":"fly_eagle",
                  "desc":"Fly at 100 mph with eagle maneuverability. Can target self or one other. 30 min/level." ]);
    case "turn dead superior":
        return ([ "ppe_cost":20, "duration":0, "range":"room",
                  "effect":"turn_dead_sup",
                  "desc":"Destroy up to 1d6 per level undead outright. Powerful undead save at 14." ]);
    case "impervious to magic energy":
        return ([ "ppe_cost":30, "duration":120, "range":"self",
                  "effect":"impervious_mdce",
                  "desc":"Immune to magical energy attacks and MDC energy weapons. 2 min/level." ]);

    /* ── Level 11 spells ────────────────────────────────────────────────────────── */
    case "dimensional envelope":
        return ([ "ppe_cost":80, "duration":86400, "range":"touch",
                  "effect":"dim_envelope",
                  "desc":"Create pocket dimension inside an object. 30 cu ft/level capacity. 24 hours/level." ]);
    case "soul drain":
        return ([ "ppe_cost":60, "duration":0, "range":"single",
                  "effect":"soul_drain",
                  "desc":"Drain 1d6 ME from target. Drained ME returns at 1/day. Save 18 ME negates." ]);
    case "mystic fulcrum":
        return ([ "ppe_cost":35, "duration":240, "range":"self",
                  "effect":"mystic_fulcrum",
                  "desc":"Double all spell ranges and damage. PPE cost for subsequent spells doubled. 4 min/level." ]);
    case "animate object":
        return ([ "ppe_cost":30, "duration":300, "range":"single",
                  "effect":"animate_object",
                  "desc":"Animate one object. PS 20, 50 SDC. Follows simple commands. 5 min/level." ]);
    case "anti magic cloud":
        return ([ "ppe_cost":140, "duration":600, "range":"room",
                  "effect":"anti_magic_cloud",
                  "desc":"No magic of any kind works in cloud. Affects caster. 10 min/level." ]);

    /* ── Level 12 spells ────────────────────────────────────────────────────────── */
    case "rift dimensional pocket":
        return ([ "ppe_cost":100, "duration":600, "range":"self",
                  "effect":"rift_dim_pocket",
                  "desc":"Step into pocket rift. Invisible and intangible. Can observe but not interact. 10 min/level." ]);
    case "resurrection":
        return ([ "ppe_cost":500, "duration":0, "range":"touch",
                  "effect":"resurrection_spell",
                  "desc":"Restore full life to recently dead (within 24 hours). Costs caster 2d6 PE permanently." ]);
    case "ressurection":
        return ([ "ppe_cost":500, "duration":0, "range":"touch",
                  "effect":"resurrection_spell",
                  "desc":"Restore full life to recently dead (within 24 hours). Costs caster 2d6 PE permanently." ]);
    case "create rift":
        return ([ "ppe_cost":200, "duration":0, "range":"touch",
                  "effect":"create_rift",
                  "desc":"Open a dimensional rift. Requires ley line proximity. Can transport unwilling target (save 18 PE)." ]);
    case "time hole":
        return ([ "ppe_cost":120, "duration":0, "range":"single",
                  "effect":"time_hole",
                  "desc":"Target falls into temporal bubble, frozen 1d4 melees. Save 18 PE negates." ]);

    /* ── Level 13 spells ────────────────────────────────────────────────────────── */
    case "close rift":
        return ([ "ppe_cost":300, "duration":0, "range":"300ft",
                  "effect":"close_rift",
                  "desc":"Seal an open rift permanently. Prevents dimensional bleed. Caster must be level 13+." ]);
    case "create golem":
        return ([ "ppe_cost":500, "duration":0, "range":"touch",
                  "effect":"create_golem",
                  "desc":"Animate a golem from stone or metal. MDC 200, PS 40 supernatural, AR 16, APM 3. One max." ]);
    case "calling":
        return ([ "ppe_cost":140, "duration":0, "range":"dimensional",
                  "effect":"calling_spell",
                  "desc":"Call a named supernatural being across dimensions. Save 14 ME or compelled to appear." ]);
    case "dimensional lock":
        return ([ "ppe_cost":200, "duration":86400, "range":"60ft radius",
                  "effect":"dim_lock",
                  "desc":"No dimensional travel, rifts, or teleportation in area. 24 hours per level." ]);
    case "re animation":
        return ([ "ppe_cost":250, "duration":0, "range":"touch",
                  "effect":"re_animation",
                  "desc":"Raise skeleton warrior from bones. MDC 60, AR 10, two attacks, 2d6 bone weapon." ]);

    /* ── Level 14 spells ────────────────────────────────────────────────────────── */
    case "power word kill":
        return ([ "ppe_cost":250, "duration":0, "range":"30ft",
                  "effect":"power_word_kill",
                  "desc":"Instantly kill one target level 6 or lower (save 18 ME). Higher levels: 4d6x10 MDC instead." ]);
    case "create magic weapon":
        return ([ "ppe_cost":400, "duration":0, "range":"touch",
                  "effect":"create_magic_weapon",
                  "desc":"Permanently enchant a weapon. +2 strike, +2d6 MDC, counts as MDC weapon. Glows faintly." ]);
    case "conjure elemental":
        return ([ "ppe_cost":350, "duration":300, "range":"self",
                  "effect":"conjure_elemental",
                  "desc":"Summon a minor elemental (fire/water/earth/air). MDC 150, 4d6 MDC attack, AR 14. 5 min/level." ]);
    case "seal rift":
        return ([ "ppe_cost":400, "duration":0, "range":"100ft",
                  "effect":"seal_rift",
                  "desc":"Permanently seal a nexus point, preventing all future rift formation there. Caster 14+." ]);

    /* ── Level 15 spells ────────────────────────────────────────────────────────── */
    case "wish":
        return ([ "ppe_cost":1000, "duration":0, "range":"unlimited",
                  "effect":"wish_spell",
                  "desc":"Alter reality in a limited but significant way. Scope is admin discretion. Wish is logged." ]);
    case "time warp age":
        return ([ "ppe_cost":500, "duration":0, "range":"60ft",
                  "effect":"time_warp_age",
                  "desc":"Age target 10 years per caster level. Humans past natural lifespan die. Save 20 ME." ]);
    case "rift to oblivion":
        return ([ "ppe_cost":700, "duration":0, "range":"30ft",
                  "effect":"rift_to_oblivion",
                  "desc":"Micro-rift under target. Save 20 PE or transported to random dimension. Success: 6d6 MDC." ]);
    case "supreme being summoning":
        return ([ "ppe_cost":2000, "duration":600, "range":"self",
                  "effect":"supreme_being",
                  "desc":"Summon a god-like entity for one task. Lose 1d4x10 max PPE permanently. Nexus required." ]);

    default:
        return 0;
    }
}

/* ── Save vs magic ─────────────────────────────────────────────────────────── */
/* Returns 1 if target successfully saves. */

private int save_check(object target) {
    int me;
    int bonus;
    int roll;

    me = (int)target->query_stats("ME");
    if(me >= 26)      bonus = 3;
    else if(me >= 21) bonus = 2;
    else if(me >= 17) bonus = 1;
    else              bonus = 0;
    roll = random(20) + 1 + bonus;
    return (roll >= 15);
}

/* ── Individual effect handlers ────────────────────────────────────────────── */

private void fx_globe(object target) {
    object room;
    room = environment(this_player());
    if(!room) return;
    room->set_property("light", 5);
    write("A brilliant globe of daylight springs into being, illuminating the area.\n");
    say(this_player()->query_cap_name() + " summons a globe of daylight.");
    call_out("end_globe", 240, room);
}

void end_globe(object room) {
    if(!objectp(room)) return;
    room->set_property("light", 2);
    tell_room(room, "The globe of daylight fades.\n");
}

private void fx_sense_evil(object target) {
    object *contents;
    int i;
    int found;
    string names;

    contents = all_inventory(environment(this_player()));
    found = 0;
    names = "";
    for(i = 0; i < sizeof(contents); i++) {
        if(contents[i] == this_player()) continue;
        if(!living(contents[i])) continue;
        if((int)contents[i]->query_alignment() < -200) {
            found++;
            names += (string)contents[i]->query_short() + " ";
        }
    }
    if(found > 0)
        write("Your magical senses tingle.  You detect evil: " + names + "\n");
    else
        write("You sense no evil in the immediate area.\n");
}

private void fx_sense_magic(object target) {
    object *contents;
    int i;
    int found;
    string names;

    contents = all_inventory(environment(this_player()));
    found = 0;
    names = "";
    for(i = 0; i < sizeof(contents); i++) {
        if(contents[i] == this_player()) continue;
        if((int)contents[i]->query_stats("max_PPE") > 0) {
            found++;
            names += (string)contents[i]->query_short() + " ";
        } else if((int)contents[i]->query_property("magical")) {
            found++;
            names += (string)contents[i]->query_short() + " ";
        }
    }
    if(found > 0)
        write("Your magical senses tingle.  You detect magic: " + names + "\n");
    else
        write("You sense no magic in the immediate area.\n");
}

private void fx_stun(object target) {
    object *contents;
    int i;
    int stunned;

    write("You clap your hands and unleash a thunderous boom!\n");
    say(this_player()->query_cap_name() + " unleashes a deafening thunderclap!");
    contents = all_inventory(environment(this_player()));
    stunned = 0;
    for(i = 0; i < sizeof(contents); i++) {
        if(!living(contents[i])) continue;
        if(contents[i] == this_player()) continue;
        if(member_array(this_player(), (object *)contents[i]->query_attackers()) == -1)
            continue;
        if(save_check(contents[i])) {
            tell_object(contents[i], "You resist the thunderclap!\n");
            continue;
        }
        contents[i]->set_property("stunned", 1);
        tell_object(contents[i],
            "You are stunned by the thunderclap and cannot act!\n");
        call_out("end_stun", 15, contents[i]);
        stunned++;
    }
    if(stunned > 0)
        write(stunned + " enemy/enemies stunned.\n");
    else
        write("No enemies were stunned.\n");
}

void end_stun(object who) {
    if(!objectp(who)) return;
    who->set_property("stunned", 0);
    tell_object(who, "You recover from the stun.\n");
}

private void fx_damage(object target) {
    int level;
    int damage;
    int i;
    int cur_mdc;
    int cur_sdc;

    if(!target) {
        write("Cast at whom?  Syntax: cast energy bolt at <target>\n");
        return;
    }
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    damage = 0;
    for(i = 0; i < level; i++)
        damage += random(6) + 1;

    write("You hurl a bolt of magical energy!  " + damage + " MDC!\n");
    tell_object(target,
        this_player()->query_cap_name() + " hurls a bolt of magical energy at you!\n");
    say(this_player()->query_cap_name() + " hurls a bolt of energy at " +
        target->query_cap_name() + "!");

    /* MDC damage: bypass armor, hit MDC pool directly (magic ignores physical armor) */
    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        cur_mdc = (int)target->query_stats("MDC");
        cur_mdc -= damage;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= -20) {
            tell_object(target, "You are destroyed by magical energy!\n");
            catch(target->die());
        }
    } else {
        /* Non-Rifts target: hit HP */
        cur_sdc = (int)target->query_hp();
        cur_sdc -= damage;
        if(cur_sdc < 0) {
            target->set_hp(0);
            catch(target->die());
        } else {
            target->set_hp(cur_sdc);
        }
    }
}

private void fx_heal(object target) {
    int hp_roll;
    int sdc_roll;
    int cur_hp;
    int max_hp;
    int cur_sdc;
    int max_sdc;

    if(!target) target = this_player();
    hp_roll  = random(6) + random(6) + 2;   /* 2d6 */
    sdc_roll = random(8) + 1;                /* 1d8 */

    cur_hp  = (int)target->query_stats("rifts_hp");
    max_hp  = (int)target->query_stats("max_rifts_hp");
    cur_sdc = (int)target->query_stats("SDC");
    max_sdc = (int)target->query_stats("max_SDC");

    if(max_sdc > 0 && cur_sdc < max_sdc) {
        int new_sdc;
        new_sdc = cur_sdc + sdc_roll;
        if(new_sdc > max_sdc) new_sdc = max_sdc;
        target->set_stats("SDC", new_sdc);
        write("You channel healing energy.  " + sdc_roll + " SDC restored.\n");
    } else {
        int new_hp;
        new_hp = cur_hp + hp_roll;
        if(new_hp > max_hp) new_hp = max_hp;
        target->set_stats("rifts_hp", new_hp);
        write("You channel healing energy.  " + hp_roll + " HP restored.\n");
    }
    if(target != this_player())
        tell_object(target, this_player()->query_cap_name() + " heals your wounds.\n");
}

private void fx_mark(object target) {
    if(!target) {
        write("Cast at whom?  Syntax: cast light target at <target>\n");
        return;
    }
    target->set_property("magically_lit", 1);
    write(target->query_cap_name() + " is surrounded by magical light!\n");
    tell_object(target, "You are marked by magical light and cannot hide!\n");
    say(this_player()->query_cap_name() + " marks " + target->query_cap_name() +
        " with magical light!");
    call_out("end_mark", 180, target);
}

void end_mark(object who) {
    if(!objectp(who)) return;
    who->set_property("magically_lit", 0);
    tell_object(who, "The magical light fades from you.\n");
}

private void fx_pocket(object target) {
    write("You weave a dimensional pocket into existence around yourself.\n"
          "Extradimensional storage is active for one week.\n"
          "Use 'store <item>' and 'retrieve <item>' while the pocket is open.\n");
    this_player()->setenv("has_dim_pocket", "1");
    this_player()->set_property("dim_pocket_until", time() + 604800);
}

private void fx_familiar(object target) {
    object fam;
    object room;

    room = environment(this_player());
    if(this_player()->query_property("has_familiar")) {
        write("You already have a familiar bonded.\n");
        return;
    }
    fam = new("/std/obj/rifts_familiar");
    if(!fam) {
        write("The summoning fails.\n");
        return;
    }
    if(room) fam->move(room);
    catch(fam->bind_to(this_player()));
    this_player()->set_property("has_familiar", 1);
    write("A small magical familiar materializes and bonds to your spirit.\n");
    say((string)this_player()->query_cap_name() + " summons a familiar!");
}

private void windrush_scatter_items(object target, object room) {
    object *inv;
    string tname;
    int i;
    int dropped;

    if(!target || !room) return;
    tname = (string)target->query_cap_name();
    inv = all_inventory(target);
    dropped = 0;
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i]) continue;
        if(inv[i]->query_worn()) continue;
        if(inv[i]->query_wielded()) continue;
        /* each loose item has a 1 in 3 chance of being torn away */
        if(random(3)) continue;
        if((int)inv[i]->move(room) != 0) continue;
        dropped++;
    }
    if(dropped > 0) {
        tell_object(target, "The wind tears " + dropped +
            " of your belongings from your grasp!\n");
        tell_room(room, "Some of " + tname +
            "'s belongings scatter across the ground!\n", ({ target }));
    }
}

private void windrush_blow(object target, object room) {
    object dest_room;
    string *exits;
    string dest;
    string tname;
    int idx;
    int pe_save;
    int impact_dmg;
    int cur_sdc;

    if(!target || !room) return;
    tname = (string)target->query_cap_name();

    /* PE save vs difficulty 14 to resist */
    pe_save = random(20) + 1;
    pe_save += ((int)target->query_stats("PE") > 10)
               ? ((int)target->query_stats("PE") - 10) : 0;
    /* creatures flagged can_be_windrush are always blown away, no PE save */
    if((int)target->query_property("can_be_windrush")) pe_save = 0;

    if(pe_save >= 14) {
        write(tname + " staggers but resists your wind blast!\n");
        tell_object(target,
            "A wall of wind slams into you - you stagger but hold your ground!\n");
        say(tname + " staggers but resists the wind blast!",
            ({ this_player(), target }));
        return;
    }

    exits = (string *)room->query_exits();
    if(!exits || sizeof(exits) < 1) {
        /* no exits: slam into wall */
        impact_dmg = random(6) + random(6) + 2;
        write("There is nowhere to blow " + tname + "! " + tname +
            " slams into the wall for " + impact_dmg + " SDC!\n");
        tell_object(target,
            "A wall of wind slams into you! You crash against the wall! " +
            impact_dmg + " SDC!\n");
        say(tname + " slams into the wall from the magical gust!",
            ({ this_player(), target }));
        cur_sdc = (int)target->query_sdc();
        if(cur_sdc > 0)
            target->add_sdc(-impact_dmg);
        else
            target->add_hp(-impact_dmg);
        windrush_scatter_items(target, room);
        return;
    }

    idx  = random(sizeof(exits));
    dest = (string)room->query_exit(exits[idx]);

    write("You hurl " + tname + " away on a blast of wind, toward the " +
        exits[idx] + "!\n");
    tell_object(target,
        "A wall of wind slams into you and hurls you across the room!\n");

    windrush_scatter_items(target, room);

    if(objectp(target) && stringp(dest)) {
        target->cease_all_attacks();
        target->move(dest);
        dest_room = environment(target);
        if(dest_room && dest_room != room) {
            tell_room(dest_room,
                tname + " tumbles in, thrown by magical wind.\n",
                ({ target }));
        }
    }

    /* 1d6 SDC impact on landing */
    impact_dmg = random(6) + 1;
    cur_sdc = (int)target->query_sdc();
    if(cur_sdc > 0)
        target->add_sdc(-impact_dmg);
    else
        target->add_hp(-impact_dmg);
}

private void fx_windrush(object target) {
    object room;
    object *inv;
    int i;
    int hit;

    room = environment(this_player());
    if(!room) return;

    if(target && (!living(target) || environment(target) != room)) {
        write("The wind can only hurl living beings.\n");
        return;
    }

    write("A blast of magical wind erupts from your hands!\n");
    if(target) {
        say(this_player()->query_cap_name() +
            " hurls a blast of magical wind at " +
            (string)target->query_cap_name() + "!",
            ({ this_player(), target }));
        windrush_blow(target, room);
        return;
    }

    /* no target: the gust sweeps the whole area, each victim rolls a save */
    say(this_player()->query_cap_name() +
        " unleashes a howling blast of wind across the area!",
        ({ this_player() }));
    inv = all_inventory(room);
    hit = 0;
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || !living(inv[i])) continue;
        if(inv[i] == this_player()) continue;
        windrush_blow(inv[i], room);
        hit++;
    }
    if(!hit)
        write("The wind howls through the empty area.\n");
}

private void fx_swimasafish(object target) {
    int duration;
    int level;
    string tname;

    if(!target) target = this_player();
    level    = (int)this_player()->query_level();
    duration = 600 * level;
    tname    = (string)target->query_cap_name();

    target->set_property("swim_as_fish_active",  1);
    target->set_property("swim_as_fish_expires",  time() + duration);

    tell_object(target,
        "Gills briefly form along your neck. You feel at home in water.\n");
    if(environment(target)) {
        say(tname + " shimmers briefly as water-breathing magic takes hold.",
            ({ target }));
    }
    call_out("end_swimasafish", duration, target);
}

void end_swimasafish(object who) {
    if(!objectp(who)) return;
    who->set_property("swim_as_fish_active", 0);
    who->set_property("swim_as_fish_expires", 0);
    tell_object(who, "Your aquatic adaptation fades.\n");
}

private void fx_tongues(object target) {
    int duration;
    int level;
    string tname;

    if(!target) target = this_player();
    level    = (int)this_player()->query_level();
    duration = 300 * level;
    tname    = (string)target->query_cap_name();

    target->set_property("tongues_active",  1);
    target->set_property("tongues_expires", time() + duration);

    tell_object(target,
        "Your tongue and mind open to all languages. "
        "You could speak to anyone right now.\n");
    if(target != this_player()) {
        tell_object(this_player(),
            tname + " glows faintly as the tongues spell takes hold.\n");
    }
    if(environment(target))
        say(tname + " glows faintly as a tongues spell takes hold.",
            ({ target, this_player() }));
    call_out("end_tongues", duration, target);
}

void end_tongues(object who) {
    if(!objectp(who)) return;
    who->set_property("tongues_active",  0);
    who->set_property("tongues_expires", 0);
    tell_object(who, "The gift of tongues fades from your mind.\n");
}

/* ── New Level 1-3 spell handlers ──────────────────────────────────────────── */

private void fx_smoke(object target) {
    object room;
    room = environment(this_player());
    if(!room) return;
    room->set_property("smoke_filled", 1);
    write("Thick clouds of smoke billow from your hands, filling the area!\n");
    say(this_player()->query_cap_name() +
        " fills the room with billowing clouds of magical smoke!");
    call_out("end_smoke", 120, room);
}

void end_smoke(object room) {
    if(!objectp(room)) return;
    room->set_property("smoke_filled", 0);
    tell_room(room, "The magical smoke dissipates.\n");
}

private void fx_blind(object target) {
    int dur;
    if(!target) {
        write("Cast at whom?  Syntax: cast blinding flash at <target>\n");
        return;
    }
    dur = random(4) + 1;
    target->set_property("blinded", 1);
    write("A burst of blinding light erupts in " + (string)target->query_cap_name() +
          "'s face!\n");
    tell_object(target,
        "A blinding flash of light sears your eyes! You cannot see!\n");
    say(this_player()->query_cap_name() + " unleashes a blinding flash!");
    call_out("end_blind", dur * 15, target);
}

void end_blind(object who) {
    if(!objectp(who)) return;
    who->set_property("blinded", 0);
    tell_object(who, "Your vision clears.\n");
}

private void fx_levitate(object target) {
    if((int)this_player()->query_property("levitating")) {
        write("You are already levitating.\n");
        return;
    }
    this_player()->set_property("levitating", 1);
    write("You rise gently off the ground, floating on magical force.\n");
    say(this_player()->query_cap_name() + " rises off the ground, levitating!");
    call_out("end_levitate", 180, this_player());
}

void end_levitate(object who) {
    if(!objectp(who)) return;
    who->set_property("levitating", 0);
    tell_object(who, "The levitation fades and you settle back to the ground.\n");
}

private object find_net_combat_target(object caster) {
    object *inv;
    object *atk;
    int i;

    if(!caster) return 0;
    atk = (object *)caster->query_attackers();
    if(atk && sizeof(atk)) return atk[0];
    inv = all_inventory(environment(caster));
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || !living(inv[i]) || inv[i] == caster) continue;
        atk = (object *)inv[i]->query_attackers();
        if(atk && member_array(caster, atk) != -1) return inv[i];
    }
    return 0;
}

private void fx_net(object target) {
    object caster;
    object net;
    int me_save;

    caster = this_player();
    if(!target)
        target = find_net_combat_target(caster);
    if(!target) {
        write("Cast at whom?  Syntax: cast magic net at <target>\n"
              "Or: magicnet <target>\n");
        return;
    }
    if(!living(target) || environment(target) != environment(caster)) {
        write("You don't see them here.\n");
        return;
    }
    if(target->query_property("magic_net_obj")) {
        write("They are already caught in a magic net!\n");
        return;
    }
    if(lower_case((string)caster->query_race()) != "wizard") {
        me_save = (int)target->query_stats("ME") + random(20) + 1;
        if(me_save >= 14) {
            tell_object(target, "Glowing strands reach for you but you resist!\n");
            write((string)target->query_cap_name() + " resists the magic net!\n");
            return;
        }
    }
    net = new("/std/obj/magic_net");
    if(!net) {
        write("The spell fizzles.\n");
        return;
    }
    net->move(environment(target));
    net->capture(target, caster);
}

void end_net(object who) {
    object net;

    if(!objectp(who)) return;
    net = who->query_property("magic_net_obj");
    if(net && objectp(net)) {
        catch(net->release_victim(""));
        return;
    }
    who->remove_property("entangled");
    who->remove_property("cannot_move");
    tell_object(who, "The magical net dissolves. You can move freely again.\n");
}

private void fx_chameleon(object target) {
    this_player()->set_property("chameleon_active", 1);
    write("Your form shifts to match the surroundings.\n");
    call_out("end_chameleon", 600, this_player());
}

void end_chameleon(object who) {
    if(!objectp(who)) return;
    who->remove_property("chameleon_active");
    tell_object(who, "Your chameleon camouflage fades.\n");
}

private void fx_fear(object target) {
    if(!target) {
        write("Cast at whom?  Syntax: cast fear at <target>\n");
        return;
    }
    if((int)target->query_stats("ME") + random(20) + 1 >= 12) {
        write((string)target->query_cap_name() + " resists your fear spell!\n");
        return;
    }
    target->set_property("feared", 1);
    tell_object(target, "Terror grips you! You struggle to hold your ground.\n");
    call_out("end_fear", 60, target);
}

private void fx_see_aura(object target) {
    object *inv;
    int i;

    inv = all_inventory(environment(this_player()));
    write("Auras flicker into view:\n");
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || !living(inv[i])) continue;
        write("  " + (string)inv[i]->query_cap_name() +
            " (alignment " + (string)inv[i]->query_alignment() + ")\n");
    }
    this_player()->set_property("see_aura_active", 1);
    call_out("end_see_aura", 120, this_player());
}

void end_see_aura(object who) {
    if(!objectp(who)) return;
    who->remove_property("see_aura_active");
    tell_object(who, "Your aura sight fades.\n");
}

private void fx_impervious_fire(object target) {
    if((int)this_player()->query_property("impervious_fire")) {
        write("You are already impervious to fire.\n");
        return;
    }
    this_player()->set_property("impervious_fire", 1);
    write("Flames cannot touch you for the next two minutes.\n");
    call_out("end_impervious_fire", 120, this_player());
}

void end_impervious_fire(object who) {
    if(!objectp(who)) return;
    who->remove_property("impervious_fire");
    tell_object(who, "Your fire immunity fades.\n");
}

private void fx_impervious_cold(object target) {
    if((int)this_player()->query_property("impervious_cold")) {
        write("You are already impervious to cold.\n");
        return;
    }
    this_player()->set_property("impervious_cold", 1);
    write("Cold cannot harm you for the next two minutes.\n");
    call_out("end_impervious_cold", 120, this_player());
}

void end_impervious_cold(object who) {
    if(!objectp(who)) return;
    who->remove_property("impervious_cold");
    tell_object(who, "Your cold immunity fades.\n");
}

private void fx_see_invisible_spell(object target) {
    if((int)this_player()->query_property("see_invisible_active")) {
        write("You already see the invisible.\n");
        return;
    }
    this_player()->set_property("see_invisible_active", 1);
    write("Hidden shapes shimmer into view for the next two minutes.\n");
    call_out("end_see_invisible_spell", 120, this_player());
}

void end_see_invisible_spell(object who) {
    if(!objectp(who)) return;
    who->remove_property("see_invisible_active");
    tell_object(who, "Your magical sight of the invisible fades.\n");
}

private void fx_animate_dead(object target) {
    int cur;

    cur = (int)this_player()->query_property("zombie_count");
    if(cur >= 1) {
        write("You already control an animated dead servant.\n");
        return;
    }
    this_player()->set_property("zombie_count", 1);
    write("Necromantic energy crawls into a nearby corpse. It lurches upright!\n"
          "A shambling servant awaits your orders.\n");
    say(this_player()->query_cap_name() + " animates a corpse!");
}

private void fx_constrain_being(object target) {
    int me_save;

    if(!target || !living(target)) {
        write("Constrain whom?  Syntax: cast constrain being at <target>\n");
        return;
    }
    if((int)target->query_property("entangled")) {
        write("They are already constrained.\n");
        return;
    }
    me_save = (int)target->query_stats("ME") + random(20) + 1;
    if(me_save >= 14) {
        write((string)target->query_cap_name() + " breaks free of the binding!\n");
        tell_object(target, "You shrug off a magical attempt to bind you.\n");
        return;
    }
    target->set_property("entangled", 1);
    write("Bands of force lock " + (string)target->query_cap_name() + " in place!\n");
    tell_object(target, "Magical bonds hold you fast! You cannot flee.\n");
    call_out("end_constrained_being", 60, target);
}

void end_constrained_being(object who) {
    if(!objectp(who)) return;
    who->remove_property("entangled");
    tell_object(who, "The magical bonds holding you dissolve.\n");
}

private void fx_astral_projection(object target) {
    if((int)this_player()->query_property("astral_active")) {
        write("You are already projecting astrally.\n");
        return;
    }
    this_player()->set_property("astral_active", 1);
    write("Your spirit rises from your body. You perceive the room from above.\n"
          "Your body remains behind, vulnerable, for two minutes.\n");
    say(this_player()->query_cap_name() +
        "'s eyes glaze over as their spirit departs the body.");
    call_out("end_astral_projection", 120, this_player());
}

void end_astral_projection(object who) {
    if(!objectp(who)) return;
    who->remove_property("astral_active");
    tell_object(who, "Your spirit snaps back into your body.\n");
    if(objectp(environment(who)))
        tell_room(environment(who),
            who->query_cap_name() + " blinks and returns fully to the flesh.\n",
            ({ who }));
}

private void fx_detect_concealment(object target) {
    object *inv;
    int i;
    int found;

    inv = all_inventory(environment(this_player()));
    found = 0;
    write("You sweep the area for hidden presences:\n");
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || !living(inv[i])) continue;
        if((int)inv[i]->query_property("invisible_spell") ||
           (int)inv[i]->query_property("chameleon_active")) {
            write("  You reveal " + (string)inv[i]->query_cap_name() +
                " concealed nearby.\n");
            found = 1;
        }
    }
    if(!found) write("  Nothing concealed detected.\n");
}

private void fx_mend_wounds(object target) {
    int heal;
    int cur;

    if(!target || !living(target)) {
        write("Mend whom?  Syntax: cast mend wounds at <target>\n");
        return;
    }
    heal = random(6) + 1 + random(6) + 1;
    cur = (int)target->query_stats("rifts_hp");
    if(cur > 0) {
        target->set_stats("rifts_hp", cur + heal);
        target->set_hp((int)target->query_hp() + heal);
    } else {
        cur = (int)target->query_stats("SDC");
        target->set_stats("SDC", cur + heal);
    }
    write("You mend " + (string)target->query_cap_name() +
        " for " + heal + " points.\n");
    tell_object(target, "Warm healing magic closes your wounds.\n");
}

private void fx_sense_danger(object target) {
    this_player()->set_property("sense_danger_active", 1);
    write("Your instincts sharpen. You sense danger for the next five minutes.\n");
    call_out("end_sense_danger", 300, this_player());
}

void end_sense_danger(object who) {
    if(!objectp(who)) return;
    who->remove_property("sense_danger_active");
    tell_object(who, "Your danger sense fades.\n");
}

private void fx_death_trance(object target) {
    this_player()->set_property("death_trance", 1);
    write("You slow your heartbeat and still your breath. You appear dead.\n");
    say(this_player()->query_cap_name() + " collapses, seemingly lifeless.");
    call_out("end_death_trance", 600, this_player());
}

void end_death_trance(object who) {
    if(!objectp(who)) return;
    who->remove_property("death_trance");
    tell_object(who, "The death trance ends. Life returns to your limbs.\n");
}

private void fx_globe_silence(object target) {
    object room;

    room = environment(this_player());
    if(!room) return;
    room->set_property("globe_silence", 1);
    write("You weave a globe of silence over the room.\n");
    say(this_player()->query_cap_name() + " gestures and sound seems to die.");
    call_out("end_globe_silence", 120, room);
}

void end_globe_silence(object room) {
    if(!objectp(room)) return;
    room->remove_property("globe_silence");
}

private void fx_burn(object target) {
    int damage;
    int i;
    int cur_mdc;
    int cur_hp;

    if(!target) {
        write("Cast at whom?  Syntax: cast burn at <target>\n");
        return;
    }
    if((int)target->query_property("impervious_fire")) {
        write("Your flames fail to harm " + (string)target->query_cap_name() + ".\n");
        return;
    }
    damage = 0;
    for(i = 0; i < 2; i++) damage += random(4) + 1;
    write("You scorch " + (string)target->query_cap_name() + " for " + damage + "!\n");
    tell_object(target,
        this_player()->query_cap_name() + " lashes you with magical fire!\n");
    say(this_player()->query_cap_name() + " scorches " +
        (string)target->query_cap_name() + " with a burst of flame!");
    if((int)target->query_property("mdc_creature")) {
        cur_mdc = (int)target->query_stats("MDC");
        target->set_stats("MDC", cur_mdc - damage);
    } else {
        cur_hp = (int)target->query_stats("rifts_hp");
        if(cur_hp > 0)
            target->set_stats("rifts_hp", cur_hp - damage);
        else {
            cur_hp = (int)target->query_stats("SDC");
            target->set_stats("SDC", cur_hp - damage);
        }
    }
}

private void fx_cure_disorders(object target) {
    if(!target || !living(target)) {
        write("Cure whom?  Syntax: cast cure disorders at <target>\n");
        return;
    }
    target->remove_property("feared");
    target->remove_property("dominated");
    target->remove_property("slowed");
    target->remove_property("confused");
    write("You cleanse " + (string)target->query_cap_name() +
          " of minor mental afflictions.\n");
    tell_object(target, "A calming warmth clears your mind.\n");
}

private void fx_charismatic_aura(object target) {
    this_player()->set_property("charismatic_aura", 1);
    write("A magnetic aura surrounds you for five minutes.\n");
    call_out("end_charismatic_aura", 300, this_player());
}

void end_charismatic_aura(object who) {
    if(!objectp(who)) return;
    who->remove_property("charismatic_aura");
    tell_object(who, "Your charismatic aura fades.\n");
}

private void fx_repel_animals(object target) {
    object room;

    room = environment(this_player());
    if(!room) return;
    room->set_property("repel_animals", 1);
    write("You ward the room against natural animals.\n");
    call_out("end_repel_animals", 300, room);
}

void end_repel_animals(object room) {
    if(!objectp(room)) return;
    room->remove_property("repel_animals");
}

private void fx_cleanse(object target) {
    if(!target || !living(target)) {
        write("Cleanse whom?  Syntax: cast cleanse at <target>\n");
        return;
    }
    target->remove_property("poisoned");
    target->remove_property("diseased");
    target->remove_property("possessed");
    target->remove_property("cursed");
    write("You cleanse " + (string)target->query_cap_name() + ".\n");
    tell_object(target, "Purifying magic washes through you.\n");
}

private void fx_luck_curse(object target) {
    int me_save;

    if(!target || !living(target)) {
        write("Curse whom?  Syntax: cast luck curse at <target>\n");
        return;
    }
    me_save = (int)target->query_stats("ME");
    if(random(20) + 1 + me_save / 5 >= 14) {
        write((string)target->query_cap_name() + " resists your curse!\n");
        return;
    }
    target->set_property("luck_cursed", 1);
    write("You curse " + (string)target->query_cap_name() + " with ill fortune!\n");
    tell_object(target, "A cold weight settles on your luck.\n");
    call_out("end_luck_curse", 300, target);
}

void end_luck_curse(object who) {
    if(!objectp(who)) return;
    who->remove_property("luck_cursed");
    tell_object(who, "The curse on your luck fades.\n");
}

private void fx_wall_of_light(object target) {
    object room;

    room = environment(this_player());
    if(!room) return;
    room->set_property("wall_of_light", 1);
    write("Radiance floods the room!\n");
    say(this_player()->query_cap_name() + " fills the room with blinding holy light!");
    call_out("end_wall_of_light", 120, room);
}

void end_wall_of_light(object room) {
    if(!objectp(room)) return;
    room->remove_property("wall_of_light");
    tell_room(room, "The wall of light fades.\n");
}

private void fx_wisps_confusion(object target) {
    int me_save;

    if(!target || !living(target)) {
        write("Confuse whom?  Syntax: cast wisps of confusion at <target>\n");
        return;
    }
    me_save = (int)target->query_stats("ME");
    if(random(20) + 1 + me_save / 5 >= 15) {
        write((string)target->query_cap_name() + " shakes off the wisps!\n");
        return;
    }
    target->set_property("confused", 1);
    write("Swirling lights bewilder " + (string)target->query_cap_name() + "!\n");
    tell_object(target, "Dancing lights scramble your thoughts!\n");
    call_out("end_wisps_confusion", 60, target);
}

void end_wisps_confusion(object who) {
    if(!objectp(who)) return;
    who->remove_property("confused");
    tell_object(who, "Your mind clears.\n");
}

private void fx_mystic_alarm(object target) {
    object room;

    room = environment(this_player());
    if(!room) return;
    room->set_property("mystic_alarm", 1);
    room->set_property("mystic_alarm_caster", this_player());
    write("You ward the room with a silent mystic alarm.\n");
    say(this_player()->query_cap_name() + " murmurs a warding incantation.");
}

private void fx_slow_movement(object target) {
    int me_save;

    if(!target || !living(target)) {
        write("Slow whom?  Syntax: cast slow movement at <target>\n");
        return;
    }
    me_save = (int)target->query_stats("ME") + random(20) + 1;
    if(me_save >= 14) {
        write((string)target->query_cap_name() + " resists the slowing magic.\n");
        return;
    }
    target->set_property("slowed", 1);
    write("Magic drags at " + (string)target->query_cap_name() + "'s limbs.\n");
    tell_object(target, "Your movements feel heavy and sluggish!\n");
    call_out("end_slow_movement", 60, target);
}

void end_slow_movement(object who) {
    if(!objectp(who)) return;
    who->remove_property("slowed");
    tell_object(who, "The slowing magic wears off.\n");
}

private void fx_detect_poison(object target) {
    if(!target) {
        write("Detect poison on what?  Syntax: cast detect poison at <target>\n");
        return;
    }
    if((int)target->query_property("poisoned")) {
        write("You detect active poison in " + (string)target->query_short() + ".\n");
        return;
    }
    write("You detect no poison.\n");
}

private void fx_breath_of_life(object target) {
    int heal;
    int cur;

    if(!target || !living(target)) {
        write("Cast on whom?  Syntax: cast breath of life at <target>\n");
        return;
    }
    heal = random(6) + 1 + random(6) + 1;
    cur = (int)target->query_stats("rifts_hp");
    if(cur <= 0) {
        target->set_stats("rifts_hp", heal);
        target->set_hp(heal);
        write("Life returns to " + (string)target->query_cap_name() + "!\n");
        tell_object(target, "Warm life-force flows into you!\n");
        return;
    }
    cur = (int)target->query_stats("SDC");
    target->set_stats("SDC", cur + heal);
    write("You channel healing life-force into " +
        (string)target->query_cap_name() + ".\n");
}

private void fx_exorcism(object target) {
    int dmg;

    if(!target) {
        write("Cast at whom?  Syntax: cast exorcism at <target>\n");
        return;
    }
    if((int)target->query_property("undead") ||
       (string)target->query_race() == "secondary vampire" ||
       (string)target->query_race() == "wild vampire") {
        dmg = (random(6) + 1 + random(6) + 1) * 10;
        RIFTS_COMBAT_D->apply_rifts_damage(this_player(), target, dmg);
        write("Holy power sears the undead!\n");
        return;
    }
    if((int)target->query_stats("ME") + random(20) + 1 >= 14) {
        write((string)target->query_cap_name() + " resists the exorcism.\n");
        return;
    }
    target->remove_property("possessed");
    write("You banish any possessing influence from " +
        (string)target->query_cap_name() + ".\n");
}

private void fx_heavy_darkness(object target) {
    object room;

    room = environment(this_player());
    if(!room) return;
    room->set_property("heavy_darkness", 1);
    room->set_property("light", 0);
    write("Magical darkness swallows the room.\n");
    say((string)this_player()->query_cap_name() +
        " plunges the room into heavy darkness!");
    call_out("end_heavy_darkness", 300, room);
}

void end_heavy_darkness(object room) {
    if(!objectp(room)) return;
    room->remove_property("heavy_darkness");
    if(!(int)room->query_property("light"))
        room->set_property("light", 1);
    tell_room(room, "The magical darkness lifts.\n", ({}));
}

private void fx_words_truth(object target) {
    if(!target || !living(target)) {
        write("Cast on whom?  Syntax: cast words of truth at <target>\n");
        return;
    }
    if((int)target->query_stats("ME") + random(20) + 1 >= 14) {
        write((string)target->query_cap_name() + " resists your compulsion.\n");
        return;
    }
    target->set_property("words_of_truth", 1);
    tell_object(target, "You feel compelled to speak only the truth.\n");
    call_out("end_words_truth", 120, target);
}

void end_words_truth(object who) {
    if(!objectp(who)) return;
    who->remove_property("words_of_truth");
    tell_object(who, "The compulsion to speak truth fades.\n");
}

private void fx_fireball(object target) {
    int damage;
    int i;
    int cur_mdc;
    int cur_hp;

    if(!target) {
        write("Cast at whom?  Syntax: cast fire ball at <target>\n");
        return;
    }
    if((int)target->query_property("impervious_fire")) {
        write("The fire ball washes over " + (string)target->query_cap_name() +
            " harmlessly.\n");
        tell_object(target, "Flames curl around you without effect.\n");
        return;
    }
    damage = 0;
    for(i = 0; i < 4; i++) damage += random(6) + 1;
    write("A blazing ball of fire erupts from your hands! " + damage + " MDC!\n");
    tell_object(target,
        this_player()->query_cap_name() + " hurls a fire ball at you!\n");
    say(this_player()->query_cap_name() + " unleashes a fire ball at " +
        (string)target->query_cap_name() + "!");
    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        cur_mdc = (int)target->query_stats("MDC");
        cur_mdc -= damage;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= -20) catch(target->die());
    } else {
        cur_hp = (int)target->query_hp() - damage;
        if(cur_hp < 0) { target->set_hp(0); catch(target->die()); }
        else target->set_hp(cur_hp);
    }
}

private void fx_adhesion(object target) {
    object *contents;
    int i;
    int stuck;
    object room;

    room = environment(this_player());
    if(!room) return;
    write("The floor becomes a sticky mass of magical adhesive!\n");
    say(this_player()->query_cap_name() + " coats the floor in magical adhesive!");
    room->set_property("adhesion_floor", 1);
    contents = all_inventory(room);
    stuck = 0;
    for(i = 0; i < sizeof(contents); i++) {
        if(!living(contents[i])) continue;
        if(contents[i] == this_player()) continue;
        if((int)contents[i]->query_stats("ME") + random(20) + 1 >= 14) {
            tell_object(contents[i], "You resist the carpet of adhesion!\n");
            continue;
        }
        contents[i]->set_property("stuck_adhesion", 1);
        tell_object(contents[i],
            "The floor holds your feet fast! You are stuck!\n");
        call_out("end_adhesion_one", 120, contents[i]);
        stuck++;
    }
    call_out("end_adhesion_room", 120, room);
    if(stuck > 0) write(stuck + " beings are stuck to the floor.\n");
}

void end_adhesion_one(object who) {
    if(!objectp(who)) return;
    who->set_property("stuck_adhesion", 0);
    tell_object(who, "You pull free from the adhesion.\n");
}

void end_adhesion_room(object room) {
    if(!objectp(room)) return;
    room->set_property("adhesion_floor", 0);
    tell_room(room, "The magical adhesive dissolves from the floor.\n");
}

private void fx_invisible(object target) {
    if((int)this_player()->query_property("invisible_spell")) {
        write("You are already invisible.\n");
        return;
    }
    this_player()->set_property("invisible_spell", 1);
    write("You fade from sight completely.\n");
    say(this_player()->query_cap_name() + " shimmers and vanishes!");
    call_out("end_invisible", 180, this_player());
}

void end_invisible(object who) {
    if(!objectp(who)) return;
    who->set_property("invisible_spell", 0);
    tell_object(who, "Your invisibility fades.\n");
    tell_room(environment(who),
        who->query_cap_name() + " shimmers back into visibility.\n",
        ({ who }));
}

private void fx_firebolt(object target) {
    int damage;
    int i;
    int cur_mdc;
    int cur_hp;

    if(!target) {
        write("Cast at whom?  Syntax: cast fire bolt at <target>\n");
        return;
    }
    if((int)target->query_property("impervious_fire")) {
        write("Your fire bolt fails to harm " + (string)target->query_cap_name() + ".\n");
        tell_object(target, "Flames curl around you without effect.\n");
        return;
    }
    damage = 0;
    for(i = 0; i < 5; i++) damage += random(6) + 1;
    write("A lance of fire streaks from your hand! " + damage + " MDC!\n");
    tell_object(target,
        this_player()->query_cap_name() + " hurls a fire bolt at you!\n");
    say(this_player()->query_cap_name() + " hurls a fire bolt at " +
        (string)target->query_cap_name() + "!");
    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        cur_mdc = (int)target->query_stats("MDC");
        cur_mdc -= damage;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= -20) catch(target->die());
    } else {
        cur_hp = (int)target->query_hp() - damage;
        if(cur_hp < 0) { target->set_hp(0); catch(target->die()); }
        else target->set_hp(cur_hp);
    }
}

/*  Since 2026-07-19 protective spells are barrier pools drained by
    the damage chain in rifts_combat.c, exactly like worn armor MDC.
    The property IS the remaining pool; the wearer's own MDC stat is
    never touched.  */
private void fx_ithan_armor(object target) {
    if((int)this_player()->query_property("ithan_armor")) {
        write("You already have an Armor of Ithan field.\n");
        return;
    }
    this_player()->set_property("ithan_armor", 30);
    write("A glowing magical field surrounds you, absorbing 30 MDC of damage!\n");
    say(this_player()->query_cap_name() + " is surrounded by a shimmering magical field!");
    call_out("end_ithan_armor", 300, this_player());
}

void end_ithan_armor(object who) {
    if(!objectp(who)) return;
    if((int)who->query_property("ithan_armor") < 1) {
        who->set_property("ithan_armor", 0);
        return;
    }
    who->set_property("ithan_armor", 0);
    tell_object(who, "Your Armor of Ithan field dissipates.\n");
}

private void fx_flame_circle(object target) {
    object room;
    room = environment(this_player());
    if(!room) return;
    room->set_property("flame_circle", 1);
    write("A ring of magical fire erupts around you!\n");
    say(this_player()->query_cap_name() + " is ringed by magical flame!");
    call_out("end_flame_circle", 120, room);
}

void end_flame_circle(object room) {
    if(!objectp(room)) return;
    room->set_property("flame_circle", 0);
    tell_room(room, "The circle of flame dies down.\n");
}

private void fx_spell_tk(object target) {
    string target_name;
    if(!target) {
        write("Move what?  Syntax: cast telekinesis at <target>\n");
        return;
    }
    target_name = (string)target->query_name();
    if(!target_name) target_name = "object";
    if(living(target)) {
        write("You push " + (string)target->query_cap_name() +
              " with telekinetic force!\n");
        tell_object(target,
            this_player()->query_cap_name() +
            " pushes you with invisible telekinetic force!\n");
        say(this_player()->query_cap_name() +
            " pushes " + (string)target->query_cap_name() + " telekinetically!");
    } else {
        write("You lift " + target_name + " with your mind and set it nearby.\n");
        say(this_player()->query_cap_name() + " levitates " + target_name +
            " telekinetically.");
    }
}

/* ── Level 4-8 spell handlers ──────────────────────────────────────────────── */

private void fx_energy_field(object target) {
    int cur_mdc;
    if((int)this_player()->query_property("energy_field_active")) {
        write("Your energy field is already active.\n");
        return;
    }
    this_player()->set_property("energy_field_active", 100);
    cur_mdc = (int)this_player()->query_stats("MDC");
    this_player()->set_stats("MDC", cur_mdc + 100);
    write("An invisible force barrier crackles to life around you: +100 MDC!\n");
    say(this_player()->query_cap_name() + " is shielded by an invisible energy field!");
    call_out("end_energy_field", 120, this_player());
}

void end_energy_field(object who) {
    int mdc_val;
    int field_val;
    if(!objectp(who)) return;
    field_val = (int)who->query_property("energy_field_active");
    who->set_property("energy_field_active", 0);
    mdc_val = (int)who->query_stats("MDC");
    if(mdc_val > field_val) mdc_val -= field_val;
    else mdc_val = 0;
    who->set_stats("MDC", mdc_val);
    tell_object(who, "Your energy field dissipates.\n");
}

private void fx_ignite_fire(object target) {
    object room;
    room = environment(this_player());
    if(!room) return;
    write("You concentrate and a burst of magical flame ignites nearby materials!\n");
    say(this_player()->query_cap_name() + " ignites a burst of magical fire!");
    room->set_property("fire_ignited", 1);
    call_out("end_ignite_fire", 60, room);
}

void end_ignite_fire(object room) {
    if(!objectp(room)) return;
    room->set_property("fire_ignited", 0);
    tell_room(room, "The magical flames flicker out.\n");
}

private void fx_multiple_image(object target) {
    int copies;
    if((int)this_player()->query_property("mirror_images")) {
        write("Your images are already active.\n");
        return;
    }
    copies = random(4) + 2;
    this_player()->set_property("mirror_images", copies);
    write("You split into " + copies + " identical copies! Attackers must guess the real you.\n");
    say(this_player()->query_cap_name() + " multiplies into " + copies + " identical images!");
    call_out("end_mirror_images", 240, this_player());
}

void end_mirror_images(object who) {
    if(!objectp(who)) return;
    who->set_property("mirror_images", 0);
    tell_object(who, "Your mirror images fade.\n");
    tell_room(environment(who), who->query_cap_name() + "'s illusions vanish.\n", ({ who }));
}

private void fx_shadow_meld(object target) {
    object room;
    int light;
    room = environment(this_player());
    if(!room) return;
    light = (int)room->query_property("light");
    if(light > 3) {
        write("There are no shadows to meld into here. You need dim light or darkness.\n");
        return;
    }
    if((int)this_player()->query_property("shadow_melded")) {
        write("You are already merged with the shadows.\n");
        return;
    }
    this_player()->set_property("shadow_melded", 1);
    write("You flow into the shadows, becoming nearly invisible in the dimness.\n");
    say(this_player()->query_cap_name() + " merges with the shadows!");
    call_out("end_shadow_meld", 120, this_player());
}

void end_shadow_meld(object who) {
    if(!objectp(who)) return;
    who->set_property("shadow_melded", 0);
    tell_object(who, "You emerge from the shadows.\n");
}

private void fx_mask_deceit(object target) {
    if((int)this_player()->query_property("mask_of_deceit")) {
        write("You are already wearing a mask of deceit.\n");
        return;
    }
    this_player()->set_property("mask_of_deceit", 1);
    write("Your face shimmers and reshapes into different features. Your voice changes to match.\n");
    say(this_player()->query_cap_name() + "'s features shift and blur...");
    call_out("end_mask_deceit", 600, this_player());
}

void end_mask_deceit(object who) {
    if(!objectp(who)) return;
    who->set_property("mask_of_deceit", 0);
    tell_object(who, "The mask of deceit fades. Your true face returns.\n");
}

private void fx_domination(object target) {
    int me;
    int roll;
    if(!target) {
        write("Dominate whom?  Syntax: cast domination at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Domination only works on living beings.\n");
        return;
    }
    me   = (int)target->query_stats("ME");
    roll = random(20) + 1 + (me >= 26 ? 3 : me >= 21 ? 2 : me >= 17 ? 1 : 0);
    if(roll >= 16) {
        tell_object(target, "A crushing mental force washes over you, but you resist!\n");
        write(target->query_cap_name() + " resists your domination!\n");
        return;
    }
    target->set_property("dominated", 1);
    write("Your will crushes " + (string)target->query_cap_name() + "! They obey your commands!\n");
    tell_object(target, this_player()->query_cap_name() + "'s will overwhelms your mind!\n");
    say(this_player()->query_cap_name() + " dominates " + (string)target->query_cap_name() + "!");
    call_out("end_domination", 60, target);
}

void end_domination(object who) {
    if(!objectp(who)) return;
    who->set_property("dominated", 0);
    tell_object(who, "The magical compulsion fades. Your will is your own again.\n");
}

private void fx_escape_spell(object target) {
    object room;
    string *exits;
    string dest;
    int idx;
    room  = environment(this_player());
    if(!room) return;
    exits = (string *)room->query_exits();
    if(!exits || sizeof(exits) < 1) {
        write("There is nowhere to teleport to.\n");
        return;
    }
    idx  = random(sizeof(exits));
    dest = (string)room->query_exit(exits[idx]);
    write("Reality tears around you and you vanish!\n");
    say(this_player()->query_cap_name() + " vanishes in a flash of magical energy!");
    this_player()->stop_fight();
    if(stringp(dest)) {
        this_player()->move(dest);
        write("You materialize 60 feet away, safe from immediate danger.\n");
    }
}

private void fx_fly_spell(object target) {
    if((int)this_player()->query_property("flying_magic")) {
        write("You are already flying by magical means.\n");
        return;
    }
    this_player()->set_property("flying_magic", 1);
    write("You rise from the ground on magical force. You can fly at 50 mph!\n");
    say(this_player()->query_cap_name() + " rises into the air on magical wings!");
    call_out("end_fly_spell", 300, this_player());
}

void end_fly_spell(object who) {
    if(!objectp(who)) return;
    who->set_property("flying_magic", 0);
    tell_object(who, "Your magical flight ends. You descend to the ground.\n");
}

private void fx_horrific_illusion(object target) {
    int me;
    int roll;
    if(!target) {
        write("Cast at whom?  Syntax: cast horrific illusion at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Horrific illusion only works on living beings.\n");
        return;
    }
    me   = (int)target->query_stats("ME");
    roll = random(20) + 1 + (me >= 26 ? 3 : me >= 21 ? 2 : me >= 17 ? 1 : 0);
    if(roll >= 14) {
        tell_object(target, "Terrible visions grip you for a moment, but you see through the illusion!\n");
        write(target->query_cap_name() + " resists the horrific illusion!\n");
        return;
    }
    target->set_property("terrified", 1);
    target->stop_fight();
    write("The illusion strikes " + (string)target->query_cap_name() + " with absolute horror!\n");
    tell_object(target, "Nightmarish visions overwhelm you! You must flee!\n");
    say(this_player()->query_cap_name() + " drives " + (string)target->query_cap_name() + " into blind panic!");
    call_out("end_horrific_illusion", 240, target);
}

void end_horrific_illusion(object who) {
    if(!objectp(who)) return;
    who->set_property("terrified", 0);
    tell_object(who, "The horrific visions fade from your mind.\n");
}

private void fx_impervious_energy(object target) {
    if((int)this_player()->query_property("impervious_energy")) {
        write("You are already impervious to energy attacks.\n");
        return;
    }
    this_player()->set_property("impervious_energy", 1);
    write("A magical field envelops you. Lasers, plasma, and electricity cannot harm you!\n");
    say(this_player()->query_cap_name() + " becomes impervious to energy attacks!");
    call_out("end_impervious_energy", 120, this_player());
}

void end_impervious_energy(object who) {
    if(!objectp(who)) return;
    who->set_property("impervious_energy", 0);
    tell_object(who, "Your energy immunity fades.\n");
}

private void fx_metamorphosis(object target) {
    if((int)this_player()->query_property("metamorphed")) {
        write("You are already in animal form.\n");
        return;
    }
    this_player()->set_property("metamorphed", 1);
    write("Your body flows and reshapes. You transform into an animal form!\n");
    say(this_player()->query_cap_name() + " transforms into an animal!");
    call_out("end_metamorphosis", 600, this_player());
}

void end_metamorphosis(object who) {
    if(!objectp(who)) return;
    who->set_property("metamorphed", 0);
    tell_object(who, "You flow back to your natural form.\n");
    tell_room(environment(who), who->query_cap_name() + " returns to normal form.\n", ({ who }));
}

private void fx_call_lightning(object target) {
    int damage;
    int cur_mdc;
    int cur_hp;
    object room;
    if(!target) {
        write("Call lightning at whom?  Syntax: cast call lightning at <target>\n");
        return;
    }
    room = environment(this_player());
    if(room && !(int)room->query_property("outdoors") &&
       !(int)room->query_property("open_sky")) {
        write("There is no open sky above. Call lightning requires outdoor conditions.\n");
        return;
    }
    damage = (random(6) + 1) * 10;
    write("Storm clouds gather! A bolt of lightning crashes down! " + damage + " MDC!\n");
    tell_object(target, this_player()->query_cap_name() + " calls lightning down upon you!\n");
    say(this_player()->query_cap_name() + " summons a bolt of lightning!");
    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        cur_mdc = (int)target->query_stats("MDC") - damage;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= -20) catch(target->die());
    } else {
        cur_hp = (int)target->query_hp() - damage;
        if(cur_hp < 0) { target->set_hp(0); catch(target->die()); }
        else target->set_hp(cur_hp);
    }
}

private void fx_freeze_spell(object target) {
    int damage;
    int i;
    int cur_mdc;
    int cur_hp;
    int pe;
    int roll;
    if(!target) {
        write("Freeze whom?  Syntax: cast freeze at <target>\n");
        return;
    }
    if((int)target->query_property("impervious_cold")) {
        write("The cold fails to harm " + (string)target->query_cap_name() + ".\n");
        tell_object(target, "Ice and frost slide off you without effect.\n");
        return;
    }
    damage = 0;
    for(i = 0; i < 2; i++) damage += random(6) + 1;
    write("A wave of intense cold sweeps over " + (string)target->query_cap_name() +
          "! " + damage + " MDC!\n");
    tell_object(target, this_player()->query_cap_name() + " encases you in ice!\n");
    say(this_player()->query_cap_name() + " freezes " + (string)target->query_cap_name() + " solid!");
    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        cur_mdc = (int)target->query_stats("MDC") - damage;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= -20) catch(target->die());
    } else {
        cur_hp = (int)target->query_hp() - damage;
        if(cur_hp < 0) { target->set_hp(0); catch(target->die()); }
        else target->set_hp(cur_hp);
    }
    pe   = (int)target->query_stats("PE");
    roll = random(20) + 1 + (pe >= 16 ? 2 : pe >= 12 ? 1 : 0);
    if(roll < 16) {
        target->set_property("frozen", 1);
        target->set_property("cannot_move", 1);
        tell_object(target, "Ice encases your limbs! You cannot move!\n");
        call_out("end_freeze", 60, target);
    }
}

void end_freeze(object who) {
    if(!objectp(who)) return;
    who->set_property("frozen", 0);
    who->set_property("cannot_move", 0);
    tell_object(who, "The ice melts away. You can move again.\n");
}

private void fx_negate_mechanics(object target) {
    if(!target) {
        write("Negate what machine?  Syntax: cast negate mechanics at <target>\n");
        return;
    }
    target->set_property("systems_down", 1);
    write("The machine's systems shut down completely!\n");
    tell_object(target, "Your mechanical systems go dead!\n");
    say(this_player()->query_cap_name() + " shuts down all mechanical systems in " +
        (string)target->query_short() + "!");
    call_out("end_negate_mechanics", 60, target);
}

void end_negate_mechanics(object who) {
    if(!objectp(who)) return;
    who->set_property("systems_down", 0);
    tell_object(who, "Your systems come back online.\n");
}

private void fx_restoration_spell(object target) {
    int heal_hp;
    int heal_sdc;
    int heal_mdc;
    int i;
    int level;
    int cur_hp;
    int max_hp;
    int cur_sdc;
    int max_sdc;
    int cur_mdc;
    int max_mdc;
    int new_val;
    if(!target) target = this_player();
    level    = (int)this_player()->query_level();
    if(level < 1) level = 1;
    heal_hp  = 0;
    heal_sdc = 0;
    for(i = 0; i < 2; i++) heal_hp  += random(6) + 1;
    for(i = 0; i < 2; i++) heal_sdc += random(6) + 1;
    heal_mdc = level;
    cur_mdc  = (int)target->query_stats("MDC");
    max_mdc  = (int)target->query_stats("max_MDC");
    if(max_mdc > 0 && cur_mdc < max_mdc) {
        new_val = cur_mdc + heal_mdc;
        if(new_val > max_mdc) new_val = max_mdc;
        target->set_stats("MDC", new_val);
        write("Restoration magic heals " + (string)target->query_cap_name() + ". +" + heal_mdc + " MDC.\n");
        if(target != this_player())
            tell_object(target, "Restoration magic heals you. +" + heal_mdc + " MDC.\n");
        return;
    }
    cur_sdc = (int)target->query_stats("SDC");
    max_sdc = (int)target->query_stats("max_SDC");
    if(max_sdc > 0 && cur_sdc < max_sdc) {
        new_val = cur_sdc + heal_sdc;
        if(new_val > max_sdc) new_val = max_sdc;
        target->set_stats("SDC", new_val);
        write("Restoration magic heals " + (string)target->query_cap_name() + ". +" + heal_sdc + " SDC.\n");
        if(target != this_player())
            tell_object(target, "Restoration magic heals you. +" + heal_sdc + " SDC.\n");
        return;
    }
    cur_hp = (int)target->query_stats("rifts_hp");
    max_hp = (int)target->query_stats("max_rifts_hp");
    new_val = cur_hp + heal_hp;
    if(new_val > max_hp) new_val = max_hp;
    target->set_stats("rifts_hp", new_val);
    write("Restoration magic heals " + (string)target->query_cap_name() + ". +" + heal_hp + " HP.\n");
    if(target != this_player())
        tell_object(target, "Restoration magic heals you. +" + heal_hp + " HP.\n");
}

private void fx_shadow_bolt(object target) {
    int damage;
    int i;
    int cur_mdc;
    int cur_hp;
    if(!target) {
        write("Cast at whom?  Syntax: cast shadow bolt at <target>\n");
        return;
    }
    damage = 0;
    for(i = 0; i < 3; i++) damage += random(6) + 1;
    write("A bolt of writhing shadow energy tears through the air! " + damage + " MDC!\n");
    tell_object(target, this_player()->query_cap_name() + " hurls shadow energy at you!\n");
    say(this_player()->query_cap_name() + " launches a shadow bolt at " +
        (string)target->query_cap_name() + "!");
    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        cur_mdc = (int)target->query_stats("MDC") - damage;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= -20) catch(target->die());
    } else {
        cur_hp = (int)target->query_hp() - damage;
        if(cur_hp < 0) { target->set_hp(0); catch(target->die()); }
        else target->set_hp(cur_hp);
    }
}

private void fx_time_slip(object target) {
    if((int)this_player()->query_property("time_slipped")) {
        write("You are already phased out of time.\n");
        return;
    }
    this_player()->set_property("time_slipped", 1);
    write("You phase slightly out of normal time. Physical attacks pass through you!\n");
    say(this_player()->query_cap_name() + " shimmers and becomes slightly translucent!");
    call_out("end_time_slip", 30, this_player());
}

void end_time_slip(object who) {
    if(!objectp(who)) return;
    who->set_property("time_slipped", 0);
    tell_object(who, "You snap back into normal time.\n");
    tell_room(environment(who), who->query_cap_name() + " snaps back into normal time.\n", ({ who }));
}

private void fx_cloud_of_fear(object target) {
    object *contents;
    int i;
    int max_targets;
    int panicked;
    int me;
    int roll;
    write("A terrifying cloud of magical fear erupts from your hands!\n");
    say(this_player()->query_cap_name() + " unleashes a cloud of pure fear!");
    contents    = all_inventory(environment(this_player()));
    max_targets = random(4) + random(4) + 2;
    panicked    = 0;
    for(i = 0; i < sizeof(contents) && panicked < max_targets; i++) {
        if(!living(contents[i])) continue;
        if(contents[i] == this_player()) continue;
        me   = (int)contents[i]->query_stats("ME");
        roll = random(20) + 1 + (me >= 26 ? 3 : me >= 21 ? 2 : me >= 17 ? 1 : 0);
        if(roll >= 14) {
            tell_object(contents[i], "An aura of dread surrounds you, but you resist!\n");
            continue;
        }
        contents[i]->set_property("panicking", 1);
        contents[i]->stop_fight();
        tell_object(contents[i], "Overwhelming fear grips your mind! You must flee!\n");
        call_out("end_fear", 240, contents[i]);
        panicked++;
    }
    if(panicked > 0)
        write(panicked + " beings flee in terror!\n");
    else
        write("None of your enemies succumb to fear.\n");
}

void end_fear(object who) {
    if(!objectp(who)) return;
    who->remove_property("feared");
    who->set_property("panicking", 0);
    tell_object(who, "The magical fear fades. Your courage returns.\n");
}

private void fx_dispel_barrier(object target) {
    object room;
    room = environment(this_player());
    if(!room) return;
    room->set_property("magic_barrier", 0);
    room->set_property("ward_active", 0);
    room->set_property("protection_circle", 0);
    write("A pulse of dispelling magic sweeps the area. Magical barriers dissolve!\n");
    say(this_player()->query_cap_name() + " dispels the magical barriers here!");
}

/*  Barrier pool, same model as Armor of Ithan (see note there).  */
private void fx_invincible_armor(object target) {
    if((int)this_player()->query_property("invincible_armor")) {
        write("Your invincible armor is already active.\n");
        return;
    }
    this_player()->set_property("invincible_armor", 200);
    write("A crackling shell of magical force surrounds you, absorbing 200 MDC of damage!\n");
    say(this_player()->query_cap_name() + " is enclosed in crackling invincible magical armor!");
    call_out("end_invincible_armor", 300, this_player());
}

void end_invincible_armor(object who) {
    if(!objectp(who)) return;
    if((int)who->query_property("invincible_armor") < 1) {
        who->set_property("invincible_armor", 0);
        return;
    }
    who->set_property("invincible_armor", 0);
    tell_object(who, "Your invincible armor field collapses.\n");
}

private void fx_mystic_portal(object target) {
    if((int)this_player()->query_property("portal_active")) {
        write("You already have an active portal.\n");
        return;
    }
    this_player()->set_property("portal_active", 1);
    write("You stretch the fabric of space, tearing open a shimmering doorway.\n"
          "A gateway between two points appears before you.\n");
    say(this_player()->query_cap_name() + " tears open a shimmering mystic portal!");
    call_out("end_mystic_portal", 120, this_player());
}

void end_mystic_portal(object who) {
    if(!objectp(who)) return;
    who->set_property("portal_active", 0);
    tell_object(who, "The mystic portal closes.\n");
    if(objectp(environment(who)))
        tell_room(environment(who), "A shimmering portal collapses and vanishes.\n", ({ who }));
}

private void fx_protection_circle(object target) {
    object room;
    room = environment(this_player());
    if(!room) return;
    room->set_property("protection_circle", 1);
    write("You inscribe a glowing circle. Supernatural beings cannot enter!\n");
    say(this_player()->query_cap_name() + " creates a glowing circle of protection!");
    call_out("end_protection_circle", 600, room);
}

void end_protection_circle(object room) {
    if(!objectp(room)) return;
    room->set_property("protection_circle", 0);
    tell_room(room, "The protection circle fades.\n");
}

private void fx_energy_drain(object target) {
    int drain_ppe;
    int i;
    int target_ppe;
    int caster_ppe;
    int max_caster_ppe;
    int pe;
    int roll;
    int new_val;
    if(!target) {
        write("Drain whom?  Syntax: cast energy drain at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Energy drain only works on living beings.\n");
        return;
    }
    pe   = (int)target->query_stats("PE");
    roll = random(20) + 1 + (pe >= 16 ? 2 : pe >= 12 ? 1 : 0);
    if(roll >= 15) {
        tell_object(target, "You feel your energy surge away, then snap back as you resist!\n");
        write(target->query_cap_name() + " resists the energy drain!\n");
        return;
    }
    drain_ppe = 0;
    for(i = 0; i < 2; i++) drain_ppe += random(6) + 1;
    target_ppe = (int)target->query_stats("PPE");
    if(target_ppe < drain_ppe) drain_ppe = target_ppe;
    target->set_stats("PPE", target_ppe - drain_ppe);
    caster_ppe     = (int)this_player()->query_stats("PPE");
    max_caster_ppe = (int)this_player()->query_stats("max_PPE");
    new_val = caster_ppe + drain_ppe;
    if(new_val > max_caster_ppe) new_val = max_caster_ppe;
    this_player()->set_stats("PPE", new_val);
    write("You drain " + drain_ppe + " PPE from " + (string)target->query_cap_name() + "!\n");
    tell_object(target, this_player()->query_cap_name() +
        " drains your magical energy! -" + drain_ppe + " PPE!\n");
    say(this_player()->query_cap_name() + " drains magical energy from " +
        (string)target->query_cap_name() + "!");
}

private void fx_eyes_of_thoth(object target) {
    if((int)this_player()->query_property("eyes_of_thoth")) {
        write("You can already read all languages.\n");
        return;
    }
    this_player()->set_property("eyes_of_thoth", 1);
    write("Knowledge floods your mind. You can now read any language, inscription, or alien text!\n");
    say(this_player()->query_cap_name() + "'s eyes glow with the light of Thoth!");
    call_out("end_eyes_of_thoth", 3600, this_player());
}

void end_eyes_of_thoth(object who) {
    if(!objectp(who)) return;
    who->set_property("eyes_of_thoth", 0);
    tell_object(who, "The magical comprehension of all languages fades.\n");
}

private void fx_locate_spell(object target) {
    if(!target) {
        write("Locate whom?  Syntax: cast locate at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Locate only works on living beings.\n");
        return;
    }
    write("Your magical senses lock onto " + (string)target->query_cap_name() + ".\n");
    if(objectp(environment(target)))
        write("They are in: " + (string)environment(target)->query_short() + "\n");
    else
        write("Their location cannot be determined.\n");
}

private void fx_mass_confusion(object target) {
    object *contents;
    int i;
    int confused;
    int iq;
    int roll;
    write("Swirling magical energy clouds the minds of all nearby!\n");
    say(this_player()->query_cap_name() + " releases a cloud of mass confusion!");
    contents = all_inventory(environment(this_player()));
    confused = 0;
    for(i = 0; i < sizeof(contents); i++) {
        if(!living(contents[i])) continue;
        if(contents[i] == this_player()) continue;
        iq   = (int)contents[i]->query_stats("IQ");
        roll = random(20) + 1 + (iq >= 16 ? 2 : iq >= 12 ? 1 : 0);
        if(roll >= 15) {
            tell_object(contents[i], "A wave of confusion washes over you, but you shake it off!\n");
            continue;
        }
        contents[i]->set_property("confused", 1);
        tell_object(contents[i], "Your mind goes blank! You wander aimlessly!\n");
        call_out("end_confusion", 240, contents[i]);
        confused++;
    }
    if(confused > 0)
        write(confused + " beings are confused and wandering aimlessly!\n");
    else
        write("Everyone resists the confusion.\n");
}

void end_confusion(object who) {
    if(!objectp(who)) return;
    who->set_property("confused", 0);
    tell_object(who, "Your mind clears.\n");
}

private void fx_negate_magic(object target) {
    int mdc_val;
    int armor_val;
    if(!target) {
        write("Negate magic on whom?  Syntax: cast negate magic at <target>\n");
        return;
    }
    if((int)target->query_property("ithan_armor")) {
        /* Barrier pool model: clearing the property removes the field. */
        target->set_property("ithan_armor", 0);
        write("The Armor of Ithan on " + (string)target->query_cap_name() + " is negated!\n");
        tell_object(target, "Your magical armor is negated!\n");
        return;
    }
    if((int)target->query_property("invincible_armor")) {
        target->set_property("invincible_armor", 0);
        write("The Invincible Armor on " + (string)target->query_cap_name() + " is negated!\n");
        tell_object(target, "Your invincible armor is shattered!\n");
        return;
    }
    if((int)target->query_property("invisible_spell")) {
        target->set_property("invisible_spell", 0);
        write("The invisibility on " + (string)target->query_cap_name() + " is negated!\n");
        tell_object(target, "Your invisibility fails!\n");
        return;
    }
    write("The magic on " + (string)target->query_cap_name() +
          " resists negation, or no active magic was detected.\n");
}

private void fx_rift_walk(object target) {
    object room;
    room = environment(this_player());
    if(!room) return;
    if(!(int)RIFTS_D->room_on_ley_line(room) &&
       !(int)room->query_property("rift_active")) {
        write("You are not near an active ley line or rift. "
              "Rift Walk requires a ley line within one mile.\n");
        return;
    }
    write("You step into the ley line and reality tears apart around you.\n"
          "In a moment of infinite darkness you are transported to your destination.\n");
    say(this_player()->query_cap_name() + " steps into the ley line and vanishes in a blaze of light!");
}

/* ── Level 9 spell handlers ────────────────────────────────────────────────── */

private void fx_meta_superior(object target) {
    int level;
    int dur;
    if((int)this_player()->query_property("metamorphed") ||
       (int)this_player()->query_property("meta_superior")) {
        write("You are already transformed.\n");
        return;
    }
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur = level * 600;
    this_player()->set_property("meta_superior", 1);
    write("Your body becomes fluid, reshaping into a large and powerful creature form!\n"
          "You can become any creature up to 10 times your own size.\n"
          "You retain all psionic abilities in this form.\n");
    say(this_player()->query_cap_name() + " undergoes a powerful metamorphosis!");
    call_out("end_meta_superior", dur, this_player());
}

void end_meta_superior(object who) {
    if(!objectp(who)) return;
    who->set_property("meta_superior", 0);
    tell_object(who, "The superior metamorphosis fades. You return to your natural form.\n");
    tell_room(environment(who),
        who->query_cap_name() + " reverts to normal form.\n", ({ who }));
}

private void fx_create_zombie(object target) {
    int level;
    int max_zombies;
    int cur_zombies;
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    max_zombies = level / 2;
    if(max_zombies < 1) max_zombies = 1;
    cur_zombies = (int)this_player()->query_property("zombie_count");
    if(cur_zombies >= max_zombies) {
        write("You already control the maximum number of zombies for your level (" +
              max_zombies + ").\n");
        return;
    }
    this_player()->set_property("zombie_count", cur_zombies + 1);
    write("You channel necromantic energy into the corpse. It shudders and rises!\n"
          "A mindless zombie servant stands ready to obey your commands.\n"
          "You now control " + (cur_zombies + 1) + " of " + max_zombies + " zombie(s).\n");
    say(this_player()->query_cap_name() + " animates a corpse! A zombie rises!");
}

private void fx_amulet_spell(object target) {
    if(!target) {
        write("Enchant what? Syntax: cast amulet at <object in inventory>\n");
        return;
    }
    if(living(target)) {
        write("Amulet can only enchant objects, not living beings.\n");
        return;
    }
    if((int)target->query_property("amulet_enchanted")) {
        write("That object is already enchanted.\n");
        return;
    }
    target->set_property("amulet_enchanted", 1);
    target->set_property("save_bonus", 2);
    write("You inscribe glowing runes of protection on " + (string)target->query_short() +
          ".\nIt shimmers briefly. +2 to saves while carried.\n");
    say(this_player()->query_cap_name() + " enchants " + (string)target->query_short() +
        " as a protective amulet!");
}

private void fx_summon_fog(object target) {
    object room;
    int level;
    int dur;
    room = environment(this_player());
    if(!room) return;
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur = level * 600;
    if((int)room->query_property("magical_fog")) {
        write("Magical fog already fills this area.\n");
        return;
    }
    room->set_property("magical_fog", 1);
    write("Thick magical fog rolls in from nowhere, cutting visibility to near zero!\n");
    say(this_player()->query_cap_name() + " summons a blinding magical fog!");
    call_out("end_summon_fog", dur, room);
}

void end_summon_fog(object room) {
    if(!objectp(room)) return;
    room->set_property("magical_fog", 0);
    tell_room(room, "The magical fog slowly lifts.\n");
}

private void fx_teleport_spell(object target) {
    string dest;
    int level;
    int fail_chance;
    int roll;
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    fail_chance = 10 - level;
    if(fail_chance < 1) fail_chance = 1;
    roll = random(100) + 1;
    dest = (string)this_player()->getenv("recall_point");
    if(!dest || !sizeof(dest)) dest = "/domains/Praxis/praxis";
    write("Reality tears around you. In a flash of light, you vanish!\n");
    say(this_player()->query_cap_name() + " vanishes in a flash of teleportation!");
    this_player()->stop_fight();
    this_player()->move(dest);
    if(roll <= fail_chance)
        write("The teleportation goes slightly wrong! You materialize a few feet off target.\n");
    else
        write("You materialize at your destination.\n");
}

private void fx_energy_disruption(object target) {
    object room;
    object *contents;
    int i;
    int dur;
    room = environment(this_player());
    if(!room) return;
    dur = (random(6) + 1) * 15;
    room->set_property("energy_disrupted", 1);
    write("A pulse of disruption energy ripples through the area! "
          "Electronic and energy systems fail!\n");
    say(this_player()->query_cap_name() + " unleashes a pulse of energy disruption!");
    contents = all_inventory(room);
    for(i = 0; i < sizeof(contents); i++) {
        if(!living(contents[i])) continue;
        if(contents[i] == this_player()) continue;
        contents[i]->set_property("systems_disrupted", 1);
        tell_object(contents[i], "Your electronic systems flicker and fail!\n");
        call_out("end_disruption_one", dur, contents[i]);
    }
    call_out("end_energy_disruption", dur, room);
}

void end_disruption_one(object who) {
    if(!objectp(who)) return;
    who->set_property("systems_disrupted", 0);
    tell_object(who, "Your electronic systems come back online.\n");
}

void end_energy_disruption(object room) {
    if(!objectp(room)) return;
    room->set_property("energy_disrupted", 0);
    tell_room(room, "The energy disruption fades. Electronic systems come back online.\n");
}

/* ── Level 10 spell handlers ────────────────────────────────────────────────── */

private void fx_life_drain(object target) {
    int pe;
    int roll;
    int i;
    int drain;
    int cur_hp;
    int new_hp;
    int caster_hp;
    int caster_max;
    int gain;
    if(!target) {
        write("Drain whom? Syntax: cast life drain at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Life drain only works on living beings.\n");
        return;
    }
    pe   = (int)target->query_stats("PE");
    roll = random(20) + 1 + (pe >= 16 ? 2 : pe >= 12 ? 1 : 0);
    if(roll >= 16) {
        tell_object(target, "A cold sensation passes through you, but you resist!\n");
        write(target->query_cap_name() + " resists the life drain!\n");
        return;
    }
    drain = 0;
    for(i = 0; i < 4; i++) drain += random(6) + 1;
    cur_hp = (int)target->query_stats("rifts_hp");
    new_hp = cur_hp - drain;
    if(new_hp < 0) new_hp = 0;
    target->set_stats("rifts_hp", new_hp);
    gain       = drain / 2;
    caster_hp  = (int)this_player()->query_stats("rifts_hp");
    caster_max = (int)this_player()->query_stats("max_rifts_hp");
    new_hp = caster_hp + gain;
    if(new_hp > caster_max) new_hp = caster_max;
    this_player()->set_stats("rifts_hp", new_hp);
    write("You drain " + drain + " HP from " + (string)target->query_cap_name() +
          "! You gain " + gain + " HP.\n");
    tell_object(target, this_player()->query_cap_name() +
        " drains your life force! -" + drain + " HP!\n");
    say(this_player()->query_cap_name() + " drains life force from " +
        (string)target->query_cap_name() + "!");
    if(new_hp <= 0) catch(target->die());
}

private void fx_circle_superior(object target) {
    object room;
    int level;
    int dur;
    room = environment(this_player());
    if(!room) return;
    if((int)room->query_property("circle_superior")) {
        write("A superior circle of protection is already active here.\n");
        return;
    }
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur = level * 1800;
    room->set_property("circle_superior", 1);
    room->set_property("protection_circle", 1);
    write("You inscribe a glowing circle of supreme protection.\n"
          "No supernatural being may enter or leave. Magic attacks from outside are blocked!\n");
    say(this_player()->query_cap_name() + " creates a superior circle of protection!");
    call_out("end_circle_superior", dur, room);
}

void end_circle_superior(object room) {
    if(!objectp(room)) return;
    room->set_property("circle_superior", 0);
    room->set_property("protection_circle", 0);
    tell_room(room, "The superior circle of protection fades.\n");
}

private void fx_create_scroll(object target) {
    string spell_name;
    if(!target) {
        write("Encode onto what? Syntax: cast create magic scroll at <parchment>\n");
        return;
    }
    if(living(target)) {
        write("Scrolls must be made from parchment or paper, not living beings.\n");
        return;
    }
    if((int)target->query_property("magic_scroll")) {
        write("That is already a magic scroll.\n");
        return;
    }
    spell_name = (string)this_player()->getenv("last_spell_cast");
    if(!spell_name || !sizeof(spell_name)) spell_name = "unknown spell";
    target->set_property("magic_scroll", 1);
    target->set_property("scroll_spell", spell_name);
    write("You channel magical knowledge into the parchment.\n"
          "It glows briefly as the spell is encoded.\n"
          "The scroll can now be used by anyone to cast: " + spell_name + "\n");
    say(this_player()->query_cap_name() + " creates a magic scroll!");
}

private void fx_fly_eagle(object target) {
    object who;
    int level;
    int dur;
    if(!target) who = this_player();
    else who = target;
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur = level * 1800;
    if((int)who->query_property("flying_eagle")) {
        write((who == this_player() ? "You are" :
               (string)who->query_cap_name() + " is") +
              " already flying like an eagle.\n");
        return;
    }
    who->set_property("flying_eagle", 1);
    who->set_property("flying_magic", 1);
    if(who == this_player()) {
        write("Eagle-like wings of magical force carry you upward at 100 mph!\n");
        say(this_player()->query_cap_name() + " soars upward on magical wings!");
    } else {
        write((string)who->query_cap_name() + " is lifted on magical eagle wings!\n");
        tell_object(who, "Magical wings carry you upward at 100 mph!\n");
        say(this_player()->query_cap_name() + " grants " +
            (string)who->query_cap_name() + " the gift of flight!");
    }
    call_out("end_fly_eagle", dur, who);
}

void end_fly_eagle(object who) {
    if(!objectp(who)) return;
    who->set_property("flying_eagle", 0);
    who->set_property("flying_magic", 0);
    tell_object(who, "The eagle flight magic fades. You descend to the ground.\n");
}

private void fx_turn_dead_sup(object target) {
    object *contents;
    int i;
    int level;
    int max_destroyed;
    int destroyed;
    int me;
    int roll;
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    max_destroyed = 0;
    for(i = 0; i < level; i++) max_destroyed += random(6) + 1;
    write("You channel divine power against the undead!\n");
    say(this_player()->query_cap_name() + " channels power to destroy the undead!");
    contents  = all_inventory(environment(this_player()));
    destroyed = 0;
    for(i = 0; i < sizeof(contents) && destroyed < max_destroyed; i++) {
        if(!living(contents[i])) continue;
        if(contents[i] == this_player()) continue;
        if(!(int)contents[i]->query_property("undead")) continue;
        me   = (int)contents[i]->query_stats("ME");
        roll = random(20) + 1 + (me >= 26 ? 3 : me >= 21 ? 2 : me >= 17 ? 1 : 0);
        if((int)contents[i]->query_property("powerful_undead") && roll >= 14) {
            tell_object(contents[i], "Holy power washes over you, but you resist!\n");
            continue;
        }
        tell_object(contents[i], "Divine energy tears through your undead form!\n");
        catch(contents[i]->die());
        destroyed++;
    }
    if(destroyed > 0)
        write(destroyed + " undead destroyed!\n");
    else
        write("No undead here were destroyed.\n");
}

private void fx_impervious_mdce(object target) {
    int level;
    int dur;
    if((int)this_player()->query_property("impervious_mdce")) {
        write("You are already impervious to MDC energy attacks.\n");
        return;
    }
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur = level * 120;
    this_player()->set_property("impervious_mdce", 1);
    write("A shimmering field envelops you. MDC energy weapons and magical energy "
          "pass through you harmlessly!\n");
    say(this_player()->query_cap_name() + " becomes impervious to MDC energy attacks!");
    call_out("end_impervious_mdce", dur, this_player());
}

void end_impervious_mdce(object who) {
    if(!objectp(who)) return;
    who->set_property("impervious_mdce", 0);
    tell_object(who, "Your MDC energy immunity fades.\n");
}

/* ── Level 11 spell handlers ────────────────────────────────────────────────── */

private void fx_dim_envelope(object target) {
    int level;
    int dur;
    int capacity;
    if(!target) {
        write("Enchant what object? Syntax: cast dimensional envelope at <container>\n");
        return;
    }
    if(living(target)) {
        write("Dimensional envelope can only be applied to objects.\n");
        return;
    }
    if((int)target->query_property("dim_envelope")) {
        write("That object already has a dimensional envelope.\n");
        return;
    }
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur      = level * 86400;
    capacity = level * 30;
    target->set_property("dim_envelope", 1);
    target->set_property("dim_envelope_capacity", capacity);
    write("You fold dimensional space around " + (string)target->query_short() +
          ".\nIt now contains a pocket dimension with " + capacity + " cubic feet of storage.\n");
    say(this_player()->query_cap_name() + " creates a dimensional envelope in " +
        (string)target->query_short() + "!");
    call_out("end_dim_envelope", dur, target);
}

void end_dim_envelope(object item) {
    object env;
    if(!objectp(item)) return;
    item->set_property("dim_envelope", 0);
    item->set_property("dim_envelope_capacity", 0);
    env = environment(item);
    if(objectp(env) && living(env))
        tell_object(env,
            "The dimensional envelope in " + (string)item->query_short() + " collapses.\n");
}

private void fx_soul_drain(object target) {
    int me;
    int roll;
    int drain;
    int cur_me;
    if(!target) {
        write("Drain whom? Syntax: cast soul drain at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Soul drain only works on living beings.\n");
        return;
    }
    me   = (int)target->query_stats("ME");
    roll = random(20) + 1 + (me >= 26 ? 3 : me >= 21 ? 2 : me >= 17 ? 1 : 0);
    if(roll >= 18) {
        tell_object(target, "A cold mental force probes you, but your will holds!\n");
        write(target->query_cap_name() + " resists the soul drain!\n");
        return;
    }
    drain  = random(6) + 1;
    cur_me = (int)target->query_stats("ME");
    if(drain >= cur_me) drain = cur_me - 1;
    if(drain < 0) drain = 0;
    target->set_stats("ME", cur_me - drain);
    write("You tear away " + drain + " points of " + (string)target->query_cap_name() +
          "'s mental essence! -" + drain + " ME.\n");
    tell_object(target, this_player()->query_cap_name() +
        " drains your soul! -" + drain + " ME!\n");
    say(this_player()->query_cap_name() + " drains the soul of " +
        (string)target->query_cap_name() + "!");
}

private void fx_mystic_fulcrum(object target) {
    int level;
    int dur;
    if((int)this_player()->query_property("mystic_fulcrum")) {
        write("Your mystic fulcrum is already active.\n");
        return;
    }
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur = level * 240;
    this_player()->set_property("mystic_fulcrum", 1);
    write("Your connection to the ley lines snaps taut. All your spells are doubled in power!\n"
          "Warning: spell PPE costs are also doubled while the fulcrum is active.\n");
    say(this_player()->query_cap_name() + " channels the mystic fulcrum!");
    call_out("end_mystic_fulcrum", dur, this_player());
}

void end_mystic_fulcrum(object who) {
    if(!objectp(who)) return;
    who->set_property("mystic_fulcrum", 0);
    tell_object(who, "The mystic fulcrum releases. Your connection to the ley lines normalizes.\n");
}

private void fx_animate_object(object target) {
    string obj_name;
    if(!target) {
        write("Animate what? Syntax: cast animate object at <object>\n");
        return;
    }
    if(living(target)) {
        write("That is already alive.\n");
        return;
    }
    if((int)target->query_property("animated")) {
        write("That object is already animated.\n");
        return;
    }
    obj_name = (string)target->query_short();
    if(!obj_name || !sizeof(obj_name)) obj_name = "the object";
    target->set_property("animated", 1);
    write(obj_name + " lurches to life, moving under your mental direction!\n"
          "It has the strength of PS 20 and 50 SDC.\n");
    say(this_player()->query_cap_name() + " animates " + obj_name + "!");
    call_out("end_animate_object", 300, target);
}

void end_animate_object(object item) {
    object env;
    if(!objectp(item)) return;
    item->set_property("animated", 0);
    env = environment(item);
    if(objectp(env))
        tell_room(env, (string)item->query_short() + " goes still.\n");
}

private void fx_anti_magic_cloud(object target) {
    object room;
    int level;
    int dur;
    room = environment(this_player());
    if(!room) return;
    if((int)room->query_property("anti_magic_cloud")) {
        write("An anti-magic cloud is already active here.\n");
        return;
    }
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur = level * 600;
    room->set_property("anti_magic_cloud", 1);
    write("A shimmering cloud of null-magic energy fills the area!\n"
          "ALL magic is suppressed here, including your own spells and magic items.\n");
    say(this_player()->query_cap_name() +
        " unleashes an anti-magic cloud! All magic fails here!");
    call_out("end_anti_magic_cloud", dur, room);
}

void end_anti_magic_cloud(object room) {
    if(!objectp(room)) return;
    room->set_property("anti_magic_cloud", 0);
    tell_room(room, "The anti-magic cloud dissipates. Magic flows freely again.\n");
}

/* ── Level 12 spell handlers ────────────────────────────────────────────────── */

private void fx_rift_dim_pocket(object target) {
    int level;
    int dur;
    if((int)this_player()->query_property("rifted_pocket")) {
        write("You are already in a dimensional pocket rift.\n");
        return;
    }
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur = level * 600;
    this_player()->set_property("rifted_pocket", 1);
    this_player()->set_property("invisible_spell", 1);
    write("You step sideways into the fabric of reality.\n"
          "The world becomes faint and translucent around you.\n"
          "You are invisible and intangible. You can observe but not interact.\n");
    say(this_player()->query_cap_name() +
        " steps sideways into a dimensional pocket -- and vanishes!");
    call_out("end_rift_dim_pocket", dur, this_player());
}

void end_rift_dim_pocket(object who) {
    if(!objectp(who)) return;
    who->set_property("rifted_pocket", 0);
    who->set_property("invisible_spell", 0);
    tell_object(who, "You step back into normal reality.\n");
    tell_room(environment(who),
        who->query_cap_name() +
        " steps back into existence from a dimensional pocket.\n",
        ({ who }));
}

private void fx_resurrection_spell(object target) {
    int pe_loss;
    int i;
    int cur_pe;
    int max_hp;
    int max_sdc;
    pe_loss = 0;
    for(i = 0; i < 2; i++) pe_loss += random(6) + 1;
    cur_pe = (int)this_player()->query_stats("PE");
    this_player()->set_stats("PE", cur_pe - pe_loss);
    write("You pour your life essence into the resurrection ritual.\n"
          "The enormous cost tears away " + pe_loss + " points of PE -- permanently!\n");
    if(!target || living(target)) {
        write("You need to target a recently-dead corpse to resurrect.\n");
        return;
    }
    target->set_property("resurrected", 1);
    max_hp  = (int)target->query_stats("max_rifts_hp");
    max_sdc = (int)target->query_stats("max_SDC");
    if(max_hp > 0) target->set_stats("rifts_hp", max_hp);
    if(max_sdc > 0) target->set_stats("SDC", max_sdc);
    write("Life floods back into " + (string)target->query_short() + "!\n");
    say(this_player()->query_cap_name() + " performs the resurrection! Life returns!");
}

private void fx_create_rift(object target) {
    object room;
    int roll;
    int dur;
    room = environment(this_player());
    if(!room) return;
    if(!(int)RIFTS_D->room_on_ley_line(room) &&
       !(int)room->query_property("rift_active") &&
       !(int)room->query_property("nexus")) {
        write("Create Rift requires proximity to an active ley line or nexus point.\n");
        return;
    }
    dur = (random(6) + 1) * 60;
    room->set_property("rift_active", 1);
    write("Reality tears open! A howling dimensional rift erupts before you!\n"
          "Dimensional energy screams through the opening.\n");
    say(this_player()->query_cap_name() + " tears open a dimensional rift!");
    if(target && living(target) && target != this_player()) {
        roll = (int)target->query_stats("PE") + random(20) + 1;
        if(roll < 18) {
            tell_object(target, "The rift tears at you, sucking you through!\n");
            tell_room(room,
                (string)target->query_cap_name() + " is pulled into the rift!\n",
                ({ this_player(), target }));
            target->stop_fight();
            target->move("/domains/Praxis/wild1");
        } else {
            tell_object(target, "The rift pulls at you, but you resist!\n");
            write(target->query_cap_name() + " resists being pulled into the rift!\n");
        }
    }
    call_out("end_create_rift", dur, room);
}

void end_create_rift(object room) {
    if(!objectp(room)) return;
    room->set_property("rift_active", 0);
    tell_room(room, "The dimensional rift howls and collapses with a thunderclap.\n");
}

private void fx_time_hole(object target) {
    int pe;
    int roll;
    int dur;
    if(!target) {
        write("Cast at whom? Syntax: cast time hole at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Time hole only works on living beings.\n");
        return;
    }
    pe   = (int)target->query_stats("PE");
    roll = random(20) + 1 + (pe >= 16 ? 2 : pe >= 12 ? 1 : 0);
    if(roll >= 18) {
        tell_object(target,
            "You feel time distort around you, but your will pushes through!\n");
        write(target->query_cap_name() + " resists the time hole!\n");
        return;
    }
    dur = (random(4) + 1) * 15;
    target->set_property("time_frozen", 1);
    target->set_property("stunned", 1);
    write("A bubble of distorted time forms around " + (string)target->query_cap_name() +
          "! They are frozen in temporal stasis!\n");
    tell_object(target, "Time stops! You are frozen in a temporal bubble!\n");
    say(this_player()->query_cap_name() + " traps " + (string)target->query_cap_name() +
        " in a time hole!");
    call_out("end_time_hole", dur, target);
}

void end_time_hole(object who) {
    if(!objectp(who)) return;
    who->set_property("time_frozen", 0);
    who->set_property("stunned", 0);
    tell_object(who, "The temporal bubble dissolves. Time resumes normally.\n");
    tell_room(environment(who),
        who->query_cap_name() + " is released from the time hole.\n", ({ who }));
}

/* ── Level 13-15 spell handlers ────────────────────────────────────────────── */

private void fx_close_rift(object target) {
    object room;
    int level;
    room  = environment(this_player());
    if(!room) return;
    level = (int)this_player()->query_level();
    if(level < 13) {
        write("You must be at least 13th level to permanently seal a rift.\n");
        return;
    }
    if(!(int)room->query_property("rift_active") &&
       !(int)room->query_property("dimensional_bleed")) {
        write("There is no open rift here to close.\n");
        return;
    }
    room->set_property("rift_active", 0);
    room->set_property("rift_sealed", 1);
    room->set_property("dimensional_bleed", 0);
    write("You pour your will into the tear in dimensional space, forcing it closed!\n"
          "Reality knits shut. The rift is permanently sealed.\n");
    say(this_player()->query_cap_name() +
        " seals a dimensional rift! Reality mends itself!");
}

private void fx_create_golem(object target) {
    if((int)this_player()->query_property("golem_count") >= 1) {
        write("You can only control one golem at a time.\n");
        return;
    }
    this_player()->set_property("golem_count", 1);
    write("You channel animating force into the stone, infusing it with magical life!\n"
          "The massive golem lurches upright, ready to obey.\n"
          "Golem: MDC 200, PS 40 (supernatural), AR 16, APM 3.\n");
    say(this_player()->query_cap_name() +
        " animates a massive stone golem! It rises to serve!");
}

private void fx_calling_spell(object target) {
    int roll;
    roll = random(20) + 1;
    write("You project your will across dimensional barriers,\n"
          "reaching for a specific named supernatural being...\n");
    say(this_player()->query_cap_name() +
        " extends their will across the dimensional planes!");
    if(roll >= 14) {
        write("The call reaches across the planes but the being resists!\n"
              "A powerful will pushes back against the summons.\n");
    } else {
        write("The dimensional barriers shudder! Your call tears through!\n"
              "The named being stirs, compelled to seek you out.\n"
              "(The called being will arrive within 1d6 hours.)\n");
    }
}

private void fx_dim_lock(object target) {
    object room;
    int level;
    int dur;
    room = environment(this_player());
    if(!room) return;
    if((int)room->query_property("dim_lock")) {
        write("A dimensional lock is already active here.\n");
        return;
    }
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur = level * 86400;
    room->set_property("dim_lock", 1);
    room->set_property("no_teleport", 1);
    room->set_property("no_rifts", 1);
    write("You inscribe invisible sigils in the air, locking the dimensional fabric!\n"
          "No rifts, teleportation, or dimensional travel can occur in this area.\n");
    say(this_player()->query_cap_name() +
        " seals the dimensional fabric with a Dimensional Lock!");
    call_out("end_dim_lock", dur, room);
}

void end_dim_lock(object room) {
    if(!objectp(room)) return;
    room->set_property("dim_lock", 0);
    room->set_property("no_teleport", 0);
    room->set_property("no_rifts", 0);
    tell_room(room, "The dimensional lock expires. The fabric of space relaxes.\n");
}

private void fx_re_animation(object target) {
    int cur;
    cur = (int)this_player()->query_property("skeleton_count");
    this_player()->set_property("skeleton_count", cur + 1);
    write("You channel necromantic energy into the bones!\n"
          "They knit together and rise as a skeleton warrior!\n"
          "Skeleton: MDC 60, AR 10, two attacks, 2d6 bone weapon.\n"
          "You now control " + (cur + 1) + " skeleton(s).\n");
    say(this_player()->query_cap_name() +
        " raises a skeleton warrior from bleached bones!");
}

private void fx_power_word_kill(object target) {
    int tgt_level;
    int me;
    int me_bonus;
    int roll;
    int damage;
    int i;
    int cur_mdc;
    int cur_hp;
    if(!target) {
        write("Power Word: Kill at whom?  Syntax: cast power word kill at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Power Word: Kill only works on living beings.\n");
        return;
    }
    write("You speak the ancient Word of Ending!\n");
    say(this_player()->query_cap_name() +
        " speaks a word of terrible ancient power!");
    tgt_level = (int)target->query_level();
    if(tgt_level <= 6) {
        me       = (int)target->query_stats("ME");
        me_bonus = (me >= 26 ? 3 : me >= 21 ? 2 : me >= 17 ? 1 : 0);
        roll     = random(20) + 1 + me_bonus;
        if(roll >= 18) {
            tell_object(target,
                "A word of death washes over you, but your will holds!\n");
            write(target->query_cap_name() + " resists the Power Word!\n");
            return;
        }
        tell_object(target, "The Word of Power tears through your being! You die!\n");
        write(target->query_cap_name() + " is instantly slain by the Power Word!\n");
        say(target->query_cap_name() + " is slain by " +
            this_player()->query_cap_name() + "'s Power Word!");
        catch(target->die());
    } else {
        damage = 0;
        for(i = 0; i < 4; i++) damage += (random(6) + 1) * 10;
        tell_object(target,
            "The Power Word tears through you!  " + damage + " MDC!\n");
        write(target->query_cap_name() + " takes " + damage +
              " MDC from the Power Word!\n");
        if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
            cur_mdc = (int)target->query_stats("MDC") - damage;
            target->set_stats("MDC", cur_mdc);
            if(cur_mdc <= -20) catch(target->die());
        } else {
            cur_hp = (int)target->query_hp() - damage;
            if(cur_hp < 0) { target->set_hp(0); catch(target->die()); }
            else target->set_hp(cur_hp);
        }
    }
}

private void fx_create_magic_weapon(object target) {
    if(!target) {
        write("Enchant what?  Syntax: cast create magic weapon at <weapon>\n");
        return;
    }
    if(living(target)) {
        write("Create Magic Weapon can only enchant weapons, not living beings.\n");
        return;
    }
    if((int)target->query_property("magic_weapon")) {
        write("That weapon is already enchanted.\n");
        return;
    }
    target->set_property("magic_weapon", 1);
    target->set_property("magic_weapon_bonus_strike", 2);
    target->set_property("magic_weapon_bonus_mdc", 1);
    write("You pour magical energy into " + (string)target->query_short() + "!\n"
          "It glows with soft magical light as the enchantment takes hold.\n"
          "+2 to strike, +2d6 MDC damage, now counts as an MDC weapon.\n");
    say(this_player()->query_cap_name() + " enchants " +
        (string)target->query_short() + " as a permanent magic weapon!");
}

private void fx_conjure_elemental(object target) {
    string *etypes;
    string etype;
    int level;
    int dur;
    if((int)this_player()->query_property("elemental_bound")) {
        write("You already have an elemental under your command.\n");
        return;
    }
    level  = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur    = level * 300;
    etypes = ({ "fire", "water", "earth", "air" });
    etype  = etypes[random(4)];
    this_player()->set_property("elemental_bound", 1);
    this_player()->set_property("elemental_type", etype);
    write("You call upon the elemental planes!\n"
          "A " + etype + " elemental materializes before you!\n"
          "Elemental: MDC 150, " + etype + " attack 4d6 MDC, AR 14, APM 4.\n"
          "It obeys your commands for " + (dur / 60) + " minutes.\n");
    say(this_player()->query_cap_name() + " summons a " + etype + " elemental!");
    call_out("end_elemental", dur, this_player());
}

void end_elemental(object who) {
    if(!objectp(who)) return;
    who->set_property("elemental_bound", 0);
    who->set_property("elemental_type", "");
    tell_object(who, "Your elemental returns to its home plane.\n");
    if(objectp(environment(who)))
        tell_room(environment(who),
            who->query_cap_name() + "'s elemental dissolves back to its plane.\n",
            ({ who }));
}

private void fx_seal_rift(object target) {
    object room;
    int level;
    room  = environment(this_player());
    if(!room) return;
    level = (int)this_player()->query_level();
    if(level < 14) {
        write("You must be at least 14th level to permanently seal a nexus point.\n");
        return;
    }
    if(!(int)room->query_property("nexus") &&
       !(int)RIFTS_D->room_on_ley_line(room) &&
       !(int)room->query_property("rift_active")) {
        write("There is no nexus point or ley line here to seal.\n");
        return;
    }
    room->set_property("nexus", 0);
    room->set_property("ley_line", 0);
    room->set_property("rift_active", 0);
    room->set_property("rift_sealed", 1);
    room->set_property("nexus_sealed", 1);
    write("You pour your entire will into the fundamental dimensional fabric!\n"
          "The nexus shudders and collapses with a sound like shattering glass.\n"
          "This nexus is permanently sealed. No rift can open here again.\n");
    say(this_player()->query_cap_name() +
        " permanently seals a nexus point! The dimensional fabric mends itself!");
}

private void fx_wish_spell(object target) {
    write("Reality bends to your will.\n"
          "You have invoked the Wish -- one of the most powerful spells in existence.\n"
          "Scope is subject to admin/GM discretion.\n"
          "Restrictions: cannot raise the permanently dead;\n"
          "  cannot create wealth beyond 1d6x10,000 credits;\n"
          "  cannot permanently destroy a supernatural imbued being.\n"
          "Your wish has been logged for review.\n");
    say(this_player()->query_cap_name() + " invokes the WISH! Reality trembles!");
    catch(write_file("/log/adm/wishes",
        (string)this_player()->query_name() + " invoked wish at " +
        ctime(time()) + "\n"));
}

private void fx_time_warp_age(object target) {
    int me;
    int me_bonus;
    int roll;
    int level;
    int years;
    int cur_mdc;
    int mdc_loss;
    int natural_max;
    int cur_age;
    int sdc_loss;
    int cur_sdc;
    if(!target) {
        write("Age whom?  Syntax: cast time warp age at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Time Warp: Age only works on living beings.\n");
        return;
    }
    me       = (int)target->query_stats("ME");
    me_bonus = (me >= 26 ? 3 : me >= 21 ? 2 : me >= 17 ? 1 : 0);
    roll     = random(20) + 1 + me_bonus;
    if(roll >= 20) {
        tell_object(target,
            "Time itself tries to tear at you, but your will holds firm!\n");
        write(target->query_cap_name() + " resists the Time Warp!\n");
        return;
    }
    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    years = level * 10;
    write("Temporal energy tears through " + (string)target->query_cap_name() +
          "!  Time advances " + years + " years in a heartbeat!\n");
    tell_object(target,
        "You age " + years + " years in an instant!  Your body withers!\n");
    say(this_player()->query_cap_name() + " warps time around " +
        (string)target->query_cap_name() + "!  They age rapidly!");
    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        mdc_loss = years;
        cur_mdc  = (int)target->query_stats("MDC") - mdc_loss;
        if(cur_mdc < 0) cur_mdc = 0;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= 0) {
            write(target->query_cap_name() + " crumbles to dust!\n");
            catch(target->die());
        }
    } else {
        natural_max = 80;
        cur_age     = (int)target->query_property("age");
        if(cur_age + years >= natural_max) {
            write(target->query_cap_name() +
                  " ages past their natural lifespan and collapses!\n");
            catch(target->die());
        } else {
            target->set_property("age", cur_age + years);
            sdc_loss = years / 10;
            if(sdc_loss < 1) sdc_loss = 1;
            cur_sdc  = (int)target->query_stats("SDC") - sdc_loss;
            if(cur_sdc < 0) cur_sdc = 0;
            target->set_stats("SDC", cur_sdc);
            write(target->query_cap_name() + " appears visibly older.\n");
        }
    }
}

private void fx_rift_to_oblivion(object target) {
    int pe;
    int pe_bonus;
    int roll;
    int damage;
    int i;
    int cur_mdc;
    int cur_hp;
    object room;
    if(!target) {
        write("Cast at whom?  Syntax: cast rift to oblivion at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Rift to Oblivion only works on living beings.\n");
        return;
    }
    room = environment(this_player());
    write("You rip a micro-rift directly under " +
          (string)target->query_cap_name() + "!\n");
    say(this_player()->query_cap_name() + " opens a rift to oblivion beneath " +
        (string)target->query_cap_name() + "!");
    pe       = (int)target->query_stats("PE");
    pe_bonus = (pe >= 16 ? 2 : pe >= 12 ? 1 : 0);
    roll     = random(20) + 1 + pe_bonus;
    if(roll >= 20) {
        damage = 0;
        for(i = 0; i < 6; i++) damage += random(6) + 1;
        tell_object(target,
            "The rift tears open beneath you but you leap clear!  " +
            damage + " MDC!\n");
        write(target->query_cap_name() + " escapes the rift!  " + damage + " MDC!\n");
        if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
            cur_mdc = (int)target->query_stats("MDC") - damage;
            target->set_stats("MDC", cur_mdc);
            if(cur_mdc <= -20) catch(target->die());
        } else {
            cur_hp = (int)target->query_hp() - damage;
            if(cur_hp < 0) { target->set_hp(0); catch(target->die()); }
            else target->set_hp(cur_hp);
        }
    } else {
        tell_object(target,
            "The rift tears open directly beneath you!  "
            "You are sucked into oblivion!\n");
        if(objectp(room))
            tell_room(room,
                (string)target->query_cap_name() +
                " is sucked into the rift and vanishes!\n",
                ({ this_player(), target }));
        target->stop_fight();
        target->move("/domains/Praxis/wild1");
    }
}

private void fx_supreme_being(object target) {
    object room;
    int ppe_loss;
    int cur_ppe;
    int max_ppe;
    int i;
    room = environment(this_player());
    if(!room) return;
    if(!(int)room->query_property("nexus") &&
       !(int)RIFTS_D->room_on_ley_line(room)) {
        write("Supreme Being Summoning can only be cast at a ley line nexus point.\n");
        return;
    }
    if((int)this_player()->query_property("supreme_entity_active")) {
        write("You already have a supreme entity answering your call.\n");
        return;
    }
    ppe_loss = 0;
    for(i = 0; i < 4; i++) ppe_loss += (random(4) + 1) * 10;
    max_ppe  = (int)this_player()->query_stats("max_PPE") - ppe_loss;
    if(max_ppe < 0) max_ppe = 0;
    this_player()->set_stats("max_PPE", max_ppe);
    cur_ppe = (int)this_player()->query_stats("PPE");
    if(cur_ppe > max_ppe) cur_ppe = max_ppe;
    this_player()->set_stats("PPE", cur_ppe);
    this_player()->set_property("supreme_entity_active", 1);
    write("The nexus tears wide open!  Reality screams!\n"
          "An entity of incalculable power descends into this world!\n"
          "It will perform ONE task for you, then depart.\n"
          "COST: " + ppe_loss + " maximum PPE permanently lost. (Catastrophic.)\n");
    say(this_player()->query_cap_name() +
        " tears open the nexus!  A supreme entity of god-like power descends!");
    call_out("end_supreme_being", 600, this_player());
}

void end_supreme_being(object who) {
    if(!objectp(who)) return;
    who->set_property("supreme_entity_active", 0);
    tell_object(who, "The supreme entity departs. The nexus falls quiet.\n");
    if(objectp(environment(who)))
        tell_room(environment(who),
            who->query_cap_name() +
            "'s summoned entity departs with a thunderclap.\n",
            ({ who }));
}

/* Main dispatch - called by _cast.c after PPE deduction.
   target may be 0 for self/room spells. */

void apply_spell_effect(string spell_name, object target) {
    mapping sdata;
    string effect;

    sdata = query_spell(lower_case(spell_name));
    if(!sdata) return;
    effect = (string)sdata["effect"];

    switch(effect) {
    case "light":        fx_globe(target);       break;
    case "sense_evil":   fx_sense_evil(target);  break;
    case "sense_magic":  fx_sense_magic(target); break;
    case "stun":         fx_stun(target);        break;
    case "damage":       fx_damage(target);      break;
    case "heal":         fx_heal(target);        break;
    case "mark":         fx_mark(target);        break;
    case "pocket":       fx_pocket(target);      break;
    case "familiar":     fx_familiar(target);    break;
    case "windrush":     fx_windrush(target);    break;
    case "swimasafish":  fx_swimasafish(target); break;
    case "smoke":        fx_smoke(target);       break;
    case "blind":        fx_blind(target);       break;
    case "levitate":     fx_levitate(target);    break;
    case "net":          fx_net(target);         break;
    case "chameleon":    fx_chameleon(target);   break;
    case "fear":         fx_fear(target);        break;
    case "see_aura":     fx_see_aura(target);    break;
    case "impervious_fire": fx_impervious_fire(target); break;
    case "impervious_cold": fx_impervious_cold(target); break;
    case "breath_of_life": fx_breath_of_life(target); break;
    case "exorcism":     fx_exorcism(target);    break;
    case "heavy_darkness": fx_heavy_darkness(target); break;
    case "words_truth":  fx_words_truth(target); break;
    case "see_invisible_spell": fx_see_invisible_spell(target); break;
    case "animate_dead": fx_animate_dead(target); break;
    case "constrain_being": fx_constrain_being(target); break;
    case "astral_projection": fx_astral_projection(target); break;
    case "detect_concealment": fx_detect_concealment(target); break;
    case "mend_wounds":      fx_mend_wounds(target);      break;
    case "sense_danger":     fx_sense_danger(target);     break;
    case "mystic_alarm":     fx_mystic_alarm(target);     break;
    case "slow_movement":    fx_slow_movement(target);    break;
    case "detect_poison":    fx_detect_poison(target);    break;
    case "death_trance":     fx_death_trance(target);     break;
    case "globe_silence":    fx_globe_silence(target);    break;
    case "burn":             fx_burn(target);             break;
    case "cure_disorders":   fx_cure_disorders(target);   break;
    case "charismatic_aura": fx_charismatic_aura(target); break;
    case "repel_animals":    fx_repel_animals(target);    break;
    case "cleanse":          fx_cleanse(target);          break;
    case "luck_curse":       fx_luck_curse(target);       break;
    case "wall_of_light":    fx_wall_of_light(target);    break;
    case "wisps_confusion":  fx_wisps_confusion(target);  break;
    case "fireball":     fx_fireball(target);    break;
    case "adhesion":     fx_adhesion(target);    break;
    case "invisible":    fx_invisible(target);   break;
    case "firebolt":     fx_firebolt(target);    break;
    case "tongues":      fx_tongues(target);     break;
    case "ithan_armor":  fx_ithan_armor(target); break;
    case "flame_circle":       fx_flame_circle(target);      break;
    case "spell_tk":           fx_spell_tk(target);          break;
    case "energy_field":       fx_energy_field(target);      break;
    case "ignite_fire":        fx_ignite_fire(target);       break;
    case "multiple_image":     fx_multiple_image(target);    break;
    case "shadow_meld":        fx_shadow_meld(target);       break;
    case "mask_deceit":        fx_mask_deceit(target);       break;
    case "domination":         fx_domination(target);        break;
    case "escape_spell":       fx_escape_spell(target);      break;
    case "fly_spell":          fx_fly_spell(target);         break;
    case "horrific_illusion":  fx_horrific_illusion(target); break;
    case "impervious_energy":  fx_impervious_energy(target); break;
    case "metamorphosis":      fx_metamorphosis(target);     break;
    case "call_lightning":     fx_call_lightning(target);    break;
    case "freeze_spell":       fx_freeze_spell(target);      break;
    case "negate_mechanics":   fx_negate_mechanics(target);  break;
    case "restoration_spell":  fx_restoration_spell(target); break;
    case "shadow_bolt":        fx_shadow_bolt(target);       break;
    case "time_slip":          fx_time_slip(target);         break;
    case "cloud_of_fear":      fx_cloud_of_fear(target);     break;
    case "dispel_barrier":     fx_dispel_barrier(target);    break;
    case "invincible_armor":   fx_invincible_armor(target);  break;
    case "mystic_portal":      fx_mystic_portal(target);     break;
    case "protection_circle":  fx_protection_circle(target); break;
    case "energy_drain":       fx_energy_drain(target);      break;
    case "eyes_of_thoth":      fx_eyes_of_thoth(target);     break;
    case "locate_spell":       fx_locate_spell(target);      break;
    case "mass_confusion":     fx_mass_confusion(target);    break;
    case "negate_magic":       fx_negate_magic(target);      break;
    case "rift_walk":          fx_rift_walk(target);         break;
    case "meta_superior":      fx_meta_superior(target);     break;
    case "create_zombie":      fx_create_zombie(target);     break;
    case "amulet_spell":       fx_amulet_spell(target);      break;
    case "summon_fog":         fx_summon_fog(target);        break;
    case "teleport_spell":     fx_teleport_spell(target);    break;
    case "energy_disruption":  fx_energy_disruption(target); break;
    case "life_drain":         fx_life_drain(target);        break;
    case "circle_superior":    fx_circle_superior(target);   break;
    case "create_scroll":      fx_create_scroll(target);     break;
    case "fly_eagle":          fx_fly_eagle(target);         break;
    case "turn_dead_sup":      fx_turn_dead_sup(target);     break;
    case "impervious_mdce":    fx_impervious_mdce(target);   break;
    case "dim_envelope":       fx_dim_envelope(target);      break;
    case "soul_drain":         fx_soul_drain(target);        break;
    case "mystic_fulcrum":     fx_mystic_fulcrum(target);    break;
    case "animate_object":     fx_animate_object(target);    break;
    case "anti_magic_cloud":   fx_anti_magic_cloud(target);  break;
    case "rift_dim_pocket":    fx_rift_dim_pocket(target);   break;
    case "resurrection_spell": fx_resurrection_spell(target);break;
    case "create_rift":        fx_create_rift(target);       break;
    case "time_hole":          fx_time_hole(target);         break;
    case "close_rift":         fx_close_rift(target);        break;
    case "create_golem":       fx_create_golem(target);      break;
    case "calling_spell":      fx_calling_spell(target);     break;
    case "dim_lock":           fx_dim_lock(target);          break;
    case "re_animation":       fx_re_animation(target);      break;
    case "power_word_kill":    fx_power_word_kill(target);   break;
    case "create_magic_weapon":fx_create_magic_weapon(target);break;
    case "conjure_elemental":  fx_conjure_elemental(target); break;
    case "seal_rift":          fx_seal_rift(target);         break;
    case "wish_spell":         fx_wish_spell(target);        break;
    case "time_warp_age":      fx_time_warp_age(target);     break;
    case "rift_to_oblivion":   fx_rift_to_oblivion(target);  break;
    case "supreme_being":      fx_supreme_being(target);     break;
    }
}

/* ── Utility: list all known spell names for _spells.c ────────────────────── */

string *query_all_spells() {
    return ({
        "globe of daylight", "sense evil", "sense magic",
        "thunderclap", "energy bolt", "heal wounds",
        "light target", "dimensional pocket", "call familiar",
        "windrush", "swimasafish",
        "cloud of smoke", "blinding flash", "levitation", "magic net",
        "chameleon", "fear", "see aura",
        "impervious to fire", "impervious to cold", "breath of life",
        "exorcism", "heavy darkness", "words of truth",
        "see invisible", "animate dead", "constrain being",
        "astral projection", "detect concealment",
        "mend wounds", "sense danger", "mystic alarm", "slow movement",
        "detect poison", "death trance", "globe of silence", "burn",
        "cure disorders", "charismatic aura", "repel animals", "cleanse",
        "luck curse", "wall of light", "wisps of confusion",
        "fire ball", "carpet of adhesion", "invisibility simple",
        "fire bolt", "tongues", "armor of ithan", "circle of flame", "telekinesis",
        "energy field", "ignite fire", "multiple image",
        "shadow meld", "mask of deceit",
        "domination", "escape", "fly",
        "horrific illusion", "impervious to energy", "metamorphosis animal",
        "call lightning", "freeze", "negate mechanics",
        "restoration", "shadow bolt", "time slip",
        "cloud of fear", "dispel magic barrier", "invincible armor",
        "mystic portal", "protection circle simple",
        "energy drain", "eyes of thoth", "locate",
        "mass confusion", "negate magic", "rift walk",
        "metamorphosis superior", "create zombie", "amulet",
        "summon fog", "teleport", "energy disruption",
        "life drain", "circle of protection superior",
        "create magic scroll", "fly as eagle",
        "turn dead superior", "impervious to magic energy",
        "dimensional envelope", "soul drain", "mystic fulcrum",
        "animate object", "anti magic cloud",
        "rift dimensional pocket", "resurrection", "create rift", "time hole",
        "close rift", "create golem", "calling", "dimensional lock", "re animation",
        "power word kill", "create magic weapon", "conjure elemental", "seal rift",
        "wish", "time warp age", "rift to oblivion", "supreme being summoning"
    });
}

private void write_spell_line(string name, mapping sdata, string tag, int verbose) {
    string alias;
    string label;

    alias = query_spell_alias_hint(name);
    label = capitalize(name);
    if(alias && sizeof(alias))
        label += " (" + alias + ")";
    if(verbose && sdata)
        write(sprintf("  %-28s PPE %-3d  %s  %s",
            label, (int)sdata["ppe_cost"], tag, (string)sdata["desc"]));
    else if(sdata)
        write(sprintf("  %-28s PPE %-3d  %s",
            label, (int)sdata["ppe_cost"], tag));
    else
        write(sprintf("  %-28s  %s", label, tag));
}

#define LIST_COLUMNS 4

private void write_spell_group(string *names, int verbose) {
    string spell;
    mapping sdata;
    string *labels;
    int i;

    if(!names || !sizeof(names)) {
        write("  (none)");
        return;
    }
    if(verbose) {
        for(i = 0; i < sizeof(names); i++) {
            spell = names[i];
            sdata = query_spell(spell);
            write_spell_line(spell, sdata, "", 1);
        }
        return;
    }
    labels = ({});
    for(i = 0; i < sizeof(names); i++)
        labels += ({ capitalize(names[i]) });
    write(format_page(labels, LIST_COLUMNS));
}

void write_spell_list(object pl, string mode) {
    string known;
    string *known_list;
    string *race_grants;
    string *occ_grants;
    string *section;
    string race;
    string occ;
    string spell;
    int i;
    int verbose;
    int show_all;

    if(!pl) return;
    verbose = (mode == "verbose");
    show_all = (mode == "all" || mode == "verbose");
    race = lower_case((string)pl->query_race());
    occ = lower_case((string)pl->getenv("rifts_occ"));
    if(!occ) occ = "none";

    write("=== KNOWN SPELLS ===");
    write(sprintf("PPE: %d / %d",
        (int)pl->query_stats("PPE"), (int)pl->query_stats("max_PPE")));
    write(repeat_string("-", 60));

    if(race == "wizard" && !show_all) {
        known_list = query_all_spells();
        write("[Master] Wizard: full spell spectrum (" +
            sizeof(known_list) + " powers)");
        write("  Type 'spells all' for the grouped list.");
        write("  Type 'spells verbose' for descriptions.");
        write(repeat_string("-", 60));
        write("Syntax: cast <spell> [at <target>]");
        return;
    }

    if(race == "wizard")
        known_list = query_all_spells();
    else {
        known = (string)pl->getenv("known_spells");
        if(!known || !sizeof(known)) {
            write("  You know no spells.");
            write(repeat_string("-", 60));
            write("Syntax: cast <spell> [at <target>]");
            return;
        }
        known_list = explode(known, ";");
    }

    race_grants = (string *)RIFTS_START_D->query_race_spell_grant(race);
    occ_grants = (string *)RIFTS_START_D->query_occ_spell_grant(occ);

    if(race == "wizard") {
        write("[Master]");
        write_spell_group(known_list, verbose);
        write(repeat_string("-", 60));
        write("Syntax: cast <spell> [at <target>]");
        return;
    }

    write("[Race]");
    section = ({});
    for(i = 0; i < sizeof(known_list); i++) {
        spell = known_list[i];
        if(!spell || !sizeof(spell)) continue;
        if(member_array(spell, race_grants) == -1) continue;
        section += ({ spell });
    }
    write_spell_group(section, verbose);

    write("[OCC]");
    section = ({});
    for(i = 0; i < sizeof(known_list); i++) {
        spell = known_list[i];
        if(!spell || !sizeof(spell)) continue;
        if(member_array(spell, race_grants) != -1) continue;
        if(member_array(spell, occ_grants) == -1) continue;
        section += ({ spell });
    }
    write_spell_group(section, verbose);

    write("[Learned]");
    section = ({});
    for(i = 0; i < sizeof(known_list); i++) {
        spell = known_list[i];
        if(!spell || !sizeof(spell)) continue;
        if(member_array(spell, race_grants) != -1) continue;
        if(member_array(spell, occ_grants) != -1) continue;
        section += ({ spell });
    }
    write_spell_group(section, verbose);

    write(repeat_string("-", 60));
    write("Syntax: cast <spell> [at <target>]");
}
