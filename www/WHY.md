# WHY: www/

## Why a static site in the game repo

The website's job is to answer "what is this game and how do I play"
for outsiders and "what does my role let me do" for staff, offline and
without a running MUD. Keeping it in the same repo as the lib means a
gameplay change and its documentation change can land in one commit,
and CLAUDE.md can cite `www/index.html` as the canonical staff-guide
entry point. Plain HTML with no generator was chosen so that editing a
guide requires zero toolchain: any staff member can fix a page.

## Why the guides duplicate in-game help

In-game help (`/doc/help/...`) serves people already connected;
the website serves people deciding whether to connect, and staff
reading on a second screen. The duplication is accepted and managed:
each page names the code it mirrors so staleness is detectable (see
HOWTO.md). When the two disagree, in-game behavior wins and the page
gets fixed.

## Why deployment is manual

The VPS at aethermud.com pulls the repo via git, but the web root copy
is a deliberate manual step. It keeps the public site from silently
changing on every pull, gives a human checkpoint before
player-visible publishing, and matches the project's wider rule that
the user runs production actions themselves.

## Why roadmap.html exists

Public accountability for a fan recreation: it tells returning players
of the original game what is rebuilt, what is stubbed, and what is
next, without them needing repo access. It is the outward-facing
sibling of `docs/ASSESSMENT.md` and the gap report.
