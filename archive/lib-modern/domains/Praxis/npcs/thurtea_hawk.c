// /domains/Praxis/npcs/thurtea_hawk.c
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
void hawk_do_say_attack();
void hawk_do_say_return();
void hawk_do_say_stay();
void hawk_do_say_fetch(string tname);
void hawk_do_say_fly();
void hawk_do_say_perch();
void hawk_do_say_scout();
void return_with_prey(object prey);

void create() {
    ::create();
    __falconry_skill = 0;
    set_name("hawk");
    set_id(({ "hawk", "bird", "falcon", "eagle", "thurtea's hawk", "personal hawk" }));
    set_short("a majestic hawk");
    set_long(
        "A majestic hawk with keen amber eyes and dark brown feathers streaked\n"
        "with cream. A thin leather jess is attached to one leg. When at rest\n"
        "it mantles quietly, watching the room with predator focus.\n"
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
    sync_hawk_position(0);
}

string query_short() {
    return "a majestic hawk";
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
    if(this_player() && lower_case((string)this_player()->query_name()) == "thurtea") {
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
    object room;
    int i;

    room = environment(this_object());
    if(!room) return;

    foes = all_inventory(room);
    for(i = sizeof(foes) - 1; i >= 0; i--) {
        if(!living(foes[i]) || foes[i] == this_object() || foes[i] == query_owner()) {
            foes = foes[0..i-1] + foes[i+1..];
        }
    }

    if(!sizeof(foes)) {
        tell_room(room, "The hawk sweeps the area but finds nothing to attack.\n");
        return;
    }

    target = foes[random(sizeof(foes))];
    set_property("hawk_released", 1);
    set_property("hawk_perched", 0);
    set_stay_mode(0);
    sync_hawk_position(0);

    tell_room(room, "The hawk screams and dives at " + (string)target->query_cap_name() + "!\n");
    this_object()->attack_ob(target);
}

private void do_say_return() {
    object owner;
    owner = query_owner();
    if(!owner) return;

    set_stay_mode(0);
    set_property("hawk_released", 0);
    set_property("hawk_perched", 1);
    sync_hawk_position(0);

    if(environment(this_object()) != environment(owner))
        move(environment(owner));

    tell_room(environment(owner),
        "The hawk glides to " + (string)owner->query_cap_name() + " and perches.\n");
}

private void do_say_stay() {
    set_stay_mode(1);
    set_property("hawk_released", 0);
    set_property("hawk_perched", 1);
    sync_hawk_position(0);
    tell_room(environment(this_object()), "The hawk mantles its wings and settles in place.\n");
}

void hawk_do_say_attack() { do_say_attack(); }
void hawk_do_say_return() { do_say_return(); }
void hawk_do_say_stay() { do_say_stay(); }
void hawk_do_say_fetch(string tname) { do_say_fetch(tname); }

void hawk_do_say_fly() {
    set_stay_mode(0);
    set_property("hawk_released", 1);
    set_property("hawk_perched", 0);
    sync_hawk_position(0);
    tell_room(environment(this_object()),
        "The hawk launches skyward and circles overhead.\n");
}

void hawk_do_say_perch() {
    set_stay_mode(1);
    set_property("hawk_released", 0);
    set_property("hawk_perched", 1);
    sync_hawk_position(0);
    tell_room(environment(this_object()),
        "The hawk settles down and perches nearby.\n");
}

void hawk_do_say_scout() {
    tell_room(environment(this_object()), "The hawk watches for prey.\n");
}

private void do_say_fetch(string tname) {
    object owner;
    object item;

    owner = query_owner();
    if(!strlen(tname)) {
        tell_object(owner, "Fetch what? Try: say hawk fetch <item>\n");
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
        hawk_say_and_queue(str, "hawk_do_say_attack");
        return 1;
    }
    if(verb == "return" || verb == "come") {
        hawk_say_and_queue(str, "hawk_do_say_return");
        return 1;
    }
    if(verb == "stay") {
        hawk_say_and_queue(str, "hawk_do_say_stay");
        return 1;
    }
    if(verb == "fetch" && strlen(lower_case(str)) > 11) {
        hawk_say_and_queue(str, "hawk_do_say_fetch", lower_case(str)[11..]);
        return 1;
    }
    if(verb == "fly" || verb == "release") {
        hawk_say_and_queue(str, "hawk_do_say_fly");
        return 1;
    }
    if(verb == "perch") {
        hawk_say_and_queue(str, "hawk_do_say_perch");
        return 1;
    }
    if(verb == "scout") {
        hawk_say_and_queue(str, "hawk_do_say_scout");
        return 1;
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