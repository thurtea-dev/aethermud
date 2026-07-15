Before doing anything, open CLAUDE.md and confirm you're following its rules (especially the log_file literal-string convention and any rules about not introducing new daemons/files without checking existing mechanisms first).
Stop asking me conceptual questions you can answer by reading the code. Specifically:
Grep the codebase for archp(), creatorp(), and wherever position is checked for admin rights, and also grep for every place groups.cfg is actually read (not just written). Tell me definitively: does groups.cfg SECURE/ASSIST membership alone grant in-game admin command access, or is the save file's position field the actual gate, or both? Show me the exact code paths, don't summarize from memory.
Based on that answer, propose the fix as shell script(s) only — no new .c files. Requirements:
Must be idempotent (safe to run on every mud.sh start without side effects if an admin already exists).
Must NOT touch or overwrite an existing valid admin save file (thurtea.o) if one already has correct position/group status.
If editing a save file directly, first prove the exact save file format via cat on an existing admin save file (e.g. thurtea.o) so any edit matches real syntax exactly — no guessing at LPC save format.
check-setup.sh should fail (not warn) on a fresh clone with no real admin path available.
Show me the exact diffs/new script contents before writing anything, and explain in one paragraph why this satisfies the "someone else clones the repo and it just works" requirement.
Do not create admin_bootstrap.c or any other new LPC file. This is a deploy/bootstrap gap, not a missing game mechanic.