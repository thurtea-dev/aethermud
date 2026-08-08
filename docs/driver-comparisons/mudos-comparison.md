# AetherMUD C++ LPC Driver vs MudOS (LPMud lineage)

**Our driver:** `/home/thurtea/mudstack/aethermud-repo/driver` (features from `docs/STATUS.md` only)  
**Reference:** MudOS (final public line ~v22.2b14) and its LPMud ancestry. Local FluffOS 2.9 is a MudOS fork with Discworld patches — used here as the readable MudOS-family source. Amylaar/LDMud is a *sibling* LPMud 3.2 line, not MudOS’s parent; noted separately where features diverge.

---

## Lineage note (important)

```
LPMud (Pensjö)
 ├── MudOS (1992…) ──► FluffOS (Discworld fork; our mudlib’s target)
 ├── Amylaar LPMud 3.2 ──► LDMud (Mateese)
 └── DGD (Croes; rewrite, not a code fork)
```

The parenthetical “Amylaar/LPMud descendant” in the research brief mixes two branches. **MudOS** descends from LPMud via the MudOS project (Beek et al.). **Amylaar** maintained LPMud 3.2 and that line became **LDMud**. They share LPC heritage and many efuns, but differ sharply on closures (`(: :)` vs `#'` / `lambda()`), packaging, and some runtime semantics. This report centers on **MudOS**; Amylaar/LDMud appears only for contrast.

MudOS development stalled; FluffOS carries the dialect forward. Comparing “to MudOS” for this codebase is therefore almost the same semantic target as FluffOS 2.9, minus FluffOS-only extras (MXP applies, some efuns, heartbeat timing tweaks, etc.).

---

## 1. Language / architecture overview

### MudOS

| Layer | Design |
|---|---|
| Implementation | C driver (“OS” for the mudlib). Compiles LPC to compact bytecode (“tokenized form”) and interprets it. |
| Compile path | Integrated compiler: lex + yacc grammar → program structures → bytecode. Optional LPC→C / binaries in some eras (BeekOS-style dynamic compilation later folded into MudOS). |
| VM | Stack machine (`interpret.c`): control stack frames, `svalue_t` tagged values, efun opcodes, `apply`/`call_other` for cross-object calls. |
| Object model | Named objects (path-based), clones of blueprints, inherit tables, simul_efun object, master object. Network sockets pioneered here (intermud). |
| Runtime | Single-threaded backend loop: connections → commands → heartbeats → call_outs. Eval cost limits. |
| Function pointers | MudOS introduced the `function` type and `(: … :)` functionals widely used by TMI-style and Nightmare mudlibs — exactly this mudlib’s dialect. |

### Our driver

Same summary as in the FluffOS report: C++20 stack VM, AST→bytecode, FluffOS/MudOS-compatible call tiers and a growing MudOS-dialect subset. Deliberately aimed at Nightmare III / AetherMUD sources written for MudOS/FluffOS, not Amylaar LPC.

---

## 2. Feature-by-feature comparison

| Feature | Our driver (STATUS.md) | MudOS (v22-line / MudOS docs) | Amylaar / LDMud (contrast only) |
|---|---|---|---|
| **Closures / function pointers** | Bare `(: name, args… :)`, general inline lambda, `(*fp)(args)`, and bare string-constant closures all implemented (2026-08-07); `evaluate`/`funcall`; lazy resolve. Only `$n`/`$(var)` placeholder forms remain missing. | Full `function` type; `(: obj, "fn" :)`, `(: "fn" :)`, functionals, `(*f)(args)` → `call_function_pointer`. | Different surface: `#'name`, `lambda()`, symbols, operator closures; inline closures inspired by MudOS but not identical. |
| **catch / throw** | `catch(expr)` only; no `throw()`. | `catch` + `throw` efun; longjmp-style unwind to catch frame. | `catch` / `throw` also present (details differ). |
| **Inheritance** | Multi-level inherit, cycle detect, flattened vars, `::` / `qualifier::`. | Multi-level inherit; configurable inherit chain size; inherit list efuns. | Multi-level; historically deep feature set around replaces/shadows. |
| **Arrays / mappings** | Present with noted `&`/`\|` and range gaps. | First-class arrays & mappings; buffers in later MudOS. | Arrays & mappings; mapping “width” model differs from MudOS in LDMud. |
| **sscanf** | Partial (`%s`/`%d`/`%%`/`%*`). | Full sscanf efun. | Full sscanf. |
| **simul_efuns** | Loaded; tier-3 resolution; `efun::` bypass. | Canonical simul_efun object model (MudOS popularized the pattern for TMI mudlibs). | simul_efuns exist; lookup prefers them similarly. |
| **heart_beat** | Recognized apply; **scheduler stub — never fires**. | `set_heart_beat` + backend heartbeat pass every cycle. | Same general idea. |
| **call_out** | Validates + returns handle; **does not schedule**. | Full delayed-call queue from backend. | Full `call_out` / `remove_call_out`. |
| **Efun count** | ~**91** (up from ~58) | Large core set (same order as FluffOS 2.9 core ~180 before packages); sockets and contrib packages expand further. | Very large (LDMud often cited as biggest LPC feature set). |
| **Threading** | Single-threaded poll loop. | Single-threaded; no LPC threads. | Single-threaded LPC; tick limits. |
| **Sockets / intermud** | Plain game TCP only. | Native socket efuns (MudOS hallmark). | Socket support in LDMud as well (API differs). |
| **LPC→C** | None. | Optional / historical binaries & compilation packages. | Not the same LPC→C story as MudOS/BeekOS. |
| **Master / login path** | `connect` → `logon` → `input_to` / `process_input` live. | Same applies model (`connect`, `logon`, `process_input`, …). | Master/driver split exists; apply names and boot differ by mudlib. |

