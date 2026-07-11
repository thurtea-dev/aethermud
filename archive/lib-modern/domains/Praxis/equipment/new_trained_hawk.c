// /domains/Praxis/equipment/trained_hawk.c
// A trained hunting hawk. Obeys owner's commands based on their Falconry skill.
// Obtained from the falconer NPC in exchange for a leather falconry glove.
// Supports both direct action commands and owner bird-keyword say commands.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_pet";

private int __falconry_skill;

private int get_skill_val();
private int skill_check();
private void do_attack_target(object target);
private void do_scout_command(object owner);
void do_attack_command();
void do_hunt_command();

void create() {
    ::create();
    __falconry_skill = 0;
    set_name("hawk");
    set_id(({ "hawk", "bird", "falcon", "eagle", "trained hawk", "hunting hawk" }));
    set_short("a trained hunting hawk");
    set_long(
        "A large and powerful peregrine hawk with keen amber eyes and razor-\n"
        "sharp talons. Its feathers are dark brown with cream streaking. A\n"
        "thin leather jess is attached to one leg. When at rest it mantles\n"
        "quietly, watching everything in the room with predator focus.\n\n"
        "Its behavior depends on your Falconry skill. At high skill it will\n"
        "respond reliably; at low skill it may ignore commands.\n\n"
        "Commands: release hawk, call hawk, send hawk <target>, hawk scout,\n"
        "hawk perch, hawk stay, hawk come, hawk fly, hawk hunt, hawk attack."
    );
    set_mass(400);
    set_level(4);
    set_aggressive(0);
    set_moving(0);
    set_pet_name("the hawk");
    set_stats("str", 6);
    set_stats("dex", 14);
    set_hp(20);
    set_property("is_hawk", 1);
    set_property("is_pet", 1);
    set_property("hawk_released", 0);
    set_property("hawk_perched", 1);
}

string query_short() {
    if(query_property("hawk_released"))
        return "a hawk circling overhead";
    if(query_property("hawk_perched"))
        return "a hawk perched nearby";
    return "a hawk mantling quietly on a leather jess";
}

void set_falconry_skill(int v) { __falconry_skill = v; }
int query_falconry_skill() { return __falconry_skill; }

private int get_skill_val() {
    object owner;
    int val;

    owner = query_owner();
    if(!owner || !objectp(owner)) return __falconry_skill;
    val = (int)RIFTS_SKILLS_D->query_skill_percent(owner, "falconry");
    if(val <= 0) val = __falconry_skill;
    return val;
}

private int skill_check() {
    int skill;
    int roll;

    skill = get_skill_val();
    if(skill <= 0) return 0;
    roll = random(100) + 1;
    return (roll <= skill);
}

void init() {
    ::init();
    add_action("cmd_release", "release");
    add_action("cmd_call", "call");
    add_action("cmd_send", "send");
    add_action("cmd_hawk", "hawk");
    add_action("cmd_hawk_say", "say");
}

int cmd_release(string str) {
    object owner;

    owner = query_owner();
    if(!owner || owner != this_player()) return 0;
    if(!str || strsrch(str, "hawk") == -1) return 0;

    if(!query_property("hawk_perched") && query_property("hawk_released")) {
        write("The hawk is already circling overhead.\n");
        return 1;
    }
    set_property("hawk_perched", 0);
    set_property("hawk_released", 1);
    tell_room(environment(this_object()),
        (string)owner->query_cap_name() +
        " releases the hawk. It spreads its wings and launches skyward,\n"
        "circling overhead in slow, watchful spirals.\n");
    return 1;
}

int cmd_call(string str) {
    object owner;

    owner = query_owner();
    if(!owner || owner != this_player()) return 0;
    if(!str || strsrch(str, "hawk") == -1) return 0;

    if(!skill_check()) {
        tell_room(environment(this_object()),
            "The hawk tilts its head at the call but does not come.\n");
        return 1;
    }
    set_property("hawk_released", 0);
    set_property("hawk_perched", 1);
    set_stay_mode(0);
    if(environment(this_object()) != environment(owner))
        move(environment(owner));
    tell_room(environment(owner),
        "The hawk glides in and lands on " +
        (string)owner->query_cap_name() + "'s arm, mantling briefly.\n");
    return 1;
}

