/* /cmds/mortal/_mail.c
   Player mail command for AetherMUD.
   Syntax:
     mail                  -- list messages
     mail read <N>         -- read message N
     mail send <player>    -- compose and send a message
     mail delete <N>       -- delete message N
     mail clear            -- delete all read messages */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private void show_mail_list(object player) {
    mixed *mbox;
    mapping letter;
    string from_str;
    string subj_str;
    string ts_str;
    string flag;
    int i;
    int total;

    mbox  = (mixed *)MAIL_D->query_all_mail((string)player->query_name());
    total = sizeof(mbox);
    if(total == 0) {
        write("You have no messages.\n");
        return;
    }
    write(sprintf("%-4s %-12s %-30s %s\n", "#", "From", "Subject", "Date"));
    write("---- ------------ ------------------------------ ----------\n");
    for(i = 0; i < total; i++) {
        letter   = (mapping)mbox[i];
        if(!mapp(letter)) continue;
        from_str = capitalize((string)letter["from"]);
        subj_str = (string)letter["subject"];
        if(strlen(subj_str) > 29) subj_str = subj_str[0..28] + "~";
        ts_str   = ctime((int)letter["timestamp"])[4..12];
        flag     = (int)letter["read"] ? " " : "N";
        write(sprintf("%s%-3d %-12s %-30s %s\n",
            flag, i + 1, from_str, subj_str, ts_str));
    }
    write(sprintf("%d message%s. N = unread.\n",
        total, total == 1 ? "" : "s"));
}

/* Compose callbacks -- called via input_to() during mail send. */

void get_mail_body(string line) {
    string to_name;
    string subject;
    string body;

    to_name = (string)this_player()->getenv("mail_compose_to");
    subject = (string)this_player()->getenv("mail_compose_subject");
    body    = (string)this_player()->getenv("mail_compose_body");
    if(!stringp(body)) body = "";

    if(line == ".") {
        if(!stringp(to_name) || !strlen(to_name)) {
            write("Mail cancelled: recipient lost.\n");
            this_player()->setenv("mail_compose_to",      0);
            this_player()->setenv("mail_compose_subject", 0);
            this_player()->setenv("mail_compose_body",    0);
            return;
        }
        (int)MAIL_D->send_mail(
            (string)this_player()->query_name(),
            to_name,
            subject,
            body
        );
        write("Message sent to " + capitalize(to_name) + ".\n");
        this_player()->setenv("mail_compose_to",      0);
        this_player()->setenv("mail_compose_subject", 0);
        this_player()->setenv("mail_compose_body",    0);
        return;
    }

    if(strlen(line) > 0) {
        if(strlen(body) > 0)
            body = body + "\n" + line;
        else
            body = line;
    }
    this_player()->setenv("mail_compose_body", body);
    input_to("get_mail_body", 0);
}

void get_mail_subject(string line) {
    if(!stringp(line) || !strlen(line)) {
        write("Subject cannot be blank. Mail cancelled.\n");
        this_player()->setenv("mail_compose_to", 0);
        return;
    }
    this_player()->setenv("mail_compose_subject", line);
    this_player()->setenv("mail_compose_body",    "");
    write("Message (type . on its own line to send, or 'cancel' to abort):\n");
    input_to("get_mail_body", 0);
}

int cmd_mail(string str) {
    string cmd;
    string arg;
    int sp;
    int n;
    int removed;
    mapping letter;
    string body;

    if(!str || str == "") {
        show_mail_list(this_player());
        return 1;
    }

    sp = strsrch(str, " ");
    if(sp == -1) {
        cmd = lower_case(str);
        arg = "";
    } else {
        cmd = lower_case(str[0..sp-1]);
        arg = str[sp+1..];
    }

    if(cmd == "read") {
        n = to_int(arg);
        if(n < 1) {
            write("Usage: mail read <number>\n");
            return 1;
        }
        letter = (mapping)MAIL_D->read_mail(
            (string)this_player()->query_name(), n);
        if(!mapp(letter)) {
            write("No message at that number.\n");
            return 1;
        }
        write("From:    " + capitalize((string)letter["from"]) + "\n");
        write("Subject: " + (string)letter["subject"] + "\n");
        write("Date:    " + ctime((int)letter["timestamp"]) + "\n");
        write("---\n");
        body = (string)letter["body"];
        if(stringp(body) && strlen(body) > 0)
            write(body + "\n");
        else
            write("(empty)\n");
        return 1;
    }

    if(cmd == "send") {
        if(!arg || !strlen(arg)) {
            write("Usage: mail send <player>\n");
            return 1;
        }
        arg = lower_case(arg);
        this_player()->setenv("mail_compose_to", arg);
        this_player()->setenv("mail_compose_body", "");
        write("To: " + capitalize(arg) + "\n");
        write("Subject: ");
        input_to("get_mail_subject", 0);
        return 1;
    }

    if(cmd == "delete") {
        n = to_int(arg);
        if(n < 1) {
            write("Usage: mail delete <number>\n");
            return 1;
        }
        if((int)MAIL_D->delete_mail((string)this_player()->query_name(), n)) {
            write("Message " + n + " deleted.\n");
        } else {
            write("No message at that number.\n");
        }
        return 1;
    }

    if(cmd == "clear") {
        removed = (int)MAIL_D->delete_read_mail(
            (string)this_player()->query_name());
        if(removed > 0)
            write(removed + " read message" + (removed == 1 ? "" : "s") +
                " cleared.\n");
        else
            write("No read messages to clear.\n");
        return 1;
    }

    write("Usage: mail [read <N> | send <player> | delete <N> | clear]\n");
    return 1;
}

void help() {
    write(
        "Syntax: mail\n"
        "        mail read <number>\n"
        "        mail send <player>\n"
        "        mail delete <number>\n"
        "        mail clear\n\n"
        "Player mail system. Stores messages until you delete them.\n\n"
        "  mail              -- list your messages\n"
        "  mail read 1       -- read message 1\n"
        "  mail send sal     -- compose a message to Sal\n"
        "  mail delete 2     -- delete message 2\n"
        "  mail clear        -- delete all messages you have already read\n\n"
        "During compose: type your message line by line.\n"
        "Type a single . on its own line to send.\n\n"
        "See also: tell, reply, ooc\n"
    );
}
