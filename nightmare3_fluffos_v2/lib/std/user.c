//	/std/user.c
//	from the Nightmare mudlib
//	the user object
//	based on the user.c created by Sulam and Buddha @TMI
//	rewritten by Descartes of Borg september 1992
//      Pallando (93-05-27) Added multiple character handling.
//      Pallando (93-06-17) Removed call to living::setup()
//	Pallando (94-09-20) Changed harassment to new system


#include <std.h>
#include <config.h>
#include <rooms.h>
#include <security.h>
#include <daemons.h>
#include <move.h>
#include <objects.h>
#include <money.h>
#include <flags.h>
#include <dirs.h>
#include <council.h>

inherit AUTOSAVE;
inherit EDITOR;
inherit FILES;
inherit NMSH;
inherit MORE;
inherit REFS;
inherit LIVING;


static string *__IgnoreMsgClass;
#define DEATH_MSGS ({\
sprintf("A cold wind sweeps across %s, grieving the loss of %s.", mud_name(), query_cap_name()),\
sprintf("You hear the faint howl of %s death from far away.", possessive_noun(query_cap_name())),\
sprintf("%s mourns the tragic death of %s.", mud_name(), query_cap_name()),\
sprintf("The bells of %s toll for the death of %s.", mud_name(), query_cap_name()),\
sprintf("The world about you darkens a moment at the death of %s.", query_cap_name()),\
})

int platinum, gold, electrum, silver, copper;

int player_age;
int __NoQuit;
int level, ghost, rolls, verbose_moves;
int birth;
static int disable, time_of_login;
static int __LogHarass;
static mixed *__MessageCue;
static string __Client;
static int __LastAged;
mapping blocked, news;
static mapping __LastError;
static	status	snoop, earmuffs;
string	char_name, real_name, email, ip, last_on, password, race, original_site;
private string position, primary_start, *__RestrictedChannels;
private int __WhereBlock;
private static string *channels;
mapping mini_quests;
string *quests;
string *mysites;
string guild;
string	*message;
static string *__UserId;
string married;
mixed *current_marriage, *divorced;
static string net_died_here;
static mapping term_info;
static object died_here;
string rp_title;
string *face_list;

int query_where_block();
int set_where_block();
int query_blocked(string str);
int set_mini_quest(string str, int x, string desc);
int set_quest(string str);
string *query_mini_quests();
int query_log_harass();
void reset_terminal();
mapping get_mini_quest_map();
string *query_quests();
void refresh_channels();
private static register_channels();
string *query_mysites();
void set_mysites(string *str);
void set_guild(string str);
void set_position(string pos);
void set_id(string *bogus);
void set_level(int x);
void set_race(string res);
string query_race();
nomask int query_level();
string query_guild();
void get_email(string e);
int query_ghost();
int query_verbose();
void add_message(string str);
void write_messages();
string query_email();
nomask string query_position();
string query_rp_title();
void set_rp_title(string t);
string *query_face_list();
void show_sbar();
void add_to_face_list(string name);
void remove_from_face_list(string name);
int knows_player(string name);
string query_display_name(object viewer);
string query_visible_name(object viewer);
void rifts_death_pvp_check();
private void ensure_wiz_tools();
void set_primary_start(string str);
string query_primary_start();
void save_logout_start();
int set_start_here();

void get_email(string e) {
    string who, where;

    write("\n");
    if( sscanf(e, "%s@%s", who, where) != 2 ) {
	write("Sorry, email must be in the form of user@host");
	message("prompt", "Please reenter your email address: ", this_player());
	input_to("get_email");
	return;
    }
    email = e;
}

int query_where_block() {
    return __WhereBlock;
}

int set_where_block() {
    __WhereBlock = __WhereBlock ? 0 : 1;
    return __WhereBlock;
}

void describe_current_room(int verbose) {
    object env;
    string borg;
    mixed tmp;
    int light;
    string *exits_list;
    string exit_str;
    string exit_word;
    int exit_count;

    env = environment(this_object());
    if(!env){ 
    move_object(ROOM_START);
    env = environment(this_object());
    }
    if(creatorp(this_object())) borg = file_name(env)+"\n";
    else borg = "";
    if((light=effective_light(this_object())) > 6 || light < 1) {
	if(light > 6) borg += "It is too bright to see.";
	else if(light > -2) borg += "It is dark.";
	else if(light > -4) borg += "It is quite dark.";
	else if(light > -6) borg += "It is very dark.";
	else borg += "It is completely dark.";
	message("room_description", borg, this_object());
	return;
    }
    else if(light > 3) borg += "It is really bright.\n";
    /* verbose>=2: forced verbose (explicit look, always shows long desc).
       verbose==1: movement/auto; respect brief mode via verbose_moves.
       verbose==0: brief mode, always show short. */
    borg += ((verbose >= 2 || (verbose && verbose_moves)) ?
      (string)env->query_long(0)+" " :
      (string)env->query_short());
    message("room_description", borg, this_object());
    exits_list = (string *)env->query_exits();
    if(!exits_list || !sizeof(exits_list)) {
        message("room_exits", "There are no exits.", this_object());
    } else if(sizeof(exits_list) == 1) {
        message("room_exits", "There is one exit: " + exits_list[0], this_object());
    } else {
        exit_count = sizeof(exits_list);
        switch(exit_count) {
        case  2: exit_word = "two";    break;
        case  3: exit_word = "three";  break;
        case  4: exit_word = "four";   break;
        case  5: exit_word = "five";   break;
        case  6: exit_word = "six";    break;
        case  7: exit_word = "seven";  break;
        case  8: exit_word = "eight";  break;
        case  9: exit_word = "nine";   break;
        case 10: exit_word = "ten";    break;
        default: exit_word = sprintf("%d", exit_count); break;
        }
        exit_str = "There are " + exit_word + " exits: " +
                   implode(exits_list, ", ");
        message("room_exits", exit_str, this_object());
    }
    if((tmp=(string)env->describe_living_contents(({this_object()})))!="") {
        if(strlen(tmp) > 0 && tmp[strlen(tmp)-1] == '\n')
            tmp = tmp[0..strlen(tmp)-2];
        message("living_item", tmp, this_object());
    }
    if((tmp=(string)env->describe_item_contents(({})))!="")
	message("inanimate_item", tmp, this_object());
}

void basic_commands() {
    add_action("quit", "quit");
}

int query_verbose() { return verbose_moves; }

int set_brief() {
    verbose_moves = !verbose_moves;
    return 1;
}

