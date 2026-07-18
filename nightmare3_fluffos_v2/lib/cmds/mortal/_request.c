// /cmds/mortal/_request.c
// "request title <title>" - ask an RP-Wizard to grant an RP prefix title.
// Requires level 10+.  Notifies online creators and logs to /log/title_requests.

#include <std.h>
#include <dirs.h>

inherit DAEMON;

#define TITLE_LOG "title_requests"

static int valid_title_char(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ';
}

int cmd_request(string str) {
    string title;
    string pname;
    string logline;
    object *online;
    object ob;
    int i;
    int notified;

    if(!str || str == "") {
        write("Usage: request title <title>\n");
        return 1;
    }
    if(!sscanf(str, "title %s", title)) {
        write("Usage: request title <title>\n");
        return 1;
    }
    if((int)this_player()->query_level() < 10) {
        write("You must be at least level 10 to request a title.\n");
        return 1;
    }
    title = capitalize(title);
    if(strlen(title) < 2 || strlen(title) > 20) {
        write("Title must be between 2 and 20 characters.\n");
        return 1;
    }
    for(i = 0; i < strlen(title); i++) {
        if(!valid_title_char(title[i])) {
            write("Title may only contain letters and spaces.\n");
            return 1;
        }
    }

    pname = (string)this_player()->query_name();
    logline = ctime(time()) + " " + pname + " requested title '" + title + "'\n";
    log_file(TITLE_LOG, logline);

    notified = 0;
    online = users();
    for(i = 0; i < sizeof(online); i++) {
        ob = online[i];
        if(!ob || !creatorp(ob)) continue;
        tell_object(ob,
            "%^YELLOW%^[Title Request] " + capitalize(pname) +
            " has requested the title '" + title + "'.%^RESET%^\n"
            "Use: grant title " + pname + " " + title + "\n");
        notified++;
    }

    if(notified > 0)
        write("Your title request '" + title + "' has been sent to " +
              notified + " online staff member" +
              (notified == 1 ? "" : "s") + ".\n");
    else
        write("No staff are online right now.  Your request has been logged "
              "and will be reviewed.\n");
    return 1;
}

void help() {
    write(
        "Syntax: request title <title>\n\n"
        "Submits a request to an RP-Wizard to grant you a title prefix.\n"
        "Requires level 10 or higher.\n\n"
        "  - Title must be letters and spaces only, 2-20 characters.\n"
        "  - Example: request title Lord\n"
        "  - Example: request title Lady\n\n"
        "An RP-Wizard will review your request and may approve it with:\n"
        "  grant title <yourname> <title>\n\n"
        "Once granted, your name appears as 'Lord Thurtea' to players\n"
        "who have been introduced to you.\n"
    );
}
