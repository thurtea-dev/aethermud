// /secure/daemon/wiztools.c
// AetherMUD  -  Wizard tool distribution daemon
// Called from user.c set_position() when a player is promoted to a wiz rank.
// Gives the appropriate staff/tool(s) to the new wizard.

#include <std.h>
#include <security.h>

#define WIZ_TOOL_PATH "/domains/adm/wiz_tools/"

void remove_tools(object player) {
    object *inv;
    string bn;
    int i;

    if(!player || !objectp(player)) return;
    inv = all_inventory(player);
    for(i = 0; i < sizeof(inv); i++) {
        bn = base_name(inv[i]);
        if(bn && strsrch(bn, "wiz_tools") != -1) {
            inv[i]->remove();
            if(objectp(inv[i])) destruct(inv[i]);
        }
    }
}

void give_tools(object player, string position) {
    object ob;

    if(!player || !objectp(player)) return;
    if(!interactive(player))        return;

    remove_tools(player);

    switch(lower_case(position)) {

    case "head arch":
    case "arch":
        ob = clone_object(WIZ_TOOL_PATH + "staff_of_demotion");
        if(ob) {
            ob->move(player);
            tell_object(player,
                "The powers of administration place a staff of demotion in your hands.");
        }
        ob = clone_object(WIZ_TOOL_PATH + "staff_of_dominion");
        if(ob) ob->move(player);
        ob = clone_object(WIZ_TOOL_PATH + "staff_of_creation");
        if(ob) ob->move(player);
        ob = clone_object(WIZ_TOOL_PATH + "rp_skill_tool");
        if(ob) ob->move(player);
        ob = clone_object(WIZ_TOOL_PATH + "tattoo_gun");
        if(ob) ob->move(player);
        break;

    case "creator":
    case "immortal":
        ob = clone_object(WIZ_TOOL_PATH + "staff_of_creation");
        if(ob) {
            ob->move(player);
            tell_object(player,
                "The powers of creation place a staff of creation in your hands.");
        }
        break;

    case "rp_wiz":
        ob = clone_object(WIZ_TOOL_PATH + "rp_skill_tool");
        if(ob) {
            ob->move(player);
            tell_object(player,
                "The powers of roleplay place an RP-Wiz Skill Tool in your hands.");
        }
        ob = clone_object(WIZ_TOOL_PATH + "tattoo_gun");
        if(ob) {
            ob->move(player);
            tell_object(player, "A tattoo-gun materializes in your hands.");
        }
        ob = clone_object(WIZ_TOOL_PATH + "skill_slip");
        if(ob) ob->move(player);
        break;

    case "domain_wiz":
        ob = clone_object(WIZ_TOOL_PATH + "staff_of_dominion");
        if(ob) {
            ob->move(player);
            tell_object(player,
                "The powers of the domains place a staff of dominion in your hands.");
        }
        break;

    case "apprentice":
        ob = clone_object(WIZ_TOOL_PATH + "apprentice_kit");
        if(ob) {
            ob->move(player);
            tell_object(player,
                "An apprentice kit appears in your hands. Type 'kit' to begin\n"
                "your track task (rp, domain, or coding).");
        }
        break;

    default:
        break;
    }
}
