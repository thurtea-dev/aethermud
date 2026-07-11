// /std/obj/magic_net.c
// Glowing magic net from the magic net spell or net gun.
// Has MDC; victim can keep fighting until the net is destroyed or times out.

#include <std.h>

inherit "/std/Object";

#define NET_DURATION 120
#define DEFAULT_NET_MDC 30

private object __victim;
private object __caster;
private int __net_mdc;

void release_victim(string msg);

void create() {
    ::create();
    __victim = 0;
    __caster = 0;
    __net_mdc = DEFAULT_NET_MDC;
    set_name("magic net");
    set_id(({ "net", "magic net", "glowing net", "strands" }));
    set_short("a glowing magic net");
    set_long(
        "Glowing magical strands woven into a capture net. It pulses with\n"
        "PPE energy. The net has its own structural integrity and can be\n"
        "shredded by sustained attacks against whoever it holds.\n");
    set_mass(1);
    set_property("is_magic_net", 1);
}

object query_victim() { return __victim; }

int query_net_mdc() { return __net_mdc; }

void release_victim(string msg) {
    object room;
    object v;

    v = __victim;
    if(v && objectp(v)) {
        v->remove_property("entangled");
        v->remove_property("cannot_move");
        v->remove_property("magic_net_obj");
        tell_object(v, "The magic net falls away. You are free!\n");
    }
    room = environment(this_object());
    if(room && msg && strlen(msg))
        tell_room(room, msg);
    __victim = 0;
    if(this_object()) destruct(this_object());
}

void timed_release() {
    release_victim("The magic net dissolves into sparks of PPE.\n");
}

void take_net_damage(int dmg) {
    if(dmg < 1) return;
    __net_mdc -= dmg;
    if(__net_mdc <= 0) {
        release_victim("The magic net shreds and falls away!\n");
        return;
    }
    if(environment(this_object()))
        tell_room(environment(this_object()),
            "The magic net strains under the assault.\n");
}

int capture(object victim, object caster) {
    object room;
    string vshown;
    string cshown;

    if(!victim || !living(victim)) return 0;
    if(victim->query_property("magic_net_obj")) return 0;
    __victim = victim;
    __caster = caster;
    __victim->set_property("entangled", 1);
    __victim->set_property("cannot_move", 1);
    __victim->set_property("magic_net_obj", this_object());
    room = environment(this_object());
    if(!room) room = environment(victim);
    if(caster && room) {
        cshown = (string)caster->query_display_name(victim);
        vshown = (string)victim->query_display_name(caster);
        tell_object(caster,
            "Glowing strands bind " + vshown + " in a magic net!\n");
        tell_object(victim,
            cshown + " snares you in a glowing magic net! You cannot flee,\n"
            "but you can still fight back.\n");
        caster->tell_room_living(room, caster, victim,
            " snares " + vshown + " in a magic net!\n");
    }
    call_out("timed_release", NET_DURATION);
    return 1;
}

void damage_net_from_combat(object victim, int dmg) {
    if(!victim || victim != __victim || dmg < 1) return;
    take_net_damage(dmg);
}
