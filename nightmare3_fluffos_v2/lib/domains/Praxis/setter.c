// /d/Praxis/setter.c
// Character creation room - race and OCC selection for AetherMUD
// Based on the Nightmare Mudlib setter by Shadowwolf@Nightmare (July 1992)
// Modified by Descartes of Borg for the race daemon (10 June 1993)
// Extended for Palladium Rifts attributes and OCC selection

#include <std.h>
#include <rooms.h>
#include <daemons.h>

/* Legacy NM3 fantasy races retired from mortal chargen. Creators can   */
/* still pick them for testing. Rifts-legal species that also exist in  */
/* RACE_D (dwarf, elf, human, ogre, orc, atlantean) stay available      */
/* through the Rifts race list.                                         */
#define RETIRED_LEGACY_RACES ({ "artrell", "faerie", "giant", "gnome", \
    "half-elf", "halfling", "half-orc", "hobbit", "kender", "klingon", \
    "nymph", "satyr" })

inherit ROOM;

int start_cmd(string str);
void do_rolls();
void do_plain_rolls();
void do_alignment_start();
void do_region_start();
void finish_creation();
void offer_occ_or_reroll();
void do_elective_skills_start();
int do_elective_skill_pick(string str);
void do_secondary_skills_start();
int do_secondary_skill_pick(string str);
void begin_creation();
void resume_creation();
void creation_prompt(string step);
void apply_race_body_pools(string race);
void apply_rifts_race_attributes(string race);
int alignment_cmd(string str);
int list_occs_cmd(string str);
int do_occ_pick(string str);
int roll_cmd(string str);
int chargen_catch(string str);
private int do_race_pick(string str);
private int valid_race_choice(string str);
private void choose_no_occ();
private void show_race_list();
private void show_occ_list(string *available);

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_long(
        "You exist in a void between possibilities. Reality waits for you "
        "to choose what you will become. The Rifts have torn the world "
        "apart. Who will you be in what remains?\n\n"
        "Character creation walks you through zone, attributes, race, "
        "alignment, and class - one step at a time.");
    set_items(
        (["room" : "The void from which all characters are born.",
          "list" : "A list of playable races in AetherMUD."]) );
}

void init() {
    string step;

    ::init();
    /* Catch-all for plain string answers (zone, race, alignment, OCC,
       and skill names typed with no verb). Added first so every named
       verb below is checked before it; it returns 0 for anything it
       does not recognize, letting normal commands fall through to the
       player's command hook. */
    add_action("chargen_catch", "", 1);
    add_action("read", "read");
    add_action("pick", "pick");
    add_action("list_occs_cmd", "list");
    add_action("no_occ_cmd", "no");
    add_action("alignment_cmd", "alignment");
    add_action("region_cmd", "region");
    add_action("start_cmd", "start");
    add_action("elective_list_cmd","skills");
    add_action("roll_cmd", "roll");
    add_action("roll_cmd", "reroll");
    if(creatorp(this_player())) return;
    step = (string)this_player()->getenv("creation_step");
    if(this_player()->query_race() && (!step || step == "" || step == "done"))
        return;
    if(!step || step == "")
        begin_creation();
    else
        resume_creation();
}

void begin_creation() {
    this_player()->setenv("creation_step", "region");
    this_player()->set_rolls(0);
    this_player()->remove_env("stats_rolled");
    this_player()->remove_env("awaiting_occ");
    this_player()->remove_env("awaiting_alignment");
    this_player()->remove_env("awaiting_region");
    this_player()->remove_env("awaiting_elective_skills");
    this_player()->remove_env("awaiting_secondary_skills");
    this_player()->remove_env("secondary_picks_remaining");
    creation_prompt("region");
}

void creation_prompt(string step) {
    if(step == "region") {
        write("\n=== STEP 1: CHOOSE YOUR STARTING ZONE ===");
        write("Where does your story begin on Rifts Earth?\n");
        write(" americas   The Americas (outskirts of Praxis)");
        write(" europe     Europe (New Camelot)");
        write(" atlantis   Atlantis (Splynn market shores)\n");
        write("Type your choice: americas, europe, or atlantis.\n");
        return;
    }
    if(step == "stats") {
        write("\n=== STEP 2: ROLL ATTRIBUTES ===");
        write("Roll Palladium attributes (3d6 each for IQ, ME, MA, PS, PP, PE, PB, Spd).");
        write("\nType: roll");
        write("After rolling you may type: reroll   (up to 4 rerolls, 5 total rolls)\n");
        return;
    }
    if(step == "race") {
        write("\n=== STEP 3: CHOOSE RACE / RCC ===");
        if((string)this_player()->getenv("stats_rolled") == "1")
            show_race_list();
        else
            write("Roll your attributes first. Type: roll\n");
        return;
    }
    if(step == "alignment") {
        do_alignment_start();
        return;
    }
    if(step == "occ") {
        write("\n=== STEP 5: CHOOSE OCC (OPTIONAL) ===");
        offer_occ_or_reroll();
        return;
    }
}

void resume_creation() {
    string step;

    step = (string)this_player()->getenv("creation_step");
    if(!step || step == "" || step == "done") {
        if(!this_player()->query_race())
            begin_creation();
        return;
    }
    write("\nWelcome back. You are still creating your character.\n");
    if(step == "region" || (string)this_player()->getenv("awaiting_region") == "1") {
        this_player()->setenv("creation_step", "region");
        creation_prompt("region");
        return;
    }
    if(step == "stats") {
        if((string)this_player()->getenv("stats_rolled") == "1") {
            this_player()->setenv("creation_step", "race");
            creation_prompt("race");
            return;
        }
        creation_prompt("stats");
        return;
    }
    if(step == "race") {
        creation_prompt("race");
        return;
    }
    if(step == "alignment" || (string)this_player()->getenv("awaiting_alignment") == "1") {
        this_player()->setenv("creation_step", "alignment");
        creation_prompt("alignment");
        return;
    }
    if(step == "occ" || (string)this_player()->getenv("awaiting_occ") == "1") {
        this_player()->setenv("creation_step", "occ");
        this_player()->setenv("awaiting_occ", "1");
        creation_prompt("occ");
        return;
    }
    if(step == "elective" || (string)this_player()->getenv("awaiting_elective_skills") == "1") {
        write("\n=== ELECTIVE SKILLS (IN PROGRESS) ===");
        write("Type skills <category> to browse, then type a skill name to continue.\n");
        return;
    }
    if(step == "secondary" || (string)this_player()->getenv("awaiting_secondary_skills") == "1") {
        write("\n=== SECONDARY SKILLS (IN PROGRESS) ===");
        write("Type skills <category> to browse, then type a skill name to continue.\n");
        return;
    }
    begin_creation();
}

