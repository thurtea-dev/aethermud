# Style and voice guide

Standing rules for tone in AetherMUD. Apply to every new or edited
player-facing string, help file, room/item description, NPC line, and
code comment unless an exception below clearly applies.

## Default voice

AetherMUD is a professional, serious post-apocalyptic Rifts MUD. Text
should read like a Palladium RPG sourcebook or a classic early-2000s
MUD, not like Discord chat or a meme channel.

- Prefer plain, direct wording. State the fact or refusal; do not add
  asides, sarcasm, or filler.
- No casual internet slang: nah, yeah, gonna, kinda, lol, omg, ain't,
  dunno, gotta, wanna, shyeah, and similar.
- No conversational filler aimed at the player: "Oops,", "You oaf,",
  trailing "..." used as attitude, excessive exclamation points.
- Code comments stay technical and professional. Same ban on slang;
  explain what and why, not a joke about the code.
- American English in strings and comments (armor, color, defense).
  See CLAUDE.md for the armour.c rename exception.
- No em dashes (U+2014). Use a hyphen, comma, colon, or rephrase.
  See CLAUDE.md rule 16.

## Before / after (real fixes)

These came from live playtest and a codebase slang sweep. Use them as
the concrete bar for new text.

| Bad (do not ship) | Good |
|---|---|
| `Nah... it does not need healing.` | `That does not require curing.` |
| `Yeah, right.` (negative coin amount) | `You cannot give a negative amount.` |
| `You oaf, you can only raise a corpse!!` | `You can only raise a corpse.` |
| help: `those feeling kinda weak` | `a weakened ally` |
| `Shyeah...NOT.` (admin refuse) | `You cannot arrest a SECURE group member.` |

Nearby strings in the same file are the best local guide. Match that
register rather than inventing a new one.

## Legitimate exceptions

Do not "correct" these into corporate-neutral prose without checking
with the project owner first.

1. **Deliberate NPC dialect or character voice.** Example: the Praxis
   sheriff torture-guard lines use phonetic dialect ("gonna show ya",
   "lit'le bit o' mercy"). That is in-character speech, not a system
   error message. If unsure whether a crude or casual line is
   intentional voice vs. a bug, flag it for review instead of
   rewriting the whole NPC.

2. **Quoted player / historical archives.** Files under `lib/doc/classes/`
   and similar class logs are historical transcripts. Leave them as-is.

3. **Vendor / street NPC banter** that is clearly in-character ("Check
   the list. Got fresh stock.") is allowed when it fits the NPC. System
   messages (`notify_fail`, command help, score, hospital service
   lines) are never allowed to use that register.

4. **OOC channels and player-typed emotes** are player content, not
   authored game text. Rules here apply to what the mudlib and builders
   write, not to what players type.

When in doubt: system message = serious; named NPC speaking = may use
character voice; flag borderline cases rather than wholesale rewrite.

## Where this sits relative to other docs

- `CLAUDE.md` - mandatory engineering and gameplay rules; points here
  for tone.
- `docs/DEVELOPMENT.md` - LPC workflow and architecture pointers.
- Per-directory `README.md` / `HOWTO.md` / `WHY.md` /
  `LLM_BREADCRUMB.md` - local purpose and gotchas, not prose style.