varargs void move_player(mixed dest, string msg) {
    object prev;
    object new_env;
    object *inv;
    string here,going,temp1,temp2,temp3,vname;
    int i, illum, bzbd, adj, tmp, ln_skill;
    int prowl_hidden, pskill, plevel, proll;

    prev = environment( this_object() );
    here = file_name(prev);
    if(stringp(dest) && dest[0] != '/') dest = "/"+dest;
    if(move(dest) != MOVE_OK) {
	message("system","You remain where you are.", this_object());
	return;
    }
    /* land navigation passive: auto-orient in wilderness rooms */
    new_env = environment(this_object());
    if(objectp(new_env) &&
       ((int)new_env->query_property("wilderness") ||
        (int)new_env->query_property("is_wilderness"))) {
        ln_skill = (int)query_skill("land navigation");
        if(ln_skill > 0 && random(100) + 1 <= ln_skill)
            message("my_action",
                "Your land navigation sense orients you automatically "
                "to your surroundings.",
                this_object());
    }
    if(query_property("stealth"))
	set_property("stealth", (int)query_property("stealth")-1);
    if(adj = query_property("stealth")) {
	add_skill_points("stealth", adj);
	adj += query_stats("dexterity");
	add_sp(-1);
    }
    /* Prowl mode: one Prowl skill roll per move while is_sneaking is
       set (prowl/sneak/hide commands). Success hides this move's
       departure and arrival messages; failure shows them normally.
       The mode itself only ends on attack, speech, or toggling off. */
    prowl_hidden = 0;
    if((int)query_property("is_sneaking")) {
	pskill = (int)query_skill("prowl");
	if(pskill > 0) {
	    plevel = query_level();
	    if(plevel < 1) plevel = 1;
	    pskill += (plevel - 1) * 5;
	    if(pskill > 98) pskill = 98;
	    pskill += (int)ADDICTION_D->query_skill_modifier(this_object());
	    if(pskill < 1) pskill = 1;
	    proll = random(100) + 1;
	    if(proll <= pskill) prowl_hidden = 1;
	}
	if(prowl_hidden)
	    message("my_action", "You slip away unnoticed.", this_object());
	else
	    message("my_action", "You fail to move quietly.", this_object());
    }
    if(!query_invis() && !hiddenp(this_object()) && !prowl_hidden) {
	inv = all_inventory(prev);
	for(i=0, bzbd = sizeof(inv); i<bzbd; i++) {
	    if(!living(inv[i]) || inv[i] == this_object()) continue;
	    if(adj) tmp = (adj-(int)inv[i]->query_stats("wisdom"));
	    else tmp = 0;
	    if(tmp > random(101)) continue;
	    if(effective_light(inv[i])-(query_skill("stealth")/17) < 0) continue;
	    vname = query_display_name(inv[i]);
	    if(!vname || vname == "") vname = query_cap_name();
	    if(!msg || msg == "") message("mmout", replace_string(query_mmout(), query_cap_name(), vname), inv[i]);
	    else message("mout", replace_string(query_mout(msg), query_cap_name(), vname), inv[i]);
	}
	inv = all_inventory(environment(this_object()));
	for(i=0, bzbd = sizeof(inv); i<bzbd; i++) {
	    if(!living(inv[i])) continue;
	    if(inv[i] == this_object()) continue;
	    if(adj) tmp = (adj-(int)inv[i]->query_stats("wisdom"));
	    else tmp = 0;
	    if(tmp > random(101)) continue;
	    if(effective_light(inv[i])-(query_skill("stealth")/17) < 0) continue;
	    vname = query_display_name(inv[i]);
	    if(!vname || vname == "") vname = query_cap_name();
	    if(!msg || msg == "") message("mmin", replace_string(query_mmin(), query_cap_name(), vname), inv[i]);
	    else message("min", replace_string(query_min(), query_cap_name(), vname), inv[i]);
	}
    }
    if(!query_invis() && !hiddenp(this_object()) && query_followers())
	move_followers(prev);
    add_sp(-1);
    describe_current_room(1);
    show_sbar();
}

/* Single source of truth for MDC-being detection, mirroring the
   four-check logic in _score.c: stat, property, env, race table.
   Admin grant paths have historically set only some of these. */
int query_is_mdc_being() {
    string env_mdc, my_race;

    if((int)query_stats("is_MDC") == 1) return 1;
    if((int)query_property("rifts_mdc_race") == 1) return 1;
    env_mdc = (string)getenv("rifts_mdc_race");
    if(env_mdc && env_mdc != "" && to_int(env_mdc) == 1) return 1;
    my_race = (string)query_race();
    if(my_race && (int)RIFTS_D->is_mdc_race(my_race)) return 1;
    return 0;
}

void show_sbar() {
    int sbar_on;
    int is_mdc;
    int hp, max_hp, sdc, max_sdc, mdc, max_mdc;
    int ppe, max_ppe, isp, max_isp;
    string line;
    string color_on, color_off;

    sbar_on = (int)query_property("sbar_active");
    if(!sbar_on) return;
    if(!interactive(this_object())) return;

    is_mdc = query_is_mdc_being();
    mdc     = (int)query_stats("MDC");
    max_mdc = (int)query_stats("max_MDC");
    hp      = (int)query_stats("rifts_hp");
    max_hp  = (int)query_stats("max_rifts_hp");
    sdc     = (int)query_stats("SDC");
    max_sdc = (int)query_stats("max_SDC");
    ppe     = (int)query_stats("PPE");
    max_ppe = (int)query_stats("max_PPE");
    isp     = (int)query_stats("ISP");
    max_isp = (int)query_stats("max_ISP");

    if((string)getenv("TERM") == "ansi") {
        color_on  = "%^GREEN%^";
        color_off = "%^RESET%^";
    } else {
        color_on  = "";
        color_off = "";
    }

    if(is_mdc) {
        /* Pre-fix MDC characters may have their pool stored in SDC. */
        if(max_mdc == 0 && max_sdc > 0) {
            mdc     = sdc;
            max_mdc = max_sdc;
        }
        line = sprintf("[MDC: %d/%d", mdc, max_mdc);
    } else {
        line = sprintf("[HP: %d/%d | SDC: %d/%d", hp, max_hp, sdc, max_sdc);
    }
    if(max_ppe > 0)
        line += sprintf(" | PPE: %d/%d", ppe, max_ppe);
    if(max_isp > 0)
        line += sprintf(" | ISP: %d/%d", isp, max_isp);
    line += "]";

    message("status", color_on + line + color_off, this_object());
}

/* Default prompt showing the pools that apply to this character's
   race/OCC, using the same selection logic as show_sbar(). Returns
   "" for characters with no Rifts stats (nmsh falls back to "> "). */
string query_rifts_prompt() {
    int is_mdc;
    int mdc, max_mdc;
    int max_ppe, max_isp;
    string line;

    is_mdc = query_is_mdc_being();
    max_ppe = (int)query_stats("max_PPE");
    max_isp = (int)query_stats("max_ISP");

    if(is_mdc) {
        mdc     = (int)query_stats("MDC");
        max_mdc = (int)query_stats("max_MDC");
        /* Pre-fix MDC characters may have their pool stored in SDC. */
        if(max_mdc == 0 && (int)query_stats("max_SDC") > 0) {
            mdc     = (int)query_stats("SDC");
            max_mdc = (int)query_stats("max_SDC");
        }
    }
    if(is_mdc && max_mdc > 0)
        line = sprintf("MDC:%d/%d", mdc, max_mdc);
    else if((int)query_stats("max_rifts_hp") > 0)
        line = sprintf("HP:%d/%d SDC:%d/%d",
            (int)query_stats("rifts_hp"), (int)query_stats("max_rifts_hp"),
            (int)query_stats("SDC"), (int)query_stats("max_SDC"));
    else return "";

    if(max_ppe > 0)
        line += sprintf(" PPE:%d/%d", (int)query_stats("PPE"), max_ppe);
    if(max_isp > 0)
        line += sprintf(" ISP:%d/%d", (int)query_stats("ISP"), max_isp);
    return line + "> ";
}

void create() {
    autosave::create();
    editor::create();
    nmsh::create();
    more::create();
    living::create();
    __IgnoreMsgClass = ({ "broadcast", "info", "more", "room_description", "room_exits","smell","sound","write","say", "system", "prompt", "inanimate_item", "living_item"});
    position = "player";
    wielded = ([]);
    level = 1;
    set_mass(500);
    set_max_encumbrance(5000);
    __LastAged = time();
    verbose_moves = 1;
    enable_commands();
}

int remove() {
    autosave::remove();
    CHAT_D->remove_user(channels - __RestrictedChannels);
    MULTI_D->quit(query_name());
    return living::remove();
}

int quit(string str) {
    if(__NoQuit && !query_forced()) return notify_fail("Stinking cheater.\n");
    if (str) {
	notify_fail("Quit what ?\n");
	return 0;
    }
    if(query_followers()) clear_followers();
    message("environment",
      "Reality suspended.  See you another time!", this_object());
    save_logout_start();
    save_player( query_name() );
    if(environment(this_object()))
        tell_room_living(environment(this_object()), this_object(), 0,
            " goes to sleep.\n");
    log_file("enter", query_name()+" (quit): "+ctime(time())+"\n");
    PLAYER_D->add_player_info();
    remove();
    return 1;
}

void new_body() {
    mapping borg;
    string *zippo;
    int i, max;
    string tmp;

    init_limb_data();
    if(!race) return;
    tmp = race;
    set_max_hp(50 + stats["constitution"] * 10);
    set_hp( (50 + stats["constitution"]*8)/2 );
    set_max_mp( (50 + stats["intelligence"]*10) );
    set_mp( 0 );
    set_max_sp(7*query_stats("dexterity"));
    set_sp(query_stats("dexterity"));
    set_heal_rate(2);
    borg = (mapping)RACE_D->body(this_object());
    for(i=0, max=sizeof(zippo=keys(borg)); i<max; i++) 
	add_limb(zippo[i], borg[zippo[i]]["limb_ref"],borg[zippo[i]]["max_dam"], 0, 0);
    set_wielding_limbs((string *)RACE_D->query_wielding_limbs(tmp));
    set_fingers((int)RACE_D->query_fingers(tmp));
    set_max_encumbrance(stats["strength"] * 200);
    add_limb("whole_body", "", query_max_hp() + 1, 0, 0);
}