private int roll_d6(int count) {
    int total;
    int i;

    total = 0;
    for(i = 0; i < count; i++)
        total += random(6) + 1;
    return total;
}

void do_plain_rolls() {
    object player;
    int iq, me, ma, ps, pp, pe, pb, spd;
    int rolls;

    player = this_player();
    rolls = (int)player->query_rolls();
    if(rolls >= 5) {
        write("You have used all five rolls. Type a race name from the list below.\n");
        show_race_list();
        return;
    }
    if(rolls == 4)
        write("This will be your final reroll.\n");
    write("\nYou roll your attributes (3d6 each).");
    iq  = roll_d6(3);
    me  = roll_d6(3);
    ma  = roll_d6(3);
    ps  = roll_d6(3);
    pp  = roll_d6(3);
    pe  = roll_d6(3);
    pb  = roll_d6(3);
    spd = roll_d6(3);

    player->set_stats("IQ", iq);
    player->set_stats("ME", me);
    player->set_stats("MA", ma);
    player->set_stats("PS", ps);
    player->set_stats("PP", pp);
    player->set_stats("PE", pe);
    player->set_stats("PB", pb);
    player->set_stats("Spd", spd);
    player->set_stats("strength", ps);
    player->set_stats("constitution", pe);
    player->set_stats("intelligence", iq);
    player->set_stats("wisdom", me);
    player->set_stats("dexterity", pp);
    player->set_stats("charisma", pb);

    player->set_rolls(rolls + 1);
    player->setenv("stats_rolled", "1");
    player->setenv("creation_step", "race");

    write(sprintf(
        " IQ:%-3d ME:%-3d MA:%-3d PS:%-3d\n"
        " PP:%-3d PE:%-3d PB:%-3d Spd:%-3d",
        iq, me, ma, ps, pp, pe, pb, spd));
    if(rolls < 4)
        write("\nRerolls remaining: " + (4 - rolls) + ". Type reroll to roll again, or type a race name from the list below.");
    else
        write("\nType a race name from the list below.");
    show_race_list();
}

void apply_race_body_pools(string race) {
    object player;
    int pe, hp, sdc, mdc;
    string *rflags;

    player = this_player();
    if(!race || !sizeof(race)) return;
    pe = (int)player->query_stats("PE");
    if((int)RIFTS_D->is_mdc_race(race)) {
        mdc = (int)RIFTS_D->init_mdc(race, pe);
        player->set_stats("MDC", mdc);
        player->set_stats("max_MDC", mdc);
        sdc = pe * 2;
        player->set_stats("SDC", sdc);
        player->set_stats("max_SDC", sdc);
        player->set_stats("is_MDC", 1);
        player->set_property("rifts_mdc_race", 1);
        player->setenv("rifts_mdc_race", "1");
        hp = (int)RIFTS_D->init_hp(race, pe);
        player->set_stats("rifts_hp", hp);
        player->set_stats("max_rifts_hp", hp);
        write(sprintf(" Body pools: HP %d, MDC %d", hp, mdc));
    } else {
        sdc = (int)RIFTS_D->init_sdc(race, pe);
        player->set_stats("SDC", sdc);
        player->set_stats("max_SDC", sdc);
        player->set_stats("MDC", 0);
        player->set_stats("max_MDC", 0);
        player->set_stats("is_MDC", 0);
        hp = (int)RIFTS_D->init_hp(race, pe);
        player->set_stats("rifts_hp", hp);
        player->set_stats("max_rifts_hp", hp);
        write(sprintf(" Body pools: HP %d, SDC %d", hp, sdc));
    }
    rflags = (string *)RIFTS_D->query_race_flags(race);
    player->setenv("rifts_flags", implode(rflags, ","));
}

/* Apply race-specific Palladium dice after the player picks a race.
 * The earlier "pick roll" is a plain 3d6 preview only. Without this,
 * RCC-only races (dragons, vampires, etc.) kept human-scale attributes
 * forever because they never hit OCC-step pick reroll / do_rolls(). */
void apply_rifts_race_attributes(string race) {
    mapping borg;
    string *keys_arr;
    string *rflags;
    int i, tmp, sdc, mdc, hp;

    if(!race || !sizeof(race)) return;
    if(!(int)RIFTS_D->is_rifts_race(race)) return;

    borg = (mapping)RIFTS_D->do_rifts_rolls(race);
    keys_arr = keys(borg);
    for(i = 0, tmp = sizeof(keys_arr); i < tmp; i++)
        this_player()->set_stats(keys_arr[i], borg[keys_arr[i]]);

    if((int)RIFTS_D->is_mdc_race(race)) {
        mdc = (int)RIFTS_D->init_mdc(race, borg["PE"]);
        this_player()->set_stats("MDC", mdc);
        this_player()->set_stats("max_MDC", mdc);
        sdc = (int)borg["PE"] * 2;
        this_player()->set_stats("SDC", sdc);
        this_player()->set_stats("max_SDC", sdc);
        this_player()->set_stats("is_MDC", 1);
        this_player()->set_property("rifts_mdc_race", 1);
        this_player()->setenv("rifts_mdc_race", "1");
    } else {
        sdc = (int)RIFTS_D->init_sdc(race, borg["PE"]);
        this_player()->set_stats("SDC", sdc);
        this_player()->set_stats("max_SDC", sdc);
        this_player()->set_stats("MDC", 0);
        this_player()->set_stats("max_MDC", 0);
        this_player()->set_stats("is_MDC", 0);
        this_player()->remove_property("rifts_mdc_race");
        this_player()->remove_env("rifts_mdc_race");
    }

    hp = (int)RIFTS_D->init_hp(race, borg["PE"]);
    this_player()->set_stats("rifts_hp", hp);
    this_player()->set_stats("max_rifts_hp", hp);

    rflags = (string *)RIFTS_D->query_race_flags(race);
    this_player()->setenv("rifts_flags", implode(rflags, ","));

    if((int)RIFTS_D->is_mdc_race(race)) {
        write(sprintf(
            " IQ:%-3d ME:%-3d MA:%-3d PS:%-3d\n"
            " PP:%-3d PE:%-3d PB:%-3d Spd:%-3d\n"
            " HP: %d MDC: %d",
            borg["IQ"], borg["ME"], borg["MA"], borg["PS"],
            borg["PP"], borg["PE"], borg["PB"], borg["Spd"],
            hp, mdc));
    } else {
        write(sprintf(
            " IQ:%-3d ME:%-3d MA:%-3d PS:%-3d\n"
            " PP:%-3d PE:%-3d PB:%-3d Spd:%-3d\n"
            " HP: %d SDC: %d",
            borg["IQ"], borg["ME"], borg["MA"], borg["PS"],
            borg["PP"], borg["PE"], borg["PB"], borg["Spd"],
            hp, sdc));
    }
}

