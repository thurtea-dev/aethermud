/* /cmds/adm/_stat.c
   Show detailed stats for an online player. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_stat(string str) {
    object target;
    string name, race, occ, align_name;
    int lv, exp, hp, maxhp, sdc, maxsdc, mdc, maxmdc, ppe, maxppe, isp, maxisp;
    int iq, me, ma, ps, pp, pe, pb, spd;
    string hth, flags, faction;

    if(!creatorp(this_player())) {
        write("Access denied.\n");
        return 1;
    }
    if(!str || !strlen(str)) {
        write("Syntax: stat <player>\n");
        return 1;
    }
    name   = lower_case(str);
    target = find_player(name);
    if(!target) {
        write("Player '" + capitalize(name) + "' is not online.\n");
        return 1;
    }

    race      = (string)target->query_race();
    occ       = (string)target->getenv("rifts_occ");
    align_name = (string)target->getenv("rifts_alignment");
    lv        = (int)target->query_level();
    exp       = (int)target->query_exp();
    hp        = (int)target->query_stats("rifts_hp");
    maxhp     = (int)target->query_stats("max_rifts_hp");
    sdc       = (int)target->query_stats("SDC");
    maxsdc    = (int)target->query_stats("max_SDC");
    mdc       = (int)target->query_stats("MDC");
    maxmdc    = (int)target->query_stats("max_MDC");
    ppe       = (int)target->query_stats("PPE");
    maxppe    = (int)target->query_stats("max_PPE");
    isp       = (int)target->query_stats("ISP");
    maxisp    = (int)target->query_stats("max_ISP");
    iq        = (int)target->query_stats("IQ");
    me        = (int)target->query_stats("ME");
    ma        = (int)target->query_stats("MA");
    ps        = (int)target->query_stats("PS");
    pp        = (int)target->query_stats("PP");
    pe        = (int)target->query_stats("PE");
    pb        = (int)target->query_stats("PB");
    spd       = (int)target->query_stats("Spd");
    hth       = (string)target->getenv("rifts_hth");
    flags     = (string)target->getenv("rifts_flags");

    if(!occ)       occ       = "none";
    if(!align_name) align_name = "Unknown";
    if(!hth)       hth       = "basic";
    if(!flags)     flags     = "";

    write("=== STAT: " + (string)target->query_cap_name() + " ===\n");
    write(sprintf("  Race: %-20s OCC: %s\n", capitalize(race), capitalize(occ)));
    write(sprintf("  Level: %-19d XP: %d\n", lv, exp));
    write(sprintf("  Alignment: %-16s H2H: %s\n", align_name, capitalize(hth)));
    write(sprintf("  HP: %d/%d   SDC: %d/%d   MDC: %d/%d\n",
        hp, maxhp, sdc, maxsdc, mdc, maxmdc));
    write(sprintf("  PPE: %d/%d   ISP: %d/%d\n", ppe, maxppe, isp, maxisp));
    write(sprintf("  IQ:%-4d ME:%-4d MA:%-4d PS:%-4d\n", iq, me, ma, ps));
    write(sprintf("  PP:%-4d PE:%-4d PB:%-4d Spd:%-4d\n", pp, pe, pb, spd));
    if(sizeof(flags))
        write("  Flags: " + flags + "\n");
    write(sprintf("  Location: %s\n",
        environment(target) ?
        (string)environment(target)->query_short() : "(nowhere)"));
    write(sprintf("  Credits: %d\n", (int)target->query_money("credits")));
    return 1;
}

void help() {
    write("Syntax: stat <player>\n\nShow detailed Rifts stats for an online player.\n");
}
