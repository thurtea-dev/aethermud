/* /domains/adm/wiz_tools/card_format_d.c
   Shared ASCII box-frame renderer for the static wiz-tool reference
   cards in the staff supplies chest (admin cheat sheet, role
   handbooks). Wraps arbitrary text lines to fit inside an 80-column
   (by default) frame so nothing spills past the border or gets cut
   mid-phrase by client-side wrapping.

   Callers pass a flat array of logical content lines: a blank string
   for a blank spacer line, a bare line for a paragraph/heading, or a
   line with leading spaces for an indented item (typically a command
   description under its own syntax line above it). Any line longer
   than the frame interior word-wraps using FluffOS sprintf's own
   column mode (the "=" flag - see wrap() in secure/SimulEfun/
   strings.c), preserving that line's own leading indent as a hanging
   indent on every continuation line, so wrapped text never breaks
   mid-word and stays visually grouped under its heading. */

#include <std.h>

inherit DAEMON;

#define CARD_WIDTH 80

private string repeat_pad(string ch, int n);
private string render_line(string line, int inner);

varargs string render_card(string title, string *lines, int width) {
    string out;
    string border;
    int inner;
    int i;

    if(!width) width = CARD_WIDTH;
    inner = width - 4;
    border = "+" + repeat_pad("-", width - 2) + "+";
    out = border + "\n";
    out += "| " + center(title, inner) + " |\n";
    out += border + "\n";
    for(i = 0; i < sizeof(lines); i++)
        out += render_line(lines[i], inner);
    out += border;
    return out;
}

/* Wraps and frames one logical content line. May expand into several
   physical (already bordered/padded) output lines if it needs to
   word-wrap to fit. */
private string render_line(string line, int inner) {
    string indent;
    string body;
    string wrapped;
    string *plines;
    string out;
    int lead;
    int j;

    if(!line || line == "")
        return "| " + repeat_pad(" ", inner) + " |\n";
    lead = 0;
    while(lead < strlen(line) && line[lead] == ' ') lead++;
    indent = (lead > 0) ? line[0..lead-1] : "";
    body = line[lead..];
    wrapped = wrap(body, inner - lead);
    plines = explode(wrapped, "\n");
    out = "";
    for(j = 0; j < sizeof(plines); j++) {
        /* wrap() always appends a trailing "\n" after the last
           wrapped segment, so explode() always leaves one trailing
           empty element to discard here. */
        if(plines[j] == "" && j == sizeof(plines) - 1) continue;
        out += "| " + arrange_string(indent + plines[j], inner) + " |\n";
    }
    return out;
}

private string repeat_pad(string ch, int n) {
    string s;
    int i;

    s = "";
    for(i = 0; i < n; i++) s += ch;
    return s;
}
