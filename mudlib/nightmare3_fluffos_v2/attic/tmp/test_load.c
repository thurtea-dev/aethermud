// Temporary test to force-load rifts_combat and expose any compile error.
void create() {
    string err;
    err = catch(call_other("/daemon/rifts_combat", "???"));
    if(err) {
        write("RIFTS_COMBAT LOAD ERROR: " + err + "\n");
        log_file("runtime", "RIFTS_COMBAT TEST ERROR: " + err + "\n");
    } else {
        write("rifts_combat loaded OK\n");
        log_file("runtime", "rifts_combat loaded OK\n");
    }
}
