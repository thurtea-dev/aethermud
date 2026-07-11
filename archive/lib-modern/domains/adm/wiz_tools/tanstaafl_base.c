/* /domains/adm/wiz_tools/tanstaafl_base.c
   Inheritable mixin for QCS builder objects.
   Inherit this and include "tanstaafl" in your set_id() call
   to give the object build/clone/purge commands when carried by
   a coding wizard.

   Usage:
     inherit "/domains/adm/wiz_tools/tanstaafl_base";
     void create() {
         ::create();
         set_id(({ "your name", "tanstaafl" }));
     }

   The "build" command works exactly like the "qcs" command.
   "clone <path>" clones a file into inventory.
   "purge <id>" removes a non-living object from the room. */

#include <std.h>
#include <security.h>
#include <move.h>

inherit OBJECT;

void create() {
    ::create();
}

protected int coding_tool_ok(object player) {
    if(!player) return 0;
    if(!admin_wizp(player) && !coding_wizp(player)) {
        write("This tool only works for coding wizards.\n");
        return 0;
    }
    if(!has_wiz_tool(player, "staff_of_creation")) {
        write("You need the staff of creation to use this tool.\n");
        return 0;
    }
    return 1;
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_build",    "build");
    add_action("cmd_clone_ob", "clone");
    add_action("cmd_purge_ob", "purge");
}

/* Place a fresh clone with the wizard. move() returns MOVE_OK (0) on
   success and a nonzero error code on failure (see /secure/include/
   move.h). Living clones (NPCs) go to the room: an NPC inside a
   wizard's inventory is invisible to look/present and unusable.
   Returns 1 = in inventory, 2 = in the room, 0 = could not be placed. */
protected int clone_to_wizard(object ob) {
    object env;

    if(!ob) return 0;
    env = environment(this_player());
    if(living(ob)) {
        if(env && ob->move(env) == MOVE_OK) return 2;
        return 0;
    }
    if(ob->move(this_player()) == MOVE_OK) return 1;
    if(env && ob->move(env) == MOVE_OK) return 2;
    return 0;
}

int cmd_build(string str) {
    string verb;
    string rest;

    if(!coding_tool_ok(this_player())) return 1;
    if(!str || !sizeof(str)) {
        return (int)"/cmds/creator/_qcs"->qcs_dispatch("", "");
    }
    verb = "";
    rest = "";
    if(sscanf(str, "%s %s", verb, rest) == 2)
        return (int)"/cmds/creator/_qcs"->qcs_dispatch(verb, rest);
    return (int)"/cmds/creator/_qcs"->qcs_dispatch(str, "");
}

int cmd_clone_ob(string str) {
    object ob;
    string path;
    string err;
    int placed;

    if(!coding_tool_ok(this_player())) return 1;
    if(!str || !sizeof(str)) {
        notify_fail("Clone what?\n");
        return 0;
    }
    /* Resolve relative names against the wizard's cwd, then against
       the QCS realm area directories, so 'clone bandit.c' works. */
    path = absolute_path((string)this_player()->get_path(), str);
    if(file_size(path) <= 0 && file_size(path + ".c") <= 0)
        path = (string)"/cmds/creator/_qcs_room"->qcs_resolve_realm_file(
            this_player(), str);
    if(!path || (file_size(path) <= 0 && file_size(path + ".c") <= 0)) {
        write("Cannot clone (file not found): " + str + "\n");
        return 1;
    }
    err = catch(ob = clone_object(path));
    if(err || !ob) {
        write("Cannot clone: " + path + (err ? " " + err : "") + "\n");
        return 1;
    }
    placed = clone_to_wizard(ob);
    if(placed == 1)
        write("Cloned into your inventory: " +
            (string)ob->query_short() + "\n");
    else if(placed == 2)
        write("Cloned into the room: " + (string)ob->query_short() + "\n");
    else {
        write("Clone succeeded but could not be placed; destructing it.\n");
        destruct(ob);
    }
    return 1;
}

int cmd_purge_ob(string str) {
    object ob;

    if(!coding_tool_ok(this_player())) return 1;
    if(!str || !sizeof(str)) {
        notify_fail("Purge what?\n");
        return 0;
    }
    ob = present(str, environment(this_player()));
    if(!ob) {
        write("Not here: " + str + "\n");
        return 1;
    }
    if(living(ob)) {
        write("Cannot purge living objects.\n");
        return 1;
    }
    write("Purged: " + (string)ob->query_short() + "\n");
    ob->remove();
    destruct(ob);
    return 1;
}
