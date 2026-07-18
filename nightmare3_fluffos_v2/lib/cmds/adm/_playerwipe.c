// /cmds/adm/_playerwipe.c
// Wipes player save files, postal data, and realm directories.
// Restricted to archp() or wiz_role == admin.

#include <std.h>

inherit DAEMON;

private string save_path(string name) {
    return "/secure/save/users/" + name[0..0] + "/" + name + ".o";
}

private string save_bak_path(string name) {
    return "/secure/save/users/" + name[0..0] + "/" + name + ".o.bak";
}

private string postal_dir(string name) {
    return "/secure/save/postal/" + name[0..0] + "/" + name + "/";
}

private string realm_dir(string name) {
    return "/realms/" + name + "/";
}

private int is_groups_cfg_member(string name) {
    string data;
    string *lines;
    string line;
    string rest;
    int i;
    int pos;

    if(!name || !strlen(name)) return 0;
    name = lower_case(name);
    data = read_file("/secure/cfg/groups.cfg");
    if(!data || !strlen(data)) return 0;
    lines = explode(data, "\n");
    for(i = 0; i < sizeof(lines); i++) {
        line = lines[i];
        if(!line || line[0] != '(') continue;
        pos = strsrch(line, ")");
        if(pos < 0) continue;
        rest = lower_case(line[pos + 1..]);
        if(strsrch(rest, name) != -1) return 1;
    }
    return 0;
}

private int get_saved_level(string name) {
    string data;
    string *lines;
    int i;
    int lv;

    data = read_file(save_path(name));
    if(!data) return -1;
    lines = explode(data, "\n");
    for(i = 0; i < sizeof(lines); i++) {
        if(sscanf(lines[i], "level %d;", lv) == 1) return lv;
        if(sscanf(lines[i], "__level %d;", lv) == 1) return lv;
    }
    return 0;
}

private void rm_tree(string dir) {
    string *files;
    string full;
    int i;

    if(!dir || !strlen(dir)) return;
    if(file_size(dir) != -2) return;
    files = get_dir(dir);
    if(!files) return;
    for(i = 0; i < sizeof(files); i++) {
        if(files[i] == "." || files[i] == "..") continue;
        full = dir + files[i];
        if(file_size(full) == -2)
            rm_tree(full + "/");
        else
            rm(full);
    }
    rmdir(dir);
}

private void wipe_one(string name) {
    string path;
    string bak;
    string postal;
    string realm;

    path   = save_path(name);
    bak    = save_bak_path(name);
    postal = postal_dir(name);
    realm  = realm_dir(name);

    if(file_size(path) >= 0) rm(path);
    if(file_size(bak)  >= 0) rm(bak);
    if(file_size(postal) == -2) rm_tree(postal);
    if(file_size(realm)  == -2) rm_tree(realm);

    log_file("adm/playerwipes",
        ctime(time()) + " " + (string)this_player()->query_name() +
        " wiped " + name + "\n");
}

int cmd_playerwipe(string str) {
    string name;
    string *letters;
    string *names;
    string dir;
    string fname;
    int wiped;
    int lv;
    int i;
    int j;

    if(!archp(this_player()) &&
       (string)this_player()->getenv("wiz_role") != "admin") {
        write("Access denied.\n");
        return 1;
    }

    if(!str || !strlen(str)) {
        write("Syntax: playerwipe <name>\n"
              "        playerwipe confirm all\n");
        return 1;
    }

    if(str == "confirm all") {
        wiped = 0;
        letters = get_dir("/secure/save/users/");
        if(!letters) {
            write("No player save directory found.\n");
            return 1;
        }
        for(i = 0; i < sizeof(letters); i++) {
            if(letters[i] == "." || letters[i] == "..") continue;
            dir = "/secure/save/users/" + letters[i] + "/";
            if(file_size(dir) != -2) continue;
            names = get_dir(dir);
            if(!names) continue;
            for(j = 0; j < sizeof(names); j++) {
                fname = names[j];
                if(!fname || strlen(fname) < 3) continue;
                if(fname[strlen(fname)-2..] != ".o") continue;
                name = fname[0..strlen(fname)-3];
                if(!strlen(name)) continue;
                if(is_groups_cfg_member(name)) {
                    write("Skipped " + capitalize(name) + " (groups.cfg member).\n");
                    continue;
                }
                lv = get_saved_level(name);
                if(lv < 0) continue;
                if(lv >= 20) {
                    write("Skipped " + capitalize(name) + " (wizard).\n");
                    continue;
                }
                wipe_one(name);
                wiped++;
            }
        }
        write(wiped + " player" + (wiped == 1 ? "" : "s") + " wiped.\n");
        return 1;
    }

    name = lower_case(str);
    if(is_groups_cfg_member(name)) {
        write("Cannot wipe a groups.cfg admin account.\n");
        return 1;
    }
    if(file_size(save_path(name)) < 0) {
        write("No save file found for '" + capitalize(name) + "'.\n");
        return 1;
    }
    lv = get_saved_level(name);
    if(lv >= 20) {
        write("Cannot wipe a wizard account.\n");
        return 1;
    }
    wipe_one(name);
    write(capitalize(name) + " has been wiped.\n");
    return 1;
}

void help() {
    write(
        "Syntax: playerwipe <name>\n"
        "        playerwipe confirm all\n\n"
        "Wipes a player's save file and associated data (postal, realm).\n\n"
        "  playerwipe <name>       -- Wipe one player. Refuses if level >= 20.\n"
        "  playerwipe confirm all  -- Wipe all non-wizard players.\n\n"
        "All wipes are logged to /log/adm/playerwipes.\n"
        "Restricted to archp() or wiz_role == admin.\n\n"
        "See also: setrole\n"
    );
}
