// /obj/items/atlantean_text.c
// A partial Atlantean manuscript found in the Lazlo restricted archive.
// Reading it reveals a fragment of the Aerihman name (clue 2).
// The player can carry it and read it anywhere.

#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_name("ancient atlantean text");
    set_id(({ "atlantean text", "ancient text", "manuscript", "text",
              "book", "atlantean manuscript", "restricted text",
              "fragment", "partial text" }));
    set_short("an ancient Atlantean text");
    set_long(
        "A bound manuscript, old enough that the binding is held together by\n"
        "little more than habit.  The pages are covered in precise Atlantean\n"
        "script  -  not the common post-Rifts variant but the older form,\n"
        "the one used by the Tattoo Masters before the Sundering.\n\n"
        "Most of the text appears to be a registry of some kind.  Names and\n"
        "designations.  Many are redacted with black ink.  One entry near\n"
        "the end of the document is only partially obscured.\n\n"
        "Type: read text  (or carry it and read it later)");
    set_mass(50);
    set_value(5000);
}

void init() {
    ::init();
    add_action("cmd_read", "read");
}

int cmd_read(string str) {
    if(!str) return 0;
    str = lower_case(str);
    if(member_array(str, ({"text", "book", "manuscript", "atlantean text",
                           "ancient text", "fragment",
                           "ancient atlantean text", "partial text"})) == -1)
        return 0;
    write("You turn to the partially-obscured entry near the back.\n\n"
          "The heavy black ink covers most of the line, but the tail end\n"
          "bled through, too pale to have been fully hidden:\n\n"
          "  '...the Order of ...HMAN  -  blade-born, shadow-sworn,\n"
          "   inscribed in the oldest tradition...'\n\n"
          "...HMAN.  Combined with what you found in the catacombs,\n"
          "something is forming.  A name.  An order.\n"
          "AERI...HMAN.\n\n"
          "You are one fragment away from speaking it aloud.\n");
    this_player()->setenv("aerihman_clue_2", "1");
    return 1;
}
