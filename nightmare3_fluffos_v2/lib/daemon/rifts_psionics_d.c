/* /daemon/rifts_psionics_d.c
   Palladium Rifts psionic definitions and effect dispatch. */

#include <daemons.h>

private string id_name(object who, object viewer) {
    string n;

    if(!who) return "someone";
    if(function_exists("query_display_name", who)) {
        n = (string)who->query_display_name(viewer);
        if(n && sizeof(n)) return n;
    }
    return (string)who->query_cap_name();
}

private void tell_psi_room(object sender, object target, string msg) {
    object env;
    object *inv;
    int i;
    string att_disp;
    string def_disp;
    string out;

    if(!sender || !msg) return;
    env = environment(sender);
    if(!env) return;
    inv = all_inventory(env);
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || !living(inv[i])) continue;
        if(inv[i] == sender) continue;
        if(target && inv[i] == target) continue;
        att_disp = id_name(sender, inv[i]);
        out = replace_string(msg, "$A", att_disp);
        if(target) {
            def_disp = id_name(target, inv[i]);
            out = replace_string(out, "$D", def_disp);
        }
        tell_object(inv[i], out);
    }
}

/* Psionic data - returns mapping with: isp_cost, duration(s), range, effect, desc. */

string normalize_psionic_name(string name) {
    string n;

    if(!name) return "";
    n = lower_case(name);
    switch(n) {
    case "mindblock":
    case "mind-block":
        return "mind block";
    case "psi sword":
    case "psisword":
        return "psi-sword";
    case "bio regen":
        return "bio-regeneration";
    case "bio regeneration":
        return "bio regeneration psi";
    case "remoteview":
    case "remote view":
    case "remote-viewing":
        return "remote viewing";
    default:
        return n;
    }
}

string query_psionic_alias_hint(string name) {
    if(!name) return "";
    switch(lower_case(name)) {
    case "mind block":          return "mindblock";
    case "psi-sword":           return "psi sword";
    case "bio-regeneration":    return "bio regen";
    case "bio regeneration psi": return "bio regeneration";
    case "remote viewing":         return "remoteview";
    default:                    return "";
    }
}

mapping query_psionic(string name) {
    name = normalize_psionic_name(name);
    switch(name) {
    case "telepathy":
        return ([ "isp_cost":4, "duration":0, "range":"room",
                  "effect":"read_thoughts",
                  "desc":"Read surface thoughts of a target.  Save vs psionics negates." ]);
    case "mind block":
        return ([ "isp_cost":4, "duration":600, "range":"self",
                  "effect":"block",
                  "desc":"Blocks all psionic attacks for 10 minutes." ]);
    case "telekinesis":
        return ([ "isp_cost":2, "duration":0, "range":"room",
                  "effect":"tk",
                  "desc":"Move objects without touching them.  2 ISP per 10 lbs." ]);
    case "empathy":
        return ([ "isp_cost":4, "duration":0, "range":"room",
                  "effect":"empathy",
                  "desc":"Sense the emotional state of a target in the same room." ]);
    case "object read":
        return ([ "isp_cost":6, "duration":0, "range":"touch",
                  "effect":"obj_read",
                  "desc":"Sense the history and last user of an object." ]);
    case "sense evil":
        return ([ "isp_cost":2, "duration":0, "range":"room",
                  "effect":"psi_sense_evil",
                  "desc":"Detect evil presences in the room. No save." ]);
    case "sixth sense":
        return ([ "isp_cost":2, "duration":0, "range":"self",
                  "effect":"sixth_sense",
                  "desc":"Next attack against you: +6 initiative, +2 parry, +3 dodge. Auto-triggers once." ]);
    case "mind bolt":
        return ([ "isp_cost":6, "duration":0, "range":"single",
                  "effect":"mind_bolt",
                  "desc":"3d6 MDC psionic energy bolt. (Use 'psi mind bolt heavy' for 6d6 at 12 ISP.)" ]);
    case "mind bolt heavy":
        return ([ "isp_cost":12, "duration":0, "range":"single",
                  "effect":"mind_bolt_heavy",
                  "desc":"6d6 MDC psionic energy bolt." ]);
    case "psi-sword":
        return ([ "isp_cost":10, "duration":120, "range":"self",
                  "effect":"psi_sword",
                  "desc":"Creates a blade of psionic energy (3d6 MDC) wielded for 2 minutes." ]);
    case "bio-regeneration":
        return ([ "isp_cost":10, "duration":0, "range":"self",
                  "effect":"bio_regen",
                  "desc":"Heal 2d6 MDC or 4d6 SDC/HP instantly." ]);

    /* Sensitive */
    case "detect psionics":
        return ([ "isp_cost":6, "duration":120, "range":"room",
                  "effect":"detect_psionics",
                  "desc":"Detect psionic activity within the area for 2 minutes." ]);

    /* Physical (Pyrokinesis) */
    case "pyrokinesis":
        return ([ "isp_cost":6, "duration":0, "range":"touch",
                  "effect":"pyro_small",
                  "desc":"Create small flame or deal 1d6 SDC on touch." ]);
    case "pyrokinesis bolt":
        return ([ "isp_cost":12, "duration":0, "range":"single",
                  "effect":"pyro_bolt",
                  "desc":"Fire bolt: 3d6 MDC, 60ft range." ]);
    case "pyrokinesis blast":
        return ([ "isp_cost":20, "duration":0, "range":"area",
                  "effect":"pyro_blast",
                  "desc":"Fire explosion: 6d6 MDC, 10ft radius, 40ft range." ]);

    /* Healing */
    case "healing touch":
        return ([ "isp_cost":6, "duration":0, "range":"touch",
                  "effect":"healing_touch",
                  "desc":"Heal 1d8 SDC or 2 HP by laying hands on target." ]);
    case "psychic diagnosis":
        return ([ "isp_cost":4, "duration":0, "range":"touch",
                  "effect":"psychic_diag",
                  "desc":"Know all injuries and ailments affecting a target." ]);
    case "increased healing":
        return ([ "isp_cost":0, "duration":0, "range":"self",
                  "effect":"incr_healing",
                  "desc":"Passive: heals at double normal rate. Always active." ]);
    case "deaden pain":
        return ([ "isp_cost":4, "duration":1800, "range":"touch",
                  "effect":"deaden_pain",
                  "desc":"Target feels no pain for 30 minutes. Ignore pain penalties." ]);
    case "psychic surgery":
        return ([ "isp_cost":14, "duration":0, "range":"touch",
                  "effect":"psych_surgery",
                  "desc":"Repair internal injuries. Restore 2d6 HP. Cure one illness." ]);
    case "induce sleep":
        return ([ "isp_cost":4, "duration":0, "range":"single",
                  "effect":"induce_sleep",
                  "desc":"Target falls into natural sleep. Save vs psionics 15." ]);
    case "bio regeneration psi":
        return ([ "isp_cost":6, "duration":0, "range":"self",
                  "effect":"bio_regen_psi",
                  "desc":"Regenerate 2d6 SDC or 1d6 MDC instantly by concentration." ]);
    case "nightvision":
        return ([ "isp_cost":2, "duration":600, "range":"self",
                  "effect":"nightvision",
                  "desc":"See clearly in total darkness for 10 minutes." ]);
    case "see the invisible":
        return ([ "isp_cost":4, "duration":300, "range":"self",
                  "effect":"see_invisible",
                  "desc":"Perceive invisible beings for 5 minutes." ]);
    case "bio-manipulation":
        return ([ "isp_cost":8, "duration":0, "range":"single",
                  "effect":"bio_manipulation",
                  "desc":"Inflict 2d6 SDC damage to a living target. Save vs psionics." ]);
    case "clairvoyance":
        return ([ "isp_cost":4, "duration":0, "range":"remote",
                  "effect":"clairvoyance",
                  "desc":"Brief vision of a named place or person." ]);
    case "precognition":
        return ([ "isp_cost":6, "duration":60, "range":"self",
                  "effect":"precognition",
                  "desc":"Sixth sense warns of danger for 1 minute." ]);
    case "psi-shield":
        return ([ "isp_cost":10, "duration":300, "range":"self",
                  "effect":"psi_shield",
                  "desc":"MDC 30 psionic force field for 5 minutes." ]);
    case "telekinetic punch":
        return ([ "isp_cost":6, "duration":0, "range":"single",
                  "effect":"tk_punch",
                  "desc":"Telekinetic strike for 2d6 MDC." ]);
    case "telemechanics":
        return ([ "isp_cost":8, "duration":0, "range":"touch",
                  "effect":"telemechanics",
                  "desc":"Mentally operate a machine or vehicle interface." ]);
    case "remote viewing":
        return ([ "isp_cost":6, "duration":0, "range":"remote",
                  "effect":"remote_viewing",
                  "desc":"Psychically observe a distant place or person." ]);
    case "accelerate healing":
        return ([ "isp_cost":8, "duration":0, "range":"touch",
                  "effect":"accelerate_healing",
                  "desc":"Restore 3d6 HP or SDC to a living target." ]);
    case "group mind block":
        return ([ "isp_cost":12, "duration":300, "range":"room",
                  "effect":"group_mind_block",
                  "desc":"Shield all allies in the room from psionic attack for 5 minutes." ]);
    case "psychic purification":
        return ([ "isp_cost":10, "duration":0, "range":"touch",
                  "effect":"psychic_purification",
                  "desc":"Remove poison, disease, or possession from a target." ]);
    case "ectoplasm":
        return ([ "isp_cost":6, "duration":120, "range":"room",
                  "effect":"ectoplasm",
                  "desc":"Manifest ectoplasmic barrier (30 MDC) in the room for 2 minutes." ]);
    case "electrokinesis":
        return ([ "isp_cost":8, "duration":0, "range":"single",
                  "effect":"electrokinesis",
                  "desc":"Electrical bolt for 3d6 MDC." ]);
    case "hypnotic suggestion":
        return ([ "isp_cost":6, "duration":0, "range":"single",
                  "effect":"hypnotic_suggestion",
                  "desc":"Suggest a simple action. Save vs psionics." ]);
    case "sense time":
        return ([ "isp_cost":2, "duration":0, "range":"self",
                  "effect":"sense_time",
                  "desc":"Know the precise local time and date." ]);
    case "empathic transmission":
        return ([ "isp_cost":4, "duration":0, "range":"room",
                  "effect":"empathic_transmission",
                  "desc":"Broadcast a strong emotion to everyone in the room." ]);
    case "bio-feedback":
        return ([ "isp_cost":6, "duration":300, "range":"self",
                  "effect":"bio_feedback",
                  "desc":"+2 to all saves for 5 minutes." ]);
    case "presence sense":
        return ([ "isp_cost":4, "duration":0, "range":"room",
                  "effect":"presence_sense",
                  "desc":"Sense all living minds in the current room." ]);
    case "psychic locator":
        return ([ "isp_cost":8, "duration":0, "range":"special",
                  "effect":"psychic_locator",
                  "desc":"Find direction to a known online player." ]);
    case "mental stun":
        return ([ "isp_cost":10, "duration":30, "range":"single",
                  "effect":"mental_stun",
                  "desc":"Briefly stun target mind. Save vs psionics." ]);
    case "sense magic":
        return ([ "isp_cost":4, "duration":120, "range":"self",
                  "effect":"sense_magic_psi",
                  "desc":"Detect magical auras in the room for 2 minutes." ]);
    case "total recall":
        return ([ "isp_cost":6, "duration":0, "range":"self",
                  "effect":"total_recall",
                  "desc":"Restore 1d6 ISP by deep mental focus." ]);
    case "expand senses":
        return ([ "isp_cost":5, "duration":300, "range":"self",
                  "effect":"expand_senses",
                  "desc":"Heightened awareness for 5 minutes." ]);
    case "summon inner strength":
        return ([ "isp_cost":8, "duration":0, "range":"self",
                  "effect":"inner_strength",
                  "desc":"Restore 2d6 HP or SDC instantly." ]);
    case "telekinetic force field":
        return ([ "isp_cost":10, "duration":120, "range":"self",
                  "effect":"tk_force_field",
                  "desc":"Psionic barrier absorbs damage for 2 minutes." ]);
    case "mind wipe":
        return ([ "isp_cost":12, "duration":60, "range":"single",
                  "effect":"mind_wipe",
                  "desc":"Briefly blank target memory. Save vs psionics." ]);
    case "pyrokinetic aura":
        return ([ "isp_cost":8, "duration":120, "range":"self",
                  "effect":"pyro_aura",
                  "desc":"Flames wreath you, burning melee attackers." ]);

    default:
        return 0;
    }
}

