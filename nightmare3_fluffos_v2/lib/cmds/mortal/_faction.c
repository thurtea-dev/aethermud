// /cmds/mortal/_faction.c
// Displays the player's standing with major Rifts Earth factions.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_faction(string str) {
    mapping all;
    string *factions;
    string name;
    string membership;
    string rank;
    string display_name;
    int i;
    int standing;
    string tier;
    string display;

    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
    }

    name    = (string)this_player()->query_name();
    all     = (mapping)FACTION_D->query_all_standings(name);
    factions = (string *)FACTION_D->query_faction_list();

    write("%^BOLD%^=== FACTION STANDINGS ===%^RESET%^");
    membership = (string)FACTION_D->query_faction(name);
    if(membership && sizeof(membership)) {
        display_name = (string)FACTION_D->query_faction_member_display(name);
        rank = (string)FACTION_D->query_rank(name);
        if(display_name && sizeof(display_name)) {
            if(rank && sizeof(rank))
                write("Membership: " + display_name + " (" + rank + ")");
            else
                write("Membership: " + display_name);
        } else
            write("Membership: " + membership + " (secret)");
    } else
        write("Membership: none");

    if((int)WANTED_D->is_cs_enemy(this_player()))
        write("%^RED%^CS threat: Coalition forces will attack on sight.%^RESET%^");
    else
        write("CS threat: Coalition forces do not actively seek you.");

    write("");
    write(sprintf("%-22s %8s  %s", "Faction", "Score", "Standing"));
    write("-----------------------------------------------");

    for(i = 0; i < sizeof(factions); i++) {
        standing = all[factions[i]];
        if(!intp(standing)) standing = 0;
        tier     = (string)FACTION_D->query_standing_name(name, factions[i]);
        display  = (string)FACTION_D->query_faction_display_name(factions[i]);

        if(standing <= -200)
            write(sprintf("%^RED%^%-22s %8d  %s%^RESET%^", display, standing, tier));
        else if(standing <= -50)
            write(sprintf("%^ORANGE%^%-22s %8d  %s%^RESET%^", display, standing, tier));
        else if(standing >= 200)
            write(sprintf("%^GREEN%^%-22s %8d  %s%^RESET%^", display, standing, tier));
        else if(standing >= 50)
            write(sprintf("%^CYAN%^%-22s %8d  %s%^RESET%^", display, standing, tier));
        else
            write(sprintf("%-22s %8d  %s", display, standing, tier));
    }

    write("");
    write("Standings range from -1000 (Kill on Sight) to +1000 (Exalted).");
    return 1;
}

void help() {
    message("help",
        "Syntax: faction\n\n"
        "Shows your current standing with the major factions of Rifts Earth.\n\n"
        "Standing tiers:\n"
        "  Exalted      (+500 to +1000) - Trusted ally\n"
        "  Honored      (+200 to +499)  - Known friend\n"
        "  Friendly     ( +50 to +199)  - Welcomed\n"
        "  Neutral      ( -49 to  +49)  - No strong opinion\n"
        "  Unfriendly   (-200 to  -50)  - Treated with suspicion\n"
        "  Hostile      (-499 to -201)  - Attacked on sight\n"
        "  Kill on Sight(-1000 to -500) - Executed without warning\n\n"
        "Faction standing affects NPC reactions, shop access, and quest availability.\n"
        "See also: wanted, reputation",
        this_player());
}
