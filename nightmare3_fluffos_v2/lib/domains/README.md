# lib/domains/

The game world: rooms, NPCs, and zone-specific content, one directory
per zone (domain).

- `Praxis/` - the main Rifts game world and by far the largest domain:
  the town of Praxis, chargen (`setter.c`), equipment
  (`equipment/` with power_armor, cybernetics, vehicles, grenades,
  magic_items), monsters (`obj/mon/`), vehicle interiors (`vehicles/`),
  and the Splynn/Atlantis/Alvurron areas under `areas/splynn/`.
- `chitown/` - Coalition capital Chi-Town.
- `tolkeen/`, `Horton/`, `NGR/`, `newcamelot/`, `LoneStar/`,
  `PuertoAngel/`, `Lazlo/`, `demon_plane/` - other zones in varying
  states of depth (see `docs/ASSESSMENT.md` and the gap report).
- `adm/` - admin domain: wiz tools (`wiz_tools/`), test areas.
- `wizards/` - staff hallway and shared wizard spaces (personal
  workrooms live in `/realms/<name>/`).
- `Examples/` - legacy Nightmare III sample objects (reference only).
- `Praxis/standardOld/` - legacy NM3 rooms, still live and erroring on
  reset; untouched by design so far (see docs/ASSESSMENT.md).

See HOWTO.md for building rooms/NPCs here and WHY.md for how the zones
came to be shaped this way.
