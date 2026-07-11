/* /domains/adm/wiz_tools/wiz_reference_book.c
   Role-specific reference book spawned in wizard workrooms.
   Set property "book_role" to: admin, coding, rp, domain, or apprentice.
   Type: read book
   Full handbooks: help <role> *wizard staff
*/

#include <std.h>

inherit OBJECT;

private string book_content(string role);

void create() {
    ::create();
    set_name("reference book");
    set_id( ({ "book", "reference book", "wizard reference book",
               "wiz reference book", "manual" }) );
    set_short("a wizard reference manual");
    set_long(
        "A slim reference manual for wizard staff. "
        "Type 'read book' for a quick summary, or "
        "'help staff *wizard staff' for the full guide.\n"
    );
    set_mass(50);
    set_value(0);
}

void init() {
    ::init();
    add_action("cmd_read_book", "read");
}

private string book_content(string role) {
    if(!role || role == "") role = "admin";

    if(role == "apprentice") {
        return
        "=== AetherMUD Apprentice Wizard (quick ref) ===\n\n"
        "position: apprentice   wiz_role: apprentice\n"
        "Tag on who: [Apprentice]\n"
        "Tool: apprentice kit (type: kit)\n\n"
        "1. kit -> choose track (rp / domain / coding)\n"
        "2. Draft the track task and submit to domain\n"
        "3. Domain reviews; RP/coding may go to coding queue\n"
        "4. When marked ready, admin runs setrole\n\n"
        "You have ed/cat/ls. No role staff until setrole.\n"
        "Full: help apprentice *wizard staff / help kit\n"
        "Type workroom to return here.\n";
    }

    if(role == "coding") {
        return
        "=== AetherMUD Coding Wizard (quick ref) ===\n\n"
        "position: creator   wiz_role: coding\n"
        "Tool required: staff of creation, held in inventory\n\n"
        "QCS direct commands (must be in /realms/<you>/):\n"
        "  create room <dir> <file>\n"
        "  create npc|weapon|armor <file>\n"
        "  modify here <prop> <val>\n"
        "  add exit <dir> <path>\n"
        "  add <npcfile> to here\n"
        "  delete <object>\n"
        "  about|more <file|here>\n\n"
        "Older alternate form, still works: build <verb> ... (also qcs <verb> ...\n"
        "for admins). Prefer the direct verbs above.\n\n"
        "Files: /realms/<you>/area/{room,npc,weap,armor}/\n"
        "Manual edit: ed, cat, ls (ambassador commands)\n"
        "No standalone update or eval on command path.\n\n"
        "Full handbook: help coding *wizard staff\n"
        "Type workroom to return here.\n";
    }

    if(role == "rp") {
        return
        "=== AetherMUD Roleplay Wizard (quick ref) ===\n\n"
        "position: rp_wiz   wiz_role: rp\n"
        "Tools: RP skill tool (tool/rptool), tattoo gun (inscribe)\n\n"
        "RP SKILL TOOL\n"
        "  1. Select player  2. View skills  3. Grant skill\n"
        "  4. Remove skill   5. Grant language  6. Remove language\n"
        "  7. View request queue\n\n"
        "TATTOO GUN (Atlantean / Tattooed Man)\n"
        "  1. Select player  2. View  3. Grant  4. Remove\n\n"
        "MAILBOX (in workroom)\n"
        "  read mailbox, take request, approve slip, deny slip\n"
        "  Players use: requestskill <text>\n\n"
        "No build, update, or promote commands.\n\n"
        "Full handbook: help roleplay *wizard staff\n"
        "Type workroom to return here.\n";
    }

    if(role == "domain") {
        return
        "=== AetherMUD Domain Wizard (quick ref) ===\n\n"
        "position: domain_wiz   wiz_role: domain\n"
        "Tool required: staff of domains (domain)\n\n"
        "DOMAIN MENU\n"
        "  1. Create domain dir   2. Assign to coder\n"
        "  3. List domains        4. Set description\n"
        "  5. Goto start room     6. Grant access (logged)\n"
        "  7. Revoke access       8. List players\n"
        "  9. Reset loaded rooms\n\n"
        "World areas: /domains/<name>/rooms, monsters, obj\n"
        "Staff realms: /realms/<wiz>/area/ (for QCS sandboxes)\n"
        "Grant/revoke may need admin for full access.c update.\n\n"
        "No promote/demote or QCS without creation staff.\n\n"
        "Full handbook: help domain *wizard staff\n"
        "Type workroom to return here.\n";
    }

    /* admin / default */
    return
    "=== AetherMUD Admin (quick ref) ===\n\n"
    "position: arch   wiz_role: admin\n"
    "Tools: all five staff objects in inventory\n\n"
    "PROMOTE (Staff of Demotion, option 1)\n"
    "  arch, domain_wiz, creator, rp_wiz\n"
    "DEMOTE (option 2) clears tools and role tag\n"
    "Also: setrole <player> <admin|domain|coding|rp>\n"
    "Mortals: makewiz <player> first (creates Apprentice).\n\n"
    "Admin commands: update, eval, warmboot, rid, goto, etc.\n"
    "QCS still needs staff of creation in inventory. Use the direct verbs\n"
    "(create, modify, add, delete, about, more) or qcs <verb> ...\n\n"
    "Player saves: /secure/save/users/<letter>/<name>.o\n"
    "warmboot does NOT reload std/user.c or living.c\n"
    "Full reboot: ./mud.sh stop && ./mud.sh start\n\n"
    "Full handbook: help admin *wizard staff\n"
    "Type workroom to return here.\n";
}

int cmd_read_book(string str) {
    string role;
    if(!str || strsrch(lower_case(str), "book") == -1 &&
               strsrch(lower_case(str), "manual") == -1) return 0;
    role = (string)query_property("book_role");
    write(book_content(role));
    return 1;
}
