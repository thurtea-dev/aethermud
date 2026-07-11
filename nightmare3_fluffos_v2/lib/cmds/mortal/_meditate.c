// /cmds/mortal/_meditate.c
// Meditate to restore PPE (magic OCCs on ley line) or ISP (psionic OCCs).
// 10 minute cooldown.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private int is_ley_line_room(object env) {
    if(!env) return 0;
    return (int)RIFTS_D->room_on_ley_line(env);
}

private int is_magic_occ(object player) {
    string occ;
    string flags;
    occ   = lower_case((string)player->getenv("rifts_occ"));
    flags = lower_case((string)player->getenv("rifts_occ_flags"));
    if(!occ) occ = "";
    if(!flags) flags = "";
    if(strsrch(flags, "magic") != -1) return 1;
    if(strsrch(occ, "ley line") != -1) return 1;
    if(strsrch(occ, "techno-wizard") != -1) return 1;
    if(strsrch(occ, "mystic") != -1) return 1;
    if(strsrch(occ, "shifter") != -1) return 1;
    if(strsrch(occ, "shaman") != -1) return 1;
    return 0;
}

private int is_psionic_occ(object player) {
    string occ;
    string flags;
    occ   = lower_case((string)player->getenv("rifts_occ"));
    flags = lower_case((string)player->getenv("rifts_occ_flags"));
    if(!occ) occ = "";
    if(!flags) flags = "";
    if(strsrch(flags, "psychic") != -1) return 1;
    if(strsrch(occ, "mind melter") != -1) return 1;
    if(strsrch(occ, "cyber-knight") != -1) return 1;
    if(strsrch(occ, "crazy") != -1) return 1;
    if(strsrch(occ, "psi-stalker") != -1) return 1;
    return 0;
}

void do_isp_restore(object player, int amount) {
    int isp;
    int max_isp;
    int gain;

    if(!objectp(player)) return;
    isp     = (int)player->query_stats("ISP");
    max_isp = (int)player->query_stats("max_ISP");
    gain    = amount;
    if(isp + gain > max_isp) gain = max_isp - isp;
    if(gain > 0) {
        player->set_stats("ISP", isp + gain);
        tell_object(player,
            "You emerge from meditation refreshed. " + gain + " ISP restored.\n");
    } else {
        tell_object(player, "You emerge from meditation. Your ISP is full.\n");
    }
}

void do_ppe_restore(object player, int amount) {
    int ppe;
    int max_ppe;
    int gain;

    if(!objectp(player)) return;
    ppe     = (int)player->query_stats("PPE");
    max_ppe = (int)player->query_stats("max_PPE");
    gain    = amount;
    if(ppe + gain > max_ppe) gain = max_ppe - ppe;
    if(gain > 0) {
        player->set_stats("PPE", ppe + gain);
        tell_object(player,
            "You emerge from meditation. " + gain + " PPE drawn from the ley line.\n");
    } else {
        tell_object(player, "You emerge from meditation. Your PPE is full.\n");
    }
}

int cmd_meditate(string str) {
    object env;
    int now;
    int cooldown;
    string cd_str;
    int level;
    int in_ley_line;
    int magic_user;
    int psi_user;
    int gain;

    env = environment(this_player());
    if(!env || !objectp(env)) {
        write("You are nowhere.\n");
        return 1;
    }

    now      = time();
    cd_str   = (string)this_player()->getenv("meditate_cooldown");
    cooldown = cd_str ? to_int(cd_str) : 0;
    if(cooldown > now) {
        write("You are not yet ready to meditate again.\n");
        return 1;
    }

    in_ley_line = is_ley_line_room(env);
    magic_user  = is_magic_occ(this_player());
    psi_user    = is_psionic_occ(this_player());
    level       = (int)this_player()->query_level();
    if(level < 1) level = 1;

    if(psi_user) {
        int isp;
        int max_isp;
        isp     = (int)this_player()->query_stats("ISP");
        max_isp = (int)this_player()->query_stats("max_ISP");
        if(max_isp <= 0) {
            write("You have no psionic energy to restore.\n");
            return 1;
        }
        if(isp >= max_isp) {
            write("Your ISP is already full.\n");
            return 1;
        }
        gain = level * 5;
        this_player()->setenv("meditate_cooldown",
            sprintf("%d", now + 600));
        write("You enter a meditative trance. Your inner strength returns.\n");
        this_player()->tell_room_living(env, this_player(), 0,
            " sits quietly in meditation.\n");
        call_out("do_isp_restore", 60, this_player(), gain);
        return 1;
    }

    if(magic_user && in_ley_line) {
        int ppe;
        int max_ppe;
        ppe     = (int)this_player()->query_stats("PPE");
        max_ppe = (int)this_player()->query_stats("max_PPE");
        if(max_ppe <= 0) {
            write("You have no PPE to restore.\n");
            return 1;
        }
        if(ppe >= max_ppe) {
            write("Your PPE is already full.\n");
            return 1;
        }
        gain = level * 10;
        this_player()->setenv("meditate_cooldown",
            sprintf("%d", now + 600));
        write("You draw PPE from the ley line. It flows through you.\n");
        this_player()->tell_room_living(env, this_player(), 0,
            " draws ley line energy inward.\n");
        call_out("do_ppe_restore", 60, this_player(), gain);
        return 1;
    }

    if(in_ley_line) {
        int ppe;
        int max_ppe;
        ppe     = (int)this_player()->query_stats("PPE");
        max_ppe = (int)this_player()->query_stats("max_PPE");
        if(max_ppe > 0 && ppe < max_ppe) {
            gain = 10;
            this_player()->setenv("meditate_cooldown",
                sprintf("%d", now + 600));
            write("You sense the ley line energy and draw a small measure of it.\n");
            this_player()->tell_room_living(env, this_player(), 0,
                " sits quietly, drawing ambient energy inward.\n");
            call_out("do_ppe_restore", 60, this_player(), gain);
            return 1;
        }
        write("You sit quietly, but have no PPE to restore.\n");
        return 1;
    }

    write("Meditation requires inner psionic power or proximity to a ley line.\n");
    return 1;
}

void help() {
    write(
        "Syntax: meditate\n\n"
        "Concentrate to restore PPE or ISP depending on your abilities.\n\n"
        "  Psionic OCC: restores ISP equal to level*5 after 60 seconds.\n"
        "  Magic OCC on a ley line: restores PPE equal to level*10 after 60 seconds.\n"
        "  Anyone on a ley line: restores 10 PPE after 60 seconds.\n\n"
        "Cooldown: 10 minutes between uses.\n"
    );
}
