# AetherMUD C++ LPC Driver vs FluffOS

**Our driver:** `/home/thurtea/mudstack/aethermud-repo/driver` (feature inventory from `docs/STATUS.md`, not guessed)  
**Reference:** FluffOS 2.9-ds2.08 (local tree at `nightmare3_fluffos_v2/fluffos-2.9-ds2.08`), plus modern FluffOS upstream docs (github.com/fluffos/fluffos) where noted. FluffOS is the actively maintained MudOS fork; this mudlib targets the 2.9 Discworld-line build.

---

## 1. Language / architecture overview

### FluffOS

| Layer | Design |
|---|---|
| Implementation | Historically C; modern upstream is C++. Local 2.9 tree is the classic C MudOS-line driver. |
| Compile path | LPC → lexer (`lex.c`) + yacc grammar (`grammar.y`) → intermediate tree → bytecode (`icode.c` / `generate`) → in-memory `program_t`. Optional LPC→C / binary packages exist in some builds. |
| VM | Stack-based bytecode interpreter (`interpret.c`). Dedicated opcodes for efuns, operators, `call_other`, catch frames, function pointers. |
| Object model | Blueprint + clones; objects hold a `program_t` plus variable space. Inheritance is recorded in the program’s inherit table; function lookup walks the inherit chain. Destructed objects are flagged (`O_DESTRUCTED`) and rejected on apply. |
| Runtime model | Single-threaded event loop (`backend.c`): network I/O → user commands → `heart_beat` → `call_out`. Eval-cost / tick limits abort runaway LPC. No OS threads for LPC; “concurrency” is cooperative via the backend loop. |
| Extensibility | Core efuns in `func_spec.c` (~180 names in this build) plus optional `packages/` (~120 more → ~300 total with packages). Simul_efun object shadows efuns. Master applies configure security, connect, virtual objects, etc. |

### Our driver

| Layer | Design |
|---|---|
| Implementation | Modern C++20, CMake, unit-tested (212 cases per STATUS). |
| Compile path | Hand-written lexer/parser → AST → stack bytecode (`CodeGen`) → `CompiledProgram`. Preprocess via system `cpp` with FluffOS-compatible predefined macros. |
| VM | Stack-based VM (`VM::run`), `Value` variant (int/float/string/array/mapping/object/closure). Catch frames via `PushCatchFrame`/`PopCatchFrame`. |
| Object model | `LpcObject` + `ObjectManager` cache; inherit flattens object-variable layout and keeps `inheritedPrograms` for `::` / `CallParent`. Destruct is incomplete vs FluffOS (no full `O_DESTRUCTED` apply guards). |
| Runtime model | Non-blocking TCP poll loop + `Scheduler` (50 ms sleep). `tickHeartbeats()` / `tickCallOuts()` exist but are empty stubs. Fixed ~1e6 instruction eval ceiling per `VM::run()` call. |
| Extensibility | ~58 registered efuns (including aliases). Real simul_efun object load + four-tier call resolution. Master applies used live: `connect`, `compile_object`, etc. |

**Intentional relationship:** Our driver is built to run *this* FluffOS mudlib (Nightmare III / AetherMUD). Architecture choices are repeatedly checked against FluffOS 2.9 source (`grammar.y`, `interpret.c`, `function.c`, `simulate.c`, `backend.c`, `comm.c`), not invented in isolation.

---

## 2. Feature-by-feature comparison

Our column is taken from `docs/STATUS.md` “Working now” / “Known stubs” only.

