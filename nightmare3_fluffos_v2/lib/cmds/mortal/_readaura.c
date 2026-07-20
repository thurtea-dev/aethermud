// /cmds/mortal/_readaura.c
// Read aura: psychically perceive a target's true race and level,
// piercing any disguise (metamorph, visible_race, the secondary
// vampire hardcode, or appearance-override armor).

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_readaura(string str) {
    object target;
    mapping pdata;
    string psi_name;
    int isp;
    int isp_cost;

    if(!str || !sizeof(str))
        return notify_fail("Syntax: readaura <target>\n");

    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race()))
        return notify_fail("You do not have psionic abilities.\n");

    if(!(int)RIFTS_D->player_has_psi_access(this_player()))
        return notify_fail("You do not have psionic abilities.\n");

    psi_name = "read aura";
    pdata = (mapping)RIFTS_PSIONICS_D->query_psionic(psi_name);
    if(!pdata) {
        write("You do not know how to read auras.\n");
        return 1;
    }
    if(!(int)RIFTS_D->player_knows_psionic(this_player(), psi_name)) {
        write("You do not know how to read auras.\n");
        return 1;
    }

    /* Unlike remoteview, this does NOT require the target to be known:
       the whole point is unmasking a stranger. find_psi_aura_target()
       matches by real name (if you happen to know it) or by the race
       noun currently displayed to you, and never checks knows_player(). */
    target = RIFTS_PSIONICS_D->find_psi_aura_target(this_player(), lower_case(str));
    if(!target) {
        write("You cannot find anyone by that name or description within psychic range.\n"
              "They must be online and visible to you in some form.\n");
        return 1;
    }

    isp_cost = (int)pdata["isp_cost"];
    isp = (int)this_player()->query_stats("ISP");
    if(isp < isp_cost) {
        write("You do not have enough ISP. (Need " + isp_cost +
            ", have " + isp + ".)\n");
        return 1;
    }

    if(sizeof((object *)this_player()->query_attackers()) > 0) {
        if(!(int)RIFTS_COMBAT_D->can_do_attack(this_player())) return 1;
        RIFTS_COMBAT_D->use_rifts_attack(this_player());
    }

    this_player()->set_stats("ISP", isp - isp_cost);
    RIFTS_PSIONICS_D->apply_psionic_effect(psi_name, target);
    return 1;
}

void help() {
    write(
        "Syntax: readaura <target>\n\n"
        "Psychically read a target's aura, revealing their true race and\n"
        "power level to you alone, even if they are currently disguised\n"
        "or displaying as something else. Works on strangers: unlike\n"
        "remoteview, this does not require you to have introduced,\n"
        "been introduced, or remembered them first.\n\n"
        "This is a one-time read. It does not introduce you to the\n"
        "target, does not add them to your known list, and reveals\n"
        "nothing to anyone else in the room.\n\n"
        "Costs ISP. The generic 'psi'/'cast' dispatch commands will\n"
        "refuse to target a stranger, so always use readaura directly.\n\n"
        "See also: psi, psionics, introduce, remember, known\n"
    );
}
