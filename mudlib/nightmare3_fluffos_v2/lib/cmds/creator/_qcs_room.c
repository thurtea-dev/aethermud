/* /cmds/creator/_qcs_room.c - QCS room create/modify/wiring logic */

#include <std.h>

inherit OBJECT;

#define TPL_ROOM "/std/obj/templates/qcs_room.c"

string qcs_realm_base(object player) {
    return "/realms/" + lower_case((string)player->query_name()) + "/area";
}

string qcs_safe_name(string s) {
    return (string)"/cmds/creator/_qcs"->qcs_safe_name(s);
}

string qcs_safe_str(string s) {
    return (string)"/cmds/creator/_qcs"->qcs_safe_str(s);
}

string qcs_reverse_dir(string dir) {
    return (string)"/cmds/creator/_qcs"->qcs_reverse_dir(dir);
}

int qcs_write_update(string path, string body) {
    return (int)"/cmds/creator/_qcs"->qcs_write_update(path, body);
}

string qcs_resolve_realm_file(object player, string name) {
    return (string)"/cmds/creator/_qcs"->qcs_resolve_realm_file(player, name);
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

string qcs_insert_exit(string body, string dir, string dest) {
    string insert;
    int idx;

    if(!body || !sizeof(body)) return body;
    if(strsrch(body, "add_exit(\"" + dir + "\"") != -1) return body;
    insert = " add_exit(\"" + qcs_safe_str(dir) + "\", \"" + qcs_safe_str(dest) + "\");\n";
    if(strsrch(body, "set_exits( ([]) );") != -1)
        return replace_string(body, "set_exits( ([]) );",
            "add_exit(\"" + qcs_safe_str(dir) + "\", \"" + qcs_safe_str(dest) + "\");");
    idx = strsrch(body, "}\n");
    if(idx == -1) return body;
    return body[0..idx-1] + insert + body[idx..];
}

int qcs_patch_exit_file(string path, string dir, string dest) {
    string body;

    body = read_file(path);
    if(!body || !sizeof(body)) return 0;
    body = qcs_insert_exit(body, dir, dest);
    return qcs_write_update(path, body);
}

int qcs_patch_set_string(string path, string prop, string value) {
    string body;
    string line;
    string new_line;
    string search1;
    string search2;
    int idx;
    int is_numeric;

    body = read_file(path);
    if(!body || !sizeof(body)) {
        write("QCS: cannot read " + path + "\n");
        return 0;
    }
    search1 = "set_" + prop + "(";
    idx = strsrch(body, search1);
    if(idx == -1 && prop == "indoors") {
        new_line = " set_property(\"indoors\", " +
            (value == "1" || lower_case(value) == "yes" ? "1" : "0") + ");\n";
        idx = strsrch(body, "}\n");
        if(idx == -1) return 0;
        body = body[0..idx-1] + new_line + body[idx..];
        return qcs_write_update(path, body);
    }
    if(idx == -1) {
        search2 = "set_property(\"" + prop + "\"";
        idx = strsrch(body, search2);
        if(idx == -1) {
            write("QCS: property '" + prop + "' not found in " + path + "\n");
            return 0;
        }
        is_numeric = (prop == "armor_ar" || prop == "armor_sdc" ||
                      prop == "armor_mdc" || prop == "mdc_weapon" ||
                      prop == "sdc_weapon" || prop == "mdc_armor" ||
                      prop == "sdc_armor" || prop == "damage_num" ||
                      prop == "damage_sides" || prop == "damage_bonus" ||
                      prop == "mdc_creature" || prop == "current_armor_sdc" ||
                      prop == "current_armor_mdc");
        line = body[idx..];
        if(strsrch(line, "\n") != -1)
            line = line[0..strsrch(line, "\n")];
        if(is_numeric)
            new_line = " set_property(\"" + prop + "\", " + value + ");\n";
        else
            new_line = " set_property(\"" + prop + "\", \"" + qcs_safe_str(value) + "\");\n";
        body = replace_string(body, line, new_line);
        return qcs_write_update(path, body);
    }
    line = body[idx..];
    if(strsrch(line, "\n") != -1)
        line = line[0..strsrch(line, "\n")];
    if(prop == "light" || prop == "level" || prop == "wc" || prop == "ac" ||
       prop == "hp" || prop == "ar" || prop == "sdc" ||
       prop == "mdc" || prop == "armor_mdc")
        new_line = " set_" + prop + "(" + value + ");\n";
    else if(prop == "indoors")
        new_line = " set_property(\"indoors\", " +
            (value == "1" || lower_case(value) == "yes" ? "1" : "0") + ");\n";
    else
        new_line = " set_" + prop + "(\"" + qcs_safe_str(value) + "\");\n";
    body = replace_string(body, line, new_line);
    return qcs_write_update(path, body);
}

int qcs_create_room(object player, string dir, string fname) {
    string base;
    string safe;
    string cur;
    string new_path;
    string new_obj;
    string body;
    string header;
    string realm_prefix;
    mapping vars;

    base = qcs_realm_base(player);
    safe = qcs_safe_name(fname);
    cur = (string)"/cmds/creator/_qcs"->qcs_room_path(player);
    if(!cur || !sizeof(cur)) {
        write("QCS: cannot determine current room path.\n");
        return 0;
    }
    new_path = base + "/room/" + safe + ".c";
    new_obj  = base + "/room/" + safe;
    if(file_size(new_path) > 0) {
        write("QCS: file already exists: " + new_path + "\n");
        return 0;
    }
    vars = ([
        "QCS_SHORT" : "A new area",
        "QCS_LONG"  : "A bare room waiting for description."
    ]);
    body = qcs_read_template(TPL_ROOM, vars);
    header = "/* " + new_path + " */\n"
        "/* QCS room by " + (string)player->query_name() + " */\n\n";
    body = header + body;
    body = qcs_insert_exit(body, qcs_reverse_dir(dir), cur);
    if(!qcs_write_update(new_path, body)) return 0;
    realm_prefix = "/realms/" + lower_case((string)player->query_name()) + "/";
    if(strsrch(cur, realm_prefix) == 0) {
        if(!qcs_patch_exit_file(cur + ".c", dir, new_obj)) return 0;
        write("QCS: room created. Exit '" + dir + "' wired to " + new_obj + "\n");
    } else {
        write("QCS: room created at " + new_path + "\n");
        write("QCS: current room is outside your realm - forward exit not wired.\n");
    }
    return 1;
}

int qcs_modify_file(object player, string path, string prop, string value) {
    if(!path || !sizeof(path)) {
        write("QCS: no file path.\n");
        return 0;
    }
    if(strsrch(path, ".c") == -1 && file_size(path + ".c") > 0)
        path = path + ".c";
    if(file_size(path) <= 0) {
        write("QCS: file not found: " + path + "\n");
        return 0;
    }
    prop = lower_case(prop);
    if(strsrch(path, "/room/") != -1)
        return qcs_patch_set_string(path, prop, value);
    if(strsrch(path, "/npc/") != -1)
        return (int)"/cmds/creator/_qcs_npc"->qcs_modify_npc(path, prop, value);
    if(strsrch(path, "/weap/") != -1)
        return (int)"/cmds/creator/_qcs_item"->qcs_modify_item(path, prop, value, "weapon");
    if(strsrch(path, "/armor/") != -1)
        return (int)"/cmds/creator/_qcs_item"->qcs_modify_item(path, prop, value, "armor");
    return qcs_patch_set_string(path, prop, value);
}

int qcs_modify_target(object player, string target, string prop, string value) {
    string path;

    if(lower_case(target) == "here")
        path = (string)"/cmds/creator/_qcs"->qcs_room_path(player);
    else {
        path = qcs_resolve_realm_file(player, target);
        if(!path) {
            write("QCS: cannot find '" + target + "' in your realm.\n");
            return 0;
        }
    }
    return qcs_modify_file(player, path, prop, value);
}

int qcs_add_reset_line(string path, string clone_path) {
    string body;
    string insert;
    string id;
    string *parts;

    body = read_file(path);
    if(!body || !sizeof(body)) return 0;
    if(strsrch(body, "void reset()") != -1) {
        write("QCS: room already has reset(). Edit manually.\n");
        return 0;
    }
    id = clone_path;
    parts = explode(id, "/");
    id = parts[sizeof(parts)-1];
    if(sizeof(id) > 2 && id[sizeof(id)-2..sizeof(id)-1] == ".c")
        id = id[0..sizeof(id)-3];
    insert = "\nvoid reset() {\n ::reset();\n"
        " if(!present(\"" + id + "\", this_object()))\n"
        "  clone_object(\"" + clone_path + "\")->move(this_object());\n}\n";
    body = body + insert;
    return qcs_write_update(path, body);
}

int qcs_add_to_room(object player, string npcfile) {
    string room;
    string npc_path;
    string safe;

    room = (string)"/cmds/creator/_qcs"->qcs_room_path(player);
    safe = qcs_safe_name(npcfile);
    npc_path = qcs_realm_base(player) + "/npc/" + safe;
    if(file_size(npc_path + ".c") <= 0) {
        write("QCS: NPC file not found: " + npc_path + ".c\n");
        return 0;
    }
    return qcs_add_reset_line(room + ".c", npc_path);
}

int qcs_add_to_room_file(object player, string roomfile, string npcfile) {
    string room;
    string npc_path;
    string safe;

    safe = qcs_safe_name(roomfile);
    room = qcs_realm_base(player) + "/room/" + safe + ".c";
    safe = qcs_safe_name(npcfile);
    npc_path = qcs_realm_base(player) + "/npc/" + safe;
    if(file_size(room) <= 0) {
        write("QCS: room not found: " + room + "\n");
        return 0;
    }
    if(file_size(npc_path + ".c") <= 0) {
        write("QCS: NPC file not found: " + npc_path + ".c\n");
        return 0;
    }
    return qcs_add_reset_line(room, npc_path);
}

int qcs_delete_object(object player, string name) {
    object ob;
    object env;

    if(!name || !sizeof(name)) {
        write("Syntax: delete <object>\n");
        return 0;
    }
    env = environment(player);
    ob = present(lower_case(name), env);
    if(!ob) ob = present(name, env);
    if(!ob) {
        write("QCS: object not found here.\n");
        return 0;
    }
    if(interactive(ob) && ob != player) {
        write("QCS: cannot delete players.\n");
        return 0;
    }
    write("QCS: deleted " + (string)ob->query_short() + "\n");
    ob->remove();
    if(ob) destruct(ob);
    return 1;
}

int qcs_clone_file(object player, string name) {
    object ob;
    string path;

    if(!name || !sizeof(name)) {
        write("Syntax: clone <filename>\n");
        return 0;
    }
    path = qcs_resolve_realm_file(player, name);
    if(!path) {
        if(file_size(name) > 0) path = name;
        else if(file_size(name + ".c") > 0) path = name;
        else {
            write("QCS: file not found.\n");
            return 0;
        }
    }
    ob = clone_object(path);
    if(!ob) {
        write("QCS: clone failed for " + path + "\n");
        return 0;
    }
    ob->move(player);
    write("QCS: cloned " + path + " to your inventory.\n");
    return 1;
}

int qcs_about(object player, string target, int use_more) {
    string path;
    string body;
    string *lines;

    if(!target || !sizeof(target) || lower_case(target) == "here")
        path = (string)"/cmds/creator/_qcs"->qcs_room_path(player);
    else {
        path = qcs_resolve_realm_file(player, target);
        if(!path && file_size(target) > 0) path = target;
        else if(!path && file_size(target + ".c") > 0) path = target;
    }
    if(path && strsrch(path, ".c") == -1 && file_size(path + ".c") > 0)
        path = path + ".c";
    if(!path || file_size(path) <= 0) {
        write("QCS: file not found.\n");
        return 0;
    }
    body = read_file(path);
    if(!body || !sizeof(body)) {
        write("QCS: empty file.\n");
        return 0;
    }
    if(use_more) {
        lines = explode(body, "\n");
        player->more(lines);
    } else
        write(body);
    return 1;
}
