/* /domains/adm/wiz_tools/staff_supplies_chest.c
 * Reusable open chest stocked with role handbooks and standard wiz-tools.
 * Used in admin workrooms for staff discoverability.
 */

#include <std.h>

inherit STORAGE;

#define WIZ_TOOL_PATH "/domains/adm/wiz_tools/"

private void ensure_handbook(string role, string bname, string *bids);
private void ensure_tool(string tool);
private void stock_supplies();

void create() {
    ::create();
    set_name("staff supplies chest");
    set_id( ({ "chest", "staff chest", "staff supplies chest",
               "supplies chest", "wizard chest" }) );
    set_short("a staff supplies chest");
    set_long(
        "A reinforced crate stenciled STAFF SUPPLIES. It holds wizard\n"
        "handbooks, reference books, and standard wiz-tools for each role.\n"
        "The lid is open. Look inside, or take items from the chest.\n"
        "Example: take admin handbook from chest\n"
        "Full docs: help staff *wizard staff\n"
    );
    set_mass(5000);
    set_value(0);
    set_max_encumbrance(15000);
    set_can_close(1);
    set_property("no_get", 1);
    set_closed(0);
    stock_supplies();
}

void reset() {
    ::reset();
    stock_supplies();
}

private void stock_supplies() {
    ensure_handbook("admin", "admin handbook",
        ({ "admin handbook", "admin book", "book", "manual" }));
    ensure_handbook("domain", "domain wizard handbook",
        ({ "domain handbook", "domain book" }));
    ensure_handbook("coding", "coding wizard handbook",
        ({ "coding handbook", "coding book" }));
    ensure_handbook("rp", "roleplay wizard handbook",
        ({ "roleplay handbook", "rp handbook", "rp book" }));
    ensure_tool("staff_of_demotion");
    ensure_tool("staff_of_dominion");
    ensure_tool("staff_of_creation");
    ensure_tool("rp_skill_tool");
    ensure_tool("tattoo_gun");
    ensure_tool("skill_slip");
}

private void ensure_handbook(string role, string bname, string *bids) {
    object *inv;
    object bk;
    int i;

    inv = all_inventory(this_object());
    for(i = 0; i < sizeof(inv); i++) {
        if(base_name(inv[i]) != WIZ_TOOL_PATH + "wiz_reference_book") continue;
        if((string)inv[i]->query_property("book_role") == role) return;
    }
    bk = clone_object(WIZ_TOOL_PATH + "wiz_reference_book");
    if(!bk) return;
    bk->set_property("book_role", role);
    bk->set_name(bname);
    bk->set_id(bids);
    bk->set_short("the " + bname);
    bk->move(this_object());
}

private void ensure_tool(string tool) {
    object *inv;
    string path;
    object ob;
    int i;

    path = WIZ_TOOL_PATH + tool;
    inv = all_inventory(this_object());
    for(i = 0; i < sizeof(inv); i++) {
        if(base_name(inv[i]) == path) return;
    }
    ob = clone_object(path);
    if(ob) ob->move(this_object());
}
