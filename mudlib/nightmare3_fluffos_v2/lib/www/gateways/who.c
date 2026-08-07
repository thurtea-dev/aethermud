/*    /www/gateways/who.c
 *    from Nightmare IV
 *    who WWW gateway
 *    created by Descartes of Borg 940610
 *    simplified for AetherMUD
 */

#include <std.h>

inherit DAEMON;

string gateway() {
    object *people, *who;
    string ret;
    int i, maxi;

    people = filter_array(users(), "filter_people", this_object());
    ret = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\">"
      "<title>" + mud_name() + " Who</title>"
      "<style>"
      "body{font-family:Georgia,\"Times New Roman\",serif;max-width:40rem;"
      "margin:2rem auto;padding:0 1rem;line-height:1.6;color:#222;"
      "background:#f7f5f0}"
      "h1{font-size:1.75rem;font-weight:normal}"
      "h2{font-size:1.1rem;font-weight:normal;margin-top:1.5rem;"
      "border-bottom:1px solid #ccc;padding-bottom:0.2rem}"
      "ul{padding-left:1.2rem;list-style:disc}"
      "a{color:#1a5c5c}"
      "</style></head><body>"
      "<h1>" + mud_name() + "</h1>"
      "<p>" + sizeof(people) + " visible players online.</p>"
      "<p><a href=\"/\">Home</a></p>";

    ret += "<h2>Arches</h2><ul>";
    if (!(maxi = sizeof(who = filter_array(people, "filter_admin",
      this_object()))))
        ret += "<li>None online.</li>";
    else
        for (i = 0; i < maxi; i++)
            ret += "<li>" + (string)who[i]->query_title() +
              (query_idle(who[i]) > 60 ? " (idle)" : "") + "</li>";
    ret += "</ul>";

    ret += "<h2>Creators</h2><ul>";
    if (!(maxi = sizeof(who = filter_array(people, "filter_creator",
      this_object()))))
        ret += "<li>None online.</li>";
    else
        for (i = 0; i < maxi; i++)
            ret += "<li>" + (string)who[i]->query_title() +
              (query_idle(who[i]) > 60 ? " (idle)" : "") + "</li>";
    ret += "</ul>";

    ret += "<h2>High mortals</h2><ul>";
    if (!(maxi = sizeof(who = filter_array(people, "filter_hm",
      this_object()))))
        ret += "<li>None online.</li>";
    else
        for (i = 0; i < maxi; i++)
            ret += "<li>" + (string)who[i]->query_title() +
              (query_idle(who[i]) > 60 ? " (idle)" : "") + "</li>";
    ret += "</ul>";

    ret += "<h2>Mortals</h2><ul>";
    if (!(maxi = sizeof(who = filter_array(people, "filter_mortal",
      this_object()))))
        ret += "<li>None online.</li>";
    else
        for (i = 0; i < maxi; i++)
            ret += "<li>" + (string)who[i]->query_title() +
              (query_idle(who[i]) > 60 ? " (idle)" : "") + "</li>";
    ret += "</ul></body></html>";
    return ret;
}

static int filter_people(object ob) {
    if (hiddenp(ob) || (int)ob->query_invis())
        return 0;
    if (!((string)ob->query_title()))
        return 0;
    return 1;
}

static int filter_admin(object ob) { return archp(ob); }

static int filter_creator(object ob) {
    return (wizardp(ob) && !archp(ob));
}

static int filter_hm(object ob) { return high_mortalp(ob); }

static int filter_mortal(object ob) {
    return (!wizardp(ob) && !high_mortalp(ob));
}
