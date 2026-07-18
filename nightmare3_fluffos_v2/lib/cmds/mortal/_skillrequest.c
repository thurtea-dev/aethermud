// /cmds/mortal/_skillrequest.c
// skillrequest         -- show numbered skill list for your OCC
// skillrequest <N>     -- request skill number N
// skillrequest cancel  -- cancel pending request

#include <std.h>
#include <daemons.h>

inherit DAEMON;

/* Map an OCC category label (from occ.c "occ_skills" arrays) to a
   rifts_skills.c category name. */
private string map_occ_cat(string s) {
    s = lower_case(s);
    if(s == "weapons" || s == "w.p.") return "weapons";
    if(s == "wilderness" || s == "survival") return "survival";
    if(s == "science") return "technical";
    /* pass through: physical, technical, medical, espionage,
                     rogue, military, lore, pilot, electrical,
                     trade, creative */
    return s;
}

private string *build_eligible_list(object player) {
    mapping occ_data;
    string *occ_cats, *player_skills, *cat_skills, *result;
    string occ_name, cat;
    int i, j;

    occ_name = (string)player->getenv("rifts_occ");
    if(!occ_name || !sizeof(occ_name)) return ({});
    occ_name = lower_case(occ_name);

    occ_data = (mapping)OCC_D->query_occ(occ_name);
    if(!occ_data) return ({});

    occ_cats = (string *)occ_data["occ_skills"];
    if(!occ_cats) return ({});

    player_skills = (string *)RIFTS_SKILLS_D->query_player_skills(player);
    if(!player_skills) player_skills = ({});

    result = ({});
    for(i = 0; i < sizeof(occ_cats); i++) {
        cat       = map_occ_cat(occ_cats[i]);
        cat_skills = (string *)RIFTS_SKILLS_D->query_skills_by_category(cat);
        if(!cat_skills) continue;
        for(j = 0; j < sizeof(cat_skills); j++) {
            if(member_array(cat_skills[j], result) == -1 &&
               member_array(cat_skills[j], player_skills) == -1)
                result += ({ cat_skills[j] });
        }
    }
    return result;
}

int cmd_skillrequest(string str) {
    object player, ob;
    object *all;
    string pname, occ_name, skill_name, fname, msg;
    string *menu;
    int i, n, wiz_online;

    player = this_player();
    if(!player) return 0;
    pname = (string)player->query_name();

    /* --- cancel --- */
    if(str && lower_case(str) == "cancel") {
        SKILL_REQUEST_D->clear_request(pname);
        player->set_property("skill_request_menu", 0);
        write("Skill request cancelled.\n");
        all = users();
        for(i = 0; i < sizeof(all); i++) {
            ob = all[i];
            if(!ob || !objectp(ob) || ob == player) continue;
            if(!creatorp(ob)) continue;
            tell_object(ob, "[SKILL REQUEST] " +
                (string)player->query_cap_name() + " cancelled their request.\n");
        }
        return 1;
    }

    /* --- no arg: show numbered menu --- */
    if(!str || !sizeof(str)) {
        occ_name = (string)player->getenv("rifts_occ");
        if(!occ_name || !sizeof(occ_name)) {
            write("You have no OCC set.  Contact a wizard.\n");
            return 1;
        }
        occ_name = lower_case(occ_name);
        menu = build_eligible_list(player);
        if(!sizeof(menu)) {
            write("No secondary skills are available for " + occ_name + " right now.\n");
            return 1;
        }
        player->set_property("skill_request_menu", menu);
        write("Secondary skills available for " + occ_name + ":\n\n");
        for(i = 0; i < sizeof(menu); i++)
            write("  " + (i + 1) + ". " + menu[i] + "\n");
        write("\nType: skillrequest <number>  to request a skill.\n"
              "Type: skillrequest cancel    to withdraw a pending request.\n");
        return 1;
    }

    /* --- numeric pick --- */
    n = to_int(str);
    if(n > 0) {
        menu = (string *)player->query_property("skill_request_menu");
        if(!menu || !sizeof(menu)) {
            menu = build_eligible_list(player);
            player->set_property("skill_request_menu", menu);
        }
        if(!sizeof(menu)) {
            write("No skills available.  Type 'skillrequest' to check.\n");
            return 1;
        }
        if(n > sizeof(menu)) {
            write("Invalid choice.  Type 'skillrequest' to see the list (1-" +
                sizeof(menu) + ").\n");
            return 1;
        }
        skill_name = menu[n - 1];
    } else {
        write("Unknown option.  Use 'skillrequest' to see the numbered list,\n"
              "or 'skillrequest cancel' to withdraw a request.\n");
        return 1;
    }

    /* --- submit the chosen skill --- */
    SKILL_REQUEST_D->submit_request(player, skill_name);

    msg = "[SKILL REQUEST] " + (string)player->query_cap_name() +
          " requests: " + skill_name +
          ".  Use rp_skill_tool option 7 to review.\n";

    wiz_online = 0;
    all = users();
    for(i = 0; i < sizeof(all); i++) {
        ob = all[i];
        if(!ob || !objectp(ob) || ob == player) continue;
        if(!creatorp(ob)) continue;
        tell_object(ob, msg);
        wiz_online++;
    }

    if(wiz_online > 0) {
        write("Your request for '" + skill_name + "' has been sent to online wizards.\n");
    } else {
        fname = "/secure/save/skill_requests/" + pname + ".txt";
        write_file(fname,
            "player:" + pname + "\n" +
            "text:" + skill_name + "\n" +
            "time:" + time() + "\n");
        write("No wizards online.  Your request for '" + skill_name + "' has been saved.\n");
    }
    write("Type 'skillrequest' to see the list again.\n");
    return 1;
}

void init() {
    add_action("cmd_skillrequest", "skillrequest");
}

void help() {
    write(
        "Syntax: skillrequest\n"
        "        skillrequest <number>\n"
        "        skillrequest cancel\n\n"
        "  skillrequest           -- list secondary skills available for your OCC\n"
        "  skillrequest <number>  -- request the skill at that number\n"
        "  skillrequest cancel    -- withdraw your pending request\n\n"
        "Skills are drawn from your OCC secondary skill categories.  Only skills\n"
        "you do not already have are shown.  A wizard will approve or deny the\n"
        "request.\n"
    );
}
