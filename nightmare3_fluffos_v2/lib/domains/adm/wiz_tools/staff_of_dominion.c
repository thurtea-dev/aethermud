// /domains/adm/wiz_tools/staff_of_dominion.c
// Staff of Domains -- Domain Wizard tool for AetherMUD
// Menu-driven domain management for domain-level staff.

#include <std.h>
#include <security.h>
#include <dirs.h>
#include <daemons.h>

inherit OBJECT;

static private string pending_action;
static private string pending_sub;

void do_list_domains();
void show_apprentice_menu();
void handle_apprentice_choice(string str);
void list_queue(string which);
void read_submission(string id);
void after_apprentice_action();
void get_apprentice_id(string id);
void get_reject_reason(string reason);
void get_apprentice_player(string pname);
private int valid_domain_name(string name);
private string resolve_domain(string name);

// Resolve a typed domain name to the real directory name on disk,
// case-insensitively -- existing domains (Praxis, Lazlo, NGR, ...) are
// capitalized and the filesystem is case-sensitive, so an exact-case match
// cannot be assumed. Returns the on-disk name, or 0 if no domain matches.
private string resolve_domain(string name) {
    string *doms;
    int i;

    if(!name || !sizeof(name)) return 0;
    if(file_size("/domains/" + name) == -2) return name;
    doms = get_dir("/domains/");
    if(!doms) return 0;
    for(i = 0; i < sizeof(doms); i++)
        if(lower_case(doms[i]) == lower_case(name)) return doms[i];
    return 0;
}

// A domain name must be a single path component: a letter followed by
// letters, digits, or underscores. This blocks path escapes (slashes, "..",
// leading dots, "#") the driver's legal_path() would reject anyway, and also
// stops benign-but-junk names (spaces, punctuation) that legal_path() allows
// and that would otherwise create stray directories inside /domains/.
private int valid_domain_name(string name) {
    int i, c;

    if(!name || !sizeof(name)) return 0;
    if(sizeof(name) > 32)      return 0;
    c = name[0];
    if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) return 0;
    for(i = 0; i < sizeof(name); i++) {
        c = name[i];
        if(c >= 'a' && c <= 'z') continue;
        if(c >= 'A' && c <= 'Z') continue;
        if(c >= '0' && c <= '9') continue;
        if(c == '_') continue;
        return 0;
    }
    return 1;
}

void create() {
    ::create();
    set_name("staff of domains");
    set_id( ({ "staff", "staff of domains", "domains staff", "domain staff" }) );
    set_short("a staff of domains");
    set_long(
        "A gleaming crystalline staff pulsing with territorial authority.\n"
        "It grants its wielder power over the domains of the world.\n"
        "For Domain and Admin wizards. Type: domain");
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
    add_action("cmd_use_staff", "domain");
}

private void show_menu() {
    write("\n=== Domain Menu ===");
    write(" 1. Create new domain directory");
    write(" 2. Assign domain to coding wizard");
    write(" 3. List all domains");
    write(" 4. Set domain description");
    write(" 5. Teleport to domain start room");
    write(" 6. Grant building access to player");
    write(" 7. Revoke building access");
    write(" 8. List players in domain");
    write(" 9. Reset all rooms in domain");
    write("10. Apprentice submissions...");
    write(" 0. Exit");
    write("Choice: ");
}

int cmd_use_staff(string str) {
    if(!admin_wizp(this_player()) && !domain_wizp(this_player())) {
        write("The staff refuses to respond to you.\n");
        return 1;
    }
    pending_action = "";
    pending_sub = "";
    show_menu();
    input_to("handle_choice");
    return 1;
}

void handle_choice(string str) {
    int choice;
    if(!str || !sizeof(str)) { write("Invalid choice.\n"); show_menu(); input_to("handle_choice"); return; }
    choice = to_int(str);
    switch(choice) {
    case 0: write("Domain menu closed."); return;
    case 1: pending_action = "mkdomain"; write("Domain name: "); input_to("get_domain"); return;
    case 2: pending_action = "assign"; write("Domain name: "); input_to("get_domain"); return;
    case 3: do_list_domains(); return;
    case 4: pending_action = "setdesc"; write("Domain name: "); input_to("get_domain"); return;
    case 5: pending_action = "goto"; write("Domain name: "); input_to("get_domain"); return;
    case 6: pending_action = "grant"; write("Player name: "); input_to("get_player"); return;
    case 7: pending_action = "revoke"; write("Player name: "); input_to("get_player"); return;
    case 8: pending_action = "listplay"; write("Domain name: "); input_to("get_domain"); return;
    case 9: pending_action = "reset"; write("Domain name: "); input_to("get_domain"); return;
    case 10:
        show_apprentice_menu();
        input_to("handle_apprentice_choice");
        return;
    default:
        write("Invalid option.\n");
        show_menu();
        input_to("handle_choice");
    }
}