object find_psi_remote_target(object caster, string str) {
    object *online;
    int i;
    string name;
    string cap;

    if(!caster || !str || !sizeof(str)) return 0;
    str = lower_case(str);
    online = users();
    for(i = 0; i < sizeof(online); i++) {
        if(!online[i] || online[i] == caster) continue;
        if(!userp(online[i])) continue;
        name = lower_case((string)online[i]->query_name());
        cap = lower_case((string)online[i]->query_cap_name());
        if(str != name && str != cap &&
           strsrch(name, str) == -1 && strsrch(cap, str) == -1)
            continue;
        if(!creatorp(caster) &&
           !(int)caster->knows_player((string)online[i]->query_name()))
            continue;
        return online[i];
    }
    return 0;
}

/* Save vs psionics */

private int save_psi_check(object target) {
    int me;
    int bonus;
    int roll;

    me = (int)target->query_stats("ME");
    if(me >= 26)      bonus = 3;
    else if(me >= 21) bonus = 2;
    else if(me >= 17) bonus = 1;
    else              bonus = 0;
    roll = random(20) + 1 + bonus;
    return (roll >= 15);
}

/* Effect handlers */

private void fx_telepathy(object target) {
    string race;
    string occ;
    string thought;

    if(!target) {
        write("Read whose thoughts?  Syntax: psi telepathy at <target>\n");
        return;
    }
    if(!living(target)) {
        write("You can only read the thoughts of living beings.\n");
        return;
    }
    if(save_psi_check(target)) {
        tell_object(target, "You feel a psionic probe and resist it.\n");
        write(id_name(target, this_player()) +
              " resists your telepathic probe!\n");
        return;
    }
    race = lower_case((string)target->query_race());
    occ  = lower_case((string)target->getenv("rifts_occ"));
    if(!occ || !sizeof(occ)) occ = "none";

    /* Surface thought generated from race/occ */
    if(strsrch(race, "dragon") != -1)
        thought = "ancient power and contempt for lesser beings";
    else if(strsrch(race, "vampire") != -1)
        thought = "hunger and predatory patience";
    else if(occ == "headhunter" || occ == "bounty hunter")
        thought = "tactical assessment of threats and escape routes";
    else if(occ == "cs grunt")
        thought = "loyalty to the Coalition and suspicion of non-humans";
    else if(occ == "ley line walker")
        thought = "the flow of ley line energy and arcane potential";
    else
        thought = "everyday concerns and a flicker of surprise";

    write("You brush the surface thoughts of " +
          id_name(target, this_player()) +
          ":\n  \"" + capitalize(thought) + ".\"\n");
    tell_object(target,
        "Something brushes the edge of your thoughts...\n");
}

