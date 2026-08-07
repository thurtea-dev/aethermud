/* /std/rifts_npc.c
   Base inheritable for Rifts NPCs with keyword-response dialogue.
   Child NPCs call add_response(keyword, response) in their create().
   Features: keyword dialogue, repeat-visitor memory, wander, attack challenge,
             quest giver stub, faction-based aggression. */

#include <std.h>
#include <daemons.h>

inherit MONSTER;

private mapping __responses;
private string __default_response;
private string __npc_name_str;
private string __repeat_greeting;
private string __quest_name;
private string __npc_faction;
private int    __wanders;
private int    __challenged;
private int    __no_aggro;

void set_npc_name(string str) {
    __npc_name_str = str;
}

string query_npc_name() {
    if(__npc_name_str) return __npc_name_str;
    return query_cap_name();
}

/* Some NPC response strings (authoring mistake, ~60 files) already
   bake in their own "<Name> says: " or "<Name> says, '...'" prefix.
   say_here() below always adds its own "<npc name> says: " prefix, so
   an unstripped baked-in prefix doubles up. Strip a redundant leading
   prefix of that shape before adding ours. Only scans the first
   PREFIX_SCAN_LIMIT characters so genuine dialogue that happens to
   contain the word "says" further into the line is left untouched. */
#define PREFIX_SCAN_LIMIT 40

private string strip_redundant_prefix(string msg) {
    int colon_pos, comma_pos;
    int scan_len;
    string rest;

    if(!msg || !strlen(msg)) return msg;

    scan_len = strlen(msg);
    if(scan_len > PREFIX_SCAN_LIMIT) scan_len = PREFIX_SCAN_LIMIT;

    colon_pos = strsrch(msg[0..scan_len-1], " says: ");
    comma_pos = strsrch(msg[0..scan_len-1], " says, ");

    if(comma_pos != -1 && (colon_pos == -1 || comma_pos < colon_pos)) {
        rest = msg[comma_pos + strlen(" says, ")..];
        if(strlen(rest) && rest[0] == '\'') {
            rest = rest[1..];
            if(strlen(rest) && rest[strlen(rest)-1] == '\'')
                rest = rest[0..strlen(rest)-2];
        }
        return rest;
    }
    if(colon_pos != -1)
        return msg[colon_pos + strlen(" says: ")..];

    return msg;
}

void say_here(string msg) {
    if(!environment(this_object())) return;
    msg = strip_redundant_prefix(msg);
    tell_room(environment(this_object()),
        query_npc_name() + " says: " + msg + "\n",
        ({ this_object() }));
}

void add_response(string keyword, string response) {
    if(!keyword || !stringp(keyword)) return;
    __responses[lower_case(keyword)] = response;
}

void set_default_response(string str) {
    __default_response = str;
}

void set_repeat_greeting(string str) {
    __repeat_greeting = str;
}

void set_wanders(int flag) {
    __wanders = flag;
}

void set_quest_name(string str) {
    __quest_name = str;
}

string query_quest_name() {
    return __quest_name;
}

void set_npc_faction(string str) {
    __npc_faction = str;
}

string query_npc_faction() {
    return __npc_faction;
}

/* Call set_no_aggro(1) on vendors and quest NPCs to prevent faction
   standing from triggering combat even if assigned a faction. */
void set_no_aggro(int flag) {
    __no_aggro = flag;
}

int query_no_aggro() {
    return __no_aggro;
}

void create() {
    ::create();
    __responses        = ([]);
    __default_response = 0;
    __repeat_greeting  = 0;
    __quest_name       = 0;
    __npc_faction      = 0;
    __wanders          = 0;
    __challenged       = 0;
    __no_aggro         = 0;
    add_response("hello", "");
    add_response("hi",    "");
    add_response("help",  "");
    add_response("bye",   "");
}

void init() {
    string pname;

    ::init();
    if(__no_aggro) return;
    if(!__npc_faction || !strlen(__npc_faction)) return;
    if(!this_player() || !interactive(this_player())) return;
    if((int)this_player()->query_ghost()) return;
    pname = lower_case((string)this_player()->query_name());
    if((int)FACTION_D->is_hostile(pname, __npc_faction))
        kill_ob(this_player(), 0);
}

void do_wander() {
    string *exits, dest;
    object env;
    int idx;

    env = environment(this_object());
    if(!env) return;
    exits = (string *)env->query_exits();
    if(!exits || !sizeof(exits)) return;
    idx  = random(sizeof(exits));
    dest = (string)env->query_exit(exits[idx]);
    if(!dest) return;
    this_object()->move(dest);
}

void heart_beat() {
    object attacker;

    ::heart_beat();

    if(!__challenged) {
        attacker = query_current_attacker();
        if(attacker) {
            say_here("Halt.  State your business.");
            __challenged = 1;
        }
    }

    if(__wanders && !random(8)) {
        do_wander();
    }
}

void catch_tell(string str) {
    string a, b, matched;
    string *ks;
    string met, sender;
    int i;

    if(sscanf(str, "%s says, \"%s\"", a, b) != 2) return;
    sender = lower_case(a);
    b      = lower_case(b);

    ks      = keys(__responses);
    i       = sizeof(ks);
    matched = "";
    while(i--) {
        if(!strlen(ks[i])) continue;
        if(strsrch(b, ks[i]) != -1) {
            matched = ks[i];
            break;
        }
    }

    if(strlen(matched)) {
        if(strlen(__responses[matched])) {
            if((matched == "hello" || matched == "hi") && __repeat_greeting) {
                met = (string)query_property("met_players");
                if(met && strsrch(met, sender) != -1) {
                    call_out("say_here", 1, __repeat_greeting);
                    return;
                }
                set_property("met_players",
                    (met && strlen(met)) ? met + "," + sender : sender);
            }
            call_out("say_here", 1, __responses[matched]);
        }
        return;
    }

    if(__default_response && strlen(__default_response))
        call_out("say_here", 1, __default_response);
}
