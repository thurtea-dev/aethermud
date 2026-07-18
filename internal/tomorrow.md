Before doing anything else, read CLAUDE.md and confirm you are following all rules stated there.

Two tasks. Show diffs before applying. Apply only after showing.

Task 1 — MacGuyver: fix persisted Godling save.
MacGuyver was demoted but their save file still has Godling race and rifts_welcome as start room. Read staff_of_demotion.c — the new offline restore path should have fixed this but the save predates it. Run offline_demote("macguyver") via the daemon directly, or add a one-time admin command fixdemote <name> that calls apply_demote_restore() on any named offline player and writes their save. Whichever approach is cleaner — show it before applying.

Task 2 — Race/OCC starting equipment audit.
Read domains/Praxis/setter.c and the race/OCC definition files. For every race and OCC that has a give_starting_eq() call or equivalent, verify the items granted match the Rifts tabletop starting equipment. Flag any race or OCC where starting gear is missing, wrong, or empty. Do not fix all of them this session — produce a prioritized list of what is wrong, fix the top 5 most-played races/OCCs (human, dog boy, juicer, ley line walker, cyber-knight), and leave the rest documented in docs/starting-equipment-audit.md for future passes.

End with session-end report per CLAUDE.md.