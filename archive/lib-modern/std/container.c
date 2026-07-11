/*    /std/container.c
 *    from Nightmare IV
 *    object inherited by all objects with inventories
 *    created by Descartes of Borg 940212
 */

#include <std.h>
#include <config.h>
#include <daemons.h>
#include "container.h"

inherit OBJECT;

private nosave int __Encumbrance, __MaxEncumbrance;
private nosave int __IsClosed;

void create() {
    ::create();
    set_property("transparent", 0);
    __IsClosed = 0;
}

void set_transparent(int n) {
    set_property("transparent", n ? 1 : 0);
}

int query_transparent() {
    return (int)query_property("transparent");
}

void set_closed(int n) { __IsClosed = n ? 1 : 0; }
int query_closed() { return __IsClosed; }

/* Appends a period unless the string already ends in sentence punctuation. */
private string end_sentence(string s) {
    string last;

    if(!s || s == "") return s;
    last = s[strlen(s)-1..strlen(s)-1];
    if(last == "." || last == "!" || last == "?") return s;
    return s + ".";
}

int query_contents_visible() {
    if(!__IsClosed) return 1;
    return query_transparent();
}

varargs string look_in(object who) {
    string items;
    string living;

    if(__IsClosed) {
        if(!query_transparent())
            return "It is closed.\n";
        items = describe_item_contents(({}));
        living = describe_living_contents(({}));
        if((!items || items == "") && (!living || living == ""))
            return "Through the transparent surface it appears empty.\n";
        if(!items || items == "") items = "";
        else items = capitalize(items) + "\n";
        return "Through the transparent surface:\n" + items + living;
    }
    items = describe_item_contents(({}));
    living = describe_living_contents(({}));
    if((!items || items == "") && (!living || living == ""))
        return "It is empty.\n";
    if(!items || items == "") items = "";
    else items = capitalize(items) + "\n";
    return items + living;
}

varargs int receive_objects(object ob) {
    if(__IsClosed) {
        if(this_player()) write("The lid is closed.\n");
        return 0;
    }
    if(!ob) ob = previous_object();
    if((int)ob->query_mass() + query_encumbrance() > query_max_encumbrance())
      return 0;
    return 1;
}

int release_objects(object ob) {
    if(__IsClosed) {
        if(this_player()) write("The lid is closed.\n");
        return 0;
    }
    return 1;
}

string describe_living_contents(object *exclude) {
    object *inv;
    object ob;
    string tmp, pos_str, ret;
    int i;
    int is_pc;

    if(!(i = sizeof(inv = query_living_contents() - exclude))) return "";
    ret = "";
    while(i--) {
        ob = inv[i];
        if((int)ob->query_invis()) {
            if(!creatorp(ob) &&
               random(101) < (int)this_player()->query_level())
                ret = sprintf("%sa shadow\n", ret);
            continue;
        }
        tmp = (string)ob->query_short();
        if(!tmp || tmp == "") tmp = (string)ob->query_name();
        if(!tmp || tmp == "") continue;
        is_pc = (int)ob->is_player();
        pos_str = (string)ob->query_property("position_str");
        if(!pos_str || pos_str == "")
            pos_str = (string)ob->query_property("default_position");
        if((!pos_str || pos_str == "") && !is_pc)
            pos_str = (string)RIFTS_D->query_race_idle_string((string)ob->query_race());
        if(!pos_str || pos_str == "" || pos_str == "is standing around.") {
            if(is_pc) pos_str = "is standing around.";
            else {
                /* No custom idle string for this race: just the short  */
                /* description, no default "is standing around." filler. */
                ret = sprintf("%s%s\n", ret, end_sentence(capitalize(tmp)));
                continue;
            }
        }
        ret = sprintf("%s%s %s\n", ret, capitalize(tmp), pos_str);
    }
    return ret;
}

