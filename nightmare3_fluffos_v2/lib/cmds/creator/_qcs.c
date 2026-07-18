/* /cmds/creator/_qcs.c
   AetherMUD Quick Creation System - shared utilities and dispatcher.
   Coding wizards only. Files live under /realms/<wizname>/area/ */

#include <std.h>
#include <security.h>

inherit DAEMON;

#define REALM_AREA    "/area"
#define TPL_ROOM      "/std/obj/templates/qcs_room.c"
#define TPL_NPC       "/std/obj/templates/qcs_npc.c"
#define TPL_WEAPON    "/std/obj/templates/qcs_weapon.c"
#define TPL_ARMOR     "/std/obj/templates/qcs_armor.c"

string qcs_realm_base(object player);
int qcs_ensure_realm_dirs(string pname);
int qcs_in_realm(object player);
string qcs_room_path(object player);
string qcs_safe_name(string s);
string qcs_safe_str(string s);
string qcs_reverse_dir(string dir);
int qcs_write_update(string path, string body);
string qcs_resolve_realm_file(object player, string name);
int qcs_dispatch(string verb, string args);
int qcs_require_tool(object player);

string qcs_realm_base(object player) {
    return "/realms/" + lower_case((string)player->query_name()) + REALM_AREA;
}

int qcs_ensure_realm_dirs(string pname) {
    string base;
    string *subdirs;
    int i;

    if(!pname || !sizeof(pname)) return 0;
    pname = lower_case(pname);
    base = "/realms/" + pname;
    if(file_size(base) != -2) mkdir(base);
    subdirs = ({
        base + "/area",
        base + "/area/room",
        base + "/area/npc",
        base + "/area/weap",
        base + "/area/armor"
    });
    for(i = 0; i < sizeof(subdirs); i++) {
        if(file_size(subdirs[i]) != -2) mkdir(subdirs[i]);
    }
    return 1;
}

int qcs_in_realm(object player) {
    object env;
    string path;
    string rest;
    string *parts;

    env = environment(player);
    if(!env) return 0;
    path = file_name(env);
    if(sscanf(path, "/realms/%s", rest) != 1) return 0;
    parts = explode(rest, "/");
    if(!sizeof(parts)) return 0;
    if(lower_case(parts[0]) != lower_case((string)player->query_name())) return 0;
    return 1;
}

string qcs_room_path(object player) {
    object env;
    string path;

    env = environment(player);
    if(!env) return 0;
    path = file_name(env);
    if(strsrch(path, "#") != -1)
        path = path[0..strsrch(path, "#") - 1];
    return path;
}

string qcs_safe_name(string s) {
    string out;
    int i;
    int c;

    if(!s || !sizeof(s)) return "object";
    out = "";
    for(i = 0; i < sizeof(s); i++) {
        c = s[i];
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '_')
            out += s[i..i];
        else if(c == ' ' || c == '-')
            out += "_";
    }
    if(!sizeof(out)) out = "object";
    return lower_case(out);
}

string qcs_safe_str(string s) {
    if(!s) return "";
    return replace_string(replace_string(s, "\\", "\\\\"), "\"", "\\\"");
}

string qcs_reverse_dir(string dir) {
    switch(lower_case(dir)) {
    case "north":     return "south";
    case "south":     return "north";
    case "east":      return "west";
    case "west":      return "east";
    case "northeast": return "southwest";
    case "northwest": return "southeast";
    case "southeast": return "northwest";
    case "southwest": return "northeast";
    case "up":        return "down";
    case "down":      return "up";
    case "in":        return "out";
    case "out":       return "in";
    default:          return "south";
    }
}

int qcs_write_update(string path, string body) {
    string dir;
    string *parts;
    int i;

    if(!path || !sizeof(path)) return 0;
    if(!body || !sizeof(body)) return 0;
    parts = explode(path, "/");
    dir = "/";
    for(i = 1; i < sizeof(parts) - 1; i++) {
        dir += parts[i];
        if(file_size(dir) != -2) mkdir(dir);
        dir += "/";
    }
    if(!write_file(path, body)) {
        write("QCS: write_file failed for " + path + "\n");
        return 0;
    }
    catch(call_other(path, "??"));
    write("QCS: wrote and updated " + path + "\n");
    return 1;
}

string qcs_resolve_realm_file(object player, string name) {
    string base;
    string safe;
    string path;

    base = qcs_realm_base(player);
    safe = qcs_safe_name(name);
    path = base + "/room/" + safe + ".c";
    if(file_size(path) > 0) return path;
    path = base + "/npc/" + safe + ".c";
    if(file_size(path) > 0) return path;
    path = base + "/weap/" + safe + ".c";
    if(file_size(path) > 0) return path;
    path = base + "/armor/" + safe + ".c";
    if(file_size(path) > 0) return path;
    return 0;
}

int qcs_require_tool(object player) {
    if(!admin_wizp(player) && !coding_wizp(player)) {
        write("QCS is for coding wizards only.\n");
        return 0;
    }
    if(!has_wiz_tool(player, "staff_of_creation")) {
        write("You need the staff of creation to use QCS.\n");
        return 0;
    }
    return 1;
}

