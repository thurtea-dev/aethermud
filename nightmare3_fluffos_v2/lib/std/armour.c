//      /std/armour.c
//      from the Nightmare Mudlib
//      the basic inherited armour object
//      created by Descartes of Borg 25 june 1993
 
#include <std.h>
#include <move.h>
 
inherit OBJECT;
 
private mapping armour_save;
private static mapping armour_static;
 
void unwear();
void extinguish_glow();
void unequip();
void set_not_equipped();
void set_ac(int x);
void set_illuminate(int x);
void set_limbs(mixed *borg);
void set_wear(mixed val);
void set_remove(mixed val);
void set_type(string str);
void set_armour_type(string str);
int query_ac();
mixed *query_limbs();
string query_wear_string();
object query_wear_func();
string query_remove_string();
object query_remove_func();
string query_type();
string query_armour_type();
object query_worn();
void set_appearance_name(string str);
string query_appearance_name();
string query_cosmetic_slot();
int is_protective_armour();
mixed query_wear();
private string resolve_cosmetic_slot();
private string assign_cosmetic_slot(string cos);
private string slot_display_name(string cos);
private object find_worn_slot_item(object wearer, string slot);

private int armour_fits_wearer(object wearer) {
    string race;
    int dragon_form;
    int armor_dragon;

    if(!wearer) return 1;
    race = lower_case((string)wearer->query_race());
    if(strsrch(race, "great horned dragon") == -1) return 1;

    dragon_form = (int)wearer->query_property("dragon_sized");
    if(!dragon_form && !(int)wearer->query_property("metamorphed"))
        dragon_form = 1;
    armor_dragon = (int)this_object()->query_property("dragon_sized");
    if(dragon_form && !armor_dragon)
        return 0;
    if(!dragon_form && armor_dragon)
        return -1;
    return 1;
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("wear", "wear");
    add_action("do_remove", "remove");
    add_action("illuminate", "illuminate");
}
 
void create() {
    ::create();
    set_vendor_type("armour");
    armour_save = ([]);
    armour_static = ([]);
}
 
int wear(string str) {
    mixed *limbs;
    string ret, what, where, slot;
    function f;
    int i, j;

    if(!str) return notify_fail("Wear what?\n");
    if(!id(what = str) && sscanf(str, "%s on %s", what, where) != 2) {
        if(parse_objects(this_player(), str) != this_object()) {
            notify_fail("You don't have that!\n");
            return 0;
        }
    }
    else if(!id(what) && parse_objects(this_player(), what) != this_object()) {
        notify_fail("You don't have that!\n");
        return 0;
    }
    if(query_worn()) {
        message("my_action", "You are already wearing that!", this_player());
        return 1;
    }
    switch(armour_fits_wearer(this_player())) {
    case 0:
        message("my_action",
            "Your massive dragon form cannot fit into this armor.",
            this_player());
        return 1;
    case -1:
        message("my_action",
            "This armor is sized for a dragon. You cannot wear it in your current form.",
            this_player());
        return 1;
    }
    slot = 0;
    if(!is_protective_armour() && (slot = resolve_cosmetic_slot())) {
        /* Cosmetic slot wear (2026-07-19): non-protective pieces occupy
           a cosmetic slot and never touch the limb/damage system. */
        if(functionp(f = query_wear()) && !((*f)())) return 1;
        slot = assign_cosmetic_slot(slot);
        if(!slot) return 1;
        limbs = ({ slot });
    }
    else {
        slot = 0;
        i = sizeof(limbs = query_limbs());
        while(i--) {
            if(stringp(limbs[i])) continue;
            if((j=member_array(where, limbs[i])) != -1) limbs[i] = limbs[i][j];
            else if(where) {
              message("my_action", "You cannot wear this on your "+where+".",
                this_player());
              return 1;
            }
            else {
              message("my_action", "You must specify which limb you want this "
              "on.", this_player());
              return 1;
            }
        }
        if(is_protective_armour() &&
           member_array("whole_body", (string *)this_player()->query_limbs()) != -1)
            limbs = ({ "whole_body" });
        if(ret = (string)this_player()->equip_armour_to_limb(this_object(),limbs)){
            if(ret == "NO") return 1;
            message("my_action", ret, this_player());
            return 1;
        }
    }
    if(functionp(armour_static["wear"])) {}
    else if(stringp(armour_static["wear"]))
      message("my_action", armour_static["wear"], this_player());
    else message("my_action", "You wear "+query_short()+".", this_player());
    message("other_action", (string)this_player()->query_cap_name()+
      " wears "+(string)this_player()->query_possessive()+" "+query_short()+
      ".", environment(this_player()), ({ this_player() }));
    armour_static["worn by"] = this_player();
    armour_static["actual limbs"] = limbs;
    if(slot) armour_static["cosmetic_slot"] = slot;
    return 1;
}
 