void setup() {
    string tmp, *start_temp;
    int needs_chargen;

    set_living_name(query_name());
    set_heart_beat(1);
    if(!stats) init_stats();
    if(!skills) init_skills(0);
    if(!__PlayerAge) __PlayerAge = player_age;
    init_living();
    basic_commands();
    ip = query_ip_name(this_object());
    last_on = ctime(time());
    time_of_login = time();
    if(!body) new_body();
    if(!birth) birth = time();
    fix_limbs();
    if(member_array("whole_body", query_limbs()) == -1)
        add_limb("whole_body", "", query_max_hp() + 1, 0, 0);
    register_channels();
    set_id( ({}) );
    set_max_encumbrance(200*query_stats("strength"));
    nmsh::setup();
    // Auto-init languages for Rifts characters who don't have them set yet
    if(query_race() && (int)RIFTS_D->is_rifts_race(query_race()) &&
       !getenv("rifts_languages"))
        catch(LANGUAGE_D->grant_native_languages(this_object()));
    // Send to setter if no race, or mid-creation (resume via creation_step)
    needs_chargen = !creatorp(this_object()) &&
       (!query_race() ||
        (query_exp() == 0 &&
         (string)getenv("creation_step") != "" &&
         (string)getenv("creation_step") != "done"));
    if(needs_chargen)
        move(ROOM_SETTER);
    else {
	sight_bonus = (int)RACE_D->query_sight_bonus(query_race());
	if(!primary_start) primary_start = getenv("start");
	if(primary_start)
	{
	    start_temp = explode(primary_start, "/");
	    if(start_temp && sizeof(start_temp) >= 2)
	    {
		if(start_temp[0] == "d")
		{ 
		    start_temp[0] = "domains";
		    primary_start = implode(start_temp, "/");
		}
		else if(start_temp[0] == "wizards")
		{
		    start_temp[0] = "realms";
		    primary_start = implode(start_temp, "/");
		}
	    }
	}
	if(!(primary_start && stringp(primary_start) && move(primary_start) == MOVE_OK))
	    move(ROOM_START);
	setenv("start", primary_start);
    }
    if(!stringp(tmp = getenv("TERM"))) setenv("TERM", tmp = "unknown");
    term_info = (mapping)TERMINAL_D->query_term_info(tmp);
    write_messages();
    autosave::setup();
    ensure_wiz_tools();
    call_out("save_player", 2, query_name());
    PLAYER_D->add_player_info();
    log_file("enter", query_name()+" (enter): "+ctime(time())+" from "+
      query_ip_name()+" with\n"+
      query_exp()+" exp, "+
      query_money("platinum")+" pl, "+
      query_money("gold")+" gd, "+
      query_money("electrum")+" el, "+
      query_money("silver")+" sl, "+
      query_money("copper")+" cp\n");
    /* Deferred for chargen-bound players: read_news() uses input_to(),
       which always wins the player's next raw line over setter.c's
       add_action-based chargen prompts, silently eating the zone/race/
       etc answer. finish_creation() in setter.c calls read_news() once
       creation_step reaches "done" instead, when nothing else is
       waiting on that input. */
    if(!needs_chargen)
        NEWS_D->read_news();
    set_max_sp(query_stats("dexterity")*7);
    if((int)RIFTS_D->is_rifts_race(query_race())) {
        call_out("rifts_regen_tick", 60);
        call_out("rifts_hp_regen_tick", 120);
        catch(TATTOO_D->apply_passive_tattoos(this_object()));
    }
    /* Restore brief mode preference. Set directly -- do not toggle.
       restore_object() may have already loaded verbose_moves from the
       save file, and set_brief() would invert it a second time. */
    if((string)getenv("brief_mode") == "1")
        verbose_moves = 0;
    else
        verbose_moves = 1;
    call_out("check_pending_grants", 5);
}

private void check_pending_grants() {
    string fname;
    string content;
    string *lines;
    string *parts;
    string skill_name;
    int pct;
    int i;

    fname = "/secure/save/skill_grants/" + query_name() + ".txt";
    if(file_size(fname) <= 0) return;
    content = read_file(fname);
    if(!content || content == "") return;
    lines = explode(content, "\n");
    for(i = 0; i < sizeof(lines); i++) {
        if(!strlen(lines[i])) continue;
        parts = explode(lines[i], ":");
        if(sizeof(parts) < 2) continue;
        skill_name = parts[0];
        pct = to_int(parts[1]);
        if(!strlen(skill_name) || pct < 1) continue;
        catch(RIFTS_SKILLS_D->grant_skill(this_object(), skill_name, pct));
        catch(RIFTS_SKILLS_D->set_skill_pct(this_object(), skill_name, pct));
        write("An RP wizard has granted you: " + skill_name +
              " (" + pct + "%)\n");
    }
    rm(fname);
}

void heart_beat() {
    object *inv;
    string *cns;
    int i;

    if(!interactive(this_object())) {
	set_heart_beat(0);
	return;
    }
    autosave::heart_beat();
    living::heart_beat();
    ok_to_heal ++;
    if(disable) disable--;
    if(!disable) continue_attack();
    magic_round = 0;
    if(query_age() > ok_to_heal) do_healing(calculate_healing());
    else calculate_healing();
    if(query_idle(this_object()) >= 3600 &&
      !creatorp(this_object()) ) {
	this_object()->move_player(ROOM_IDLE_SHOP);
	this_object()->force_me("sell all");
	this_object()->force_me("quit");
    }
    if(query_invis() && query_sp()<0 && !creatorp(this_player()) ) set_invis(0);
}

void net_dead() {
    CHAT_D->remove_user(channels - __RestrictedChannels);
    channels = ({});
    save_logout_start();
    save_player(query_name());
    net_died_here = file_name( environment(this_object()) );
    message("other_action", sprintf("%s suddenly disappears into a sea of "
	"irreality.", query_cap_name()), environment(this_object()), ({ this_object() }));
    this_object()->move_player(ROOM_FREEZER);
    if(query_snoop(this_object()))
	tell_object(query_snoop(this_object()), capitalize(query_name())+
	  " has gone net-dead.");
    set_heart_beat(0);
}

void restart_heart() {
    string dead_ed;

    message("write", (creatorp(this_object()) &&
	file_size(dead_ed = user_path(query_name())+"dead.edit") > -1 ?
	"\nYour edit file was saved as: "+dead_ed+"\n" :
	"Reconnected."), this_object());
    set_heart_beat(1);
    if(net_died_here) this_object()->move_player(net_died_here);
    else this_object()->move_player(ROOM_START);
    net_died_here = 0;
    say(query_cap_name()+" has rejoined our reality.");
    register_channels();
}

