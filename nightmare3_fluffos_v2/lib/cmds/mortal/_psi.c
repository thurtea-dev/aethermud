/* /cmds/mortal/_psi.c
   Syntax: psi <psionic> [at <target>] */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

/* Psionic effects whose fx_ handler in rifts_psionics_d.c requires a
   resolved target and fails without one. Checked before ISP and APM are
   spent so a failed use costs nothing. Keep in sync with the !target
   guards there. */
#define NEED_TARGET_EFFECTS ({ \
    "read_thoughts", "empathy", "obj_read", "tk", "mind_bolt", \
    "mind_bolt_heavy", "pyro_bolt", "pyro_blast", "psychic_diag", \
    "psych_surgery", "induce_sleep", "clairvoyance", "remote_viewing", \
    "bio_manipulation", "tk_punch", "accelerate_healing", \
    "psychic_purification", "electrokinesis", "hypnotic_suggestion", \
    "psychic_locator", "mental_stun", "mind_wipe" })

int cmd_psi(string str) {
    string psi_name;
    string target_str;
    object target;
    mapping pdata;
    int isp;
    int isp_cost;
    int i;

    if(!str || !sizeof(str)) {
        write("Use which psionic power?  Syntax: psi <power> [at <target>]\n");
        return 1;
    }

    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race())) {
        notify_fail("You do not have psionic abilities.\n");
        return 0;
    }

    if(!(int)RIFTS_D->player_has_psi_access(this_player())) {
        write("You do not have psionic abilities.\n");
        return 1;
    }

    /* Parse "psionic at target" */
    i = strsrch(str, " at ");
    if(i > 0) {
        psi_name   = lower_case(str[0..i-1]);
        target_str = lower_case(str[i+4..]);
        if(!sizeof(target_str)) target_str = 0;
    } else {
        psi_name   = lower_case(str);
        target_str = 0;
    }

    psi_name = (string)RIFTS_PSIONICS_D->normalize_psionic_name(psi_name);

    pdata = (mapping)RIFTS_PSIONICS_D->query_psionic(psi_name);
    if(!pdata) {
        write("You don't know the psionic power '" + psi_name + "'.\n");
        return 1;
    }

    if(!(int)RIFTS_D->player_knows_psionic(this_player(), psi_name)) {
        write("You don't know the psionic power '" + psi_name + "'.\n");
        return 1;
    }

    isp_cost = (int)pdata["isp_cost"];
    isp = (int)this_player()->query_stats("ISP");
    if(isp < isp_cost) {
        write("You don't have enough ISP.  (Need " + isp_cost +
              ", have " + isp + ".)\n");
        return 1;
    }

    target = 0;
    if(target_str) {
        if((string)pdata["range"] == "remote")
            target = RIFTS_PSIONICS_D->find_psi_remote_target(this_player(), target_str);
        if(!target)
            target = present(target_str, environment(this_player()));
    }

    /* Target and range validation happens before any resources are
       spent: a failed use costs nothing. */
    if(target_str && !target) {
        if((string)pdata["range"] == "remote")
            write("You don't sense anyone like that within range.\n");
        else
            write("You don't see '" + target_str + "' here.\n");
        return 1;
    }
    if(!target &&
       member_array((string)pdata["effect"], NEED_TARGET_EFFECTS) != -1) {
        write("Use on whom?  Syntax: psi " + psi_name + " at <target>\n");
        return 1;
    }

    if(target && userp(target) && target != this_player() &&
       !creatorp(this_player()) &&
       !(int)this_player()->knows_player((string)target->query_name())) {
        write("You don't know anyone by that name well enough to focus "
              "your psionics on them. Try 'introduce' when you meet them "
              "in person.\n");
        return 1;
    }

    /* In combat: spend an APM */
    if(sizeof((object *)this_player()->query_attackers()) > 0) {
        if(!(int)RIFTS_COMBAT_D->can_do_attack(this_player())) return 1;
        RIFTS_COMBAT_D->use_rifts_attack(this_player());
    }

    /* Deduct ISP */
    this_player()->set_stats("ISP", isp - isp_cost);

    /* Apply effect */
    RIFTS_PSIONICS_D->apply_psionic_effect(psi_name, target);
    return 1;
}

