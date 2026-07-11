/* /secure/SimulEfun/wiz_role.c
 * RiftsMUD wizard role helpers. position is canonical; wiz_role is synced tag.
 */

#include <config.h>
#include "SimulEfun.h"

string position_to_wiz_role(string pos) {
    if(!pos || !sizeof(pos)) return 0;
    pos = lower_case(pos);
    switch(pos) {
    case "arch":
    case "head arch":
        return "admin";
    case "domain_wiz":
        return "domain";
    case "creator":
    case "immortal":
        return "coding";
    case "rp_wiz":
        return "rp";
    default:
        return 0;
    }
}

string query_wiz_role(object ob) {
    if(!ob) ob = previous_object();
    if(!ob) return 0;
    return position_to_wiz_role((string)ob->query_position());
}

void sync_wiz_role(object ob) {
    string role;

    if(!ob) return;
    role = position_to_wiz_role((string)ob->query_position());
    if(role) ob->setenv("wiz_role", role);
    else ob->remove_env("wiz_role");
}

int admin_wizp(object ob) {
    string pos;

    if(!ob) ob = previous_object();
    if(!ob || !creatorp(ob)) return 0;
    pos = lower_case((string)ob->query_position());
    return (pos == "arch" || pos == "head arch");
}

int domain_wizp(object ob) {
    if(!ob) ob = previous_object();
    if(!ob || !creatorp(ob)) return 0;
    return lower_case((string)ob->query_position()) == "domain_wiz";
}

int coding_wizp(object ob) {
    string pos;

    if(!ob) ob = previous_object();
    if(!ob || !creatorp(ob)) return 0;
    pos = lower_case((string)ob->query_position());
    return (pos == "creator" || pos == "immortal");
}

int rp_wizp(object ob) {
    if(!ob) ob = previous_object();
    if(!ob || !creatorp(ob)) return 0;
    return lower_case((string)ob->query_position()) == "rp_wiz";
}

varargs int has_wiz_tool(object ob, string tool) {
    object *inv;
    string bn;
    int i;

    if(!ob) ob = previous_object();
    if(!ob || !tool || !sizeof(tool)) return 0;
    inv = all_inventory(ob);
    for(i = 0; i < sizeof(inv); i++) {
        bn = base_name(inv[i]);
        if(bn && strsrch(bn, tool) != -1) return 1;
    }
    return 0;
}
