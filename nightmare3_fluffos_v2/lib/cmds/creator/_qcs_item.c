/* /cmds/creator/_qcs_item.c - QCS weapon/armor create/modify logic */

#include <std.h>

inherit OBJECT;

#define TPL_WEAPON "/std/obj/templates/qcs_weapon.c"
#define TPL_ARMOR  "/std/obj/templates/qcs_armor.c"

string qcs_realm_base(object player) {
    return "/realms/" + lower_case((string)player->query_name()) + "/area";
}

string qcs_safe_name(string s) {
    return (string)"/cmds/creator/_qcs"->qcs_safe_name(s);
}

string qcs_safe_str(string s) {
    return (string)"/cmds/creator/_qcs"->qcs_safe_str(s);
}

int qcs_write_update(string path, string body) {
    return (int)"/cmds/creator/_qcs"->qcs_write_update(path, body);
}

string qcs_read_template(string tpl, mapping vars) {
    string body;
    string *klist;
    int i;

    body = read_file(tpl);
    if(!body || !sizeof(body)) return 0;
    if(vars && mapp(vars)) {
        klist = keys(vars);
        for(i = 0; i < sizeof(klist); i++)
            body = replace_string(body, klist[i], (string)vars[klist[i]]);
    }
    return body;
}

int qcs_modify_item(string path, string prop, string value, string kind) {
    string num_str;
    string sides_str;
    int num;
    int sides;

    if(prop == "ar" && kind == "armor")
        return (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
            path, "armor_ar", value);
    if(prop == "sdc" && kind == "armor")
        return (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
            path, "armor_sdc", value);
    if(prop == "mdc" && kind == "armor") {
        if(!(int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
                path, "armor_mdc", value))
            return 0;
        (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
            path, "current_armor_mdc", value);
        if(to_int(value) > 0) {
            (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
                path, "mdc_armor", "1");
            (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
                path, "sdc_armor", "0");
        }
        return 1;
    }
    if(prop == "slot" && kind == "armor")
        return (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
            path, "rifts_slot", value);
    if(prop == "mdc" && kind == "weapon") {
        if(!(int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
                path, "mdc_weapon", value))
            return 0;
        if(to_int(value) > 0)
            (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
                path, "sdc_weapon", "0");
        return 1;
    }
    if(prop == "damage" && kind == "weapon") {
        if(sscanf(value, "%sd%s", num_str, sides_str) != 2) {
            write("QCS: damage format must be NdN (e.g. 4d6)\n");
            return 0;
        }
        num   = to_int(num_str);
        sides = to_int(sides_str);
        if(num < 1 || sides < 1) {
            write("QCS: damage values must be positive integers\n");
            return 0;
        }
        if(!(int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
                path, "damage_num", num_str))
            return 0;
        (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
            path, "damage_sides", sides_str);
        (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
            path, "damage_string", value);
        return 1;
    }
    return (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(path, prop, value);
}

int qcs_create_weapon(object player, string fname) {
    string base;
    string safe;
    string path;
    string body;
    string header;
    mapping vars;

    base = qcs_realm_base(player);
    safe = qcs_safe_name(fname);
    path = base + "/weap/" + safe + ".c";
    if(file_size(path) > 0) {
        write("QCS: file already exists: " + path + "\n");
        return 0;
    }
    vars = ([
        "QCS_NAME"  : safe,
        "QCS_ID"    : safe,
        "QCS_SHORT" : "a " + safe,
        "QCS_LONG"  : "A newly forged weapon awaiting description."
    ]);
    body = qcs_read_template(TPL_WEAPON, vars);
    header = "/* " + path + " */\n"
        "/* QCS weapon by " + (string)player->query_name() + " */\n\n";
    body = header + body;
    if(!qcs_write_update(path, body)) return 0;
    write("QCS: weapon written to " + path + "\n");
    return 1;
}

int qcs_create_armor(object player, string fname) {
    string base;
    string safe;
    string path;
    string body;
    string header;
    mapping vars;

    base = qcs_realm_base(player);
    safe = qcs_safe_name(fname);
    path = base + "/armor/" + safe + ".c";
    if(file_size(path) > 0) {
        write("QCS: file already exists: " + path + "\n");
        return 0;
    }
    vars = ([
        "QCS_NAME"  : safe,
        "QCS_ID"    : safe,
        "QCS_SHORT" : safe + " armor",
        "QCS_LONG"  : "A newly crafted armor piece awaiting description."
    ]);
    body = qcs_read_template(TPL_ARMOR, vars);
    header = "/* " + path + " */\n"
        "/* QCS armor by " + (string)player->query_name() + " */\n\n";
    body = header + body;
    if(!qcs_write_update(path, body)) return 0;
    write("QCS: armor written to " + path + "\n");
    return 1;
}