nomask void die() {
    object ob;
    object corpse;
    object money_ob;
    object *stuff;
    string *currs;
    string region;
    int tmp;
    int i;
    int is_rifts;
    int lives;
    int lives_initted;
    int is_mdc;
    int restore_val;
    int retro_level;
    object *grave_attackers;
    string grave_killer;

    is_rifts = (int)RIFTS_D->is_rifts_race(query_race());
    lives_initted = (int)this_object()->query_property("rifts_lives_initted");
    lives = lives_initted ? (int)this_object()->query_property("rifts_lives") : 0;

    if(!is_rifts)
        message("environment", "%^RED%^"+DEATH_MSGS[random(sizeof(DEATH_MSGS))], users(),
          ({ this_object() }) );
    if(creatorp(this_object())) {
	message("death", "You are immortal and cannot die.", this_object());
	return;
    }
    ob = this_object();
    died_here = environment(ob);

    if(is_rifts) {
        /* ── Rifts player death ────────────────────────────────────────────
           Players start with 5 lives and gain 1 per level-up (up to L15).
           With lives remaining: spend 1 life, knock out, respawn. No corpse.
           With 0 lives: full death (corpse, XP loss, death room). */

        /* Retroactive init for characters who died before this system existed. */
        if(!lives_initted) {
            retro_level = (int)this_object()->query_level();
            lives = 5 + (retro_level > 1 ? retro_level - 1 : 0);
            this_object()->set_property("rifts_lives", lives);
            this_object()->set_property("rifts_lives_initted", 1);
        }

        /* Clamp Rifts pools to 0 before any branch. */
        if((int)this_object()->query_stats("rifts_hp") < 0)
            this_object()->set_stats("rifts_hp", 0);
        if((int)this_object()->query_stats("MDC") < 0)
            this_object()->set_stats("MDC", 0);

        if(lives > 0) {
            /* Spend 1 life: knocked out, no corpse, no XP loss, immediate respawn. */
            this_object()->set_property("rifts_lives", lives - 1);

            message("death",
                query_cap_name() + " crumples and lies still.",
                environment());
            message("death",
                "Everything goes dark. A rift catches your falling essence.\n"
                "Lives remaining: " + (lives - 1) + ".",
                this_object());

            is_mdc = (int)RIFTS_D->is_mdc_race(query_race());
            if(is_mdc) {
                restore_val = (int)this_object()->query_stats("max_MDC") / 4;
                if(restore_val < 1) restore_val = 1;
                this_object()->set_stats("MDC", restore_val);
            } else {
                restore_val = (int)this_object()->query_stats("max_rifts_hp") / 4;
                if(restore_val < 1) restore_val = 1;
                this_object()->set_stats("rifts_hp", restore_val);
            }

            cease_all_attacks();

            region = (string)this_object()->query_property("respawn_room");
            if(!region || !sizeof(region))
                region = (string)getenv("start_region");
            if(!region || !sizeof(region))
                region = "/domains/Praxis/rifts_welcome";
            this_object()->move(region);

            message("my_action",
                "You stagger to your feet, dazed but alive. " +
                restore_val + " " + (is_mdc ? "MDC" : "HP") +
                " restored. Lives remaining: " + (lives - 1) + ".\n",
                this_object());

            save_player(query_name());
            PLAYER_D->add_player_info();
            return;
        }

        /* ── 0 lives: full death ─────────────────────────────────────── */
        message("death",
            query_cap_name() + " collapses and does not get up.",
            environment());
        message("death",
            "Everything goes dark. You have died.",
            this_object());

        /* PvP infamy/XP-bonus check BEFORE deducting XP */
        rifts_death_pvp_check();

        /* Deduct 10% XP */
        if((int)this_object()->query_level() > 1) {
            tmp = (int)this_object()->query_exp() / 10;
            if(tmp < 0) tmp = 0;
            this_object()->add_exp(-tmp);
        }

        corpse = new("/std/rifts_corpse");
        corpse->set_name(query_cap_name());
        corpse->move(environment(this_object()));
        catch(RIFTS_D->drop_death_blood(this_object(), environment(this_object())));
        /* Transfer carried and worn items to corpse */
        i = sizeof(stuff = all_inventory(this_object()));
        while(i--) {
            if(!objectp(stuff[i])) continue;
            if((int)stuff[i]->is_germ()) { stuff[i]->remove(); continue; }
            if((mixed *)stuff[i]->query_auto_load()) continue;
            stuff[i]->move(corpse);
        }

        /* Drop 10% of credits as a chip at the death site */
        {
            int total_credits, chip_amount;
            object chip;
            total_credits = (int)this_object()->query_money("credits");
            if(total_credits > 0) {
                chip_amount = total_credits / 10;
                if(chip_amount > 0) {
                    chip = new("/obj/credit_chip");
                    chip->set_credits(chip_amount);
                    chip->move(environment(this_object()));
                }
            }
        }

        if((int)query_level() >= 4) {
            grave_attackers = query_attackers();
            if(grave_attackers && sizeof(grave_attackers) > 0 &&
               objectp(grave_attackers[0]))
                grave_killer = (string)grave_attackers[0]->query_cap_name();
            else
                grave_killer = "the Rifts";
            set_property("permanently_dead", 1);
            set_property("death_level", (int)query_level());
            set_property("death_killer", grave_killer);
        }
        ghost = 1;
        this_object()->move("/domains/Praxis/death/death_room");
        cease_all_attacks();
        remove_call_out("rifts_resurrect");
        call_out("rifts_resurrect", 10);
        save_player(query_name());
        PLAYER_D->add_player_info();
        return;
    }

    // ── Standard NM3 player death ─────────────────────────────────────
    message("death", "You die.\nYou feel the sensations of nothingness " +
      "as you rise above your corpse.\nYou arrive at a destination in a reality " +
      "not like your own.", this_object());
    message("death", query_cap_name()+ " dies a horrible death.",
      environment());
    if((int)this_object()->query_level() > 1) {
	this_object()->add_exp(-((int)this_object()->query_exp()/4));
	this_object()->reduce_stats();
	reduce_skills();
	while((int)ADVANCE_D->get_exp(query_level()) > query_exp())
	    set_level(query_level()-1);
	if(!(int)RIFTS_D->is_rifts_race(query_race()))
	    setenv("TITLE", (string)ADVANCE_D->get_new_title(this_object()));
    }
    corpse = new(OB_CORPSE);
    corpse->set_name(query_cap_name());
    corpse->copy_body(this_object()); // Added for necromancy - Manny
    corpse->move(environment(this_object()));
    money_ob = new("/std/obj/coins");
    currs = query_currencies();
    for(i=0, tmp= sizeof(currs); i<tmp; i++) {
	money_ob->set_money(currs[i], query_money(currs[i]));
	add_money(currs[i], -query_money(currs[i]));
    }
    money_ob->move(corpse);
    i = sizeof(stuff = all_inventory(this_object()));
    while(i--) {
	if((int)stuff[i]->is_germ()) stuff[i]->remove();
	else if((mixed *)stuff[i]->query_auto_load()) continue;
	else if((int)stuff[i]->allow_drop(this_object()))
	    stuff[i]->move(corpse);
    }
    if((int)query_level() >= 4) {
        grave_attackers = query_attackers();
        if(grave_attackers && sizeof(grave_attackers) > 0 &&
           objectp(grave_attackers[0]))
            grave_killer = (string)grave_attackers[0]->query_cap_name();
        else
            grave_killer = "the Rifts";
        set_property("permanently_dead", 1);
        set_property("death_level", (int)query_level());
        set_property("death_killer", grave_killer);
    }
    this_object()->move("/domains/Praxis/death/death_room");
    cease_all_attacks();
    ghost = 1;
    save_player( query_name() );
    PLAYER_D->add_player_info();
}

// PvP death: track infamy and award XP bonus to killer.
void rifts_death_pvp_check() {
    object *my_attackers;
    object killer;
    int xp_loss;
    int xp_bonus;

    my_attackers = query_attackers();
    if(!my_attackers || !sizeof(my_attackers)) return;
    killer = my_attackers[0];
    if(!objectp(killer) || !killer->is_player()) return;

    if(query_alignment() > 500) {
        catch(WANTED_D->add_infamy((string)killer->query_name(), 50,
            "killed good-aligned player " + query_name()));
    } else if(query_alignment() < -200) {
        catch(WANTED_D->add_infamy((string)killer->query_name(), -10,
            "killed evil being " + query_name()));
    }
    xp_loss  = (int)this_object()->query_exp() / 10;
    if(xp_loss < 0) xp_loss = 0;
    xp_bonus = xp_loss / 2;
    if(xp_bonus > 0) catch(killer->add_exp(xp_bonus));
}

// Auto-resurrect: called via call_out 10s after Rifts player death,
// or 30s after praying.
void rifts_resurrect() {
    int is_mdc, max_val, restore_val;
    string region;

    if(!ghost) return;
    if(!(int)RIFTS_D->is_rifts_race(query_race())) return;

    ghost = 0;
    is_mdc = (int)RIFTS_D->is_mdc_race(query_race());

    // Restore 25% of max HP/MDC
    if(is_mdc) {
        max_val     = (int)this_object()->query_stats("max_MDC");
        restore_val = max_val / 4;
        if(restore_val < 1) restore_val = 1;
        this_object()->set_stats("MDC", restore_val);
    } else {
        max_val     = (int)this_object()->query_stats("max_rifts_hp");
        restore_val = max_val / 4;
        if(restore_val < 1) restore_val = 1;
        this_object()->set_stats("rifts_hp", restore_val);
    }

    message("death",
        "The grey void releases you.  A rift tears open, returning you\n"
        "to the world of the living, battered but alive.",
        this_object());

    /* Respawn: check respawn_room property, then start_region env, then default */
    region = (string)this_object()->query_property("respawn_room");
    if(!region || !sizeof(region))
        region = (string)getenv("start_region");
    if(!region || !sizeof(region))
        region = "/domains/Praxis/rifts_welcome";
    this_object()->move(region);

    message("my_action",
        "You regain consciousness, battered and stripped of your gear.\n"
        "You find yourself back in the world of the living. " + restore_val + " " +
        (is_mdc ? "MDC" : "HP") + " restored.\n"
        "Your equipment remains on your corpse. Retrieve it quickly.",
        this_object());
    save_player(query_name());
}