// ── OCC selection commands ─────────────────────────────────────────────────

int list_occs_cmd(string str) {
    string *occs, *available;
    int i;

    if((string)this_player()->getenv("creation_step") == "race") {
        if((string)this_player()->getenv("stats_rolled") != "1") {
            write("Roll your attributes first. Type: roll\n");
            return 1;
        }
        show_race_list();
        return 1;
    }
    /* Bare "list" during OCC selection lists OCCs; "list occs" also works. */
    if((!str || str == "") &&
       (string)this_player()->getenv("awaiting_occ") == "1")
        str = "occs";
    if(!str || lower_case(str) != "occs") {
        notify_fail("Type: list\n");
        return 0;
    }
    if((string)this_player()->getenv("awaiting_occ") != "1") {
        notify_fail("You are not at the OCC selection stage.\n");
        return 0;
    }
    occs = (string *)OCC_D->query_occs();
    available = ({});
    for(i = 0; i < sizeof(occs); i++) {
        if((int)OCC_D->query_occ_available(occs[i], this_player()))
            available += ({ occs[i] });
    }
    show_occ_list(available);
    return 1;
}

private void choose_no_occ() {
    this_player()->remove_env("awaiting_occ");
    this_player()->setenv("rifts_occ", "none");
    this_player()->setenv("rifts_occ_flags", "magic,psychic,borg,cybernetic");
    write("You choose to walk the world as a racial character with no OCC.");
    do_elective_skills_start();
}

int no_occ_cmd(string str) {
    if(!str || lower_case(str) != "occ") {
        notify_fail("Did you mean <none>?\n");
        return 0;
    }
    if((string)this_player()->getenv("awaiting_occ") != "1") {
        notify_fail("You are not at the OCC selection stage.\n");
        return 0;
    }
    choose_no_occ();
    return 1;
}
// str is everything after "pick", e.g. "occ headhunter" or "occ ley line walker".
int do_occ_pick(string str) {
    string occ_name;
    mapping occ_data;
    mapping reqs;
    string *req_keys;
    string cs_al;
    int i, tmp, val;

    /* Allow "reroll" during OCC selection to re-roll stats. */
    if(str && lower_case(str) == "reroll") {
        if((int)this_player()->query_rolls() >= 5) {
            write("You have used all five rolls. Type none or choose an available OCC.");
            return 1;
        }
        do_rolls();
        offer_occ_or_reroll();
        return 1;
    }

    // Accept "pick occ <name>" or bare "pick <name>" when in OCC selection state.
    if(str && strlen(str) >= 3 && lower_case(str[0..2]) == "occ") {
        occ_name = (strlen(str) > 4) ? lower_case(str[4..]) : "";
    } else if(str && strlen(str) > 0) {
        occ_name = lower_case(str);
    } else {
        write("Type an OCC name. Type list to see available classes.");
        write("Type none to play as a racial character only.");
        return 1;
    }
    if(!strlen(occ_name)) {
        write("Type an OCC name. Example: headhunter");
        return 1;
    }

    occ_data = (mapping)OCC_D->query_occ(occ_name);
    if(!occ_data) {
        write("'" + occ_name + "' is not a valid OCC.");
        write("Type list to see available classes.");
        return 1;
    }

    /* Check race and unlock availability */
    if(!creatorp(this_player()) &&
       !(int)OCC_D->query_occ_available(occ_name, this_player())) {
        write("That OCC is not available to your race or you do not meet its unlock requirements.");
        write("Type list to see what is available to you.");
        return 1;
    }

    /* CS alignment restriction: Miscreant, Aberrant, Diabolic cannot join CS military */
    cs_al = (string)this_player()->getenv("rifts_alignment");
    if(!creatorp(this_player()) &&
       (occ_name == "cs grunt" || occ_name == "cs dead boy" ||
        occ_name == "cs ranger" || occ_name == "cs military specialist" ||
        occ_name == "cs technical officer" || occ_name == "cs psi-stalker") &&
       (cs_al == "Miscreant" || cs_al == "Aberrant" || cs_al == "Diabolic")) {
        write("The Coalition States enforces strict codes of conduct.");
        write("Miscreant, Aberrant, and Diabolic characters may not join the CS military.");
        return 1;
    }

    /* Check attribute requirements (creators/admins bypass entirely) */
    if(!creatorp(this_player())) {
        reqs = (mapping)occ_data["attribute_requirements"];
        if(reqs && sizeof(reqs)) {
            req_keys = keys(reqs);
            tmp = sizeof(req_keys);
            for(i = 0; i < tmp; i++) {
                val = (int)this_player()->query_stats(req_keys[i]);
                if(val < reqs[req_keys[i]]) {
                    write(sprintf(
                        "You do not meet the attribute requirements for %s.\n"
                        "Required: %s %d (yours: %d)",
                        capitalize(occ_name),
                        req_keys[i], reqs[req_keys[i]], val));
                    return 1;
                }
            }
        }
    }

    // Apply OCC - setenv persists across logins; set_class for channel/system use
    this_player()->remove_env("awaiting_occ");
    this_player()->setenv("rifts_occ", occ_name);
    this_player()->set_class(occ_name);

    // Store OCC flags (magic, psychic, borg, etc.) for combat and score use
    {
        string *occ_flags;
        string race_flag_str;

        occ_flags = (string *)occ_data["flags"];
        if(!occ_flags) occ_flags = ({});
        this_player()->setenv("rifts_occ_flags", implode(occ_flags, ","));

        // Merge race flags with occ flags for a combined character flag set
        race_flag_str = (string)this_player()->getenv("rifts_flags");
        if(!race_flag_str) race_flag_str = "";
        if(sizeof(occ_flags) && sizeof(race_flag_str))
            this_player()->setenv("rifts_flags",
                race_flag_str + "," + implode(occ_flags, ","));
        else if(sizeof(occ_flags))
            this_player()->setenv("rifts_flags", implode(occ_flags, ","));
    }

    // Store PPE (magic power reserve)
    {
        int ppe, ppe_lvl, me;
        ppe = (int)occ_data["ppe_base"];
        ppe_lvl = (int)occ_data["ppe_per_level"];
        if(ppe > 0) {
            // Add ME attribute to starting PPE as Palladium specifies /* approx */
            me = (int)this_player()->query_stats("ME");
            ppe += me;
            this_player()->set_stats("PPE", ppe);
            this_player()->set_stats("max_PPE", ppe);
        }
        this_player()->set_stats("PPE_per_level", ppe_lvl);
    }

    // Store ISP (psionic power reserve)
    {
        int isp, isp_lvl, me;
        isp = (int)occ_data["isp_base"];
        isp_lvl = (int)occ_data["isp_per_level"];
        if(isp > 0) {
            // Add ME attribute to starting ISP as Palladium specifies /* approx */
            me = (int)this_player()->query_stats("ME");
            isp += me;
            this_player()->set_stats("ISP", isp);
            this_player()->set_stats("max_ISP", isp);
        }
        this_player()->set_stats("ISP_per_level", isp_lvl);
    }

    // Store starting equipment text for later display or spawning
    {
        string equip;
        equip = (string)occ_data["starting_equipment"];
        if(equip && sizeof(equip))
            this_player()->setenv("rifts_equipment", equip);
    }

    // Store starting skills list and apply to player via skill daemon
    {
        string *bskills;
        bskills = (string *)occ_data["base_skills"];
        if(bskills && sizeof(bskills)) {
            this_player()->setenv("rifts_skills", implode(bskills, ";"));
            RIFTS_SKILLS_D->apply_occ_skills(this_player(), bskills);
        }
    }

    write("\nYou have chosen: " + capitalize(occ_name));
    write((string)occ_data["description"]);
    write("HP per level up: " + (string)occ_data["hp_per_level"]);

    // Show PPE/ISP summary if relevant
    {
        int ppe, isp;
        ppe = (int)this_player()->query_stats("PPE");
        isp = (int)this_player()->query_stats("ISP");
        if(ppe > 0) write("Starting PPE: " + ppe);
        if(isp > 0) write("Starting ISP: " + isp);
    }

    do_elective_skills_start();
    return 1;
}

