// /domains/adm/wiz_tools/skill_slip.c
// A skill request slip pulled from the wizard mailbox.
// Commands: approve slip, deny slip

#include <std.h>
#include <daemons.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("slip");
    set_id( ({ "slip", "skill slip", "request slip", "skill request slip" }) );
    set_short("a skill request slip");
    set_long("A slip of paper. Read it for the request details.\n");
    set_mass(10);
    set_value(0);
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_read_slip",    "read");
    add_action("cmd_approve_slip", "approve");
    add_action("cmd_deny_slip",    "deny");
}

string query_long() {
    string pname, txt, ts;
    int t;

    pname = (string)query_property("req_player");
    txt   = (string)query_property("req_text");
    t     = (int)query_property("req_time");
    ts    = (t > 0) ? ctime(t) : "unknown";

    if(!pname || pname == "") pname = "(unknown)";
    if(!txt   || txt == "")   txt   = "(no text)";

    return
        "Player:    " + capitalize(pname) + "\n"
        "Request:   " + txt + "\n"
        "Submitted: " + ts + "\n"
        "Type 'approve slip' or 'deny slip'.\n";
}

int cmd_read_slip(string str) {
    if(!str || strsrch(lower_case(str), "slip") == -1) return 0;
    if(environment(this_object()) != this_player()) return 0;
    write(query_long());
    return 1;
}

int cmd_approve_slip(string str) {
    if(!str || strsrch(lower_case(str), "slip") == -1) return 0;
    if(environment(this_object()) != this_player()) return 0;
    if(!creatorp(this_player())) {
        write("Only wizards can approve skill requests.\n");
        return 1;
    }
    write("Skill name to grant: ");
    input_to("approve_get_skill");
    return 1;
}

void approve_get_skill(string skill_name) {
    if(!skill_name || !strlen(skill_name)) {
        write("Cancelled.\n");
        return;
    }
    set_property("approve_skill", lower_case(skill_name));
    write("Percentage (1-98): ");
    input_to("approve_get_pct");
}

void approve_get_pct(string pct_str) {
    string skill_name;
    string pname;
    string fname;
    object target;
    int pct;

    skill_name = (string)query_property("approve_skill");
    pname      = (string)query_property("req_player");

    if(!pct_str || !strlen(pct_str)) {
        write("Cancelled.\n");
        return;
    }
    pct = to_int(pct_str);
    if(pct < 1 || pct > 98) {
        write("Percentage must be 1-98. Cancelled.\n");
        return;
    }
    if(!skill_name || !strlen(skill_name)) {
        write("No skill selected. Cancelled.\n");
        return;
    }

    target = find_player(pname);
    if(target && objectp(target)) {
        RIFTS_SKILLS_D->grant_skill(target, skill_name, pct);
        RIFTS_SKILLS_D->set_skill_pct(target, skill_name, pct);
        tell_object(target,
            "An RP wizard has granted you: " + skill_name +
            " (" + pct + "%)\n");
        write("Skill granted to " + capitalize(pname) + ".\n");
    } else {
        /* Player offline: save approval for next login */
        fname = "/secure/save/skill_grants/" + pname + ".txt";
        write_file(fname, skill_name + ":" + pct + "\n");
        write(capitalize(pname) +
            " is offline. Grant saved for next login.\n");
    }

    catch(log_file("rp_wizard/skills",
        (string)this_player()->query_name() +
        " approved '" + skill_name + "' at " + pct +
        "% for " + pname + ": " + ctime(time()) + "\n"));

    destruct(this_object());
}

int cmd_deny_slip(string str) {
    string pname;
    string txt;

    if(!str || strsrch(lower_case(str), "slip") == -1) return 0;
    if(environment(this_object()) != this_player()) return 0;
    if(!creatorp(this_player())) {
        write("Only wizards can deny skill requests.\n");
        return 1;
    }

    pname = (string)query_property("req_player");
    txt   = (string)query_property("req_text");

    write("Request from " + capitalize(pname) + " denied and discarded.\n");
    catch(log_file("rp_wizard/skills",
        (string)this_player()->query_name() +
        " denied request from " + pname +
        " (\"" + txt + "\"): " + ctime(time()) + "\n"));
    destruct(this_object());
    return 1;
}
