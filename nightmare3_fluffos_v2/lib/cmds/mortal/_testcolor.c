/* /cmds/mortal/_testcolor.c
   Prints a sample of every named color tag so a player can check what
   their terminal actually displays after toggling colorize. */

#include <std.h>

inherit DAEMON;

int cmd_testcolor(string str) {
    string *fg;
    string *bg;
    int i;

    write("Color test. If you see plain text with %^ markers instead of\n"
          "colors below, type 'colorize' first.\n\n");

    fg = ({ "BLACK", "RED", "GREEN", "ORANGE", "YELLOW", "BLUE", "CYAN",
            "MAGENTA", "WHITE" });
    for(i = 0; i < sizeof(fg); i++)
        write("%^" + fg[i] + "%^" + fg[i] + "%^RESET%^\n");

    write("\n");

    bg = ({ "B_RED", "B_GREEN", "B_ORANGE", "B_YELLOW", "B_BLUE", "B_CYAN",
            "B_BLACK", "B_WHITE", "B_MAGENTA" });
    for(i = 0; i < sizeof(bg); i++)
        write("%^" + bg[i] + "%^" + bg[i] + "%^RESET%^\n");

    write("\n%^BOLD%^BOLD%^RESET%^\n");
    return 1;
}

void help() {
    write(
        "Syntax: testcolor\n\n"
        "Prints a sample of every named color so you can check what your\n"
        "terminal actually displays. Use 'colorize' first if colors\n"
        "do not show up.\n\n"
        "See also: colorize\n"
    );
}
