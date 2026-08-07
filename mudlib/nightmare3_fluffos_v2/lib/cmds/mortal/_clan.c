/* /cmds/mortal/_clan.c
   Clan joining and inquiry command.
   Currently: Aerihman (unlocks Sunaj Assassin OCC)
   Syntax: clan
           clan aerihman
           clan join aerihman */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

void init() {
    add_action("cmd_clan", "clan");
}

private int _check_sunaj_clue(object player) {
    int count;

    count = 0;
    if((string)player->getenv("aerihman_clue_1") == "1") count++;
    if((string)player->getenv("aerihman_clue_2") == "1") count++;
    if((string)player->getenv("aerihman_clue_3") == "1") count++;
    return count >= 2;
}

private int _can_join_aerihman(object player) {
    string race;

    if(creatorp(player)) return 1;
    race = lower_case((string)player->query_race() || "");
    if(strsrch(race, "atlantean") != -1) return 1;
    return _check_sunaj_clue(player);
}

private void _ensure_rifts_body(object player) {
    mapping borg, occ_data;
    string race, occ, *keys_arr;
    int i, sdc, mdc, hp, ppe, me;

    if((int)player->query_stats("IQ") > 0) return;

    race = (string)player->query_race();
    if(!race || !(int)RIFTS_D->is_rifts_race(race)) return;

    borg = (mapping)RIFTS_D->do_rifts_rolls(race);
    keys_arr = keys(borg);
    for(i = 0; i < sizeof(keys_arr); i++)
        player->set_stats(keys_arr[i], borg[keys_arr[i]]);

    if((int)RIFTS_D->is_mdc_race(race)) {
        mdc = (int)RIFTS_D->init_mdc(race, borg["PE"]);
        player->set_stats("MDC", mdc);
        player->set_stats("max_MDC", mdc);
        player->set_stats("SDC", 0);
        player->set_stats("max_SDC", 0);
        player->set_stats("is_MDC", 1);
    } else {
        sdc = (int)RIFTS_D->init_sdc(race, borg["PE"]);
        player->set_stats("SDC", sdc);
        player->set_stats("max_SDC", sdc);
        player->set_stats("MDC", 0);
        player->set_stats("max_MDC", 0);
        player->set_stats("is_MDC", 0);
    }

    hp = (int)RIFTS_D->init_hp(race, borg["PE"]);
    player->set_stats("rifts_hp", hp);
    player->set_stats("max_rifts_hp", hp);

    occ = lower_case((string)player->getenv("rifts_occ") || "");
    occ_data = (mapping)OCC_D->query_occ(occ);
    if(!mapp(occ_data)) return;

    ppe = (int)occ_data["ppe_base"];
    if(ppe > 0) {
        me = (int)player->query_stats("ME");
        ppe += me;
        player->set_stats("PPE", ppe);
        player->set_stats("max_PPE", ppe);
    }
    player->set_stats("PPE_per_level", (int)occ_data["ppe_per_level"]);
}

private void _apply_sunaj_occ(object player) {
    mapping occ_data;
    string *occ_flags;
    string race_flags;
    string occ;

    occ = "sunaj assassin";
    occ_data = (mapping)OCC_D->query_occ(occ);
    if(!mapp(occ_data)) return;

    player->setenv("rifts_occ", "Sunaj Assassin");
    player->set_class(occ);

    occ_flags = (string *)occ_data["flags"];
    if(!occ_flags) occ_flags = ({});
    player->setenv("rifts_occ_flags", implode(occ_flags, ","));

    race_flags = (string)player->getenv("rifts_flags");
    if(!race_flags) race_flags = "";
    if(sizeof(occ_flags)) {
        if(sizeof(race_flags))
            player->setenv("rifts_flags",
                race_flags + "," + implode(occ_flags, ","));
        else
            player->setenv("rifts_flags", implode(occ_flags, ","));
    }
}

