/* /daemon/rifts_start_d.c */
/* Central starting package daemon for Rifts characters. */
/* Called from setter.c finish_creation() after race/OCC/alignment selection. */

#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

/* ── Equipment helpers ────────────────────────────────────────────────────── */

private void give_item(object player, string file) {
    object ob;
    ob = clone_object(EQ_PATH + file);
    if(ob) ob->move(player);
}

private void give_item_path(object player, string full_path) {
    object ob;
    ob = clone_object(full_path);
    if(ob) ob->move(player);
}

private void deferred_wear_armor(object player, string keyword) {
    object *inv;
    int i;

    if(!player || !objectp(player) || !keyword) return;
    inv = all_inventory(player);
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i]) continue;
        if((int)inv[i]->is_armour() && inv[i]->id(keyword)) {
            catch(inv[i]->wear(keyword));
            return;
        }
    }
}

private void auto_wear_power_armor(object player, string occ) {
    string s;

    s = lower_case(occ);
    if(s == "glitter boy pilot")
        call_out("deferred_wear_armor", 1, player, "glitter boy");
    else if(s == "cs samas rpa pilot")
        call_out("deferred_wear_armor", 1, player, "samas");
}

private void give_credits(object player, int amount) {
    object card;
    card = clone_object("/obj/credcard");
    if(card) {
        card->init_arg(sprintf("%d", amount));
        card->move(player);
    }
}

