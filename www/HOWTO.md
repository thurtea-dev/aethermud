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

## Add a new page

Copy the `<head>` and nav structure from an existing page (index.html
is canonical), keep the shared css/js references relative, and link
the new page from index.html so it is reachable.