// Called by _pray.c: speeds resurrection to 30 seconds.
void pray_resurrect() {
    if(!ghost) {
        message("my_action", "You are not dead.", this_object());
        return;
    }
    if(!(int)RIFTS_D->is_rifts_race(query_race())) {
        message("my_action", "Your prayers echo unanswered.", this_object());
        return;
    }
    if(file_name(environment(this_object())) != "/domains/Praxis/death/death_room") {
        message("my_action",
            "You can only pray for resurrection in the death chamber.",
            this_object());
        return;
    }
    remove_call_out("rifts_resurrect");
    message("death",
        "You pray to the forces of the Rifts for swift resurrection.\n"
        "A warm light stirs in the void around you...",
        this_object());
    call_out("rifts_resurrect", 30);
}

// Called by death room on entry: (re)starts the resurrect timer.
void start_rifts_resurrect(int delay) {
    if(!ghost) return;
    remove_call_out("rifts_resurrect");
    call_out("rifts_resurrect", delay);
}

void set_rname(string rname) {
    if(!((int)master()->valid_apply(({ query_name()})))) return;
    real_name = rname;
}

int is_player() { return 1; }

string query_ip() { return ip; }

string query_email() { if (email) return email; return "???@" + ip; }

string query_rname() { return real_name ? real_name : "???"; }

string query_password() { return password; }

void set_password(string pass) {
    if(!((int)master()->valid_apply(({ query_name()})))) return 0;
    password = pass;
    save_player( query_name() );
}

void set_email(string e) {
    if(!((int)master()->valid_apply(({ query_name()})))) return 0;
    if (this_player(1) != this_player()) return 0;
    email = e;
    save_player( query_name() );
}

int set_snoop() {
    if(!((int)master()->valid_apply(({ query_name()})))) return 0;
    if(snoop) {
	message("my_action", "You are now secure from being snooped.",
	  this_object());
	snoop = 0;
	return 1;
    }
    else {
	message("my_action", "You can now be snooped by anyone.",
	  this_object());
	snoop = 1;
	return 1;
    }
}

int query_snoop() { return snoop; }


void write_messages() {
    mapping mail_stat;
    int i;
    int player_unread;

    if(getenv("TERM") == "unknown" && (string)getenv("colorize_set") != "1")
        message("system",
            "\nTerminal type not detected. Type 'colorize' to enable ANSI color.",
            this_object());
    mail_stat = (mapping)FOLDERS_D->mail_status(query_name());
    if(mail_stat["unread"]) {
	message("login", sprintf("\n        >>> %d of your %d %s are "
	    "unread! <<<", mail_stat["unread"], mail_stat["total"],
	    (mail_stat["total"]>1 ? "letters" : "letter")), this_object());
    }
    /* Player mail notification (MAIL_D -- separate from wizard FOLDERS_D) */
    player_unread = 0;
    catch(player_unread = (int)MAIL_D->query_unread_count(query_name()));
    if(player_unread > 0)
        message("login",
            "\n>>> You have " + player_unread + " unread player mail message" +
            (player_unread == 1 ? "" : "s") +
            ". Type 'mail' to read. <<<",
            this_object());
    if(query_invis()) message("login", "\n        You are currently "
	  "invisible.", this_object());
    if(creatorp(this_object())) {
	if(file_exists("/log/errors/"+query_name()))
	    message("login", "\n        >>> You have errors in /log/errors/"+
	      query_name()+" <<<", this_object());
	if(file_exists("/log/reports/"+query_name()))
	    message("login", "\n        >>> You have reports in "
	      "/log/reports/"+query_name()+" <<<", this_object());
    }
    if( !message ) return;
    for( i=0; i<sizeof(message); i++ )
	tell_object(this_object(), message[i]);
    message = ({ });
}

string query_cap_name() {
    string name;
    string title;

    if(query_invis()) return "A shadow";
    name = query_CapName();
    title = rp_title;
    if(title && strlen(title)) return title + " " + name;
    return name;
}

string query_title() {
    string str;
    string foo,fii;

    if(query_ghost()) return "A ghost";
    if(invis) return 0;
    str = getenv( "TITLE" );
    if( !str ) str = query_cap_name();
    else if( !sscanf(str, "%s$N%s", foo,fii) )
	str = query_cap_name() + " " + str;
    else
	str = substr( str, "$N", query_cap_name() );
    return str;
}

string query_short() {
    string base;

    base = query_display_name(this_player());
    if(!base) return 0;
    if(interactive(this_object())) return base;
    return base + " (link-dead)";
}

mixed *local_commands() {
    if(!((int)master()->valid_apply(({ query_name()})))) {
	message("my_action", "You aren't authorized to check this information.",
	  this_object());
	return ({});
    }
    return commands();
}

nomask string query_position() { return position; }

nomask int query_level() { return level; }

private void wiz_promote_announce(string who, string rank, string promoter) {
    object *all_users;
    string msg;
    int i;

    msg = "[WIZ] " + promoter + " has promoted " + who + " to " + rank + ".\n";
    all_users = users();
    i = sizeof(all_users);
    while(i--) {
        if(!all_users[i]) continue;
        if(creatorp(all_users[i]))
            message("wiz", msg, all_users[i]);
    }
}

private void wiz_setup_workroom(string pname, string pos) {
    string rdir;
    string rroom;
    string content;
    string book_role;

    if(pos == "creator" || pos == "immortal") book_role = "coding";
    else if(pos == "rp_wiz") book_role = "rp";
    else if(pos == "domain_wiz") book_role = "domain";
    else if(pos == "apprentice") book_role = "apprentice";
    else book_role = "admin";

    rdir  = "/realms/" + pname;
    rroom = rdir + "/workroom.c";
    if(file_size(rroom) <= 0) {
        catch(mkdir(rdir));
        content =
            "/* /realms/" + pname + "/workroom.c\n"
            "   Personal workroom for " + capitalize(pname) + ". */\n"
            "#include <std.h>\n"
            "#include <rooms.h>\n"
            "inherit ROOM;\n"
            "void create() {\n"
            "    ::create();\n"
            "    set_property(\"light\", 3);\n"
            "    set_property(\"indoors\", 1);\n"
            "    set_property(\"no attack\", 1);\n"
            "    set_short(\"" + capitalize(pname) + "'s workroom\");\n"
            "    set_long(\n"
            "        \"A functional workspace. Shelves hold reference documents\\n\"\n"
            "        \"and notes. A large desk sits in the center.\\n\"\n"
            "        \"A request mailbox is mounted near the door.\\n\"\n"
            "        \"Type 'read mailbox' to review pending skill requests.\\n\"\n"
            "    );\n"
            "    set_exits( ([\"out\" : \"/domains/wizards/hallway\"]) );\n"
            "}\n"
            "void reset() {\n"
            "    object mb;\n"
            "    object bk;\n"
            "    ::reset();\n"
            "    if(!present(\"mailbox\", this_object())) {\n"
            "        mb = clone_object(\"/domains/adm/wiz_tools/mailbox\");\n"
            "        if(mb) mb->move(this_object());\n"
            "    }\n"
            "    if(!present(\"book\", this_object())) {\n"
            "        bk = clone_object(\"/domains/adm/wiz_tools/wiz_reference_book\");\n"
            "        if(bk) {\n"
            "            bk->set_property(\"book_role\", \"" + book_role + "\");\n"
            "            bk->move(this_object());\n"
            "        }\n"
            "    }\n"
            "}\n";
        write_file(rroom, content);
    }
    catch((int)"/cmds/creator/_qcs"->qcs_ensure_realm_dirs(pname));
}

/* Callable externally (login.c) so it can decide whether to offer the
 * first-admin prompt. Not private: no sensitive side effects, just a
 * read-only scan. Authoritative enforcement still happens inside
 * set_position()'s guard below, which re-checks this itself. */
