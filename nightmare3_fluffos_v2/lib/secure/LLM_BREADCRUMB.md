# LLM breadcrumb - lib/secure/

1. Read `/CLAUDE.md` at the repo root first; its Security Notes and
   sscanf rules (rule 18) were written about files in this tree.
2. Read `README.md` here for the layout.
3. Check the parent (`nightmare3_fluffos_v2/lib/`) and repo-root
   breadcrumbs for wider context.
4. Related docs: `HOWTO.md` / `WHY.md` here; `docs/SHARING.md` for
   which cfg files are local-only vs tracked.

## Gotchas specific to this directory

- Player saves: `secure/save/users/<first_letter>/<name>.o`. Never the
  old NM3 `/save/players/` path. Never read or copy real player saves
  for testing; make throwaway characters through normal chargen.
- `secure/cmds/` files require a FULL reboot after editing, unlike
  `/cmds/` files.
- `groups.cfg` lines are parsed by master.c's load_access() with a
  two-pass sscanf; a member-less group line like `(AMBASSADOR)` is
  valid BY DESIGN. Do not "fix" the fallback (CLAUDE.md rule 18).
- `preload.cfg` is the boot manifest: new daemons must be added there
  or they silently do not exist at boot.
- Some mortal commands (tell, reply) live in `secure/cmds/mortal/` for
  historical reasons; search both command trees before concluding a
  verb does not exist.
