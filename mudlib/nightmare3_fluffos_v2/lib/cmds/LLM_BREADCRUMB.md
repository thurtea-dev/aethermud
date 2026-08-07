# LLM breadcrumb - lib/cmds/

1. Read `/CLAUDE.md` at the repo root first. It is mandatory and
   overrides default behavior.
2. Read `README.md` in this directory for what each subdirectory holds.
3. Check the parent directory's `LLM_BREADCRUMB.md`
   (`nightmare3_fluffos_v2/lib/`) if present, and the repo root's.
4. Related docs: `HOWTO.md` and `WHY.md` here; `docs/DEVELOPMENT.md`
   for the contributor workflow; `www/coding.html` for the staff guide.

## Gotchas specific to this directory

- Command files are update-safe: `update /cmds/mortal/_foo.c` in-game
  reloads them. But a brand NEW file is not visible until the command
  daemon rehashes its path (a full reboot always does this).
- The privileged twin of this tree is `/secure/cmds/` - files there
  require a FULL reboot after editing (see CLAUDE.md). Check which tree
  a command actually lives in before assuming update is enough; some
  commands (tell, reply) live under `/secure/cmds/mortal/`.
- C89 rules apply: all variables declared at the top of the function,
  `/* */` comments only, functions declared before use (prototype at
  top of file if needed).
- `help()` in a command file is the fallback; the real help system
  reads extensionless topic files under `/doc/help/user` (mortals),
  `/doc/help/creator`, and `/doc/help/wiz`.
