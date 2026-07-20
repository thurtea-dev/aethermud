// /cmds/mortal/_readaura.c
// Read aura: psychically perceive a target's true race and level,
// piercing any disguise (metamorph, visible_race, the secondary
// vampire hardcode, or appearance-override armor). Same room only.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_readaura(string str) {
    object target;
    mapping pdata;
    string psi_name;
    string target_str;
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

    /* Same-room target resolution, matching _cast.c/_kill.c/_psi.c: "me"/
       "self"/own name targets the caster, else present() in the room.
       No knows_player() gate anywhere in this file - unlike remoteview,
       this deliberately works on anyone present, known or disguised. */
    target_str = lower_case(str);
    if(target_str == "me" || target_str == "self" ||
       target_str == lower_case((string)this_player()->query_name()))
        target = this_player();
    else
        target = present(target_str, environment(this_player()));
    if(!target || !living(target)) {
        write("You don't see '" + str + "' here.\n");
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
        "Psychically read the aura of a living target in your room,\n"
        "revealing their true race and power level to you alone. This\n"
        "pierces any disguise: it works even on someone currently\n"
        "displaying as another race or shielded by an appearance\n"
        "override, and even if you have never introduced, been\n"
        "introduced, or remembered them.\n\n"
        "The reading never reveals the target's name. It refers to\n"
        "them only as 'they' or 'their true nature'.\n\n"
        "This is a one-time read, not a relationship change. It does\n"
        "not add the target to your known list, and shows nothing to\n"
        "anyone else in the room.\n\n"
        "Target by whatever word finds them in the room (their name,\n"
        "or an identifying word if they are an NPC), the same as look\n"
        "or kill.\n\n"
        "Costs ISP. The generic 'psi'/'cast' dispatch commands require\n"
        "a known target and will refuse a stranger, so always use\n"
        "readaura directly.\n\n"
        "See also: psi, psionics, introduce, remember, known\n"
    );
}
