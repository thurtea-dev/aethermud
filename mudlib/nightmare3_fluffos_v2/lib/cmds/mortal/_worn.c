// /cmds/mortal/_worn.c
// Display all currently worn armor and equipment in Rifts slot order.

#include <std.h>

inherit DAEMON;

string rifts_slot_label(object ob) {
    string slot, type;

    slot = (string)ob->query_property("rifts_slot");
    if(slot && slot != "") {
        return capitalize(slot);
    }
    type = (string)ob->query_type();
    if(!type || type == "") return "Other";
    switch(type) {
    case "body armour": return "Body";
    case "armour":      return "Body";
    case "helmet":      return "Head";
    case "gloves":      return "Hands";
    case "boots":       return "Feet";
    case "pants":       return "Legs";
    case "shirt":       return "Shirt";
    case "necklace":    return "Neck";
    case "ring":        return "Finger";
    case "cloak":       return "Back";
    case "belt":        return "Belt";
    case "shield":      return "Shield";
    case "jacket":      return "Body";
    default:            return capitalize(type);
    }
}

int cmd_worn(string str) {
    object *inv, ob;
    mapping slot_items;
    string *ordered, label;
    int i, j, count;

    ordered = ({
        "Head", "Neck", "Body", "Shirt", "Back",
        "Belt", "Legs", "Hands", "Feet", "Finger",
        "Shield", "Other"
    });

    inv = all_inventory(this_player());
    slot_items = ([]);
    count = 0;

    for(i = 0; i < sizeof(inv); i++) {
        ob = inv[i];
        if(!ob) continue;
        if(!(int)ob->is_armour()) continue;
        if(ob->query_worn() != this_player()) continue;
        label = rifts_slot_label(ob);
        if(!slot_items[label]) slot_items[label] = ({});
        slot_items[label] += ({ (string)ob->query_short() });
        count++;
    }

    write("You are wearing:\n");
    if(!count) {
        write("  Nothing.\n");
        return 1;
    }

    for(i = 0; i < sizeof(ordered); i++) {
        label = ordered[i];
        if(!slot_items[label]) continue;
        for(j = 0; j < sizeof(slot_items[label]); j++) {
            write(sprintf("  %-8s: %s\n", label, slot_items[label][j]));
        }
    }
    return 1;
}

void help() {
    write(
        "Syntax: worn\n\n"
        "Shows all armor and equipment you currently have on,\n"
        "organized by wear slot (head, body, hands, etc.).\n\n"
        "See also: inventory, score\n"
    );
}
