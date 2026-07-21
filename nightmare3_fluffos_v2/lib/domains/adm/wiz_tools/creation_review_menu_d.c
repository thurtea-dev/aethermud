// /domains/adm/wiz_tools/creation_review_menu_d.c
// Shared Staff of Creation "review" (apprentice coding queue) menu logic.
// Extracted from staff_of_creation.c so both the physical staff and
// ring_of_dominion.c can drive the same review menu without duplicating
// it. Never cloned -- reached only via -> as an auto-loaded singleton,
// like /cmds/creator/_qcs.c.
//
// pending_action/pending_id are per-player mappings, not bare statics:
// this daemon can be entered by more than one coding wizard at once, so
// in-progress menu state must be keyed by the acting player.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private mapping pending_action;
private mapping pending_id;

void show_review_menu();
void handle_review_choice(string str);
void get_review_id(string id);

void create() {
    ::create();
    pending_action = ([]);
    pending_id = ([]);
}

int begin_menu(object player) {
    if(!admin_wizp(player) && !coding_wizp(player)) {
        write("Only coding wizards can review the apprenticeship queue.\n");
        return 1;
    }
    if(!has_wiz_tool(player, "staff_of_creation")) {
        write("You need the staff of creation.\n");
        return 1;
    }
    pending_action[player] = "";
    pending_id[player] = "";
    show_review_menu();
    input_to("handle_review_choice");
    return 1;
}

void show_review_menu() {
    write("\n=== Coding Review (Apprentice Queue) ===");
    write(" 1. List coding queue");
    write(" 2. Read submission");
    write(" 3. Mark implemented (marks apprentice ready for setrole)");
    write(" 0. Exit");
    write("Choice: ");
}

void handle_review_choice(string str) {
    object player;
    string *ids;
    mapping sub;
    int i;
    int choice;

    player = this_player();
    if(!str || !sizeof(str)) {
        write("Invalid choice.\n");
        show_review_menu();
        input_to("handle_review_choice");
        return;
    }
    choice = to_int(str);
    switch(choice) {
    case 0:
        write("Review closed.\n");
        return;
    case 1:
        ids = (string *)APPRENTICE_D->list_coding_queue();
        if(!ids || !sizeof(ids))
            write("Coding queue empty.\n");
        else {
            write("=== Coding queue ===\n");
            for(i = 0; i < sizeof(ids); i++) {
                sub = (mapping)APPRENTICE_D->get_submission(ids[i]);
                if(!sub) continue;
                write(ids[i] + " [" + sub["track"] + "] " +
                      capitalize(sub["author"]) + " - " + sub["title"] + "\n");
            }
        }
        show_review_menu();
        input_to("handle_review_choice");
        return;
    case 2:
        pending_action[player] = "read";
        write("Submission id: ");
        input_to("get_review_id");
        return;
    case 3:
        pending_action[player] = "done";
        write("Submission id: ");
        input_to("get_review_id");
        return;
    default:
        write("Invalid option.\n");
        show_review_menu();
        input_to("handle_review_choice");
    }
}

void get_review_id(string id) {
    object player;
    mapping sub;
    string who;

    player = this_player();
    if(!id || !sizeof(id)) {
        write("Cancelled.\n");
        show_review_menu();
        input_to("handle_review_choice");
        return;
    }
    id = replace_string(id, " ", "");
    who = (string)player->query_name();
    if(pending_action[player] == "read") {
        sub = (mapping)APPRENTICE_D->get_submission(id);
        if(!sub)
            write("No such submission.\n");
        else
            write((string)APPRENTICE_D->format_submission(sub));
    } else if(pending_action[player] == "done") {
        if((int)APPRENTICE_D->coding_mark_done(id, who))
            write("Marked implemented. Apprentice is ready for setrole.\n");
        else
            write("Could not mark done (wrong id or not in coding_queue).\n");
    }
    show_review_menu();
    input_to("handle_review_choice");
}
