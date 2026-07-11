// /daemon/apprentice_d.c
// Apprentice track submissions: draft -> domain review -> coding queue -> ready.

#include <std.h>
#include <dirs.h>
#include <security.h>

inherit DAEMON;

#define SAVE_FILE (DIR_DAEMONS_SAVE + "/apprentice_d")
#define SAVE_DIR  "/secure/save/apprentice"
#define SUB_DIR   "/secure/save/apprentice/submissions"
#define PLAY_DIR  "/secure/save/apprentice/players"

mapping submissions;
mapping player_state;
int next_id;

void save_data() {
    unguarded((: save_object, SAVE_FILE :));
}

void ensure_dirs() {
    if(file_size(SAVE_DIR) != -2)
        mkdir(SAVE_DIR);
    if(file_size(SUB_DIR) != -2)
        mkdir(SUB_DIR);
    if(file_size(PLAY_DIR) != -2)
        mkdir(PLAY_DIR);
}

void create() {
    daemon::create();
    set_no_clean(1);
    submissions = ([]);
    player_state = ([]);
    next_id = 1;
    ensure_dirs();
    if(file_size(SAVE_FILE + ".o") > 0)
        unguarded((: restore_object, SAVE_FILE :));
    if(!submissions)
        submissions = ([]);
    if(!player_state)
        player_state = ([]);
    if(next_id < 1)
        next_id = 1;
}

string make_id() {
    string id;

    id = sprintf("%d_%d", time(), next_id);
    next_id++;
    save_data();
    return id;
}

mapping empty_draft(string track) {
    return ([
        "track" : track,
        "status" : "draft",
        "title" : "",
        "body" : ([]),
        "created" : time(),
        "updated" : time(),
        "reviewer" : "",
        "reject_reason" : "",
        "coder" : ""
    ]);
}

void write_submission_file(string id) {
    mapping sub;
    mapping body;
    string path;
    string out;
    string *bkeys;
    int i;

    if(!id || !submissions[id])
        return;
    sub = submissions[id];
    path = SUB_DIR + "/" + id + ".txt";
    out = "id:" + id + "\n";
    out += "author:" + sub["author"] + "\n";
    out += "track:" + sub["track"] + "\n";
    out += "status:" + sub["status"] + "\n";
    out += "title:" + sub["title"] + "\n";
    out += "created:" + ctime(sub["created"]) + "\n";
    out += "updated:" + ctime(sub["updated"]) + "\n";
    if(sub["reviewer"] && sizeof(sub["reviewer"]))
        out += "reviewer:" + sub["reviewer"] + "\n";
    if(sub["coder"] && sizeof(sub["coder"]))
        out += "coder:" + sub["coder"] + "\n";
    if(sub["reject_reason"] && sizeof(sub["reject_reason"]))
        out += "reject_reason:" + sub["reject_reason"] + "\n";
    out += "---\n";
    body = sub["body"];
    if(mapp(body)) {
        bkeys = keys(body);
        for(i = 0; i < sizeof(bkeys); i++)
            out += bkeys[i] + ":\n" + body[bkeys[i]] + "\n\n";
    }
    rm(path);
    write_file(path, out);
}

void write_player_file(string pname) {
    mapping st;
    string path;
    string out;

    if(!pname)
        return;
    st = player_state[pname];
    if(!st)
        return;
    path = PLAY_DIR + "/" + pname + ".txt";
    out = "player:" + pname + "\n";
    out += "track:" + (st["track"] ? st["track"] : "") + "\n";
    out += "draft_id:" + (st["draft_id"] ? st["draft_id"] : "") + "\n";
    out += "latest_id:" + (st["latest_id"] ? st["latest_id"] : "") + "\n";
    out += "ready_for_setrole:" + (st["ready_for_setrole"] ? "1" : "0") + "\n";
    rm(path);
    write_file(path, out);
}

