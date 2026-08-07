// /domains/adm/wiz_tools/mailbox_welcome.c
// Default welcome letter in wizard mailboxes.

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("welcome letter");
    set_id(({ "welcome letter", "letter", "mail", "welcome mail" }));
    set_short("a welcome letter");
    set_long(
        "AetherMUD Wizard Mailbox\n"
        "======================\n\n"
        "This mailbox collects player skill requests from the request\n"
        "command. Roleplay wizards review and approve them here.\n\n"
        "Commands:\n"
        "  read mailbox       list pending skill requests\n"
        "  take request       pull the oldest request as a slip\n"
        "  read slip          view slip details\n"
        "  approve slip       grant the requested skill (wizard)\n"
        "  deny slip          discard the request (wizard)\n"
        "  empty mailbox      clear all pending (admin only)\n\n"
        "Players submit requests with: request <skill description>\n"
        "Offline approvals are saved to /secure/save/skill_grants/.\n\n"
        "Keep this letter for reference or discard it.");
    set_mass(5);
    set_value(0);
    set_property("mailbox_welcome", 1);
}

int allow_get(object who) {
    if(!who || !creatorp(who)) return 0;
    return 1;
}

void init() {
    ::init();
    add_action("cmd_read_letter", "read");
}

int cmd_read_letter(string str) {
    if(!str || strsrch(lower_case(str), "letter") == -1) return 0;
    write(query_long());
    return 1;
}