void get_domain(string name) {
    if(!name || !sizeof(name)) { write("Cancelled.\n"); return; }
    if(!valid_domain_name(name)) {
        write("Invalid domain name. Use letters, digits, and underscores only, "
              "starting with a letter (no slashes, dots, or spaces).\n");
        return;
    }
    pending_sub = name;
    switch(pending_action) {
    case "mkdomain":
        {
            string dpath;

            if(resolve_domain(pending_sub)) {
                write("Domain already exists.\n"); return;
            }
            dpath = "/domains/" + pending_sub;
            mkdir(dpath);
            mkdir(dpath + "/areas");
            mkdir(dpath + "/monsters");
            mkdir(dpath + "/obj");
            // Seed a minimal start room so option 5 (goto) works immediately
            // and the domain matches the areas/ layout every live domain uses.
            if(file_size(dpath + "/areas/start.c") < 0)
                write_file(dpath + "/areas/start.c",
                    "// /domains/" + pending_sub + "/areas/start.c\n"
                    "// Auto-generated domain start room. Edit or replace.\n\n"
                    "#include <std.h>\n\n"
                    "inherit ROOM;\n\n"
                    "void create() {\n"
                    "    ::create();\n"
                    "    set_property(\"light\", 3);\n"
                    "    set_property(\"indoors\", 1);\n"
                    "    set_short(\"" + capitalize(pending_sub) + " start room\");\n"
                    "    set_long(\n"
                    "        \"A newly created domain awaiting development. Build outward\\n\"\n"
                    "        \"from here.\\n\");\n"
                    "}\n");
            write("Domain /domains/" + pending_sub +
                  "/ created with areas/, monsters/, obj/ and a start room.");
        }
        break;
    case "assign":
        write("Wizard to assign as owner: ");
        input_to("do_assign");
        return;
    case "setdesc":
        write("Description: ");
        input_to("do_setdesc");
        return;
    case "goto":
        {
            string real;
            string *candidates;
            string found;
            int i;

            real = resolve_domain(pending_sub);
            if(!real) { write("No such domain."); break; }
            candidates = ({
                "/domains/" + real + "/areas/start",
                "/domains/" + real + "/room/start",
                "/domains/" + real + "/rooms/start"
            });
            found = 0;
            for(i = 0; i < sizeof(candidates); i++)
                if(file_size(candidates[i] + ".c") > 0) { found = candidates[i]; break; }
            if(found) this_player()->move_player(found);
            else write("No start room found for domain " + real + ".");
        }
        break;
    case "listplay":
        {
            object *all, *here;
            int i;
            string dom_path;
            string real;
            real = resolve_domain(pending_sub);
            if(!real) { write("No such domain."); break; }
            dom_path = "/domains/" + real;
            all = users();
            here = ({});
            for(i = 0; i < sizeof(all); i++) {
                string env;
                env = file_name(environment(all[i]));
                if(env && strsrch(env, dom_path) != -1) here += ({ all[i] });
            }
            if(!sizeof(here)) write("No players in domain " + real + ".");
            else {
                write("Players in " + real + ":");
                for(i = 0; i < sizeof(here); i++)
                    write(" " + (string)here[i]->query_cap_name());
            }
        }
        break;
    case "reset":
        {
            string *try_dirs;
            string *files;
            string rdir;
            string base;
            object ob;
            string real;
            int i;
            int j;
            int cnt;

            real = resolve_domain(pending_sub);
            if(!real) { write("No such domain."); break; }
            try_dirs = ({
                "/domains/" + real + "/rooms",
                "/domains/" + real + "/room",
                "/domains/" + real + "/areas"
            });
            cnt = 0;
            for(i = 0; i < sizeof(try_dirs); i++) {
                rdir = try_dirs[i];
                if(file_size(rdir) != -2) continue;
                files = get_dir(rdir + "/");
                if(!files || !sizeof(files)) continue;
                for(j = 0; j < sizeof(files); j++) {
                    if(sizeof(files[j]) < 3) continue;
                    if(files[j][sizeof(files[j])-2..] != ".c") continue;
                    base = rdir + "/" + files[j][0..sizeof(files[j])-3];
                    ob = find_object(base);
                    if(ob) { ob->reset(); cnt++; }
                }
            }
            write("Reset " + cnt + " loaded room(s) in " + real + ".");
        }
        break;
    }
}

