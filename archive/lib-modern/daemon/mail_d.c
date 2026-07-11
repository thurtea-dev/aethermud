/* /daemon/mail_d.c
   Player-to-player mail daemon for RiftsMUD.
   Persistent: save_object/restore_object on /daemon/save/mail_d.
   All mail is stored in __mailboxes: player_name -> ({ letter_mapping, ... }) */

#include <std.h>
#include <daemons.h>
#include <dirs.h>
#include <security.h>

inherit DAEMON;

private mapping __mailboxes;
private mapping __forwards;

#define SAVE_FILE (DIR_DAEMONS_SAVE + "/mail_d")

string query_forward(string pname);
int set_forward(string pname, string target);

void create() {
    daemon::create();
    set_no_clean(1);
    __mailboxes = ([]);
    __forwards = ([]);
    if(unguarded((: file_size, SAVE_FILE + ".o" :)) > 0)
        unguarded((: restore_object, SAVE_FILE :));
    if(!mapp(__mailboxes)) __mailboxes = ([]);
    if(!mapp(__forwards)) __forwards = ([]);
}

private void do_save() {
    unguarded((: save_object, SAVE_FILE :));
}

/* send_mail(from, to, subject, body)
   Stores a new letter in to's mailbox.  Notifies recipient if online. */
int send_mail(string from, string to, string subject, string body) {
    mapping letter;
    mixed *mbox;
    object recipient;
    string fwd;
    mapping fwd_letter;

    if(!stringp(from) || !stringp(to) || !stringp(subject)) return 0;
    if(!stringp(body)) body = "";
    to = lower_case(to);
    letter = ([
        "from"      : lower_case(from),
        "subject"   : subject,
        "body"      : body,
        "timestamp" : time(),
        "read"      : 0
    ]);
    mbox = __mailboxes[to];
    if(!mbox || !pointerp(mbox)) mbox = ({});
    mbox += ({ letter });
    __mailboxes[to] = mbox;
    do_save();
    recipient = find_living(to);
    if(recipient && objectp(recipient) && interactive(recipient))
        tell_object(recipient,
            "\n[Mail] New message from " + capitalize(from) + ": " +
            subject + "\n");
    fwd = query_forward(to);
    if(fwd && sizeof(fwd) && fwd != to) {
        fwd_letter = ([
            "from"      : lower_case(from),
            "subject"   : "[Fwd " + capitalize(to) + "] " + subject,
            "body"      : body,
            "timestamp" : time(),
            "read"      : 0
        ]);
        mbox = __mailboxes[fwd];
        if(!mbox || !pointerp(mbox)) mbox = ({});
        mbox += ({ fwd_letter });
        __mailboxes[fwd] = mbox;
        do_save();
        recipient = find_living(fwd);
        if(recipient && objectp(recipient) && interactive(recipient))
            tell_object(recipient,
                "\n[Mail] Forwarded copy for " + capitalize(to) +
                " from " + capitalize(from) + ": " + subject + "\n");
    }
    return 1;
}

/* query_all_mail(player_name) -- returns copy of player's mailbox array. */
mixed *query_all_mail(string pname) {
    mixed *mbox;
    if(!stringp(pname)) return ({});
    pname = lower_case(pname);
    mbox  = __mailboxes[pname];
    if(!mbox || !pointerp(mbox)) return ({});
    return mbox + ({});
}

/* query_mail_count(player_name) -- total message count. */
int query_mail_count(string pname) {
    mixed *mbox;
    if(!stringp(pname)) return 0;
    pname = lower_case(pname);
    mbox  = __mailboxes[pname];
    if(!mbox || !pointerp(mbox)) return 0;
    return sizeof(mbox);
}

/* query_unread_count(player_name) -- count of unread messages. */
int query_unread_count(string pname) {
    mixed *mbox;
    int i;
    int n;

    if(!stringp(pname)) return 0;
    pname = lower_case(pname);
    mbox  = __mailboxes[pname];
    if(!mbox || !pointerp(mbox)) return 0;
    n = 0;
    i = sizeof(mbox);
    while(i--) {
        if(!mapp(mbox[i])) continue;
        if(!(int)mbox[i]["read"]) n++;
    }
    return n;
}

/* read_mail(player_name, index) -- marks as read, returns letter mapping or 0. */
mapping read_mail(string pname, int idx) {
    mixed *mbox;
    mapping letter;

    if(!stringp(pname) || idx < 1) return 0;
    pname = lower_case(pname);
    mbox  = __mailboxes[pname];
    if(!mbox || !pointerp(mbox) || idx > sizeof(mbox)) return 0;
    letter = (mapping)mbox[idx - 1];
    if(!mapp(letter)) return 0;
    letter["read"] = 1;
    mbox[idx - 1]  = letter;
    __mailboxes[pname] = mbox;
    do_save();
    return letter;
}

/* delete_mail(player_name, index) -- removes a letter. Returns 1 on success. */
int delete_mail(string pname, int idx) {
    mixed *mbox;
    mixed *newmbox;
    int i;

    if(!stringp(pname) || idx < 1) return 0;
    pname = lower_case(pname);
    mbox  = __mailboxes[pname];
    if(!mbox || !pointerp(mbox) || idx > sizeof(mbox)) return 0;
    newmbox = ({});
    for(i = 0; i < sizeof(mbox); i++) {
        if(i != idx - 1) newmbox += ({ mbox[i] });
    }
    __mailboxes[pname] = newmbox;
    do_save();
    return 1;
}

/* delete_read_mail(player_name) -- deletes all read messages. Returns count. */
int delete_read_mail(string pname) {
    mixed *mbox;
    mixed *newmbox;
    int i;
    int removed;

    if(!stringp(pname)) return 0;
    pname = lower_case(pname);
    mbox  = __mailboxes[pname];
    if(!mbox || !pointerp(mbox)) return 0;
    newmbox = ({});
    removed = 0;
    for(i = 0; i < sizeof(mbox); i++) {
        if(mapp(mbox[i]) && (int)mbox[i]["read"])
            removed++;
        else
            newmbox += ({ mbox[i] });
    }
    __mailboxes[pname] = newmbox;
    if(removed > 0) do_save();
    return removed;
}

string query_forward(string pname) {
    if(!stringp(pname) || !sizeof(pname)) return "";
    pname = lower_case(pname);
    if(!mapp(__forwards) || !stringp(__forwards[pname])) return "";
    return __forwards[pname];
}

int set_forward(string pname, string target) {
    if(!stringp(pname) || !sizeof(pname)) return 0;
    pname = lower_case(pname);
    if(!mapp(__forwards)) __forwards = ([]);
    if(!target || !sizeof(target)) {
        map_delete(__forwards, pname);
        do_save();
        return 1;
    }
    __forwards[pname] = lower_case(target);
    do_save();
    return 1;
}
