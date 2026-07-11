/* /domains/adm/wiz_tools/apprentice_kit.c
   Apprentice wizard kit: pick a track, draft a small task, submit for review.
   Verbs: kit, apprentice */

#include <std.h>
#include <daemons.h>

inherit OBJECT;

static private string pending_action;
static private mapping draft_fields;
static private mixed prompt_keys;
static private mixed prompt_labels;
static private int prompt_index;

void show_menu();
void handle_choice(string str);
void handle_track(string str);
void start_draft_prompts();
void next_prompt();
void handle_prompt(string str);
void do_submit();
void show_status();
void show_brief();
mixed track_keys(string track);
mixed track_labels(string track);
string track_brief(string track);

void create() {
    ::create();
    set_name("apprentice kit");
    set_id( ({ "kit", "apprentice kit", "apprentice_kit", "tool" }) );
    set_short("an apprentice kit");
    set_long(
        "A slim binder of apprenticeship forms and ley-line seals.\n"
        "It guides temporary staff through a track task for RP, Domain,\n"
        "or Coding. Type: kit   (also: apprentice)");
    set_mass(100);
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
    add_action("cmd_kit", "kit");
    add_action("cmd_kit", "apprentice");
}

int cmd_kit(string str) {
    if(!apprentice_wizp(this_player()) && !admin_wizp(this_player())) {
        write("The kit only opens for Apprentice wizards.\n");
        return 1;
    }
    pending_action = "";
    draft_fields = ([]);
    prompt_keys = ({});
    prompt_labels = ({});
    prompt_index = 0;
    show_menu();
    input_to("handle_choice");
    return 1;
}

void show_menu() {
    string track;

    track = (string)APPRENTICE_D->query_track(this_player());
    write("\n=== Apprentice Kit ===");
    write(" Track: " + (track ? track : "(none chosen)"));
    write(" 1. Choose track (rp / domain / coding)");
    write(" 2. View task brief");
    write(" 3. Draft / edit submission");
    write(" 4. Submit to domain queue");
    write(" 5. View my submission status");
    write(" 0. Exit");
    write("Choice: ");
}