int qcs_require_realm(object player) {
    if(!qcs_require_tool(player)) return 0;
    if(!qcs_in_realm(player)) {
        write("QCS: you must be in your realm under /realms/" +
            lower_case((string)player->query_name()) + "/\n");
        return 0;
    }
    return 1;
}

void qcs_help() {
    write(
        "AetherMUD Quick Creation System (QCS)\n"
        "Files: /realms/<you>/area/{room,npc,weap,armor}/\n\n"
        " create room <dir> <filename>\n"
        " create npc <filename>\n"
        " create weapon <filename>\n"
        " create armor <filename>\n"
        " modify here <property> <value>\n"
        " modify <filename> <property> <value>\n"
        " add exit <direction> <path>\n"
        " add <npcfile> to here\n"
        " delete <object>\n"
        " clone <filename>\n"
        " about <filename|here>\n"
        " more <filename|here>\n\n"
        "Room properties: short, long, light, indoors\n"
        "NPC properties: name, short, long, level, hp, race, occ, align, mdc\n"
        "Weapon properties: name, short, long, wc, type, damage (NdN), mdc (0/1)\n"
        "Armor properties: name, short, long, ac, ar, sdc, mdc, slot\n"
        "Note: files auto-reload after each modify.\n");
}

int qcs_dispatch(string verb, string args) {
    string sub;
    string rest;
    string a;
    string b;
    string c;

    if(!qcs_require_tool(this_player())) return 1;
    if(!verb || !sizeof(verb)) {
        qcs_help();
        return 1;
    }
    verb = lower_case(verb);
    if(!args) args = "";

    if(verb == "create") {
        if(sscanf(args, "%s %s %s", sub, a, b) == 3 && lower_case(sub) == "room")
            return (int)"/cmds/creator/_qcs_room"->qcs_create_room(this_player(), a, b);
        if(sscanf(args, "%s %s", sub, a) == 2) {
            if(lower_case(sub) == "npc")
                return (int)"/cmds/creator/_qcs_npc"->qcs_create_npc(this_player(), a);
            if(lower_case(sub) == "weapon")
                return (int)"/cmds/creator/_qcs_item"->qcs_create_weapon(this_player(), a);
            if(lower_case(sub) == "armor")
                return (int)"/cmds/creator/_qcs_item"->qcs_create_armor(this_player(), a);
        }
        write("Syntax: create room <dir> <file> | create npc|weapon|armor <file>\n");
        return 1;
    }
    if(verb == "modify") {
        if(sscanf(args, "here %s %s", a, b) == 2) {
            if(!qcs_require_realm(this_player())) return 1;
            return (int)"/cmds/creator/_qcs_room"->qcs_modify_file(
                this_player(), qcs_room_path(this_player()), a, b);
        }
        if(sscanf(args, "%s %s %s", sub, a, b) == 3)
            return (int)"/cmds/creator/_qcs_room"->qcs_modify_target(
                this_player(), sub, a, b);
        write("Syntax: modify here <property> <value>\n"
              "        modify <filename> <property> <value>\n");
        return 1;
    }
    if(verb == "add") {
        if(!qcs_require_realm(this_player())) return 1;
        if(sscanf(args, "exit %s %s", a, b) == 2)
            return (int)"/cmds/creator/_qcs_room"->qcs_patch_exit_file(
                qcs_room_path(this_player()) + ".c", a, b);
        if(sscanf(args, "%s to here", a) == 1)
            return (int)"/cmds/creator/_qcs_room"->qcs_add_to_room(this_player(), a);
        if(sscanf(args, "%s to %s", a, b) == 2)
            return (int)"/cmds/creator/_qcs_room"->qcs_add_to_room_file(this_player(), b, a);
        write("Syntax: add exit <dir> <path>  |  add <npcfile> to here\n");
        return 1;
    }
    if(verb == "delete") {
        if(!qcs_require_realm(this_player())) return 1;
        return (int)"/cmds/creator/_qcs_room"->qcs_delete_object(this_player(), args);
    }
    if(verb == "clone") {
        return (int)"/cmds/creator/_qcs_room"->qcs_clone_file(this_player(), args);
    }
    if(verb == "about") {
        return (int)"/cmds/creator/_qcs_room"->qcs_about(this_player(), args, 0);
    }
    if(verb == "more") {
        return (int)"/cmds/creator/_qcs_room"->qcs_about(this_player(), args, 1);
    }
    if(verb == "help") {
        qcs_help();
        return 1;
    }
    write("Unknown QCS command. Type: qcs\n");
    return 1;
}

int cmd_qcs(string str) {
    string verb;
    string rest;

    if(!creatorp(this_player())) return 0;
    if(!qcs_require_tool(this_player())) return 1;
    if(!str || !sizeof(str)) {
        qcs_help();
        return 1;
    }
    if(sscanf(str, "%s %s", verb, rest) == 2)
        return qcs_dispatch(verb, rest);
    return qcs_dispatch(str, "");
}

void help() {
    qcs_help();
}
