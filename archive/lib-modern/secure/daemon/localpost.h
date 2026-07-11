#ifndef __LOCALPOST_H
#define __LOCALPOST_H

void load_groups();
varargs string *send_post(mapping borg, mixed who);
private nosave mixed map_groups(string str, mapping grps, string who, int x);
string *query_mud_group(string str);
mapping query_mud_groups();

#endif /* __LOCALPOST_H */