void handle_choice(string str) {
    int choice;

    if(!str || !sizeof(str)) {
        write("Invalid choice.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    choice = to_int(str);
    switch(choice) {
    case 0:
        write("Apprentice Kit closed.\n");
        return;
    case 1:
        write("Track (rp, domain, coding): ");
        input_to("handle_track");
        return;
    case 2:
        show_brief();
        show_menu();
        input_to("handle_choice");
        return;
    case 3:
        if(!(string)APPRENTICE_D->query_track(this_player())) {
            write("Choose a track first.\n");
            show_menu();
            input_to("handle_choice");
            return;
        }
        start_draft_prompts();
        return;
    case 4:
        do_submit();
        show_menu();
        input_to("handle_choice");
        return;
    case 5:
        show_status();
        show_menu();
        input_to("handle_choice");
        return;
    default:
        write("Invalid option.\n");
        show_menu();
        input_to("handle_choice");
    }
}

void handle_track(string str) {
    if(!str || !sizeof(str)) {
        write("Cancelled.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    str = lower_case(replace_string(str, " ", ""));
    if(str == "roleplay") str = "rp";
    if(!(int)APPRENTICE_D->set_track(this_player(), str)) {
        write("Valid tracks: rp, domain, coding.\n");
        show_menu();
        input_to("handle_choice");
        return;
    }
    write("Track set to " + str + ".\n");
    write(track_brief(str));
    show_menu();
    input_to("handle_choice");
}

string track_brief(string track) {
    if(track == "rp")
        return
        "RP task: write a campaign/quest proposal for domain review.\n"
        "Domain will forward approved proposals to coding for implementation.\n";
    if(track == "domain")
        return
        "Domain task: outline a new area (theme, rooms, NPCs, hub hooks).\n"
        "Domain approval marks you ready for setrole (admin still confirms).\n";
    if(track == "coding")
        return
        "Coding task: build one room and one NPC under /realms/<you>/area/\n"
        "using ed/cat/ls, then submit paths for domain review and coding check.\n";
    return "";
}

void show_brief() {
    string track;

    track = (string)APPRENTICE_D->query_track(this_player());
    if(!track) {
        write("No track chosen yet.\n");
        return;
    }
    write(track_brief(track));
}

mixed track_keys(string track) {
    if(track == "rp")
        return ({ "title", "campaign", "giver", "start_location", "desc",
                  "objectives", "rewards", "rooms" });
    if(track == "domain")
        return ({ "title", "area_name", "theme", "rooms", "npcs", "hooks" });
    if(track == "coding")
        return ({ "title", "room_path", "npc_path", "notes" });
    return ({});
}

mixed track_labels(string track) {
    if(track == "rp")
        return ({
            "Quest title",
            "Campaign name",
            "Giver NPC id/name",
            "Start location hint",
            "Description (player-facing)",
            "Objectives (free text)",
            "Reward notes",
            "Involved rooms / areas"
        });
    if(track == "domain")
        return ({
            "Outline title",
            "Domain / area name",
            "Theme",
            "Room list",
            "NPC list",
            "Hooks to existing hubs"
        });
    if(track == "coding")
        return ({
            "Build title",
            "Room path under /realms/<you>/area/",
            "NPC path under /realms/<you>/area/",
            "Test notes"
        });
    return ({});
}

void start_draft_prompts() {
    string track;
    mapping existing;
    mapping body;
    string *bkeys;
    int bi;

    track = (string)APPRENTICE_D->query_track(this_player());
    prompt_keys = track_keys(track);
    prompt_labels = track_labels(track);
    draft_fields = ([]);
    existing = (mapping)APPRENTICE_D->get_draft(this_player());
    if(existing) {
        if(existing["title"]) draft_fields["title"] = existing["title"];
        body = existing["body"];
        if(mapp(body)) {
            bkeys = keys(body);
            for(bi = 0; bi < sizeof(bkeys); bi++)
                draft_fields[bkeys[bi]] = body[bkeys[bi]];
        }
        write("Editing existing draft. Press enter to keep a value.\n");
    }
    prompt_index = 0;
    next_prompt();
}

void next_prompt() {
    string key;
    string label;
    string cur;

    if(prompt_index >= sizeof(prompt_keys)) {
        if(!(int)APPRENTICE_D->save_draft(this_player(), draft_fields)) {
            write("Failed to save draft.\n");
        } else {
            write("Draft saved. Use option 4 to submit when ready.\n");
        }
        show_menu();
        input_to("handle_choice");
        return;
    }
    key = prompt_keys[prompt_index];
    label = prompt_labels[prompt_index];
    cur = draft_fields[key];
    if(cur && sizeof(cur))
        write(label + " [" + cur + "]: ");
    else
        write(label + ": ");
    input_to("handle_prompt");
}

void handle_prompt(string str) {
    string key;
    string cur;

    key = prompt_keys[prompt_index];
    cur = draft_fields[key];
    if((!str || !sizeof(str)) && cur && sizeof(cur))
        str = cur;
    if(!str || !sizeof(str)) {
        write("A value is required (or keep the previous one).\n");
        next_prompt();
        return;
    }
    draft_fields[key] = str;
    prompt_index++;
    next_prompt();
}

void do_submit() {
    int r;

    r = (int)APPRENTICE_D->submit(this_player());
    if(r == 1) {
        write("Submitted to the domain review queue.\n");
        return;
    }
    if(r == -1) {
        write("Draft needs a title. Use option 3 to edit.\n");
        return;
    }
    if(r == -2) {
        write("Draft body is empty. Use option 3 to edit.\n");
        return;
    }
    write("Nothing to submit. Draft a submission first (option 3).\n");
}

void show_status() {
    string pname;
    mapping st;
    string *ids;
    mapping sub;
    int i;
    string id;

    pname = (string)this_player()->query_name();
    st = (mapping)APPRENTICE_D->query_player_state(pname);
    write("\n=== Your apprenticeship status ===\n");
    if(!st) {
        write("No submissions yet.\n");
        return;
    }
    write("Track: " + (st["track"] ? st["track"] : "(none)") + "\n");
    write("Ready for setrole: " +
          (st["ready_for_setrole"] ? "YES (admin must still setrole)" : "no") +
          "\n");
    ids = (string *)APPRENTICE_D->list_author_submissions(pname);
    if(!ids || !sizeof(ids)) {
        write("No submissions on file.\n");
        return;
    }
    for(i = 0; i < sizeof(ids); i++) {
        id = ids[i];
        sub = (mapping)APPRENTICE_D->get_submission(id);
        if(!sub) continue;
        write("- " + id + " [" + sub["status"] + "] " + sub["title"] + "\n");
        if(sub["status"] == "rejected" && sub["reject_reason"])
            write("  reason: " + sub["reject_reason"] + "\n");
    }
}
