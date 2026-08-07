#!/usr/bin/env python3
"""Create fresh AetherMUD playtest characters via telnet chargen.

Does NOT touch existing accounts. All new names, shared known password.
Requires live MUD on port 1122.
"""
from __future__ import annotations

import os
import re
import socket
import sys
import time

HOST = "127.0.0.1"
PORT = 1122
LIB = "/home/thurtea/aethermud/nightmare3_fluffos_v2/lib"
# Alphabetic only, 2-15 letters (driver rejects digits/punctuation in names).
PASSWORD = "playtestaa"

# Fresh names only - never reuse live admin accounts.
CHARS = [
    # Race eyeball set (checklist section 1)
    {"name": "ptdragona", "race": "great horned dragon", "occ": None,
     "note": "Great Horned Dragon A - RCC only"},
    {"name": "ptdragonb", "race": "great horned dragon", "occ": None,
     "note": "Great Horned Dragon B - RCC only"},
    {"name": "ptvampa", "race": "secondary vampire", "occ": None,
     "note": "Secondary Vampire A - RCC only"},
    {"name": "ptvampb", "race": "secondary vampire", "occ": None,
     "note": "Secondary Vampire B - RCC only"},
    {"name": "ptatlana", "race": "atlantean", "occ": "atlantean nomad",
     "cats": ["physical", "wilderness", "technical", "lore"],
     "elect": 5, "sec": 5,
     "note": "Atlantean A - Atlantean Nomad"},
    {"name": "ptatlanb", "race": "atlantean", "occ": "vagabond",
     "cats": ["physical", "rogue", "wilderness", "technical"],
     "elect": 8, "sec": 8,
     "note": "Atlantean B - Vagabond"},
    {"name": "pttitana", "race": "titan", "occ": "vagabond",
     "cats": ["physical", "rogue", "wilderness", "technical"],
     "elect": 8, "sec": 8,
     "note": "Titan A - Vagabond"},
    {"name": "pttitanb", "race": "titan", "occ": "headhunter",
     "cats": ["espionage", "military", "physical", "pilot", "technical", "weapons"],
     "elect": 6, "sec": 5,
     "note": "Titan B - Headhunter"},
    # Wizard promotion candidates (checklist section 2) - new names
    {"name": "ptdacon", "race": "human", "occ": "city rat",
     "cats": ["rogue", "technical", "physical"],
     "elect": 6, "sec": 6,
     "note": "Wiz candidate Domain-Wiz (promote: makewiz ptdacon; setrole ptdacon domain)"},
    {"name": "ptcoder", "race": "human", "occ": "rogue scholar",
     "cats": ["technical", "medical", "lore"],
     "elect": 6, "sec": 5,
     "note": "Wiz candidate Coding-Wiz (promote: makewiz ptcoder; setrole ptcoder coding)"},
    {"name": "ptrpwiz", "race": "human", "occ": "mystic",
     "cats": ["technical", "lore", "physical"],
     "elect": 6, "sec": 4,
     "note": "Wiz candidate RP-Wiz (promote: makewiz ptrpwiz; setrole ptrpwiz rp)"},
    # Cyber-Knight design verification (human, new reqs PS/PE/MA/ME 10)
    {"name": "ptcknight", "race": "human", "occ": "cyber-knight",
     "cats": ["espionage", "military", "physical", "technical", "weapons"],
     "elect": 5, "sec": 5, "occ_rerolls": 8,
     "note": "Human Cyber-Knight - verifies relaxed attr reqs"},
]

results = []


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


def save_path(name):
    return f"{LIB}/secure/save/users/{name[0]}/{name}.o"


def assert_fresh(name):
    p = save_path(name)
    if os.path.isfile(p):
        raise SystemExit(
            f"REFUSING: {name} already has a save at {p}. "
            "Pick a new name or delete that save first."
        )


def flush_news(m):
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