int any_admin_exists() {
    string *letters, *files, *lines;
    string ldir, data;
    int i, j, k;

    letters = get_dir(DIR_USERS + "/");
    for(i = 0; i < sizeof(letters); i++) {
        if(letters[i] == "." || letters[i] == "..") continue;
        ldir = DIR_USERS + "/" + letters[i] + "/";
        if(file_size(ldir) != -2) continue;
        files = get_dir(ldir);
        for(j = 0; j < sizeof(files); j++) {
            if(strlen(files[j]) < 3 || files[j][<2..] != ".o") continue;
            data = read_file(ldir + files[j]);
            if(!data) continue;
            lines = explode(data, "\n");
            for(k = 0; k < sizeof(lines); k++) {
                if(lines[k] == "position \"arch\"" ||
                   lines[k] == "position \"head arch\"") return 1;
            }
        }
    }
    return 0;
}

void set_position(string pos) {
    string prev_pos;
    string pname;
    string promoter_name;
    int entering_wiz;

    /* Bootstrap escape hatch: PRIVS is #undef in options.h, so
     * master()->valid_apply() below can never return true for anyone,
     * privileged directory or not (query_privs() is always falsy with
     * PRIVS off, and check_access()'s first line denies on that).  This
     * clause does not depend on valid_apply() at all - it authenticates
     * the caller by file identity (base_name(previous_object()) ==
     * OB_LOGIN, the same idiom already used by set_cap_name() and
     * set_client() in this file) and by re-scanning for an existing
     * admin, so it works whether or not PRIVS is ever enabled.
     * Known limitation: two simultaneous first-time registrations could
     * both observe any_admin_exists() == 0 before either set_position()
     * call lands, granting admin to both. Accepted as a low-likelihood
     * edge case for a small MUD; not fixed here.
     * This clause is checked FIRST, not last: it runs during account
     * registration, before exec_user() has bound the connection to a
     * player, so this_player() is still 0 here. archp(0) falls back to
     * previous_object() and calls query_position() on a non-player
     * object, which crashes lower_case() with a 0 argument. Putting
     * this clause first lets it short-circuit the && chain before
     * archp()/creatorp() are ever reached in the bootstrap case. */
    if(!(base_name(previous_object()) == OB_LOGIN && !any_admin_exists()) &&
       !creatorp(this_object()) && !archp(this_player()) &&
       !((int)master()->valid_apply(({ query_name()})))) return;
    prev_pos = position;
    if(member_array(position, MORTAL_POSITIONS) != -1)
    if(position == "high mortal" && pos != "high mortal")
	refresh_command_path();
    position = pos;
    entering_wiz = (pos == "creator" || pos == "arch" || pos == "head arch" ||
       pos == "immortal" || pos == "rp_wiz" || pos == "domain_wiz" ||
       pos == "apprentice");
    if(entering_wiz) {
        catch(sync_wiz_role(this_object()));
        catch(WIZTOOLS_D->remove_tools(this_object()));
        catch(WIZTOOLS_D->give_tools(this_object(), pos));
        if(member_array(prev_pos, MORTAL_POSITIONS) != -1) {
            pname = query_name();
            promoter_name = previous_object() ?
                (string)previous_object()->query_cap_name() : "The system";
            wiz_setup_workroom(pname, pos);
            tell_object(this_object(),
                "You have been promoted.  Welcome to the staff of AetherMUD.\n"
                "Type 'workroom' to teleport to your workroom.\n");
            call_out("wiz_promote_announce", 2,
                capitalize(pname), pos, promoter_name);
        }
    } else {
        remove_env("wiz_role");
        remove_env("has_wiz_tools");
        catch(WIZTOOLS_D->remove_tools(this_object()));
    }
    refresh_command_path();
    refresh_channels();
    return;
}

private void ensure_wiz_tools() {
    object *inv;
    int i;
    string bn;

    if(!creatorp(this_object())) return;
    catch(sync_wiz_role(this_object()));
    /* has_wiz_tools records that this wizard's tool set was already
       granted once. Honoring it here lets wizards keep their tools in
       a supplies chest instead of inventory without every login
       re-granting a fresh set to an empty inventory. Demotion clears
       the env (set_position else-branch), so re-promotion regrants. */
    if((string)getenv("has_wiz_tools") == "1") return;
    inv = all_inventory(this_object());
    for(i = 0; i < sizeof(inv); i++) {
        bn = base_name(inv[i]);
        if(bn && strsrch(bn, "wiz_tools") != -1) {
            setenv("has_wiz_tools", "1");
            return;
        }
    }
    catch(WIZTOOLS_D->remove_tools(this_object()));
    catch(WIZTOOLS_D->give_tools(this_object(), position));
    setenv("has_wiz_tools", "1");
}

void move_to_workroom() {
    string wroom;
    wroom = "/realms/" + query_name() + "/workroom";
    if(file_size(wroom + ".c") > 0) {
        move_player(wroom);
        return;
    }
    wroom = "/domains/wizards/" + query_name() + "/workroom";
    if(file_size(wroom + ".c") > 0)
        move_player(wroom);
    else if(file_size("/domains/wizards/hallway.c") > 0)
        move_player("/domains/wizards/hallway");
}

void set_level(int lev) {
    level = lev;
    PLAYER_D->add_player_info();
    log_file("advance", this_object()->query_name()+" advanced to level "+lev+": "+ctime(time())+"\n");
    if(position == "high mortal" && level < 20) {
	position = "player";
	search_path -= ({ DIR_HM_CMDS });
	set_env("start", ROOM_START);
	set_env("TITLE", "Mortal $N the fallen high mortal");
    }
    return;
}


int set_earmuffs() {
    if( earmuffs )
    {
	earmuffs = 0;
	message("my_action", "Your earmuffs are now off.",
	  this_object());
    }
    else
    {
	message("my_action", "Your earmuffs are now on.", this_object());
	earmuffs = 1;
    }
    return earmuffs;
}

void receive_message(string msg_class, string msg) {
    string *words;
    int i, max;
    string str, pre, post;
    int x, do_wrap;

    if(query_log_harass())
        log_file(DIR_LOGS "/harass/"+query_name(), strip_colours(msg)+"\n");
    if(query_locked() && msg_class != "prompt") {
	if(!__MessageCue) __MessageCue = ({});
	__MessageCue += ({ ({ msg_class, msg }) });
	return;
    }
    if(__Client) {
	receive("<"+msg_class+">"+msg+"\n");
	return;
    }
    if(!stringp(str=getenv("SCREEN"))) str = "75";
    x = to_int(str);
    if(msg_class[0] == 'N') msg_class = msg_class[1..sizeof(msg_class)-1];
    else if(msg_class != "prompt") msg = wrap(msg, x);  // prompt fix -Val
    if(msg_class == "system" || msg_class == "help" || msg_class == "more"){
	receive(msg);
	return;
    }
    if(query_blocked(msg_class) &&
      member_array(msg_class, __IgnoreMsgClass) == -1) return;
    switch(msg_class) {
    case "room_exits": msg = "%^BOLD%^%^GREEN%^"+msg+"%^RESET%^"; break;
    case "ooc":        msg = "%^RED%^"+msg; break;
    case "radio":      msg = "%^GREEN%^"+msg; break;
    case "snoop":      msg = "%^CYAN%^"+msg; break;
    case "tell":
	if(sscanf(msg, "%s:%s", pre, post) == 2)
	    msg = "%^BOLD%^%^BLUE%^"+pre+":"+post;
	break;
    }
    if(!term_info) reset_terminal();
    for(i=0, max=sizeof(words=explode(msg, "%^")); i<max; i++)
	if(term_info[words[i]]) words[i] = term_info[words[i]];
    receive(implode(words, "")+term_info["RESET"]);
}

void catch_tell(string str) {
    if(query_log_harass()) {
        log_file(DIR_LOGS "/harass/" + query_name(), str+"\n");
    }
    receive(str);
}

void set_guild(string str) {
    guild = str;
    if(!guild) guild = str;
}

string query_guild() { return guild; }

void set_rolls(int x) { rolls = x; }

int query_rolls() { return rolls; }

void set_blocked(string str) {
    if(!blocked) blocked = ([]);
    blocked[str] = !blocked[str];
    if(blocked[str]) message("info", "You are now blocking "+str+".",
	  this_object());
    else message("info", "You are no longer blocking "+str+".",this_object());
}

int query_blocked(string str) {
    if(!blocked) blocked = ([]);
    if(blocked["all"]) return blocked["all"];
    else return blocked[str];
}

string *query_mysites() {
    if(!mysites) mysites = ({});
    return mysites;
}

void set_mysites(string *borg) {
    if(!borg) return;
    if(!((int)master()->valid_apply(({ query_name()})))) return;
    mysites = borg;
    return;
}

