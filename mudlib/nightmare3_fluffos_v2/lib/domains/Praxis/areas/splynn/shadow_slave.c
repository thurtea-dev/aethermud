// /domains/Praxis/areas/splynn/shadow_slave.c
// A scarred slave in the Splynn market who whispers about a shadow clan.
// Provides Aerihman clue 3.

#include <std.h>
#include <daemons.h>

inherit MONSTER;

void create() {
    ::create();
    set_name("scarred slave");
    set_id(({ "slave", "scarred slave", "man", "human", "scarred man",
              "tattooed man" }));
    set_short("a scarred slave");
    set_long(
        "A human slave, male, late middle age. He has survived long enough in\n"
        "Splynn to become invisible to the guards. His skin is covered in old\n"
        "burn scars, but beneath them you can see the faint ghost of tattoos -\n"
        "not the crude slave markings the Splugorth use, but something else.\n"
        "Something deliberate. He avoids your eyes, but does not entirely avoid you.");
    set_race("human");
    set_level(3);
    set_alignment(-50);
    set_stats("strength", 10);
    set_stats("dexterity", 12);
    set_stats("intelligence", 14);
    set_hp(20);
    set_property("no_attack", 1);
    set_property("passive", 1);
    set_property("aggressive", 0);
}

void init() {
    ::init();
    add_action("cmd_talk", "talk");
    add_action("cmd_talk", "ask");
    add_action("cmd_examine", "examine");
}

int cmd_examine(string str) {
    if(!str) return 0;
    str = lower_case(str);
    if(member_array(str, ({"tattoos", "scars", "marks", "skin", "tattoo"})) == -1)
        return 0;
    write("The slave notices you looking at his scarred skin. He turns slightly,\n"
          "just enough that you can see more clearly. Beneath the burn scars,\n"
          "the ghost of tattoo ink is unmistakable - not decorative.\n"
          "The patterns are deliberate, geometric, Atlantean.\n\n"
          "He catches your eye. One word, barely a breath:\n\n"
          " \"Aerihman.\"\n\n"
          "Then he looks away as a guard passes.\n");
    this_player()->setenv("aerihman_clue_3", "1");
    return 1;
}

int cmd_talk(string str) {
    if(!str) return 0;
    str = lower_case(str);
    if(strsrch(str, "shadow") != -1 || strsrch(str, "tattoo") != -1 ||
       strsrch(str, "clan") != -1 || strsrch(str, "order") != -1 ||
       strsrch(str, "aerihman") != -1) {
        write("The scarred man glances at the nearest guard, then at your eyes.\n"
              "He does not smile. He does not run. He waits until you are\n"
              "standing close, then speaks without moving his lips much:\n\n"
              " \"There are those who were Tattooed before Atlantis fell.\n"
              " Not all of them are gone. Their order has a name.\n"
              " AERIHMAN. Say it only when you are sure you are ready\n"
              " to be known by it.\"\n\n"
              "He moves away to carry a crate. The conversation is over.\n");
        this_player()->setenv("aerihman_clue_3", "1");
        return 1;
    }
    write("The slave glances at you but says nothing useful. He is watching\n"
          "the guards. Try asking about shadows, tattoos, or the order.\n");
    return 1;
}
