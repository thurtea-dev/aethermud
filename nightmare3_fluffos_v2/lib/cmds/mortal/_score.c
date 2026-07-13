/* /cmds/mortal/_score.c
   Compact single-panel score display for Rifts characters. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

#define PANEL_INNER 62
#define PANEL_LEFT  30
#define PANEL_RIGHT 30

private string tc(string s) {
    string *w;
    int i;

    if(!s || s == "") return "";
    w = explode(lower_case(s), " ");
    for(i = 0; i < sizeof(w); i++) {
        if(w[i] && w[i] != "") w[i] = capitalize(w[i]);
    }
    return implode(w, " ");
}

private string alignment_str(int al) {
    if(al > 800)   return "Principled";
    if(al > 400)   return "Scrupulous";
    if(al > 0)     return "Unprincipled";
    if(al > -300)  return "Anarchist";
    if(al > -700)  return "Miscreant";
    if(al > -1100) return "Aberrant";
    return "Diabolic";
}

private string clip_text(string s, int n) {
    if(!s) return "";
    if(strlen(s) > n) return s[0..(n - 1)];
    return s;
}

private string panel_border() {
    return "+" + repeat_string("-", PANEL_INNER) + "+";
}

private string panel_row(string text) {
    string t;

    t = clip_text(text, PANEL_INNER - 2);
    return "|" + sprintf(" %-*s ", PANEL_INNER - 2, t) + "|";
}

private string panel_row_status(string text, string ansi_open, string ansi_close) {
    int pad;

    pad = PANEL_INNER - 2 + strlen(ansi_open) + strlen(ansi_close);
    return "|" + sprintf(" %-*s ", pad, ansi_open + text + ansi_close) + "|";
}

private string panel_two_col(string left, string right) {
    string l;
    string r;

    l = clip_text(left, PANEL_LEFT);
    r = clip_text(right, PANEL_RIGHT);
    return panel_row(sprintf("%-*s%-*s", PANEL_LEFT, l, PANEL_RIGHT, r));
}

private int is_mdc_being(object who) {
    string env_mdc;
    string race;

    if((int)who->query_stats("is_MDC") == 1) return 1;
    if((int)who->query_property("rifts_mdc_race") == 1) return 1;
    env_mdc = (string)who->getenv("rifts_mdc_race");
    if(env_mdc && env_mdc != "" && to_int(env_mdc) == 1) return 1;
    race = (string)who->query_race();
    if(race && (int)RIFTS_D->is_mdc_race(race)) return 1;
    return 0;
}

private int query_current_ar(object who) {
    object *inv;
    object ob;
    int i;
    int ar;
    int best;

    inv  = all_inventory(who);
    best = 0;

    for(i = 0; i < sizeof(inv); i++) {
        ob = inv[i];
        if(!ob) continue;
        if(!function_exists("is_armour", ob)) continue;
        if(!(int)ob->is_armour()) continue;
        if((object)ob->query_worn() != who) continue;
        ar = (int)ob->query_property("armor_ar");
        if(ar > best) best = ar;
    }
    return best;
}

private int query_next_level_xp(object who, int level) {
    string race;
    string occ;
    mapping occ_data;
    int *xp_table;
    int *def;

    race     = (string)who->query_race();
    occ_data = 0;

    if(!(int)RIFTS_D->is_rifts_race(race))
        return (int)ADVANCE_D->get_exp(level + 1);

    occ = (string)who->getenv("rifts_occ");
    if(occ && occ != "none" && occ != "")
        occ_data = (mapping)OCC_D->query_occ(occ);

    if(occ_data && occ_data["xp_table"]) {
        xp_table = (int *)occ_data["xp_table"];
        if(level >= sizeof(xp_table)) return -1;
        return xp_table[level];
    }

    def = ({ 0, 2000, 4000, 8000, 16000, 24000, 48000, 80000,
             120000, 160000, 230000, 330000, 430000, 530000, 630000 });
    if(level >= sizeof(def)) return -1;
    return def[level];
}

/* ---- section helpers ---- */

private void show_header(object who) {
    string name;
    string race;
    string occ_name;
    string occ_display;
    string align_text;
    string faction_disp;
    string faction_rank;
    string cs_tier;
    int level;

    name       = (string)who->query_cap_name();
    race       = tc((string)who->query_race());
    occ_name   = (string)who->getenv("rifts_occ");
    align_text = (string)who->getenv("rifts_alignment");
    if(!align_text || !sizeof(align_text))
        align_text = alignment_str((int)who->query_alignment());
    level      = (int)who->query_level();

    if(!occ_name || occ_name == "" || occ_name == "none")
        occ_display = "None (RCC)";
    else
        occ_display = tc(occ_name);

    write(panel_two_col(name, "OCC: " + occ_display) + "\n");
    write(panel_two_col("Race: " + race, sprintf("Level: %d", level)) + "\n");
    write(panel_row("Alignment: " + align_text) + "\n");

    faction_disp = (string)FACTION_D->query_faction_member_display((string)who->query_name());
    if(faction_disp && sizeof(faction_disp)) {
        faction_rank = (string)FACTION_D->query_rank((string)who->query_name());
        if(faction_rank && sizeof(faction_rank))
            write(panel_row("Faction: " + faction_disp + " (" + faction_rank + ")") + "\n");
        else
            write(panel_row("Faction: " + faction_disp) + "\n");
    }

    cs_tier = (string)FACTION_D->query_standing_name((string)who->query_name(), "coalition");
    write(panel_row("CS Standing: " + cs_tier) + "\n");
}

