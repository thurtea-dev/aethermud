/* /cmds/mortal/_eq.c
   Show worn equipment and wielded weapons in formatted slots.
   Syntax: eq */

#include <std.h>

inherit DAEMON;

#define EQ_TOP  "+------------------------------------------------------------+"
#define EQ_HDR  "|                        EQUIPMENT                          |"
#define EQ_SEP  "+------------------------------------------------------------+"

private string slot_name_from_ob(object ob) {
    string slot, type;
    slot = (string)ob->query_property("rifts_slot");
    if(slot && sizeof(slot)) return capitalize(slot);
    type = (string)ob->query_type();
    if(!type || !sizeof(type)) return "Other";
    switch(type) {
    case "body armour": return "Armor";
    case "armour":      return "Armor";
    case "helmet":      return "Head";
    case "gloves":      return "Hands";
    case "boots":       return "Feet";
    case "pants":       return "Legs";
    case "necklace":    return "Neck";
    case "ring":        return "Finger";
    case "cloak":       return "Back";
    case "belt":        return "Belt";
    case "backpack":    return "Back";
    case "shield":      return "Shield";
    case "jacket":      return "Armor";
    default:            return capitalize(type);
    }
}

private string armor_tag(object ob) {
    int max_mdc, cur_mdc, max_sdc, cur_sdc, ar;

    max_mdc = (int)ob->query_property("armor_mdc");
    if(max_mdc > 0) {
        cur_mdc = (int)ob->query_property("current_armor_mdc");
        if(cur_mdc <= 0) cur_mdc = max_mdc;
        return "[MDC " + cur_mdc + "/" + max_mdc + "]";
    }
    max_sdc = (int)ob->query_property("armor_sdc");
    if(max_sdc > 0) {
        cur_sdc = (int)ob->query_property("current_armor_sdc");
        if(cur_sdc <= 0) cur_sdc = max_sdc;
        return "[SDC " + cur_sdc + "/" + max_sdc + "]";
    }
    ar = (int)ob->query_property("armor_ar");
    if(ar > 0) return "[AC " + ar + "]";
    return "";
}

private string eq_row(string label, string item_str) {
    return sprintf("| %-14s %-43s |", label, item_str);
}

int cmd_eq(string str) {
    object *inv, ob;
    string *slot_order;
    mapping slot_map;
    object right_wield, left_wield;
    string ws, base_name, tag, item_str, cur_slot;
    int i, j;

    slot_order = ({ "Head", "Neck", "Shirt", "Back", "Belt",
                    "Legs", "Hands", "Feet" });

    inv = all_inventory(this_player());
    slot_map    = ([]);
    right_wield = 0;
    left_wield  = 0;

    for(i = 0; i < sizeof(inv); i++) {
        ob = inv[i];
        if(!ob) continue;
        if((int)ob->is_weapon() &&
           ob->query_wielded() == this_player()) {
            ws = (string)ob->query_short();
            if(ws && strsrch(ws, "right") != -1) {
                right_wield = ob;
            } else if(ws && strsrch(ws, "left") != -1) {
                left_wield = ob;
            } else if(!right_wield) {
                right_wield = ob;
            }
            continue;
        }
        if((int)ob->is_armour() &&
           ob->query_worn() == this_player()) {
            slot_map[slot_name_from_ob(ob)] = ob;
        }
    }

    write(EQ_TOP + "\n");
    write(EQ_HDR + "\n");
    write(EQ_SEP + "\n");

    /* Armor slot at top with MDC/SDC tag */
    if(slot_map["Armor"]) {
        ob = slot_map["Armor"];
        base_name = (string)ob->query_name();
        tag = armor_tag(ob);
        if(tag && sizeof(tag))
            item_str = sprintf("%-28s %s", capitalize(base_name), tag);
        else
            item_str = capitalize(base_name);
        write(eq_row("Armor:", item_str) + "\n");
    } else {
        write(eq_row("Armor:", "(nothing)") + "\n");
    }
    write("| " + sprintf("%-56s", "") + " |\n");

    for(j = 0; j < sizeof(slot_order); j++) {
        cur_slot = slot_order[j];
        if(slot_map[cur_slot]) {
            ob = slot_map[cur_slot];
            base_name = (string)ob->query_name();
            tag = armor_tag(ob);
            if(tag && sizeof(tag))
                item_str = sprintf("%-28s %s", capitalize(base_name), tag);
            else
                item_str = capitalize(base_name);
        } else {
            item_str = "(nothing)";
        }
        write(eq_row(cur_slot + ":", item_str) + "\n");
    }

    write("| " + sprintf("%-56s", "") + " |\n");
    if(right_wield)
        write(eq_row("Wielded R:", capitalize(
            (string)right_wield->query_name())) + "\n");
    else
        write(eq_row("Wielded R:", "(nothing)") + "\n");

    if(left_wield)
        write(eq_row("Wielded L:", capitalize(
            (string)left_wield->query_name())) + "\n");
    else
        write(eq_row("Wielded L:", "(nothing)") + "\n");

    write(EQ_TOP + "\n");
    return 1;
}

void help() {
    write(
        "Syntax: eq\n\n"
        "Displays worn equipment and wielded weapons by slot.\n\n"
        "See also: worn, inventory, score\n"
    );
}