// ── Race selection ──────────────────────────────────────────────────────────

int pick(string str) {
    string step;

    if(str) str = lower_case(str);
    step = (string)this_player()->getenv("creation_step");

    if(step == "stats") {
        if(str == "roll" || str == "reroll") {
            do_plain_rolls();
            return 1;
        }
        write("Type roll to roll attributes, or reroll to roll again (up to 4 rerolls).\n");
        return 1;
    }

    // Re-route to OCC selection if awaiting_occ env var is set
    if((string)this_player()->getenv("awaiting_occ") == "1") {
        return do_occ_pick(str);
    }
    // Re-route to elective skill selection
    if((string)this_player()->getenv("awaiting_elective_skills") == "1") {
        if(str && strlen(str) >= 5 && lower_case(str[0..4]) == "skill") {
            string sk;
            sk = (strlen(str) > 6) ? str[6..] : "";
            return do_elective_skill_pick(sk);
        }
    }
    if((string)this_player()->getenv("awaiting_secondary_skills") == "1") {
        if(str && strlen(str) >= 5 && lower_case(str[0..4]) == "skill") {
            string sk;
            sk = (strlen(str) > 6) ? str[6..] : "";
            return do_secondary_skill_pick(sk);
        }
    }
    if((string)this_player()->getenv("awaiting_alignment") == "1") {
        write("Please choose your alignment first. Type its name, for example: scrupulous");
        return 1;
    }
    if(step != "race") {
        write("You are not at race selection yet. Follow the creation prompts.\n");
        return 1;
    }
    if((string)this_player()->getenv("stats_rolled") != "1") {
        write("Roll your attributes first. Type: roll\n");
        return 1;
    }
    /* "pick reroll" after the first roll used to fall through to race
       matching and fail with "not a valid race". Reroll stays legal
       until a race is chosen. */
    if(str == "roll" || str == "reroll") {
        do_plain_rolls();
        return 1;
    }
    if(!str) {
        write("To pick a race, type its name. Example: human");
        return 1;
    }
    return do_race_pick(str);
}

/* Silent validity check so the catch-all only claims real race names. */
private int valid_race_choice(string str) {
    if(!str || !strlen(str)) return 0;
    if((int)RIFTS_D->is_admin_race(str) && !creatorp(this_player()))
        return 0;
    if(member_array(str, RETIRED_LEGACY_RACES) != -1 &&
       !creatorp(this_player()))
        return 0;
    if(member_array(str, (string *)RACE_D->query_races()) == -1 &&
       !(int)RIFTS_D->is_rifts_race(str))
        return 0;
    return 1;
}