| Feature | Our driver (STATUS.md) | FluffOS 2.9 / modern |
|---|---|---|
| **Closures / function pointers** | Bare-name form `(: name, bound_args... :)` only; `Closure` value; `evaluate`/`funcall`; lazy name resolve at invoke. Object-bound / string-constant / `(*fp)(...)` / `$1` lambdas **not** implemented (STATUS stubs / next blocker). | Full `function` type: bare-name, bound args, inline functionals (`FP_FUNCTIONAL`), string-constant form, `(*fp)(args)`, `$n`/`$(var)` placeholders; bind type baked at construction (`FP_LOCAL`/`FP_SIMUL`/`FP_EFUN`/…). |
| **catch / throw** | `catch(expr)` as real VM control flow (`PushCatchFrame`/`PopCatchFrame`); success → `0`, error → message string; eval-cost errors not catchable. **`throw()` not implemented.** | `catch` + `F_CATCH`/`F_END_CATCH`; `throw()` efun (`f_throw` / `throw_error`). Block form `catch { }` also supported in grammar. |
| **Inheritance depth** | Single- and multi-level `inherit "path";`, cycle detection, flattened variables, `::name` / `qualifier::name` via `CallParent`. No hard depth constant called out in STATUS. | Multi-level inherit with program inherit tables; config `inherit chain size` (this mud’s `mudos.cfg`: **30**). `deep_inherit_list` / `shallow_inherit_list` efuns. |
| **Arrays / mappings** | Literals (trailing comma ok), indexing R/W, range + `<N` from-end, concat; `&` intersection (order/duplicates differ from FluffOS); `\|` int-only (no array union). | Full arrays/mappings/buffers/classes; sorted de-duped array `&`; array `\|` union; rich efun surface (`filter`, `map`, `sort_array`, …). |
| **sscanf** | Literal, `%s`, `%d`, `%%`, `%*` skip; `%f`/`%x`/`%(regexp)` throw. Adjacent `%s%…` without literal text **not** implemented. | Full sscanf including floats, hex, regexp captures, adjacent `%s` forms, etc. |
| **simul_efuns** | Config `simul_efun_file` loaded at boot; tier-3 in call chain; `efun::name` bypass. | Same model: dedicated simul_efun object; lookup prefers simul over core efun; `efun::` escape. |
| **heart_beat** | Apply name recognized in `ApplyTable`; `Scheduler::tickHeartbeats()` **empty stub**; never called. | Full: `set_heart_beat()`, backend `call_heart_beat()`, mud-time-relative beats (FluffOS changelog). |
| **call_out scheduling** | Efun validates args (string or closure), returns handle; **`tickCallOuts()` stub — does not schedule/fire.** | Full scheduler in `call_out.c`: delay queue, handles, remove/find, fires from backend; accepts string or function. |
| **Efun count** | **~58** registered names (aliases counted once each: e.g. `new`/`clone_object`, `evaluate`/`funcall`). | Local 2.9: ~**180** core + ~**120** package ≈ **~300**; modern FluffOS docs cite **600+** with packages/DB/crypto/etc. |
| **Threading / coroutines** | Single-threaded poll loop; no LPC coroutines. | Single-threaded backend; no LPC threads/coroutines. Optional async package for non-LPC work. |
| **Master / connect / input_to** | Master load, `connect()`, `logon()`, `input_to()`, `process_input` fallback — live end-to-end. | Same protocol (`backend`/`comm`); plus telnet negotiation, MXP, etc. |
| **Virtual objects** | `master()->compile_object()` wired into `loadObject()` when `.c` missing. | `int_load_object` + `load_virtual_object`; also clone-of-virtual paths. |
| **save/restore** | Own recursive serialization format (not FluffOS on-disk text). | Classic space/tab LPC-literal `.o` format (and related variants). |
| **Networking** | Plain TCP; no telnet echo suppression yet (`I_NOECHO` flags accepted then discarded). | Telnet IAC, optional TLS/WebSocket (modern), interactive flags, etc. |

---

## 3. Notable gaps (ours relative to FluffOS)

Priority-shaped for *this* mudlib, not a full FluffOS checklist:

1. **Closure surface area** — general lambda / bare string-constant / `(*fp)(args)` still missing; STATUS’s next compile blocker for `std/user/editor.c`.
2. **Real `call_out` + `heart_beat`** — efun/API present, scheduler stubs empty; idle timeouts and most daemons will hang without this.
3. **`throw()`** — catch without throw blocks mudlib patterns that rethrow or signal via `throw`.
4. **Efun breadth** — missing hundreds of efuns (`add_action`, living/inventory, shadows, sockets package, regexp-rich string ops, full `sprintf`/`sscanf`, `map`/`filter`/`sort_array` as closure consumers, etc.).
5. **Object lifecycle fidelity** — weak destruct / once-interactive / living tables; `message()` only hits current connection.
6. **Save-file compatibility** — cannot restore historical FluffOS `.o` files.
7. **Telnet / interactive polish** — echo suppression, terminal_colour, MXP, etc.
8. **Partial operator parity** — array `|`, exact `&` semantics, `arr[i]++`, full `replace_string` bounds, eval-limit accumulation across nested calls.

---

## 4. Deliberate design differences

| Choice | Ours | FluffOS | Match / diverge |
|---|---|---|---|
| **Call resolution** | Explicit four-tier chain at runtime: local → inherited → simul_efun → core efun; `efun::` bypass. | Same conceptual order (local/inherit apply vs simul vs efun table); heavily optimized with `apply_low` caches and compile-time function indices where possible. | **Matches** intent; ours re-walks tiers more naively. |
| **Closure name binding** | Lazy re-resolve bare name at invoke via the same tiered lookup. | Bakes `FP_*` kind + index at construction. | **Diverges** deliberately for bring-up simplicity; STATUS notes observably identical for current boot/login path. |
| **Object-bound `(: obj, "name" :)`** | Implements FluffOS’s *actual* comma-expression lambda semantics (return last value), not the mudlib authors’ “call method” assumption. | Same: general functional executes comma expr; does **not** auto-`call_other`. | **Matches** FluffOS source; diverges from mudlib intent (documented STATUS finding). |
| **Save format** | Custom recursive tagged format. | Historic LPC-literal text. | **Diverges** on purpose for nested Value kinds; tradeoff is incompatibility with shipped saves. |
| **Eval cost** | Fixed per-`run()` ceiling; `set_eval_limit` no-op. | Configurable / signal-based cost across nested work. | **Diverges** (simpler guard). |
| **C++ / clean modular layout** | Separate `compiler`/`vm`/`object`/`efun`/`net`/`scheduler` libs. | Monolithic classic C tree (modern FluffOS more modular). | Structural diverge; semantic target remains FluffOS. |

---

## 5. Performance / architecture tradeoffs if we keep building

- **Apply cache:** FluffOS’s `apply_low` hash cache is a major hot path. Continuing without an equivalent will hurt once `call_other` volume rises (combat, heartbeats, channels).
- **Closure bake-vs-lazy:** Lazy resolve is fine for rare `unguarded((: efun :))` paths; once closures are stored and invoked in tight loops (`filter`/`map` over `users()`), baking FP kinds like FluffOS will matter.
- **Scheduler correctness before micro-opts:** Filling `tickCallOuts`/`tickHeartbeats` with FluffOS-like mud-time advancing and eval-cost budgeting is more important than raw VM speed for playability.
- **Memory model:** FluffOS uses careful refcounts and swap; ours uses `shared_ptr`/`Value` copies. Simpler and safer for bring-up; under large inventories/mappings, copy pressure and lack of buffer/class types will show.
- **Staying FluffOS-compatible** remains the highest-leverage strategy: every STATUS slice that consulted `grammar.y`/`interpret.c` avoided silent mudlib divergence. Chasing DGD-style persistence or Amylaar lambdas would be a different product.

---

## Sources

- `docs/STATUS.md` (our feature inventory)
- Local FluffOS 2.9-ds2.08 (`func_spec.c`, `packages/*_spec.c`, `backend.c`, `call_out.c`, `interpret.c`, `function.c`, mudos.cfg)
- https://github.com/fluffos/fluffos (modern architecture summary)
