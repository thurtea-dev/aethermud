Context: I cleaned up /home/thurtea/aethermud. It's now the single 
source of truth for AetherMUD (live driver runs from here). The old 
archive/ directory (which held a submodule pointer, fluffos-modern) 
is gone. Current top-level contents are:

nightmare3_fluffos_v2, scripts, staff-handbook, tools, www, zip-files,
.gitignore, aethermud.code-workspace, CLAUDE.md, DEVELOPMENT.md,
information.md, install-aethermud.md, INSTALL.md, mud.sh,
playtest-checklist.md, playtest-roster.md, PUBLISHING.md, README.md,
reference-chart.md, report.md, RUNNING.md, SHARING.md, tomorrow.md,
TRANSFER-CHECKLIST.md, zone-expansion-plan.md

I want to start a brand new git repository for this directory, 
separate from the old thurtea-dev/aethermud remote (which had the 
stale submodule and other cruft). Steps:

1. Check current git state of /home/thurtea/aethermud — is it still 
   pointing at the old remote? Show me the current .git/config remote 
   URL and confirm whether removing archive/ left anything broken 
   (dangling submodule references, .gitmodules, etc).

2. Tell me the cleanest way to detach this from the old remote and 
   set up as a fresh repo (new git history vs. keeping current history 
   but repointing origin — give me both options with tradeoffs, don't 
   just pick one).

3. Do NOT push anywhere yet or create the new GitHub remote yourself — 
   I'll create the empty repo on GitHub first and give you the URL.

Once that's sorted, pick up where we left off on actual development:

4. Verify the two untested pieces from the Splynn Preserves loop 
   (committed as a36ab02 before this cleanup, should still be intact): 
   preserve_hollow (dip south from bonefield) and the loot cache at 
   preserve_snag (search cache / get alien hide). Confirm both work.

5. I noticed splynn_forest_edge now has a shovel and a description of 
   soft/hollow disturbed ground that flexes underfoot. Tell me plainly: 
   was that already in the room before this session's work, or did you 
   add it? If you added it, what was it hinting at — is it a loose 
   thread I should follow up on, or should I remove it since it wasn't 
   part of the approved room map?

6. Give me an updated status on the Splynn Preserves "+30 rooms" 
   Sprint 4 target from zone-expansion-plan.md: how many rooms exist 
   now, how many remain, and what's the next well-scoped increment — 
   extend this same loop further, or open a second branch off a 
   different hook room.

Report back on all six points before doing any further building.
Running "warmboot" just crashed with:

*Bad argument 1 to lower_case()
Expected: string Got: 0.
Trace written to /log/catch

Do this now, don't skip straight to a fix:

1. Read /log/catch — get the full trace, not just the error line, so 
   we know the exact call stack and which object/function triggered it.

2. Grep for every lower_case( call across the codebase, but prioritize 
   anything touched this session: staff_of_dominion.c 
   (resolve_domain(), valid_domain_name(), get_domain(), do_grant(), 
   do_revoke()) and anything in the warmboot/reload path itself 
   (daemon reload code, whatever "ensuring dirs, reloading 
   daemons/cmds/obj/adm tools" actually calls).

3. Identify exactly which lower_case(X) call received 0 instead of a 
   string — this means some variable that should hold a domain name 
   or similar was never set, is uninitialized, or a lookup failed and 
   returned 0 instead of a string.

4. Tell me: is this a new bug introduced by tonight's Staff of Domains 
   changes (staff_of_dominion.c edits, commit a36ab02 or the earlier 
   d870a20), or does it predate tonight's work? Check whether 
   resolve_domain() or valid_domain_name() get called anywhere during 
   warmboot/reload — e.g. if some daemon iterates all domains and 
   calls resolve_domain() with a bad or missing name.

5. Propose the minimal fix (likely a null/type check before calling 
   lower_case(), or fixing whatever produced the 0 in the first place) 
   and wait for my approval before applying it.

6. Confirm the live server is currently in a stable state — did the 
   warmboot fully fail, partially apply, or did the mud recover? Tell 
   me if players are currently affected and if I need to do anything 
   right now versus tomorrow.