void get_player(string pname) {
    object ob;
    if(!pname || !sizeof(pname)) { write("Cancelled.\n"); return; }
    pending_sub = lower_case(pname);
    ob = find_player(pending_sub);
    switch(pending_action) {
    case "grant":
        write("Domain to grant access to: ");
        input_to("do_grant");
        return;
    case "revoke":
        write("Domain to revoke: ");
        input_to("do_revoke");
        return;
    }
}

void do_assign(string wiz) {
    if(!wiz || !sizeof(wiz)) { write("Cancelled.\n"); return; }
    write("Domain " + pending_sub + " assigned to " + capitalize(wiz) + ".");
    write("(Ownership is logged only. To give " + capitalize(wiz) +
          " write access, run:\n"
          "   grant write to " + lower_case(wiz) +
          " on /domains/" + pending_sub + "/)");
    catch(log_file("/log/adm/domain_log", this_player()->query_name() +
        " assigned domain " + pending_sub + " to " + wiz + ": " +
        ctime(time()) + "\n"));
}

void do_setdesc(string desc) {
    string real;
    if(!desc || !sizeof(desc)) { write("Cancelled.\n"); return; }
    real = resolve_domain(pending_sub);
    if(!real) { write("No such domain.\n"); return; }
    write_file("/domains/" + real + "/.desc", desc + "\n");
    write("Description set for domain " + real + ".");
}

void do_grant(string domain) {
    if(!domain || !sizeof(domain)) { write("Cancelled.\n"); return; }
    if(!valid_domain_name(domain)) { write("Invalid domain name.\n"); return; }
    write("Access request logged: " + capitalize(pending_sub) +
          " -> /domains/" + domain + "/");
    write("(Logged only. To make it effective, an admin runs:\n"
          "   grant write to " + lower_case(pending_sub) +
          " on /domains/" + domain + "/\n"
          " which records it in /domains/" + domain + "/adm/access.c.)");
    catch(log_file("/log/adm/domain_log", this_player()->query_name() +
        " granted " + pending_sub + " access to " + domain + ": " +
        ctime(time()) + "\n"));
}

void do_revoke(string domain) {
    if(!domain || !sizeof(domain)) { write("Cancelled.\n"); return; }
    if(!valid_domain_name(domain)) { write("Invalid domain name.\n"); return; }
    write("Revocation request logged: " + capitalize(pending_sub) +
          " from /domains/" + domain + "/");
    write("(Logged only. To make it effective, an admin must remove the\n"
          " grant from /domains/" + domain + "/adm/access.c -- there is no\n"
          " revoke command; edit the file or call its remove_access().)");
    catch(log_file("/log/adm/domain_log", this_player()->query_name() +
        " revoked " + pending_sub + " from " + domain + ": " +
        ctime(time()) + "\n"));
}

void do_list_domains() {
    string *doms;
    doms = get_dir("/domains/");
    if(!doms || !sizeof(doms)) { write("No domains found."); }
    else {
        write("=== Domains ===");
        write(implode(sort_array(doms, 1), "\n "));
    }
    show_menu();
    input_to("handle_choice");
}

void show_apprentice_menu() {
    write("\n=== Apprentice Submissions ===");
    write(" 1. List domain review queue");
    write(" 2. List coding queue");
    write(" 3. Read submission");
    write(" 4. Approve submission");
    write(" 5. Reject submission");
    write(" 6. Forward to coding");
    write(" 7. Accept coding build as-is");
    write(" 8. Mark apprentice ready for setrole");
    write(" 0. Back");
    write("Choice: ");
}

void after_apprentice_action() {
    show_apprentice_menu();
    input_to("handle_apprentice_choice");
}

