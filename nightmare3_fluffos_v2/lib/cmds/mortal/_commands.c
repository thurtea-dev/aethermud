/* /cmds/mortal/_commands.c
   List player commands in columns, grouped into the same categories
   used by the help daemon (daemon/help.c). */

#include <std.h>
#include <daemons.h>
#include <dirs.h>

inherit DAEMON;

#define CMD_COLUMNS 5

#define SEC_SKILLS   0
#define SEC_COMBAT   1
#define SEC_COMM     2
#define SEC_SYSTEMS  3
#define SEC_COMMANDS 4
#define SEC_STAFF    5

private int classify_command(string cmd);
private void print_command_section(string title, string *items);

int cmd_commands(string str) {
    string *cmds;
    string *skills_cmds, *combat_cmds, *comm_cmds, *systems_cmds;
    string *staff_cmds, *catchall_cmds;
    int i, sec;

    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
    }
    if(str && sizeof(str)) return 0;

    cmds = sort_array(distinct_array(
        (string *)CMD_D->query_commands(DIR_MORTAL_CMDS) +
        (string *)CMD_D->query_commands(DIR_SECURE_MORTAL_CMDS)),
        1);

    skills_cmds = ({});
    combat_cmds = ({});
    comm_cmds = ({});
    systems_cmds = ({});
    staff_cmds = ({});
    catchall_cmds = ({});

    for(i = 0; i < sizeof(cmds); i++) {
        sec = classify_command(cmds[i]);
        switch(sec) {
        case SEC_SKILLS:  skills_cmds   += ({ cmds[i] }); break;
        case SEC_COMBAT:  combat_cmds   += ({ cmds[i] }); break;
        case SEC_COMM:    comm_cmds     += ({ cmds[i] }); break;
        case SEC_SYSTEMS: systems_cmds  += ({ cmds[i] }); break;
        case SEC_STAFF:   staff_cmds    += ({ cmds[i] }); break;
        default:          catchall_cmds += ({ cmds[i] }); break;
        }
    }

    write("=== PLAYER COMMANDS ===");
    print_command_section("SKILLS & ABILITIES", skills_cmds);
    print_command_section("COMBAT & MOVEMENT", combat_cmds);
    print_command_section("COMMUNICATION & SOCIAL", comm_cmds);
    print_command_section("SYSTEMS", systems_cmds);
    print_command_section("COMMANDS", catchall_cmds);
    print_command_section("STAFF / WIZARD", staff_cmds);
    write("");
    write("Type help <command> for details on any command above.");
    return 1;
}

private void print_command_section(string title, string *items) {
    if(!items || !sizeof(items)) return;
    write("\n" + title);
    write(" ---------------------------------------------------------------");
    write(format_page(items, CMD_COLUMNS));
}

private int classify_command(string cmd) {
    switch(cmd) {
    case "abilities": case "armorofithan": case "assassination":
    case "bandage": case "bionics": case "breath": case "cast":
    case "empathy": case "improve": case "known": case "language":
    case "languages": case "magicnet": case "meditate": case "metamorph":
    case "mindblock": case "objectread": case "pilot": case "psi":
    case "psihelp": case "psionics": case "psisword": case "pskills":
    case "readaura": case "remember": case "remoteview": case "retrieve":
    case "senseevil": case "skillrequest": case "skills": case "sneak":
    case "spell": case "spells": case "sskills": case "store":
    case "tattoo": case "telekinesis": case "telemechanics": case "tongues":
    case "treat":
        return SEC_SKILLS;

    case "assassinate": case "assist": case "autododge": case "autoparry":
    case "combat": case "dodge": case "drive": case "eject": case "fire":
    case "flee": case "fly": case "follow": case "hide": case "kill":
    case "kills": case "lastkiller": case "parry": case "position":
    case "prowl": case "push": case "reload": case "rest":
    case "saving_throws": case "shoot": case "stance": case "stop":
    case "unload": case "wake": case "wanted":
        return SEC_COMBAT;

    case "card": case "converse": case "emote": case "face": case "forward":
    case "greet": case "harass": case "introduce": case "mail":
    case "mudparty": case "ooc": case "party": case "pemote":
    case "posting": case "praise": case "pray": case "radio": case "reply":
    case "say": case "shout": case "speak": case "tell": case "telepathy":
    case "touch": case "watchradio": case "whisper": case "yell":
        return SEC_COMM;

    case "advance": case "biography": case "body": case "brief":
    case "chfn": case "clan": case "cls": case "color": case "colorize":
    case "credits": case "customize": case "date": case "describe":
    case "drop": case "echo": case "eq": case "equipment": case "examine":
    case "exchange": case "exp": case "experience": case "faction":
    case "finger": case "get": case "give": case "guild": case "help":
    case "inventory": case "left": case "lineecho": case "lines":
    case "look": case "money": case "mudlist": case "news":
    case "nextreboot": case "passwd": case "password": case "peer":
    case "picture": case "prompt": case "quests": case "reputation":
    case "request": case "right": case "rift": case "roll": case "rwho":
    case "save": case "sbar": case "score": case "setenv":
    case "setrespawn": case "sirname": case "sites": case "snoopable":
    case "status": case "stats": case "suicide": case "take":
    case "testcolor": case "trade": case "users": case "version":
    case "where": case "who": case "whimpy": case "wimpy": case "wimpydir":
    case "worn": case "xp": case "zero":
        return SEC_SYSTEMS;

    case "chat": case "wiz": case "wizchat": case "workroom":
        return SEC_STAFF;
    }
    return SEC_COMMANDS;
}

void help() {
    write("Syntax: commands\n\n"
          "Lists available player commands in five columns, grouped by\n"
          "category.\n"
          "See also: help, syntax\n");
}
