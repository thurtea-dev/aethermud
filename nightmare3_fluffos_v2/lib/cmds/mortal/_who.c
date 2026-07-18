// /cmds/mortal/_who.c
// Rifts MUD who command.
// Players see: staff tags + population indicator only.
// Wizards see: full player list with race and current room.
// who <name> is wizard-only.

#include <std.h>

inherit DAEMON;

string get_role_tag(object ob);
string format_wiz_row(object ob, int use_color);
string format_player_row(object ob, int use_color, int is_arch);
string format_uptime();
void show_single(string name);

int cmd_who(string str) {
    object *all_u, *wizards, *players;
    string out, term, pop_str, row;
    int i, wiz_count, player_count, use_color, is_wiz, is_arch;

    term = (string)this_player()->getenv("TERM");
    use_color = (term && term != "unknown" && term != "plain");
    is_wiz = creatorp(this_player());
    is_arch = archp(this_player());

    all_u = users();
    wizards = ({});
    players = ({});
    i = sizeof(all_u);
    while (i--) {
        if (!all_u[i]) continue;
        if (creatorp(all_u[i])) {
            if (!(int)all_u[i]->query_invis() || is_wiz)
                wizards += ({ all_u[i] });
        } else {
            players += ({ all_u[i] });
        }
    }
    wiz_count = sizeof(wizards);
    player_count = sizeof(players);

    // Subcommands restricted to wizards
    if (str && str != "") {
        str = lower_case(str);

        if (!is_wiz) {
            write("Type 'who' to see staff and population.\n");
            return 1;
        }

        if (str == "wizards") {
            if (use_color)
                out = "%^BOLD%^=== RIFTS EARTH MUD ===%^RESET%^\n\n";
            else
                out = "=== RIFTS EARTH MUD ===\n\n";
            if (wiz_count) {
                if (use_color)
                    out += "%^BOLD%^--- Staff Online ---%^RESET%^\n";
                else
                    out += "--- Staff Online ---\n";
                for (i = 0; i < wiz_count; i++)
                    out += format_wiz_row(wizards[i], use_color) + "\n";
            } else {
                out += "No staff currently online.\n";
            }
            out += "\n" + wiz_count + " wizard" + (wiz_count == 1 ? "" : "s") +
                   " online.  " + format_uptime() + "\n";
            this_player()->more(explode(out, "\n"));
            return 1;
        }

        if (str == "players") {
            if (use_color)
                out = "%^BOLD%^=== RIFTS EARTH MUD ===%^RESET%^\n\n";
            else
                out = "=== RIFTS EARTH MUD ===\n\n";
            if (use_color)
                out += "%^BOLD%^--- Players ---%^RESET%^\n";
            else
                out += "--- Players ---\n";
            if (player_count) {
                for (i = 0; i < player_count; i++) {
                    row = format_player_row(players[i], use_color, is_arch);
                    if (row && strlen(row)) out += row + "\n";
                }
            } else {
                out += "No players currently online.\n";
            }
            out += "\n" + player_count + " player" + (player_count == 1 ? "" : "s") +
                   " online.  " + format_uptime() + "\n";
            this_player()->more(explode(out, "\n"));
            return 1;
        }

        if (str != "all") {
            show_single(str);
            return 1;
        }
    }

    // Population string based on non-wizard player count
    if (player_count == 0)
        pop_str = "Population: Empty";
    else if (player_count <= 5)
        pop_str = "Population: Low";
    else if (player_count <= 15)
        pop_str = "Population: Medium";
    else
        pop_str = "Population: High";

    if (!is_wiz) {
        // Player view: header + population indicator + staff list only
        if (use_color)
            out = "%^BOLD%^=== RIFTS EARTH MUD ===%^RESET%^\n\n";
        else
            out = "=== RIFTS EARTH MUD ===\n\n";
        out += pop_str + "\n\n";
        if (wiz_count) {
            if (use_color)
                out += "%^BOLD%^Staff online:%^RESET%^\n";
            else
                out += "Staff online:\n";
            for (i = 0; i < wiz_count; i++)
                out += format_wiz_row(wizards[i], use_color) + "\n";
        } else {
            out += "No staff currently online.\n";
        }
        this_player()->more(explode(out, "\n"));
        return 1;
    }

    // Wizard view: full list with population count, staff, and players
    if (use_color)
        out = "%^BOLD%^=== RIFTS EARTH MUD ===%^RESET%^\n\n";
    else
        out = "=== RIFTS EARTH MUD ===\n\n";
    out += pop_str + "  (" + player_count + " player" +
           (player_count == 1 ? "" : "s") + " online)\n\n";

    if (wiz_count) {
        if (use_color)
            out += "%^BOLD%^Staff:%^RESET%^\n";
        else
            out += "Staff:\n";
        for (i = 0; i < wiz_count; i++)
            out += format_wiz_row(wizards[i], use_color) + "\n";
        out += "\n";
    } else {
        out += "No staff currently online.\n\n";
    }

    if (use_color)
        out += "%^BOLD%^Players:%^RESET%^\n";
    else
        out += "Players:\n";
    if (player_count) {
        for (i = 0; i < player_count; i++) {
            row = format_player_row(players[i], use_color, is_arch);
            if (row && strlen(row)) out += row + "\n";
        }
    } else {
        out += "No players currently online.\n";
    }
    out += "\n" + format_uptime() + "\n";
    this_player()->more(explode(out, "\n"));
    return 1;
}