private int _join_aerihman(object player) {
    string current_clan, occ, pname;

    current_clan = lower_case((string)player->getenv("clan") || "");
    occ          = lower_case((string)player->getenv("rifts_occ") || "");
    pname        = lower_case((string)player->query_name());

    if(sizeof(current_clan) && current_clan != "aerihman") {
        write("You are already a member of " + capitalize(current_clan) + ".\n"
              "A Sunaj serves one master only.\n");
        return 1;
    }

    if(!_can_join_aerihman(player)) {
        write("You have heard rumours of a name  -  a fragment, nothing more.\n"
              "Seek the full truth before you dare speak it aloud.\n");
        return 1;
    }

    if(occ == "sunaj assassin" && current_clan == "aerihman") {
        write("You are already walking the path of the Sunaj.\n"
              "The clan recognises you.\n");
        return 1;
    }

    player->setenv("clan", "aerihman");
    player->setenv("tattoo_slots", "20");
    _apply_sunaj_occ(player);
    _ensure_rifts_body(player);
    catch(RIFTS_START_D->grant_occ_package(player));
    catch(TATTOO_D->grant_tattoo(player, "shadow meld", "back"));
    catch(FACTION_D->add_member("aerihman", pname, "initiate"));
    catch(FACTION_D->add_member("sunaj", pname, "assassin"));
    catch(player->save_player(pname));

    write("\n"
          "The shadows shift around you.\n"
          "A mark appears on your skin  -  not painful, but undeniable.\n"
          "You are Sunaj. The Order of Aerihman claims you.\n\n"
          "OCC changed to: Sunaj Assassin\n"
          "Your tattoo slots are now 20.\n"
          "Starting gear, tattoos, and skills have been granted.\n\n"
          "The darkness is your ally. Walk quietly.\n");

    say(player->query_cap_name() +
        " shudders as shadow tattoos begin to appear on their skin.",
        player);

    log_file("rp_wizard/clan",
        (string)player->query_name() + " joined Aerihman (Sunaj Assassin) "
        "at " + ctime(time()) + "\n");
    return 1;
}

int cmd_clan(string str) {
    object player;
    string current_clan, clan_name;

    player = this_player();
    if(!player) return 0;

    current_clan = lower_case((string)player->getenv("clan") || "");

    if(!str || !sizeof(str)) {
        if(!sizeof(current_clan)) {
            write("You are not a member of any clan.\n"
                  "Clans are secretive organizations within Rifts Earth.\n"
                  "Seek out rumours in Splynn, the catacombs, and the Lazlo library.\n"
                  "Type 'clan join <name>' once you know of one.\n");
        } else {
            write("You are a member of clan: " + capitalize(current_clan) + "\n");
            if(current_clan == "aerihman")
                write("The Order of Aerihman  -  Sunaj Assassins.\n"
                      "Shadow is your weapon. The Tattoo your oath.\n");
        }
        return 1;
    }

    str = lower_case(str);

    if(str == "leave") {
        if(!sizeof(current_clan)) {
            write("You are not a member of any clan.\n");
            return 1;
        }
        player->setenv("clan", "");
        write("You leave " + capitalize(current_clan) + ".\n");
        catch(player->save_player((string)player->query_name()));
        return 1;
    }

    if(str == "aerihman")
        return _join_aerihman(player);

    if(str[0..3] == "join") {
        if(sizeof(str) <= 5) {
            write("Join which clan?  Syntax: clan join <name>\n");
            return 1;
        }
        clan_name = str[5..];
        if(clan_name == "aerihman")
            return _join_aerihman(player);
        write("There is no clan by that name that will answer your call.\n");
        return 1;
    }

    write("Syntax: clan               -- check your clan status\n"
          "        clan <name>        -- join a clan you know\n"
          "        clan join <name>   -- join a clan\n"
          "        clan leave         -- leave your current clan\n");
    return 1;
}
