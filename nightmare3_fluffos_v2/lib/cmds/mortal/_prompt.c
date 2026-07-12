// /cmds/mortal/_prompt.c
// View and manage your command prompt.
// Wraps the PROMPT environment variable with a friendlier interface.

#include <std.h>

inherit DAEMON;

int cmd_prompt(string str) {
    string cur;

    if(!str || str == "") {
        cur = (string)this_player()->getenv("PROMPT");
        if(stringp(cur) && cur != "") {
            write("Your custom prompt: \"" + cur + "\"\n"
                  "Use 'prompt default' to return to the automatic "
                  "pool display.\n");
        } else {
            cur = (string)this_player()->query_rifts_prompt();
            if(!stringp(cur) || cur == "") cur = "> ";
            write("Prompt: (default - auto pool display)\n"
                  "Currently renders as: " + cur + "\n");
        }
        return 1;
    }
    if(str == "default" || str == "reset") {
        this_player()->remove_env("PROMPT");
        this_player()->reset_prompt();
        write("Prompt reset to the automatic pool display.\n");
        return 1;
    }
    if(str == "tokens") {
        write(
            "Prompt tokens (use in 'prompt <string>'):\n\n"
            "  $N / $n   Your name (capitalized / lowercase)\n"
            "  $M / $m   Mud name (capitalized / lowercase)\n"
            "  $C        Command number\n"
            "  $V / $v   INVIS/HID marker when invisible or hidden\n"
            "  $D        Current directory (wizards only)\n\n"
            "Rifts pools (current value lowercase, maximum uppercase):\n"
            "  $h / $H   Hit points (legacy pool)\n"
            "  $g / $G   Magic points (legacy pool)\n"
            "  $i / $I   Spell points (legacy pool)\n"
            "  $s / $S   SDC\n"
            "  $p / $P   PPE\n"
            "  $j / $J   ISP\n"
            "  $k / $K   MDC\n\n"
            "Example: prompt [$k/$K MDC $p/$P PPE]>\n");
        return 1;
    }
    this_player()->setenv("PROMPT", str);
    this_player()->reset_prompt();
    write("Prompt set to: \"" + str + "\"\n");
    return 1;
}

void help() {
    write(
        "Syntax: prompt\n"
        "        prompt <string>\n"
        "        prompt default\n"
        "        prompt tokens\n\n"
        "With no argument, shows your current prompt setting.\n"
        "With a string, sets a custom prompt (tokens are expanded;\n"
        "see 'prompt tokens' for the list).\n"
        "'prompt default' (or 'prompt reset') clears your custom\n"
        "prompt and returns to the automatic display, which shows\n"
        "the stat pools that apply to your race and OCC, for example\n"
        "MDC:450/450 PPE:120/120> or HP:80/80 SDC:200/200 ISP:60/60>\n\n"
        "Note: the words 'default', 'reset', and 'tokens' are reserved\n"
        "by this command. To set a prompt that literally starts with\n"
        "one of them, use: set PROMPT <string>\n\n"
        "See also: sbar, set, unset\n");
}