---

## 3. Notable gaps (ours relative to MudOS)

Because our semantic target *is* MudOS/FluffOS LPC, gaps vs MudOS largely mirror the FluffOS report:

1. **Non-functional timers — now the single largest remaining gap.** Chargen runs completely end to end as of 2026-08-07 (login through a real room, working `look`), which makes this the next thing that actually blocks gameplay: `Scheduler::tickHeartbeats()`/`tickCallOuts()` are still empty function bodies, confirmed by reading `src/scheduler/Scheduler.cpp` directly. MudOS gameplay assumes live `call_out` and `heart_beat`; nothing time-based fires here at all yet.
2. **Missing `throw`** — MudOS error idiom is catch/throw pairs.
3. **`function` type is now essentially complete** for this mudlib's dialect — `(*fp)()`, stored/inline functionals, and string-constant closures are all implemented; only `$n`/`$(var)` placeholder forms remain, unused on any path reached live.
4. **Efun surface has grown (~91, up from ~58) but is still tiny next to MudOS's hundreds** — `add_action`/`enable_commands` is now a real command-dispatch subsystem (not missing), `living()`/`all_inventory()`/`deep_inventory()`/`present()` implemented; still no socket efuns, incomplete string/format efuns (`sprintf` lacks field-width specifiers like `%*`, `sscanf` lacks floats/hex/regexp).
5. **No object swap / binary programs** — MudOS-era memory management and optional compiled binaries absent (acceptable for bring-up).
6. **Destruct / interactive semantics** — MudOS tracks destructed and once-interactive objects carefully; STATUS documents weaker behavior.
7. **Save format** — not MudOS `.o` compatible.

Gaps vs **Amylaar/LDMud** that we should *not* chase unless goals change: `#'` closures, `lambda()` arrays-as-code, LDMud-only types/operators. This mudlib does not speak that dialect.

---

## 4. Deliberate design differences

| Choice | Ours | MudOS | Notes |
|---|---|---|---|
| **Tiered runtime call fallback** | local → inherit → simul → efun | Same conceptual fallback; MudOS resolves many calls to program indices + `apply_low` cache | **Matches** MudOS/FluffOS model; ours is the explicit educational/bring-up form of that chain |
| **Dialect choice** | MudOS/FluffOS `(: :)` LPC | MudOS `function` type | **Matches**; deliberately **not** Amylaar |
| **Lazy closure resolve** | Re-lookup name at `callClosure` | Bind FP kind at construction | **Diverges** (simplify); same for FluffOS comparison |
| **Modern C++ modules** | Clear package boundaries, gtest | Monolithic C sources | Structural diverge only |
| **Save format / telnet** | Simplified | Full historic behavior | Conscious stubs per STATUS |

MudOS’s big architectural bet — **driver as OS, mudlib as userspace, sockets as first-class** — is what our driver is incrementally reimplementing. We have the OS skeleton (compile, VM, objects, master, TCP) but not yet the “kernel services” density (timers, commands, sockets, full efun table) MudOS provided by the mid-1990s.

---

## 5. Performance / architecture tradeoffs

- **Following MudOS/FluffOS semantics maximizes mudlib reuse.** Forking toward Amylaar features would split the dialect and break Nightmare sources for little gain.
- **MudOS’s apply cache and program indices** are the proven answer once heartbeat storms and `call_other` chains dominate CPU. Plan for that before micro-optimizing the C++ VM loop.
- **Socket efuns** were MudOS’s differentiator for intermud; only needed when AetherMUD’s intermud/daemons are in scope — after timers and closures.
- **Optional LPC→C** is a late-game MudOS idea; irrelevant until the interpreter is complete and hotspots are measured.
- **Eval cost + single thread** is the MudOS safety model; keep it. Introducing OS threads for LPC would diverge from every MudOS mudlib assumption.

---

## Sources

- `docs/STATUS.md` (last cross-checked against this doc 2026-08-07 after chargen reached a full end-to-end run)
- Local MudOS-family tree: FluffOS 2.9-ds2.08 (Credits.MudOS, interpret.c, func_spec.c, backend.c)
- MudOS conceptual docs (e.g. “What is the MudOS driver?” / Intermediate LPC ch.2 heart_beat & call_out)
- Historical notes: LPMud FAQ / Mateese comparison (MudOS vs LDMud vs DGD feature posture)
- https://github.com/fluffos/fluffos (MudOS successor)