int do_remove(string str) {
    if(!str) return notify_fail("Remove what?\n");
    if(!id(str) && parse_objects(this_player(), str) != this_object()) {
        notify_fail("You don't have that!\n");
        return 0;
    }
    if(!query_worn()) {
        message("my_action", "You are not wearing that!", this_player());
        return 1;
    }
	if(functionp(armour_static["unwear"])){
		if(!((*armour_static["unwear"])())) return 1;
	}
    unwear();
    return 1;
}
 
int illuminate(string str) {
    int heure;
 
    if(!query_property("magic item") || 
      member_array("illuminate", query_property("magic item")) == -1) return 0;
    if(!str || !id(str)) {
        notify_fail("Illuminate what?\n");
        return 0;
    }
    heure = 5*((int)this_player()->query_stats("wisdom")) +
      (int)this_player()->query_skill("conjuring");
    if(heure < 20) heure = 20;
    if((int)this_player()->query_mp() < heure/10) {
        write("Too low on magic power.\n");
        return 1;
    }
    this_player()->add_mp(-(heure/10));
    write("Your "+query_name()+" begins to glow.\n");
    say(this_player()->query_cap_name()+"'s "+query_name()+" begins to
glow.\n");
    environment(this_object())->add_property("light",
      (armour_static["lit"] = 1+random(3)));
    call_out("extinguish_glow", heure);
    return 1;
}
 
void unwear() {
    if(stringp(armour_static["unwear"]))
      message("my_action", armour_static["unwear"], armour_static["worn by"]);
    else message("my_action", "You remove your "+query_short()+".",
      armour_static["worn by"]);
    /* Cosmetic slot items never registered on a limb, so skip the limb
       unhook (slot names can collide with real limb names like head). */
    if(!armour_static["cosmetic_slot"])
        armour_static["worn by"]->remove_armour_from_limb(this_object(),
          armour_static["actual limbs"]);
    message("other_action", (string)armour_static["worn by"]->query_cap_name()+
      " removes "+(string)armour_static["worn by"]->query_possessive()
      +" "+query_name()+".", environment(armour_static["worn by"]),
      ({ armour_static["worn by"] }));
    map_delete(armour_static, "worn by");
    map_delete(armour_static, "actual limbs");
    map_delete(armour_static, "cosmetic_slot");
}
 
void extinguish_glow() {
    if(!this_object()) return;
    if(!armour_static["lit"]) return;
    if(living(environment(this_object()))) {
        tell_object(environment(this_object()),"The magic glow of your
"+query_name()+" fades away.\n");
        tell_room(environment(environment(this_object())),
environment(this_object())->query_cap_name()+"'s "+query_name()+
          "magic glow fades away.\n", ({environment(this_object()) }) );
    }
    else tell_room(environment(this_object()), "The magic glow of the "+
      query_name()+" fades away.\n");
    environment(this_object())->add_property("light", -armour_static["lit"]);
    map_delete(armour_static, "lit");
}
 
void unequip() { if(armour_static["worn by"]) unwear(); }
 
void set_not_equipped() {
    if(query_worn()) map_delete(armour_static, "worn by");
    if(armour_static["actual limbs"]) map_delete(armour_static, "actual limbs");
    if(armour_static["cosmetic_slot"]) map_delete(armour_static, "cosmetic_slot");
}
 
int remove() {
    if(armour_static && armour_static["lit"]) {
      if(environment(this_object()))
        environment(this_object())->add_property("light",-armour_static["lit"]);
        map_delete(armour_static, "lit");
    }
    unequip();
    return ::remove();
}
 
int move(mixed dest) {
    int x;
 
    if(armour_static["lit"])
      environment(this_object())->add_property("light", -armour_static["lit"]);
    x = ::move(dest);
    if(armour_static["lit"])
      environment(this_object())->add_property("light", armour_static["lit"]);
    if(x == MOVE_OK) unequip();
    return x;
}
 
void set_ac(int x) { armour_save["ac"] = x; }
 
void set_illuminate(int x) { set_property("magic item", ({ "illuminate" })); }
 
void set_limbs(mixed *borg) { armour_save["possible limbs"] = borg; }
 
void set_wear(mixed val) {
    armour_static["wear"] = val;
}
 
void set_remove(mixed val) {
    armour_static["unwear"] = val;
}
 
void set_type(string str) { armour_save["type"] = str; }

/* Alias for set_type()/query_type(). CLAUDE.md protects this name --
   several equipment files (rebreather.c, faction_amulet.c, and the
   thurtea_test cosmetic-slot samples) call set_armour_type() directly. */
void set_armour_type(string str) { set_type(str); }

int query_ac() { return armour_save["ac"] + query_property("enchantment"); }

mixed *query_limbs() { return armour_save["possible limbs"]; }

mixed query_wear() { return armour_static["wear"]; }

mixed query_unwear() { return armour_static["unwear"]; }

string query_type() { return armour_save["type"]; }

string query_armour_type() { return query_type(); }

object query_worn() { return armour_static["worn by"]; }
 
 
string query_short() {
    if(!armour_static["worn by"]) return ::query_short();
    else return ::query_short()+" (worn)";
}
 
string query_long(string str) {
    string ret;
    int i;
    if(!armour_static["worn by"]) return ::query_long(str);
    else {
        ret = ::query_long(str);
        i = sizeof(armour_static["actual limbs"]);
        ret += "Worn on: "+slot_display_name(armour_static["actual limbs"][--i]);
        if(i>-1) while(i--) ret += ", "+slot_display_name(armour_static["actual limbs"][i]);
        ret += ".\n";
        return ret;
    }
}
 
string *query_actual_limbs() { return armour_static["actual limbs"]; }
 
int is_armour() { return 1; }

void set_struck(mixed val) {
    armour_static["struck"] = val;
}

mixed query_struck() { return armour_static["struck"]; }

/*  Protective armor is worn on whole_body and carries MDC/SDC pools;
    everything else that resolves to a cosmetic slot is display-only.  */
int is_protective_armour() {
    if((int)query_property("mdc_armor")) return 1;
    if((int)query_property("sdc_armor")) return 1;
    if((string)query_type() == "body armour") return 1;
    if((string)query_property("rifts_slot") == "armor") return 1;
    return 0;
}

/*  Map the rifts_slot property (or, failing that, the armour type) to
    one of the cosmetic wear slots: head, neck, shirt, back, belt,
    legs, hands, feet, ring (left/right finger, see assign_cosmetic_slot),
    admin_ring (Ring of Dominion's own dedicated slot, independent of the
    mortal ring pair). Returns 0 when the piece does not fit the slot
    system (shields, legacy NM3 armour types), which falls through to
    the old limb-based path.  */
private string resolve_cosmetic_slot() {
    string slot, type;

    slot = (string)query_property("rifts_slot");
    if(slot == "armor") return 0;
    if(!slot || slot == "") {
        type = (string)query_type();
        if(!type || type == "") return 0;
        slot = type;
    }
    switch(slot) {
    case "head": case "hat": case "helmet": return "head";
    case "neck": case "necklace": case "amulet": return "neck";
    case "shirt": return "shirt";
    case "back": case "cloak": case "backpack": return "back";
    case "belt": return "belt";
    case "legs": case "pants": return "legs";
    case "hands": case "gloves": return "hands";
    case "feet": case "shoes": case "boots": case "boot": return "feet";
    case "ring": case "ring1": case "ring2":
    case "ring_left": case "ring_right": return "ring";
    case "admin_ring": return "admin_ring";
    default: return 0;
    }
}

/*  Pick the concrete slot for this piece, checking occupancy.
    Ring family: an item marked wedding_band claims ring_left
    exclusively (no overflow either way -- refused outright if
    ring_left is taken). A regular ring claims ring_right first,
    then overflows to ring_left if ring_right is occupied and
    ring_left is free. admin_ring (Ring of Dominion) is a distinct,
    single-occupant slot that never competes with the ring pair.  */
private string assign_cosmetic_slot(string cos) {
    if(cos == "ring") {
        if((int)query_property("wedding_band")) {
            if(find_worn_slot_item(this_player(), "ring_left")) {
                message("my_action", "You are already wearing a wedding band.",
                  this_player());
                return 0;
            }
            return "ring_left";
        }
        if(!find_worn_slot_item(this_player(), "ring_right"))
            return "ring_right";
        if(!find_worn_slot_item(this_player(), "ring_left"))
            return "ring_left";
        message("my_action", "You are already wearing two rings.",
          this_player());
        return 0;
    }
    if(find_worn_slot_item(this_player(), cos)) {
        message("my_action", "You are already wearing something on your " +
          slot_display_name(cos) + ". Remove it first.",
          this_player());
        return 0;
    }
    return cos;
}

/*  Human-readable label for a cosmetic slot token, used in "Worn on:"
    and the "already wearing something" refusal. Falls through
    unchanged for real anatomical limb names (head, torso, ...) from
    the old limb-based path, and for any slot not listed here.  */
private string slot_display_name(string cos) {
    switch(cos) {
    case "ring_left": return "left ring finger";
    case "ring_right": return "right ring finger";
    case "admin_ring": return "signet hand";
    default: return cos;
    }
}

private object find_worn_slot_item(object wearer, string slot) {
    object *inv;
    int i;

    inv = all_inventory(wearer);
    if(!inv) return 0;
    for(i=0; i<sizeof(inv); i++) {
        if(!inv[i]) continue;
        if(!((int)inv[i]->is_armour())) continue;
        if((object)inv[i]->query_worn() != wearer) continue;
        if((string)inv[i]->query_cosmetic_slot() == slot) return inv[i];
    }
    return 0;
}

string query_cosmetic_slot() { return armour_static["cosmetic_slot"]; }

/*  Appearance override (2026-07-19): when this piece is worn on
    whole_body, strangers see this string in place of the wearer's
    race-based room-listing name.  Stored verbatim, so include the
    article you want: "A Coalition Dead Boy", "Knight".  */
void set_appearance_name(string str) { armour_save["appearance_name"] = str; }

string query_appearance_name() { return armour_save["appearance_name"]; }
