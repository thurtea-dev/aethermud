/*    /secure/daemon/shutdown.c
 *    from Nightmare 3.3
 *    the daemon which handles all shutdown nonsense
 *    created by Descartes of Borg 940924
 */

#include <std.h>
#include <rooms.h>
#include <commands.h>
#include <objects.h>
#include "shutdown.h"

inherit DAEMON;

static private object *__Notify;

void create() {
    daemon::create();
    __Notify = ({});
}

void reboot_mud(int x) {
    if(x < 2) return;
    if(!((int)master()->valid_apply(({}))) && file_name(previous_object(0)) !=
      CMD_END) return;
    catch(new(OB_SHUT)->move(ROOM_START));
    call_out("countdown", 60, x-1);
    message("broadcast", "Driver: server going down for reboot in "+
      x+" minutes.", users());
}

static void countdown(int x) {
    if(x == 1) call_out("final_warning", 50);
    else call_out("countdown", 60, x-1);
    message("broadcast", "Driver: server going down for reboot in "+
      consolidate(x, "a minute")+".", users());
    notify_listeners(x);
}

static void final_warning() {
    call_out("reboot", 10);
    message("broadcast", "Driver: final warning, reboot in 10 seconds.",
      users());
    notify_listeners(0);
}

static void reboot() {
    catch(users()->force_me("quit"));
    shutdown();
}

static private void notify_listeners(int x) {
    catch(__Notify->call_end(x*60));
}

void notify_armageddon(object ob) {
    __Notify = distinct_array(({ ob }) + __Notify) - ({ 0 });
}

