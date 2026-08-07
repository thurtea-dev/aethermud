#ifndef __HELP_H
#define __HELP_H

varargs void help(string topic, string category);
static void flat_help_display();
static void categories_display();
static void category_display(int sec);
static string category_title(int sec);
static void category_footer(int sec);
static string *category_shadowed_files(int sec);
static int match_category_name(string str);
static string squash_topic(string s);
static string squash_user_help_match(string sq);
static void print_help_section(string title, string *topics);
static string read_first_line(string path);
static string normalize_help_topic(string topic);
static string *dedupe_user_topics(string *topics);
static string help_alias_canonical(string topic);
static int help_index_junk(string topic);
static string topic_slug(string name);
static string find_user_help_file(string name);
static string *index_race_topics();
static string *index_occ_topics();
static int is_psionics_topic(string topic);
static int classify_topic(string topic, string first_line);
static void suggest_similar_topics(string topic);
static string *query_categories();
static void cmd_help(string topic, string category);
static int find_help(string topic, string category);

#endif /* __HELP_H */
