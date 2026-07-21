/* /domains/adm/wiz_tools/ring_of_dominion.c
   A plain gold ring that stands in for the full admin wiz-tool set.
   While worn by an admin (admin_wizp), it grants the same functions as
   the five tools /secure/daemon/wiztools.c gives a head arch/arch --
   staff_of_demotion, staff_of_dominion, staff_of_creation, rp_skill_tool,
   and tattoo_gun -- directly. It never clones any tool object into
   inventory; the wearer only needs the ring. domain/promote/demote/
   tool/rptool/inscribe/review delegate into the same shared, never-
   cloned menu daemons the physical staff/tool/gun items delegate into
   (dominion_menu_d.c, demotion_menu_d.c, rp_skill_menu_d.c,
   tattoo_menu_d.c, creation_review_menu_d.c), so there is exactly one
   copy of each menu's logic regardless of whether it is reached by
   staff, tool, gun, or ring. build/clone/purge call QCS directly
   (/cmds/creator/_qcs.c), gated on admin_wizp rather than physically
   carrying a staff of creation -- the ring does not try to impersonate
   one for has_wiz_tool()'s sake.

   Every granted verb below checks worn_by_me() before doing anything:
   add_action() bindings persist for as long as the ring sits in
   inventory, worn or not, so removing the ring (which does not move the
   object out of inventory) cannot un-bind them the way un-carrying an
   item would. The per-call check is what actually makes "remove the
   ring and the functionality stops immediately" true. The ten shared
   verbs (domain/promote/demote/tool/rptool/inscribe/review/build/clone/
   purge) return 0 rather than printing a refusal when not worn, so a
   physically carried staff/tool/gun bound to the same verb still works
   even while an unworn ring sits in the same inventory. askring is the
   one verb unique to the ring, so it alone claims the command and
   refuses with a message when not worn.

   query_auto_load()/init_arg() restore the ring to worn state
   automatically on reconnect -- the driver's autoload contract only
   restores the object into inventory, not its worn state, so without
   this the ring would come back unworn (or not at all, if
   query_auto_load() were missing entirely, which was the previous bug:
   this file never overrode it, so the ring never made it into
   __AutoLoad and simply did not survive a reconnect).

   No_drop/no_give: this is a one-off personal item, not something
   meant to circulate. */

#include <std.h>

inherit ARMOUR;

int on_ring_worn();
int on_ring_removed();
int cmd_askring(string str);
private string ring_answer(string question);
private int worn_by_me();
private int clone_to_wizard(object ob);

void create() {
    ::create();
    set_name("gold ring");
    set_id( ({ "ring", "gold ring", "plain gold ring", "ring of dominion" }) );
    set_short("a plain gold ring");
    set_long(
        "A plain, unmarked band of gold. There is nothing about it that\n"
        "looks unusual, until it is worn.\n"
    );
    set_type("ring");
    set_property("rifts_slot", "admin_ring");
    set_limbs( ({ "right hand" }) );
    set_ac(0);
    set_mass(1);
    set_value(0);
    set_property("no_drop", 1);
    set_property("no_give", 1);
    set_wear( (: on_ring_worn :) );
    set_remove( (: on_ring_removed :) );
}

mixed *query_auto_load() {
    return ({ base_name(this_object()), ({}) });
}

/* Runs once the restored ring is already back in the player's inventory
   (autosave.c moves it in before calling init_arg()). Re-wearing here,
   rather than leaving it sitting unworn, is the actual fix for "admin
   shouldn't have to manually re-wear after every reconnect." */
void init_arg(mixed *arg) {
    wear("ring");
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_askring",  "askring");
    add_action("cmd_domain",   "domain");
    add_action("cmd_demote",   "demote");
    add_action("cmd_demote",   "promote");
    add_action("cmd_tool",     "tool");
    add_action("cmd_tool",     "rptool");
    add_action("cmd_inscribe", "inscribe");
    add_action("cmd_review",   "review");
    add_action("cmd_build",    "build");
    add_action("cmd_clone_ob", "clone");
    add_action("cmd_purge_ob", "purge");
}

/* True only while this exact ring is worn by the player running the
   current command. See the file header for why every handler needs
   this rather than a one-time check at wear/init time. */
private int worn_by_me() {
    return query_worn() == this_player();
}

int on_ring_worn() {
    object player;

    player = this_player();
    if(!admin_wizp(player)) {
        write("The ring stays cold and inert on your hand.\n");
        return 0;
    }
    write("You slide the gold ring onto your finger.\n");
    write("The ring grants: domain, promote, demote, tool, rptool,\n"
          "inscribe, review, build, clone, and purge, exactly as if you\n"
          "held all five staff. Ask it about any of them any time:\n"
          "askring <question>.\n");
    return 1;
}

int on_ring_removed() {
    object player;

    player = query_worn();
    if(!player) player = this_player();
    tell_object(player, "The ring cools, and its powers fade from your hand.\n");
    return 1;
}

// ── domain / demote+promote / tool+rptool / inscribe / review ───────────────
// Each delegates into the same shared menu daemon the physical staff/
// tool/gun items use. The daemon re-checks its own role gate
// (admin_wizp/domain_wizp/rp_wizp as appropriate) via begin_menu()'s
// player argument, so nothing further needs checking here beyond
// "is this ring actually worn by whoever just typed the verb."

