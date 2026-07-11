/* /cmds/mortal/_guild.c
   Guild system command.
   Syntax:
     guild                      -- show your guild info
     guild create <name> <tag>  -- found a new guild (5000 credits)
     guild invite <player>      -- invite a player (leader only)
     guild accept               -- accept pending invite
     guild decline              -- decline pending invite
     guild kick <player>        -- remove a member (leader only)
     guild leave                -- leave your guild
     guild bank                 -- show bank balance
     guild deposit <n>          -- deposit credits
     guild withdraw <n>         -- withdraw credits (rank 2+)
     guild members              -- list guild members
     guild tag on               -- enable [TAG] prefix in say
     guild tag off              -- disable [TAG] prefix in say */

#include <std.h>
#include <daemons.h>

#define GUILD_D "/daemon/guild_d"
#define GUILD_COST 5000

inherit DAEMON;

int cmd_guild(string str) {
    string cmd, arg, pname, gname, tag;
    object g, partner, gb;
    mapping guild_data;
    int rank, credits;
    int sp;

    pname = lower_case((string)this_player()->query_name());
    g = find_object(GUILD_D);
    if(!g) g = load_object(GUILD_D);
    if(!g) {
        write("Guild system unavailable.\n");
        return 1;
    }

    if(!str || str == "") {
        gname = (string)g->query_guild(this_player());
        if(!gname || gname == "") {
            write("You are not a member of any guild.\n"
                  "Use 'guild create <name> <tag>' to found one (5000 credits).\n");
            return 1;
        }
        guild_data = (mapping)g->query_guild_data(gname);
        if(!guild_data) { write("Guild data error.\n"); return 1; }
        rank = (int)g->query_rank(this_player());
        write("=== Guild: " + upper_case(gname) + " [" +
              (string)guild_data["tag"] + "] ===\n");
        write("Your rank: " +
            (rank == 1 ? "Leader" : (rank == 2 ? "Officer" : "Member")) + "\n");
        write("Bank: " + g->query_guild_bank(gname) + " credits\n");
        return 1;
    }

    sp = strsrch(str, " ");
    if(sp == -1) { cmd = lower_case(str); arg = ""; }
    else { cmd = lower_case(str[0..sp-1]); arg = str[sp+1..]; }

    /* ── guild create <name> <tag> ─────────────────────────────────── */
    if(cmd == "create") {
        int sp2;
        string garg, gtag;

        if(!arg || arg == "") {
            write("Syntax: guild create <name> <tag>\n"
                  "The tag is a short 2-4 character identifier (e.g. BS).\n");
            return 1;
        }
        sp2 = strsrch(arg, " ");
        if(sp2 == -1) {
            write("Syntax: guild create <name> <tag>\n");
            return 1;
        }
        garg = arg[0..sp2-1];
        gtag = arg[sp2+1..];
        if(strlen(gtag) < 1 || strlen(gtag) > 4) {
            write("Guild tag must be 1-4 characters.\n");
            return 1;
        }
        if(strlen(garg) < 2) {
            write("Guild name must be at least 2 characters.\n");
            return 1;
        }
        gname = (string)g->query_guild(this_player());
        if(gname && gname != "") {
            write("You are already in a guild. Leave it first.\n");
            return 1;
        }
        credits = (int)this_player()->query_money("credits");
        if(credits < GUILD_COST) {
            write("Founding a guild costs " + GUILD_COST + " credits. "
                  "You have " + credits + ".\n");
            return 1;
        }
        if(!(int)g->create_guild(garg, gtag, this_player())) {
            write("That guild name or tag is already taken.\n");
            return 1;
        }
        this_player()->add_money("credits", -GUILD_COST);
        write("Guild '" + upper_case(garg) + "' [" + upper_case(gtag) +
              "] founded! " + GUILD_COST + " credits deducted.\n");
        return 1;
    }

    /* ── guild invite <player> ─────────────────────────────────────── */
    if(cmd == "invite") {
        gname = (string)g->query_guild(this_player());
        if(!gname || gname == "") { write("You are not in a guild.\n"); return 1; }
        rank = (int)g->query_rank(this_player());
        if(rank != 1) { write("Only the guild leader can invite members.\n"); return 1; }
        if(!arg || arg == "") { write("Invite whom?\n"); return 1; }
        partner = find_player(lower_case(arg));
        if(!partner || !interactive(partner)) { write("That player is not online.\n"); return 1; }
        {
            string partner_guild;
            partner_guild = (string)g->query_guild(partner);
            if(partner_guild && partner_guild != "") {
                write(capitalize(arg) + " is already in a guild.\n");
                return 1;
            }
        }
        partner->set_property("guild_invite", gname);
        partner->set_property("guild_invite_from", pname);
        tell_object(partner,
            capitalize(pname) + " has invited you to join guild '" +
            upper_case(gname) + "'.\n"
            "Type 'guild accept' or 'guild decline'.\n");
        write("Invitation sent to " + capitalize(arg) + ".\n");
        return 1;
    }

    /* ── guild accept ──────────────────────────────────────────────── */
    if(cmd == "accept") {
        string invite_guild, inviter;
        object inv_ob;

        invite_guild = (string)this_player()->query_property("guild_invite");
        inviter      = (string)this_player()->query_property("guild_invite_from");
        if(!invite_guild || invite_guild == "") {
            write("You have no pending guild invitation.\n");
            return 1;
        }
        gname = (string)g->query_guild(this_player());
        if(gname && gname != "") {
            write("You are already in a guild. Leave it first.\n");
            return 1;
        }
        if(!(int)g->add_member(invite_guild, this_player(), 3)) {
            write("Could not join guild (it may no longer exist).\n");
            this_player()->set_property("guild_invite", "");
            this_player()->set_property("guild_invite_from", "");
            return 1;
        }
        this_player()->set_property("guild_invite", "");
        this_player()->set_property("guild_invite_from", "");
        write("You joined guild '" + upper_case(invite_guild) + "'.\n");
        if(inviter && inviter != "") {
            inv_ob = find_player(inviter);
            if(inv_ob)
                tell_object(inv_ob, capitalize(pname) + " joined the guild.\n");
        }
        return 1;
    }

    /* ── guild decline ─────────────────────────────────────────────── */
    if(cmd == "decline") {
        string invite_guild, inviter;
        object inv_ob;

        invite_guild = (string)this_player()->query_property("guild_invite");
        if(!invite_guild || invite_guild == "") {
            write("You have no pending guild invitation.\n");
            return 1;
        }
        inviter = (string)this_player()->query_property("guild_invite_from");
        this_player()->set_property("guild_invite", "");
        this_player()->set_property("guild_invite_from", "");
        write("Guild invitation declined.\n");
        if(inviter && inviter != "") {
            inv_ob = find_player(inviter);
            if(inv_ob)
                tell_object(inv_ob, capitalize(pname) + " declined your guild invitation.\n");
        }
        return 1;
    }

    /* ── guild kick <player> ───────────────────────────────────────── */
    if(cmd == "kick") {
        string tkname;
        object target;

        gname = (string)g->query_guild(this_player());
        if(!gname || gname == "") { write("You are not in a guild.\n"); return 1; }
        rank = (int)g->query_rank(this_player());
        if(rank != 1) { write("Only the leader can kick members.\n"); return 1; }
        if(!arg || arg == "") { write("Kick whom?\n"); return 1; }
        tkname = lower_case(arg);
        if(tkname == pname) { write("You cannot kick yourself. Use 'guild leave'.\n"); return 1; }
        {
            string target_guild;
            target = find_player(tkname);
            if(!target) {
                write("That player is not online.\n");
                return 1;
            }
            target_guild = (string)g->query_guild(target);
            if(!target_guild || lower_case(target_guild) != lower_case(gname)) {
                write(capitalize(tkname) + " is not in your guild.\n");
                return 1;
            }
            g->remove_member(gname, target);
            target->set_property("guild_tag_on", 0);
            target->set_property("guild_tag", "");
        }
        write(capitalize(tkname) + " has been removed from the guild.\n");
        if(target)
            tell_object(target, "You have been removed from guild '" +
                upper_case(gname) + "'.\n");
        return 1;
    }

    /* ── guild leave ───────────────────────────────────────────────── */
    if(cmd == "leave") {
        gname = (string)g->query_guild(this_player());
        if(!gname || gname == "") { write("You are not in a guild.\n"); return 1; }
        rank = (int)g->query_rank(this_player());
        if(rank == 1) {
            /* leader leaving - need to check if others exist */
            guild_data = (mapping)g->query_guild_data(gname);
            if(guild_data && sizeof((mapping)guild_data["members"]) > 1) {
                write("You are the leader. Transfer leadership or kick all members first.\n");
                return 1;
            }
        }
        g->remove_member(gname, this_player());
        this_player()->set_property("guild_tag_on", 0);
        this_player()->set_property("guild_tag", "");
        write("You have left guild '" + upper_case(gname) + "'.\n");
        return 1;
    }

    /* ── guild bank ────────────────────────────────────────────────── */
    if(cmd == "bank") {
        gname = (string)g->query_guild(this_player());
        if(!gname || gname == "") { write("You are not in a guild.\n"); return 1; }
        write("Guild bank balance: " + g->query_guild_bank(gname) + " credits.\n");
        return 1;
    }

    /* ── guild deposit <n> ─────────────────────────────────────────── */
    if(cmd == "deposit") {
        int amount;

        gname = (string)g->query_guild(this_player());
        if(!gname || gname == "") { write("You are not in a guild.\n"); return 1; }
        amount = to_int(arg);
        if(amount <= 0) { write("Deposit how many credits?\n"); return 1; }
        credits = (int)this_player()->query_money("credits");
        if(credits < amount) {
            write("You only have " + credits + " credits.\n");
            return 1;
        }
        this_player()->add_money("credits", -amount);
        g->deposit_guild(gname, amount);
        write("Deposited " + amount + " credits to guild bank. "
              "New balance: " + g->query_guild_bank(gname) + ".\n");
        return 1;
    }

    /* ── guild withdraw <n> ────────────────────────────────────────── */
    if(cmd == "withdraw") {
        int amount;

        gname = (string)g->query_guild(this_player());
        if(!gname || gname == "") { write("You are not in a guild.\n"); return 1; }
        rank = (int)g->query_rank(this_player());
        if(rank > 2) {
            write("Only guild leaders and officers can withdraw from the bank.\n");
            return 1;
        }
        amount = to_int(arg);
        if(amount <= 0) { write("Withdraw how many credits?\n"); return 1; }
        if(!(int)g->withdraw_guild(gname, this_player(), amount)) {
            write("Insufficient guild bank funds.\n");
            return 1;
        }
        this_player()->add_money("credits", amount);
        write("Withdrew " + amount + " credits from guild bank. "
              "New balance: " + g->query_guild_bank(gname) + ".\n");
        return 1;
    }

    /* ── guild members ─────────────────────────────────────────────── */
    if(cmd == "members") {
        gname = (string)g->query_guild(this_player());
        if(!gname || gname == "") { write("You are not in a guild.\n"); return 1; }
        write("=== " + upper_case(gname) + " Members ===\n");
        write((string)g->query_member_list(gname));
        return 1;
    }

    /* ── guild tag on/off ──────────────────────────────────────────── */
    if(cmd == "tag") {
        string onoff;

        gname = (string)g->query_guild(this_player());
        if(!gname || gname == "") { write("You are not in a guild.\n"); return 1; }
        onoff = lower_case(arg);
        if(onoff == "on") {
            tag = (string)g->query_guild_tag(gname);
            this_player()->set_property("guild_tag",    tag);
            this_player()->set_property("guild_tag_on", 1);
            write("Guild tag [" + tag + "] enabled in say.\n");
        } else if(onoff == "off") {
            this_player()->set_property("guild_tag_on", 0);
            this_player()->set_property("guild_tag",    "");
            write("Guild tag disabled.\n");
        } else {
            write("Syntax: guild tag on  or  guild tag off\n");
        }
        return 1;
    }

    write("Unknown guild command. Type 'help guild' for syntax.\n");
    return 1;
}

void help() {
    write(
        "Syntax:\n"
        "  guild                      -- show your guild info\n"
        "  guild create <name> <tag>  -- found a guild (5000 credits)\n"
        "  guild invite <player>      -- invite a player (leader only)\n"
        "  guild accept               -- accept a guild invitation\n"
        "  guild decline              -- decline a guild invitation\n"
        "  guild kick <player>        -- remove a member (leader only)\n"
        "  guild leave                -- leave your guild\n"
        "  guild bank                 -- show guild bank balance\n"
        "  guild deposit <n>          -- deposit credits to guild bank\n"
        "  guild withdraw <n>         -- withdraw credits (rank 2+)\n"
        "  guild members              -- list all guild members\n"
        "  guild tag on/off           -- toggle [TAG] prefix in say\n\n"
        "Guild tags display in say when 'guild tag on' is active.\n"
        "Founding a guild costs 5000 credits.\n"
    );
}