private void give_equip(object player, string occ) {
    string s;
    s = lower_case(occ);

    switch(s) {

    /* ── CS / Coalition ──────────────────────────────────────────────────────── */
    case "cs grunt":
    case "cs dead boy":
    case "coalition soldier":
        give_item(player, "dead_boy_armor.c");
        give_item(player, "cp30_laser.c");
        give_item(player, "e_clip.c");
        give_item(player, "e_clip.c");
        give_item(player, "military_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    case "cs ranger":
    case "cs military specialist":
        give_item(player, "coalition_grunt_armor.c");
        give_item(player, "cp40_laser.c");
        give_item(player, "personal_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Headhunter ──────────────────────────────────────────────────────────── */
    case "headhunter":
        give_item(player, "c18_pistol.c");
        give_item(player, "vibro_knife.c");
        give_item(player, "environmental_body_armor.c");
        give_item(player, "handcuffs.c");
        give_credits(player, (random(6)+1 + random(6)+1 + random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Bounty Hunter ──────────────────────────────────────────────────────── */
    case "bounty hunter":
        give_item(player, "c18_pistol.c");
        give_item(player, "vibro_knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "handcuffs.c");
        give_item(player, "personal_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Mercenary / Merc Soldier ────────────────────────────────────────────── */
    case "mercenary":
    case "merc soldier":
        give_item(player, "c18_pistol.c");
        give_item(player, "vibro_knife.c");
        give_item(player, "environmental_body_armor.c");
        give_item(player, "e_clip.c");
        give_item(player, "e_clip.c");
        give_credits(player, (random(6)+1 + random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Special Forces ──────────────────────────────────────────────────────── */
    case "special forces (merc)":
        give_item(player, "environmental_body_armor.c");
        give_item(player, "cp40_laser.c");
        give_item(player, "vibro_knife.c");
        give_item(player, "personal_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Assassin ───────────────────────────────────────────────────────────── */
    case "assassin":
    case "master assassin":
    case "sunaj assassin":
        give_item(player, "vibro_knife.c");
        give_item(player, "vibro_knife.c");
        give_item(player, "leather_jacket.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Wilderness Scout ───────────────────────────────────────────────────── */
    case "wilderness scout":
    case "kankoran":
        give_item(player, "leather_jacket.c");
        give_item(player, "vibro_knife.c");
        give_item(player, "personal_radio.c");
        give_item(player, "rations.c");
        give_item(player, "rations.c");
        give_item(player, "rope.c");
        give_item(player, "trained_hawk.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Tribal Warrior ──────────────────────────────────────────────────────── */
    case "tribal warrior":
        give_item(player, "obsidian_blade.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "trained_hawk.c");
        give_item(player, "rations.c");
        give_credits(player, (random(6)+1) * 100);
        break;

    /* ── Cyber-Knight ───────────────────────────────────────────────────────── */
    case "cyber-knight":
    case "knight (europe)":
    case "royal knight":
        give_item(player, "short_sword.c");
        give_item_path(player, "/domains/newcamelot/equipment/knight_shield.c");
        give_item(player, "chain_mail.c");
        give_item(player, "knife.c");
        give_item(player, "backpack.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Juicer variants ────────────────────────────────────────────────────── */
    case "juicer":
    case "ninja juicer":
    case "delphi juicer":
    case "hyperion juicer":
        give_item(player, "environmental_body_armor.c");
        give_item(player, "vibro_sword.c");
        give_credits(player, (random(6)+1) * 100);
        break;

    /* ── Vagabond ───────────────────────────────────────────────────────────── */
    case "vagabond":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "rations.c");
        give_item(player, "rations.c");
        give_item(player, "rope.c");
        give_credits(player, (random(4)+1) * 100);
        break;

    /* ── City Rat ───────────────────────────────────────────────────────────── */
    case "city rat":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "personal_radio.c");
        give_credits(player, (random(6)+1) * 100);
        break;

    /* ── Rogue Scholar ───────────────────────────────────────────────────────── */
    case "rogue scholar":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "personal_radio.c");
        give_item(player, "field_journal.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Rogue Scientist ─────────────────────────────────────────────────────── */
    case "rogue scientist":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "science_kit.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Operator ───────────────────────────────────────────────────────────── */
    case "operator":
    case "cs technical officer":
        give_item(player, "c18_pistol.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "rations.c");
        give_item(player, "rations.c");
        give_item(player, "tool_kit.c");
        give_credits(player, (random(6)+1 + random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Techno-Wizard ───────────────────────────────────────────────────────── */
    case "techno-wizard":
        give_item(player, "c18_pistol.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "knife.c");
        give_item(player, "spell_components.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Ley Line Walker / Ley Line Rifter / Shaman ─────────────────────────── */
    case "ley line walker":
    case "ley line rifter":
    case "shaman":
    case "air warlock":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_credits(player, (random(6)+1) * 100);
        break;

    /* ── Mystic ──────────────────────────────────────────────────────────────── */
    case "mystic":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_credits(player, (random(6)+1) * 100);
        break;

    /* ── Shifter ─────────────────────────────────────────────────────────────── */
    case "shifter":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "ritual_components.c");
        give_credits(player, (random(6)+1) * 100);
        break;

    /* ── Body Fixer ──────────────────────────────────────────────────────────── */
    case "body fixer":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "first_aid_kit.c");
        give_item(player, "medical_bag.c");
        give_credits(player, (random(6)+1 + random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Cyber-Doc ───────────────────────────────────────────────────────────── */
    case "cyber-doc":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "first_aid_kit.c");
        give_item(player, "medical_bag.c");
        give_credits(player, (random(6)+1 + random(6)+1 + random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Gunfighter ─────────────────────────────────────────────────────────── */
    case "gunfighter":
        give_item(player, "c18_pistol.c");
        give_item(player, "c18_pistol.c");
        give_item(player, "vibro_knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "personal_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Palmer ─────────────────────────────────────────────────────────────── */
    case "palmer":
        give_item(player, "c18_pistol.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "id_card.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    case "palmer2":
        give_item(player, "c18_pistol.c");
        give_item(player, "c18_pistol.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "field_journal.c");
        give_item(player, "id_card.c");
        give_credits(player, (random(6)+1 + random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Necromancer ────────────────────────────────────────────────────────── */
    case "necromancer":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "bone_dust.c");
        give_credits(player, (random(4)+1) * 100);
        break;

    /* ── Borg ───────────────────────────────────────────────────────────────── */
    case "borg":
        player->set_property("built_in_mdc", 80);
        catch(BIONIC_D->install_bionic(player, "all", "full_borg_conversion"));
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── NGR ─────────────────────────────────────────────────────────────────── */
    case "ngr soldier":
        give_item(player, "ngr_body_armor.c");
        give_item(player, "ngr_rail_gun.c");
        give_item(player, "personal_radio.c");
        give_item(player, "vibro_knife.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    case "ngr mechanic":
        give_item(player, "ngr_body_armor.c");
        give_item(player, "c18_pistol.c");
        give_item(player, "tool_kit.c");
        give_item(player, "computer_pad.c");
        give_item(player, "personal_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Atlantean Tattoo Warrior ────────────────────────────────────────────── */
    case "tattoo warrior":
    case "tattooed man":
    case "atlantean nomad":
        give_item(player, "vibro_sword.c");
        give_item(player, "environmental_body_armor.c");
        give_item(player, "e_clip.c");
        give_item(player, "e_clip.c");
        give_credits(player, (random(4)+1 + random(4)+1) * 100);
        break;

    /* ── Kittani ─────────────────────────────────────────────────────────────── */
    case "kittani warrior":
        give_item_path(player,
            "/domains/Praxis/equipment/power_armor/kittani_power_armor.c");
        give_item(player, "kittani_laser.c");
        give_item(player, "vibro_knife.c");
        give_credits(player, (random(4)+1) * 1000);
        break;

    case "kittani field mechanic":
        give_item(player, "kittani_laser.c");
        give_item(player, "tool_kit.c");
        give_item(player, "computer_pad.c");
        give_item(player, "leather_jacket.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Glitter Boy Pilot ──────────────────────────────────────────────────── */
    case "glitter boy pilot":
        give_item_path(player,
            "/domains/Praxis/equipment/power_armor/GB_Glitter_Boy.c");
        give_item(player, "c18_pistol.c");
        give_item(player, "vibro_knife.c");
        give_item(player, "personal_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── CS SAMAS RPA Pilot ─────────────────────────────────────────────────── */
    case "cs samas rpa pilot":
        give_item_path(player,
            "/domains/Praxis/equipment/power_armor/PA-06A_SAMAS.c");
        give_item(player, "cp30_laser.c");
        give_item(player, "e_clip.c");
        give_item(player, "military_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    case "maxi-man":
    case "crazy":
        give_item(player, "environmental_body_armor.c");
        give_item(player, "vibro_knife.c");
        give_item(player, "cp30_laser.c");
        give_item(player, "e_clip.c");
        give_credits(player, (random(6)+1 + random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Forger ──────────────────────────────────────────────────────────────── */
    case "forger":
        give_item(player, "c18_pistol.c");
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "field_journal.c");
        give_item(player, "personal_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Freelance Spy ───────────────────────────────────────────────────────── */
    case "freelance spy":
        give_item(player, "c18_pistol.c");
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "personal_radio.c");
        give_item(player, "handcuffs.c");
        give_credits(player, (random(6)+1 + random(6)+1 + random(6)+1) * 100);
        break;

    /* ── ISS Peacekeeper ─────────────────────────────────────────────────────── */
    case "iss peacekeeper":
    case "ntset protector":
        give_item(player, "coalition_grunt_armor.c");
        give_item(player, "cp30_laser.c");
        give_item(player, "e_clip.c");
        give_item(player, "handcuffs.c");
        give_item(player, "personal_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── ISS Specter ─────────────────────────────────────────────────────────── */
    case "iss specter":
        give_item(player, "c18_pistol.c");
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "personal_radio.c");
        give_item(player, "handcuffs.c");
        give_credits(player, (random(6)+1 + random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Pirate (S.A.) ───────────────────────────────────────────────────────── */
    case "pirate (s.a.)":
    case "pirate":
        give_item(player, "vibro_sword.c");
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "rope.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Sailor (S.A.) ───────────────────────────────────────────────────────── */
    case "sailor (s.a.)":
    case "sailor":
        give_item(player, "leather_jacket.c");
        give_item(player, "knife.c");
        give_item(player, "rope.c");
        give_item(player, "personal_radio.c");
        give_item(player, "rations.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Professional Thief ──────────────────────────────────────────────────── */
    case "professional thief":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "handcuffs.c");
        give_item(player, "rope.c");
        give_item(player, "c18_pistol.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    case "smuggler":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "handcuffs.c");
        give_item(player, "rope.c");
        give_item(player, "c18_pistol.c");
        give_item(player, "personal_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        player->add_money("black credits", (random(6)+1) * 100);
        break;

    /* ── Gifted Gypsy ───────────────────────────────────────────────────────── */
    case "gifted gypsy":
        give_item(player, "knife.c");
        give_item(player, "leather_jacket.c");
        give_item(player, "personal_radio.c");
        give_item(player, "spell_components.c");
        give_credits(player, (random(6)+1) * 100);
        break;

    /* ── Psychic / pilot extras ─────────────────────────────────────────────── */
    case "mind melter":
        give_item(player, "leather_jacket.c");
        give_credits(player, (random(6)+1) * 100);
        break;

    case "nega-psychic":
        give_item(player, "c18_pistol.c");
        give_item(player, "leather_jacket.c");
        give_credits(player, (random(6)+1) * 100);
        break;

    case "atlantean slave":
        give_item(player, "knife.c");
        give_item(player, "vibro_knife.c");
        give_item(player, "leather_jacket.c");
        give_credits(player, (random(4)+1) * 100);
        break;

    case "robot pilot":
        give_item_path(player,
            "/domains/Praxis/equipment/power_armor/NG_EX10_Samson.c");
        give_item(player, "c18_pistol.c");
        give_item(player, "personal_radio.c");
        give_credits(player, (random(6)+1 + random(6)+1) * 100);
        break;

    /* ── Default ─────────────────────────────────────────────────────────────── */
    case "none":
    default:
        give_item(player, "leather_jacket.c");
        give_item(player, "knife.c");
        give_credits(player, 500);
        break;
    }
}

/* ── H2H bonus helper ────────────────────────────────────────────────────── */
/* Applies melee and attack bonuses for a given Hand-to-Hand type string. */

private void do_hth(object player, string hth_type) {
    string s;
    int m;
    int a;

    s = lower_case(hth_type);
    m = 2; a = 2;
    if(strsrch(s, "dragon") != -1)              { m = 9; a = 9; }
    else if(strsrch(s, "vampire") != -1)         { m = 8; a = 8; }
    else if(strsrch(s, "martial arts") != -1)    { m = 7; a = 7; }
    else if(strsrch(s, "cyber-knight") != -1)    { m = 6; a = 6; }
    else if(strsrch(s, "commando") != -1)        { m = 6; a = 6; }
    else if(strsrch(s, "combat 4") != -1)        { m = 5; a = 5; }
    else if(strsrch(s, "expert") != -1)          { m = 5; a = 5; }
    else if(strsrch(s, "combat 3") != -1)        { m = 4; a = 4; }
    player->add_skill_bonus("melee",  m);
    player->add_skill_bonus("attack", a);
    player->setenv("rifts_hth", capitalize(hth_type));
}

/* ── Base skills ───────────────────────────────────────────────────────────── */
/* Granted to every character regardless of race or OCC. */
/* Native language is handled by LANGUAGE_D in finish_creation; skip here. */

private void do_base_skills(object player) {
    RIFTS_SKILLS_D->grant_skill(player, "literacy",        60);
    RIFTS_SKILLS_D->grant_skill(player, "mathematics basic", 45);
    RIFTS_SKILLS_D->grant_skill(player, "pilot automobile", 36);
}

/* ── OCC skill packages ────────────────────────────────────────────────────── */

private void do_occ_skills(object player, string occ, string race) {
    string s;

    s = lower_case(occ);

    switch(s) {

    case "headhunter":
    case "bounty hunter":
    case "mercenary":
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",      65);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",     60);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette",   50);
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush",        45);
        RIFTS_SKILLS_D->grant_skill(player, "tracking",             40);
        RIFTS_SKILLS_D->grant_skill(player, "wilderness survival",  40);
        RIFTS_SKILLS_D->grant_skill(player, "first aid",            50);
        do_hth(player, "combat 3");
        break;

    case "gunfighter":
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",     75);
        RIFTS_SKILLS_D->grant_skill(player, "wp revolver",            70);
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush",        50);
        RIFTS_SKILLS_D->grant_skill(player, "land navigation",      45);
        RIFTS_SKILLS_D->grant_skill(player, "prowl",                40);
        RIFTS_SKILLS_D->grant_skill(player, "first aid",            45);
        do_hth(player, "expert");
        break;

    case "palmer":
        RIFTS_SKILLS_D->grant_skill(player, "lore psychic",         55);
        RIFTS_SKILLS_D->grant_skill(player, "research",             50);
        RIFTS_SKILLS_D->grant_skill(player, "tracking",             45);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise",           45);
        RIFTS_SKILLS_D->grant_skill(player, "detect concealment",   50);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",     50);
        do_hth(player, "basic");
        break;

    case "palmer2":
        RIFTS_SKILLS_D->grant_skill(player, "lore psychic",         65);
        RIFTS_SKILLS_D->grant_skill(player, "research",             60);
        RIFTS_SKILLS_D->grant_skill(player, "tracking",             55);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise",           50);
        RIFTS_SKILLS_D->grant_skill(player, "detect concealment",   60);
        RIFTS_SKILLS_D->grant_skill(player, "intelligence",         55);
        RIFTS_SKILLS_D->grant_skill(player, "interrogation",        50);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",     60);
        do_hth(player, "combat 3");
        break;

    case "coalition soldier":
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",      70);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",     60);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette",   60);
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush",        50);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic",          60);
        RIFTS_SKILLS_D->grant_skill(player, "first aid",            55);
        do_hth(player, "expert");
        break;

    case "wilderness scout":
        RIFTS_SKILLS_D->grant_skill(player, "wilderness survival",  65);
        RIFTS_SKILLS_D->grant_skill(player, "track animals",        55);
        RIFTS_SKILLS_D->grant_skill(player, "hunting",              50);
        RIFTS_SKILLS_D->grant_skill(player, "identify plants",      45);
        RIFTS_SKILLS_D->grant_skill(player, "swimming",             50);
        RIFTS_SKILLS_D->grant_skill(player, "climbing",             55);
        RIFTS_SKILLS_D->grant_skill(player, "navigation",           40);
        RIFTS_SKILLS_D->grant_skill(player, "wp rifle",             60);
        RIFTS_SKILLS_D->grant_skill(player, "falconry",             40);
        do_hth(player, "combat 4");
        break;

    case "tribal warrior":
        RIFTS_SKILLS_D->grant_skill(player, "wilderness survival",  60);
        RIFTS_SKILLS_D->grant_skill(player, "track animals",        50);
        RIFTS_SKILLS_D->grant_skill(player, "hunting",              50);
        RIFTS_SKILLS_D->grant_skill(player, "land navigation",      40);
        RIFTS_SKILLS_D->grant_skill(player, "climbing",             50);
        RIFTS_SKILLS_D->grant_skill(player, "swimming",             45);
        RIFTS_SKILLS_D->grant_skill(player, "falconry",             35);
        RIFTS_SKILLS_D->grant_skill(player, "wp two ancient",       55);
        RIFTS_SKILLS_D->grant_skill(player, "wp rifle",             50);
        do_hth(player, "basic");
        break;

    case "vagabond":
        RIFTS_SKILLS_D->grant_skill(player, "streetwise",           36);
        RIFTS_SKILLS_D->grant_skill(player, "prowl",                30);
        RIFTS_SKILLS_D->grant_skill(player, "beg",                  40);
        RIFTS_SKILLS_D->grant_skill(player, "disguise",             30);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",     50);
        do_hth(player, "basic");
        break;

    case "city rat":
        RIFTS_SKILLS_D->grant_skill(player, "streetwise",           46);
        RIFTS_SKILLS_D->grant_skill(player, "pick locks",           30);
        RIFTS_SKILLS_D->grant_skill(player, "pick pockets",         25);
        RIFTS_SKILLS_D->grant_skill(player, "prowl",                35);
        RIFTS_SKILLS_D->grant_skill(player, "concealment",          20);
        do_hth(player, "basic");
        break;

    case "ley line walker":
    case "ley line rifter":
        RIFTS_SKILLS_D->grant_skill(player, "lore magic",           60);
        RIFTS_SKILLS_D->grant_skill(player, "spell knowledge",      70);
        do_hth(player, "basic");
        break;

    case "mystic":
        RIFTS_SKILLS_D->grant_skill(player, "lore magic",           50);
        RIFTS_SKILLS_D->grant_skill(player, "meditation",           60);
        do_hth(player, "basic");
        break;

    case "mind melter":
        RIFTS_SKILLS_D->grant_skill(player, "detect psionics", 55);
        RIFTS_SKILLS_D->grant_skill(player, "meditation",      60);
        do_hth(player, "basic");
        break;

    case "cs grunt":
    case "cs dead boy":
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",      70);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette",   55);
        RIFTS_SKILLS_D->grant_skill(player, "pilot hovercraft",     52);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic",          55);
        do_hth(player, "combat 3");
        break;

    case "juicer":
    case "ninja juicer":
    case "delphi juicer":
    case "hyperion juicer":
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",      65);
        RIFTS_SKILLS_D->grant_skill(player, "athletics",            50);
        RIFTS_SKILLS_D->grant_skill(player, "acrobatics",           50);
        do_hth(player, "martial arts");
        break;

    case "cyber-knight":
        RIFTS_SKILLS_D->grant_skill(player, "wp sword",             65);
        RIFTS_SKILLS_D->grant_skill(player, "wp vibroblade",        55);
        RIFTS_SKILLS_D->grant_skill(player, "horsemanship",         55);
        do_hth(player, "cyber-knight");
        break;

    case "crazy":
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",      60);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",     60);
        RIFTS_SKILLS_D->grant_skill(player, "athletics",            50);
        RIFTS_SKILLS_D->grant_skill(player, "acrobatics",           55);
        do_hth(player, "martial arts");
        break;

    case "operator":
        RIFTS_SKILLS_D->grant_skill(player, "mechanical engineer",  50);
        RIFTS_SKILLS_D->grant_skill(player, "electrical engineer",  45);
        RIFTS_SKILLS_D->grant_skill(player, "pilot hover vehicle",  52);
        RIFTS_SKILLS_D->grant_skill(player, "salvage",              45);
        do_hth(player, "basic");
        break;

    case "techno-wizard":
        RIFTS_SKILLS_D->grant_skill(player, "magical engineering",  50);
        RIFTS_SKILLS_D->grant_skill(player, "lore magic",           55);
        RIFTS_SKILLS_D->grant_skill(player, "weapon systems",       50);
        do_hth(player, "basic");
        break;

    case "body fixer":
        RIFTS_SKILLS_D->grant_skill(player, "medical doctor",       60);
        RIFTS_SKILLS_D->grant_skill(player, "first aid",            75);
        RIFTS_SKILLS_D->grant_skill(player, "pathology",            50);
        do_hth(player, "basic");
        break;

    case "shifter":
    case "shaman":
        RIFTS_SKILLS_D->grant_skill(player, "dimensional travel",   50);
        RIFTS_SKILLS_D->grant_skill(player, "lore: demons & monsters", 55);
        RIFTS_SKILLS_D->grant_skill(player, "lore magic",           55);
        do_hth(player, "basic");
        break;

    case "necromancer":
        RIFTS_SKILLS_D->grant_skill(player, "lore: demons & monsters", 60);
        RIFTS_SKILLS_D->grant_skill(player, "lore magic",              55);
        RIFTS_SKILLS_D->grant_skill(player, "pathology",               50);
        do_hth(player, "basic");
        break;

    case "ngr soldier":
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",      65);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",     60);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette",   60);
        RIFTS_SKILLS_D->grant_skill(player, "tactics",              45);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic",          55);
        RIFTS_SKILLS_D->grant_skill(player, "first aid",            50);
        RIFTS_SKILLS_D->grant_skill(player, "land navigation",      40);
        do_hth(player, "expert");
        break;

    case "ngr mechanic":
        RIFTS_SKILLS_D->grant_skill(player, "mechanical engineer",  50);
        RIFTS_SKILLS_D->grant_skill(player, "robot mechanics",      45);
        RIFTS_SKILLS_D->grant_skill(player, "basic electronics",    50);
        RIFTS_SKILLS_D->grant_skill(player, "computer operation",   50);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic",          45);
        RIFTS_SKILLS_D->grant_skill(player, "salvage",              40);
        do_hth(player, "basic");
        break;

    case "tattoo warrior":
        RIFTS_SKILLS_D->grant_skill(player, "wp sword",             65);
        RIFTS_SKILLS_D->grant_skill(player, "wp knife",             60);
        RIFTS_SKILLS_D->grant_skill(player, "lore magic",           55);
        RIFTS_SKILLS_D->grant_skill(player, "acrobatics",           50);
        RIFTS_SKILLS_D->grant_skill(player, "climbing",             50);
        RIFTS_SKILLS_D->grant_skill(player, "wilderness survival",  45);
        do_hth(player, "martial arts");
        break;

    case "kittani warrior":
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",      65);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",     60);
        RIFTS_SKILLS_D->grant_skill(player, "wp sword",             60);
        RIFTS_SKILLS_D->grant_skill(player, "pilot power armor",    60);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette",   55);
        RIFTS_SKILLS_D->grant_skill(player, "tactics",              50);
        do_hth(player, "martial arts");
        break;

    case "kittani field mechanic":
        RIFTS_SKILLS_D->grant_skill(player, "mechanical engineer",  50);
        RIFTS_SKILLS_D->grant_skill(player, "advanced electronics", 45);
        RIFTS_SKILLS_D->grant_skill(player, "robot mechanics",      50);
        RIFTS_SKILLS_D->grant_skill(player, "weapon systems",       45);
        RIFTS_SKILLS_D->grant_skill(player, "computer operation",   55);
        do_hth(player, "basic");
        break;

    case "glitter boy pilot":
        RIFTS_SKILLS_D->grant_skill(player, "pilot power armor",  72);
        RIFTS_SKILLS_D->grant_skill(player, "weapon systems",     60);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",    65);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic",        55);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette", 45);
        RIFTS_SKILLS_D->grant_skill(player, "first aid",          45);
        do_hth(player, "expert");
        break;

    case "cs samas rpa pilot":
        RIFTS_SKILLS_D->grant_skill(player, "pilot power armor",  70);
        RIFTS_SKILLS_D->grant_skill(player, "weapon systems",     60);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",    65);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic",        60);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette", 55);
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush",      40);
        do_hth(player, "expert");
        break;

    case "maxi-man":
        RIFTS_SKILLS_D->grant_skill(player, "boxing",         70);
        RIFTS_SKILLS_D->grant_skill(player, "body building",  70);
        RIFTS_SKILLS_D->grant_skill(player, "acrobatics",     65);
        RIFTS_SKILLS_D->grant_skill(player, "athletics",      55);
        RIFTS_SKILLS_D->grant_skill(player, "gymnastics",     60);
        RIFTS_SKILLS_D->grant_skill(player, "climbing",       55);
        RIFTS_SKILLS_D->grant_skill(player, "running",        50);
        RIFTS_SKILLS_D->grant_skill(player, "swimming",       60);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle", 60);
        RIFTS_SKILLS_D->grant_skill(player, "wp knife",       60);
        do_hth(player, "commando");
        break;

    case "none":
        /* RCC-only: check race for species-appropriate combat style */
        s = lower_case(race);
        if(strsrch(s, "dragon") != -1) {
            RIFTS_SKILLS_D->grant_skill(player, "lore magic",       75);
            RIFTS_SKILLS_D->grant_skill(player, "lore: demons & monsters", 65);
            RIFTS_SKILLS_D->grant_skill(player, "fly",              98);
            do_hth(player, "dragon");
        } else if(strsrch(s, "vampire") != -1) {
            RIFTS_SKILLS_D->grant_skill(player, "prowl",            65);
            RIFTS_SKILLS_D->grant_skill(player, "seduction",        60);
            do_hth(player, "vampire");
        } else if(strsrch(s, "faerie") != -1 || strsrch(s, "fairy") != -1) {
            RIFTS_SKILLS_D->grant_skill(player, "lore magic",       55);
            RIFTS_SKILLS_D->grant_skill(player, "prowl",            60);
            do_hth(player, "basic");
        } else if(s == "godling") {
            RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",           98);
            RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",            98);
            RIFTS_SKILLS_D->grant_skill(player, "wp heavy weapons",           98);
            RIFTS_SKILLS_D->grant_skill(player, "wp knife",                   98);
            RIFTS_SKILLS_D->grant_skill(player, "wp sword",                   98);
            RIFTS_SKILLS_D->grant_skill(player, "wp vibroblade",              98);
            RIFTS_SKILLS_D->grant_skill(player, "wp blunt",                   98);
            RIFTS_SKILLS_D->grant_skill(player, "wp chain",                   98);
            RIFTS_SKILLS_D->grant_skill(player, "wp paired weapons",          98);
            RIFTS_SKILLS_D->grant_skill(player, "wp rifle",                   98);
            RIFTS_SKILLS_D->grant_skill(player, "wp archery",                 98);
            RIFTS_SKILLS_D->grant_skill(player, "pilot automobile",           98);
            RIFTS_SKILLS_D->grant_skill(player, "pilot hovercraft",           98);
            RIFTS_SKILLS_D->grant_skill(player, "pilot hovercycle",           98);
            RIFTS_SKILLS_D->grant_skill(player, "pilot power armor",          98);
            RIFTS_SKILLS_D->grant_skill(player, "pilot robot",                98);
            RIFTS_SKILLS_D->grant_skill(player, "pilot glitter boy",          98);
            RIFTS_SKILLS_D->grant_skill(player, "pilot jet pack",             98);
            RIFTS_SKILLS_D->grant_skill(player, "pilot boat",                 98);
            RIFTS_SKILLS_D->grant_skill(player, "pilot submersible",          98);
            RIFTS_SKILLS_D->grant_skill(player, "radio basic",                98);
            RIFTS_SKILLS_D->grant_skill(player, "computer operation",         98);
            RIFTS_SKILLS_D->grant_skill(player, "computer programming",       98);
            RIFTS_SKILLS_D->grant_skill(player, "basic electronics",          98);
            RIFTS_SKILLS_D->grant_skill(player, "advanced electronics",       98);
            RIFTS_SKILLS_D->grant_skill(player, "read sensory equipment",     98);
            RIFTS_SKILLS_D->grant_skill(player, "weapon systems",             98);
            RIFTS_SKILLS_D->grant_skill(player, "electronic countermeasures", 98);
            RIFTS_SKILLS_D->grant_skill(player, "mechanical engineer",        98);
            RIFTS_SKILLS_D->grant_skill(player, "robot mechanics",            98);
            RIFTS_SKILLS_D->grant_skill(player, "weapons engineer",           98);
            RIFTS_SKILLS_D->grant_skill(player, "electrical engineer",        98);
            RIFTS_SKILLS_D->grant_skill(player, "first aid",                  98);
            RIFTS_SKILLS_D->grant_skill(player, "paramedic",                  98);
            RIFTS_SKILLS_D->grant_skill(player, "medical doctor",             98);
            RIFTS_SKILLS_D->grant_skill(player, "holistic medicine",          98);
            RIFTS_SKILLS_D->grant_skill(player, "biology",                    98);
            RIFTS_SKILLS_D->grant_skill(player, "chemistry",                  98);
            RIFTS_SKILLS_D->grant_skill(player, "pathology",                  98);
            RIFTS_SKILLS_D->grant_skill(player, "wilderness survival",        98);
            RIFTS_SKILLS_D->grant_skill(player, "land navigation",            98);
            RIFTS_SKILLS_D->grant_skill(player, "tracking",                   98);
            RIFTS_SKILLS_D->grant_skill(player, "camouflage",                 98);
            RIFTS_SKILLS_D->grant_skill(player, "hunting",                    98);
            RIFTS_SKILLS_D->grant_skill(player, "fishing",                    98);
            RIFTS_SKILLS_D->grant_skill(player, "swimming",                   98);
            RIFTS_SKILLS_D->grant_skill(player, "climbing",                   98);
            RIFTS_SKILLS_D->grant_skill(player, "running",                    98);
            RIFTS_SKILLS_D->grant_skill(player, "navigation",                 98);
            RIFTS_SKILLS_D->grant_skill(player, "detect ambush",              98);
            RIFTS_SKILLS_D->grant_skill(player, "detect concealment",         98);
            RIFTS_SKILLS_D->grant_skill(player, "intelligence",               98);
            RIFTS_SKILLS_D->grant_skill(player, "surveillance",               98);
            RIFTS_SKILLS_D->grant_skill(player, "pick locks",                 98);
            RIFTS_SKILLS_D->grant_skill(player, "pick pockets",               98);
            RIFTS_SKILLS_D->grant_skill(player, "safe-cracking",              98);
            RIFTS_SKILLS_D->grant_skill(player, "forgery",                    98);
            RIFTS_SKILLS_D->grant_skill(player, "disguise",                   98);
            RIFTS_SKILLS_D->grant_skill(player, "impersonation",              98);
            RIFTS_SKILLS_D->grant_skill(player, "streetwise",                 98);
            RIFTS_SKILLS_D->grant_skill(player, "gambling",                   98);
            RIFTS_SKILLS_D->grant_skill(player, "concealment",                98);
            RIFTS_SKILLS_D->grant_skill(player, "prowl",                      98);
            RIFTS_SKILLS_D->grant_skill(player, "interrogation",              98);
            RIFTS_SKILLS_D->grant_skill(player, "tactics",                    98);
            RIFTS_SKILLS_D->grant_skill(player, "military etiquette",         98);
            RIFTS_SKILLS_D->grant_skill(player, "demolitions",                98);
            RIFTS_SKILLS_D->grant_skill(player, "boxing",                     98);
            RIFTS_SKILLS_D->grant_skill(player, "gymnastics",                 98);
            RIFTS_SKILLS_D->grant_skill(player, "acrobatics",                 98);
            RIFTS_SKILLS_D->grant_skill(player, "body building",              98);
            RIFTS_SKILLS_D->grant_skill(player, "meditation",                 98);
            RIFTS_SKILLS_D->grant_skill(player, "athletics",                  98);
            RIFTS_SKILLS_D->grant_skill(player, "lore magic",                 98);
            RIFTS_SKILLS_D->grant_skill(player, "lore demons and monsters",   98);
            RIFTS_SKILLS_D->grant_skill(player, "lore rifts and dimensions",  98);
            RIFTS_SKILLS_D->grant_skill(player, "lore psychic",               98);
            RIFTS_SKILLS_D->grant_skill(player, "lore spirits",               98);
            RIFTS_SKILLS_D->grant_skill(player, "research",                   98);
            RIFTS_SKILLS_D->grant_skill(player, "salvage",                    98);
            RIFTS_SKILLS_D->grant_skill(player, "horsemanship",               98);
            RIFTS_SKILLS_D->grant_skill(player, "seduction",                  98);
            RIFTS_SKILLS_D->grant_skill(player, "identify plants",            98);
            RIFTS_SKILLS_D->grant_skill(player, "barter",                     98);
            RIFTS_SKILLS_D->grant_skill(player, "appraise goods",             98);
            RIFTS_SKILLS_D->grant_skill(player, "black market",               98);
            RIFTS_SKILLS_D->grant_skill(player, "fly",                        98);
            do_hth(player, "dragon");
        } else {
            RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol", 50);
            RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",  50);
            do_hth(player, "basic");
        }
        break;

    case "forger":
        RIFTS_SKILLS_D->grant_skill(player, "forgery",            55);
        RIFTS_SKILLS_D->grant_skill(player, "disguise",           40);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise",         46);
        RIFTS_SKILLS_D->grant_skill(player, "computer operation", 45);
        RIFTS_SKILLS_D->grant_skill(player, "surveillance",       35);
        do_hth(player, "basic");
        break;

    case "freelance spy":
        RIFTS_SKILLS_D->grant_skill(player, "disguise",           45);
        RIFTS_SKILLS_D->grant_skill(player, "surveillance",       50);
        RIFTS_SKILLS_D->grant_skill(player, "pick locks",         35);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic",        55);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise",         40);
        RIFTS_SKILLS_D->grant_skill(player, "prowl",              40);
        do_hth(player, "basic");
        break;

    case "iss peacekeeper":
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette", 60);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic",        55);
        RIFTS_SKILLS_D->grant_skill(player, "first aid",          50);
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush",      45);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",   65);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise",         35);
        do_hth(player, "expert");
        break;

    case "iss specter":
        RIFTS_SKILLS_D->grant_skill(player, "disguise",           55);
        RIFTS_SKILLS_D->grant_skill(player, "prowl",              50);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise",         50);
        RIFTS_SKILLS_D->grant_skill(player, "surveillance",       55);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic",        50);
        RIFTS_SKILLS_D->grant_skill(player, "pick locks",         35);
        do_hth(player, "basic");
        break;

    case "pirate (s.a.)":
    case "pirate":
        RIFTS_SKILLS_D->grant_skill(player, "pilot boat",         65);
        RIFTS_SKILLS_D->grant_skill(player, "swimming",           55);
        RIFTS_SKILLS_D->grant_skill(player, "navigation",         50);
        RIFTS_SKILLS_D->grant_skill(player, "wp sword",           60);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise",         45);
        do_hth(player, "expert");
        break;

    case "sailor (s.a.)":
    case "sailor":
        RIFTS_SKILLS_D->grant_skill(player, "pilot boat",         72);
        RIFTS_SKILLS_D->grant_skill(player, "swimming",           60);
        RIFTS_SKILLS_D->grant_skill(player, "navigation",         55);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic",        50);
        RIFTS_SKILLS_D->grant_skill(player, "first aid",          40);
        do_hth(player, "basic");
        break;

    case "professional thief":
        RIFTS_SKILLS_D->grant_skill(player, "pick locks",         55);
        RIFTS_SKILLS_D->grant_skill(player, "pick pockets",       50);
        RIFTS_SKILLS_D->grant_skill(player, "prowl",              50);
        RIFTS_SKILLS_D->grant_skill(player, "concealment",        40);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise",         50);
        RIFTS_SKILLS_D->grant_skill(player, "climbing",           45);
        do_hth(player, "basic");
        break;

    case "gifted gypsy":
        RIFTS_SKILLS_D->grant_skill(player, "lore magic",         55);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise",         45);
        RIFTS_SKILLS_D->grant_skill(player, "disguise",           40);
        RIFTS_SKILLS_D->grant_skill(player, "holistic medicine",  35);
        RIFTS_SKILLS_D->grant_skill(player, "prowl",              35);
        do_hth(player, "basic");
        break;

    case "rogue scholar":
        RIFTS_SKILLS_D->grant_skill(player, "computer operation", 0);
        RIFTS_SKILLS_D->grant_skill(player, "research", 0);
        RIFTS_SKILLS_D->grant_skill(player, "lore magic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "lore rifts and dimensions", 0);
        do_hth(player, "basic");
        break;

    case "rogue scientist":
        RIFTS_SKILLS_D->grant_skill(player, "computer operation", 0);
        RIFTS_SKILLS_D->grant_skill(player, "research", 0);
        RIFTS_SKILLS_D->grant_skill(player, "biology", 0);
        RIFTS_SKILLS_D->grant_skill(player, "chemistry", 0);
        RIFTS_SKILLS_D->grant_skill(player, "first aid", 0);
        do_hth(player, "basic");
        break;

    case "cs ranger":
        RIFTS_SKILLS_D->grant_skill(player, "first aid", 0);
        RIFTS_SKILLS_D->grant_skill(player, "hunting", 0);
        RIFTS_SKILLS_D->grant_skill(player, "land navigation", 0);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette", 0);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wilderness survival", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp one ancient", 0);
        do_hth(player, "basic");
        break;

    case "cs military specialist":
        RIFTS_SKILLS_D->grant_skill(player, "first aid", 0);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette", 0);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush", 0);
        RIFTS_SKILLS_D->grant_skill(player, "tactics", 0);
        RIFTS_SKILLS_D->grant_skill(player, "electronic countermeasures", 0);
        RIFTS_SKILLS_D->grant_skill(player, "weapon systems", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp heavy military weapons", 0);
        do_hth(player, "expert");
        break;

    case "merc soldier":
        RIFTS_SKILLS_D->grant_skill(player, "first aid", 0);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette", 0);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp two ancient", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp three modern", 0);
        do_hth(player, "basic");
        break;

    case "special forces (merc)":
        RIFTS_SKILLS_D->grant_skill(player, "demolitions", 0);
        RIFTS_SKILLS_D->grant_skill(player, "first aid", 0);
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush", 0);
        RIFTS_SKILLS_D->grant_skill(player, "intelligence", 0);
        RIFTS_SKILLS_D->grant_skill(player, "climbing", 0);
        RIFTS_SKILLS_D->grant_skill(player, "swimming", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wilderness survival", 0);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette", 0);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp two ancient", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp three modern", 0);
        do_hth(player, "commando");
        break;

    case "master assassin":
        RIFTS_SKILLS_D->grant_skill(player, "disguise", 0);
        RIFTS_SKILLS_D->grant_skill(player, "impersonation", 0);
        RIFTS_SKILLS_D->grant_skill(player, "concealment", 0);
        RIFTS_SKILLS_D->grant_skill(player, "prowl", 0);
        RIFTS_SKILLS_D->grant_skill(player, "pick locks", 0);
        RIFTS_SKILLS_D->grant_skill(player, "surveillance", 0);
        RIFTS_SKILLS_D->grant_skill(player, "intelligence", 0);
        RIFTS_SKILLS_D->grant_skill(player, "tracking", 0);
        RIFTS_SKILLS_D->grant_skill(player, "forgery", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp two ancient", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp two modern", 0);
        do_hth(player, "martial arts");
        break;

    case "sunaj assassin":
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush", 0);
        RIFTS_SKILLS_D->grant_skill(player, "intelligence", 0);
        RIFTS_SKILLS_D->grant_skill(player, "tracking", 0);
        RIFTS_SKILLS_D->grant_skill(player, "surveillance systems", 0);
        RIFTS_SKILLS_D->grant_skill(player, "disguise", 0);
        RIFTS_SKILLS_D->grant_skill(player, "impersonation", 0);
        RIFTS_SKILLS_D->grant_skill(player, "prowl", 0);
        RIFTS_SKILLS_D->grant_skill(player, "detect concealment", 0);
        do_hth(player, "expert");
        break;

    case "robot pilot":
        RIFTS_SKILLS_D->grant_skill(player, "pilot robot combat elite", 0);
        RIFTS_SKILLS_D->grant_skill(player, "pilot two vehicles of choice", 0);
        RIFTS_SKILLS_D->grant_skill(player, "computer operation", 0);
        RIFTS_SKILLS_D->grant_skill(player, "read sensory equipment", 0);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "weapon systems", 0);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette", 0);
        RIFTS_SKILLS_D->grant_skill(player, "electronic countermeasures", 0);
        do_hth(player, "expert");
        break;

    case "cyber-doc":
        RIFTS_SKILLS_D->grant_skill(player, "biology", 0);
        RIFTS_SKILLS_D->grant_skill(player, "chemistry", 0);
        RIFTS_SKILLS_D->grant_skill(player, "medical doctor", 0);
        RIFTS_SKILLS_D->grant_skill(player, "pathology", 0);
        RIFTS_SKILLS_D->grant_skill(player, "computer operation", 0);
        RIFTS_SKILLS_D->grant_skill(player, "first aid", 0);
        do_hth(player, "basic");
        break;

    case "borg":
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette", 0);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "read sensory equipment", 0);
        RIFTS_SKILLS_D->grant_skill(player, "weapon systems", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp heavy military weapons", 0);
        do_hth(player, "commando");
        break;

    case "air warlock":
        RIFTS_SKILLS_D->grant_skill(player, "lore magic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wilderness survival", 0);
        RIFTS_SKILLS_D->grant_skill(player, "meditation", 0);
        RIFTS_SKILLS_D->grant_skill(player, "holistic medicine", 0);
        do_hth(player, "basic");
        break;

    case "nega-psychic":
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush", 0);
        RIFTS_SKILLS_D->grant_skill(player, "intelligence", 0);
        RIFTS_SKILLS_D->grant_skill(player, "lore magic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "lore psychic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "meditation", 0);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise", 0);
        do_hth(player, "basic");
        break;

    case "atlantean nomad":
        RIFTS_SKILLS_D->grant_skill(player, "lore rifts and dimensions", 0);
        RIFTS_SKILLS_D->grant_skill(player, "lore magic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wilderness survival", 0);
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush", 0);
        RIFTS_SKILLS_D->grant_skill(player, "intelligence", 0);
        do_hth(player, "basic");
        break;

    case "atlantean slave":
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush", 0);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise", 0);
        RIFTS_SKILLS_D->grant_skill(player, "lore magic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "first aid", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wilderness survival", 0);
        do_hth(player, "basic");
        break;

    case "ntset protector":
        RIFTS_SKILLS_D->grant_skill(player, "lore demons and monsters", 0);
        RIFTS_SKILLS_D->grant_skill(player, "lore magic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "first aid", 0);
        RIFTS_SKILLS_D->grant_skill(player, "tracking", 0);
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush", 0);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle", 0);
        do_hth(player, "expert");
        break;

    case "smuggler":
        RIFTS_SKILLS_D->grant_skill(player, "concealment", 0);
        RIFTS_SKILLS_D->grant_skill(player, "streetwise", 0);
        RIFTS_SKILLS_D->grant_skill(player, "detect ambush", 0);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "barter", 0);
        RIFTS_SKILLS_D->grant_skill(player, "black market", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol", 0);
        do_hth(player, "basic");
        break;

    case "royal knight":
    case "knight (europe)":
        RIFTS_SKILLS_D->grant_skill(player, "horsemanship", 0);
        RIFTS_SKILLS_D->grant_skill(player, "body building", 0);
        RIFTS_SKILLS_D->grant_skill(player, "climbing", 0);
        RIFTS_SKILLS_D->grant_skill(player, "first aid", 0);
        RIFTS_SKILLS_D->grant_skill(player, "lore magic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp two ancient", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp two modern", 0);
        do_hth(player, "expert");
        break;

    case "cs technical officer":
        RIFTS_SKILLS_D->grant_skill(player, "computer operation", 0);
        RIFTS_SKILLS_D->grant_skill(player, "computer programming", 0);
        RIFTS_SKILLS_D->grant_skill(player, "basic electronics", 0);
        RIFTS_SKILLS_D->grant_skill(player, "advanced electronics", 0);
        RIFTS_SKILLS_D->grant_skill(player, "military etiquette", 0);
        RIFTS_SKILLS_D->grant_skill(player, "radio basic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol", 0);
        do_hth(player, "basic");
        break;

    case "tattooed man":
        RIFTS_SKILLS_D->grant_skill(player, "body building", 0);
        RIFTS_SKILLS_D->grant_skill(player, "running", 0);
        RIFTS_SKILLS_D->grant_skill(player, "boxing", 0);
        RIFTS_SKILLS_D->grant_skill(player, "lore magic", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wilderness survival", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp two ancient", 0);
        RIFTS_SKILLS_D->grant_skill(player, "wp two modern", 0);
        do_hth(player, "commando");
        break;

    default:
        RIFTS_SKILLS_D->grant_skill(player, "wp energy pistol",     50);
        RIFTS_SKILLS_D->grant_skill(player, "wp energy rifle",      50);
        do_hth(player, "basic");
        break;
    }
}

/* ── Spells / psionics / PPE-ISP bonuses ─────────────────────────────────── */
/* Spells and psionics stored as semicolon-delimited env var strings. */
/* PPE/ISP bonuses added on top of OCC base values set during creation. */

private void do_occ_spells(object player, string occ) {
    string s;
    string spells;
    string psionics;
    int ppe_bonus;
    int isp_bonus;

    s       = lower_case(occ);
    spells   = "";
    psionics = "";
    ppe_bonus = 0;
    isp_bonus = 0;

    switch(s) {
    case "ley line walker":
    case "ley line rifter":
        spells = "cloud of smoke;blinding flash;thunderclap;globe of daylight;levitation;magic net;energy bolt;sense evil;sense magic;armor of ithan";
        ppe_bonus = 30;
        break;

    case "mystic":
        spells = "sense evil;sense magic;heal wounds;light target;globe of daylight;windrush;swimasafish;armor of ithan";
        ppe_bonus = 20;
        isp_bonus = 30;
        break;

    case "mind melter":
        psionics = "telepathy;empathy;sense evil;sixth sense;object read;mind block;telekinesis;bio-regeneration";
        isp_bonus = 50;
        break;

    case "palmer":
        psionics = "telepathy;empathy;object read;sixth sense;detect psionics;mind block";
        isp_bonus = 40;
        break;

    case "palmer2":
        psionics = "telepathy;empathy;object read;sixth sense;detect psionics;mind block;clairvoyance;remote viewing;precognition";
        isp_bonus = 60;
        break;

    case "cyber-knight":
        psionics = "psi-sword;sense evil;empathy;sixth sense;mind block";
        isp_bonus = 30;
        break;

    case "crazy":
        psionics = "empathy;sixth sense;bio-regeneration;detect psionics";
        isp_bonus = 20;
        break;

    case "shifter":
    case "shaman":
        spells = "sense evil;sense magic;dimensional pocket;call familiar";
        break;

    case "techno-wizard":
        spells = "sense magic;energy bolt;armor of ithan";
        ppe_bonus = 10;
        break;

    case "necromancer":
        spells = "sense evil;sense magic;energy bolt";
        ppe_bonus = 15;
        break;

    case "sunaj assassin":
        ppe_bonus = 30;
        break;

    case "tattoo warrior":
        ppe_bonus = 40;
        break;

    case "tattooed man":
        ppe_bonus = 30;
        break;
    }

    if(sizeof(spells))
        player->setenv("known_spells", spells);
    if(sizeof(psionics))
        player->setenv("known_psionics", psionics);
    if(ppe_bonus > 0) {
        player->set_stats("PPE",     (int)player->query_stats("PPE")     + ppe_bonus);
        player->set_stats("max_PPE", (int)player->query_stats("max_PPE") + ppe_bonus);
    }
    if(isp_bonus > 0) {
        player->set_stats("ISP",     (int)player->query_stats("ISP")     + isp_bonus);
        player->set_stats("max_ISP", (int)player->query_stats("max_ISP") + isp_bonus);
    }
}

string *query_occ_spell_grant(string occ) {
    string s;

    s = lower_case(occ ? occ : "");
    switch(s) {
    case "ley line walker":
    case "ley line rifter":
        return explode(";",
            "cloud of smoke;blinding flash;thunderclap;globe of daylight;"
            "levitation;magic net;energy bolt;sense evil;sense magic;armor of ithan");
    case "mystic":
        return explode(";",
            "sense evil;sense magic;heal wounds;light target;"
            "globe of daylight;windrush;swimasafish;armor of ithan");
    case "shifter":
    case "shaman":
        return explode(";",
            "sense evil;sense magic;dimensional pocket;call familiar");
    case "techno-wizard":
        return explode(";", "sense magic;energy bolt;armor of ithan");
    case "necromancer":
        return explode(";", "sense evil;sense magic;energy bolt");
    default:
        return ({});
    }
}

string *query_occ_psionic_grant(string occ) {
    string s;

    s = lower_case(occ ? occ : "");
    switch(s) {
    case "mind melter":
        return explode(";",
            "telepathy;empathy;sense evil;sixth sense;object read;"
            "mind block;telekinesis;bio-regeneration");
    case "cyber-knight":
        return explode(";",
            "psi-sword;sense evil;empathy;sixth sense;mind block");
    case "crazy":
        return explode(";",
            "empathy;sixth sense;bio-regeneration;detect psionics");
    default:
        return ({});
    }
}

string *query_race_spell_grant(string race) {
    string r;

    r = lower_case(race ? race : "");
    switch(r) {
    case "godling":
        return (string *)RIFTS_SPELLS_D->query_all_spells();
    case "great horned dragon":
        return explode(";",
            "globe of daylight;blinding flash;cloud of smoke;levitation;magic net;"
            "thunderclap;fire ball;invisibility simple;carpet of adhesion;"
            "fire bolt;armor of ithan;energy field;shadow meld;fly;impervious to energy");
    case "fire dragon":
    case "ice dragon":
    case "thunder lizard dragon":
        return explode(";",
            "globe of daylight;blinding flash;cloud of smoke;levitation;magic net;"
            "fire ball;fire bolt;armor of ithan;fly");
    case "dragon hatchling":
        return explode(";",
            "globe of daylight;blinding flash;cloud of smoke;levitation");
    /* Minor fae magic: light, smoke, illusion/evasion. Minimal starter
       package, small on purpose - expand later. */
    case "common faerie":
    case "common pixie":
    case "frost pixie":
    case "green wood faerie":
    case "night-elves faerie":
    case "silver bells faerie":
        return explode(";", "globe of daylight;cloud of smoke;chameleon;invisibility simple");
    /* Minor nature magic: light, camouflage, animal/magic awareness.
       Minimal starter package, small on purpose - expand later. */
    case "tree sprite":
    case "water sprite":
    case "brownie":
        return explode(";", "globe of daylight;chameleon;sense magic;repel animals");
    default:
        return ({});
    }
}

string *query_race_psionic_grant(string race) {
    string r;

    r = lower_case(race ? race : "");
    switch(r) {
    case "godling":
        return (string *)RIFTS_PSIONICS_D->query_all_psionics();
    case "mind melter":
        return explode(";",
            "telepathy;empathy;sense evil;sixth sense;object read;mind block;telekinesis");
    case "burster":
        return explode(";",
            "pyrokinesis;pyrokinesis bolt;bio-regeneration;sixth sense");
    case "great horned dragon":
        return explode(";",
            "telepathy;mind block;telekinesis;empathy;object read;sense evil");
    case "wild psi-stalker":
    case "cs psi-stalker":
        return explode(";",
            "mind block;object read;sense evil;sixth sense");
    default:
        return ({});
    }
}

private void do_race_spells(object player, string race) {
    string existing;
    string spells;
    string *grants;
    int ppe_base;

    grants = query_race_spell_grant(race);
    if(!grants || !sizeof(grants)) return;
    spells = implode(grants, ";");
    ppe_base = 0;
    switch(lower_case(race)) {
    case "godling":
        ppe_base = 2000;
        break;
    case "great horned dragon":
        ppe_base = 200;
        break;
    case "fire dragon":
    case "ice dragon":
    case "thunder lizard dragon":
        ppe_base = 100;
        break;
    case "dragon hatchling":
        ppe_base = 40;
        break;
    case "common faerie":
    case "common pixie":
    case "frost pixie":
    case "green wood faerie":
    case "night-elves faerie":
    case "silver bells faerie":
        ppe_base = 30;
        break;
    case "tree sprite":
    case "water sprite":
    case "brownie":
        ppe_base = 20;
        break;
    default:
        break;
    }

    existing = (string)player->getenv("known_spells");
    if(existing && sizeof(existing))
        player->setenv("known_spells", existing + ";" + spells);
    else
        player->setenv("known_spells", spells);
    if(ppe_base > 0) {
        player->set_stats("PPE", ppe_base);
        player->set_stats("max_PPE", ppe_base);
    }
}

private void do_race_psionics(object player, string race) {
    string existing;
    string add;
    string *grants;
    int isp_bonus;

    grants = query_race_psionic_grant(race);
    if(!grants || !sizeof(grants)) return;
    add = implode(grants, ";");
    existing = (string)player->getenv("known_psionics");
    if(!existing) existing = "";
    isp_bonus = 0;
    switch(lower_case(race)) {
    case "godling":
        isp_bonus = 2000;
        break;
    case "mind melter":
        isp_bonus = 50;
        break;
    case "burster":
        isp_bonus = 20;
        break;
    case "great horned dragon":
        isp_bonus = 40;
        break;
    case "wild psi-stalker":
    case "cs psi-stalker":
        isp_bonus = 20;
        break;
    default:
        break;
    }

    if(sizeof(existing))
        player->setenv("known_psionics", existing + ";" + add);
    else
        player->setenv("known_psionics", add);
    if(isp_bonus > 0) {
        player->set_stats("ISP",     (int)player->query_stats("ISP")     + isp_bonus);
        player->set_stats("max_ISP", (int)player->query_stats("max_ISP") + isp_bonus);
    }
}

/* Atlantean starting tattoos
   Grant 2 starting tattoos based on OCC. Only for Atlantean race (or Sunaj). */

private void do_atlantean_tattoos(object player, string occ, string race) {
    string t1_name, t1_loc;
    string t2_name, t2_loc;
    int cur_ppe;

    if(strsrch(lower_case(race), "atlantean") == -1 &&
       lower_case(occ) != "sunaj assassin" &&
       lower_case(occ) != "tattoo warrior" &&
       lower_case(occ) != "tattooed man") return;
    /* Ensure atlantean-race tattoo users have at least 20 PPE if OCC set none. */
    cur_ppe = (int)player->query_stats("PPE");
    if(cur_ppe < 20) {
        player->set_stats("PPE",     20);
        player->set_stats("max_PPE", 20);
    }

    t1_name = "";
    t1_loc  = "";
    t2_name = "";
    t2_loc  = "";

    switch(lower_case(occ)) {
    case "atlantean nomad":
        t1_name = "flaming sword";
        t1_loc  = "right forearm";
        t2_name = "armor of ithan";
        t2_loc  = "chest";
        break;
    case "atlantean slave":
        t1_name = "globe of daylight";
        t1_loc  = "face";
        t2_name = "eyes of the wolf";
        t2_loc  = "left forearm";
        break;
    case "sunaj assassin":
        t1_name = "shadow meld";
        t1_loc  = "back";
        t2_name = "flaming sword";
        t2_loc  = "right forearm";
        break;
    case "tattoo warrior":
        /* Tattoo warriors get 4 tattoos: two weapons, one armor, one power */
        catch(TATTOO_D->grant_tattoo(player, "flaming sword",
            "right forearm"));
        catch(TATTOO_D->grant_tattoo(player, "spear",
            "left forearm"));
        catch(TATTOO_D->grant_tattoo(player, "armor of ithan",
            "chest"));
        catch(TATTOO_D->grant_tattoo(player, "globe of daylight",
            "face"));
        return;
    default:
        t1_name = "flaming sword";
        t1_loc  = "right forearm";
        t2_name = "dragon";
        t2_loc  = "back";
        break;
    }

    if(strlen(t1_name) > 0)
        catch(TATTOO_D->grant_tattoo(player, t1_name, t1_loc));
    if(strlen(t2_name) > 0)
        catch(TATTOO_D->grant_tattoo(player, t2_name, t2_loc));
}

/* ── Main entry point ───────────────────────────────────────────────────────
   Called from setter.c finish_creation(). */

void grant_starting_package(object player) {
    string occ;
    string race;

    if(!player) return;
    occ  = (string)player->getenv("rifts_occ");
    race = (string)player->query_race();
    if(!occ)  occ  = "none";
    if(!race) race = "";

    give_item(player, "id_card.c");
    give_equip(player, occ);
    auto_wear_power_armor(player, occ);
    do_base_skills(player);
    do_occ_skills(player, occ, race);
    do_occ_spells(player, occ);
    do_race_spells(player, race);
    do_race_psionics(player, race);
    do_atlantean_tattoos(player, occ, race);
    if(strsrch(lower_case(race), "dragon") != -1) {
        player->set_property("dragon_sized", 1);
        player->set_property("current_form", race);
    }
    catch(FACTION_D->apply_race_defaults(player));
}

/* grant_race_package -- public entry for backfill tool.
   Grants race spells and psionics only, without re-giving equipment or
   OCC skills. Safe to call on an existing character. */
void grant_race_package(object player) {
    string race;

    if(!player) return;
    race = (string)player->query_race();
    if(!race) return;
    do_race_spells(player, race);
    do_race_psionics(player, race);
}

/* grant_occ_skills -- public wrapper for backfill tool.
   Grants OCC-based skills only, using the player's stored OCC and race. */
void grant_occ_skills(object player) {
    string occ;
    string race;

    if(!player) return;
    occ  = (string)player->getenv("rifts_occ");
    race = (string)player->query_race();
    if(!occ)  occ  = "none";
    if(!race) race = "";
    do_occ_skills(player, occ, race);
}

/* grant_occ_package -- called when OCC changes post-creation (e.g. clan join).
   Grants equipment, OCC skills, and OCC spells without tattoo or race grants.
   Prevents duplicate tattoo grant when OCC is set by an outside caller. */
void grant_occ_package(object player) {
    string occ;
    string race;

    if(!player) return;
    occ  = (string)player->getenv("rifts_occ");
    race = (string)player->query_race();
    if(!occ)  occ  = "none";
    if(!race) race = "";
    give_equip(player, occ);
    auto_wear_power_armor(player, occ);
    do_occ_skills(player, occ, race);
    do_occ_spells(player, occ);
}
