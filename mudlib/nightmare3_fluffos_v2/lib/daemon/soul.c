/*    /daemon/soul.c
 *    from Nightmare IV
 *    fear a soul from Discworld
 *    created by Pinkfish@Discworld on an unknown date
 *    ported to Nightmare doing very little by Descartes of Borg 940207
 */

#include <soul.h>
#include <save.h>
#include <daemons.h>
#include <security.h>

private mapping __Soul;
static private string __Adj, __Chunk;
static private object __CurrentTarget;
static private object __CurrentViewer;

static private void become_emotional(mapping info, string adj, object *who);
static private void compile_soul(string str);
static private string compile_message(mixed *parts, object *targets);
static private string remove_whitespace(string str);
static string filter_stuff(mixed *parts, object *targets);
static private void save_soul();
static private void restore_soul();
static private string display_name_for(object ob, object viewer);
static string handle_names(object ob);
static private void compile_file(string str);
static private void parse_assoc(mapping borg);
static private mixed parse_chunk();
static private mapping parse_mapping();
static private string *parse_array();

/*  Canned single-word emotes (nod, smile, wave, and the ~208 other
    verbs across daemon/soul/*.l) were retired 2026-07-26 by explicit,
    permanent design decision: emote <description> and its : shorthand
    are now the only player action commands, no exceptions. This is
    the single point where the verb table used to get populated --
    leaving __Soul permanently empty means do_cmd() below can never
    match any verb, for every caller (std/living.c's cmd_hook, or any
    future one), with no need to also gate living.c itself. The old
    restore_soul()/file-scan loop, the parser (parse_chunk() etc),
    do_cmd()/become_emotional(), and the introduction-aware
    display_name_for()/__CurrentViewer machinery added 2026-07-25 are
    all kept intact below, not deleted, in case this is ever reversed.
    recompile_soul() (still present, below) and daemon/soul/*.l (still
    on disk) would resurrect the old verb table if called/reloaded --
    that's intentional dormancy, not an oversight, but do not wire
    either back up without an explicit decision to bring canned
    emotes back. cmds/creator/_rehash.c's old "soul" rehash case,
    which called recompile_soul(), was removed the same day so staff
    can no longer trigger that path by accident. */
void create() {
    __Soul = ([]);
}

object *match_living(string nom) {
    object *who;
    object ob;

    if(!nom || nom == "") return ({});
    if(nom == "everyone") {
        who = filter_array(all_inventory(environment(this_player())),
          (: living :));
        return who - ({ this_player() });
    }
    else if(ob = present(lower_case(nom), environment(this_player())))
      return (hiddenp(ob) || ob == this_player()) ? ({}) : ({ ob });
    else return ({});
}

varargs string match_adj(string str, mapping data1, mapping data2) {
    string a, b;

    a = (data1["adverbs"] ? data1["adverbs"] : "");
    b = (data2["adverbs"] ? data2["adverbs"] : "");
    if(str == "" && b[0] == '#' && strlen(b) > 2) b = b[2..strlen(b)-1];
    if(a[0] == '#' || b[0] == '#') return str;
    if(sscanf(sprintf(",%s,%s,", b, a), "%s,"+str+"%s,", a, b)) {
        if(b && strlen(b)) return sprintf("%s%s", str, b);
        else return str;
    }
    return 0;
}

