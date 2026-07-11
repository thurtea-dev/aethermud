# Install AetherMUD on the Fedora Mini PC

Step by step guide for moving AetherMUD from WSL to the Fedora Workstation
mini PC via HDD. Written 2026-07-10. Companion docs in the project root:
INSTALL.md (generic fresh-machine setup), RUNNING.md (daily operations),
SHARING.md (local config details), README.md (overview).

Run commands as your normal user unless sudo is shown.

## The transfer archive

| Item | Value |
|------|-------|
| File | `aethermud-full-20260711.tar.gz` |
| Size | 193.5M compressed |
| Contents | ~18,692 files: full project including `archive/` and `zip-files/` |
| Checksum file | `aethermud-full-20260711.tar.gz.sha256` |
| SHA256 | `96d79eb26e31293c4c0e5d385473c245feed323f82b118051662bc4a6fded083` |

The archive includes the live mudlib, the FluffOS 2.9 driver source, all
player saves (`lib/secure/save/users/`), mud.sh, scripts, tools, docs, and
the reference material. Pid files and the live runtime log were excluded.

To recreate the archive later on WSL:

```bash
cd /home/thurtea/aethermud && ./mud.sh stop
cd /home/thurtea
DATE=$(date +%Y%m%d)
tar czf production-aethermud/aethermud-full-$DATE.tar.gz \
  --exclude='aethermud/nightmare3_fluffos_v2/bin/mud.pid' \
  --exclude='aethermud/nightmare3_fluffos_v2/bin/addr.pid' \
  --exclude='aethermud/nightmare3_fluffos_v2/lib/log/runtime' \
  aethermud
cd production-aethermud
sha256sum aethermud-full-$DATE.tar.gz | tee aethermud-full-$DATE.tar.gz.sha256
```

WARNING: never add `--exclude='*.o'` to slim the tarball. LPC player saves
are `.o` files and GNU tar exclude wildcards would strip your characters
out of the backup.

## 1. Copy WSL to HDD

The transfer package lives at `/home/thurtea/production-aethermud/` on
WSL: the tarball, its `.sha256`, this guide, and copies of INSTALL.md,
RUNNING.md, SHARING.md, and README.md. Staff HTML lives in the archive at
`aethermud/www/` (open `index.html` after extract).

From WSL, the HDD mounts under `/mnt/` (adjust the drive letter):

```bash
cp -r /home/thurtea/production-aethermud /mnt/d/
```

Or from Windows Explorer: the folder is at
`\\wsl$\<distro>\home\thurtea\production-aethermud`. Copy the whole
folder so the checksum and docs travel with the archive.

## 2. Copy HDD to Fedora and verify

Plug the HDD into the mini PC. Fedora auto-mounts it under
`/run/media/<user>/<label>/`.

```bash
cp -r /run/media/$USER/*/production-aethermud ~/
cd ~/production-aethermud
sha256sum -c aethermud-full-20260711.tar.gz.sha256
```

It must print `OK`. If not, recopy; do not extract a corrupt archive.

## 3. Install build dependencies

```bash
sudo dnf groupinstall "Development Tools" -y
sudo dnf install gcc gcc-c++ make bison flex openssl-devel pcre-devel \
  libevent-devel jemalloc-devel sqlite-devel libpq-devel \
  mariadb-connector-c-devel telnet -y
```

## 4. Extract

```bash
cd ~
tar xzf ~/production-aethermud/aethermud-full-20260711.tar.gz
cd aethermud
chmod +x mud.sh scripts/*.sh
```

## 5. Check local config

`nightmare3_fluffos_v2/bin/mudos.cfg` hardcodes absolute paths:

```
mudlib directory : /home/thurtea/aethermud/nightmare3_fluffos_v2/lib
binary directory : /home/thurtea/aethermud/nightmare3_fluffos_v2/bin
```

- If your Fedora username is `thurtea` and you extracted into your home
  directory, these are already correct. Do nothing.
- Otherwise, edit those two lines in `mudos.cfg`, or delete the file and
  run `./mud.sh init-config` to regenerate it (init-config never
  overwrites an existing file).