string *query_quests() {
    if(!quests) quests = ({});
    return quests;
}

int set_quest(string str) {
    if(!quests) quests = ({});
    if(member_array(str, quests) != -1) return 0;
    player_data["general"]["quest points"] += (int)call_other(ROOM_QUEST, "query_quest_points", str);
    quests += ({ str });
    log_file("quests", query_name()+" completed "+str+": "+ctime(time())+"\n");
    return 1;
}

void return_to_death_site() {
    if(!died_here) this_object()->move("/domains/Praxis/square");
    else this_object()->move(died_here);
}

void revive() {
    message("my_action", "You return from the dead!", this_object());
    message("other_action", query_cap_name()+ " returns from the dead.",
      environment());
    //  tell_room(environment(this_object()), query_cap_name()+" returns from the dead.\n", this_object());
    new_body();
    ghost = 0;
}

int query_ghost() { return ghost; }

int query_quest_points() {
    if(!player_data["general"]["quest points"]) return 0;
    else return player_data["general"]["quest points"];
}

void set_disable(int x) { 
    if(x) disable = x;
    else disable = 1;
}

int query_disable() { return disable; }

void set_married(string str) {
    if(current_marriage) return;
    current_marriage = ({ lower_case(str), time() });
}

void divorce_me() {
    if(!current_marriage) return;
    if(!divorced) divorced = ({ ({ current_marriage[0], current_marriage[1], time() }) });
    else divorced += ({ ({current_marriage[0], current_marriage[1], time() }) });
    current_marriage = 0;
}

string query_married() {
    return (current_marriage && sizeof(current_marriage) ? 
      current_marriage[0] : 0);
}

mixed *query_current_marriage() { return current_marriage; }

mixed *query_divorced() { return divorced; }


int query_birthday() { return birth; }


void set_news(string which, int size) {
    if(!news) news = ([]);
    news[which] = size;
}

int query_news(string which) {
    if(!news) news = ([]);
    return news[which];
}


void hide(int x) {
    object *inv;
    int i;

    i = sizeof(inv = all_inventory(this_object()));
    if(x) {
	set_hide(x);
	while(i--) inv[i]->hide(x);
    }
    else {
	while(i--) inv[i]->hide(x);
	set_hide(x);
    }
}

void set_race(string str) { 
    race = str;
    LANG_D->init_languages(this_object());
}

string query_race() { return race; }

int set_mini_quest(string str, int x, string desc) {
    if(!mini_quests) mini_quests = ([]);
    if(mini_quests[str]) return 0;
    else mini_quests[str] = ({ time(), desc });
    add_exp(x);
    log_file("quests", query_name()+" completed mini-quest "+str+" "+ctime(time())+" for "+x+" exp.\n");
    return 1;
}

string *query_mini_quests() {
    if(!mini_quests) return ({});
    else return keys(mini_quests);
}

mapping query_mini_quest_map() { return (mini_quests ? mini_quests : ([])); }

int query_login_time() { return time_of_login; }

void reset_terminal() {
    term_info = (mapping)TERMINAL_D->query_term_info(getenv("TERM"));
}

void set_name(string str) {
    if(!((int)master()->valid_apply(({ str })))){
//debug_message("tried to set_name(\""+str+"\") in user, got invalid apply");
 return;
}
    char_name = str;
    ::set_name(str);
}

void restrict_channel(string channel) {
    if(base_name(previous_object()) != "/cmds/adm/_channel") return;
    if(!__RestrictedChannels) __RestrictedChannels = ({});
    __RestrictedChannels += ({ channel });
    CHAT_D->remove_user( ({ channel }) );
}

void unrestrict_channel(string str) {
    if(base_name(previous_object()) != "/cmds/adm/_channel") return;
    __RestrictedChannels -= ({ str });
    CHAT_D->add_user( ({ str }) );
}

nomask string *query_channels() { return channels - __RestrictedChannels; }

static private register_channels() {
    if(creatorp(this_object())) {
        channels = ({
            "cre", "gossip", "intercre", "newbie", "hm",
            "fighter", "mage", "monk", "rogue", "cleric", "kataan"
        });
        if(archp(this_object())) channels += ({ "admin" });
        if(leaderp(this_object())) channels += ({ "council" });
    }
    else channels = ({});
    if(!__RestrictedChannels) __RestrictedChannels = ({});
    CHAT_D->add_user(channels - __RestrictedChannels);
}

void refresh_channels() {
    string *old;

    if(!userp(this_object())) return;
    old = channels ? (channels - __RestrictedChannels) : ({});
    if(sizeof(old)) CHAT_D->remove_user(old);
    register_channels();
}

string query_first_site() { return original_site; }

void set_primary_start(string str) {
    if(!str || !stringp(str) || !strlen(str)) return;
    setenv("start", primary_start = str);
}

string query_primary_start() { return primary_start; }

private int valid_start_room(object who, object env) {
    string path;
    string step;

    if(!who || !env || env == who) return 0;
    if(living(env) && !(int)env->query_property("mountable")) return 0;
    path = base_name(env);
    if(!path || path == "" || path == "0") return 0;
    switch(path) {
    case ROOM_FREEZER:
    case ROOM_SETTER:
    case ROOM_VOID:
    case ROOM_DEATH:
    case ROOM_QUEST:
    case ROOM_PARTY:
    case ROOM_CACHE:
    case ROOM_LOCKED:
        return 0;
    }
    if((int)who->query_ghost()) return 0;
    if(!creatorp(who)) {
        step = (string)who->getenv("creation_step");
        if(step && step != "" && step != "done") return 0;
        if(strsrch(path, "workroom") != -1) return 0;
    }
    if((int)env->query_property("no teleport")) return 0;
    if(file_size(path + ".c") <= 0) return 0;
    return 1;
}

void save_logout_start() {
    object env;
    string path;

    env = environment(this_object());
    if(!valid_start_room(this_object(), env)) return;
    path = base_name(env);
    set_primary_start(path);
}

int set_start_here() {
    object env;
    string path;

    env = environment(this_object());
    if(!valid_start_room(this_object(), env)) {
        write("You cannot set this as your login location.\n");
        return 0;
    }
    path = base_name(env);
    set_primary_start(path);
    write("You will now login here");
    if(stringp(env->query_short()) && strlen((string)env->query_short()))
        write(" (" + (string)env->query_short() + ")");
    write(".\n");
    log_file("start", query_name() + " is set to login at " + path + "\n");
    return 1;
}

void clean_net_dead() {
    if(base_name(previous_object()) != ROOM_FREEZER) return;
    this_object()->remove();
}

void set_id(string *bogus) {
    int i;

    if(__UserId) return;
    if(!query_CapName()) {
	__UserId = ({ query_name() });
	return;
    }
    __UserId = ({ query_name(), lower_case(query_CapName()) });
    if((i=sizeof(bogus = explode(lower_case(query_CapName()), " "))) == 1)
	return;
    while(i--)
	if(!user_exists(bogus[i])) __UserId += ({ bogus[i] });
}

string *query_id() { return __UserId; }

int id(string str) {
    if(!str) return 0;
    else return member_array(str, __UserId) != -1;
}

void set_cap_name(string str) {
    if(query_name() != convert_name(str) && 
      base_name(previous_object()) != OB_LOGIN) return;
    living::set_cap_name(capitalize(str));
}

int query_undead() { return query_ghost(); }

void set_last_on(int x) {
    if(!archp(this_object())) return;
    last_on = ctime(x);
    save_player(query_name());
}

void empty_cue() {
    int i, maxi;

    if(previous_object() != this_object()) return;
    for(i=0, maxi = sizeof(__MessageCue); i < maxi; i++)
	message(__MessageCue[i][0], __MessageCue[i][1], this_object());
    __MessageCue = 0;
}

void set_log_harass(int x) {
    string txt;

    if(this_player() != this_object()) return;
    if( __LogHarass == x ) return;
    if( x )
    {
	txt = "**************** Start of Log *****************\n"+
	    "Time: " + ctime( time() ) + "\n";
	if( environment( this_object() ) ) txt += "Place: " +
	    file_name( environment( this_object() ) ) + "\n";
    } else {
	txt = "**************** End of Log *****************\n"+
	    "Time: " + ctime( time() ) + "\n";
    }
    log_file(DIR_LOGS "/harass/" + query_name(), txt);
    __LogHarass = x;
}

