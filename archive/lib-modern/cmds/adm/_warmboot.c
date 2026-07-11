// /cmds/adm/_warmboot.c
// Bulk file reload for admin wizards (no driver restart).

#include <std.h>
#include <dirs.h>

inherit DAEMON;

protected string *collect_c_files(string dir, int depth);
protected int skip_reload(string file);
protected int reload_one(string file, int verbose);
protected int *reload_dir(string dir, int verbose, int loaded_only);
protected void do_warmboot_quick();
protected void do_warmboot_loaded();
protected void do_warmboot_std();
protected void do_warmboot_domain(string domain);
protected void do_warmboot_all();

int cmd_warmboot(string str) {
    if(!archp(this_player())) {
        notify_fail("You don't have access to warmboot.\n");
        return 0;
    }
    if(!str || str == "") {
        do_warmboot_quick();
        return 1;
    }
    str = lower_case(str);
    if(str == "all") {
        write("Reload ALL domains, daemons, cmds, std, and obj files?\n"
              "Connected players are NOT reloaded. Confirm (yes/no): ");
        input_to("confirm_all");
        return 1;
    }
    if(str == "loaded") {
        do_warmboot_loaded();
        return 1;
    }
    if(str == "std") {
        write("Reload every /std/ blueprint? Connected players keep old\n"
              "inherited code until relog. Confirm (yes/no): ");
        input_to("confirm_std");
        return 1;
    }
    do_warmboot_domain(str);
    return 1;
}

void confirm_all(string answer) {
    if(!answer || lower_case(answer) != "yes") {
        write("Warmboot aborted.\n");
        return;
    }
    do_warmboot_all();
}

void confirm_std(string answer) {
    if(!answer || lower_case(answer) != "yes") {
        write("Warmboot aborted.\n");
        return;
    }
    do_warmboot_std();
}

protected int skip_reload(string file) {
    if(!file || file == "") return 1;
    if(file == "/secure/daemon/master") return 1;
    if(strlen(file) > 18 && file[0..17] == "/secure/save/users") return 1;
    return 0;
}

protected void do_warmboot_quick() {
    int reloaded;
    int errors;
    int *counts;
    string *dirs;
    int i;

    reloaded = 0;
    errors = 0;
    write("Warmboot: reloading daemons, cmds, and obj...\n");

    dirs = ({
        "/daemon", "/secure/daemon",
        "/cmds", "/secure/cmds",
        "/obj"
    });
    for(i = 0; i < sizeof(dirs); i++) {
        counts = reload_dir(dirs[i], 0, 0);
        reloaded += counts[0];
        errors += counts[1];
    }

    write("Warmboot complete. " + reloaded + " file" +
          (reloaded == 1 ? "" : "s") + " reloaded" +
          (errors ? ", " + errors + " error" + (errors == 1 ? "" : "s") + "." : ".") + "\n");
    write("See: warmboot loaded | warmboot std | warmboot all | warmboot <domain>\n");
}

protected void do_warmboot_loaded() {
    object *obs;
    mapping seen;
    string bn;
    int reloaded;
    int errors;
    int i;

    reloaded = 0;
    errors = 0;
    seen = ([]);
    obs = objects();
    write("Warmboot loaded: recompiling unique loaded programs...\n");

    for(i = 0; i < sizeof(obs); i++) {
        if(!obs[i] || userp(obs[i])) continue;
        bn = base_name(obs[i]);
        if(skip_reload(bn) || seen[bn]) continue;
        seen[bn] = 1;
        if(reload_one(bn, 0)) reloaded++;
        else errors++;
    }

    write("Warmboot loaded complete. " + reloaded + " file" +
          (reloaded == 1 ? "" : "s") + " reloaded" +
          (errors ? ", " + errors + " error" + (errors == 1 ? "" : "s") + "." : ".") + "\n");
}

protected void do_warmboot_std() {
    int *counts;

    write("Warmboot std: reloading /std/ blueprints...\n");
    counts = reload_dir("/std", 1, 0);
    write("Warmboot std complete. " + counts[0] + " file" +
          (counts[0] == 1 ? "" : "s") + " reloaded" +
          (counts[1] ? ", " + counts[1] + " error" + (counts[1] == 1 ? "" : "s") + "." : ".") + "\n");
    write("Note: online players keep old behavior until relog.\n"
          "Use ./mud.sh stop && ./mud.sh start for full std/ reload.\n");
}