void notify_staff(string kind, string msg) {
    object *all;
    object ob;
    string pos;
    int i;

    all = users();
    for(i = 0; i < sizeof(all); i++) {
        ob = all[i];
        if(!ob || !creatorp(ob))
            continue;
        pos = lower_case((string)ob->query_position());
        if(kind == "domain") {
            if(pos != "arch" && pos != "head arch" && pos != "domain_wiz")
                continue;
        } else if(kind == "coding") {
            if(pos != "arch" && pos != "head arch" && pos != "creator" &&
               pos != "immortal")
                continue;
        }
        tell_object(ob, "%^YELLOW%^[Apprentice] " + msg + "%^RESET%^\n");
    }
}

string format_submission(mapping sub) {
    string out;
    mapping body;
    string *bkeys;
    int i;

    if(!sub)
        return "(none)\n";
    out = "Author: " + capitalize(sub["author"]) + "\n";
    out += "Track: " + sub["track"] + "\n";
    out += "Status: " + sub["status"] + "\n";
    out += "Title: " + sub["title"] + "\n";
    out += "Updated: " + ctime(sub["updated"]) + "\n";
    if(sub["reject_reason"] && sizeof(sub["reject_reason"]))
        out += "Reject reason: " + sub["reject_reason"] + "\n";
    out += "----\n";
    body = sub["body"];
    if(mapp(body)) {
        bkeys = keys(body);
        for(i = 0; i < sizeof(bkeys); i++)
            out += capitalize(bkeys[i]) + ":\n  " + body[bkeys[i]] + "\n\n";
    }
    return out;
}

int set_track(object player, string track) {
    string pname;
    mapping st;

    if(!player || !track)
        return 0;
    track = lower_case(track);
    if(track != "rp" && track != "domain" && track != "coding")
        return 0;
    pname = (string)player->query_name();
    st = player_state[pname];
    if(!st)
        st = ([]);
    st["track"] = track;
    if(!st["ready_for_setrole"])
        st["ready_for_setrole"] = 0;
    player_state[pname] = st;
    player->setenv("apprentice_track", track);
    write_player_file(pname);
    save_data();
    return 1;
}

string query_track(object player) {
    string pname;
    mapping st;
    string env;

    if(!player)
        return 0;
    pname = (string)player->query_name();
    st = player_state[pname];
    if(st && st["track"])
        return st["track"];
    env = (string)player->getenv("apprentice_track");
    if(env && sizeof(env))
        return env;
    return 0;
}

mapping query_player_state(string pname) {
    if(!pname)
        return 0;
    return player_state[pname];
}

int query_ready(string pname) {
    mapping st;

    if(!pname)
        return 0;
    st = player_state[pname];
    if(!st)
        return 0;
    return st["ready_for_setrole"] ? 1 : 0;
}

mapping get_submission(string id) {
    if(!id)
        return 0;
    return submissions[id];
}

mapping get_draft(object player) {
    string pname;
    mapping st;
    string id;

    if(!player)
        return 0;
    pname = (string)player->query_name();
    st = player_state[pname];
    if(!st || !st["draft_id"])
        return 0;
    id = st["draft_id"];
    return submissions[id];
}

int save_draft(object player, mapping fields) {
    string pname;
    string track;
    string id;
    mapping st;
    mapping sub;
    mapping body;
    string *fkeys;
    int i;

    if(!player || !mapp(fields))
        return 0;
    pname = (string)player->query_name();
    track = query_track(player);
    if(!track)
        return 0;
    st = player_state[pname];
    if(!st)
        st = ([ "track": track, "ready_for_setrole": 0 ]);
    id = st["draft_id"];
    sub = 0;
    if(id && submissions[id]) {
        sub = submissions[id];
        if(sub["status"] == "draft") {
            /* reuse */
        } else if(sub["status"] == "rejected") {
            sub["status"] = "draft";
            sub["reject_reason"] = "";
        } else
            sub = 0;
    }
    if(!sub) {
        id = make_id();
        sub = empty_draft(track);
        sub["id"] = id;
        sub["author"] = pname;
        submissions[id] = sub;
        st["draft_id"] = id;
    }
    if(sub["author"] != pname)
        return 0;
    body = sub["body"];
    if(!mapp(body))
        body = ([]);
    fkeys = keys(fields);
    for(i = 0; i < sizeof(fkeys); i++) {
        if(fkeys[i] == "title")
            sub["title"] = fields["title"];
        else
            body[fkeys[i]] = fields[fkeys[i]];
    }
    sub["body"] = body;
    sub["track"] = track;
    sub["updated"] = time();
    sub["status"] = "draft";
    submissions[id] = sub;
    st["track"] = track;
    st["draft_id"] = id;
    player_state[pname] = st;
    write_submission_file(id);
    write_player_file(pname);
    save_data();
    return 1;
}