string describe_item_contents(object *exclude) {
    object *inv;
    mapping list;
    string ret, tmp;
    string *shorts;
    string *ids;
    object viewer;
    object *corpses;
    object *blood;
    string *corpse_texts;
    string corpse_lines;
    int i, x;

    viewer = this_player();
    corpses = ({});
    blood = ({});
    list = allocate_mapping(i=sizeof(inv=query_item_contents()-exclude));
    while(i--) {
        if(environment(inv[i]) != this_object()) continue;
        if(viewer && (int)inv[i]->query_invis(viewer)) continue;
        if((int)inv[i]->query_property("is_corpse")) {
            corpses += ({ inv[i] });
            continue;
        }
        if((int)inv[i]->query_property("is_blood")) {
            blood += ({ inv[i] });
            continue;
        }
        if((int)inv[i]->query_property("is_magic_net") &&
           (object)inv[i]->query_victim()) {
            /* Actively binding a target: it reads as attached to the victim, */
            /* not as a standalone item lying in the room. */
            continue;
        }
        if(!(tmp = (string)inv[i]->query_short()))
          tmp = (string)inv[i]->query_name();
        if(!tmp || tmp == "")
          tmp = (string)inv[i]->query_cap_name();
        if((!tmp || tmp == "") && pointerp(ids = inv[i]->query_id()) &&
          sizeof(ids))
          tmp = ids[0];
        if(!tmp || tmp == "") continue;
        if(!list[tmp]) list[tmp] = ({ inv[i] });
        else list[tmp] += ({ inv[i] });
    }

    /* Corpses and blood pools get their own lines, corpses first, ahead */
    /* of the regular per-line item listing below.                       */
    corpse_texts = ({});
    for(i = 0; i < sizeof(corpses); i++) {
        /* Victim name, not the "the corpse of X" query_short() phrasing,   */
        /* which is still used as-is elsewhere (drop messages, examine).   */
        tmp = (string)corpses[i]->query_name();
        if(!tmp || tmp == "") tmp = (string)corpses[i]->query_short();
        if(tmp && tmp != "") corpse_texts += ({ capitalize(tmp) + " corpse." });
    }
    for(i = 0; i < sizeof(blood); i++) {
        tmp = (string)blood[i]->query_short();
        if(tmp && tmp != "") corpse_texts += ({ end_sentence(capitalize(tmp)) });
    }
    corpse_lines = sizeof(corpse_texts) ? implode(corpse_texts, "\n") : "";

    if(!(i = sizeof(shorts = keys(list)))) return corpse_lines;
    /* One line per distinct item, each ending as its own sentence.       */
    /* Multiples of the same item still consolidate ("Two laser rifles.") */
    ret = "";
    for(x = 0; x < i; x++) {
        if(sizeof(list[shorts[x]]) == 1) tmp = capitalize(shorts[x]);
        else tmp = capitalize(consolidate(sizeof(list[shorts[x]]), shorts[x]));
        if(x) ret = sprintf("%s\n", ret);
        ret = sprintf("%s%s", ret, end_sentence(tmp));
    }
    return sizeof(corpse_lines) ? corpse_lines + "\n" + ret : ret;
}

void add_encumbrance(int x) {
    if(x + query_encumbrance() > query_max_encumbrance()) return;
    if((__Encumbrance += x) < 0) __Encumbrance = 0;
}

int query_encumbrance() { return __Encumbrance; }

void set_max_encumbrance(int x) { __MaxEncumbrance = x; } 

int query_max_encumbrance() { return __MaxEncumbrance; }

float query_encumbered() {
    return percent(query_encumbrance(), query_max_encumbrance());
}

object *query_living_contents() { 
    return filter_array(all_inventory(this_object()), "filter_living",
      this_object());
}

object *query_item_contents() {
    return filter_array(all_inventory(this_object()),"filter_items",
      this_object());
}

protected int filter_living(object ob) { return living(ob); }

protected int filter_items(object ob) { return !living(ob); }