def login_new(name):
    assert_fresh(name)
    m = Mud()
    m.expect("name do you wish", timeout=5.0)
    m.send(name)
    r = m.expect(["really wish", "choose a password"], timeout=5.0)
    if "really wish" in r:
        m.send("y")
        m.expect("choose a password", timeout=5.0)
    m.send(PASSWORD)
    m.expect("confirm your password", timeout=5.0)
    m.send(PASSWORD)
    m.expect("gender", timeout=5.0)
    m.send("male")
    m.expect("display name", timeout=5.0)
    m.send("")
    m.expect("Email", timeout=5.0)
    m.send("")
    m.expect("real name", timeout=5.0)
    m.send("")
    flush_news(m)
    return m


def enter_region(m):
    """Send 'start americas', re-flushing any late news screens."""
    # Late news boards can still be paging after login; clear them first.
    for _ in range(20):
        if "Press <return> to continue" in m.buf[-400:]:
            m.sock.sendall(b"\n")
            m._drain(1.0)
        else:
            m._drain(0.5)
            if "Press <return> to continue" not in m.buf[-400:]:
                break
    for attempt in range(6):
        m.send("start americas")
        out = m.expect(["STEP 2", "ROLL ATTRIBUTES", "outskirts",
                        "Press <return> to continue"], timeout=8.0)
        if "STEP 2" in out or "ROLL ATTRIBUTES" in out or "outskirts" in out:
            return True
        while "Press <return> to continue" in m.buf[-400:]:
            m.sock.sendall(b"\n")
            m._drain(1.2)
    return False


def pick_skills_from(m, cats, done_marker, max_picks, already):
    picked = []
    ci = 0
    for _ in range(max_picks + 12):
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


def snapshot_score(m):
    m.send("score")
    out = m.expect(["IQ", "Credits", "credits", "HP", "MDC", "Error"], timeout=6.0)
    # Keep a compact slice for the roster doc
    lines = [ln.strip() for ln in out.replace("\r", "").split("\n") if ln.strip()]
    keep = []
    for ln in lines:
        low = ln.lower()
        if any(k in low for k in ("iq:", "me:", "mdc", "sdc", "hp", "race",
                                  "occ", "ppe", "isp", "credits")):
            keep.append(ln)
        if len(keep) >= 12:
            break
    return keep, out


def create_one(spec):
    name = spec["name"]
    race = spec["race"]
    occ = spec.get("occ")
    print(f"\n=== Creating {name} ({race}" + (f" / {occ}" if occ else " / RCC") + ") ===",
          flush=True)
    m = login_new(name)
    if not enter_region(m):
        m.close()
        return False, "region failed", []

    m.send("pick roll")
    out = m.expect(["STEP 3", "race", "IQ:"], timeout=8.0)
    if "IQ:" not in out and "STEP 3" not in out:
        m.close()
        return False, f"roll failed: {out[-120:]}", []

    m.send(f"pick {race}")
    out = m.expect(["STEP 4", "alignment", "Alignment", "not a valid"], timeout=8.0)
    if "alignment" not in out.lower():
        m.close()
        return False, f"race pick failed: {out[-200:]}", []

    m.send("alignment 4")
    out = m.expect(["STEP 5", "OCC", "eligible", "ELECTIVE", "SECONDARY",
                    "Creation is complete", "Rift tears open", "CHOOSE YOUR"],
                   timeout=10.0)

    if occ:
        chosen_ok = False
        rerolls = spec.get("occ_rerolls", 5)
        for attempt in range(rerolls + 1):
            m.send(f"pick {occ}")
            out = m.expect(["You have chosen", "not a valid OCC", "do not meet",
                            "requirement", "ELECTIVE", "SECONDARY"], timeout=8.0)
            if "You have chosen" in out:
                chosen_ok = True
                break
            if "do not meet" in out or "requirement" in out:
                print(f"  attr miss on {occ}, reroll {attempt+1}", flush=True)
                m.send("pick reroll")
                m.expect(["IQ:", "rolls", "OCC"], timeout=8.0)
                continue
            break
        if not chosen_ok:
            m.close()
            return False, f"OCC '{occ}' unreachable: {out[-200:]}", []

        cats = spec.get("cats") or ["physical", "technical"]
        elect = spec.get("elect", 5)
        sec = spec.get("sec", 5)
        picked = pick_skills_from(m, cats, "All elective skills chosen", elect, [])
        sec_cats = ["technical", "physical", "medical", "espionage", "military",
                    "pilot", "weapons", "lore", "survival", "domestic"]
        secondary = pick_skills_from(m, sec_cats, "All secondary skills chosen",
                                     sec, picked)
        out = m.expect(["Creation is complete", "Rift tears open", "Welcome,"],
                       timeout=10.0)
        if "Creation is complete" not in m.buf and "Rift tears open" not in m.buf:
            pick_skills_from(m, sec_cats, "All secondary skills chosen", 4,
                             picked + secondary)
            out = m.expect(["Creation is complete", "Rift tears open", "Welcome,"],
                           timeout=10.0)
    else:
        # RCC-only: elective/secondary counts are 0 -> finish_creation immediately
        if "Creation is complete" not in out and "Rift tears open" not in out:
            out = m.expect(["Creation is complete", "Rift tears open", "Welcome,",
                            "ELECTIVE", "SECONDARY"], timeout=8.0)
            # If somehow stuck in skills with 0 picks, nudge
            if "ELECTIVE" in out or "SECONDARY" in out:
                m.send("score")

    m.send("look")
    world = m.expect(["survivor", "exits", "Rift"], timeout=6.0)
    in_world = ("survivor" in world or "exit" in world.lower()
                or "Rift" in world or "Creation is complete" in m.buf)
    score_lines, _ = snapshot_score(m)
    m.send("quit")
    m._drain(2.0)
    m.close()

    if not in_world and not os.path.isfile(save_path(name)):
        return False, f"did not reach world: {world[-200:]}", score_lines
    if not os.path.isfile(save_path(name)):
        return False, "no save file written", score_lines
    return True, "ok", score_lines