void apply_telepathy_send(object target, string message) {
    object sender;
    string sender_name;
    string target_name;

    if(!target || !living(target)) return;
    if(!message || !sizeof(message)) return;
    sender = this_player();
    if(!sender) return;
    sender_name = (string)sender->query_display_name(target);
    target_name = (string)target->query_display_name(sender);
    tell_object(target,
        sender_name + " speaks in your mind: " + message + "\n");
    target->set_property("reply", (string)sender->query_name());
    write("You send a telepathic message to " + target_name +
          ": " + message + "\n");
}

private void fx_block(object target) {
    if((int)this_player()->query_property("mind_blocked")) {
        write("Your mind is already shielded.\n");
        return;
    }
    this_player()->set_property("mind_blocked", 1);
    write("You erect a psychic barrier around your mind.  "
          "Psionic attacks will be blocked for 10 minutes.\n");
    call_out("end_block", 600, this_player());
}

void end_block(object who) {
    if(!objectp(who)) return;
    who->set_property("mind_blocked", 0);
    tell_object(who, "Your psionic mind block fades.\n");
}

private void fx_empathy(object target) {
    string emotion;

    if(!target) {
        write("Sense whose emotions?  Syntax: psi empathy at <target>\n");
        return;
    }
    if(!living(target)) {
        write("You can only sense emotions of living beings.\n");
        return;
    }
    if(save_psi_check(target)) {
        write(id_name(target, this_player()) +
              " feels something but shakes it off.\n");
        return;
    }
    if((int)target->query_alignment() > 400)
        emotion = "calm resolve and genuine goodwill";
    else if((int)target->query_alignment() > 0)
        emotion = "cautious optimism and lingering doubt";
    else if((int)target->query_alignment() > -300)
        emotion = "restless agitation and self-interest";
    else if((int)target->query_alignment() > -700)
        emotion = "cold calculation and opportunistic intent";
    else
        emotion = "hostility and contempt bordering on rage";

    write("You sense the emotional state of " +
          id_name(target, this_player()) +
          ":\n  " + capitalize(emotion) + ".\n");
    tell_object(target,
        "A warm prickling sensation crosses your mind briefly.\n");
}

private void fx_obj_read(object target) {
    string obj_name;
    string history;

    if(!target) {
        write("Read which object?  Syntax: psi object read at <object>\n");
        return;
    }
    if(living(target)) {
        write("Object read works on items, not living beings.\n");
        return;
    }
    obj_name = (string)target->query_name();
    if(!obj_name) obj_name = "this object";

    if((int)target->query_property("rifts_weapon") ||
       (int)target->query_property("rifts_mdcweapon"))
        history = "conflict and blood, hands that wielded it with intent to kill";
    else if((int)target->query_property("rifts_armor"))
        history = "fear and survival, a body that wore it under fire";
    else if((int)target->query_property("quest_item"))
        history = "purpose and urgency, someone who needed it desperately";
    else
        history = "ordinary use and the residue of daily life";

    write("You press your hands against " + obj_name +
          " and read its history:\n  " + capitalize(history) + ".\n");
}

private void fx_tk(object target) {
    string target_name;
    object sender;
    object env;
    object *inv;
    int i;

    if(!target) {
        write("Move what?  Syntax: psi telekinesis at <object>\n");
        return;
    }
    target_name = (string)target->query_name();
    if(!target_name) target_name = "object";
    sender = this_player();

    if(living(target)) {
        write("You press " + id_name(target, sender) +
              " with telekinetic force!\n");
        tell_object(target,
            id_name(sender, target) +
            " pushes you with invisible telekinetic force!\n");
        env = environment(sender);
        if(env) {
            inv = all_inventory(env);
            for(i = 0; i < sizeof(inv); i++) {
                if(!inv[i] || !living(inv[i])) continue;
                if(inv[i] == sender || inv[i] == target) continue;
                tell_object(inv[i], id_name(sender, inv[i]) +
                    " pushes " + id_name(target, inv[i]) +
                    " telekinetically!\n");
            }
        }
    } else {
        write("You lift " + target_name + " with your mind and set it nearby.\n");
        if(sender)
            sender->tell_room_living(environment(sender), sender, 0,
                " levitates " + target_name + " telekinetically.");
    }
}

/* New psionic effect handlers */

private void fx_psi_sense_evil(object target) {
    object *contents;
    int i;
    int found;
    string names;

    contents = all_inventory(environment(this_player()));
    found = 0;
    names = "";
    for(i = 0; i < sizeof(contents); i++) {
        if(contents[i] == this_player()) continue;
        if(!living(contents[i])) continue;
        if((int)contents[i]->query_alignment() < -200) {
            found++;
            names += (string)contents[i]->query_short() + " ";
        }
    }
    if(found > 0)
        write("You sense darkness in this room: " + names + "\n");
    else
        write("You sense nothing unusual in the immediate area.\n");
}

private void fx_sixth_sense(object target) {
    if((int)this_player()->query_property("sixth_sense_active")) {
        write("Your sixth sense is already primed.\n");
        return;
    }
    this_player()->set_property("sixth_sense_active", 1);
    write("Your sixth sense sharpens. You will react to the next attack.\n");
}

private void fx_mind_bolt(object target) {
    int damage;
    int i;
    int cur_mdc;
    int cur_hp;

    if(!target) {
        write("Use against whom?  Syntax: psi mind bolt at <target>\n");
        return;
    }
    damage = 0;
    for(i = 0; i < 3; i++) damage += random(6) + 1;
    write("You hurl a bolt of psionic energy! " + damage + " MDC!\n");
    tell_object(target,
        id_name(this_player(), target) +
        " blasts you with a psionic energy bolt!\n");
    tell_psi_room(this_player(), target,
        "$A hurls a mind bolt at $D!");
    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        cur_mdc = (int)target->query_stats("MDC");
        cur_mdc -= damage;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= -20) catch(target->die());
    } else {
        cur_hp = (int)target->query_hp() - damage;
        if(cur_hp < 0) { target->set_hp(0); catch(target->die()); }
        else target->set_hp(cur_hp);
    }
}

private void fx_mind_bolt_heavy(object target) {
    int damage;
    int i;
    int cur_mdc;
    int cur_hp;

    if(!target) {
        write("Use against whom?  Syntax: psi mind bolt heavy at <target>\n");
        return;
    }
    damage = 0;
    for(i = 0; i < 6; i++) damage += random(6) + 1;
    write("You unleash a devastating psionic blast! " + damage + " MDC!\n");
    tell_object(target,
        id_name(this_player(), target) +
        " unleashes a massive psionic blast at you!\n");
    tell_psi_room(this_player(), target,
        "$A devastates $D with a heavy mind bolt!");
    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        cur_mdc = (int)target->query_stats("MDC");
        cur_mdc -= damage;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= -20) catch(target->die());
    } else {
        cur_hp = (int)target->query_hp() - damage;
        if(cur_hp < 0) { target->set_hp(0); catch(target->die()); }
        else target->set_hp(cur_hp);
    }
}

private void fx_psi_sword(object target) {
    object sword;
    if((int)this_player()->query_property("has_psi_sword")) {
        write("Your psi-sword is already manifested.\n");
        return;
    }
    catch(sword = clone_object("/obj/psi_sword"));
    if(!sword) {
        /* Fallback: grant melee damage bonus if psi_sword.c doesn't exist yet */
        this_player()->set_property("has_psi_sword", 1);
        this_player()->add_skill_bonus("melee", 3);
        write("A blade of psionic energy materializes in your hand! (3d6 MDC)\n");
        this_player()->tell_room_living(environment(this_player()),
            this_player(), 0,
            " manifests a shimmering psionic blade!\n");
        call_out("end_psi_sword_fallback", 120, this_player());
        return;
    }
    sword->move(this_player());
    this_player()->set_property("has_psi_sword", 1);
    this_player()->force_me("wield psi-sword in right hand");
    write("A blade of psionic energy materializes in your hand! (3d6 MDC)\n");
    this_player()->tell_room_living(environment(this_player()),
        this_player(), 0,
        " manifests a shimmering psionic blade!\n");
    call_out("end_psi_sword", 120, this_player(), sword);
}

