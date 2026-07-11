// /domains/Lazlo/areas/lazlo_library.c
// The Great Library of Lazlo -- largest repository of pre- and post-Rifts knowledge.

#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_short("Lazlo, Great Library entrance");
    set_long(
        "The entrance hall of the Great Library of Lazlo. High ceilings of\n"
        "polished stone, rows of reading tables, and corridors branching in\n"
        "every direction filled with shelves from floor to ceiling. This is\n"
        "the largest collection of pre-Rifts and post-Rifts knowledge on the\n"
        "continent.\n\n"
        "The scholars here are quiet, focused, and largely indifferent to\n"
        "you unless you engage them. A librarian at the front desk, a\n"
        "white-haired Elf with reading spectacles and ink-stained fingers,\n"
        "looks up and nods toward a directory board near the entrance.\n\n"
        "The market quarter is south. The arrival courtyard is further south.");
    set_exits( ([
        "south" : "/domains/Lazlo/areas/lazlo_square"
    ]) );
    set_listen("default",
        "The scratch of pen on paper. Quiet voices asking questions. Pages turning.");
    set_smell("default",
        "Old paper, binding glue, lamp oil, and the dry cool smell of preserved knowledge.");
    set_items( ([
        "shelves" : "Floor-to-ceiling shelves packed with bound volumes, scrolls, and data-slates.",
        "volumes" : "Thousands of books. Pre-Rifts science, post-Rifts history, magical theory, dimensional records.",
        "librarian" :
            "An Elven woman with white hair pulled back severely. Reading spectacles on her nose,\n"
            "ink on her fingers. She keeps track of everything in this building.",
        "tables" : "Long reading tables, most occupied. Someone is copying a map. Someone else has three books open at once.",
        "directory" :
            "A notice board:\n"
            " Level 1 - General reference, post-Rifts history\n"
            " Level 2 - Pre-Rifts science and engineering archives\n"
            " Level 3 - Magical theory and dimensional research\n"
            " Restricted - Access by authorization only",
        "scholars" : "Researchers from across the continent. They are here to learn, not to fight.",
        "spectacles" : "Old-fashioned reading spectacles. The librarian's preferred tool.",
        "ancient text": "A damaged manuscript on the reading table, left out by\n"
                        "a scholar who did not finish. The script is pre-Rifts Atlantean.\n"
                        "You can pick it up and read it.",
        "restricted text": "The librarian mentioned someone left a restricted text\n"
                           "unattended. It is on the reading table near the north shelves."
    ]) );
}

void reset() {
    object book;
    object *obs;
    int count;
    int j;
    ::reset();
    if(!present("atlantean text", this_object())) {
        book = new("/obj/items/atlantean_text");
        book->move(this_object());
    }
    obs   = all_inventory(this_object());
    count = 0;
    for(j = 0; j < sizeof(obs); j++) {
        if(living(obs[j]) &&
           strsrch((string)obs[j]->query_short(), "Lazlo scholar") != -1)
            count++;
    }
    while(count < 2) {
        clone_object("/domains/Lazlo/npcs/lazlo_scholar.c")->move(this_object());
        count++;
    }
}