def main():
    # Refuse if any target already exists
    for spec in CHARS:
        assert_fresh(spec["name"])

    print(f"Creating {len(CHARS)} playtest characters on {HOST}:{PORT}")
    print(f"Shared password: {PASSWORD}")
    print("Existing admin accounts are NOT used.\n")

    for spec in CHARS:
        ok, detail, score = create_one(spec)
        results.append({
            "name": spec["name"],
            "ok": ok,
            "detail": detail,
            "note": spec["note"],
            "race": spec["race"],
            "occ": spec.get("occ") or "(racial / none)",
            "score": score,
        })
        print(("PASS" if ok else "FAIL") + f": {spec['name']} - {detail}", flush=True)
        time.sleep(0.5)

    passed = sum(1 for r in results if r["ok"])
    failed = len(results) - passed
    print(f"\n=== DONE: {passed} created, {failed} failed ===")

    roster = os.path.join(os.path.dirname(__file__), "..", "playtest-roster.md")
    roster = os.path.abspath(roster)
    with open(roster, "w") as f:
        f.write("# Playtest roster (created 2026-07-10)\n\n")
        f.write(f"Shared password for all characters below: `{PASSWORD}`\n\n")
        f.write("These are **new** accounts. Do not use thurtea / splynncryth "
                "for this pass.\n\n")
        f.write("| Name | Race | OCC | Purpose | Status |\n")
        f.write("|------|------|-----|---------|--------|\n")
        for r in results:
            status = "READY" if r["ok"] else f"FAIL: {r['detail'][:60]}"
            f.write(f"| {r['name']} | {r['race']} | {r['occ']} | "
                    f"{r['note']} | {status} |\n")
        f.write("\n## Score snapshots (at creation)\n\n")
        for r in results:
            f.write(f"### {r['name']}\n")
            if r["score"]:
                f.write("```\n" + "\n".join(r["score"]) + "\n```\n\n")
            else:
                f.write("_no score capture_\n\n")
        f.write("## Wizard promotion (admin must be online)\n\n")
        f.write("Target must be logged in. From an admin character:\n\n")
        f.write("```\n")
        f.write("makewiz ptdacon\nsetrole ptdacon domain\n")
        f.write("makewiz ptcoder\nsetrole ptcoder coding\n")
        f.write("makewiz ptrpwiz\nsetrole ptrpwiz rp\n")
        f.write("```\n")
    print(f"Roster written to {roster}")
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