void end_psi_sword(object who, object sword) {
    if(!objectp(who)) return;
    who->set_property("has_psi_sword", 0);
    if(objectp(sword)) sword->destruct();
    tell_object(who, "Your psi-sword dissipates.\n");
}

void end_psi_sword_fallback(object who) {
    if(!objectp(who)) return;
    who->set_property("has_psi_sword", 0);
    who->add_skill_bonus("melee", -3);
    tell_object(who, "Your psi-sword dissipates.\n");
}

private void fx_bio_regen(object target) {
    int heal_mdc;
    int heal_sdc;
    int i;
    int cur_mdc;
    int max_mdc;
    int cur_sdc;
    int max_sdc;
    int cur_hp;
    int max_hp;
    int new_val;

    heal_mdc = 0;
    heal_sdc = 0;
    for(i = 0; i < 2; i++) heal_mdc += random(6) + 1;
    for(i = 0; i < 4; i++) heal_sdc += random(6) + 1;

    cur_mdc = (int)this_player()->query_stats("MDC");
    max_mdc = (int)this_player()->query_stats("max_MDC");
    if(max_mdc > 0 && cur_mdc < max_mdc) {
        new_val = cur_mdc + heal_mdc;
        if(new_val > max_mdc) new_val = max_mdc;
        this_player()->set_stats("MDC", new_val);
        write("Your body regenerates psionically. +" + heal_mdc + " MDC.\n");
        return;
    }
    cur_sdc = (int)this_player()->query_stats("SDC");
    max_sdc = (int)this_player()->query_stats("max_SDC");
    if(max_sdc > 0 && cur_sdc < max_sdc) {
        new_val = cur_sdc + heal_sdc;
        if(new_val > max_sdc) new_val = max_sdc;
        this_player()->set_stats("SDC", new_val);
        write("Your body regenerates psionically. +" + heal_sdc + " SDC.\n");
        return;
    }
    cur_hp = (int)this_player()->query_stats("rifts_hp");
    max_hp = (int)this_player()->query_stats("max_rifts_hp");
    if(max_hp > 0 && cur_hp < max_hp) {
        new_val = cur_hp + heal_sdc;
        if(new_val > max_hp) new_val = max_hp;
        this_player()->set_stats("rifts_hp", new_val);
        write("Your body regenerates psionically. +" + heal_sdc + " HP.\n");
        return;
    }
    write("You are already at full health.\n");
}

/* New effect handlers */

private void fx_detect_psionics(object target) {
    if((int)this_player()->query_property("detect_psionics_active")) {
        write("Your psionic detection sense is already active.\n");
        return;
    }
    this_player()->set_property("detect_psionics_active", 1);
    write("Your senses sharpen. You will detect psionic activity nearby for 2 minutes.\n");
    this_player()->tell_room_living(environment(this_player()),
        this_player(), 0,
        "'s eyes glow faintly as psionic senses extend.\n");
    call_out("end_detect_psionics", 120, this_player());
}

void end_detect_psionics(object who) {
    if(!objectp(who)) return;
    who->set_property("detect_psionics_active", 0);
    tell_object(who, "Your psionic detection sense fades.\n");
}

private void fx_pyro_small(object target) {
    if(!target) {
        write("A small flame flickers into existence in your palm.\n");
        this_player()->tell_room_living(environment(this_player()),
            this_player(), 0,
            " summons a small flame in their hand.\n");
        return;
    }
    write("You press a focused flame against " +
          id_name(target, this_player()) +
          "! 1d" + (string)(random(6)+1) + " SDC!\n");
    tell_object(target, id_name(this_player(), target) +
        " sears you with pyrokinetic flame!\n");
}

private void fx_pyro_bolt(object target) {
    int damage;
    int i;
    int cur_mdc;
    int cur_hp;

    if(!target) {
        write("Use against whom?  Syntax: psi pyrokinesis bolt at <target>\n");
        return;
    }
    damage = 0;
    for(i = 0; i < 3; i++) damage += random(6) + 1;
    write("You hurl a bolt of psychic fire! " + damage + " MDC!\n");
    tell_object(target, id_name(this_player(), target) +
        " blasts you with a bolt of pyrokinetic fire!\n");
    tell_psi_room(this_player(), target,
        "$A hurls a fire bolt at $D!");
    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        cur_mdc = (int)target->query_stats("MDC") - damage;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= -20) catch(target->die());
    } else {
        cur_hp = (int)target->query_hp() - damage;
        if(cur_hp < 0) { target->set_hp(0); catch(target->die()); }
        else target->set_hp(cur_hp);
    }
}

private void fx_pyro_blast(object target) {
    int damage;
    int i;
    int cur_mdc;
    int cur_hp;

    if(!target) {
        write("Use against whom?  Syntax: psi pyrokinesis blast at <target>\n");
        return;
    }
    damage = 0;
    for(i = 0; i < 6; i++) damage += random(6) + 1;
    write("A roiling explosion of psychic fire erupts! " + damage + " MDC!\n");
    tell_object(target, id_name(this_player(), target) +
        " engulfs you in a pyrokinetic blast!\n");
    tell_psi_room(this_player(), target,
        "$A detonates a fire blast near $D!");
    if((int)RIFTS_D->is_rifts_race((string)target->query_race())) {
        cur_mdc = (int)target->query_stats("MDC") - damage;
        target->set_stats("MDC", cur_mdc);
        if(cur_mdc <= -20) catch(target->die());
    } else {
        cur_hp = (int)target->query_hp() - damage;
        if(cur_hp < 0) { target->set_hp(0); catch(target->die()); }
        else target->set_hp(cur_hp);
    }
}

private void fx_healing_touch(object target) {
    int heal_sdc;
    int heal_hp;
    int cur_sdc;
    int max_sdc;
    int cur_hp;
    int max_hp;
    int new_val;

    if(!target) target = this_player();
    if(!living(target)) {
        write("You can only heal living beings.\n");
        return;
    }
    heal_sdc = random(8) + 1;
    heal_hp  = 2;
    cur_sdc = (int)target->query_stats("SDC");
    max_sdc = (int)target->query_stats("max_SDC");
    if(max_sdc > 0 && cur_sdc < max_sdc) {
        new_val = cur_sdc + heal_sdc;
        if(new_val > max_sdc) new_val = max_sdc;
        target->set_stats("SDC", new_val);
        write("You lay your hands on " + id_name(target, this_player()) +
              " and channel healing energy. +" + heal_sdc + " SDC.\n");
        tell_object(target, id_name(this_player(), target) +
              " heals you with a gentle touch. +" + heal_sdc + " SDC.\n");
        return;
    }
    cur_hp  = (int)target->query_stats("rifts_hp");
    max_hp  = (int)target->query_stats("max_rifts_hp");
    if(max_hp > 0 && cur_hp < max_hp) {
        new_val = cur_hp + heal_hp;
        if(new_val > max_hp) new_val = max_hp;
        target->set_stats("rifts_hp", new_val);
        write("You channel healing energy into " +
              id_name(target, this_player()) +
              ". +" + heal_hp + " HP.\n");
        tell_object(target, id_name(this_player(), target) +
              " heals you. +" + heal_hp + " HP.\n");
        return;
    }
    write(id_name(target, this_player()) + " is already at full health.\n");
}

