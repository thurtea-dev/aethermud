// /cmds/mortal/_equipment.c
// Shows equipped items by body slot and active tattoos.
// Syntax: equipment

#include <std.h>
#include <daemons.h>

inherit DAEMON;

void init() {
    add_action("cmd_equipment", "equipment");
    add_action("cmd_equipment", "eq");
}

private string _slot_label(string slot, int width) {
    string pad;
    int len;
    len = sizeof(slot);
    pad = "";
    while(len < width) { pad += " "; len++; }
    return capitalize(slot) + pad;
}

int cmd_equipment(string str) {
    object player;
    object *inv;
    mapping wielded, worn;
    string *slots;
    string tattoo_line, loc, tname;
    mapping tattoos;
    int i;

    player = this_player();
    if(!player) return 0;

    inv    = all_inventory(player);
    wielded = ([]);
    worn    = ([]);

    // Scan inventory for wielded weapons and worn armor
    for(i = 0; i < sizeof(inv); i++) {
        object ob;
        ob = inv[i];
        if(!objectp(ob)) continue;
        if((mixed)ob->query_property("wielded") ||
           (object)ob->query_wielded() == player) {
            string hand;
            hand = (string)ob->query_property("wield_hand");
            if(!hand || !sizeof(hand)) hand = "right hand";
            wielded[hand] = ob;
        } else if((int)ob->query_is_armor() ||
                  (mixed)ob->query_property("worn")) {
            string slot;
            slot = (string)ob->query_property("wear_slot");
            if(!slot || !sizeof(slot)) slot = (string)ob->query_wear_location();
            if(!slot || !sizeof(slot)) slot = "body";
            worn[slot] = ob;
        }
    }

    write("\n%^BOLD%^Equipment:%^RESET%^\n");
    write("  Right hand:   " +
        (wielded["right hand"]
            ? (string)wielded["right hand"]->query_short()
            : "(empty)") + "\n");
    write("  Left hand:    " +
        (wielded["left hand"]
            ? (string)wielded["left hand"]->query_short()
            : "(empty)") + "\n");

    // Common armor slots
    slots = ({
        "head", "face", "neck", "chest", "back",
        "right arm", "left arm",
        "right hand", "left hand",
        "legs", "right leg", "left leg",
        "feet", "right foot", "left foot"
    });

    for(i = 0; i < sizeof(slots); i++) {
        object piece;
        piece = worn[slots[i]];
        if(piece)
            write("  " + _slot_label(slots[i], 14) +
                  (string)piece->query_short() + "\n");
    }

    // Scan for any worn items in other slots
    {
        string *worn_slots;
        worn_slots = keys(worn);
        for(i = 0; i < sizeof(worn_slots); i++) {
            if(member_array(worn_slots[i], slots) == -1 &&
               worn_slots[i] != "right hand" &&
               worn_slots[i] != "left hand") {
                write("  " + _slot_label(worn_slots[i], 14) +
                      (string)worn[worn_slots[i]]->query_short() + "\n");
            }
        }
    }

    // Tattoo line
    tattoos = (mapping)TATTOO_D->query_tattoos(player);
    if(tattoos && sizeof(tattoos)) {
        string *locs;
        string parts;
        parts = "";
        locs  = keys(tattoos);
        for(i = 0; i < sizeof(locs); i++) {
            if(sizeof(parts)) parts += ", ";
            parts += tattoos[locs[i]] + " (" + locs[i] + ")";
        }
        write("\n  Tattoos: " + parts + "\n");
    } else {
        write("\n  Tattoos: (none)\n");
    }
    write("\n");
    return 1;
}