private int do_race_pick(string str) {
    string Class;

    Class = "child";
    // Legacy gender restrictions for non-Rifts races
    if(str == "satyr" && (string)this_player()->query_gender() != "male") {
        write("You must be male to be a satyr.\nPick again.");
        return 1;
    }
    else if(str == "nymph" && (string)this_player()->query_gender() != "female") {
        write("You must be female to be a nymph.\nPick again.");
        return 1;
    }
    // Admin races (godling, etc.) require creator rank
    if((int)RIFTS_D->is_admin_race(str) && !creatorp(this_player())) {
        write("That is not a valid race.\nType list to see available races.\n");
        return 1;
    }
    /* Retired legacy NM3 races are creator-only. */
    if(member_array(str, RETIRED_LEGACY_RACES) != -1 &&
       !creatorp(this_player())) {
        write("That is not a valid race.\nType list to see available races.\n");
        return 1;
    }
    if(member_array(str, (string *)RACE_D->query_races()) == -1 &&
       !RIFTS_D->is_rifts_race(str)) {
        write("That is not a valid race.\nType list to see available races.\n");
        return 1;
    }
    this_player()->set_race(str);
    this_player()->new_body();
    this_player()->set_class(Class);
    if((string)this_player()->query_gender() == "male")
        this_player()->setenv("TITLE", "Newbie $N the boy");
    else
        this_player()->setenv("TITLE", "Newbie $N the girl");
    this_player()->init_skills(Class);

    if((int)RIFTS_D->is_rifts_race(str))
        write("\n" + (string)RIFTS_D->query_race_description(str) + "\n");
    /* Rifts races: replace the plain 3d6 preview with racial dice + pools.
     * Non-Rifts (legacy creator testing) keep body pools from the preview. */
    if((int)RIFTS_D->is_rifts_race(str)) {
        write("Your race reshapes your attributes.");
        apply_rifts_race_attributes(str);
    } else
        apply_race_body_pools(str);

    if(str == "secondary vampire" || str == "wild vampire")
        this_player()->set_property("visible_race", "human");

    this_player()->setenv("creation_step", "alignment");
    do_alignment_start();
    return 1;
}

// offer_occ_or_reroll - called after rolling stats when awaiting_occ is set.
// If no OCC is available due to stat requirements (and rolls remain), offers a reroll.
void offer_occ_or_reroll() {
    string *occs, *available;
    mapping occ_data, reqs;
    string *req_keys;
    int i, j, val, ok, rolls_left;
    string race;

    race = (string)this_player()->query_race();
    if(!(int)RIFTS_D->is_rifts_race(race)) return;

    occs = (string *)OCC_D->query_occs();
    available = ({});
    for(i = 0; i < sizeof(occs); i++) {
        occ_data = (mapping)OCC_D->query_occ(occs[i]);
        if(!occ_data) continue;
        if(!(int)OCC_D->query_occ_available(occs[i], this_player())) continue;
        reqs = (mapping)occ_data["attribute_requirements"];
        ok = 1;
        if(reqs && sizeof(reqs)) {
            req_keys = keys(reqs);
            for(j = 0; j < sizeof(req_keys); j++) {
                val = (int)this_player()->query_stats(req_keys[j]);
                if(val < reqs[req_keys[j]]) { ok = 0; break; }
            }
        }
        if(ok) available += ({ occs[i] });
    }

    rolls_left = 5 - (int)this_player()->query_rolls();

    write("\nYou are eligible to choose an Occupational Character Class.");
    if(!sizeof(available) && rolls_left > 0) {
        write("Your current stats do not qualify you for any OCC.");
        write("Rolls remaining: " + rolls_left);
        write("Type reroll to re-roll. Type none to play as racial only.");
    } else if(!sizeof(available)) {
        write("Your stats do not qualify for any OCC. You may play as a racial character.");
        write("Type: none\n");
    } else {
        show_occ_list(available);
        if(rolls_left > 0)
            write("Type reroll to re-roll stats (" + rolls_left + " roll(s) remaining).");
    }
}

// ── Alignment selection ────────────────────────────────────────────────────
//
// Presented to Rifts characters after race and OCC are finalized.
// Numeric alignment stored in Nightmare's standard -1500..1500 range;
// Palladium name stored in "rifts_alignment" env var.

void do_alignment_start() {
    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race())) {
        finish_creation();
        return;
    }
    this_player()->setenv("awaiting_alignment", "1");
    this_player()->setenv("creation_step", "alignment");
    write("\n=== STEP 4: CHOOSE YOUR ALIGNMENT ===");
    write(" principled     Honest, law-abiding hero");
    write(" scrupulous     Good but bends rules for the greater good");
    write(" unprincipled   Selfish but not evil, values freedom");
    write(" anarchist      No rules, but not necessarily cruel");
    write(" miscreant      Self-serving, uses others without remorse");
    write(" aberrant       Evil but bound by a personal code of honor");
    write(" diabolic       Pure evil, no remorse, no limits");
    write("\nType your choice. Example: scrupulous");
}

/* alignment_cmd - parses an alignment name during character creation. */
int alignment_cmd(string str) {
    int al_val;
    string al_name;

    if((string)this_player()->getenv("awaiting_alignment") != "1") {
        notify_fail("You are not at the alignment selection stage.\n");
        return 0;
    }
    if(!str || !strlen(str)) {
        write("Type your alignment choice. Example: scrupulous");
        return 1;
    }
    str = lower_case(str);
    if(str == "principled")        { al_name = "Principled";   al_val = 900; }
    else if(str == "scrupulous")   { al_name = "Scrupulous";   al_val = 600; }
    else if(str == "unprincipled") { al_name = "Unprincipled"; al_val = 200; }
    else if(str == "anarchist")    { al_name = "Anarchist";    al_val = -100; }
    else if(str == "miscreant")    { al_name = "Miscreant";    al_val = -500; }
    else if(str == "aberrant")     { al_name = "Aberrant";     al_val = -900; }
    else if(str == "diabolic")     { al_name = "Diabolic";     al_val = -1200; }
    else {
        write("Please type one of: principled, scrupulous, unprincipled,");
        write("anarchist, miscreant, aberrant, diabolic.");
        return 1;
    }
    this_player()->remove_env("awaiting_alignment");
    this_player()->set_alignment(al_val);
    this_player()->setenv("rifts_alignment", al_name);
    write("\nAlignment chosen: " + al_name);

    {
        string race;
        race = lower_case((string)this_player()->query_race());
        if(race == "dog boy") {
            string *occ_flags;
            this_player()->setenv("dog_boy", "1");
            this_player()->setenv("rifts_occ", "coalition soldier");
            this_player()->set_class("coalition soldier");
            occ_flags = ({ "military", "coalition" });
            this_player()->setenv("rifts_occ_flags", implode(occ_flags, ","));
            write("Dog Boys serve the Coalition States as military scouts.\n"
                  "You have been assigned the Coalition Soldier role.");
            do_elective_skills_start();
            return 1;
        }
        if(race == "cs psi-stalker" || race == "wild psi-stalker") {
            this_player()->setenv("rifts_occ", "none");
            this_player()->setenv("rifts_occ_flags", "magic,psychic,borg,cybernetic");
        }
    }

    if((int)RIFTS_D->is_rifts_race((string)this_player()->query_race()) &&
       (int)OCC_D->query_occ_eligible((string)this_player()->query_race())) {
        this_player()->setenv("awaiting_occ", "1");
        this_player()->setenv("creation_step", "occ");
        offer_occ_or_reroll();
    } else {
        this_player()->setenv("rifts_occ", "none");
        this_player()->setenv("rifts_occ_flags", "magic,psychic,borg,cybernetic");
        do_elective_skills_start();
    }
    return 1;
}