private void fx_psychic_diag(object target) {
    int sdc;
    int max_sdc;
    int mdc;
    int max_mdc;
    int hp;
    int max_hp;

    if(!target) {
        write("Diagnose whom?  Syntax: psi psychic diagnosis at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Psychic diagnosis only works on living beings.\n");
        return;
    }
    sdc     = (int)target->query_stats("SDC");
    max_sdc = (int)target->query_stats("max_SDC");
    mdc     = (int)target->query_stats("MDC");
    max_mdc = (int)target->query_stats("max_MDC");
    hp      = (int)target->query_stats("rifts_hp");
    max_hp  = (int)target->query_stats("max_rifts_hp");
    write("Your psychic senses reveal the condition of " +
          id_name(target, this_player()) + ":\n");
    if(max_mdc > 0)
        write("  MDC: " + mdc + " / " + max_mdc + "\n");
    if(max_sdc > 0)
        write("  SDC: " + sdc + " / " + max_sdc + "\n");
    if(max_hp > 0)
        write("  HP:  " + hp  + " / " + max_hp  + "\n");
    tell_object(target, id_name(this_player(), target) +
        " briefly scans your injuries with a touch.\n");
}

private void fx_incr_healing(object target) {
    if((int)this_player()->query_property("increased_healing")) {
        write("Your accelerated healing is already active.\n");
        return;
    }
    this_player()->set_property("increased_healing", 1);
    write("Your body's natural healing accelerates. You recover at double speed.\n");
}

private void fx_deaden_pain(object target) {
    if(!target) target = this_player();
    if(!living(target)) {
        write("Deaden pain only works on living beings.\n");
        return;
    }
    if((int)target->query_property("pain_deadened")) {
        write(id_name(target, this_player()) + " already feels no pain.\n");
        return;
    }
    target->set_property("pain_deadened", 1);
    write("You project a pain-blocking field into " +
          id_name(target, this_player()) +
          ". They will feel no pain for 30 minutes.\n");
    if(target != this_player())
        tell_object(target, id_name(this_player(), target) +
            " deadens your pain. Pain penalties ignored for 30 minutes.\n");
    call_out("end_deaden_pain", 1800, target);
}

void end_deaden_pain(object who) {
    if(!objectp(who)) return;
    who->set_property("pain_deadened", 0);
    tell_object(who, "The psionic pain-block fades. Sensation returns.\n");
}

private void fx_psych_surgery(object target) {
    int heal_hp;
    int i;
    int cur_hp;
    int max_hp;
    int new_val;

    if(!target) {
        write("Operate on whom?  Syntax: psi psychic surgery at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Psychic surgery only works on living beings.\n");
        return;
    }
    heal_hp = 0;
    for(i = 0; i < 2; i++) heal_hp += random(6) + 1;
    write("You perform psychic surgery on " + id_name(target, this_player()) +
          ". One hour of concentration. +" + heal_hp + " HP.\n");
    tell_object(target, id_name(this_player(), target) +
        " channels deep healing through psychic surgery. +" + heal_hp + " HP.\n");
    cur_hp = (int)target->query_stats("rifts_hp");
    max_hp = (int)target->query_stats("max_rifts_hp");
    if(max_hp > 0) {
        new_val = cur_hp + heal_hp;
        if(new_val > max_hp) new_val = max_hp;
        target->set_stats("rifts_hp", new_val);
    }
}

private void fx_induce_sleep(object target) {
    if(!target) {
        write("Induce sleep on whom?  Syntax: psi induce sleep at <target>\n");
        return;
    }
    if(!living(target)) {
        write("Induce sleep only works on living beings.\n");
        return;
    }
    if(save_psi_check(target)) {
        tell_object(target, "You feel a wave of drowsiness and shake it off.\n");
        write(id_name(target, this_player()) +
              " resists the sleep induction!\n");
        return;
    }
    target->set_property("psi_sleep", 1);
    write(id_name(target, this_player()) + " crumples into a deep sleep.\n");
    tell_object(target, "An irresistible drowsiness overwhelms you...\n");
    target->tell_room_living(environment(target), target, target,
        " collapses into psychically-induced sleep.\n");
    call_out("end_psi_sleep", (random(6)+1) * 3600, target);
}

void end_psi_sleep(object who) {
    if(!objectp(who)) return;
    who->set_property("psi_sleep", 0);
    tell_object(who, "You stir from a psionic sleep, feeling strangely rested.\n");
}

private void fx_bio_regen_psi(object target) {
    int heal_mdc;
    int heal_sdc;
    int i;
    int cur_mdc;
    int max_mdc;
    int cur_sdc;
    int max_sdc;
    int new_val;

    heal_mdc = 0;
    heal_sdc = 0;
    for(i = 0; i < 1; i++) heal_mdc += random(6) + 1;
    for(i = 0; i < 2; i++) heal_sdc += random(6) + 1;

    cur_mdc = (int)this_player()->query_stats("MDC");
    max_mdc = (int)this_player()->query_stats("max_MDC");
    if(max_mdc > 0 && cur_mdc < max_mdc) {
        new_val = cur_mdc + heal_mdc;
        if(new_val > max_mdc) new_val = max_mdc;
        this_player()->set_stats("MDC", new_val);
        write("You concentrate and regenerate tissue. +" + heal_mdc + " MDC.\n");
        return;
    }
    cur_sdc = (int)this_player()->query_stats("SDC");
    max_sdc = (int)this_player()->query_stats("max_SDC");
    if(max_sdc > 0 && cur_sdc < max_sdc) {
        new_val = cur_sdc + heal_sdc;
        if(new_val > max_sdc) new_val = max_sdc;
        this_player()->set_stats("SDC", new_val);
        write("You concentrate and regenerate tissue. +" + heal_sdc + " SDC.\n");
        return;
    }
    write("You are already at full health.\n");
}

private void fx_nightvision(object target) {
    this_player()->set_property("nightvision_active", 1);
    write("Your vision adapts to darkness.\n");
    call_out("end_nightvision", 600, this_player());
}

void end_nightvision(object who) {
    if(!objectp(who)) return;
    who->remove_property("nightvision_active");
    tell_object(who, "Your nightvision fades.\n");
}

private void fx_see_invisible(object target) {
    this_player()->set_property("see_invisible_active", 1);
    write("Hidden shapes become visible to your psi-sight.\n");
    call_out("end_see_invisible", 300, this_player());
}

void end_see_invisible(object who) {
    if(!objectp(who)) return;
    who->remove_property("see_invisible_active");
    tell_object(who, "Your invisible-sight fades.\n");
}

private void fx_bio_manipulation(object target) {
    int dmg;

    if(!target || !living(target)) {
        write("Bio-manipulate whom?  Syntax: psi bio-manipulation at <target>\n");
        return;
    }
    if(save_psi_check(target)) {
        write((string)target->query_cap_name() + " resists bio-manipulation!\n");
        return;
    }
    dmg = random(6) + 1 + random(6) + 1;
    RIFTS_COMBAT_D->apply_rifts_damage(this_player(), target, dmg);
    write("You twist " + (string)target->query_cap_name() +
        "'s biology for " + dmg + " damage.\n");
}

private void fx_clairvoyance(object target) {
    object room;
    string desc;

    if(!target) {
        write("Clairvoyance on what?  Syntax: psi clairvoyance at <name>\n");
        return;
    }
    room = environment(this_player());
    desc = room ? (string)room->query_short() : "unknown";
    write("Your mind reaches outward... you glimpse fragments tied to '" +
        target + "'.\nNearby anchor: " + desc + "\n");
}