int cmd_send(string str) {
    object owner;
    object target;
    int skill;
    string tname;

    owner = query_owner();
    if(!owner || owner != this_player()) return 0;
    if(!str) return 0;
    if(strsrch(str, "hawk ") != 0) return 0;

    tname = str[5..];
    if(!strlen(tname)) {
        write("Send the hawk at whom? Use: send hawk <target>\n");
        return 1;
    }
    skill = get_skill_val();
    if(skill < 40) {
        write("Your Falconry skill is too low to reliably direct an attack. Train more before attempting this.\n");
        return 1;
    }
    target = present(tname, environment(owner));
    if(!target || !living(target)) {
        write("You don't see " + tname + " here.\n");
        return 1;
    }
    if(target == owner || target == this_object()) {
        write("You can't send the hawk at that target.\n");
        return 1;
    }
    if(!skill_check()) {
        tell_room(environment(this_object()),
            "The hawk screams and dives, then pulls up at the last moment,\n"
            "circling without connecting. Not reliable enough yet.\n");
        return 1;
    }
    do_attack_target(target);
    return 1;
}

int cmd_hawk(string str) {
    object owner;

    owner = query_owner();
    if(!owner || owner != this_player()) return 0;
    if(!str) return 0;

    if(str == "scout") {
        do_scout_command(owner);
        return 1;
    }
    if(str == "perch") {
        set_property("hawk_released", 0);
        set_property("hawk_perched", 1);
        set_stay_mode(1);
        if(environment(this_object()) != environment(owner))
            move(environment(owner));
        tell_room(environment(owner),
            "The hawk drops from the air and lands on " +
            (string)owner->query_cap_name() + "'s arm with a flutter of wings.\n");
        return 1;
    }
    if(str == "attack") {
        do_attack_command();
        return 1;
    }
    return 0;
}

private void do_attack_target(object target) {
    set_property("hawk_released", 1);
    set_property("hawk_perched", 0);
    set_stay_mode(0);
    tell_room(environment(this_object()),
        "The hawk screams and dives at " +
        (string)target->query_cap_name() + "!\n");
    attack(target);
}

void do_attack_command() {
    object *fighters;
    object target;

    if(!skill_check()) {
        tell_room(environment(this_object()),
            "The hawk circles but cannot identify a clear target.\n");
        return;
    }
    fighters = filter(all_inventory(environment(this_object())),
        (: living($1) && $1 != this_object() && $1 != query_owner() :));
    if(!sizeof(fighters)) {
        tell_room(environment(this_object()),
            "The hawk sweeps the area but finds no one to attack.\n");
        return;
    }
    target = fighters[random(sizeof(fighters))];
    do_attack_target(target);
}

private void do_scout_command(object owner) {
    string *dirs;
    mapping exits;
    object here;
    object adj_room;
    string report;
    string dir;
    string rname;
    int i;

    if(!skill_check()) {
        tell_room(environment(owner),
            "The hawk spirals upward but returns quickly, unable to relay\n"
            "what it saw. Your Falconry skill is not high enough.\n");
        return;
    }

    here = environment(owner);
    exits = (mapping)here->query_exits();
    if(!mapp(exits) || !sizeof(exits)) {
        tell_object(owner,
            "The hawk circles and returns. Nothing but open space in all directions.\n");
        return;
    }

    dirs = keys(exits);
    report = "Your hawk circles and returns. Scouting report:\n";
    i = sizeof(dirs);
    while(i--) {
        dir = dirs[i];
        adj_room = load_object(exits[dir]);
        if(adj_room && objectp(adj_room)) {
            rname = (string)adj_room->query_short();
            if(!rname || !strlen(rname)) rname = "an unknown area";
        } else {
            rname = "impassable terrain";
        }
        report += "  " + capitalize(dir) + ": " + rname + "\n";
    }
    tell_object(owner, report);
    tell_room(environment(owner),
        "The hawk glides out in widening arcs, then wheels back to " +
        (string)owner->query_cap_name() + ".\n",
        ({ owner }));
}

void do_hunt_command() {
    object candidate;
    int skill;

    skill = get_skill_val();
    if(skill < 30) {
        tell_room(environment(this_object()),
            "The hawk tilts its head, confused by the command.\n");
        return;
    }
    tell_room(environment(this_object()),
        "The hawk launches skyward and begins hunting.\n");
    if(skill >= 60) {
        candidate = clone_object("/domains/Praxis/monsters/rat");
        if(candidate) {
            call_out( (: this_object()->return_with_prey, candidate :), 8);
        }
    }
}

