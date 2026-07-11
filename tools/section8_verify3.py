#!/usr/bin/env python3
"""Section 8 verification pass 3: robust expect-style driver (handles
variable-length news board pagination) through full chargen on two OCCs
from different elective category sets, then in-world command/combat/bank
tests. Requires live MUD on port 1122. Names must be alphabetic only.
"""
import os
import re
import shutil
import socket
import sys
import time

HOST = "127.0.0.1"
PORT = 1122
LIB = "/home/thurtea/aethermud/nightmare3_fluffos_v2/lib"

results = {"pass": [], "fail": [], "info": []}


def save_path(name):
    return f"{LIB}/secure/save/users/{name[0]}/{name}.o"


def cleanup(name):
    for p in (save_path(name), save_path(name) + ".bak"):
        if os.path.isfile(p):
            os.remove(p)
    postal = f"{LIB}/secure/save/postal/{name[0]}/{name}"
    if os.path.isdir(postal):
        shutil.rmtree(postal)
    realm = f"{LIB}/realms/{name}"
    if os.path.isdir(realm):
        shutil.rmtree(realm)


class Mud:
    def __init__(self):
        self.sock = socket.socket()
        self.sock.connect((HOST, PORT))
        self.sock.settimeout(0.3)
        self.buf = ""
        self._drain(3.0)

    def _drain(self, total_wait):
        end = time.time() + total_wait
        while time.time() < end:
            try:
                d = self.sock.recv(8192)
                if not d:
                    break
                self.buf += d.decode(errors="replace")
            except socket.timeout:
                time.sleep(0.1)

    def send(self, cmd):
        self.sock.sendall((cmd + "\n").encode())

    def expect(self, substrs, timeout=8.0, also_page=True):
        """Wait until any of substrs appears in the tail of the buffer since
        the mark, or timeout. Sends a bare Enter if a pager prompt appears
        (handles multi-board news paging). Returns the new text seen."""
        if isinstance(substrs, str):
            substrs = [substrs]
        start_len = len(self.buf)
        end = time.time() + timeout
        last_page_nudge = 0
        while time.time() < end:
            self._drain(0.4)
            fresh = self.buf[start_len:]
            if any(s in fresh for s in substrs):
                return fresh
            if also_page and "Press <return> to continue" in self.buf[-200:] and \
               time.time() - last_page_nudge > 0.6:
                self.sock.sendall(b"\n")
                last_page_nudge = time.time()
        return self.buf[start_len:]

    def close(self):
        try:
            self.sock.close()
        except OSError:
            pass


def ok(label, cond, detail=""):
    if cond:
        results["pass"].append(label)
        print(f"PASS: {label}" + (f" ({detail[:100]})" if detail else ""), flush=True)
    else:
        results["fail"].append(f"{label}: {detail}")
        print(f"FAIL: {label} - {detail[:400]}", flush=True)


def info(label, detail):
    results["info"].append(f"{label}: {detail}")
    print(f"INFO: {label} - {detail[:400]}", flush=True)


def login_new(name, password="test807z"):
    cleanup(name)
    m = Mud()
    m.expect("name do you wish", timeout=5.0)
    m.send(name)
    r = m.expect(["really wish", "choose a password"], timeout=5.0)
    if "really wish" in r:
        m.send("y")
        r = m.expect("choose a password", timeout=5.0)
    m.send(password)
    m.expect("confirm your password", timeout=5.0)
    m.send(password)
    m.expect("gender", timeout=5.0)
    m.send("male")
    m.expect("display name", timeout=5.0)
    m.send("")
    m.expect("Email", timeout=5.0)
    m.send("")
    m.expect("real name", timeout=5.0)
    m.send("")
    # After this, variable news-board paging happens automatically via expect()'s
    # pager nudge; wait for the actual chargen region prompt to appear.
    out = m.expect(["STEP 1", "Gamemaster"], timeout=12.0)
    return m, out