int do_cmd(string verb, string arg) {
    mapping data, info;
    object *target;
    string *args;
    string adj;

    if(!(data = __Soul[verb])) return 0;
    target = ({});
    if(!arg) {
        info = data["noargs"];
        adj = "nothing";
        args = ({ arg = "" });
    }
    if(!info) {
        if(!args) {
            args = explode(arg, " ");
	}
        if(data["prepositions"]) args = args - (string *)data["prepositions"];
        arg = implode(args, " ");
        if(sizeof(args) == 1) {
            if(!sizeof(target = match_living(arg))) {
                if(!(info = data["undirected"])) {
                    message("emote", sprintf("%s whom?", capitalize(verb)),
                      this_player());
                    return 1;
                }
                adj = match_adj(arg, data, info);
            }
            else {
                if(!(info = data["directed"]))
                  return notify_fail("That makes no sense.\n");
                adj = match_adj("", data, info);
            }
        }
        else {
            if(sizeof(target = match_living(args[0]))) {
                if(!(info = data["directed"]))
                  return notify_fail("That makes no sense.\n");
		adj = match_adj(implode(args[1..sizeof(args)-1]," "),data,info);
            }
            else {
                if(sizeof(target = match_living(args[sizeof(args)-1]))) {
                    if(!(info = data["directed"]))
                      return notify_fail("That makes no sense.\n");
                    adj = match_adj(implode(args[0..sizeof(args)-2], " "),
                      data, info);
                }
                else {
                    if(!(info = data["undirected"])) {
                        message("emote", sprintf("%s whom?", capitalize(verb)),
                          this_player());
                        return 1;
                    }
                    adj = match_adj(arg, data, info);
                }
            }
        }
    }
    if(!adj) {
        message("emote", sprintf("What an odd way in which to %s!", verb),
          this_player());
	return 1;
    }
    become_emotional(info, adj, target);
    return 1;
}

/*  Every recipient of an emote (the performer, each bystander, each
    target) gets its OWN compiled string instead of one string shared
    across the whole room -- because $TP_NAME (the performer) and
    $OB_NAME (the target(s)) must each resolve through that specific
    recipient's own introduction state, not the performer's. Mirrors
    the personalized per-viewer loop cmds/mortal/_emote.c already uses
    for freeform emotes. __CurrentViewer tells handle_names()/
    handle_part() whose knows_player() to check for this pass. */
void become_emotional(mapping info, string adj, object *who) {
    object *inv;
    object viewer;
    int i;

    __Adj = adj;
    __CurrentTarget = 0;
    __CurrentViewer = this_player();
    message("emote", compile_message(info["mymsg"], who), this_player());

    inv = all_inventory(environment(this_player()));
    for(i = 0; i < sizeof(inv); i++) {
        viewer = inv[i];
        if(!viewer || !living(viewer) || viewer == this_player()) continue;
        if(who && sizeof(who) && member_array(viewer, who) != -1) continue;
        __CurrentTarget = 0;
        __CurrentViewer = viewer;
        message("emote", compile_message(info["theirmsg"], who), viewer);
    }

    i = sizeof(who);
    while(i--) {
        __CurrentTarget = who[i];
        __CurrentViewer = who[i];
        if(info["obmsg"])
          message("emote", compile_message(info["obmsg"], who), who[i]);
        else message("emote", compile_message(info["theirmsg"], who), who[i]);
      }
  }

void compile_soul(string str) {
    compile_file(sprintf("%s/%s", DIR_SOUL_FILES, str));
}

string compile_message(mixed *parts, object *targets) {
    return filter_stuff(parts, targets);
  }

string compile_names(object *who) {
    int x;

    if((x = sizeof(who)) == 1) return handle_names(who[0]);
    else return implode(map_array(who[1..x-1], "handle_names", this_object()),
      ", ") + " and " + handle_names(who[0]);
  }

static void save_soul() {
    save_object(SAVE_SOUL);
}

static private void restore_soul() {
    restore_object(SAVE_SOUL);
}

string filter_stuff(mixed *parts, object *targets) {
    string *arr;

    arr = map_array(parts, "handle_part", this_object(), targets) - ({ "" });
    return implode(arr, " ");
  }

/*  NPCs (std/monster.c) have no query_display_name()/knows_player() --
    they are not part of the player introduction system -- so fall
    back to their plain cap name for any non-player object. */
static private string display_name_for(object ob, object viewer) {
    if(!ob) return "";
    if(!viewer || !function_exists("query_display_name", ob))
        return (string)ob->query_cap_name();
    return (string)ob->query_display_name(viewer);
  }

string handle_names(object ob) {
    if(!ob) return "";
    if(ob == __CurrentTarget) return "you";
    else return display_name_for(ob, __CurrentViewer);
  }

