// /daemon/bionic_d.c
// Bionic and cybernetic installation tracker.
// Player bionics stored as env var "installed_bionics": mapping of slot -> bionic_name.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private mapping __bionics;

void create() {
    daemon::create();
    set_no_clean(1);
    __bionics = ([
        "cyber_arm_right"    : ([ "slot": "right_arm", "ps_bonus": 5, "sdc_bonus": 10,
                                   "desc": "A cybernetic right arm (+5 PS, +10 SDC, 2d6 punch)." ]),
        "cyber_arm_left"     : ([ "slot": "left_arm", "ps_bonus": 5, "sdc_bonus": 10,
                                   "desc": "A cybernetic left arm (+5 PS, +10 SDC, 2d6 punch)." ]),
        "cyber_legs"         : ([ "slot": "legs", "spd_bonus": 10, "sdc_bonus": 20,
                                   "desc": "Cybernetic legs (+10 Spd, +20 SDC, 2d6 kick)." ]),
        "cyber_eye_optics"   : ([ "slot": "eye", "sight_bonus": 1,
                                   "desc": "Optical implants.  No darkness penalty." ]),
        "cyber_ear"          : ([ "slot": "ear", "hearing_bonus": 1,
                                   "desc": "Enhanced hearing sensors." ]),
        "partial_borg_chest" : ([ "slot": "torso", "mdc_bonus": 60, "converts_torso_mdc": 1,
                                   "desc": "Partial borg chest conversion.  Torso gains 60 MDC." ]),
        "cyber_lung"         : ([ "slot": "lung", "pe_bonus": 2,
                                   "desc": "Cybernetic lungs. +2 PE, filters toxins." ]),
        "neural_net"         : ([ "slot": "brain", "me_bonus": 1,
                                   "desc": "Neural net implant. +1 ME for computer ops." ]),
        "full_borg_conversion": ([ "slot": "all", "total_mdc": 150, "converts_mdc": 1,
                                   "desc": "Full borg conversion.  150 total MDC, +15 PS, +5 Spd." ]),
        "biocomp_implant"    : ([ "slot": "brain_internal",
                                   "desc": "Bio-Comp self-monitoring system. Tracks vitals, warns of poison and disease." ]),
        "gyro_compass"       : ([ "slot": "internal",
                                   "desc": "Gyro-compass implant. Unerring sense of direction." ]),
        "headjack"           : ([ "slot": "head_internal",
                                   "desc": "Headjack port for direct interface with computers and vehicles." ]),
        "multi_optic_eye"    : ([ "slot": "eye", "sight_bonus": 2,
                                   "desc": "Multi-optic eye: telescopic, macro, nightvision and thermal filters." ]),
        "targeting_eye"      : ([ "slot": "eye", "strike_bonus": 1,
                                   "desc": "Targeting sight eye. +1 to strike with ranged weapons." ]),
        "clock_calendar"     : ([ "slot": "internal_clock",
                                   "desc": "Clock calendar chip. Always knows the exact time and date." ]),
        "radio_implant"      : ([ "slot": "ear_internal",
                                   "desc": "Built-in radio receiver and transmitter, five mile range." ]),
        "language_translator": ([ "slot": "translator",
                                   "desc": "Language translator implant. Understands six common languages." ]),
        "oxygen_storage_cell": ([ "slot": "torso_internal",
                                   "desc": "Oxygen storage cell. Thirty minutes of independent air supply." ]),
        "irmss_medical"      : ([ "slot": "internal_medical",
                                   "desc": "IRMSS internal robot medical system. Auto-stabilizes critical wounds." ]),
        "cyber_armor"        : ([ "slot": "torso_armor", "sdc_bonus": 50,
                                   "desc": "Subdermal cyber-armor plating. +50 SDC." ])
    ]);
}

/* Return the full bionics catalog (for shop/list displays). */
mapping query_all_bionics() { return copy(__bionics); }

mapping query_bionic_stats(string bionic_name) {
    if(!bionic_name || !__bionics[bionic_name]) return ([]);
    return (mapping)__bionics[bionic_name];
}

mapping query_bionics(object player) {
    mixed ev;

    if(!player) return ([]);
    ev = player->getenv("installed_bionics");
    if(!ev || !mapp(ev)) return ([]);
    return (mapping)ev;
}

int install_bionic(object player, string slot, string bionic_name) {
    mapping bionics, bdef;
    int ps, spd, sdc;

    if(!player || !slot || !bionic_name) return 0;
    bdef = (mapping)__bionics[bionic_name];
    if(!mapp(bdef)) return 0;

    bionics = query_bionics(player);
    bionics[slot] = bionic_name;
    player->setenv("installed_bionics", bionics);

    if(bionic_name == "full_borg_conversion") {
        ps  = (int)player->query_stats("PS");
        spd = (int)player->query_stats("Spd");
        player->set_stats("MDC",     150);
        player->set_stats("max_MDC", 150);
        player->set_stats("is_MDC",  1);
        player->set_stats("PS",      ps + 15);
        player->set_stats("Spd",     spd + 5);
        player->set_stats("SDC",     0);
        player->set_stats("max_SDC", 0);
        return 1;
    }
    /* Generic stat bonuses driven by the table entry, so new bionics only
       need a table row. sdc/mdc bonuses raise current and max together. */
    if((int)bdef["ps_bonus"]) {
        ps = (int)player->query_stats("PS");
        player->set_stats("PS", ps + (int)bdef["ps_bonus"]);
    }
    if((int)bdef["spd_bonus"]) {
        spd = (int)player->query_stats("Spd");
        player->set_stats("Spd", spd + (int)bdef["spd_bonus"]);
    }
    if((int)bdef["pe_bonus"]) {
        sdc = (int)player->query_stats("PE");
        player->set_stats("PE", sdc + (int)bdef["pe_bonus"]);
    }
    if((int)bdef["me_bonus"]) {
        sdc = (int)player->query_stats("ME");
        player->set_stats("ME", sdc + (int)bdef["me_bonus"]);
    }
    if((int)bdef["mdc_bonus"]) {
        sdc = (int)player->query_stats("MDC");
        player->set_stats("MDC",     sdc + (int)bdef["mdc_bonus"]);
        player->set_stats("max_MDC", sdc + (int)bdef["mdc_bonus"]);
    }
    if((int)bdef["sdc_bonus"]) {
        sdc = (int)player->query_stats("SDC");
        player->set_stats("SDC",     sdc + (int)bdef["sdc_bonus"]);
        sdc = (int)player->query_stats("max_SDC");
        player->set_stats("max_SDC", sdc + (int)bdef["sdc_bonus"]);
    }
    return 1;
}

int remove_bionic(object player, string slot) {
    mapping bionics;

    if(!player || !slot) return 0;
    bionics = query_bionics(player);
    if(!mapp(bionics) || !sizeof(bionics)) return 0;
    map_delete(bionics, slot);
    player->setenv("installed_bionics", bionics);
    return 1;
}
