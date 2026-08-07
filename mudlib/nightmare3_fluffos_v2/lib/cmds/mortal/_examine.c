/* /cmds/mortal/_examine.c
   Examine an object: shows the long description plus MDC/SDC type,
   current condition, and value. For room scenery items without a real
   object, falls back to the plain description like 'look'. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private string item_flags(object ob) {
    string out;
    int mdc_w, sdc_w, mdc_a, sdc_a;
    int cur_mdc, max_mdc, cur_sdc, max_sdc, ar;
    int val;
    string dmg;

    out = "";

    /* weapon flags */
    mdc_w = (int)ob->query_property("mdc_weapon");
    sdc_w = (int)ob->query_property("sdc_weapon");
    dmg   = (string)ob->query_property("damage_string");
    if(mdc_w || sdc_w) {
        if(mdc_w)
            out += "Type: MDC weapon\n";
        else
            out += "Type: SDC weapon\n";
        if(dmg && sizeof(dmg))
            out += "Damage: " + dmg + "\n";
    }

    /* armor flags */
    mdc_a = (int)ob->query_property("mdc_armor");
    sdc_a = (int)ob->query_property("sdc_armor");
    ar    = (int)ob->query_property("armor_ar");
    if(mdc_a) {
        max_mdc = (int)ob->query_property("armor_mdc");
        cur_mdc = (int)ob->query_property("current_armor_mdc");
        if(cur_mdc <= 0) cur_mdc = max_mdc;
        out += "Type: MDC armor\n";
        if(max_mdc > 0)
            out += "Condition: " + cur_mdc + "/" + max_mdc + " MDC\n";
        if(ar > 0)
            out += "Armor Rating: " + ar + "\n";
    } else if(sdc_a) {
        max_sdc = (int)ob->query_property("armor_sdc");
        cur_sdc = (int)ob->query_property("current_armor_sdc");
        if(cur_sdc <= 0) cur_sdc = max_sdc;
        out += "Type: SDC armor\n";
        if(max_sdc > 0)
            out += "Condition: " + cur_sdc + "/" + max_sdc + " SDC\n";
        if(ar > 0)
            out += "Armor Rating: " + ar + "\n";
    }

    /* value */
    val = (int)ob->query_value();
    if(val > 0)
        out += "Value: " + val + " credits\n";

    return out;
}

int cmd_examine(string str) {
    object env;
    object ob;
    object *obs;
    string tmp;
    string flags;
    string att_name;
    string ob_name;
    int i;

    if(!str) {
        write("Examine what?\n");
        return 1;
    }
    if(sscanf(str, "at %s", tmp) == 1)
        str = tmp;

    if(effective_light(this_player()) < 1) {
        write("It is too dark.\n");
        return 1;
    }

    env = environment(this_player());

    /* check room scenery first */
    if(env->id(str)) {
        write((string)env->query_long(str));
        return 1;
    }

    /* check room contents */
    ob = present(str, env);
    if(!ob) ob = present(str, this_player());

    if(!ob) {
        write("You do not notice that here.\n");
        return 1;
    }

    if((int)this_player()->query_invis() == 0) {
        obs = all_inventory(env);
        for(i = 0; i < sizeof(obs); i++) {
            if(!obs[i] || obs[i] == this_player()) continue;
            if(living(ob) && obs[i] == ob) continue;
            att_name = (string)this_player()->query_display_name(obs[i]);
            if(living(ob)) {
                ob_name = (string)ob->query_display_name(obs[i]);
                tell_object(obs[i], att_name + " examines " + ob_name + ".\n");
            } else
                tell_object(obs[i], att_name + " examines the " + str + ".\n");
        }
    }

    write((string)ob->query_long(str));

    flags = item_flags(ob);
    if(sizeof(flags))
        write(flags);

    return 1;
}

void help() {
    write(
        "Syntax: examine <object>\n\n"
        "Examines an object closely. Shows the description plus MDC/SDC\n"
        "type, current condition, and trade value where applicable.\n\n"
        "See also: look, inventory, eq\n"
    );
}