private void fx_precognition(object target) {
    this_player()->set_property("precognition_active", 1);
    write("Danger sense prickles at the back of your mind.\n");
    call_out("end_precognition", 60, this_player());
}

void end_precognition(object who) {
    if(!objectp(who)) return;
    who->remove_property("precognition_active");
    tell_object(who, "Your precognition fades.\n");
}

private void fx_psi_shield(object target) {
    if((int)this_player()->query_property("psi_shield_mdc") > 0) {
        write("Your psi-shield is already active.\n");
        return;
    }
    this_player()->set_property("psi_shield_mdc", 30);
    write("A shimmering psionic field wraps around you (30 MDC).\n");
    call_out("end_psi_shield", 300, this_player());
}

void end_psi_shield(object who) {
    if(!objectp(who)) return;
    who->remove_property("psi_shield_mdc");
    tell_object(who, "Your psi-shield collapses.\n");
}

private void fx_tk_punch(object target) {
    int dmg;

    if(!target || !living(target)) {
        write("Strike whom?  Syntax: psi telekinetic punch at <target>\n");
        return;
    }
    dmg = (random(6) + 1 + random(6) + 1) * 5;
    RIFTS_COMBAT_D->apply_rifts_damage(this_player(), target, dmg);
    write("You lash out with telekinetic force for " + dmg + " MDC!\n");
}

private void fx_telemechanics(object target) {
    object env;

    env = environment(this_player());
    if(!env) {
        write("Nothing here to interface with.\n");
        return;
    }
    if((int)env->query_property("vehicle_interior") ||
       (int)env->query_property("vehicle_daemon")) {
        write("You mentally interface with the vehicle systems.\n"
              "Use normal pilot commands from the cockpit.\n");
        return;
    }
    write("You probe nearby machinery but find no complex interface.\n");
}

private void fx_remote_viewing(object target) {
    object room;
    string short_desc;
    string long_desc;

    if(!target) {
        write("View whom?  Syntax: remoteview <name>\n");
        return;
    }
    if(objectp(target) && userp(target)) {
        if(!interactive(target)) {
            write((string)target->query_cap_name() +
                " is link-dead. You cannot fix their location.\n");
            return;
        }
        room = environment(target);
        if(!room) {
            write("You cannot fix a location for " +
                (string)target->query_cap_name() + ".\n");
            return;
        }
        short_desc = (string)room->query_short();
        long_desc = (string)room->query_long();
        if(sizeof(long_desc) > 240)
            long_desc = long_desc[0..239] + "...";
        write("Your consciousness reaches out to " +
            (string)target->query_cap_name() + "...\n");
        write("Psychic snapshot: " + short_desc + "\n");
        if((int)target->query_property("mind_blocked"))
            write("Their mind is shielded. Location only, no detail.\n");
        else if(long_desc && sizeof(long_desc))
            write(long_desc + "\n");
        return;
    }
    if(stringp(target) && sizeof(target)) {
        room = environment(this_player());
        write("Your consciousness drifts toward '" + target + "'...\n");
        if(room)
            write("You receive only static. Named places require a known person as anchor.\n");
        else
            write("Nothing clear.\n");
    }
}

private void fx_accelerate_healing(object target) {
    int heal;
    int cur;

    if(!target || !living(target)) {
        write("Heal whom?  Syntax: psi accelerate healing at <target>\n");
        return;
    }
    heal = random(6) + 1 + random(6) + 1 + random(6) + 1;
    cur = (int)target->query_stats("rifts_hp");
    if(cur > 0) {
        target->set_stats("rifts_hp", cur + heal);
        target->set_hp((int)target->query_hp() + heal);
    } else {
        cur = (int)target->query_stats("SDC");
        target->set_stats("SDC", cur + heal);
    }
    write("You channel accelerated healing into " +
        (string)target->query_cap_name() + " (" + heal + " restored).\n");
    tell_object(target, "Warm psychic energy accelerates your recovery.\n");
}

private void fx_group_mind_block(object target) {
    object *inv;
    object room;
    int i;
    int count;

    room = environment(this_player());
    if(!room) return;
    inv = all_inventory(room);
    count = 0;
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || !living(inv[i])) continue;
        if(inv[i] == this_player() ||
           (int)inv[i]->query_property("party_member")) {
            inv[i]->set_property("mind_blocked", 1);
            count++;
            if(inv[i] != this_player())
                tell_object(inv[i], "A psychic shield wraps your mind.\n");
        }
    }
    this_player()->set_property("mind_blocked", 1);
    write("You erect a group mind block over " + count + " mind(s) for five minutes.\n");
    call_out("end_group_mind_block", 300, room, this_player());
}

void end_group_mind_block(object room, object caster) {
    object *inv;
    int i;

    if(!room || !objectp(room)) return;
    inv = all_inventory(room);
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || !living(inv[i])) continue;
        if(inv[i] == caster ||
           (int)inv[i]->query_property("party_member"))
            inv[i]->set_property("mind_blocked", 0);
    }
    if(objectp(caster))
        tell_object(caster, "The group mind block fades.\n");
}

private void fx_psychic_purification(object target) {
    if(!target || !living(target)) {
        write("Purify whom?  Syntax: psi psychic purification at <target>\n");
        return;
    }
    target->remove_property("poisoned");
    target->remove_property("diseased");
    target->remove_property("possessed");
    write("Psychic light washes through " + (string)target->query_cap_name() +
        ", cleansing afflictions.\n");
    tell_object(target, "A cleansing psychic force purges toxins and corruption.\n");
}

private void fx_ectoplasm(object target) {
    object room;

    room = environment(this_player());
    if(!room) return;
    if((int)room->query_property("ectoplasm_barrier")) {
        write("Ectoplasm already fills this area.\n");
        return;
    }
    room->set_property("ectoplasm_barrier", 1);
    room->set_property("ectoplasm_mdc", 30);
    write("Shimmering ectoplasm wells up, forming a protective barrier (30 MDC).\n");
    say(this_player()->query_cap_name() + " manifests a wall of ectoplasm!");
    call_out("end_ectoplasm_barrier", 120, room);
}

void end_ectoplasm_barrier(object room) {
    if(!objectp(room)) return;
    room->remove_property("ectoplasm_barrier");
    room->remove_property("ectoplasm_mdc");
    tell_room(room, "The ectoplasmic barrier dissolves into mist.\n");
}

private void fx_electrokinesis(object target) {
    int dmg;
    int i;

    if(!target || !living(target)) {
        write("Target whom?  Syntax: psi electrokinesis at <target>\n");
        return;
    }
    dmg = 0;
    for(i = 0; i < 3; i++) dmg += random(6) + 1;
    this_player()->set_property("last_attack_element", "lightning");
    RIFTS_COMBAT_D->apply_rifts_damage(this_player(), target, dmg);
    this_player()->remove_property("last_attack_element");
    write("You hurl a bolt of psychic lightning for " + dmg + " MDC!\n");
}

private void fx_hypnotic_suggestion(object target) {
    if(!target || !living(target)) {
        write("Suggest to whom?  Syntax: psi hypnotic suggestion at <target>\n");
        return;
    }
    if(save_psi_check(target)) {
        write((string)target->query_cap_name() + " resists your suggestion.\n");
        return;
    }
    target->set_property("hypnotized", 1);
    write("You implant a hypnotic suggestion in " +
        (string)target->query_cap_name() + "'s mind.\n");
    tell_object(target, "A compelling thought surfaces in your mind...\n");
    call_out("end_hypnotic_suggestion", 120, target);
}

void end_hypnotic_suggestion(object who) {
    if(!objectp(who)) return;
    who->remove_property("hypnotized");
}