void return_with_prey(object prey) {
    object owner;

    owner = query_owner();
    if(!owner) return;
    if(prey && objectp(prey)) {
        prey->move(environment(owner));
        tell_room(environment(owner),
            "The hawk returns and drops a freshly killed " +
            (string)prey->query_short() + " at " +
            (string)owner->query_cap_name() + "'s feet.\n");
    } else {
        tell_object(owner, "The hawk circles back. It found nothing this time.\n");
        this_object()->move(environment(owner));
    }
}

int catch_say(string msg, object speaker) {
    string cmd;
    string bird;
    string verb;
    object owner;
    int pos;

    owner = query_owner();
    if(!owner || !objectp(owner)) return 0;
    if(speaker != owner) return 0;
    cmd = lower_case(msg);
    pos = strsrch(cmd, " ");
    if(pos == -1) return 0;

    bird = cmd[0..pos-1];
    verb = cmd[pos+1..];

    if(bird != "hawk" && bird != "falcon" && bird != "eagle")
        return 0;

    if(verb == "attack" || verb == "attack!") {
        do_attack_command();
        return 1;
    }
    if(verb == "stay") {
        set_stay_mode(1);
        set_property("hawk_perched", 1);
        set_property("hawk_released", 0);
        tell_room(environment(this_object()),
            "The hawk mantles its wings and settles where it stands.\n");
        return 1;
    }
    if(verb == "come") {
        set_stay_mode(0);
        set_property("hawk_released", 0);
        set_property("hawk_perched", 1);
        if(environment(this_object()) != environment(owner))
            move(environment(owner));
        tell_room(environment(owner),
            "The hawk glides to " + (string)owner->query_cap_name() + " and perches.\n");
        return 1;
    }
    if(verb == "hunt") {
        do_hunt_command();
        return 1;
    }
    if(verb == "fly" || verb == "release") {
        cmd_release("hawk");
        return 1;
    }
    if(verb == "perch") {
        cmd_hawk("perch");
        return 1;
    }
    return 0;
}

void heart_beat() {
    ::heart_beat();
    follow_owner();
    eat_from_room();
}
'''
Path('/tmp/trained_hawk_rewrite.c').write_text(trained, encoding='utf-8')
custom = r'''// /domains/Praxis/npcs/thurtea_hawk.c
// Thurtea's personal hawk. Bound to Thurtea. Responds to her bird-keyword commands.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_pet";

private int __falconry_skill;

private int get_skill_val();
private int skill_check();
private string parse_bird_verb(string str);
private void do_say_attack();
private void do_say_return();
private void do_say_stay();
private void do_say_fetch(string tname);

void create() {
    ::create();
    __falconry_skill = 0;
    set_name("hawk");
    set_id(({ "hawk", "bird", "falcon", "eagle", "thurtea's hawk", "personal hawk" }));
    set_short("a majestic hawk");
    set_long(
        "A majestic hawk with keen amber eyes and dark brown feathers streaked\n"
        "with cream. A thin leather jess is attached to one leg. At rest it\n"
        "mantaltes quietly, watching the room with predator focus.\n"
    );
    set_mass(400);
    set_level(4);
    set_aggressive(0);
    set_moving(0);
    set_pet_name("the hawk");
    set_stats("str", 6);
    set_stats("dex", 14);
    set_hp(20);
    set_property("is_hawk", 1);
    set_property("is_pet", 1);
    set_property("owner_name", "thurtea");
    set_property("hawk_released", 0);
    set_property("hawk_perched", 1);
}

string query_short() {
    if(query_property("hawk_released"))
        return "a majestic hawk circling overhead";
    if(query_property("hawk_perched"))
        return "a majestic hawk perched nearby";
    return "a majestic hawk mantling quietly on a leather jess";
}

private int get_skill_val() {
    object owner;
    int val;

    owner = query_owner();
    if(!owner || !objectp(owner)) return __falconry_skill;
    val = (int)RIFTS_SKILLS_D->query_skill_percent(owner, "falconry");
    if(val <= 0) val = __falconry_skill;
    return val;
}

private int skill_check() {
    int skill;
    int roll;

    skill = get_skill_val();
    if(skill <= 0) return 0;
    roll = random(100) + 1;
    return (roll <= skill);
}

void init() {
    ::init();
    if(this_player() &&
       lower_case((string)this_player()->query_name()) == "thurtea") {
        if(!query_owner() || !objectp(query_owner()))
            set_owner(this_player());
    }
    add_action("cmd_hawk_say", "say");
}

