/* /domains/adm/wiz_tools/staff_board.c
   Shared staff bulletin board. Clone one into any staff-only room.

   Two jobs in one object:

   1. A normal bulletin board (inherits BBOARD): post / read / followup /
      remove / edit. Every clone uses board id "staff", so all of them are
      views onto the SAME thread pool -- a note posted in the lounge is
      visible from the admin chamber and from a workroom. That sharing is
      the whole point; do not give a second clone its own board id unless
      you actually want a separate, disconnected board.

   2. A live queue panel appended to the description, so staff see what is
      waiting on them without having to remember to run each list command.
      The panel is computed fresh on every look; nothing is cached and
      nothing is stored on the board.

   Queue sources, all read-only:
     - ACCOUNT_D->list_pending_accounts()  (accounts awaiting approval)
     - /secure/save/skill_requests/*.txt   (pending skill requests)
     - APPRENTICE_D->list_domain_queue() / list_coding_queue()

   Skill requests are counted from the FILES on disk, not from
   SKILL_REQUEST_D->get_all_requests(). The daemon keeps requests in a
   plain in-memory mapping with no save/restore, so it reports zero after
   every reboot while the files survive. mailbox.c reads the files for the
   same reason. Do not "simplify" this to the daemon call.

   Staff-only. Placement in a staff-only room is the primary control; the
   creatorp() gates below are defense in depth.
*/

#include <std.h>
#include <daemons.h>

inherit BBOARD;

/* Rule 17: FluffOS 2.9 needs prototypes for anything called above its
   definition. */
private string queue_panel();
private int count_skill_requests();
private string account_block();
private string skill_block();
private string apprentice_block();

void create() {
    ::create();
    set_name("board");
    set_id( ({ "board", "bulletin board", "staff board",
               "staff bulletin board", "noticeboard" }) );
    set_short("the staff bulletin board");
    set_long(
        "A broad slate board mounted at eye height, ruled into columns for\n"
        "staff notices. Pending requests are listed underneath.\n"
        "Type 'read <number>' to open a post, 'post <subject>' to write one,\n"
        "'followup <number>' to reply, 'remove <number>' to delete your own.\n"
    );
    set_board_id("staff");
    set_property("no steal", 1);
}

/* ── Access gates ──────────────────────────────────────────────────────────
   ::init() still runs for everyone so Object::init() and the ordinary
   object actions register normally; the command entry points below refuse
   non-staff instead. */

int cmd_post(string str) {
    if(!creatorp(this_player()))
        return notify_fail("Only staff may post to that board.\n");
    return ::cmd_post(str);
}

int cmd_read(string str) {
    if(!creatorp(this_player()))
        return notify_fail("The notices mean nothing to you.\n");
    return ::cmd_read(str);
}

int cmd_followup_and_respond(string str) {
    if(!creatorp(this_player()))
        return notify_fail("Only staff may reply to that board.\n");
    return ::cmd_followup_and_respond(str);
}

int cmd_remove(string str) {
    if(!creatorp(this_player()))
        return notify_fail("Only staff may remove posts from that board.\n");
    return ::cmd_remove(str);
}

int cmd_edit(string str) {
    if(!creatorp(this_player()))
        return notify_fail("Only staff may edit posts on that board.\n");
    return ::cmd_edit(str);
}

/* ── Queue panel ─────────────────────────────────────────────────────────── */

private int count_skill_requests() {
    string *files;
    int i, n;

    files = unguarded((: get_dir, "/secure/save/skill_requests/" :));
    if(!files) return 0;
    n = 0;
    for(i = 0; i < sizeof(files); i++) {
        if(sizeof(files[i]) > 4 && files[i][sizeof(files[i])-4..] == ".txt")
            n++;
    }
    return n;
}

private string account_block() {
    string out;
    string *accts;
    string em;
    int i;

    accts = (string *)ACCOUNT_D->list_pending_accounts();
    if(!accts) accts = ({});
    if(!sizeof(accts)) return "  Account requests   none pending\n";

    out = "  Account requests   " + sizeof(accts) + " pending\n";
    for(i = 0; i < sizeof(accts); i++) {
        em = (string)ACCOUNT_D->query_account_email(accts[i]);
        out += sprintf("      %-16s %s\n", capitalize(accts[i]),
            (em && sizeof(em)) ? em : "(no email given)");
    }
    out += "      Approve with: accountapproval approve <account>\n";
    return out;
}

private string skill_block() {
    int n;

    n = count_skill_requests();
    if(!n) return "  Skill requests     none pending\n";
    return "  Skill requests     " + n + " pending\n"
           "      Review with: read mailbox (in a wizard workroom)\n";
}

private string apprentice_block() {
    string out;
    mixed dq, cq;
    int nd, nc;

    dq = APPRENTICE_D->list_domain_queue();
    cq = APPRENTICE_D->list_coding_queue();
    nd = arrayp(dq) ? sizeof(dq) : 0;
    nc = arrayp(cq) ? sizeof(cq) : 0;

    if(!nd && !nc) return "  Apprentice work    nothing in review\n";
    out = "";
    if(nd) out += "  Apprentice domain  " + nd + " awaiting domain review\n";
    if(nc) out += "  Apprentice coding  " + nc + " in the coding queue\n";
    return out;
}

private string queue_panel() {
    return "\n"
           "--- Pending staff queues ---\n"
         + account_block()
         + skill_block()
         + apprentice_block();
}

varargs string query_long(string str) {
    if(!this_player() || !creatorp(this_player()))
        return "A slate board ruled into columns and covered in staff\n"
               "notices. Whatever the marks mean, they are not for you.\n";
    return ::query_long(str) + queue_panel();
}
