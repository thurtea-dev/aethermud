# LLM breadcrumb

Read, in order: the repo root `CLAUDE.md`, this directory's
`README.md`, then the parent directory's `LLM_BREADCRUMB.md`
(`/domains/wizards/`), then `docs/STYLE.md` before editing any
player-visible strings.

Directory-specific gotchas:

- This wing must stay unreachable by mortals. Both rooms bounce
  non-creator users in `init()`; keep that check if you add rooms,
  and keep `no teleport` in the properties mapping.
- Room spawns follow CLAUDE.md rule 11: clone in `reset()` behind
  `present()` checks, never in `create()`.
- The cosmetic sample items must stay non-protective: no
  `mdc_armor`/`sdc_armor` properties, no `body armour` type, and
  `rifts_slot` set to the cosmetic slot name. `std/armour.c`
  `is_protective_armour()` decides which wear path an item takes.
- `sparring_drone.c` intentionally has NO MDC stats (legacy NM3
  damage path); `rift_crawler.c` intentionally has MDC stats. Do
  not "fix" either to match the other.