private void fx_sense_time(object target) {
    write("Local time: " + ctime(time()) + "\n");
}

private void fx_empathic_transmission(object target) {
    object env;
    string emotion;

    env = environment(this_player());
    if(!env) return;
    emotion = ({ "fear", "rage", "calm", "joy", "sorrow" })[random(5)];
    write("You broadcast a wave of " + emotion + " to the room.\n");
    tell_room(env, "A sudden wave of " + emotion +
        " washes over everyone present.\n", ({ this_player() }));
}

private void fx_bio_feedback(object target) {
    this_player()->set_property("bio_feedback_active", 1);
    write("You enter a bio-feedback trance. +2 to saves for five minutes.\n");
    call_out("end_bio_feedback", 300, this_player());
}

void end_bio_feedback(object who) {
    if(!objectp(who)) return;
    who->remove_property("bio_feedback_active");
    tell_object(who, "Your bio-feedback trance ends.\n");
}

private void fx_presence_sense(object target) {
    object room;
    object *inv;
    int i;
    int count;

    room = environment(this_player());
    if(!room) return;
    inv = all_inventory(room);
    count = 0;
    write("You extend your psychic senses...\n");
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || !living(inv[i])) continue;
        write("  " + capitalize((string)inv[i]->query_short()) + "\n");
        count++;
    }
    if(!count) write("  No living minds detected.\n");
}

private void fx_psychic_locator(object target) {
    object found;
    object my_room;
    object their_room;
    string dir;

    if(!target || !sizeof((string)target)) {
        write("Locate whom?  Syntax: psi psychic locator at <name>\n");
        return;
    }
    found = find_psi_remote_target(this_player(), (string)target);
    if(!found) {
        write("You cannot fix on that mind.\n");
        return;
    }
    my_room = environment(this_player());
    their_room = environment(found);
    if(!my_room || !their_room) {
        write("Your senses cannot pierce the void.\n");
        return;
    }
    if(my_room == their_room) {
        write((string)found->query_cap_name() + " is here with you.\n");
        return;
    }
    dir = "somewhere distant";
    write("You sense " + (string)found->query_cap_name() +
          " at " + (string)their_room->query_short() + " (" + dir + ").\n");
}

private void fx_mental_stun(object target) {
    if(!target || !living(target)) {
        write("Stun whom?  Syntax: psi mental stun at <target>\n");
        return;
    }
    if(save_psi_check(target)) {
        write((string)target->query_cap_name() + " shrugs off the mental assault.\n");
        tell_object(target, "A psychic blow glances off your mind.\n");
        return;
    }
    target->set_property("confused", 1);
    write("You stun " + (string)target->query_cap_name() + "'s mind!\n");
    tell_object(target, "A crushing psychic force blanks your thoughts!\n");
    call_out("end_mental_stun", 30, target);
}

void end_mental_stun(object who) {
    if(!objectp(who)) return;
    who->remove_property("confused");
    tell_object(who, "Your thoughts clear.\n");
}

private void fx_sense_magic_psi(object target) {
    this_player()->set_property("sense_magic_psi", 1);
    write("Your psychic sight opens to magical auras for two minutes.\n");
    call_out("end_sense_magic_psi", 120, this_player());
}

void end_sense_magic_psi(object who) {
    if(!objectp(who)) return;
    who->remove_property("sense_magic_psi");
    tell_object(who, "Your magical sight fades.\n");
}

private void fx_total_recall(object target) {
    int gain;
    int cur;
    int max_isp;

    gain = random(6) + 1;
    cur = (int)this_player()->query_stats("ISP");
    max_isp = (int)this_player()->query_stats("max_ISP");
    if(cur + gain > max_isp) gain = max_isp - cur;
    if(gain < 1) {
        write("Your mind is already at full focus.\n");
        return;
    }
    this_player()->set_stats("ISP", cur + gain);
    write("Deep recall restores " + gain + " ISP.\n");
}

private void fx_expand_senses(object target) {
    this_player()->set_property("expand_senses", 1);
    write("Your senses expand. You feel hyper-aware for five minutes.\n");
    call_out("end_expand_senses", 300, this_player());
}

void end_expand_senses(object who) {
    if(!objectp(who)) return;
    who->remove_property("expand_senses");
    tell_object(who, "Your expanded senses fade.\n");
}

private void fx_inner_strength(object target) {
    int heal;
    int cur;

    heal = random(6) + 1 + random(6) + 1;
    cur = (int)this_player()->query_stats("rifts_hp");
    if(cur > 0) {
        this_player()->set_stats("rifts_hp", cur + heal);
        this_player()->set_hp((int)this_player()->query_hp() + heal);
    } else {
        cur = (int)this_player()->query_stats("SDC");
        this_player()->set_stats("SDC", cur + heal);
    }
    write("You summon inner strength and recover " + heal + " points.\n");
}

private void fx_tk_force_field(object target) {
    this_player()->set_property("tk_force_field", 1);
    write("A telekinetic barrier shimmers around you.\n");
    call_out("end_tk_force_field", 120, this_player());
}

void end_tk_force_field(object who) {
    if(!objectp(who)) return;
    who->remove_property("tk_force_field");
    tell_object(who, "Your force field collapses.\n");
}

private void fx_mind_wipe(object target) {
    if(!target || !living(target)) {
        write("Wipe whose mind?  Syntax: psi mind wipe at <target>\n");
        return;
    }
    if(save_psi_check(target)) {
        write((string)target->query_cap_name() + " resists the mind wipe.\n");
        return;
    }
    target->set_property("mind_wiped", 1);
    target->set_property("confused", 1);
    write("You scour " + (string)target->query_cap_name() + "'s recent memories!\n");
    tell_object(target, "Your thoughts scatter like ash!\n");
    call_out("end_mind_wipe", 60, target);
}

void end_mind_wipe(object who) {
    if(!objectp(who)) return;
    who->remove_property("mind_wiped");
    who->remove_property("confused");
    tell_object(who, "Your memories slowly return.\n");
}

private void fx_pyro_aura(object target) {
    this_player()->set_property("pyro_aura", 1);
    write("Flames wreath your body for two minutes.\n");
    say(this_player()->query_cap_name() + " is wreathed in psionic fire!");
    call_out("end_pyro_aura", 120, this_player());
}

void end_pyro_aura(object who) {
    if(!objectp(who)) return;
    who->remove_property("pyro_aura");
    tell_object(who, "Your pyrokinetic aura fades.\n");
}

/* Main dispatch - called by _psi.c after ISP deduction. */

