/* /cmds/creator/_qcs_npc.c - QCS NPC create/modify logic */

#include <std.h>

inherit OBJECT;

#define TPL_NPC "/std/obj/templates/qcs_npc.c"

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

protected int qcs_patch_stats_line(string path, string key, string value) {
    string body;
    string search;
    string line;
    string new_line;
    int idx;

    body = read_file(path);
    if(!body || !sizeof(body)) return 0;
    search = "set_stats(\"" + key + "\",";
    idx = strsrch(body, search);
    if(idx == -1) return 0;
    line = body[idx..];
    if(strsrch(line, "\n") != -1)
        line = line[0..strsrch(line, "\n")];
    new_line = " set_stats(\"" + key + "\", " + value + ");";
    body = replace_string(body, line, new_line);
    return (int)"/cmds/creator/_qcs"->qcs_write_update(path, body);
}

int qcs_modify_npc(string path, string prop, string value) {
    string body;
    string search;
    string line;
    string new_line;
    int idx;

    if(prop == "occ") {
        body = read_file(path);
        if(!body || !sizeof(body)) {
            write("QCS: cannot read " + path + "\n");
            return 0;
        }
        search = "setenv(\"rifts_occ\"";
        idx = strsrch(body, search);
        if(idx == -1) {
            write("QCS: no rifts_occ line in " + path + "\n");
            return 0;
        }
        line = body[idx..];
        if(strsrch(line, "\n") != -1)
            line = line[0..strsrch(line, "\n")];
        new_line = " setenv(\"rifts_occ\", \"" +
            (string)"/cmds/creator/_qcs"->qcs_safe_str(value) + "\");";
        body = replace_string(body, line, new_line);
        return (int)"/cmds/creator/_qcs"->qcs_write_update(path, body);
    }
    if(prop == "align")
        return (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
            path, "alignment", value);
    if(prop == "mdc") {
        if(!(int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(
                path, "mdc_creature", (to_int(value) > 0 ? "1" : "0")))
            return 0;
        qcs_patch_stats_line(path, "MDC", value);
        qcs_patch_stats_line(path, "max_MDC", value);
        return 1;
    }
    return (int)"/cmds/creator/_qcs_room"->qcs_patch_set_string(path, prop, value);
}

int qcs_create_npc(object player, string fname) {
    string base;
    string safe;
    string path;
    string body;
    string header;
    string cap;
    mapping vars;

    base = qcs_realm_base(player);
    safe = qcs_safe_name(fname);
    cap = capitalize(safe);
    path = base + "/npc/" + safe + ".c";
    if(file_size(path) > 0) {
        write("QCS: file already exists: " + path + "\n");
        return 0;
    }
    vars = ([
        "QCS_NAME"  : cap,
        "QCS_ID"    : safe,
        "QCS_SHORT" : "a " + safe,
        "QCS_LONG"  : "A newly created NPC awaiting description."
    ]);
    body = qcs_read_template(TPL_NPC, vars);
    header = "/* " + path + " */\n"
        "/* QCS NPC by " + (string)player->query_name() + " */\n\n";
    body = header + body;
    if(!qcs_write_update(path, body)) return 0;
    write("QCS: NPC written to " + path + "\n");
    return 1;
}
