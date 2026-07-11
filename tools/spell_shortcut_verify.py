#!/usr/bin/env python3
"""Verify the bare spell-name cast shortcut (daemon/chat.c gate fix +
cmds/mortal/_cast.c classic-syntax support).

Creates a human Mystic (starts with windrush, globe of daylight, heal
wounds) via real telnet chargen, then asserts:
  1. bare `windrush`            -> spell fires ("Windrush at whom?"), not "What?"
  2. bare `globe of daylight`   -> multi-word spell name casts
  3. `heal wounds <self>`       -> classic "spell target" form (no "at")
  4. `cast globe of daylight`   -> original cast command still works
  5. gibberish verb             -> still falls through to "What?"
"""
import sys
sys.path.insert(0, "/home/thurtea/aethermud/tools")
import section8_verify4 as v


def main():
    # Fresh name per attempt: a failed attempt leaves a link-dead
    # character mid-chargen that a same-name relogin reconnects to
    # (with all five stat rolls already spent).
    name = None
    m = None
    done = False
    for attempt in ("aethspela", "aethspelb", "aethspelc", "aethspeld"):
        name = attempt
        m, done = v.full_chargen(
            name, "mystic",
            ["technical", "lore", "physical"],
            want_elect=6, want_sec=4,
            occ_rerolls=4)
        if done:
            break
        v.info(f"[{name}] chargen missed mystic reqs, retrying fresh",
               m.buf[-120:])
        m.send("quit")
        m.expect(["Goodbye", "leaves"], timeout=4.0)
        m.close()
        v.cleanup(name)
    if not done:
        v.ok("chargen completed on some attempt", False, "all attempts missed")
        return 1

    # 1. bare spell name, no target: must reach the spell effect, whose
    # own no-target message proves dispatch got past the "What?" parser.
    m.send("windrush")
    out = m.expect(["Windrush at whom", "What?"], timeout=6.0)
    v.ok("bare 'windrush' dispatches to the spell (not 'What?')",
         "Windrush at whom" in out and "What?" not in out, out[-200:])

    # 2. bare multi-word spell name.
    m.send("globe of daylight")
    out = m.expect(["globe", "daylight", "What?", "PPE"], timeout=6.0)
    v.ok("bare 'globe of daylight' casts",
         "What?" not in out and out.strip() != "", out[-200:])

    # 3. classic "spell target" form without 'at', against the test-zone
    # coalition grunt one room north of the Rift arrival room.
    # (Note: spells whose first word is a real command, like "heal ...",
    # are intercepted by that command first - correct precedence.)
    v.walk(m, name, ["north"])
    m.send("windrush grunt")
    out = m.expect(["blast of magical wind", "What?", "don't see"], timeout=6.0)
    v.ok("classic 'windrush grunt' (no 'at') casts at the grunt",
         "blast of magical wind" in out, out[-200:])

    # 3b. "me" self-target keyword through the cast command.
    m.send("cast heal wounds at me")
    out = m.expect(["healing energy", "What?", "Heal whom"], timeout=6.0)
    v.ok("'cast heal wounds at me' self-targets",
         "healing energy" in out, out[-200:])

    # 4. original cast command regression.
    m.send("cast globe of daylight")
    out = m.expect(["globe", "daylight", "What?", "PPE"], timeout=6.0)
    v.ok("'cast globe of daylight' still works",
         "What?" not in out and out.strip() != "", out[-200:])

    # 5. unknown verbs still fall through to the parser error.
    m.send("zzzquux")
    out = m.expect(["What?"], timeout=6.0)
    v.ok("gibberish still returns 'What?'", "What?" in out, out[-120:])

    m.send("quit")
    m.expect(["Goodbye", "leaves the"], timeout=5.0)
    m.close()
    v.cleanup(name)

    print("\n=== SUMMARY ===", flush=True)
    print(f"Passed: {len(v.results['pass'])}", flush=True)
    print(f"Failed: {len(v.results['fail'])}", flush=True)
    for f_ in v.results["fail"]:
        print(f"  - {f_}", flush=True)
    return 1 if v.results["fail"] else 0


if __name__ == "__main__":
    sys.exit(main())