private void show_attributes(object who) {
    int iq;
    int me;
    int ma;
    int ps;
    int pp;
    int pe;
    int pb;
    int spd;
    string line;

    iq  = (int)who->query_stats("IQ");
    me  = (int)who->query_stats("ME");
    ma  = (int)who->query_stats("MA");
    ps  = (int)who->query_stats("PS");
    pp  = (int)who->query_stats("PP");
    pe  = (int)who->query_stats("PE");
    pb  = (int)who->query_stats("PB");
    spd = (int)who->query_stats("Spd");

    write(panel_row("ATTRIBUTES") + "\n");
    line = sprintf("IQ: %d   ME: %d   MA: %d   PS: %d", iq, me, ma, ps);
    write(panel_row(line) + "\n");
    line = sprintf("PP: %d   PE: %d   PB: %d   Spd: %d", pp, pe, pb, spd);
    write(panel_row(line) + "\n");
}

private void show_health(object who) {
    int hp;
    int max_hp;
    int sdc;
    int max_sdc;
    int mdc;
    int max_mdc;
    int ppe;
    int max_ppe;
    int isp;
    int max_isp;
    int lives;
    int lives_initted;
    string intox_s;
    string type_s;
    string week_s;
    string drunk_s;
    int intox;
    int week;
    int quenched;

    hp      = (int)who->query_stats("rifts_hp");
    max_hp  = (int)who->query_stats("max_rifts_hp");
    sdc     = (int)who->query_stats("SDC");
    max_sdc = (int)who->query_stats("max_SDC");
    mdc     = (int)who->query_stats("MDC");
    max_mdc = (int)who->query_stats("max_MDC");
    ppe     = (int)who->query_stats("PPE");
    max_ppe = (int)who->query_stats("max_PPE");
    isp     = (int)who->query_stats("ISP");
    max_isp = (int)who->query_stats("max_ISP");

    write(panel_row("HEALTH") + "\n");
    if(is_mdc_being(who)) {
        /* Characters created before the Atlantean MDC fix have SDC set
           instead of MDC. Use SDC as the display value for these. */
        if(max_mdc == 0 && max_sdc > 0) {
            mdc     = sdc;
            max_mdc = max_sdc;
        }
        write(panel_row(sprintf("MDC: %d/%d", mdc, max_mdc)) + "\n");
    } else {
        write(panel_row(sprintf("HP: %d/%d", hp, max_hp)) + "\n");
        write(panel_row(sprintf("SDC: %d/%d", sdc, max_sdc)) + "\n");
    }
    if(max_ppe > 0)
        write(panel_row(sprintf("PPE: %d/%d", ppe, max_ppe)) + "\n");
    if(max_isp > 0)
        write(panel_row(sprintf("ISP: %d/%d", isp, max_isp)) + "\n");

    lives_initted = (int)who->query_property("rifts_lives_initted");
    lives = lives_initted ? (int)who->query_property("rifts_lives") : 5;
    if(lives == 0)
        write(panel_row_status("Lives: 0  (next death = full death penalty)",
                               "\e[31m", "\e[0m") + "\n");
    else
        write(panel_row(sprintf("Lives: %d", lives)) + "\n");

    if((int)who->query_property("psi_sword_active"))
        write(panel_row("Psi-Sword: ACTIVE") + "\n");

    intox_s = (string)who->getenv("addiction_intox");
    type_s  = (string)who->getenv("addiction_type");
    week_s  = (string)who->getenv("addiction_week");
    drunk_s = (string)who->getenv("addiction_drunk");
    intox   = (intox_s && sizeof(intox_s)) ? to_int(intox_s) : 0;
    week    = (week_s  && sizeof(week_s))  ? to_int(week_s)  : 0;
    quenched = (int)who->query_quenched();

    if(quenched > 0)
        write(panel_row_status("Status: Thirst quenched", "\e[34m", "\e[0m") + "\n");

    if(intox) {
        if(type_s == "drug")
            write(panel_row_status("Status: Stoned", "\e[34m", "\e[0m") + "\n");
        else if(drunk_s && drunk_s == "1")
            write(panel_row_status("Status: Totally drunk", "\e[31m", "\e[0m") + "\n");
        else
            write(panel_row_status("Status: Intoxicated", "\e[31m", "\e[0m") + "\n");
    } else if(type_s && sizeof(type_s) && week > 0) {
        write(panel_row_status("Status: Withdrawal (week " + week + ")",
                               "\e[33m", "\e[0m") + "\n");
    }
}