// ── Attribute rolling ───────────────────────────────────────────────────────

void do_rolls() {
    mapping borg;
    string race;
    int i, tmp;
    string *keys_arr;

    if((int)this_player()->query_rolls() >= 5) {
        write("You can roll your stats no more.");
        return;
    }
    write("\nYou roll your attributes.");
    race = (string)this_player()->query_race();

    if((int)RIFTS_D->is_rifts_race(race)) {
        apply_rifts_race_attributes(race);
    }
    else {
        keys_arr = keys(borg = (mapping)RACE_D->do_rolls(race));
        for(i=0, tmp=sizeof(keys_arr); i<tmp; i++)
            this_player()->set_stats(keys_arr[i], borg[keys_arr[i]]);
    }
    this_player()->set_rolls(this_player()->query_rolls() + 1);
}

/* Plain "roll" / "reroll" typed with no verb prefix. During the OCC
   step a reroll uses racial dice; before race selection it repeats
   the plain 3d6 preview. Returns 0 outside chargen so the normal
   roll command still works. */
int roll_cmd(string str) {
    string step;

    if(!this_player()) return 0;
    step = (string)this_player()->getenv("creation_step");
    if((string)this_player()->getenv("awaiting_occ") == "1")
        return do_occ_pick("reroll");
    if(step == "stats" || step == "race") {
        do_plain_rolls();
        return 1;
    }
    return 0;
}

/* Catch-all for plain string answers: zone names, race names,
   alignment names, OCC names, and skill names typed with no verb.
   With add_action(fn, "", 1) the driver passes only the text after
   the first word; the first word arrives via query_verb(), so the
   full line is rebuilt here. Returns 0 for anything unrecognized so
   normal commands still reach the player's command hook. */
int chargen_catch(string str) {
    string step;
    string low;
    string verb;

    if(!this_player()) return 0;
    verb = query_verb();
    if(!verb || !strlen(verb)) return 0;
    step = (string)this_player()->getenv("creation_step");
    if(!step || step == "" || step == "done") return 0;
    low = lower_case(verb);
    if(str && strlen(str)) low += " " + lower_case(str);

    /* Bare roll/reroll must win even if the catch-all is tried before
       the named add_action("roll_cmd", "reroll") binding. */
    if(step == "stats" || step == "race") {
        if(low == "roll" || low == "reroll")
            return roll_cmd(0);
    }

    if(step == "region" ||
       (string)this_player()->getenv("awaiting_region") == "1") {
        if(low == "americas" || low == "chitown" ||
           low == "europe" || low == "camelot" ||
           low == "atlantis" || low == "splynn")
            return start_cmd(low);
        return 0;
    }
    if((string)this_player()->getenv("awaiting_alignment") == "1") {
        if(low == "principled" || low == "scrupulous" ||
           low == "unprincipled" || low == "anarchist" ||
           low == "miscreant" || low == "aberrant" ||
           low == "diabolic")
            return alignment_cmd(low);
        return 0;
    }
    if((string)this_player()->getenv("awaiting_occ") == "1") {
        if(low == "none" || low == "no occ") {
            choose_no_occ();
            return 1;
        }
        if(OCC_D->query_occ(low))
            return do_occ_pick(low);
        return 0;
    }
    if((string)this_player()->getenv("awaiting_elective_skills") == "1") {
        if(RIFTS_SKILLS_D->query_rifts_skill(low))
            return do_elective_skill_pick(low);
        return 0;
    }
    if((string)this_player()->getenv("awaiting_secondary_skills") == "1") {
        if(RIFTS_SKILLS_D->query_rifts_skill(low))
            return do_secondary_skill_pick(low);
        return 0;
    }
    if(step == "race") {
        if((string)this_player()->getenv("stats_rolled") != "1") return 0;
        if(valid_race_choice(low))
            return do_race_pick(low);
        return 0;
    }
    return 0;
}

// ── Region selection ────────────────────────────────────────────────────────

void do_region_start() {
    creation_prompt("region");
}

/* Legacy alias: 'region <zone>' behaves like typing the zone name. */
int region_cmd(string str) {
    if(str && strlen(str)) return start_cmd(str);
    notify_fail("Type your choice: americas, europe, or atlantis.\n");
    return 0;
}

int start_cmd(string str) {
    string region;
    string dest_room;

    if((string)this_player()->getenv("creation_step") != "region" &&
       (string)this_player()->getenv("awaiting_region") != "1")
        return 0;
    if(!str || !strlen(str)) {
        write("Type your choice: americas, europe, or atlantis.\n");
        return 1;
    }
    str = lower_case(str);
    if(str == "americas" || str == "chitown") {
        region = "americas";
        dest_room = "/domains/Praxis/rifts_welcome";
        write("The Americas. You will begin on the outskirts of Praxis.");
    } else if(str == "atlantis" || str == "splynn") {
        region = "atlantis";
        dest_room = "/domains/Praxis/areas/splynn/splynn_entry";
        if(file_size(dest_room + ".c") <= 0)
            dest_room = "/domains/Praxis/rifts_welcome";
        write("Atlantis. The alien shores of Splynn await.");
    } else if(str == "europe" || str == "camelot") {
        region = "europe";
        dest_room = "/domains/newcamelot/areas/camelot_gate";
        if(file_size(dest_room + ".c") <= 0)
            dest_room = "/domains/Praxis/rifts_welcome";
        write("Europe. New Camelot and the old world's magic await.");
    } else {
        write("Type your choice: americas, europe, or atlantis.\n");
        return 1;
    }
    this_player()->remove_env("awaiting_region");
    this_player()->setenv("home_region", region);
    this_player()->setenv("rifts_region", region);
    this_player()->set_primary_start(dest_room);
    this_player()->setenv("creation_step", "stats");
    this_player()->set_rolls(0);
    creation_prompt("stats");
    return 1;
}

