#!/usr/bin/env python3
"""Section 8 verification pass 4: remaining live-verification items.

  1. Regression: 'score' mid-chargen (no OCC yet) after the daemon/rifts.c
     lower_case() guard fix - this crashed in is_smuggler_occ() before.
  2. Skills category alias fix (daemon/rifts_skills.c): 'skills espionage'
     lists rogue-tagged skills (city rat), 'skills technical' lists
     science-tagged skills (rogue scholar), during elective picks.
  3. Bank walkthrough in the real bank room: show id -> ATM card ->
     deposit/withdraw/balance with number checks.
  4. Real combat vs the test-zone coalition grunt as a Cyber-Knight,
     with autododge/autoparry on and psi-sword active (MDC vs SDC).
  5. Radio transmit between two simultaneous connections on channel 5.

Requires live MUD on port 1122. Reuses the expect-style driver from
section8_verify3.py (the pattern that handles news-board pagination).
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
RUNTIME_LOG = f"{LIB}/log/runtime"

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
        print(f"PASS: {label}" + (f" ({detail[:120]})" if detail else ""), flush=True)
    else:
        results["fail"].append(f"{label}: {detail}")
        print(f"FAIL: {label} - {detail[:400]}", flush=True)


def info(label, detail):
    results["info"].append(f"{label}: {detail}")
    print(f"INFO: {label} - {detail[:300]}", flush=True)


def login_new(name, password="test807z"):
    cleanup(name)
    m = Mud()
    m.expect("name do you wish", timeout=5.0)
    m.send(name)
    r = m.expect(["really wish", "choose a password"], timeout=5.0)
    if "really wish" in r:
        m.send("y")
        m.expect("choose a password", timeout=5.0)
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
    # Flush every stacked news/pager screen: nudge while a pager prompt is
    # pending, and only proceed once the stream has been quiet for a while.
    quiet = 0
    for _ in range(60):
        pre = len(m.buf)
        if "Press <return> to continue" in m.buf[-300:]:
            m.sock.sendall(b"\n")
        m._drain(1.0)
        if len(m.buf) == pre and "Press <return> to continue" not in m.buf[-300:]:
            quiet += 1
            if quiet >= 3:
                break
        else:
            quiet = 0
    return m, m.buf[-400:]


def enter_region(m, name):
    """Send 'start americas', re-flushing any late news screens."""
    for attempt in range(4):
        m.send("start americas")
        out = m.expect(["STEP 2", "outskirts", "Americas",
                        "Press <return> to continue"], timeout=8.0)
        if "STEP 2" in out or "outskirts" in out or "Americas" in out:
            return True
        while "Press <return> to continue" in m.buf[-300:]:
            m.sock.sendall(b"\n")
            m._drain(1.2)
    return False


def pick_skills_from(m, name, cats, done_marker, max_picks, already):
    """Pick skills by browsing categories until done_marker appears."""
    picked = []
    ci = 0
    for _ in range(max_picks + 10):
        cat = cats[ci % len(cats)]
        ci += 1
        m.send(f"skills {cat}")
        listing = m.expect(["Skills in category", "cannot pick", "No skills found",
                            "only available"], timeout=6.0)
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
            if key and key not in picked and key not in already:
                chosen = key
                break
        if not chosen:
            continue
        m.send(f"pick skill {chosen}")
        res = m.expect(["Skill added", "already have", "not a recognized",
                        "not in your allowed"], timeout=6.0)
        if "Skill added" in res:
            picked.append(chosen)
            if done_marker in res:
                break
    return picked


def full_chargen(name, occ, cats, want_elect, want_sec,
                 mid_chargen_score=False, alias_check=None,
                 occ_rerolls=4, race="human"):
    """Create a character end to end. Returns (mud, notes dict)."""
    m, out = login_new(name)
    info(f"[{name}] post-login", out[-150:])

    ok(f"[{name}] region set", enter_region(m, name), m.buf[-200:])

    m.send("pick roll")
    out = m.expect(["STEP 3", "race", "IQ:"], timeout=8.0)
    ok(f"[{name}] stats rolled", "IQ:" in out or "STEP 3" in out, out[-150:])

    if mid_chargen_score:
        # Regression for the daemon/rifts.c lower_case(0) crash: score with
        # no OCC set used to blow up in show_wealth -> is_smuggler_occ.
        m.send("score")
        out = m.expect(["IQ", "Credits", "credits", "HP", "Error"], timeout=6.0)
        ok(f"[{name}] mid-chargen score (no OCC) returns cleanly",
           bool(out.strip()) and "Error" not in out, out[:200])

    m.send(f"pick {race}")
    out = m.expect(["STEP 4", "alignment", "Alignment"], timeout=8.0)
    ok(f"[{name}] race picked ({race})", "alignment" in out.lower(), out[-150:])

    m.send("alignment 4")
    out = m.expect(["STEP 5", "OCC", "eligible"], timeout=8.0)

    # OCC pick, with rerolls if attribute requirements are not met.
    chosen_ok = False
    for attempt in range(occ_rerolls + 1):
        m.send(f"pick {occ}")
        out = m.expect(["You have chosen", "not a valid OCC", "do not meet",
                        "requirement"], timeout=8.0)
        if "You have chosen" in out:
            chosen_ok = True
            break
        if "do not meet" in out or "requirement" in out:
            info(f"[{name}] OCC '{occ}' attr reqs not met, rerolling", out[-120:])
            m.send("pick reroll")
            m.expect(["IQ:", "rolls", "OCC"], timeout=8.0)
            continue
        break
    if not chosen_ok and race == "titan":
        # dice-luck miss on a retryable character; caller retries fresh
        info(f"[{name}] OCC '{occ}' unreachable this roll set", out[-150:])
        return m, False
    ok(f"[{name}] OCC pick '{occ}'", chosen_ok, out[-250:])
    if not chosen_ok:
        return m, False

    if alias_check:
        cat, expects, label = alias_check
        m.send(f"skills {cat}")
        listing = m.expect(["Skills in category", "cannot pick",
                            "No skills found"], timeout=6.0)
        found = [e for e in expects if e in listing.lower()]
        ok(f"[{name}] alias fix: 'skills {cat}' lists {label}",
           len(found) > 0, f"found {found or 'none'} in listing")

    picked = pick_skills_from(m, name, cats, "All elective skills chosen",
                              want_elect, [])
    ok(f"[{name}] elective skills", len(picked) >= want_elect,
       f"picked {len(picked)}: {picked}")

    sec_cats = ["technical", "physical", "medical", "espionage", "military",
                "pilot", "weapons", "lore", "survival", "domestic"]
    sec = pick_skills_from(m, name, sec_cats, "All secondary skills chosen",
                           want_sec, picked)
    ok(f"[{name}] secondary skills", len(sec) >= want_sec,
       f"picked {len(sec)}: {sec}")

    # World arrival is synchronous with the final secondary pick
    # (finish_creation teleports immediately). If we are still in the
    # void, take one more pass at secondary picks before giving up.
    out = m.expect(["Creation is complete", "Rift tears open", "Welcome,"],
                   timeout=8.0)
    if "Creation is complete" not in m.buf and "Rift tears open" not in m.buf:
        extra = pick_skills_from(m, name, sec_cats,
                                 "All secondary skills chosen", 4, picked + sec)
        sec += extra
        out = m.expect(["Creation is complete", "Rift tears open",
                        "Welcome,"], timeout=8.0)
    m.send("look")
    out = m.expect(["survivor", "exits"], timeout=6.0)
    ok(f"[{name}] reaches world (Rift arrival room)",
       "survivor" in out or "four exits: north, south, east, west" in out,
       out[-200:])
    return m, True


def walk(m, name, dirs, pause=0.7):
    """Step-verified walk: every successful move prints an exits line;
    retry a step once if it produced no room output."""
    for d in dirs:
        moved = False
        for attempt in range(2):
            m.send(d)
            out = m.expect(["exit", "There are", "There is"], timeout=4.0)
            if "exit" in out or "There are" in out or "There is" in out:
                moved = True
                break
            time.sleep(pause)
        if not moved:
            info(f"[{name}] move '{d}' produced no room output", m.buf[-150:])
    m._drain(1.0)


def test_bank(m, name):
    # rifts_welcome -> bank: west, south, east, east, east(square),
    # south, south, west(alley1), north(bank)
    walk(m, name, ["west", "south", "east", "east", "east",
                   "south", "south", "west", "north"])
    m.send("look")
    out = m.expect(["Credit Exchange", "teller"], timeout=6.0)
    in_bank = "Credit Exchange" in out or "teller" in out
    ok(f"[{name}] reached bank room", in_bank, out[-250:])
    if not in_bank:
        return

    m.send("read sign")
    out = m.expect(["Services", "show id"], timeout=5.0)
    ok(f"[{name}] read sign", "show id" in out, out[:150])

    m.send("show id")
    out = m.expect(["issues you an ATM card", "already have"], timeout=6.0)
    ok(f"[{name}] show id -> ATM card issued", "ATM card" in out, out[:200])

    m.send("balance")
    out = m.expect(["Account balance:"], timeout=5.0)
    mo = re.search(r"Account balance: (\d+) credits", out)
    start_bal = int(mo.group(1)) if mo else None
    ok(f"[{name}] balance works", start_bal is not None, out[:150])
    if start_bal is None:
        return

    m.send("deposit 50")
    out = m.expect(["Deposited 50", "Insufficient"], timeout=5.0)
    ok(f"[{name}] deposit 50", "Deposited 50" in out, out[:150])

    m.send("balance")
    out = m.expect(["Account balance:"], timeout=5.0)
    mo = re.search(r"Account balance: (\d+) credits", out)
    bal2 = int(mo.group(1)) if mo else None
    ok(f"[{name}] balance after deposit = +50",
       bal2 == start_bal + 50, f"{start_bal} -> {bal2}")

    m.send("withdraw 10")
    out = m.expect(["Withdrew 10", "Insufficient"], timeout=5.0)
    ok(f"[{name}] withdraw 10", "Withdrew 10" in out, out[:150])

    m.send("balance")
    out = m.expect(["Account balance:"], timeout=5.0)
    mo = re.search(r"Account balance: (\d+) credits", out)
    bal3 = int(mo.group(1)) if mo else None
    ok(f"[{name}] balance after withdraw = -10",
       bal3 == start_bal + 40, f"{bal2} -> {bal3}")

    m.send("close account")
    out = m.expect(["closes your", "returns"], timeout=5.0)
    ok(f"[{name}] close account", "closes your" in out, out[:150])


def test_combat_psisword(m, name):
    m.send("autododge")
    out = m.expect(["Autododge", "dodge"], timeout=5.0)
    if "OFF" in out:
        m.send("autododge")
        out = m.expect(["Autododge", "dodge"], timeout=5.0)
    ok(f"[{name}] autododge on (APM spend shown)",
       "APM" in out or "ON" in out, out[:200])

    m.send("autoparry")
    out = m.expect(["Autoparry", "parry"], timeout=5.0)
    if "OFF" in out:
        m.send("autoparry")
        out = m.expect(["Autoparry", "parry"], timeout=5.0)
    ok(f"[{name}] autoparry on (parry bonus shown)",
       "parry" in out.lower(), out[:200])

    m.send("psisword")
    out = m.expect(["crackles to life", "Not enough ISP",
                    "Only Cyber-Knights"], timeout=6.0)
    ok(f"[{name}] psi-sword activates (10 ISP, 2d6 MDC)",
       "crackles to life" in out, out[:250])

    # Grunt room is one move north of the chargen start room; it is
    # aggressive so combat starts on arrival.
    m.send("north")
    out = m.expect(["Checkpoint", "grunt"], timeout=6.0)
    ok(f"[{name}] entered test zone", "grunt" in out.lower() or
       "Checkpoint" in out, out[-250:])

    m.send("kill grunt")
    start_len = len(m.buf)
    end = time.time() + 150
    outcome = None
    while time.time() < end:
        m._drain(1.0)
        fresh = m.buf[start_len:]
        low = fresh.lower()
        if "you killed" in low or "you gain" in low or \
           ("grunt" in low and ("dies" in low or "is dead" in low or
                                "corpse" in low)):
            outcome = "won"
            break
        if "you die" in low or "you are dead" in low or "ghost" in low:
            outcome = "died"
            break
    combat_text = m.buf[start_len:]
    saw_hits = bool(re.search(r"(hit|slash|strike|dodge|parr|damage|shoot|"
                              r"blast|miss)", combat_text, re.I))
    ok(f"[{name}] combat rounds resolved", saw_hits,
       combat_text[-300:] if combat_text else "no combat output")
    ok(f"[{name}] combat concluded (killed grunt or died trying)",
       outcome is not None, f"outcome={outcome}")
    info(f"[{name}] combat outcome", f"{outcome}; sample: {combat_text[-400:]}")
    if outcome == "won":
        m.send("score")
        out = m.expect(["ISP", "HP", "IQ"], timeout=5.0)
        info(f"[{name}] post-fight score tail", out[-250:])


def test_radio(m_a, name_a, m_b, name_b):
    for m, name in ((m_a, name_a), (m_b, name_b)):
        m.send("radio on")
        out = m.expect(["Radio on", "already", "Radio is"], timeout=5.0)
        info(f"[{name}] radio on", out[:120])
        m.send("radio tune 5")
        out = m.expect(["frequency 5", "tuned"], timeout=5.0)
        ok(f"[{name}] radio tuned to 5", "5" in out, out[:120])

    marker = "verifyradiocheck"
    m_a.send(f"radio hello {marker}")
    got = m_b.expect([marker], timeout=10.0)
    ok(f"[{name_b}] received radio transmit from {name_a}",
       marker in got and "[Radio 5]" in got, got[-200:])


def main():
    runtime_start = os.path.getsize(RUNTIME_LOG) if os.path.isfile(RUNTIME_LOG) else 0
    names = ["aethverifya", "aethverifyb", "aethverifyc"]

    # -- Character B: Titan Cyber-Knight (combat + psi-sword + mid-chargen
    #    score). Titan attribute dice (ME 3d6+2, PS 5d6+10, PE 4d6+8) make
    #    the cyber-knight requirements reachable within the 5-roll cap;
    #    retry with a fresh character if the dice still miss. --
    ok_b = False
    for retry in range(3):
        try:
            mb, ok_b = full_chargen(
                "aethverifyb", "cyber-knight",
                ["espionage", "military", "physical", "technical", "weapons"],
                want_elect=5, want_sec=5,
                mid_chargen_score=(retry == 0), race="titan")
            if ok_b:
                test_combat_psisword(mb, "aethverifyb")
            mb.send("quit")
            time.sleep(1.0)
            mb.close()
        except Exception as e:
            results["fail"].append(f"aethverifyb exception: {e}")
            print(f"EXCEPTION aethverifyb: {e}", flush=True)
            break
        if ok_b:
            break
        cleanup("aethverifyb")
        info("[aethverifyb] rolls missed cyber-knight reqs, fresh retry",
             f"attempt {retry + 2}")
    ok("[aethverifyb] titan cyber-knight created", ok_b, "after retries")

    # -- Character A: City Rat (espionage alias check + bank walkthrough) --
    ma = None
    try:
        ma, ok_a = full_chargen(
            "aethverifya", "city rat",
            ["espionage", "technical", "physical"],
            want_elect=6, want_sec=6,
            alias_check=("espionage",
                         ["seduction", "escape artist", "trap detection",
                          "cardsharp", "palming"],
                         "rogue-tagged skills"))
        if ok_a:
            test_bank(ma, "aethverifya")
    except Exception as e:
        results["fail"].append(f"aethverifya exception: {e}")
        print(f"EXCEPTION aethverifya: {e}", flush=True)

    # -- Character C: Rogue Scholar (technical alias check + radio with A) --
    try:
        mc, ok_c = full_chargen(
            "aethverifyc", "rogue scholar",
            ["technical", "medical", "lore"],
            want_elect=6, want_sec=5,
            alias_check=("technical",
                         ["archaeology", "astronomy", "botany",
                          "anthropology", "geology"],
                         "science-tagged skills"))
        if ok_c and ma:
            test_radio(mc, "aethverifyc", ma, "aethverifya")
        mc.send("quit")
        time.sleep(1.0)
        mc.close()
    except Exception as e:
        results["fail"].append(f"aethverifyc exception: {e}")
        print(f"EXCEPTION aethverifyc: {e}", flush=True)

    if ma:
        ma.send("quit")
        time.sleep(1.0)
        ma.close()

    for n in names:
        cleanup(n)

    # Regression net: no lower_case / rifts.c errors logged during the run.
    new_log = ""
    if os.path.isfile(RUNTIME_LOG):
        with open(RUNTIME_LOG, errors="replace") as f:
            f.seek(runtime_start)
            new_log = f.read()
    bad = [ln for ln in new_log.splitlines()
           if "Bad argument" in ln or "/daemon/rifts" in ln or
           "Division by zero" in ln or "Error in loading" in ln]
    ok("no runtime error traces during run", not bad,
       "; ".join(bad[:3]))

    print("\n=== SUMMARY ===", flush=True)
    print(f"Passed: {len(results['pass'])}", flush=True)
    print(f"Failed: {len(results['fail'])}", flush=True)
    for f_ in results["fail"]:
        print(f"  - {f_}", flush=True)
    print(f"Info: {len(results['info'])}", flush=True)
    for i in results["info"]:
        print(f"  - {i}", flush=True)
    return 1 if results["fail"] else 0


if __name__ == "__main__":
    sys.exit(main())
