// /cmds/mortal/_reputation.c
// Show player faction standings.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private string rep_tier(int val) {
    if(val <= -200) return "Enemy";
    if(val <=  -50) return "Hostile";
    if(val <    50) return "Neutral";
    if(val <   150) return "Friendly";
    return "Allied";
}

int cmd_reputation(string str) {
    string name;
    int cs;
    int lazlo;
    int splug;
    int bm;
    int nc;

    name  = lower_case((string)this_player()->query_name());
    cs    = (int)FACTION_D->query_standing(name, "coalition");
    lazlo = (int)FACTION_D->query_standing(name, "lazlo");
    splug = (int)FACTION_D->query_standing(name, "splugorth");
    bm    = (int)FACTION_D->query_standing(name, "black_market");
    nc    = (int)FACTION_D->query_standing(name, "new_camelot");

    write("=== YOUR REPUTATION ===\n");
    write(sprintf("Coalition States:  %5d   %s\n",  cs,    rep_tier(cs)));
    write(sprintf("Lazlo:             %5d   %s\n",  lazlo, rep_tier(lazlo)));
    write(sprintf("Splugorth:         %5d   %s\n",  splug, rep_tier(splug)));
    write(sprintf("Black Market:      %5d   %s\n",  bm,    rep_tier(bm)));
    write(sprintf("New Camelot:       %5d   %s\n",  nc,    rep_tier(nc)));
    write("========================\n");
    return 1;
}