void list_queue(string which) {
    string *ids;
    mapping sub;
    int i;

    if(which == "coding")
        ids = (string *)APPRENTICE_D->list_coding_queue();
    else
        ids = (string *)APPRENTICE_D->list_domain_queue();
    if(!ids || !sizeof(ids)) {
        write("Queue empty.\n");
        return;
    }
    write("=== " + capitalize(which) + " queue ===\n");
    for(i = 0; i < sizeof(ids); i++) {
        sub = (mapping)APPRENTICE_D->get_submission(ids[i]);
        if(!sub) continue;
        write(ids[i] + " [" + sub["track"] + "] " +
              capitalize(sub["author"]) + " - " + sub["title"] + "\n");
    }
}

void read_submission(string id) {
    mapping sub;

    sub = (mapping)APPRENTICE_D->get_submission(id);
    if(!sub) {
        write("No such submission.\n");
        return;
    }
    write((string)APPRENTICE_D->format_submission(sub));
}

void handle_apprentice_choice(string str) {
    int choice;

    if(!str || !sizeof(str)) {
        write("Invalid choice.\n");
        after_apprentice_action();
        return;
    }
    choice = to_int(str);
    switch(choice) {
    case 0:
        show_menu();
        input_to("handle_choice");
        return;
    case 1:
        list_queue("domain");
        after_apprentice_action();
        return;
    case 2:
        list_queue("coding");
        after_apprentice_action();
        return;
    case 3:
        pending_action = "ap_read";
        write("Submission id: ");
        input_to("get_apprentice_id");
        return;
    case 4:
        pending_action = "ap_approve";
        write("Submission id: ");
        input_to("get_apprentice_id");
        return;
    case 5:
        pending_action = "ap_reject";
        write("Submission id: ");
        input_to("get_apprentice_id");
        return;
    case 6:
        pending_action = "ap_forward";
        write("Submission id: ");
        input_to("get_apprentice_id");
        return;
    case 7:
        pending_action = "ap_accept";
        write("Submission id (coding track): ");
        input_to("get_apprentice_id");
        return;
    case 8:
        pending_action = "ap_ready";
        write("Apprentice player name: ");
        input_to("get_apprentice_player");
        return;
    default:
        write("Invalid option.\n");
        after_apprentice_action();
    }
}

void get_apprentice_id(string id) {
    string who;

    if(!id || !sizeof(id)) {
        write("Cancelled.\n");
        after_apprentice_action();
        return;
    }
    id = replace_string(id, " ", "");
    who = (string)this_player()->query_name();
    switch(pending_action) {
    case "ap_read":
        read_submission(id);
        break;
    case "ap_approve":
        if((int)APPRENTICE_D->domain_approve(id, who))
            write("Approved.\n");
        else
            write("Could not approve (wrong id or status).\n");
        break;
    case "ap_reject":
        pending_sub = id;
        write("Reject reason: ");
        input_to("get_reject_reason");
        return;
    case "ap_forward":
        if((int)APPRENTICE_D->domain_forward_coding(id, who))
            write("Forwarded to coding queue.\n");
        else
            write("Could not forward.\n");
        break;
    case "ap_accept":
        if((int)APPRENTICE_D->domain_accept_as_is(id, who))
            write("Accepted as-is; apprentice marked ready for setrole.\n");
        else
            write("Could not accept (must be coding track in review/queue).\n");
        break;
    }
    after_apprentice_action();
}

void get_reject_reason(string reason) {
    string who;

    who = (string)this_player()->query_name();
    if(!reason || !sizeof(reason)) reason = "No reason given.";
    if((int)APPRENTICE_D->domain_reject(pending_sub, who, reason))
        write("Rejected.\n");
    else
        write("Could not reject.\n");
    after_apprentice_action();
}

void get_apprentice_player(string pname) {
    string who;

    if(!pname || !sizeof(pname)) {
        write("Cancelled.\n");
        after_apprentice_action();
        return;
    }
    pname = lower_case(pname);
    who = (string)this_player()->query_name();
    if((int)APPRENTICE_D->mark_ready_for_setrole(pname, who))
        write(capitalize(pname) + " marked ready for setrole.\n");
    else
        write("Failed.\n");
    after_apprentice_action();
}
