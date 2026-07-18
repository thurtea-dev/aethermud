Here are two prompts. I've split them by risk: Prompt 1 is pure text/doc changes (zero gameplay risk), Prompt 2 is the three that touch live LPC code.

***

**Prompt 1 — send first (low risk, no LPC):**

> Before doing anything else, read CLAUDE.md and confirm you are following all rules stated there.
>
> Three cleanup tasks, all report-then-apply in one pass. Nothing here touches LPC combat, stats, or player data.
>
> **Task 1 — `start_ambient()` ruling: strip from all 4 rooms.**
> Find the four files confirmed last session: `domains/Praxis/areas/rockys_bar.c`, `domains/Praxis/areas/splynn/rockys_bar_splynn.c`, `domains/chitown/areas/chitown_burbs.c`, `domains/newcamelot/areas/black_forest.c`. In each, remove the `start_ambient()` call from `reset()` (and any `#include` or `inherit` line that becomes dead after removal). Do not touch anything else in those files. Confirm whether `std/rifts_ambient.c` is still referenced anywhere else in the tree after the removals; if it is orphaned entirely, note it but do not delete it.
>
> **Task 2 — Stale `sirname` help file.**
> Find `docs/help/user/sirname` (or wherever it actually lives — confirm the path first). Rewrite it as a short redirect notice: the `sirname` command has been removed; use `rp_title` instead. Match the formatting convention of adjacent help files. If `rp_title` has its own help file, confirm its path and cross-reference it. Do not rewrite the `rp_title` help file itself.
>
> **Task 3 — `setter.c` skip-OCC chargen not setting `rifts_occ_flags`.**
> Find `setter.c` (confirm path). Locate the skip-OCC chargen path(s) that set `rifts_occ` but omit `rifts_occ_flags`. Add the same default `rifts_occ_flags` initialisation that `login.c`'s bootstrap sets (confirmed working from the earlier null-guard sweep). Belt-and-braces fix only — do not change any other setter logic.
>
> Show diffs for all three tasks before applying. Apply only after showing diffs, then commit with a descriptive message. Do not push. End with the full session-end report format required by CLAUDE.md.

***

**Prompt 2 — send after Prompt 1 is confirmed and committed:**

> Before doing anything else, read CLAUDE.md and confirm you are following all rules stated there.
>
> Three gameplay fixes. Investigate first, show diffs, apply only after showing diffs.
>
> **Task 1 — Splynn e-clip economy: scope and build.**
> Find `rocky_barkeep.c` and any existing Splynn shop/vendor NPCs under `domains/Praxis/areas/splynn/`. Determine: (a) whether any player-facing e-clip purchase or recharge exists anywhere in the tree (grep for `e-clip`, `eclip`, `e_clip`, `recharge` in domains/); (b) what rifle/ranged weapon items already exist that consume e-clips. Then build the minimum viable e-clip economy: a Splynn vendor NPC that sells e-clips and a recharge point (either a separate object or added to an existing vendor), wired into the Splynn area. Follow existing working vendor patterns (e.g. `alvurron_dealer.c`, `kittani_merchant.c`). All locals declared at top of function per FluffOS C89 rule.
>
> **Task 2 — Camelot flame hilt respawn timer.**
> Find the flame hilt object and its reset/respawn logic (confirm exact file path). The current behaviour is respawn on driver reset (~30-60 real min). The target is once per in-game day (6h40m real time). Search the codebase for any existing timed-respawn or call_out-based daily-respawn pattern to use as a model. If none exists, implement a `call_out` based approach in the item's or its room's reset logic. Show the pattern you're copying or proposing before applying.
>
> **Task 3 — `webclient.html` TLS fallback notice.**
> The live site at `https://aethermud.com/webclient.html` shows a WebSocket timeout because the bridge on port 1129 is plain `ws://` and the browser requires `wss://` on an HTTPS page. This is a known infrastructure gap — do not attempt to fix the bridge or nginx config (that's VPS-side, out of scope). Instead, update `www/webclient.html` so that when the WebSocket `onerror`/`onclose` fires on initial connect, it displays a clear user-facing message in the terminal area: something like "Browser play is temporarily unavailable on the live site. Connect with Mudlet or telnet: aethermud.com port 1122." Replace the generic "Disconnected" state with this specific message. The Reconnect button can stay.
>
> Show diffs for all three tasks before applying. Apply only after showing diffs, then commit with a descriptive message. Do not push. End with the full session-end report format required by CLAUDE.md.