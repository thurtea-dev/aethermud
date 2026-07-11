/* /realms/thurtea/adm/access.c
 * Realm access control for thurtea's wizard realm.
 */

#include <std.h>

inherit DAEMON;

nomask int check_access(object ob, string fun, string file, string oper) {
    if(!ob) return 0;
    if(lower_case((string)ob->query_name()) == "thurtea") return 1;
    return 0;
}