// ── Elective skill selection ────────────────────────────────────────────────

private int get_elective_count(string occ) {
    mapping occ_data;
    int picks;

    if(!occ || occ == "" || occ == "none") return 0;
    occ_data = (mapping)OCC_D->query_occ(lower_case(occ));
    if(!occ_data) return 0;
    picks = (int)occ_data["occ_skill_picks"];
    return picks > 0 ? picks : 0;
}

private string map_occ_pick_category(string occ_cat) {
    string c;

    if(!occ_cat || !sizeof(occ_cat)) return "";
    c = lower_case(occ_cat);
    if(c == "rogue") return "espionage";
    if(c == "w.p.") return "weapons";
    if(c == "wilderness") return "survival";
    if(c == "pilot related") return "pilot";
    if(c == "science") return "technical";
    return c;
}

private string normalize_skill_category(string skill_cat) {
    string c;

    if(!skill_cat || !sizeof(skill_cat)) return "";
    c = lower_case(skill_cat);
    if(c == "rogue") return "espionage";
    if(c == "science") return "technical";
    return c;
}

private string get_elective_cats(string occ) {
    mapping occ_data;
    string *cats;
    string *mapped;
    int i;

    if(!occ || occ == "" || occ == "none") return "";
    occ_data = (mapping)OCC_D->query_occ(lower_case(occ));
    if(!occ_data) return "";
    cats = (string *)occ_data["occ_skills"];
    if(!cats || !sizeof(cats)) return "any";
    mapped = ({});
    for(i = 0; i < sizeof(cats); i++)
        mapped += ({ map_occ_pick_category(cats[i]) });
    return implode(mapped, ",");
}

private int get_secondary_count(string occ) {
    mapping occ_data;
    int picks;

    if(!occ || occ == "" || occ == "none") return 0;
    occ_data = (mapping)OCC_D->query_occ(lower_case(occ));
    if(!occ_data) return 0;
    picks = (int)occ_data["secondary_skills"];
    return picks > 0 ? picks : 0;
}

void do_elective_skills_start() {
    string occ;
    int count;
    string cats;

    occ = lower_case((string)this_player()->getenv("rifts_occ"));
    if(!occ) occ = "none";
    count = get_elective_count(occ);
    if(count <= 0) {
        do_secondary_skills_start();
        return;
    }
    cats = get_elective_cats(occ);
    this_player()->setenv("awaiting_elective_skills", "1");
    this_player()->setenv("creation_step", "elective");
    this_player()->setenv("elective_picks_remaining", sprintf("%d", count));
    this_player()->setenv("elective_cats_allowed", cats);
    write("\n=== CHOOSE YOUR ELECTIVE SKILLS ===");
    write("You have " + count + " elective skill pick(s).");
    if(cats == "any")
        write("You may pick from any skill category.");
    else
        write("Allowed categories: " + cats);
    write("\nType 'skills <category>' to list skills. Example: skills weapons");
    write("Then type a skill name to select it. Example: tracking");
    write("Available categories: weapons, pilot, military, espionage, technical, survival, physical, lore");
}

int elective_list_cmd(string str) {
    string *skill_list;
    string *labels;
    string cats;
    string cat;
    int i;
    int secondary_mode;

    secondary_mode = ((string)this_player()->getenv("awaiting_secondary_skills") == "1");
    if(!secondary_mode && (string)this_player()->getenv("awaiting_elective_skills") != "1") {
        notify_fail("That command is only available during skill selection.\n");
        return 0;
    }
    if(!str || !strlen(str)) {
        write("Type: skills <category> Example: skills weapons");
        return 1;
    }
    cat = lower_case(str);
    if(!secondary_mode) {
        cats = (string)this_player()->getenv("elective_cats_allowed");
        if(cats && cats != "any" && strsrch(cats, cat) == -1) {
            write("You cannot pick from that category. Allowed: " + cats);
            return 1;
        }
    }
    skill_list = (string *)RIFTS_SKILLS_D->query_skills_by_category(cat);
    if(!skill_list || !sizeof(skill_list)) {
        write("No skills found in category: " + cat);
        return 1;
    }
    write("Skills in category " + cat + ":");
    labels = ({});
    for(i = 0; i < sizeof(skill_list); i++)
        labels += ({ capitalize(skill_list[i]) });
    write(format_page(labels, 4));
    write("\nType a skill name to select it.");
    return 1;
}

int do_elective_skill_pick(string str) {
    string skill_name;
    string cats;
    string cat;
    mapping sdata;
    int remaining;

    if((string)this_player()->getenv("awaiting_elective_skills") != "1") return 0;
    if(!str || !strlen(str)) {
        write("Type a skill name. Example: tracking");
        return 1;
    }
    skill_name = lower_case(str);
    sdata = (mapping)RIFTS_SKILLS_D->query_rifts_skill(skill_name);
    if(!sdata) {
        write("'" + skill_name + "' is not a recognized skill. Type 'skills <category>' to browse.");
        return 1;
    }
    cats = (string)this_player()->getenv("elective_cats_allowed");
    cat = normalize_skill_category((string)sdata["category"]);
    if(cats && cats != "any" && strsrch(lower_case(cats), cat) == -1) {
        write("That skill is not in your allowed categories. Allowed: " + cats);
        return 1;
    }
    if((int)this_player()->query_skill(skill_name) > 0) {
        write("You already have that skill.");
        return 1;
    }
    RIFTS_SKILLS_D->grant_skill(this_player(), skill_name, 0);
    remaining = to_int((string)this_player()->getenv("elective_picks_remaining")) - 1;
    this_player()->setenv("elective_picks_remaining", sprintf("%d", remaining));
    write("Skill added: " + capitalize(skill_name));
    if(remaining <= 0) {
        this_player()->remove_env("awaiting_elective_skills");
        this_player()->remove_env("elective_picks_remaining");
        this_player()->remove_env("elective_cats_allowed");
        write("All elective skills chosen.");
        do_secondary_skills_start();
    } else {
        write("Picks remaining: " + remaining);
    }
    return 1;
}