`lib/secure/cfg/groups.cfg` carries over from WSL with your admin name
already in the SECURE group; no change needed.

## 6. Build the FluffOS driver

The tarball contains a driver binary built on WSL. Rebuild it on Fedora
to avoid glibc version surprises:

```bash
cd nightmare3_fluffos_v2/fluffos-2.9-ds2.08
./configure nm3
make
make install
cd ../..
```

`make install` copies `driver`, `addr_server`, and `portbind` into
`nightmare3_fluffos_v2/bin/`.

## 7. Verify, start, connect

```bash
./mud.sh doctor     # fix any [FAIL] lines before starting
./mud.sh start
./mud.sh status
./mud.sh log 50     # watch the boot; preload takes 1-3 minutes
telnet localhost 1122
```

Log in with your existing character; the saves came over in the tarball.
In-game, `colorize on` enables color (requires `TERM ansi`).

Mudlet: new connection, telnet protocol, host `localhost`, port `1122`.

## 8. Firewall for LAN access

```bash
sudo firewall-cmd --permanent --add-port=1122/tcp   # player telnet
sudo firewall-cmd --permanent --add-port=1129/tcp   # websocket
sudo firewall-cmd --reload
```

Port map:

| Service | Port |
|---------|------|
| Player telnet | 1122 |
| Websocket | 1129 |
| addr_server (internal) | 1199 |

Do not open 1199; it is driver-internal.

## 9. Optional: start on boot

```bash
mkdir -p ~/.config/systemd/user
tee ~/.config/systemd/user/aethermud.service << 'EOF'
[Unit]
Description=AetherMUD FluffOS driver
After=network-online.target

[Service]
Type=forking
WorkingDirectory=%h/aethermud
ExecStart=%h/aethermud/mud.sh start
ExecStop=%h/aethermud/mud.sh stop
RemainAfterExit=yes

[Install]
WantedBy=default.target
EOF
systemctl --user daemon-reload
systemctl --user enable --now aethermud
sudo loginctl enable-linger $USER
```

## 10. Reclaim disk space (optional)

- `archive/` (~408M): paused FluffOS-modern porting experiment
- `zip-files/` (~53M): reference mudlib zips

Safe to delete on the mini PC if the tarball on the HDD is kept:

```bash
rm -rf ~/aethermud/archive ~/aethermud/zip-files
```

## 11. Daily operations quick reference

All from the project root (`~/aethermud`). Full detail in RUNNING.md.

```bash
./mud.sh start | stop | restart | status
./mud.sh log [n]
./mud.sh boot
./mud.sh doctor
```

Reload rules:

| Change | Action |
|--------|--------|
| Daemon, command, or domain LPC | in-game `update /path/to/file` or `warmboot` |
| Any file under `/std/` or simul-efun | `./mud.sh restart` |
| `preload.cfg` or a new daemon | `./mud.sh restart` |
| FluffOS C source | rebuild driver, then `./mud.sh restart` |

Staff promotion: `makewiz` (Apprentice + kit) then `setrole` after review.

Key paths:

| What | Where |
|------|-------|
| Player saves | `lib/secure/save/users/<first_letter>/<name>.o` |
| Apprentice submissions | `lib/secure/save/apprentice/` |
| Runtime log | `lib/log/runtime` |
| Warmboot log | `lib/log/adm/warmboot` |
| Compile errors | `lib/log/errors/<dir>` |
| Daemon preload list | `lib/secure/cfg/preload.cfg` |

## 12. Troubleshooting

| Symptom | Fix |
|---------|-----|
| `driver` not found | Run `make install` in the fluffos directory |
| Port 1122 never opens | `./mud.sh log`; look for LPC compile errors |
| Wrong mudlib loaded | Fix paths in `mudos.cfg` (step 5) |
| Connection refused right after start | Preload still running; wait, check `./mud.sh status` |
| `groups.cfg missing` | `./mud.sh init-config` |
| Object mysteriously fails to load | Check `lib/log/errors/<dir>`, not just runtime |
| Can connect locally but not from LAN | Firewall (step 8) |
| Driver builds but crashes at boot | Confirm deps from step 3; clean rebuild |