void apply_psionic_effect(string psi_name, object target) {
    mapping pdata;
    string effect;

    pdata = query_psionic(lower_case(psi_name));
    if(!pdata) return;
    effect = (string)pdata["effect"];

    switch(effect) {
    case "read_thoughts":   fx_telepathy(target);       break;
    case "block":           fx_block(target);           break;
    case "tk":              fx_tk(target);              break;
    case "empathy":         fx_empathy(target);         break;
    case "obj_read":        fx_obj_read(target);        break;
    case "psi_sense_evil":  fx_psi_sense_evil(target);  break;
    case "sixth_sense":     fx_sixth_sense(target);     break;
    case "mind_bolt":       fx_mind_bolt(target);       break;
    case "mind_bolt_heavy": fx_mind_bolt_heavy(target); break;
    case "psi_sword":       fx_psi_sword(target);       break;
    case "bio_regen":       fx_bio_regen(target);       break;
    case "detect_psionics": fx_detect_psionics(target); break;
    case "pyro_small":      fx_pyro_small(target);      break;
    case "pyro_bolt":       fx_pyro_bolt(target);       break;
    case "pyro_blast":      fx_pyro_blast(target);      break;
    case "healing_touch":   fx_healing_touch(target);   break;
    case "psychic_diag":    fx_psychic_diag(target);    break;
    case "incr_healing":    fx_incr_healing(target);    break;
    case "deaden_pain":     fx_deaden_pain(target);     break;
    case "psych_surgery":   fx_psych_surgery(target);   break;
    case "induce_sleep":    fx_induce_sleep(target);    break;
    case "bio_regen_psi":   fx_bio_regen_psi(target);  break;
    case "nightvision":     fx_nightvision(target);    break;
    case "see_invisible":   fx_see_invisible(target);  break;
    case "bio_manipulation": fx_bio_manipulation(target); break;
    case "clairvoyance":    fx_clairvoyance(target);    break;
    case "precognition":    fx_precognition(target);    break;
    case "psi_shield":      fx_psi_shield(target);      break;
    case "tk_punch":        fx_tk_punch(target);        break;
    case "telemechanics":   fx_telemechanics(target);   break;
    case "remote_viewing":  fx_remote_viewing(target);  break;
    case "accelerate_healing": fx_accelerate_healing(target); break;
    case "group_mind_block": fx_group_mind_block(target); break;
    case "psychic_purification": fx_psychic_purification(target); break;
    case "ectoplasm":       fx_ectoplasm(target);       break;
    case "electrokinesis":  fx_electrokinesis(target);  break;
    case "hypnotic_suggestion": fx_hypnotic_suggestion(target); break;
    case "sense_time":      fx_sense_time(target);      break;
    case "empathic_transmission": fx_empathic_transmission(target); break;
    case "bio_feedback":    fx_bio_feedback(target);    break;
    case "presence_sense":  fx_presence_sense(target);  break;
    case "psychic_locator": fx_psychic_locator(target); break;
    case "mental_stun":     fx_mental_stun(target);     break;
    case "sense_magic_psi": fx_sense_magic_psi(target); break;
    case "total_recall":    fx_total_recall(target);    break;
    case "expand_senses":   fx_expand_senses(target);   break;
    case "inner_strength":  fx_inner_strength(target);  break;
    case "tk_force_field":  fx_tk_force_field(target);  break;
    case "mind_wipe":       fx_mind_wipe(target);       break;
    case "pyro_aura":       fx_pyro_aura(target);       break;
    }
}

/* Utility */

string *query_all_psionics() {
    return ({
        "telepathy", "mind block", "telekinesis", "empathy", "object read",
        "sense evil", "sixth sense", "mind bolt", "mind bolt heavy",
        "psi-sword", "bio-regeneration",
        "detect psionics",
        "pyrokinesis", "pyrokinesis bolt", "pyrokinesis blast",
        "healing touch", "psychic diagnosis", "increased healing",
        "deaden pain", "psychic surgery", "induce sleep",
        "bio regeneration psi", "nightvision", "see the invisible",
        "bio-manipulation", "clairvoyance", "precognition", "psi-shield",
        "telekinetic punch", "telemechanics", "remote viewing",
        "accelerate healing", "group mind block", "psychic purification",
        "ectoplasm", "electrokinesis", "hypnotic suggestion", "sense time",
        "empathic transmission", "bio-feedback",
        "presence sense", "psychic locator", "mental stun", "sense magic",
        "total recall", "expand senses", "summon inner strength",
        "telekinetic force field", "mind wipe", "pyrokinetic aura"
    });
}

private void write_psionic_line(string name, mapping pdata, string tag, int verbose) {
    string alias;
    string label;

    alias = query_psionic_alias_hint(name);
    label = capitalize(name);
    if(alias && sizeof(alias))
        label += " (" + alias + ")";
    if(verbose && pdata)
        write(sprintf("  %-28s ISP %-3d  %s  %s",
            label, (int)pdata["isp_cost"], tag, (string)pdata["desc"]));
    else if(pdata)
        write(sprintf("  %-28s ISP %-3d  %s",
            label, (int)pdata["isp_cost"], tag));
    else
        write(sprintf("  %-28s  %s", label, tag));
}

#define LIST_COLUMNS 4

private void write_psionic_group(string *names, int verbose) {
    string psi;
    mapping pdata;
    string *labels;
    int i;

    if(!names || !sizeof(names)) {
        write("  (none)");
        return;
    }
    if(verbose) {
        for(i = 0; i < sizeof(names); i++) {
            psi = names[i];
            pdata = query_psionic(psi);
            write_psionic_line(psi, pdata, "", 1);
        }
        return;
    }
    labels = ({});
    for(i = 0; i < sizeof(names); i++)
        labels += ({ capitalize(names[i]) });
    write(format_page(labels, LIST_COLUMNS));
}

void write_psionic_list(object pl, string mode) {
    string known;
    string *known_list;
    string *race_grants;
    string *occ_grants;
    string *section;
    string race;
    string occ;
    string psi;
    int i;
    int verbose;
    int show_all;

    if(!pl) return;
    verbose = (mode == "verbose");
    show_all = (mode == "all" || mode == "verbose");
    race = lower_case((string)pl->query_race());
    occ = lower_case((string)pl->getenv("rifts_occ"));
    if(!occ) occ = "none";

    write("=== KNOWN PSIONICS ===");
    write(sprintf("ISP: %d / %d",
        (int)pl->query_stats("ISP"), (int)pl->query_stats("max_ISP")));
    write(repeat_string("-", 60));

    if(race == "godling" && !show_all) {
        known_list = query_all_psionics();
        write("[Master] Godling: full psionic spectrum (" +
            sizeof(known_list) + " powers)");
        write("  Type 'psionics all' for the grouped list.");
        write("  Type 'psionics verbose' for descriptions.");
        write(repeat_string("-", 60));
        write("Syntax: psi <power> [at <target>]");
        return;
    }

    if(race == "godling")
        known_list = query_all_psionics();
    else {
        known = (string)pl->getenv("known_psionics");
        if(!known || !sizeof(known)) {
            write("  You have no psionic abilities.");
            write(repeat_string("-", 60));
            write("Syntax: psi <power> [at <target>]");
            return;
        }
        known_list = explode(known, ";");
    }

    race_grants = (string *)RIFTS_START_D->query_race_psionic_grant(race);
    occ_grants = (string *)RIFTS_START_D->query_occ_psionic_grant(occ);

    if(race == "godling") {
        write("[Master]");
        write_psionic_group(known_list, verbose);
        write(repeat_string("-", 60));
        write("Syntax: psi <power> [at <target>]");
        return;
    }

    write("[Innate]");
    section = ({});
    for(i = 0; i < sizeof(known_list); i++) {
        psi = known_list[i];
        if(!psi || !sizeof(psi)) continue;
        if(member_array(psi, race_grants) == -1) continue;
        section += ({ psi });
    }
    write_psionic_group(section, verbose);

    write("[OCC]");
    section = ({});
    for(i = 0; i < sizeof(known_list); i++) {
        psi = known_list[i];
        if(!psi || !sizeof(psi)) continue;
        if(member_array(psi, race_grants) != -1) continue;
        if(member_array(psi, occ_grants) == -1) continue;
        section += ({ psi });
    }
    write_psionic_group(section, verbose);

    write("[Learned]");
    section = ({});
    for(i = 0; i < sizeof(known_list); i++) {
        psi = known_list[i];
        if(!psi || !sizeof(psi)) continue;
        if(member_array(psi, race_grants) != -1) continue;
        if(member_array(psi, occ_grants) != -1) continue;
        section += ({ psi });
    }
    write_psionic_group(section, verbose);

    write(repeat_string("-", 60));
    write("Syntax: psi <power> [at <target>]");
}
