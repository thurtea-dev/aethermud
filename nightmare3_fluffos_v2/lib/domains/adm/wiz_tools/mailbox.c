// /domains/adm/wiz_tools/mailbox.c
// Wizard skill-request mailbox. Clone in wizard workrooms.
// Reads pending requests from /secure/save/skill_requests/

#include <std.h>
#include <daemons.h>

inherit "/std/storage";

private string *list_request_files();
private int count_requests();

void create() {
    ::create();
    set_name("mailbox");
    set_id( ({ "mailbox", "wizard mailbox", "request mailbox" }) );
    set_short("a wizard's request mailbox");
    set_long(
        "A solid box mounted to the wall. A small indicator light shows\n"
        "the number of pending skill requests waiting inside.\n"
        "Type 'read mailbox' to list them, 'take request' to pull the oldest.\n"
        "Type 'look in mailbox' for the welcome letter.\n"
    );
    set_mass(500);
    set_value(0);
    set_property("no_drop", 1);
    set_property("no_get", 1);
    set_max_encumbrance(5000);
}

void reset() {
    object letter;

    ::reset();
    letter = present("welcome letter", this_object());
    if(!letter)
        letter = present("welcome mail", this_object());
    if(!letter) {
        letter = clone_object("/domains/adm/wiz_tools/mailbox_welcome");
        if(letter) letter->move(this_object());
    }
}

private string *list_request_files() {
    string *files;
    string *result;
    int i;

    files = get_dir("/secure/save/skill_requests/");
    if(!files) return ({});
    result = ({});
    for(i = 0; i < sizeof(files); i++) {
        if(sizeof(files[i]) > 4 &&
           files[i][sizeof(files[i])-4..] == ".txt")
            result += ({ files[i] });
    }
    return result;
}

private int count_requests() {
    return sizeof(list_request_files());
}

void init() {
    ::init();
    if(!creatorp(this_player())) return;
    add_action("cmd_read_mailbox", "read");
    add_action("cmd_take_request", "take");
    add_action("cmd_empty_mailbox", "empty");
}

int cmd_read_mailbox(string str) {
    string *files;
    string content;
    string *lines;
    string pname_val, text_val, time_val;
    string line;
    int i, j, age_mins;

    if(!str || strsrch(lower_case(str), "mailbox") == -1) return 0;
    if(!creatorp(this_player())) {
        write("You cannot use the mailbox.\n");
        return 1;
    }

    files = list_request_files();
    if(!sizeof(files)) {
        write("No pending skill requests.\n"
              "Type 'look in mailbox' for the welcome letter.\n");
        return 1;
    }

    write("=== Pending Skill Requests (" + sizeof(files) + ") ===\n");
    for(i = 0; i < sizeof(files); i++) {
        content = read_file("/secure/save/skill_requests/" + files[i]);
        if(!content) continue;
        pname_val = "";
        text_val = "";
        time_val = "";
        lines = explode(content, "\n");
        for(j = 0; j < sizeof(lines); j++) {
            line = lines[j];
            if(!strlen(line)) continue;
            if(line[0..6] == "player:") pname_val = line[7..];
            if(line[0..4] == "text:")  text_val  = line[5..];
            if(line[0..4] == "time:")  time_val  = line[5..];
        }
        age_mins = (time() - to_int(time_val)) / 60;
        write(sprintf("  %d. %-16s  \"%s\"  (%d min ago)\n",
            i + 1, capitalize(pname_val), text_val, age_mins));
    }
    write("Use 'take request' to pull the oldest one.\n");
    return 1;
}

int cmd_take_request(string str) {
    string *files;
    string fname;
    string content;
    string *lines;
    string pname_val, text_val, time_val;
    string line;
    object slip;
    int j;

    if(!str || lower_case(str) != "request") return 0;
    if(!creatorp(this_player())) {
        write("You cannot use the mailbox.\n");
        return 1;
    }

    files = list_request_files();
    if(!sizeof(files)) {
        write("The mailbox is empty.\n");
        return 1;
    }

    fname = "/secure/save/skill_requests/" + files[0];
    content = read_file(fname);
    if(!content) {
        write("Error reading request file.\n");
        return 1;
    }

    pname_val = "";
    text_val = "";
    time_val = "";
    lines = explode(content, "\n");
    for(j = 0; j < sizeof(lines); j++) {
        line = lines[j];
        if(!strlen(line)) continue;
        if(line[0..6] == "player:") pname_val = line[7..];
        if(line[0..4] == "text:")  text_val  = line[5..];
        if(line[0..4] == "time:")  time_val  = line[5..];
    }

    slip = clone_object("/domains/adm/wiz_tools/skill_slip");
    if(!slip) {
        write("Error creating request slip.\n");
        return 1;
    }
    slip->set_property("req_player", pname_val);
    slip->set_property("req_text",   text_val);
    slip->set_property("req_time",   to_int(time_val));
    slip->move(this_player());

    rm(fname);
    write("You take a skill request slip from the mailbox.\n");
    tell_room(environment(this_player()),
        (string)this_player()->query_cap_name() +
        " takes a slip from the mailbox.\n",
        ({ this_player() }));
    return 1;
}

int cmd_empty_mailbox(string str) {
    string *files;
    string role;
    int i;

    if(!str || lower_case(str) != "mailbox") return 0;
    role = (string)this_player()->getenv("wiz_role");
    if(role != "admin" && !archp(this_player())) {
        write("Only admins can empty the mailbox.\n");
        return 1;
    }

    files = list_request_files();
    if(!sizeof(files)) {
        write("The mailbox is already empty.\n");
        return 1;
    }
    for(i = 0; i < sizeof(files); i++)
        rm("/secure/save/skill_requests/" + files[i]);
    write("Mailbox emptied. " + sizeof(files) + " request(s) cleared.\n");
    return 1;
}

string query_short() {
    int n;
    n = count_requests();
    if(n > 0)
        return "a wizard's request mailbox [" + n + " pending]";
    return "a wizard's request mailbox";
}
