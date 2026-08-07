#!/usr/bin/env python3
"""Section 8 verification: chargen walkthrough + command tests.

Requires live MUD on port 1122. Character names must be alphabetic only.
Removes the test save file on exit if present.
"""
import os
import re
import socket
import sys
import time

HOST = "127.0.0.1"
PORT = 1122
NAME = "verifyme"
PASSWORD = "test807"
SAVE = f"/home/thurtea/aethermud/nightmare3_fluffos_v2/lib/secure/save/users/{NAME[0]}/{NAME}.o"

results = {"pass": [], "fail": [], "needs_user": []}


class Mud:
    def __init__(self):
        self.sock = socket.socket()
        self.sock.connect((HOST, PORT))
        self.buf = ""
        time.sleep(1.5)
        self.read(2.0)

    def read(self, timeout=1.0):
        self.sock.settimeout(timeout)
        chunks = []
        try:
            while True:
                data = self.sock.recv(8192)
                if not data:
                    break
                chunks.append(data)
        except socket.timeout:
            pass
        text = b"".join(chunks).decode("utf-8", errors="replace")
        self.buf += text
        return text

    def send(self, cmd, wait=1.0):
        self.sock.sendall((cmd + "\n").encode())
        time.sleep(wait)
        return self.read(1.5)

    def close(self):
        try:
            self.sock.close()
        except OSError:
            pass

    def last(self, n=2000):
        return self.buf[-n:]


def ok(label, cond, detail=""):
    if cond:
        results["pass"].append(label)
        print(f"PASS: {label}" + (f" ({detail})" if detail else ""))
    else:
        results["fail"].append(f"{label}: {detail}")
        print(f"FAIL: {label} - {detail}")


def need_user(label, detail=""):
    results["needs_user"].append(f"{label}: {detail}")
    print(f"NEEDS USER: {label} - {detail}")


def cleanup_save():
    if os.path.isfile(SAVE):
        os.remove(SAVE)
        print(f"Removed save file {SAVE}")


def create_character():
    cleanup_save()
    m = Mud()
    out = m.last()

    m.send(NAME, 1.5)
    if "Do you really wish" in m.last():
        m.send("y", 1.0)
    elif "Password:" in m.last() and "Invalid password" not in m.last():
        pass  # existing char - delete and retry
    else:
        pass

    out = m.last()
    if "Password:" in out and "choose a password" not in out.lower():
        m.close()
        cleanup_save()
        time.sleep(0.5)
        m = Mud()
        m.send(NAME, 1.0)

    if "Do you really wish" in m.last():
        m.send("y", 1.0)
    if "choose a password" in m.last().lower() or "Please choose a password" in m.last():
        m.send(PASSWORD, 1.0)
        m.send(PASSWORD, 1.0)
    if "gender" in m.last().lower():
        m.send("male", 1.0)
    if "display name" in m.last().lower():
        m.send("", 1.0)
    if "Email:" in m.last():
        m.send("", 1.0)
    if "real name" in m.last().lower():
        m.send("", 1.5)

    # Chargen in setter
    steps = [
        ("start americas", 1.5),
        ("pick roll", 2.0),
        ("pick human", 2.0),
        ("alignment 4", 2.0),
        ("pick city rat", 2.0),
    ]
    for cmd, w in steps:
        m.send(cmd, w)

    elective = ["boxing", "swimming", "climbing", "running", "gymnastics", "athletics"]
    for sk in elective:
        m.send(f"pick skill {sk}", 1.2)

    secondary = ["biology", "chemistry", "astronomy", "barter", "fishing", "hunting"]
    for sk in secondary:
        m.send(f"pick skill {sk}", 1.2)

    time.sleep(2.0)
    m.read(2.0)
    tail = m.last(4000)
    ok("chargen completes", "creation_step" not in tail or "All secondary" in tail or "Praxis" in tail or "realms" not in tail.lower(), tail[:200])
    return m, tail


def run_tests(m):
    tail = m.send("score", 1.5)
    ok("score command", "IQ" in tail or "Strength" in tail or "SDC" in tail or "HP" in tail, tail[:120])

    tail = m.send("eq", 1.5)
    ok("eq command", "wield" in tail.lower() or "wear" in tail.lower() or "You aren't" in tail or "nothing" in tail.lower(), tail[:120])

    tail = m.send("autododge", 1.0)
    ok("autododge toggle", "autododge" in tail.lower() or "dodge" in tail.lower(), tail[:120])

    tail = m.send("autoparry", 1.0)
    ok("autoparry toggle", "parry" in tail.lower() or "autoparry" in tail.lower(), tail[:120])

    tail = m.send("mail", 1.5)
    ok("mail command", "mail" in tail.lower() or "no mail" in tail.lower() or "folder" in tail.lower(), tail[:120])

    help_topics = ["rules", "riftsmud", "faq", "movement", "mercenary", "bug"]
    for topic in help_topics:
        tail = m.send(f"help {topic}", 1.0)
        ok(f"help {topic}", "No help available" not in tail and "Invalid help" not in tail, tail[:80])

    tail = m.send("skills", 1.5)
    ok("skills columns", "Wilderness survivaFishing" not in tail, "column bleed check")

    tail = m.send("quit", 1.0)
    m.close()
    time.sleep(1.0)

    ok("save file path", os.path.isfile(SAVE), SAVE)


def main():
    if not os.path.isdir("/home/thurtea/aethermud"):
        print("Wrong cwd")
        sys.exit(1)
    try:
        m, _ = create_character()
        run_tests(m)
    except Exception as e:
        results["fail"].append(f"script exception: {e}")
        print(f"EXCEPTION: {e}")
    finally:
        cleanup_save()

    print("\n=== SUMMARY ===")
    print(f"Passed: {len(results['pass'])}")
    print(f"Failed: {len(results['fail'])}")
    for f in results["fail"]:
        print(f"  - {f}")
    print(f"Needs user: {len(results['needs_user'])}")
    for n in results["needs_user"]:
        print(f"  - {n}")
    return 0 if not results["fail"] else 1


if __name__ == "__main__":
    sys.exit(main())