private void show_combat(object who) {
    int apm;
    int strike;
    int parry;
    int dodge;
    int dmg;
    int ar;
    int save_mag;
    int save_psi;
    int pe;
    int me;
    string autododge_s;
    string autoparry_s;
    string line;

    apm      = (int)RIFTS_COMBAT_D->query_apm(who);
    strike   = (int)RIFTS_COMBAT_D->query_strike_bonus(who);
    parry    = (int)RIFTS_COMBAT_D->query_parry_bonus(who);
    dodge    = (int)RIFTS_COMBAT_D->query_dodge_bonus(who);
    dmg      = (int)RIFTS_COMBAT_D->query_damage_bonus(who);
    pe       = (int)who->query_stats("PE");
    me       = (int)who->query_stats("ME");
    save_mag = (pe > 10) ? (pe - 10) : 0;
    save_psi = (me > 10) ? (me - 10) : 0;
    ar       = query_current_ar(who);
    autododge_s = (who->getenv("rifts_autododge") == "1") ? "ON" : "OFF";
    autoparry_s = (who->getenv("rifts_autoparry_off") != "1") ? "ON" : "OFF";

    write(panel_row("COMBAT") + "\n");
    line = sprintf("APM: %d    Strike: +%d    Parry: +%d    Dodge: +%d",
        apm, strike, parry, dodge);
    write(panel_row(line) + "\n");
    line = sprintf("Dmg: +%d  AR: %d  Save vs Magic: +%d  Save vs Psi: +%d",
        dmg, ar, save_mag, save_psi);
    write(panel_row(line) + "\n");
    line = sprintf("Autododge: %s   Autoparry: %s", autododge_s, autoparry_s);
    write(panel_row(line) + "\n");
}

private void show_wealth(object who) {
    int cred;
    int black;
    int rate;
    string line;

    cred  = (int)who->query_money("credits");
    black = (int)who->query_money("black credits");

    write(panel_row("WEALTH") + "\n");
    line = sprintf("Credits: %d", cred);
    write(panel_row(line) + "\n");
    if(black > 0 || (int)RIFTS_D->is_smuggler_occ(who)) {
        line = sprintf("Black credits: %d", black);
        write(panel_row(line) + "\n");
    }
    if((int)RIFTS_D->is_smuggler_occ(who)) {
        rate = (int)RIFTS_D->query_smuggler_exchange_rate(who);
        line = sprintf("Exchange rate: %d%%", rate);
        write(panel_row(line) + "\n");
    }
}

private void show_experience(object who) {
    int level;
    int exp;
    int next_exp;
    string line;

    level    = (int)who->query_level();
    exp      = (int)who->query_exp();
    next_exp = query_next_level_xp(who, level);

    write(panel_row("EXPERIENCE") + "\n");
    if(next_exp >= 0)
        line = sprintf("Level: %d    XP: %d / %d (next level)", level, exp, next_exp);
    else
        line = sprintf("Level: %d    XP: %d (maximum level reached)", level, exp);
    write(panel_row(line) + "\n");
}

private void show_skills(object who) {
    string *all_skills;
    string active_lang;
    string left;
    string right;
    int pending;

    all_skills  = (string *)RIFTS_SKILLS_D->query_player_skills(who);
    if(!all_skills) all_skills = ({});

    active_lang = (string)LANGUAGE_D->query_active(who);
    if((int)LANGUAGE_D->understands_all(who))
        active_lang = "All tongues";
    else if(!active_lang || active_lang == "") active_lang = "American";

    pending = (int)who->query_property("pending_skill_slots");

    write(panel_row("SKILLS  (type 'skills' for full list)") + "\n");
    left  = sprintf("Known skills: %d", sizeof(all_skills));
    right = "Active language: " + active_lang;
    write(panel_two_col(left, right) + "\n");
    if(pending > 0)
        write(panel_row_status(
            sprintf("Pending skill slots: %d  (type: improve <skill>)", pending),
            "\e[36m", "\e[0m") + "\n");
}

/* ---- command entry ---- */

int cmd_score(string str) {
    object who;

    who = this_player();

    if((int)who->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
    }

    if(str) {
        notify_fail("Syntax: score\n");
        return 0;
    }

    write(panel_border() + "\n");
    show_header(who);
    write(panel_row("") + "\n");
    show_attributes(who);
    write(panel_row("") + "\n");
    show_health(who);
    write(panel_row("") + "\n");
    show_combat(who);
    write(panel_row("") + "\n");
    show_experience(who);
    write(panel_row("") + "\n");
    show_wealth(who);
    write(panel_row("") + "\n");
    show_skills(who);
    write(panel_border() + "\n");
    return 1;
}

void help() {
    write("Syntax: score\n");
    write("Displays a compact boxed character panel with stats, health,\n");
    write("combat, experience, and skills summary.\n");
}
