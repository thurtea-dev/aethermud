#ifndef __HELP_H
#define __HELP_H

varargs void help(string topic, string category, int menu);
protected void flat_help_display();
protected void print_help_section(string title, string *topics);
protected string read_first_line(string path);
protected string normalize_help_topic(string topic);
protected string *dedupe_user_topics(string *topics);
protected int classify_topic(string topic, string first_line);
protected void suggest_similar_topics(string topic);
protected void help_menu(string category, string *topics, int ind);
protected string *query_categories();
protected string *query_topics(string category);
protected void cmd_help(string topic, string category, int menu);
protected int find_help(string topic, string category, int menu);
protected void menu_return(string duh, string category);
void endmore(string category);

#endif /* __HELP_H */