int submit(object player) {
    string pname;
    mapping st;
    mapping sub;
    mapping body;
    string id;
    string title;

    if(!player)
        return 0;
    pname = (string)player->query_name();
    st = player_state[pname];
    if(!st || !st["draft_id"])
        return 0;
    id = st["draft_id"];
    sub = submissions[id];
    if(!sub || sub["author"] != pname)
        return 0;
    if(sub["status"] != "draft" && sub["status"] != "rejected")
        return 0;
    title = sub["title"];
    if(!title || !sizeof(title))
        return -1;
    body = sub["body"];
    if(!mapp(body) || !sizeof(keys(body)))
        return -2;
    sub["status"] = "domain_review";
    sub["updated"] = time();
    sub["reject_reason"] = "";
    submissions[id] = sub;
    st["latest_id"] = id;
    player_state[pname] = st;
    write_submission_file(id);
    write_player_file(pname);
    save_data();
    notify_staff("domain",
        capitalize(pname) + " submitted a " + sub["track"] +
        " apprenticeship task for review (" + title + ").");
    catch(log_file("adm/apprentice",
        ctime(time()) + " " + pname + " submitted " + id + " track=" +
        sub["track"] + "\n"));
    return 1;
}

mixed list_domain_queue() {
    string *ids;
    string *out;
    mapping sub;
    int i;

    out = ({});
    ids = keys(submissions);
    for(i = 0; i < sizeof(ids); i++) {
        sub = submissions[ids[i]];
        if(sub && sub["status"] == "domain_review")
            out += ({ ids[i] });
    }
    return out;
}

mixed list_coding_queue() {
    string *ids;
    string *out;
    mapping sub;
    int i;

    out = ({});
    ids = keys(submissions);
    for(i = 0; i < sizeof(ids); i++) {
        sub = submissions[ids[i]];
        if(sub && sub["status"] == "coding_queue")
            out += ({ ids[i] });
    }
    return out;
}

mixed list_author_submissions(string pname) {
    string *ids;
    string *out;
    mapping sub;
    int i;

    out = ({});
    if(!pname)
        return out;
    ids = keys(submissions);
    for(i = 0; i < sizeof(ids); i++) {
        sub = submissions[ids[i]];
        if(sub && sub["author"] == pname)
            out += ({ ids[i] });
    }
    return out;
}

void ensure_player_state(string pname) {
    if(!player_state[pname])
        player_state[pname] = ([ "ready_for_setrole": 0 ]);
}

int mark_ready_for_setrole(string pname, string reviewer) {
    mapping st;
    object pl;

    if(!pname)
        return 0;
    ensure_player_state(pname);
    st = player_state[pname];
    st["ready_for_setrole"] = 1;
    player_state[pname] = st;
    write_player_file(pname);
    save_data();
    pl = find_player(pname);
    if(pl)
        tell_object(pl,
            "%^YELLOW%^A domain wizard marked you ready for setrole. "
            "An admin must still run setrole.%^RESET%^\n");
    catch(log_file("adm/apprentice",
        ctime(time()) + " " + reviewer + " marked " + pname +
        " ready_for_setrole\n"));
    return 1;
}

