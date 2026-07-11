#ifndef __CONTAINER_H
#define __CONTAINER_H

void create();
void set_transparent(int n);
int query_transparent();
void set_closed(int n);
int query_closed();
int query_contents_visible();
varargs string look_in(object who);
varargs int receive_objects(object ob);
int release_objects(object ob);
string describe_living_contents(object *exclude);
string describe_item_contents(object *exclude);
void add_encumbrance(int x);
int query_encumbrance();
void set_max_encumbrance(int x);
int query_max_encumbrance();
float query_encumbered();
object *query_living_contents();
object *query_item_contents();
protected int filter_living(object ob);
protected int filter_items(object ob);

#endif /* __CONTAINER_H */