int cmd_domain(string str) {
    if(!worn_by_me()) return 0;
    return (int)"/domains/adm/wiz_tools/dominion_menu_d"->begin_menu(this_player());
}

int cmd_demote(string str) {
    if(!worn_by_me()) return 0;
    return (int)"/domains/adm/wiz_tools/demotion_menu_d"->begin_menu(this_player());
}

int cmd_tool(string str) {
    if(!worn_by_me()) return 0;
    return (int)"/domains/adm/wiz_tools/rp_skill_menu_d"->begin_menu(this_player());
}

int cmd_inscribe(string str) {
    if(!worn_by_me()) return 0;
    return (int)"/domains/adm/wiz_tools/tattoo_menu_d"->begin_menu(this_player());
}

int cmd_review(string str) {
    if(!worn_by_me()) return 0;
    return (int)"/domains/adm/wiz_tools/creation_review_menu_d"->begin_menu(this_player());
}

// ── build / clone / purge ────────────────────────────────────────────────────
// Same QCS entry points tanstaafl_base.c uses, but gated on admin_wizp()
// directly instead of coding_tool_ok()/has_wiz_tool() -- the ring is not
// pretending to be a carried staff of creation for inventory-scan
// purposes, it grants the function directly while worn.

int cmd_build(string str) {
    string verb;
    string rest;

    if(!worn_by_me()) return 0;
    if(!admin_wizp(this_player())) return 0;
    if(!str || !sizeof(str))
        return (int)"/cmds/creator/_qcs"->qcs_dispatch("", "");
    verb = "";
    rest = "";
    if(sscanf(str, "%s %s", verb, rest) == 2)
        return (int)"/cmds/creator/_qcs"->qcs_dispatch(verb, rest);
    return (int)"/cmds/creator/_qcs"->qcs_dispatch(str, "");
}

/* Place a fresh clone with the wizard. move() returns MOVE_OK (0) on
   success and a nonzero error code on failure. Living clones (NPCs) go
   to the room: an NPC inside a wizard's inventory is invisible to
   look/present and unusable. Returns 1 = in inventory, 2 = in the room,
   0 = could not be placed. Mirrors tanstaafl_base.c's clone_to_wizard(). */
private int clone_to_wizard(object ob) {
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

int cmd_clone_ob(string str) {
    object ob;
    string path;
    string err;
    int placed;

    if(!worn_by_me()) return 0;
    if(!admin_wizp(this_player())) return 0;
    if(!str || !sizeof(str)) {
        notify_fail("Clone what?\n");
        return 0;
    }
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
        write("Cloned into " +
            (string)environment(this_player())->query_short() + ": " +
            (string)ob->query_short() + "\n");
    else {
        write("Clone succeeded but could not be placed; destructing it.\n");
        destruct(ob);
    }
    return 1;
}

int cmd_purge_ob(string str) {
    object ob;

    if(!worn_by_me()) return 0;
    if(!admin_wizp(this_player())) return 0;
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

// ── askring ───────────────────────────────────────────────────────────────

int cmd_askring(string str) {
    if(!worn_by_me()) {
        write("The ring is silent. It only answers while worn.\n");
        return 1;
    }
    if(!str || !sizeof(str)) {
        write("Ask it what? Try: askring <question>\n");
        return 1;
    }
    write(ring_answer(lower_case(str)));
    return 1;
}

private string ring_answer(string question) {
    if(strsrch(question, "promote") != -1 || strsrch(question, "demote") != -1)
        return "promote/demote handle this directly (promote/demote/setlevel/\n"
               "setrace/setocc/setstats/title/goto/summon/boot, plus daemon\n"
               "reload and shutdown). Requires admin_wizp.\n";
    if(strsrch(question, "build") != -1 || strsrch(question, "qcs") != -1 ||
       strsrch(question, "clone") != -1 || strsrch(question, "purge") != -1)
        return "build <verb> ... handles QCS building, clone <path> clones a\n"
               "file into your inventory, purge <id> removes a non-living\n"
               "object from the room. review opens the apprentice coding\n"
               "queue.\n";
    if(strsrch(question, "domain") != -1)
        return "domain opens the dominion menu: create or list domains, set\n"
               "a description, reset loaded rooms, and review pending\n"
               "RP/domain apprentice submissions.\n";
    if(strsrch(question, "tattoo") != -1 || strsrch(question, "inscribe") != -1)
        return "inscribe opens the tattoo menu: select a player and grant or\n"
               "remove an Atlantean/Tattooed Man tattoo.\n";
    if(strsrch(question, "skill") != -1 || strsrch(question, "language") != -1 ||
       strsrch(question, "rptool") != -1)
        return "tool or rptool opens the skill menu: grant or remove a\n"
               "player's skill or language percentage, or view the skill\n"
               "request queue.\n";
    if(strsrch(question, "reboot") != -1 || strsrch(question, "warmboot") != -1)
        return "Warmboot reloads daemon and command files, but never\n"
               "std/user.c, living.c, room.c, armour.c, or combat.c for\n"
               "players already connected. For those, stop and start the\n"
               "driver from the shell: ./mud.sh stop && ./mud.sh start\n";
    if(strsrch(question, "save") != -1)
        return "Player saves live at /secure/save/users/<first letter>/\n"
               "<name>.o, never at /save/players/.\n";
    return "I don't have an answer for that. Try asking about: promote,\n"
           "demote, build, clone, purge, domain, tattoo, skill, warmboot,\n"
           "or save.\n";
}