private string parse_bird_verb(string str) {
    string msg;
    int pos;

    msg = lower_case(str);
    pos = strsrch(msg, " ");
    if(pos == -1) return 0;
    if(msg[0..pos-1] != "hawk" && msg[0..pos-1] != "falcon" && msg[0..pos-1] != "eagle")
        return 0;
    return msg[pos+1..];
}

private void do_say_attack() {
    object *foes;
    object target;

    if(!skill_check()) {
        tell_room(environment(this_object()),
            "The hawk tilts its head at the command and ignores it.\n");
        return;
    }
    foes = filter(all_inventory(environment(this_object())),
        (: living($1) && $1 != this_object() && $1 != query_owner() :));
    if(!sizeof(foes)) {
        tell_room(environment(this_object()),
            "The hawk sweeps the area but finds nothing to attack.\n");
        return;
    }
    target = foes[random(sizeof(foes))];
    set_property("hawk_released", 1);
    set_property("hawk_perched", 0);
    set_stay_mode(0);
    tell_room(environment(this_object()),
        "The hawk screams and dives at " + (string)target->query_cap_name() + "!\n");
    this_object()->kill_ob(target, 0);
}

private void do_say_return() {
    object owner;

    owner = query_owner();
    if(!skill_check()) {
        tell_room(environment(this_object()),
            "The hawk ignores the call.\n");
        return;
    }
    set_stay_mode(0);
    set_property("hawk_released", 0);
    set_property("hawk_perched", 1);
    if(environment(this_object()) != environment(owner))
        move(environment(owner));
    tell_room(environment(owner),
        "The hawk glides to " + (string)owner->query_cap_name() + " and perches.\n");
}

private void do_say_stay() {
    if(!skill_check()) {
        tell_room(environment(this_object()),
            "The hawk ruffles its feathers and ignores you.\n");
        return;
    }
    set_stay_mode(1);
    set_property("hawk_released", 0);
    set_property("hawk_perched", 1);
    tell_room(environment(this_object()),
        "The hawk mantles its wings and settles in place.\n");
}

private void do_say_fetch(string tname) {
    object owner;
    object item;

    owner = query_owner();
    if(!strlen(tname)) {
        tell_object(owner, "Fetch what? Try: say hawk fetch <item>\n");
        return;
    }
    if(!skill_check()) {
        tell_room(environment(this_object()),
            "The hawk cocks its head and does nothing.\n");
        return;
    }
    item = present(tname, environment(this_object()));
    if(!item || living(item)) {
        tell_object(owner, "The hawk cannot find that here.\n");
        return;
    }
    item->move(owner);
    tell_room(environment(owner),
        "The hawk snatches up " + (string)item->query_short() +
        " and drops it at " + (string)owner->query_cap_name() + "'s feet.\n");
}

int cmd_hawk_say(string str) {
    string verb;
    object owner;

    owner = query_owner();
    if(!owner || owner != this_player()) return 0;
    if(!str || !strlen(str)) return 0;

    verb = parse_bird_verb(str);
    if(!verb) return 0;

    if(verb == "attack" || verb == "attack!") {
        do_say_attack();
        return 0;
    }
    if(verb == "return" || verb == "come") {
        do_say_return();
        return 0;
    }
    if(verb == "stay") {
        do_say_stay();
        return 0;
    }
    if(verb == "fetch" && strlen(lower_case(str)) > 11) {
        do_say_fetch(lower_case(str)[11..]);
        return 0;
    }
    if(verb == "fly" || verb == "release") {
        set_stay_mode(0);
        set_property("hawk_perched", 0);
        set_property("hawk_released", 1);
        tell_room(environment(this_object()),
            "The hawk launches skyward and circles overhead.\n");
        return 0;
    }
    if(verb == "perch") {
        set_stay_mode(1);
        set_property("hawk_released", 0);
        set_property("hawk_perched", 1);
        tell_room(environment(this_object()),
            "The hawk settles down and perches nearby.\n");
        return 0;
    }
    if(verb == "hunt") {
        tell_room(environment(this_object()),
            "The hawk watches for prey.\n");
        return 0;
    }
    return 0;
}

void heart_beat() {
    object *inv;
    int i;

    ::heart_beat();
    follow_owner();
    eat_from_room();
    if(!query_owner() || !objectp(query_owner())) {
        inv = all_inventory(environment(this_object()));
        for(i = 0; i < sizeof(inv); i++) {
            if(living(inv[i]) && lower_case((string)inv[i]->query_name()) == "thurtea") {
                set_owner(inv[i]);
                break;
            }
        }
    }
}