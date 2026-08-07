// /std/rifts_ambient.c
// Mixin for ambient room messages.  Inherit in a room and call start_ambient() from reset().

private string *__ambient_msgs;
private int     __ambient_running;

void set_ambient_messages(string *msgs) {
    __ambient_msgs = msgs;
}

string *query_ambient_messages() {
    return __ambient_msgs ? __ambient_msgs : ({});
}

void ambient_tick();

void start_ambient() {
    int delay;

    if(__ambient_running) return;
    __ambient_running = 1;
    delay = 60 + random(61);
    call_out("ambient_tick", delay);
}

void stop_ambient() {
    __ambient_running = 0;
    remove_call_out("ambient_tick");
}

void ambient_tick() {
    string msg;
    int delay;

    if(!__ambient_running) return;
    if(!__ambient_msgs || !sizeof(__ambient_msgs)) {
        __ambient_running = 0;
        return;
    }
    msg = __ambient_msgs[random(sizeof(__ambient_msgs))];
    tell_room(this_object(), msg + "\n");
    delay = 60 + random(61);
    call_out("ambient_tick", delay);
}
