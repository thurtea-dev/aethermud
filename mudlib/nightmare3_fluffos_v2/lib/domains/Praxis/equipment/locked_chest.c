// /domains/Praxis/equipment/locked_chest.c
// A locked chest requiring Pick Locks skill (difficulty 100) to open.
// Contains items set via set_chest_contents().

#include <std.h>
#include <daemons.h>

inherit CONTAINER;

private int __locked;
private string *__content_files;

void create() {
    ::create();
    set_name("chest");
    set_id( ({ "chest", "locked chest", "metal chest", "container" }) );
    set_short("a heavy locked chest");
    set_long(
        "A heavy metal chest sealed with a combination lock. Old military\n"
        "manufacture, pre-Rifts CS surplus by the look of it. The lock is\n"
        "serious hardware. A skilled lock picker could get it open.\n"
    );
    set_mass(2000);
    set_max_encumbrance(5000);
    __locked = 1;
    __content_files = ({});
    set_closed(1);
}

void set_chest_contents(string *files) { __content_files = files; }
int query_locked() { return __locked; }

int can_open() {
    if(__locked) {
        write("The chest is locked.\n");
        return 0;
    }
    return 1;
}

int cmd_pick(string str) {
    int skill_val;
    int roll;
    object bearer;

    if(!str || strsrch(lower_case(str), "lock") == -1 &&
               strsrch(lower_case(str), "chest") == -1) return 0;
    if(!__locked) {
        write("The chest is already open.\n");
        return 1;
    }
    bearer = this_player();
    skill_val = (int)RIFTS_SKILLS_D->query_skill_pct(bearer, "pick locks");
    if(skill_val <= 0) {
        write("You have no idea how to pick a lock.\n");
        return 1;
    }
    roll = random(100) + 1;
    if(roll <= skill_val && skill_val >= 100) {
        __locked = 0;
        set_closed(0);
        write("Click. The lock gives way. The chest swings open.\n");
        say((string)bearer->query_cap_name() + " picks the lock on a chest with practiced ease.");
        if(__content_files && sizeof(__content_files) > 0) {
            int i;
            object ob;
            for(i = 0; i < sizeof(__content_files); i++) {
                if(file_size(__content_files[i] + ".c") > 0) {
                    ob = clone_object(__content_files[i]);
                    if(ob) ob->move(this_object());
                }
            }
            __content_files = ({});
        }
    } else if(roll <= skill_val) {
        __locked = 0;
        set_closed(0);
        write("After several tense minutes, the lock clicks open.\n");
        say((string)bearer->query_cap_name() + " carefully picks the lock on a chest.");
        if(__content_files && sizeof(__content_files) > 0) {
            int i;
            object ob;
            for(i = 0; i < sizeof(__content_files); i++) {
                if(file_size(__content_files[i] + ".c") > 0) {
                    ob = clone_object(__content_files[i]);
                    if(ob) ob->move(this_object());
                }
            }
            __content_files = ({});
        }
    } else {
        write("You work at the lock but it holds firm.\n");
    }
    return 1;
}

void init() {
    ::init();
    add_action("cmd_pick", "pick");
}
