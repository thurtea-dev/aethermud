# HOWTO: www/

## Edit a page

1. Pages are hand-written static HTML; open the `.html` file and edit
   directly. Shared styling lives in `css/`, scripts in `js/`. There
   is no build step, framework, or generator.
2. Preview locally by opening the file in a browser
   (`firefox www/index.html`); nothing needs to be running.
3. Prose rules apply to the website too: no em dashes, American
   spelling, commands written the way players type them (spaced, no
   underscores).

## Keep guides in sync with the game

The staff guides duplicate in-game reality on purpose, so every change
to these systems has a website chore attached:

- `chargen.html` mirrors the chargen flow in
  `nightmare3_fluffos_v2/lib/domains/Praxis/setter.c` (plain-string
  answers, no numbered menus). If chargen changes, update this page.
- `qcs.html` and `coding.html` mirror the QCS verbs in
  `/cmds/creator/_qcs.c` and the in-game `help build` topic.
- `admin.html`, `domain.html`, `roleplay.html`, `apprentice.html`
  mirror the role capabilities granted by wiztools and setrole.
- `races.html` / `classes.html` track the daemon counts (62 races; class
  list in `/daemon/occ.c`).

## Publish to the live site

The MUD server does not serve these files. Deployment is manual: after
the repo changes are pulled on the VPS, copy `www/` into the web
root (`public_html`) on the aethermud.com box. The user does this step
by hand; tooling should hand them the commands, not run them.

Include `.htaccess` in that copy. Staff pages listed there require
Apache Basic Auth (see below).

## Staff page login (Basic Auth)

Staff HTML guides (`admin.html`, `apprentice.html`, `coding.html`,
`domain.html`, `roleplay.html`, `qcs.html`, plus `admin-commands.html`
and `staff-toolkit.html`) are gated by Apache Basic Auth via
`.htaccess`. That login is **not** the MUD login: it does not read
player saves or `groups.cfg`. You choose the usernames yourself in a
password file outside the docroot.

For now, use either:

- a shared account named `guest` (or `staff`), or
- each person's MUD character name (thurtea, etc.) as a convenience
  label only - it is still a separate web password, not their MUD
  password.

Temporary shared password (change later): `password`.

Create the password file on the VPS only. Never put `.htpasswd` under
`www/` or commit it (`.gitignore` already ignores it).

```bash
# First user (creates the file). Path must match AuthUserFile in www/.htaccess
htpasswd -c -b /home/thurtea/domains/aethermud.com/.htpasswd guest password

# More users (no -c, or you wipe the file)
htpasswd -b /home/thurtea/domains/aethermud.com/.htpasswd thurtea password
htpasswd -b /home/thurtea/domains/aethermud.com/.htpasswd staff password
```

`-b` puts the password on the command line (fine for this temporary
shared secret; omit `-b` to be prompted instead). After creating the
file, copy updated `www/` (including `.htaccess`) into `public_html`
if you have not already. Apache needs `AllowOverride AuthConfig` (or
`All`) for that directory, or the browser will never ask for a login.

## Add a new page

Copy the `<head>` and nav structure from an existing page (index.html
is canonical), keep the shared css/js references relative, and link
the new page from index.html so it is reachable.