void do_secondary_skills_start() {
    string occ;
    int count;

    occ = lower_case((string)this_player()->getenv("rifts_occ"));
    if(!occ) occ = "none";
    count = get_secondary_count(occ);
    if(count <= 0) {
        finish_creation();
        return;
    }
    this_player()->setenv("awaiting_secondary_skills", "1");
    this_player()->setenv("creation_step", "secondary");
    this_player()->setenv("secondary_picks_remaining", sprintf("%d", count));
    write("\n=== CHOOSE YOUR SECONDARY SKILLS ===");
    write("You have " + count + " secondary skill pick(s).");
    write("You may pick from any skill category.");
    write("\nType 'skills <category>' to list skills. Example: skills weapons");
    write("Then type a skill name to select it. Example: tracking");
    write("Available categories: weapons, pilot, military, espionage, technical, survival, physical, lore");
}

int do_secondary_skill_pick(string str) {
    string skill_name;
    mapping sdata;
    int remaining;

    if((string)this_player()->getenv("awaiting_secondary_skills") != "1") return 0;
    if(!str || !strlen(str)) {
        write("Type a skill name. Example: tracking");
        return 1;
    }
    skill_name = lower_case(str);
    sdata = (mapping)RIFTS_SKILLS_D->query_rifts_skill(skill_name);
    if(!sdata) {
        write("'" + skill_name + "' is not a recognized skill. Type 'skills <category>' to browse.");
        return 1;
    }
    if((int)this_player()->query_skill(skill_name) > 0) {
        write("You already have that skill.");
        return 1;
    }
    RIFTS_SKILLS_D->grant_skill(this_player(), skill_name, 0);
    remaining = to_int((string)this_player()->getenv("secondary_picks_remaining")) - 1;
    this_player()->setenv("secondary_picks_remaining", sprintf("%d", remaining));
    write("Skill added: " + capitalize(skill_name));
    if(remaining <= 0) {
        this_player()->remove_env("awaiting_secondary_skills");
        this_player()->remove_env("secondary_picks_remaining");
        write("All secondary skills chosen.");
        finish_creation();
    } else {
        write("Picks remaining: " + remaining);
    }
    return 1;
}

// ── Creation finalisation ───────────────────────────────────────────────────

void finish_creation() {
    string region;
    string dest;
    object player;

    player = this_player();
    RIFTS_START_D->grant_starting_package(player);
    LANGUAGE_D->grant_native_languages(player);
    if((int)RIFTS_D->is_rifts_race((string)player->query_race()))
        player->setenv("TITLE", "");

    player->remove_env("awaiting_occ");
    player->remove_env("awaiting_alignment");
    player->remove_env("awaiting_region");
    player->remove_env("awaiting_elective_skills");
    player->remove_env("elective_picks_remaining");
    player->remove_env("elective_cats_allowed");
    player->remove_env("awaiting_secondary_skills");
    player->remove_env("secondary_picks_remaining");
    player->remove_env("stats_rolled");
    player->setenv("creation_step", "done");
    player->setenv("chargen_complete", "1");

    dest = (string)player->query_primary_start();
    if((int)RIFTS_D->is_rifts_race((string)player->query_race())) {
        region = (string)player->getenv("rifts_region");
        if(!region || !strlen(region)) region = "americas";
        if(!dest || !strlen(dest)) {
            if(region == "americas")
                dest = "/domains/Praxis/rifts_welcome";
            else if(region == "atlantis")
                dest = "/domains/Praxis/areas/splynn/splynn_entry";
            else if(region == "europe")
                dest = "/domains/newcamelot/areas/camelot_gate";
            else
                dest = "/domains/Praxis/rifts_welcome";
        }
        if(file_size(dest + ".c") <= 0)
            dest = "/domains/Praxis/rifts_welcome";
        player->set_primary_start(dest);
        write("\nA Rift tears open around you and reality reassembles.");
        write("You step onto Rifts Earth. Welcome, " + capitalize((string)player->query_race()) + ".");
        player->move_player(dest);
    } else {
        if(!dest || !strlen(dest)) dest = ROOM_START;
        player->set_primary_start(dest);
        write("\nYou enter the world.");
        player->move_player(dest);
    }
    catch("/cmds/mortal/_score"->cmd_score(""));
    /* News was deferred in std/user.c's setup() specifically to avoid
       racing the chargen prompts (input_to() always beats add_action-
       based commands for the next raw input). Nothing is waiting on the
       player's next line here, so it's safe to show now. Skip the
       end_news() room re-describe: move_player already showed the room. */
    player->set_property("skip_news_room_look", 1);
    NEWS_D->read_news();
    player->save_player();
}

// ── Race list ───────────────────────────────────────────────────────────────

private void show_race_list() {
    string *rifts_races, *all_races, *other_races;

    rifts_races = (string *)RIFTS_D->query_rifts_races();
    all_races = (string *)RACE_D->query_races();
    other_races = all_races - rifts_races;
    /* Retired legacy NM3 races are hidden from mortals entirely. */
    if(!creatorp(this_player()))
        other_races -= RETIRED_LEGACY_RACES;

    write("\n=== RIFTS EARTH: PLAYABLE RACES ===");
    write(format_page(rifts_races, 3));
    if(sizeof(other_races)) {
        write("=== Legacy races (creator testing only) ===");
        message("Ninfo", format_page(other_races, 4), this_player());
    }
    write("=====================================");
    write("Type a race name to choose it. Example: human");
}

private void show_occ_list(string *available) {
    if(!available || !sizeof(available)) {
        write("(No OCCs available for your race with current stats.)");
        write("Type: no occ   to continue as a racial character only.");
        return;
    }
    write("\n=== AVAILABLE OCCUPATIONAL CHARACTER CLASSES ===");
    write(format_page(available, 3));
    write("Type an OCC name to choose it. Example: headhunter");
    write("Type: no occ   to play as a racial character only.");
}

int read(string str) {
    string step;

    step = (string)this_player()->getenv("creation_step");
    if(step != "race") {
        notify_fail("Type list during race selection (after rolling attributes).\n");
        return 0;
    }
    if(!str) {
        notify_fail("What do you want to read? Try: read list\n");
        return 0;
    }
    if(str != "list") {
        notify_fail("That is not here to be read.\n");
        return 0;
    }
    if((string)this_player()->getenv("stats_rolled") != "1") {
        write("Roll your attributes first. Type: roll\n");
        return 1;
    }

    show_race_list();
    return 1;
}

void reset() {
    ::reset();
    if(!present("gamemaster", this_object()))
        clone_object("/domains/Praxis/npcs/chargen_guide")->move(this_object());
}