string get_role_tag(object ob) {
    string role;

    role = (string)ob->getenv("wiz_role");
    if (!role || !strlen(role)) return "[Wiz]";
    if (role == "admin")  return "[Admin-Wiz]";
    if (role == "domain") return "[Domain-Wiz]";
    if (role == "coding") return "[Coding-Wiz]";
    if (role == "rp")     return "[RP-Wiz]";
    if (role == "apprentice") return "[Apprentice]";
    return "[Wiz]";
}

string format_wiz_row(object ob, int use_color) {
    string tag, name, row, color, wt;
    int idle;

    tag = get_role_tag(ob);
    name = (string)ob->query_cap_name();
    if (!name || !strlen(name) || name == "0")
        name = capitalize((string)ob->query_name());
    /* whotitle suffix: explicit env wins; the founder position gets a
       default so the first admin reads "- The First Admin" in who. */
    wt = (string)ob->getenv("whotitle");
    if ((!wt || !strlen(wt)) &&
        lower_case((string)ob->query_position()) == "head arch")
        wt = "The First Admin";
    idle = query_idle(ob);

    if (use_color) {
        if (tag == "[Admin-Wiz]")
            color = "%^BOLD%^%^RED%^";
        else if (tag == "[Apprentice]")
            color = "%^CYAN%^";
        else
            color = "";
        row = color + arrange_string(tag, 13) + "%^RESET%^  " +
              arrange_string(name, 16);
    } else {
        row = arrange_string(tag, 13) + "  " + arrange_string(name, 16);
    }

    if (wt && strlen(wt))
        row += "- " + wt + " ";

    if (idle > 300)
        row += "(idle " + (idle / 60) + "m)";

    return row;
}

string format_player_row(object ob, int use_color, int is_arch) {
    string race, name, loc, tag, row;

    if ((int)ob->query_invis() && !is_arch) return "";

    race = (string)ob->query_race();
    if (!race || !strlen(race)) race = "unknown";
    tag = "[" + capitalize(race) + "]";

    name = (string)ob->query_cap_name();
    if (!name || !strlen(name) || name == "0")
        name = capitalize((string)ob->query_name());

    if (environment(ob) && (string)environment(ob)->query_short())
        loc = (string)environment(ob)->query_short();
    else
        loc = "unknown";

    if (use_color)
        row = "%^WHITE%^" + arrange_string(tag, 13) + "  %^RESET%^" +
              arrange_string(name, 16) + loc;
    else
        row = arrange_string(tag, 13) + "  " + arrange_string(name, 16) + loc;

    return row;
}

void show_single(string name) {
    object target;
    string race, occ, region, out;

    target = find_player(lower_case(name));
    if (!target) {
        write("No player named '" + capitalize(name) + "' is online.\n");
        return;
    }

    out = "=== " + (string)target->query_cap_name() + " ===\n";

    if (creatorp(target)) {
        out += "Role: " + get_role_tag(target) + "\n";
    } else {
        race = (string)target->query_race();
        occ = (string)target->getenv("rifts_occ");
        region = (string)target->getenv("start_region");
        if (race && strlen(race))
            out += "Race: " + capitalize(race) + "\n";
        if (occ && strlen(occ) && occ != "none")
            out += "OCC: " + capitalize(occ) + "\n";
        if (region && strlen(region))
            out += "Home region: " + region + "\n";
    }
    out += "Idle: " + (query_idle(target) / 60) + " minute" +
           (query_idle(target) / 60 == 1 ? "" : "s") + "\n";

    write(out);
}

string format_uptime() {
    int tm, x;
    string str;

    tm = uptime();
    str = "Server up: ";
    x = tm / 604800;
    if (x) { str += x + "w "; tm -= x * 604800; }
    x = tm / 86400;
    if (x) { str += x + "d "; tm -= x * 86400; }
    x = tm / 3600;
    if (x) { str += x + "h "; tm -= x * 3600; }
    x = tm / 60;
    if (x) { str += x + "m "; tm -= x * 60; }
    if (tm) str += tm + "s";
    return str;
}

void help() {
    write(
        "Syntax: who\n"
        "        who <name>    (wizard only)\n"
        "        who all       (wizard only)\n"
        "        who wizards   (wizard only)\n"
        "        who players   (wizard only)\n\n"
        "Shows who is currently online.\n\n"
        "  who           -- Players: staff list + population indicator.\n"
        "                   Wizards: full list with race and room.\n"
        "  who <name>    -- Wizard only. Detailed info for one player.\n"
        "  who all       -- Wizard only. Same as 'who' for wizards.\n"
        "  who wizards   -- Wizard only. Staff online only.\n"
        "  who players   -- Wizard only. Players online only.\n\n"
        "Wizard role tags: [Admin-Wiz] [Domain-Wiz] [Coding-Wiz] [RP-Wiz]\n"
        "                  [Apprentice] [Wiz]\n"
        "Population levels: Empty (0) / Low (1-5) / Medium (6-15) / High (16+)\n"
        "See also: finger, ooc\n"
    );
}
