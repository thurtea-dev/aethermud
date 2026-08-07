/* /domains/adm/wiz_tools/wiz_reference_book.c
   Role-specific reference book spawned in wizard workrooms.
   Set property "book_role" to: admin, coding, rp, domain, or apprentice.
   Type: read book
   Full handbooks: help <role> *wizard staff
*/

#include <std.h>

inherit OBJECT;

private string book_title(string role);
private string *book_lines(string role);

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

private string book_title(string role) {
    if(!role || role == "") role = "admin";
    switch(role) {
    case "apprentice": return "AetherMUD Apprentice Wizard (quick ref)";
    case "coding":      return "AetherMUD Coding Wizard (quick ref)";
    case "rp":          return "AetherMUD Roleplay Wizard (quick ref)";
    case "domain":      return "AetherMUD Domain Wizard (quick ref)";
    default:            return "AetherMUD Admin (quick ref)";
    }
}

private string *book_lines(string role) {
    if(!role || role == "") role = "admin";

    if(role == "apprentice") {
        return ({
            "position: apprentice   wiz_role: apprentice",
            "Tag on who: [Apprentice]",
            "Tool: apprentice kit (type: kit)",
            "",
            "1. kit -> choose track (rp / domain / coding)",
            "2. Draft the track task and submit to domain",
            "3. Domain reviews; RP/coding may go to coding queue",
            "4. When marked ready, admin runs setrole",
            "",
            "You have ed/cat/ls. No role staff until setrole.",
            "Full: help apprentice *wizard staff / help kit",
            "Type workroom to return here."
        });
    }

    if(role == "coding") {
        return ({
            "position: creator   wiz_role: coding",
            "Tool required: staff of creation, held in inventory",
            "",
            "QCS direct commands (must be in /realms/<you>/):",
            "    create room <dir> <file>",
            "    create npc|weapon|armor <file>",
            "    modify here <prop> <val>",
            "    add exit <dir> <path>",
            "    add <npcfile> to here",
            "    delete <object>",
            "    about|more <file|here>",
            "",
            "Older alternate form, still works: build <verb> ... (also qcs <verb> ... for admins). Prefer the direct verbs above.",
            "",
            "Files: /realms/<you>/area/{room,npc,weap,armor}/",
            "Manual edit: ed, cat, ls (ambassador commands)",
            "No standalone update or eval on command path.",
            "",
            "Full handbook: help coding *wizard staff",
            "Type workroom to return here."
        });
    }

    if(role == "rp") {
        return ({
            "position: rp_wiz   wiz_role: rp",
            "Tools: RP skill tool (tool/rptool), tattoo gun (inscribe)",
            "",
            "RP SKILL TOOL",
            "    1. Select player",
            "    2. View skills",
            "    3. Grant skill",
            "    4. Remove skill",
            "    5. Grant language",
            "    6. Remove language",
            "    7. View request queue",
            "",
            "TATTOO GUN (Atlantean / Tattooed Man)",
            "    Menu (inscribe): 1. Select player  2. View  3. Grant  4. Remove",
            "    Direct: tattoo <player> list | available | <location> <tattoo name>",
            "",
            "MAILBOX (in workroom)",
            "    read mailbox, take request, approve slip, deny slip",
            "    Players use: requestskill <text>",
            "",
            "No build, update, or promote commands.",
            "",
            "Full handbook: help roleplay *wizard staff",
            "Type workroom to return here."
        });
    }

    if(role == "domain") {
        return ({
            "position: domain_wiz   wiz_role: domain",
            "Tool required: staff of dominion (domain)",
            "",
            "DOMAIN MENU",
            "    1. Create domain dir",
            "    2. Assign to coder",
            "    3. List domains",
            "    4. Set description",
            "    5. Goto start room",
            "    6. Grant access (logged)",
            "    7. Revoke access",
            "    8. List players",
            "    9. Reset loaded rooms",
            "",
            "World areas: /domains/<name>/rooms, monsters, obj",
            "Staff realms: /realms/<wiz>/area/ (for QCS sandboxes)",
            "Grant/revoke may need admin for full access.c update.",
            "",
            "No promote/demote or QCS without creation staff.",
            "",
            "Full handbook: help domain *wizard staff",
            "Type workroom to return here."
        });
    }

    /* admin / default */
    return ({
        "position: arch   wiz_role: admin",
        "Tools: all five staff objects, or the ring of dominion (admin only)",
        "",
        "PROMOTE (Staff of Demotion, option 1)",
        "    arch, domain_wiz, creator, rp_wiz",
        "DEMOTE (option 2) clears tools and role tag",
        "Also: setrole <player> <admin|domain|coding|rp>",
        "Mortals: makewiz <player> first (creates Apprentice).",
        "",
        "Admin commands: update, eval, warmboot, rid, goto, etc.",
        "QCS works via the Staff of Creation or the ring of dominion. Use the direct verbs (create, modify, add, delete, about, more) or qcs <verb> ...",
        "",
        "Player saves: /secure/save/users/<letter>/<name>.o",
        "warmboot does NOT reload std/user.c or living.c",
        "Full reboot: ./mud.sh stop && ./mud.sh start",
        "",
        "Full handbook: help admin *wizard staff",
        "Type workroom to return here."
    });
}

int cmd_read_book(string str) {
    string role;
    if(!str || strsrch(lower_case(str), "book") == -1 &&
               strsrch(lower_case(str), "manual") == -1) return 0;
    role = (string)query_property("book_role");
    /* Not write(): see admin_cheat_sheet.c for why -- write() routes
       through message("my_action", ...), which std/user.c's
       receive_message() would word-wrap a second time at the
       player's SCREEN width, breaking this already-framed card. */
    message("Nmy_action",
        (string)"/domains/adm/wiz_tools/card_format_d"->render_card(
            book_title(role), book_lines(role)) + "\n",
        this_player());
    return 1;
}