protected void do_warmboot_domain(string domain) {
    string dir;
    int *counts;

    dir = "/domains/" + capitalize(domain);
    if(file_size(dir) != -2) {
        dir = "/domains/" + domain;
        if(file_size(dir) != -2) {
            write("Domain '" + domain + "' not found under /domains/.\n");
            return;
        }
    }

    write("Warmboot: reloading domain " + domain + "...\n");
    counts = reload_dir(dir, 1, 0);
    write("Domain " + domain + " complete. " + counts[0] + " file" +
          (counts[0] == 1 ? "" : "s") + " reloaded" +
          (counts[1] ? ", " + counts[1] + " error" + (counts[1] == 1 ? "" : "s") + "." : ".") + "\n");
}

protected void do_warmboot_all() {
    string *domains;
    string *dirs;
    string ddir;
    int reloaded;
    int errors;
    int *counts;
    int i;

    reloaded = 0;
    errors = 0;
    write("Warmboot all: beginning full reload...\n");

    domains = get_dir("/domains/");
    if(!domains) domains = ({});
    for(i = 0; i < sizeof(domains); i++) {
        ddir = "/domains/" + domains[i];
        if(file_size(ddir) != -2) continue;
        write("  Domain: " + domains[i] + "\n");
        counts = reload_dir(ddir, 1, 0);
        reloaded += counts[0];
        errors += counts[1];
    }

    dirs = ({
        "/daemon", "/secure/daemon",
        "/cmds", "/secure/cmds",
        "/std", "/obj"
    });
    for(i = 0; i < sizeof(dirs); i++) {
        write("  Tree: " + dirs[i] + "\n");
        counts = reload_dir(dirs[i], 1, 0);
        reloaded += counts[0];
        errors += counts[1];
    }

    write("Full warmboot complete. " + reloaded + " file" +
          (reloaded == 1 ? "" : "s") + " reloaded" +
          (errors ? ", " + errors + " error" + (errors == 1 ? "" : "s") + "." : ".") + "\n");
    write("Player bodies and master.c were not touched. Driver restart\n"
          "is still required for simul-efun or master.c changes.\n");
}

protected int *reload_dir(string dir, int verbose, int loaded_only) {
    string *files;
    int reloaded;
    int errors;
    int i;

    reloaded = 0;
    errors = 0;
    if(file_size(dir) != -2) return ({ 0, 0 });

    files = collect_c_files(dir, 0);
    for(i = 0; i < sizeof(files); i++) {
        if(skip_reload(files[i])) continue;
        if(loaded_only && !find_object(files[i])) continue;
        if(reload_one(files[i], verbose)) reloaded++;
        else errors++;
    }
    return ({ reloaded, errors });
}

protected string *collect_c_files(string dir, int depth) {
    string *entries;
    string *result;
    int i;
    string entry;
    string fullpath;

    if(depth > 8) return ({});
    entries = get_dir(dir + "/");
    if(!entries) return ({});
    result = ({});
    for(i = 0; i < sizeof(entries); i++) {
        entry = entries[i];
        fullpath = dir + "/" + entry;
        if(file_size(fullpath) == -2) {
            result += collect_c_files(fullpath, depth + 1);
        } else if(strlen(entry) > 2 &&
                  entry[strlen(entry)-2..strlen(entry)-1] == ".c") {
            result += ({ fullpath[0..strlen(fullpath)-3] });
        }
    }
    return result;
}

protected int reload_one(string file, int verbose) {
    object ob;
    string err;

    if(skip_reload(file)) return 0;
    ob = find_object(file);
    if(ob && ob == master()) return 0;
    if(ob) catch(destruct(ob));
    if(file_size(file + ".c") < 0) return 0;
    err = catch(call_other(file, "???"));
    if(err) {
        write("  ERROR: " + file + ": " + err + "\n");
        return 0;
    }
    if(verbose) write("  OK: " + file + "\n");
    return 1;
}

void help() {
    write(
        "Syntax: warmboot\n"
        "        warmboot loaded\n"
        "        warmboot std\n"
        "        warmboot <domain>\n"
        "        warmboot all\n\n"
        "  warmboot           Reload all .c under /daemon/, /secure/daemon/,\n"
        "                     /cmds/, /secure/cmds/, and /obj/.\n"
        "  warmboot loaded    Reload every unique non-player program currently\n"
        "                     loaded in memory (rooms, NPC clones, etc.).\n"
        "  warmboot std       Reload all /std/ blueprints (confirm).\n"
        "  warmboot <domain>  Reload all files under /domains/<domain>/.\n"
        "  warmboot all       Full reload: all domains + daemon/cmd/std/obj.\n\n"
        "Does not reload player save files or disconnect anyone.\n"
        "Driver restart (mud.sh stop/start) still required for master.c\n"
        "and simul-efun changes, and is the only way to refresh online\n"
        "player inherited code after /std/ edits.\n\n"
        "See also: update\n"
    );
}