string handle_part(mixed part, object *targets) {
    if(stringp(part)) return part;
    else switch(part) {
    case ADJ: return __Adj;
    case TP_NAME: return display_name_for(this_player(), __CurrentViewer);
    case TP_PRONOUN: return nominative(this_player());
    case TP_POSSESSIVE: return possessive(this_player());
    case TP_NPOSS: return possessive_noun(this_player());
    case TP_FOO: return reflexive(this_player());
    case OB_NAME: return compile_names(targets);
    case OB_PRONOUN: return nominative(targets[0]);
    case OB_POSSESSIVE: return possessive(targets[0]);
    case OB_NPOSS: return possessive_noun(targets[0]);
    case OB_FOO: return reflexive(targets[0]);
    }
    return part + "";
  }

mapping query_soul() { return copy(__Soul); }
 
static private void compile_file(string file) { 
    string *cles; 
 
    if(!(file = read_file(file))) error("Failed to read file.\n"); 
    file = replace_string(file, "\t", "");
    cles = map_array(explode(file, "\n"), "remove_indentation", this_object()); 
    filter_array(explode(implode(cles, ""), "$$"), "decode_file",this_object()); 
  } 
 
static void decode_file(string str) {
    call_out("define_action", 0, str);
}
 
static void define_action(string str) { 
    __Chunk = remove_whitespace(str);
    parse_assoc(__Soul); 
  } 
 
static private void parse_assoc(mapping borg) { 
    string str; 
    mixed val; 
 
    if(sscanf(__Chunk, "(%s %s", str, __Chunk) < 2)  
      error("Syntax error compiling file in parse_assoc().\n"); 
    val = parse_chunk(); 
    __Chunk = __Chunk[1..strlen(__Chunk)-1]; 
    borg[str] = val; 
  } 
 
static private mixed parse_chunk() { 
    string str; 
 
    __Chunk = remove_whitespace(__Chunk);
    switch(__Chunk[0]) { 
    case '*': return parse_mapping(); 
    case '[': return parse_array(); 
    case '(': return error("Syntax error compiling file in parse_chunk().\n"); 
    } 
    if(sscanf(__Chunk, "%s)%s", str, __Chunk) == 2) return str; 
    error("sscanf() failed in parse_chunk().\n"); 
  } 
 
static private mapping parse_mapping() { 
    mapping borg; 
    int i; 
 
    borg = ([]); 
    if(!sscanf(__Chunk, "*(%s", __Chunk)) 
        error("Failed: sscanf() in parse_mapping().\n"); 
    while((__Chunk = remove_whitespace(__Chunk))[0] != ')') parse_assoc(borg);
    __Chunk = __Chunk[1..strlen(__Chunk)-1]; 
    return borg; 
  } 
 
static private string *parse_array() { 
    string str; 
 
    if(sscanf(__Chunk, "[%s]%s", str, __Chunk) != 2) 
      error("Failed: sscanf() in parse_array().\n"); 
    return map_array(explode(str, " "), "fix_macros", this_object()); 
  } 
 
static mixed fix_macros(string foo) { 
    if(foo[0] == '$') { 
        switch(foo) { 
        case "$ADJ": return ADJ; 
        case "$TP_NAME": return TP_NAME; 
        case "$OB_NAME": return OB_NAME; 
        case "$TP_PRONOUN": return TP_PRONOUN; 
        case "$OB_PRONOUN": return OB_PRONOUN; 
        case "$TP_POSS": return TP_POSSESSIVE; 
        case "$OB_POSS": return OB_POSSESSIVE; 
        case "$TP_NPOSS": return TP_NPOSS; 
        case "$OB_NPOSS": return OB_NPOSS; 
        case "$TP_FOO": return TP_FOO; 
        case "$OB_FOO": return OB_FOO; 
	} 
      } 
    return foo; 
  } 
 
static string remove_indentation(string str) { 
    str = remove_whitespace(str);
    if(!(['(': 1, '[':1, '*':1 ])[str[0]]) return str; 
    else return sprintf(" %s", str); 
  } 

void recompile_soul() {
    string *files;
    int i;

    __Soul = ([]);
    i = sizeof(files = get_dir(DIR_SOUL_FILES+"/"));
    while(i--) compile_soul(files[i]);
    save_soul();
}

static private string remove_whitespace(string str) {
    while(sscanf(str, "    %s", str));
    sscanf(str, "  %s", str);
    sscanf(str, " %s", str);
    return str;
}
