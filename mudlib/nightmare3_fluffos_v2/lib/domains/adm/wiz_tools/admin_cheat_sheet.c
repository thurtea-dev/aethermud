/* /domains/adm/wiz_tools/admin_cheat_sheet.c
   Single-card index of admin-only (arch/head arch) commands: the Ring of
   Dominion's ten delegated verbs plus the direct cmds/adm and secure/
   cmds/creator staples. One line each: syntax + one-clause purpose, not
   full help-file detail (see help admin / help <command> for that).
   Spawned in the same staff supplies chest as the role handbooks.
   Type: read sheet
*/

#include <std.h>

inherit OBJECT;

private string sheet_content();

void create() {
    ::create();
    set_name("admin cheat sheet");
    set_id( ({ "sheet", "cheat sheet", "admin cheat sheet",
               "command sheet", "admin sheet" }) );
    set_short("an admin command cheat sheet");
    set_long(
        "A single card listing every admin-specific command: syntax and a\n"
        "one-line purpose, nothing more. Type 'read sheet' to see it.\n"
    );
    set_mass(10);
    set_value(0);
}

void init() {
    ::init();
    add_action("cmd_read_sheet", "read");
}

private string sheet_content() {
    string *lines;

    lines = ({
        "Admin-only (arch / head arch). Syntax + purpose, one line each.",
        "Full detail: help admin, or help <command>",
        "",
        "RING OF DOMINION",
        "(worn ring; same functions as the five staff/tool objects)",
        "",
        "domain",
        "    Dominion menu: create/list domains, set desc, reset rooms",
        "promote / demote",
        "    Demotion menu: set position/role, clears tools on demote",
        "tool / rptool",
        "    Skill menu: grant/remove a player's skill or language percentage",
        "inscribe",
        "    Tattoo menu: grant/remove an Atlantean/Tattooed Man tattoo",
        "review",
        "    Opens the apprentice coding review queue",
        "build <verb> ...",
        "    QCS building shortcut (create/modify/add/delete/about/more)",
        "clone <path>",
        "    Clone a file into your inventory",
        "purge <id>",
        "    Destruct a non-living object in the room",
        "askring <question>",
        "    Ask the ring for a pointer on any verb above",
        "",
        "DIRECT ADMIN COMMANDS",
        "",
        "setrole <player> <admin|domain|coding|rp>",
        "    Set wizard position and synced role tag",
        "sever <player> <limb>",
        "    Narrative limb sever (also: restore, list)",
        "fixdemote <player>",
        "    Repair a stale offline demotion",
        "playerwipe <name>",
        "    Wipe a player's save, postal, and realm data",
        "warmboot [loaded|std|<domain>|all]",
        "    Reload daemon/command files (never std/ for players already connected)",
        "update [file]",
        "    Recompile one file",
        "eval <lpc commands>",
        "    Run raw LPC",
        "rid <name>",
        "    Force-disconnect a player",
        "goto <player|room_path>",
        "    Teleport to a player or an absolute room path",
        "",
        "Full reboot for std/ changes (shell, not in-game):",
        "    ./mud.sh stop && ./mud.sh start",
        "",
        "Full command index: help admin"
    });
    return (string)"/domains/adm/wiz_tools/card_format_d"->render_card(
        "AetherMUD Admin Command Cheat Sheet", lines) + "\n";
}

int cmd_read_sheet(string str) {
    if(!str || (strsrch(lower_case(str), "sheet") == -1 &&
                strsrch(lower_case(str), "cheat") == -1)) return 0;
    /* Not write(): write() is a simul_efun that routes through
       message("my_action", ...), and std/user.c's receive_message()
       word-wraps any non-"N"-prefixed class to the player's SCREEN
       width (default 75) -- which would re-wrap this already-framed
       80-column card and break its borders. The "N" prefix (see
       cmds/creator/_stat.c and _ac.c for the same idiom on other
       column-aligned output) skips that second wrap pass. */
    message("Nmy_action", sheet_content(), this_player());
    return 1;
}
