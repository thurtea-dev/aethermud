/* /domains/adm/wiz_tools/staff_of_creation.c
   Staff of Creation - primary QCS building tool for coding wizards.
   Inherits tanstaafl_base which provides build/clone/purge commands.
   Also: review - apprentice coding queue. */

#include <std.h>
#include <daemons.h>

inherit "/domains/adm/wiz_tools/tanstaafl_base";

static private string pending_action;
static private string pending_id;

void show_review_menu();
void handle_review_choice(string str);
void get_review_id(string id);

void create() {
    ::create();
    set_name("staff of creation");
    set_id(({ "staff", "staff of creation", "creation staff", "tanstaafl" }));
    set_short("a staff of creation");
    set_long(
        "A luminous white staff humming with creative potential. It allows\n"
        "its wielder to bring new rooms, monsters, and objects into being.\n"
        "For Coding Wizard use only. Type: build help\n"
        "Apprentice coding queue: review");
    set_mass(500);
    set_value(0);
    set_property("no_drop", 1);
    set_property("no_give", 1);
    set_invis(1);
}

mixed *query_auto_load() {
    return ({ base_name(this_object()), ({}) });
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_review", "review");
}

int cmd_review(string str) {
    if(!admin_wizp(this_player()) && !coding_wizp(this_player())) {
        write("Only coding wizards can review the apprenticeship queue.\n");
        return 1;
    }
    if(!has_wiz_tool(this_player(), "staff_of_creation")) {
        write("You need the staff of creation.\n");
        return 1;
    }
    pending_action = "";
    pending_id = "";
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
    string *ids;
    mapping sub;
    int i;
    int choice;

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
        pending_action = "read";
        write("Submission id: ");
        input_to("get_review_id");
        return;
    case 3:
        pending_action = "done";
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
    mapping sub;
    string who;

    if(!id || !sizeof(id)) {
        write("Cancelled.\n");
        show_review_menu();
        input_to("handle_review_choice");
        return;
    }
    id = replace_string(id, " ", "");
    who = (string)this_player()->query_name();
    if(pending_action == "read") {
        sub = (mapping)APPRENTICE_D->get_submission(id);
        if(!sub)
            write("No such submission.\n");
        else
            write((string)APPRENTICE_D->format_submission(sub));
    } else if(pending_action == "done") {
        if((int)APPRENTICE_D->coding_mark_done(id, who))
            write("Marked implemented. Apprentice is ready for setrole.\n");
        else
            write("Could not mark done (wrong id or not in coding_queue).\n");
    }
    show_review_menu();
    input_to("handle_review_choice");
}
