# thurtea_test - Staff equipment/combat testing wing

Private testing wing attached to Thurtea's workroom
(`/realms/thurtea/workroom`, exit `wing`). Staff only: both rooms
eject non-creator players in `init()` and carry `no teleport`, and
the whole `/domains/wizards/` tree is blocked as a mortal login
start by `staff_area_start()` in `std/user.c`.

Contents:

- `test_hall.c` - main hall. Spawns Renn (quartermaster vendor) and
  Sela (dialogue archivist). Exits: `out` to the workroom, `arena`.
- `test_arena.c` - sparring room. Spawns one legacy NM3-stat target
  (`sparring_drone.c`, plain hp pool) and one Rifts MDC target
  (`rift_crawler.c`, 30 MDC) for damage-model testing.
- `quartermaster.c` - `/std/rifts_vendor` shop. Stock covers one
  sample per category: weapon (C-12 rifle), whole-body protective
  armor (Dead Boy, has appearance override), cybernetic implant
  (optic eye), and one cosmetic item per wear slot (head, neck,
  shirt, back, belt, legs, hands, feet, ring). Token prices.
- `archivist.c` - response-set NPC for dialogue testing.
- `field_cap.c`, `canvas_shirt.c`, `utility_belt.c`,
  `canvas_trousers.c`, `work_gloves.c`, `travel_boots.c`,
  `signet_ring.c` - cosmetic slot samples (no protection). Neck and
  back slots reuse `/domains/Praxis/equipment/faction_amulet.c` and
  `backpack.c`.

The playtest steps that use this wing live in
`docs/playtest-checklist.md` at the repo root.
