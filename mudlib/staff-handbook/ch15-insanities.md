# Insanities and Addiction

**Addiction is implemented.** See `daemon/addiction_d.c` (intoxication,
withdrawal, combat/skill modifiers from drugs and alcohol).

**Insanities are intentionally out of scope, not an oversight.**
Palladium's insanity system is normally triggered by a failed Horror
Factor save, and Horror Factor itself is not implemented in this
codebase beyond a displayed saving-throw number (`vs. Horror Factor`
in `saving throws`) - no monster carries a Horror Factor stat, and
nothing in combat rolls against it. The related `vs. Insanity` number
shown in the same command is likewise flavor-only: no code path reads
it. Building a full insanity system would mean building the Horror
Factor trigger infrastructure first, which was judged not worth the
scope for this MUD. Decision made 2026-07-28; if a lighter,
staff-triggered-only version is wanted later, ask for it as a new
task rather than assuming this chapter is stale.