int domain_forward_coding(string id, string reviewer) {
    mapping sub;
    string author;

    if(!id || !submissions[id])
        return 0;
    sub = submissions[id];
    if(sub["status"] != "domain_review" && sub["status"] != "coding_queue")
        return 0;
    sub["status"] = "coding_queue";
    sub["reviewer"] = reviewer;
    sub["updated"] = time();
    submissions[id] = sub;
    write_submission_file(id);
    save_data();
    author = sub["author"];
    notify_staff("coding",
        "Submission " + id + " (" + sub["track"] + " / " +
        capitalize(author) + ") is in the coding queue.");
    catch(log_file("adm/apprentice",
        ctime(time()) + " " + reviewer + " forwarded " + id +
        " to coding\n"));
    return 1;
}

int domain_approve(string id, string reviewer) {
    mapping sub;
    string track;
    string author;

    if(!id || !submissions[id])
        return 0;
    sub = submissions[id];
    if(sub["status"] != "domain_review")
        return 0;
    track = sub["track"];
    author = sub["author"];
    sub["reviewer"] = reviewer;
    sub["updated"] = time();

    if(track == "domain") {
        sub["status"] = "ready";
        submissions[id] = sub;
        write_submission_file(id);
        save_data();
        mark_ready_for_setrole(author, reviewer);
        notify_staff("domain",
            "Domain outline " + id + " approved; " + capitalize(author) +
            " marked ready for setrole.");
        return 1;
    }

    sub["status"] = "coding_queue";
    submissions[id] = sub;
    write_submission_file(id);
    save_data();
    notify_staff("coding",
        "Submission " + id + " (" + track + " / " + capitalize(author) +
        ") approved by domain and sent to coding.");
    catch(log_file("adm/apprentice",
        ctime(time()) + " " + reviewer + " approved " + id +
        " -> coding_queue\n"));
    return 1;
}

int domain_reject(string id, string reviewer, string reason) {
    mapping sub;
    mapping st;
    string author;
    object pl;

    if(!id || !submissions[id])
        return 0;
    sub = submissions[id];
    if(sub["status"] != "domain_review" && sub["status"] != "coding_queue")
        return 0;
    author = sub["author"];
    sub["status"] = "rejected";
    sub["reviewer"] = reviewer;
    if(reason && sizeof(reason))
        sub["reject_reason"] = reason;
    else
        sub["reject_reason"] = "No reason given.";
    sub["updated"] = time();
    submissions[id] = sub;
    ensure_player_state(author);
    st = player_state[author];
    st["draft_id"] = id;
    player_state[author] = st;
    write_submission_file(id);
    write_player_file(author);
    save_data();
    pl = find_player(author);
    if(pl)
        tell_object(pl,
            "%^YELLOW%^Your apprenticeship submission was rejected: " +
            sub["reject_reason"] + "%^RESET%^\n"
            "Use kit to revise and resubmit.\n");
    catch(log_file("adm/apprentice",
        ctime(time()) + " " + reviewer + " rejected " + id + ": " +
        sub["reject_reason"] + "\n"));
    return 1;
}

int coding_mark_done(string id, string coder) {
    mapping sub;
    string author;

    if(!id || !submissions[id])
        return 0;
    sub = submissions[id];
    if(sub["status"] != "coding_queue")
        return 0;
    author = sub["author"];
    sub["status"] = "implemented";
    sub["coder"] = coder;
    sub["updated"] = time();
    submissions[id] = sub;
    write_submission_file(id);
    save_data();
    mark_ready_for_setrole(author, coder);
    notify_staff("domain",
        "Coding marked " + id + " implemented; " + capitalize(author) +
        " is ready for setrole.");
    catch(log_file("adm/apprentice",
        ctime(time()) + " " + coder + " implemented " + id + "\n"));
    return 1;
}

int domain_accept_as_is(string id, string reviewer) {
    mapping sub;
    string author;

    if(!id || !submissions[id])
        return 0;
    sub = submissions[id];
    if(sub["status"] != "domain_review" && sub["status"] != "coding_queue")
        return 0;
    if(sub["track"] != "coding")
        return 0;
    author = sub["author"];
    sub["status"] = "implemented";
    sub["reviewer"] = reviewer;
    sub["updated"] = time();
    submissions[id] = sub;
    write_submission_file(id);
    save_data();
    mark_ready_for_setrole(author, reviewer);
    return 1;
}
