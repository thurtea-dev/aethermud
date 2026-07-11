// /daemon/skill_request_d.c
// Stores pending skill requests from players for RP wizard review.
// Each player can have one pending request at a time.

inherit "/secure/std/daemon";

private mapping __requests;

void create() {
    __requests = ([]);
}

// Store a request. Replaces any existing request from this player.
void submit_request(object player, string request_text) {
    string pname;
    if(!player || !request_text || !sizeof(request_text)) return;
    pname = (string)player->query_name();
    __requests[pname] = ([
        "text":      request_text,
        "time":      time(),
        "cap_name":  (string)player->query_cap_name()
    ]);
}

// Returns a mapping of all pending requests: player_name -> data
mapping get_all_requests() {
    return __requests + ([]);
}

// Returns one player's request data mapping, or 0 if none.
mapping get_request(string player_name) {
    if(!player_name) return 0;
    return __requests[player_name];
}

// Removes a player's request after it is processed.
void clear_request(string player_name) {
    if(!player_name) return;
    map_delete(__requests, player_name);
}