int query_log_harass() { return __LogHarass; }

void set_client(string str) {
    if(base_name(previous_object()) != OB_LOGIN) return;
    __Client = str;
}

string query_client() { return __Client; }

int query_hp() { return living::query_hp(); }

int query_max_hp() { return living::query_max_hp(); }

int query_mp() { return living::query_mp(); }

int query_max_mp() { return living::query_max_mp(); }

int query_sp() { return living::query_sp(); }

int query_max_sp() { return living::query_max_sp(); }

string get_path() { return nmsh::query_cwd(); }

string query_cwd() { return nmsh::query_cwd(); }

string query_prompt() {
    string str;

    if(stringp(str = getenv("PROMPT"))) return str;
    else return nmsh::query_prompt(); 
}

string getenv(string key) {
    return living::getenv(key);
}

string query_name() { 
string tmp;

//tmp =  living::query_name();
tmp = __TrueName;
////debug_message("I am "+identify(this_object())+", and my name is: "+tmp);
//debug_message("My name is: "+tmp);
return tmp;
}

varargs int query_invis(object ob) { return living::query_invis(ob); }

void receive_snoop(string str) { receive_message("snoop", "%"+str); }

void set_backstab(int x) {
    if(file_name(previous_object(0)) != "/cmds/skills/_backstab") return;
    __NoQuit = 1;
    call_out("remove_backstab", x);
}

void remove_backstab() { __NoQuit = 0; }

void set_error(mapping mp) {
    if(previous_object() != master()) return;
    __LastError = mp;
}

mapping query_error() {
    if(file_name(previous_object(0))[0..24] != "/secure/cmds/creator/_dbx") return ([]);
    else return __LastError;
}

// Rifts PPE/ISP regeneration: 5/min at rest, 10/min on a ley line.
void rifts_regen_tick() {
    int ppe;
    int max_ppe;
    int isp;
    int max_isp;
    int ppe_rate;
    int isp_rate;
    object env;
    int in_ley_line;

    if(!interactive(this_object())) return;
    if(!(int)RIFTS_D->is_rifts_race(query_race())) return;

    if(sizeof((object *)query_attackers()) > 0) {
        call_out("rifts_regen_tick", 60);
        return;
    }

    env = environment(this_object());
    in_ley_line = (env && (int)RIFTS_D->room_on_ley_line(env));

    ppe_rate = in_ley_line ? 10 : 5;
    isp_rate = 1;

    if(present("nexus stone", this_object())) ppe_rate++;

    ppe     = (int)query_stats("PPE");
    max_ppe = (int)query_stats("max_PPE");
    isp     = (int)query_stats("ISP");
    max_isp = (int)query_stats("max_ISP");

    if(max_ppe > 0 && ppe < max_ppe) {
        ppe += ppe_rate;
        if(ppe > max_ppe) ppe = max_ppe;
        set_stats("PPE", ppe);
    }
    if(max_isp > 0 && isp < max_isp) {
        isp += isp_rate;
        if(isp > max_isp) isp = max_isp;
        set_stats("ISP", isp);
    }
    call_out("rifts_regen_tick", 60);
}

void rifts_hp_regen_tick() {
    string race_val;
    string occ_val;
    int in_combat;
    int delay;
    int cur_hp;
    int max_hp;
    int cur_sdc;
    int max_sdc;
    int cur_mdc;
    int max_mdc;

    if(!interactive(this_object())) return;
    if(!(int)RIFTS_D->is_rifts_race(query_race())) return;

    race_val  = lower_case((string)query_race());
    occ_val   = (string)getenv("rifts_occ");
    if(!occ_val) occ_val = "";
    else occ_val = lower_case(occ_val);
    in_combat = (sizeof((object *)query_attackers()) > 0);

    if(strsrch(race_val, "dragon") != -1) {
        delay = 600;
        if(!in_combat) {
            cur_mdc = (int)query_stats("MDC");
            max_mdc = (int)query_stats("max_MDC");
            if(cur_mdc > 0 && cur_mdc < max_mdc)
                set_stats("MDC", cur_mdc + 1);
        }
    } else if((int)RIFTS_D->has_race_flag(this_object(), "regeneration")) {
        delay = 300;
        if(!in_combat) {
            cur_mdc = (int)query_stats("MDC");
            max_mdc = (int)query_stats("max_MDC");
            cur_sdc = (int)query_stats("SDC");
            max_sdc = (int)query_stats("max_SDC");
            cur_hp  = (int)query_stats("rifts_hp");
            max_hp  = (int)query_stats("max_rifts_hp");
            if(max_mdc > 0 && cur_mdc > 0 && cur_mdc < max_mdc)
                set_stats("MDC", cur_mdc + random(6) + random(6) + 1);
            else if(max_sdc > 0 && cur_sdc < max_sdc)
                set_stats("SDC", cur_sdc + random(4) + 2);
            else if(max_hp > 0 && cur_hp < max_hp)
                set_stats("rifts_hp", cur_hp + random(4) + 2);
        }
    } else if(occ_val == "juicer" || occ_val == "ninja juicer" ||
              occ_val == "delphi juicer" || occ_val == "hyperion juicer") {
        delay = 120;
        if(!in_combat) {
            cur_sdc = (int)query_stats("SDC");
            max_sdc = (int)query_stats("max_SDC");
            cur_hp  = (int)query_stats("rifts_hp");
            max_hp  = (int)query_stats("max_rifts_hp");
            if(max_sdc > 0 && cur_sdc < max_sdc)
                set_stats("SDC", cur_sdc + 1);
            else if(max_hp > 0 && cur_hp < max_hp)
                set_stats("rifts_hp", cur_hp + 1);
        }
    } else if(occ_val == "cyber-knight") {
        delay = 900;
        if(!in_combat) {
            cur_hp = (int)query_stats("rifts_hp");
            max_hp = (int)query_stats("max_rifts_hp");
            if(max_hp > 0 && cur_hp < max_hp)
                set_stats("rifts_hp", cur_hp + 1);
        }
    } else {
        delay = 1800;
        if(!in_combat) {
            cur_hp = (int)query_stats("rifts_hp");
            max_hp = (int)query_stats("max_rifts_hp");
            if(max_hp > 0 && cur_hp < max_hp)
                set_stats("rifts_hp", cur_hp + 1);
        }
    }
    call_out("rifts_hp_regen_tick", delay);
}

string query_rp_title() {
    return (rp_title && strlen(rp_title)) ? rp_title : "";
}

void set_rp_title(string t) {
    if(this_player() && !creatorp(this_player())) return;
    rp_title = (t && strlen(t)) ? t : 0;
    save_player(query_name());
}

string *query_face_list() {
    if(!face_list) face_list = ({});
    return face_list + ({});
}

void add_to_face_list(string name) {
    if(!name || name == "") return;
    if(!face_list) face_list = ({});
    if(member_array(name, face_list) == -1)
        face_list += ({ name });
}

void remove_from_face_list(string name) {
    int idx;

    if(!name || name == "" || !face_list) return;
    idx = member_array(name, face_list);
    if(idx != -1)
        face_list = face_list[0..idx-1] + face_list[idx+1..];
}

int knows_player(string name) {
    if(!face_list) return 0;
    return member_array(name, face_list) != -1;
}

string query_display_name(object viewer) {
    string my_race;

    if(query_ghost()) return "A ghost";
    if(invis) return 0;

    if(!viewer || viewer == this_object() || creatorp(viewer) ||
       creatorp(this_object()))
        return query_cap_name();

    if(!(int)viewer->knows_player(query_name())) {
        my_race = (string)query_property("visible_race");
        if((!my_race || my_race == "") && (int)query_property("metamorphed")) {
            my_race = (string)query_property("apparent_race");
            if(!my_race || my_race == "")
                my_race = (string)query_property("current_form");
        }
        if(!my_race || my_race == "") my_race = query_race();
        /* secondary vampires appear human; wild vampires remain visibly monstrous */
        if(my_race && lower_case(my_race) == "secondary vampire" &&
           !archp(viewer))
            my_race = "human";
        if(!my_race || my_race == "") return "A stranger";
        switch(my_race[0]) {
        case 'a': case 'e': case 'i': case 'o': case 'u':
            return "An " + my_race;
        default:
            return "A " + my_race;
        }
    }
    return query_cap_name();
}

string query_visible_name(object viewer) {
    return query_display_name(viewer);
}
