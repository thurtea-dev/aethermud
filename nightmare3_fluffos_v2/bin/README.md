# nightmare3_fluffos_v2/bin/

Build output and runtime configuration for the driver.

- `driver`, `addr_server` - the built binaries (from
  `fluffos-2.9-ds2.08/`). Rebuild via `./configure nm3 && make && make
  install` from within `fluffos-2.9-ds2.08/`.
- `portbind` - a small helper binary used by the driver for low-port
  binding.
- `mudos.cfg` - the actual runtime config the driver reads (local,
  machine-specific, gitignored). Generated from `mudos.cfg.example` by
  `scripts/init-local-config.sh`.
- `mudos.cfg.example` - the tracked template `mudos.cfg` is generated
  from; has `@MUDLIB_DIR@`/`@BINARY_DIR@` placeholders filled in with
  absolute paths at init time.
- `mudos.cfg.win32` - Windows variant of the config template.
