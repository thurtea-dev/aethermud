// /cmds/mortal/_remoteview.c
// Remote viewing: psychically locate a known player by name.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_remoteview(string str) {
    object target;
    mapping pdata;
    string psi_name;
    int isp;
    int isp_cost;

    if(!str || !sizeof(str))
        return notify_fail("Syntax: remoteview <name>\n");

    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race()))
        return notify_fail("You do not have psionic abilities.\n");

    if(!(int)RIFTS_D->player_has_psi_access(this_player()))
        return notify_fail("You do not have psionic abilities.\n");

    psi_name = "remote viewing";
    pdata = (mapping)RIFTS_PSIONICS_D->query_psionic(psi_name);
    if(!pdata) {
        write("You do not know remote viewing.\n");
        return 1;
    }
    if(!(int)RIFTS_D->player_knows_psionic(this_player(), psi_name)) {
        write("You do not know remote viewing.\n");
        return 1;
    }

    /* Self-target first: you always know yourself, so never send self
       through the known-player range gate (which excludes the caster). */
    str = lower_case(str);
    if(str == "me" || str == "self" ||
       str == lower_case((string)this_player()->query_name()))
        target = this_player();
    else
        target = RIFTS_PSIONICS_D->find_psi_remote_target(this_player(), str);
    if(!target) {
        write("You cannot find anyone by that name within psychic range.\n"
              "They must be online and you must know them (introduce or remember).\n");
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
        "Syntax: remoteview <name>\n\n"
        "Psionic remote viewing on a player you know. They must be online\n"
        "and you must have met them (introduce) or remembered them.\n"
        "Costs ISP. Also available as: psi remote viewing at <name>\n\n"
        "See also: psi, psionics, introduce, remember\n"
    );
}