int do_psi_power(string psi_name, string args) {
    string cmd;

    if(!psi_name || !sizeof(psi_name)) return 0;
    if(!args || !sizeof(args))
        cmd = psi_name;
    else
        cmd = psi_name + " at " + args;
    return cmd_psi(cmd);
}

int do_telepathy_send(string target_str, string message) {
    object target;
    mapping pdata;
    int isp;
    int isp_cost;
    string psi_name;

    psi_name = "telepathy";
    if(!target_str || !sizeof(target_str)) {
        write("Telepathy whom?  Syntax: telepathy <target> <message>\n");
        return 1;
    }
    if(!message || !sizeof(message)) {
        write("What message do you want to send?\n");
        return 1;
    }

    if(!(int)RIFTS_D->is_rifts_race((string)this_player()->query_race())) {
        write("You do not have psionic abilities.\n");
        return 1;
    }

    if(!(int)RIFTS_D->player_has_psi_access(this_player())) {
        write("You do not have psionic abilities.\n");
        return 1;
    }

    pdata = (mapping)RIFTS_PSIONICS_D->query_psionic(psi_name);
    if(!pdata) {
        write("You don't know the psionic power '" + psi_name + "'.\n");
        return 1;
    }

    if(!(int)RIFTS_D->player_knows_psionic(this_player(), psi_name)) {
        write("You don't know the psionic power '" + psi_name + "'.\n");
        return 1;
    }

    isp_cost = (int)pdata["isp_cost"];
    isp = (int)this_player()->query_stats("ISP");
    if(isp < isp_cost) {
        write("You don't have enough ISP.  (Need " + isp_cost +
              ", have " + isp + ".)\n");
        return 1;
    }

    target = present(target_str, environment(this_player()));
    if(!target || !living(target))
        target = find_living(target_str);
    if(!target || !living(target)) {
        write("You don't sense anyone like that within range.\n");
        return 1;
    }
    if(userp(target) && !creatorp(this_player())) {
        if(!(int)this_player()->knows_player((string)target->query_name())) {
            write("You don't know anyone by that name well enough to reach "
                  "their mind. Try 'introduce' when you meet them in person.\n");
            return 1;
        }
    }
    if(userp(target) && !interactive(target)) {
        write((string)target->query_cap_name() +
              " is link-dead and cannot hear you.\n");
        return 1;
    }
    if((int)target->query_property("mind_blocked")) {
        write(target->query_cap_name() +
              "'s mind is shielded. Your message cannot get through.\n");
        return 1;
    }

    if(sizeof((object *)this_player()->query_attackers()) > 0) {
        if(!(int)RIFTS_COMBAT_D->can_do_attack(this_player())) return 1;
        RIFTS_COMBAT_D->use_rifts_attack(this_player());
    }

    this_player()->set_stats("ISP", isp - isp_cost);
    RIFTS_PSIONICS_D->apply_telepathy_send(target, message);
    return 1;
}

int do_telepathy(string str) {
    string target_str;
    string message;
    int i;

    if(!str || !sizeof(str)) {
        write("Telepathy whom?  Syntax: telepathy <target> [message]\n");
        return 1;
    }
    i = strsrch(str, " ");
    if(i < 0)
        return do_psi_power("telepathy", str);

    target_str = lower_case(str[0..i - 1]);
    message = str[i + 1..];
    if(!message || !sizeof(message))
        return do_psi_power("telepathy", target_str);

    return do_telepathy_send(target_str, message);
}

void help() {
    write(
        "Syntax: psi <power> [at <target>]\n\n"
        "Activate a psionic ability.  Costs ISP.  In combat, costs 1 APM.\n\n"
        "  Examples:\n"
        "    psi telepathy at guard\n"
        "    psi mind block\n"
        "    psi telekinesis at crate\n\n"
        "Type <psionics> to see your known powers and ISP cost.\n"
    );
}
