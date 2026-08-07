# scripts/

Shell tooling for setting up and verifying a local AetherMUD checkout.
These are one-time or occasional setup helpers, not something the running
game calls.

- `init-local-config.sh` - creates `nightmare3_fluffos_v2/bin/mudos.cfg` and
  `nightmare3_fluffos_v2/lib/secure/cfg/groups.cfg` from their `.example`
  templates. Run this once after cloning, before the first `./mud.sh start`.
- `check-setup.sh` - verifies a checkout is ready to build/run: config files
  present and filled in, driver binaries built, directories writable. Run it
  any time via `./mud.sh doctor`.

Both scripts are safe to re-run - they only create files that don't already
exist and never overwrite something you've already configured.