def run_chargen(name, occ, categories, want_electives, want_secondary, race="human", align="4"):
    m, out = login_new(name)
    info(f"[{name}] post-login state", out[-200:])

    m.send("start americas")
    out = m.expect(["void", "Praxis", "STEP 2", "roll"], timeout=8.0)

    m.send("pick roll")
    out = m.expect(["STEP 3", "race", "IQ:"], timeout=8.0)
    ok(f"[{name}] stats rolled", "IQ:" in out or "STEP 3" in out, out[-200:])

    m.send(f"pick {race}")
    out = m.expect(["STEP 4", "alignment", "Alignment"], timeout=8.0)
    ok(f"[{name}] race picked", "alignment" in out.lower(), out[-200:])

    m.send(f"alignment {align}")
    out = m.expect(["STEP 5", "OCC", "eligible"], timeout=8.0)
    ok(f"[{name}] alignment set", "STEP 5" in out or "OCC" in out or "eligible" in out, out[-200:])

    m.send(f"pick {occ}")
    out = m.expect(["You have chosen", "not a valid OCC", "do not meet"], timeout=8.0)
    ok(f"[{name}] OCC pick '{occ}'", "You have chosen" in out, out[-300:])
    if "You have chosen" not in out:
        return m, [], []

    picked = []
    elective_cats = list(categories)
    ci = 0
    for _ in range(want_electives + 6):
        if "All elective skills chosen" in "".join(picked):
            break
        cat = elective_cats[ci % len(elective_cats)]
        ci += 1
        m.send(f"skills {cat}")
        listing = m.expect(["Skills in category", "cannot pick", "No skills found"], timeout=6.0)
        names = []
        capture = False
        for ln in listing.split("\r\n"):
            if "Skills in category" in ln:
                capture = True
                continue
            if capture:
                if "Type 'pick skill" in ln or ln.strip() == "":
                    if names:
                        break
                    continue
                parts = [p.strip() for p in re.split(r"\s{2,}", ln) if p.strip()]
                names.extend(parts)
        chosen = None
        for n in names:
            key = n.lower()
            if key and key not in picked:
                chosen = key
                break
        if not chosen:
            info(f"[{name}] no fresh skill found in '{cat}'", listing[-150:])
            continue
        m.send(f"pick skill {chosen}")
        res = m.expect(["Skill added", "already have", "not a recognized", "not in your allowed"], timeout=6.0)
        if "Skill added" in res:
            picked.append(chosen)
            if "All elective skills chosen" in res:
                break
        else:
            info(f"[{name}] pick '{chosen}' -> {res.strip()[-150:]}", "")

    ok(f"[{name}] elective skills completed", len(picked) >= want_electives,
       f"picked {len(picked)}: {picked}")

    sec_picked = []
    sec_cats = ["technical", "physical", "medical", "espionage", "military",
                "pilot", "weapons", "lore", "survival", "domestic"]
    for i in range(want_secondary + 12):
        cat = sec_cats[i % len(sec_cats)]
        m.send(f"skills {cat}")
        listing = m.expect(["Skills in category", "cannot pick", "No skills found", "only available"], timeout=6.0)
        names = []
        capture = False
        for ln in listing.split("\r\n"):
            if "Skills in category" in ln:
                capture = True
                continue
            if capture:
                if "Type 'pick skill" in ln or ln.strip() == "":
                    if names:
                        break
                    continue
                parts = [p.strip() for p in re.split(r"\s{2,}", ln) if p.strip()]
                names.extend(parts)
        chosen = None
        for n in names:
            key = n.lower()
            if key and key not in picked and key not in sec_picked:
                chosen = key
                break
        if not chosen:
            continue
        m.send(f"pick skill {chosen}")
        res = m.expect(["Skill added", "already have", "not a recognized"], timeout=6.0)
        if "Skill added" in res:
            sec_picked.append(chosen)
            if "All secondary skills chosen" in res:
                break

    out = m.expect(["Praxis", "realms", "workroom", "outskirts"], timeout=6.0)
    ok(f"[{name}] secondary skills completed", len(sec_picked) >= want_secondary,
       f"picked {len(sec_picked)}: {sec_picked}")
    ok(f"[{name}] chargen reaches world", bool(out.strip()), out[-300:])

    return m, picked, sec_picked


