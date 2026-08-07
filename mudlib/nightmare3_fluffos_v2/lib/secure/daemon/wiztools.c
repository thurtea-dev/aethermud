// /secure/daemon/wiztools.c
// AetherMUD  -  Wizard tool distribution daemon
// Called from user.c set_position() when a player is promoted to a wiz rank.
// Gives the appropriate staff/tool(s) to the new wizard.

#include <std.h>
#include <security.h>

#define WIZ_TOOL_PATH "/domains/adm/wiz_tools/"

/* Every base_name give_tools() below ever clones, across every position
   case (not just head arch/arch's five - rp_wiz's skill_slip and
   apprentice's apprentice_kit are reissued too). remove_tools() only
   strips exactly these paths now, by exact match, instead of a
   "wiz_tools" substring match against the whole directory. The old
   substring match also caught ring_of_dominion.c, a hand-placed
   one-off item that lives in this same directory but is never cloned
   by give_tools() and so was never reissued after being wiped
   (2026-07-29). */
string *REISSUED_TOOLS = ({
    WIZ_TOOL_PATH + "staff_of_demotion",
    WIZ_TOOL_PATH + "staff_of_dominion",
    WIZ_TOOL_PATH + "staff_of_creation",
    WIZ_TOOL_PATH + "rp_skill_tool",
    WIZ_TOOL_PATH + "tattoo_gun",
    WIZ_TOOL_PATH + "skill_slip",
    WIZ_TOOL_PATH + "apprentice_kit",
});

void remove_tools(object player) {
    object *inv;
    string bn;
    int i;

    if(!player || !objectp(player)) return;
    inv = all_inventory(player);
    for(i = 0; i < sizeof(inv); i++) {
        bn = base_name(inv[i]);
        if(bn && member_array(bn, REISSUED_TOOLS) != -1) {
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
