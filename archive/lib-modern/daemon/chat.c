/*    /daemon/chat.c
 *    from the Nightmare 3.2 Mudlib
 *    daemon to handle all mud chat channels
 *    created by Descartes of Borg 931220
 */
#define DEBUG(x) tell_object(find_player("descartes"), x)

#include <std.h>
#include <network.h>
#include <daemons.h>
#include <objects.h>

inherit DAEMON;

#define MAGENTA "%^MAGENTA%^"
#define RESET "%^RESET%^"

private nosave string *staff_chans = ({
    "newbie", "cre", "gossip", "admin", "error",
    "priest", "mage", "explorer", "thief", "fighter", "death",
    "connections", "monk", "rogue", "cleric", "kataan", "hm",
    "intercre", "intergossip", "council"
});

private nosave mapping channels;

int list_channel(string str);
private string normalize_channel(string verb);
private int channel_allowed(object who, string ch);

private string normalize_channel(string verb) {
    if(sscanf(verb, "%semote", verb)) { }
    return verb;
}

private int channel_allowed(object who, string ch) {
    string base;

    if(!who || !userp(who)) return 0;
    base = normalize_channel(ch);
    if(!base || !sizeof(base)) return 0;
    if(!creatorp(who)) return 0;
    if(base == "admin") return archp(who);
    if(base == "cre" || base == "intercre") return creatorp(who);
    if(base == "gossip" || base == "intergossip")
        return creatorp(who) || ambassadorp(who);
    if(base == "council") return creatorp(who) || leaderp(who);
    if(member_array(base, ({ "newbie", "hm", "fighter", "mage", "monk",
        "rogue", "cleric", "kataan", "priest", "explorer", "thief" })) != -1)
        return creatorp(who);
    return 0;
}

void create() {
    string *chans;
    object *who;
    int i, j;

    daemon::create();
    set_no_clean(1);
    channels = ([]);
    i = sizeof(staff_chans);
    while(i--)
        if(!channels[staff_chans[i]]) channels[staff_chans[i]] = ({});
    i = sizeof(who = users());
    while(i--) {
        j = sizeof(chans = (string *)who[i]->query_channels());
        while(j--) {
            if(!channels[chans[j]]) channels[chans[j]] = ({});
            channels[chans[j]] = distinct_array(channels[chans[j]]+({who[i]}));
        }
    }
}

void add_user(string *chans) {
    object ob;
    int i;

    if(!userp(ob = previous_object())) return;
    i = sizeof(chans);
    while(i--) { 
        if(!channels[chans[i]]) channels[chans[i]] = ({});
         channels[chans[i]] = distinct_array(channels[chans[i]]+({ob}));
    }
}

void remove_user(string *chans) {
    object ob;
    int i;

    if(!userp(ob = previous_object())) return;
    i = sizeof(chans);
    while(i--) {
        if(!channels[chans[i]]) continue;
        else channels[chans[i]] -= ({ ob });
        if(!sizeof(channels[chans[i]])) map_delete(channels, chans[i]);
    }
}

int do_chat(string verb, string str) {
    string msg;
    int emote;
    string name;
    string full_input;

    if(!channel_allowed(this_player(), verb)) return 0;
    if(verb == "list") return list_channel(str);
    full_input = (str && sizeof(str)) ? (verb + " " + str) : verb;
    if(!channels[verb]) {
        if(sscanf(verb, "%semote", verb)) emote = 1;
        else {
            if((int)"/cmds/mortal/_cast"->try_spell_shortcut(full_input)) return 1;
            return 0;
        }
        if(!channels[verb]) {
            if((int)"/cmds/mortal/_cast"->try_spell_shortcut(full_input)) return 1;
            return 0;
        }
    }
    if(member_array(this_player(), channels[verb]) == -1) return 0;
    if(!str) {
        this_player()->set_blocked(verb);
        return 1;
    }
    if((int)this_player()->query_blocked(verb)) {
        if((int)this_player()->query_blocked("all")) {
            message(verb, "You cannot chat while totally blocked.", this_player());
            return 1;
        }
        this_player()->set_blocked(verb);
    }
    if(verb == "admin" || verb=="cre") {
        if(!(name = (string)this_player()->query_CapName()))
          name = capitalize((string)this_player()->query_name());
    }
    else name=this_player()->query_cap_name();
    if(emote) msg = sprintf("%s<%s>%s %s %s", MAGENTA, verb, RESET,
      name, str);
    else msg = sprintf("%s %s<%s>%s %s", name, MAGENTA, verb, RESET, str);
    message(verb, msg, channels[verb]);
    if(member_array(verb, INTERMUD_CHANNELS) != -1){
      //SERVICES_D->send_gwizmsg(str, emote);
      SERVICES_D-> eventSendChannel(name, "imud_code", str, 0);
    }
    return 1;
}

void inter_channels(string who, string verb, string str, int emote) {
    string msg;

    if(!channels[verb] || base_name(previous_object()) != SERVICES_D) return;
    if(emote)
    msg = sprintf("%s<%s>%s %s %s", MAGENTA, verb, RESET, who, str);
    else msg = sprintf("%s %s<%s>%s %s", who, MAGENTA, verb, RESET, str);
    message(verb, msg, channels[verb]);
}

int list_channel(string str) {
    string list;
    int i;

    if(!channel_allowed(this_player(), str)) return 0;
    if(!channels[str]) return 0;
    if(member_array(this_player(), channels[str]) == -1) return 0;
    list = "";
    list = "Online: ";
    i =sizeof(channels[str]);
    while(i--) {
        if(!channels[str][i]) continue;
        if(hiddenp(channels[str][i]) || (int)channels[str][i]->query_invis() ||
          (int)channels[str][i]->query_blocked(str)) continue;
        list += "    "+(string)channels[str][i]->query_cap_name();
    }
    message("info", list, this_player());
    return 1;
}