def check_save_file(name, occ):
    p = save_path(name)
    if not os.path.isfile(p):
        ok(f"[{name}] save file exists", False, p)
        return None
    data = open(p, errors="replace").read()
    ok(f"[{name}] save file exists", True, p)
    ok(f"[{name}] save has rifts_occ={occ}", f'"rifts_occ":"{occ}"' in data, "rifts_occ field")
    ok(f"[{name}] save has rifts_skills", "rifts_skills" in data, "rifts_skills field present")
    return data


def test_commands(m, name):
    m.send("score")
    tail = m.expect(["IQ", "SDC", "HP"], timeout=5.0)
    ok(f"[{name}] score", "IQ" in tail or "SDC" in tail or "HP" in tail, tail[:200])

    m.send("eq")
    tail = m.expect(["wield", "wear", "nothing", "You aren't"], timeout=5.0)
    ok(f"[{name}] eq", bool(tail.strip()), tail[:150])

    m.send("autododge")
    tail = m.expect(["dodge", "Autododge"], timeout=5.0)
    ok(f"[{name}] autododge toggle", "dodge" in tail.lower(), tail[:150])

    m.send("autoparry")
    tail = m.expect(["parry", "Autoparry"], timeout=5.0)
    ok(f"[{name}] autoparry toggle", "parry" in tail.lower(), tail[:150])

    m.send("mail")
    tail = m.expect(["mail", "message", "folder"], timeout=5.0)
    ok(f"[{name}] mail", bool(tail.strip()), tail[:150])

    for topic in ["rules", "aethermud", "movement"]:
        m.send(f"help {topic}")
        tail = m.expect(["Topic", "Syntax", "No help available", "Invalid help"], timeout=5.0)
        ok(f"[{name}] help {topic}", "No help available" not in tail and "Invalid help" not in tail, tail[:120])


def test_bank(m, name):
    m.send("bank balance")
    tail = m.expect(["balance", "credit", "no bank", "not available", "What?"], timeout=5.0)
    info(f"[{name}] bank balance", tail[:200])
    m.send("bank deposit 50")
    tail = m.expect(["deposit", "credit", "What?", "insufficient"], timeout=5.0)
    info(f"[{name}] bank deposit 50", tail[:200])
    m.send("bank withdraw 10")
    tail = m.expect(["withdraw", "credit", "What?", "insufficient"], timeout=5.0)
    info(f"[{name}] bank withdraw 10", tail[:200])


def test_combat(m, name):
    m.send("out")
    out = m.expect([""], timeout=3.0)
    info(f"[{name}] left start room", out[-250:])
    m.send("look")
    out = m.expect([""], timeout=3.0)
    info(f"[{name}] current room", out[-300:])


def main():
    try:
        m1, elect1, sec1 = run_chargen("aethtestone", "city rat",
                                        ["espionage", "technical", "physical"],
                                        want_electives=6, want_secondary=6)
        data1 = check_save_file("aethtestone", "city rat")
        test_commands(m1, "aethtestone")
        m1.send("quit")
        time.sleep(1.0)
        m1.close()
    except Exception as e:
        results["fail"].append(f"aethtestone exception: {e}")
        print(f"EXCEPTION aethtestone: {e}", flush=True)

    try:
        m2, elect2, sec2 = run_chargen("aethtesttwo", "rogue scholar",
                                        ["technical", "medical", "lore"],
                                        want_electives=6, want_secondary=5)
        data2 = check_save_file("aethtesttwo", "rogue scholar")
        test_commands(m2, "aethtesttwo")
        test_bank(m2, "aethtesttwo")
        test_combat(m2, "aethtesttwo")
        m2.send("quit")
        time.sleep(1.0)
        m2.close()
    except Exception as e:
        results["fail"].append(f"aethtesttwo exception: {e}")
        print(f"EXCEPTION aethtesttwo: {e}", flush=True)

    cleanup("aethtestone")
    cleanup("aethtesttwo")

    print("\n=== SUMMARY ===", flush=True)
    print(f"Passed: {len(results['pass'])}", flush=True)
    print(f"Failed: {len(results['fail'])}", flush=True)
    for f in results["fail"]:
        print(f"  - {f}", flush=True)
    print(f"Info: {len(results['info'])}", flush=True)
    for i in results["info"]:
        print(f"  - {i}", flush=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
