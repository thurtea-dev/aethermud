# Range/slice indexing feature slice: plan

Status: planning only, no implementation yet.

Scope: add LPC range/slice indexing syntax, `str[start..end]`, as a
genuine expression construct distinct from single-value indexing
(`str[i]`, already implemented). Confirmed as the blocker identified by
the ninth boot attempt:

```
[object] compile error in .../secure/daemon/master.c: lexer: unrecognized character '.' at line 254
```

That "line 254" is not a typo or a stale reference: it is the line
number inside the preprocessed compilation unit the driver actually
feeds to the lexer (`ObjectManager::compile()` runs `cpp -I <includeDir>
-x c <path>` before lexing, then strips any leftover `# ...` line-marker
directives; see `src/object/ObjectManager.cpp`). The raw source file
(what an editor shows) numbers the same statement as line 215. Both
numbers point at the same code; I reproduced the driver's own
preprocess-plus-strip pipeline by hand to confirm the mapping rather
than assuming it, since the two numbers disagree and either could have
been the stale one. From here on this document uses raw `master.c` line
numbers (the ones in the Step 1 table also carry the matching
preprocessed number, since boot logs will keep reporting the
preprocessed one going forward).

## Step 1 findings: re-confirmed sites and their shapes

Re-ran the search rather than trusting the char-literals slice's
numbers unchanged, since the file could in principle have moved between
then and now. It has not: `git status` on `master.c` is clean and the
file's mtime (2026-07-30) predates the ninth boot attempt's log
(2026-08-05), so no edits happened in between. The seven real sites
land at exactly the same raw line numbers the char-literals slice
already reported, confirmed again directly against the current file:

| Raw line | Preprocessed line | Code |
|---|---|---|
| 215 | 254 | `else if(file == DIR_USERS+"/"+__PlayerName[0..0]+"/"+__PlayerName+__SAVE_EXTENSION__)` |
| 317 | 348 | `else if(file_size(DIR_USERS+"/"+nom[0..0]) != -2)` |
| 318 | 349 | `mkdir(DIR_USERS+"/"+nom[0..0]);` |
| 505 | 536 | `str = DIR_USERS+"/"+str[0..0]+"/"+str+__SAVE_EXTENSION__;` |
| 593 | 624 | `if(file_size(DIR_USERS+"/"+str[0..0]) == -2) return;` |
| 595 | 626 | `mkdir(DIR_USERS+"/"+str[0..0]);` |
| 601 | 632 | `string pfile = DIR_USERS+"/"+nom[0..0]+"/"+nom;` |

An eighth occurrence at raw line 608 remains commented out
(`//mkdir(DIR_USERS+"/"+nom[0..0]);`), unreachable by the lexer since
comments are stripped before token dispatch. Not counted among the
seven.

All seven are the exact same shape: `DIR_USERS+"/"+name[0..0]`, a
single-character-prefix sharding idiom, where `name` is always a
`string`-typed local or parameter (`__PlayerName`, `nom`, or `str`
depending on the function) holding a player/account name, never
something already known to be empty at that point in the surrounding
control flow. Every occurrence uses:
- a literal `0` start and a literal `0` end (never a non-zero start,
  never unequal start/end, never a variable or expression on either
  side of `..`)
- both bounds present (never an omitted end like `[0..]`, never an
  omitted start like `[..0]`)

So this is a single, uniform shape across all seven real sites, not a
family of related-but-different range shapes needing separate
handling.

Two false-positive categories worth naming, confirmed while scanning so
they are not mistaken for additional range-indexing gaps:
- `"..."` (a literal ellipsis) appears twice, inside already-quoted
  strings (`"Preloading: "+str+"..."` at line 147, and `"Whoop whoop
  whoop.... BOOM!!!"` at line 494). Both are bytes inside a
  double-quoted string literal, fully consumed by `lexString()` before
  reaching the main dispatch, same reasoning as the char-literals
  slice's `sprintf("'%s' ...)` false positive.
- Found incidentally while re-scanning for this slice, not part of the
  requested `..` search but directly relevant to Step 4 below: three
  occurrences of `?` outside any string or comment, at raw lines 433,
  469, and 519 (`(caught ? "catch" : "runtime")`, `obj ? file_name(obj)
  : "<none>"`, `(__PlayerName ? __PlayerName : "Mudlib")`). These are
  real ternary-conditional expressions, not part of this slice's scope
  and not a range-indexing variant, but they matter for Step 4's
  answer below, so they are flagged here rather than silently dropped.

## Step 2 findings: string-only in this file, but Index's own pattern argues for symmetry

Searched `master.c` and `master.h` for any range-index expression whose
target is a declared array variable (`object *stack`, `string *privs`,
`string *ok`, or any other `type *name` in either file). None found:
every one of the seven real `..` sites indexes a `string`-typed target,
never an array. `master.h` has zero `.` characters at all (already true
for the char-literals slice's search and still true now, so nothing
arrives through the header either).

So the confirmed real need in this codebase is substring-returning
range indexing on `string` targets only. Array range indexing
(sub-array-returning) has zero confirmed sites here. Even so, the VM's
existing `Index` opcode (`VM.cpp`, `case OpCode::Index`) already
dispatches on target type with an `Array`/`Mapping`/`string` chain, and
adding a matching `Array` branch to the new range-index handling below
costs one more `else if` block of the same shape the `string` branch
already needs, no new parsing or lexing work (target type is a runtime
dispatch, not a syntax difference). This is the same "generically
correct version costs almost nothing" reasoning already used for the
char-literal escape table and for non-empty mapping literals: the
design below includes an `Array` branch alongside the confirmed-needed
`string` branch, flagged explicitly as unconfirmed-by-real-usage so it
is not mistaken for a requirement found in master.c. `Mapping` is not a
sensible range-index target (LPC does not define slicing a mapping by
integer range) and is excluded the same way `Add`'s design excluded
nonsensical operand combinations.

## Lexer changes (Lexer.cpp, Lexer.hpp)

1. Add `c == '.'` to the character whitelist in `tokenize()`'s
   dispatch (currently at `src/compiler/Lexer.cpp` lines 188 to 192),
   routed to the existing `lexSymbol()` the same way `-`, `=`, `!`,
   `<`, `>`, `|`, `&` already are:
   ```cpp
   } else if (c == '(' || c == ')' || c == '{' || c == '}' ||
              c == '[' || c == ']' || c == ':' ||
              c == ';' || c == ',' || c == '-' || c == '=' ||
              c == '!' || c == '<' || c == '>' || c == '*' || c == '+' ||
              c == '|' || c == '&' || c == '/' || c == '%' || c == '.') {
       tokens.push_back(lexSymbol());
   }
   ```

2. `lexSymbol()` gains a lookahead branch for `.` immediately followed
   by a second `.`, the same pattern already used for `->`, `==`, `!=`,
   `<=`, `>=`, `||`, `&&`:
   ```cpp
   if (c == '.' && peek() == '.') {
       advance();
       return Token{TokenType::Symbol, "..", startLine};
   }
   ```
   No new `TokenType` is needed: `..` becomes `Symbol("..")`, exactly
   how `->` becomes `Symbol("->")` today, and the parser distinguishes
   it by text via `checkText("..")`, the same mechanism already used
   for every other multi-character operator.

3. A lone `.` not followed by a second `.` falls through to
   `lexSymbol()`'s existing final line, `return Token{TokenType::Symbol,
   std::string(1, c), startLine};`, producing `Symbol(".")`. This is a
   deliberate choice, not an oversight: nothing in the parser ever
   expects a bare `.` token (unlike `-`, which is meaningful alone as
   subtraction), so a stray `Symbol(".")` simply surfaces as an
   ordinary parse error downstream ("expected expression" or similar)
   the same way a stray unconsumed token from any other construct
   would, rather than needing a dedicated lexer-level throw the way
   `lexChar()` throws on a malformed character literal. The
   alternative considered, throwing directly out of `lexSymbol()` on an
   unmatched single `.`, was rejected for consistency: it would make
   `.` behave differently from every other symbol character in this
   lexer for no benefit, since master.c has zero real single-`.`
   sites (confirmed by Step 1) for this to protect against.

4. `Lexer.hpp` needs no changes: no new method, no new `TokenType`.

## AST/Parser changes: `IndexExpr` gains an optional end, not a new node

`parsePostfix()`'s existing `[` handling (`src/compiler/Parser.cpp`,
currently lines 199 to 208) parses a single expression between `[` and
`]` and always builds an `IndexExpr`. Extended to check for `..` before
the closing `]`:

```cpp
while (checkText("[")) {
    advance();
    AstPtr startExpr = parseExpr();
    AstPtr endExpr = nullptr;
    if (checkText("..")) {
        advance();
        endExpr = parseExpr();
    }
    expectText("]", "index expression");

    auto idx = std::make_unique<IndexExpr>();
    idx->target = std::move(expr);
    idx->index = std::move(startExpr);
    idx->rangeEnd = std::move(endExpr);
    expr = std::move(idx);
}
```

`IndexExpr` (`Ast.hpp`) gains one new optional field rather than a
sibling node type:

```cpp
struct IndexExpr : AstNode {
    AstPtr target;
    AstPtr index;
    AstPtr rangeEnd = nullptr; // non-null means range index, e.g. str[start..end]
};
```

`rangeEnd == nullptr` is the existing single-index case, completely
unchanged in shape and behavior. `rangeEnd != nullptr` is a range
index, reusing `target` and `index` (as the range's start) exactly as
they already exist. This was weighed against a dedicated
`RangeIndexExpr` node with three fields (`target`, `start`, `end`) and
no shared base with `IndexExpr`. The optional-field approach wins
because the two constructs share their entire parse-time shape (same
`[` handling, same "parse an expression here" logic for both the index
and the range end) and CodeGen's target-emission is identical for both;
only the trailing emission (one opcode with two operands pushed, versus
another opcode with three) actually differs, which is naturally
expressed as an `if (idx->rangeEnd)` branch inside the one existing
`IndexExpr` case rather than as a wholly separate `dynamic_cast`
branch. This mirrors the reasoning already used for reusing
`BinaryExpr` across `Add`/`Sub`/`Mul`/`Div`/`Mod`/`Or`/`And`: one node,
differentiated by a field, when the parse-time shape is genuinely
shared. It differs from the char-literals slice's `IntLiteral` reuse
in one respect worth calling out: there, the reused node's runtime
behavior was identical to existing uses (an int is an int no matter
where its value came from), so no downstream code needed to inspect
anything new. Here, `CodeGen` and the VM do need to know which case
they are in, so `rangeEnd`'s presence is exactly that discriminator,
not decoration.

No `Parser.hpp` changes: no new parse function, only a body change to
the existing `parsePostfix()`.

## VM changes: a new `RangeIndex` opcode, not an extension of `Index`

`Index` currently pops two stack values (target, index) and pushes one
(a single element or character code). A range index needs three popped
values (target, start, end) and pushes a different kind of result (a
substring or sub-array, not a single value). This is a different stack
arity and a different result shape, not a variant of the same
operation, so it gets its own opcode rather than a widened `Index`
case, the same granularity already used to keep `IndexAssign` separate
from `Index` for the read-versus-write distinction. Unlike
`Sub`/`Mul`/`Div`/`Mod`, there is no already-declared-but-unused
`RangeIndex` stub sitting in `Bytecode.hpp` to reuse; this genuinely
adds one new `OpCode` value.

`Bytecode.hpp`:
```cpp
enum class OpCode : uint8_t {
    PushConst,
    PushInt,
    PushLocal,
    StoreLocal,
    Add, Sub, Mul, Div, Mod,
    Eq, Neq, Lt, Lte, Gt, Gte,
    Not,
    Jump,
    JumpIfFalse,
    Call,
    CallEfun,
    CallApply,
    MakeArray,
    MakeMapping,
    Index,
    IndexAssign,
    RangeIndex,
    Return,
    Pop,
    Dup,
    Halt
};
```

`CodeGen.cpp`'s existing `IndexExpr` branch in `emitExpr()` (currently
lines 159 to 164) gains a check on `rangeEnd`:

```cpp
if (auto* idx = dynamic_cast<const IndexExpr*>(&expr)) {
    emitExpr(*idx->target);
    emitExpr(*idx->index);
    if (idx->rangeEnd) {
        emitExpr(*idx->rangeEnd);
        out_->code.push_back(Instruction{OpCode::RangeIndex, 0, 0});
    } else {
        out_->code.push_back(Instruction{OpCode::Index, 0, 0});
    }
    return;
}
```

`VM.cpp` gains a new case alongside the existing `Index`/`IndexAssign`
cases, following the same stack-underflow-guard and per-type-branch
shape those already use:

```cpp
case OpCode::RangeIndex: {
    if (localStack.size() < 3) {
        throw LpcRuntimeError("RangeIndex: stack underflow");
    }
    Value endVal = localStack.back(); localStack.pop_back();
    Value startVal = localStack.back(); localStack.pop_back();
    Value targetVal = localStack.back(); localStack.pop_back();

    if (!std::holds_alternative<int64_t>(startVal.data)) {
        throw LpcRuntimeError("RangeIndex: start index must be an integer");
    }
    if (!std::holds_alternative<int64_t>(endVal.data)) {
        throw LpcRuntimeError("RangeIndex: end index must be an integer");
    }
    int64_t start = std::get<int64_t>(startVal.data);
    int64_t end = std::get<int64_t>(endVal.data);
    if (start < 0) {
        throw LpcRuntimeError("RangeIndex: start index must be non-negative");
    }

    if (auto* str = std::get_if<std::string>(&targetVal.data)) {
        int64_t len = static_cast<int64_t>(str->size());
        int64_t clampedEnd = std::min(end, len - 1);
        if (start > clampedEnd) {
            localStack.push_back(Value(std::string()));
        } else {
            localStack.push_back(Value(str->substr(
                static_cast<size_t>(start),
                static_cast<size_t>(clampedEnd - start + 1))));
        }
    } else if (auto* arr = std::get_if<std::shared_ptr<Array>>(&targetVal.data)) {
        if (!*arr) {
            throw LpcRuntimeError("RangeIndex: target array is null");
        }
        int64_t len = static_cast<int64_t>((*arr)->items.size());
        int64_t clampedEnd = std::min(end, len - 1);
        auto result = std::make_shared<Array>();
        for (int64_t i = start; i <= clampedEnd; ++i) {
            result->items.push_back((*arr)->items[static_cast<size_t>(i)]);
        }
        localStack.push_back(Value(result));
    } else {
        throw LpcRuntimeError("RangeIndex: target is not an array or string");
    }
    ++ip;
    break;
}
```

Bounds behavior, stated explicitly since it is a real design choice,
not an accident:
- a negative start is a hard error, matching the existing
  `Index`/`IndexAssign` convention of throwing on an invalid index
  rather than clamping it.
- an end at or beyond the target's length is clamped down to
  `length - 1` rather than throwing. This matches standard LPC slice
  semantics (an over-long end is not an error, the slice just runs to
  the target's actual end) and is the only bounds behavior that matters
  for the confirmed real sites, since `x[0..0]` on a nonempty name
  string never reaches this branch anyway.
- `start > clampedEnd` (covers both an inverted range like `[5..2]` and
  an empty target like `""[0..0]`, where `clampedEnd` becomes `-1`)
  returns an empty string or empty array rather than throwing. This is
  the standard LPC convention for a range that names no elements, and
  avoids a startup-time crash on an edge case (an empty player name)
  that should not exist in practice but is not worth hard-failing on
  when "nothing" is the obviously correct answer.

`start`/`end` type-checking mirrors `Index`'s existing "array index
must be an integer" / "string index must be an integer" wording exactly,
substituting "start index"/"end index" for "index" and "RangeIndex" for
"Index".

## Range indexing as an assignment target: confirmed out of scope

No occurrence of `x[a..b] = ...` (a range-index write, e.g. splicing a
new substring or sub-array into place) appears anywhere in master.c or
master.h; every one of the seven real sites is read-only, used
immediately as part of a larger string concatenation. This is the same
scope-out already applied to single-index string writes in the
char-literals slice, for the same reason: building it now would be
speculative. If a real need surfaces later, it is a self-contained
follow-up: extend `IndexAssignStmt`/`OpCode::IndexAssign` with a
range-aware variant, which for arrays can reuse `IndexAssign`'s
existing shared_ptr-aliasing mutation approach, but for strings runs
into the same by-value-not-by-reference complication the char-literals
slice already flagged for single-index string writes.

## Omitted start/end (open-ended ranges): confirmed out of scope

Real LPC supports `x[i..]` (from `i` to the end) and `x[..j]` (from the
start to `j`). Neither appears anywhere in master.c or master.h: all
seven real sites, and the one commented-out eighth, always write both
bounds explicitly (`0..0`). Supporting omitted bounds would need
`startExpr`/`endExpr` in the parser change above to each become
optional (checking for `]` or `..` immediately after `[` in place of an
expression, and for `]` immediately after `..` in place of a second
expression), plus a sentinel or defaulting convention in the VM
(something like "missing start means 0, missing end means length -
1"). This is real added parsing complexity, not a near-free
generalization the way the char-literal escape table or the `Array`
branch above were, so it is scoped out explicitly rather than built
speculatively, the same reasoning already applied to `sscanf()` in two
prior slices. Flagging the shape of the follow-up here so it is not
re-derived from scratch if a future file needs it.

## Confirmed out of scope: negative range bounds

Real LPC also supports negative range bounds as a "count from the end"
idiom, `str[0..-1]` (whole string) or `str[-3..-1]` (last three
characters), where a negative index counts backward from the end of
the target instead of forward from the start. This slice's VM design
above does not support that: the `RangeIndex` case throws
`"RangeIndex: start index must be non-negative"` for any negative
start, and a negative end would either be caught by the same start
check (if also negative and compared after) or produce a nonsensical
`clampedEnd` if it slipped through, since `std::min(end, len - 1)` was
written assuming `end >= 0`. Neither is handled, and a negative index
is a hard error here, not a from-the-end offset.

This is a real, separate gap, not a silent one: it is written down here
explicitly, the same way open-ended ranges are above, rather than left
to be rediscovered later as a mystery when some future file does
`name[-1..-1]` and gets a confusing `LpcRuntimeError` instead of the
last character. Zero of the seven confirmed real sites in master.c need
this (every real site uses a literal, non-negative `0..0`), so it is
scoped out for the same reason `sscanf()` and open-ended ranges are:
real added complexity (every bound, on both `string` and `Array`
targets, would need a "if negative, add the target's length before
using it" normalization step, applied consistently to both `start` and
`end` before any of the existing bounds checks run) with no confirmed
caller in this codebase today. If a future slice needs it, the fix is
localized to the `RangeIndex` case: normalize `start` and `end` (add
`len` once if negative) before the existing non-negative and clamping
checks, for both the `string` and `Array` branches.

## Step 4: does this actually unblock the deferred `Add` string+int question?

No, not by itself, and this is worth stating plainly since it directly
affects what the tenth boot attempt will show. Compilation in this
driver is whole-file: `ObjectManager::compile()` lexes and parses the
entire preprocessed unit before any of it runs, so nothing in
`create()` executes, including the `preload()` call and its line-152
arithmetic, until every construct anywhere in master.c parses
successfully, not just the ones before line 152 in file order.

I re-scanned the full preprocessed-and-stripped unit against this
lexer's actual whitelist (letters/digits/`_`, `"`, `'`, whitespace, the
existing symbol whitelist, comments, and `.`/`..` once this slice
lands) to check whether line 215 really is the last gap. It is not:
three ternary-conditional (`?:`) sites remain, at raw lines 433, 469,
and 519 (listed in Step 1 above), none of them inside a string or
comment, and `?` is not in the lexer's whitelist today. `master.h` has
none, so nothing arrives through the header either. Ternary
conditional expressions are a distinct, separate construct from range
indexing (a three-part `cond ? a : b` expression, with its own parser
precedence level and its own AST node, not a shape this slice's changes
happen to cover), so resolving `..` does not resolve `?:` as a side
effect.

Concretely: once this slice lands, the tenth boot attempt is very
likely to hit `unrecognized character '?' at line` (whatever the
preprocessed number for raw line 433 turns out to be), not a
successful boot. Only once that gap is also closed does the whole file
compile, `create()` actually run, and `preload()`'s
`write("("+(t/60)+"."+(t%60)+")\n")` actually execute far enough to
reach the string-concatenation step. And when it does, the deferred
question from the arithmetic slice is confirmed still open by reading
`VM.cpp`'s current `Add` case directly: it has exactly three branches
(`string`+`string`, `Array`+`Array`, numeric+numeric) and an `else`
that throws `"Add: unsupported operand types"` for anything else,
including `string`+`int64_t`. `"(" + (t/60)` is precisely that
unhandled combination, so line 152 will still throw at runtime even
after both the range-indexing and ternary gaps are closed, unless a
further slice teaches `Add` to stringify a numeric right-hand operand
the way LPC's `+` normally does. Noting this now so it is not mistaken
for a regression when a future boot attempt reaches that line: it is
the same known `Add` gap the arithmetic slice already flagged,
surfacing for the first time only once everything ahead of it in the
file finally compiles.

## Test plan (mirrors the prior slices)

Unit tests in `test_lexer.cpp`:
- Lex `..` and assert it produces a single `Symbol("..")` token, not
  two separate `Symbol(".")` tokens.
- Lex `name[0..0]` end to end and assert the token stream is
  `Ident, [, Number(0), .., Number(0), ]`, confirming no ambiguity with
  plain single-index tokenizing.
- Regression check: lex an ordinary single index, `name[0]`, and
  confirm it is unaffected (still `Ident, [, Number(0), ]`, no `..`
  token appears).

Parser tests:
- Parse `x[0..0]` and assert the result is an `IndexExpr` with
  `rangeEnd` non-null, `index` and `rangeEnd` both `IntLiteral(0)`.
- Parse `x[0]` and assert `rangeEnd` is null, confirming the existing
  single-index path is untouched.
- Parse the real shape, `DIR_USERS+"/"+name[0..0]`, and assert the
  range index binds only to `name`, not to the surrounding
  concatenation (the `[` handling belongs to `parsePostfix()`, which
  runs at higher precedence than `+`, so this should already fall out
  correctly with no special-casing, worth asserting directly rather
  than assuming).

VM-level execution tests:
- `"hello"[0..0]` returns `"h"`; `"hello"[1..3]` returns `"ell"`;
  `"hello"[0..4]` returns `"hello"`.
- `"hello"[0..99]` returns `"hello"` (end clamped, not an error).
- `"hello"[3..1]` returns `""` (inverted range, not an error).
- `""[0..0]` returns `""` (empty target, not an error).
- `"hello"[-1..2]` throws `LpcRuntimeError` (negative start).
- The `Array` branch, mirrored: `({1,2,3,4})[1..2]` returns
  `({2,3})`, `({1,2,3})[0..99]` returns `({1,2,3})`, and
  `({1,2,3})[2..0]` returns `({ })`.
- A VM-level test reproducing the real blocking shape directly: given
  `string name = "thurtea";`, `name[0..0] == "t"`.

`mudlib_stub` live-verification object: same pattern as the prior three
slices, a function reproducing `DIR_USERS+"/"+name[0..0]` against a
fixed name and asserting the resulting path string. Not written or run
as part of this planning pass; per this repository's standing
verification boundary, implementation and live-boot verification stay
with the user, this document only proposes what that step should cover
once the user asks for implementation.

## Sequencing after this slice

Confirmed still out of scope, unchanged from prior slices' notes:
`sscanf()`, string/array range indexing as an assignment target,
open-ended ranges (`[i..]`/`[..j]`), and negative range bounds
(`str[0..-1]`, `str[-3..-1]`). Newly confirmed by this slice's
own Step 4 scan: ternary conditional expressions (`?:`), found at raw
lines 433, 469, and 519, none reachable through master.h. Given the
whole-file compile model, `?:` is a strong candidate for the tenth boot
attempt's blocker once this slice lands, the same way this slice was a
reasonable (and correct) guess coming out of the char-literals slice.
Also still open, confirmed unchanged by re-reading `VM.cpp`'s `Add`
case directly rather than assumed from memory: `Add` has no
string-plus-numeric branch, so line 152's `write("("+(t/60)+"."+(t%60)+")\n")`
will throw `LpcRuntimeError("Add: unsupported operand types")` at
runtime the first time it actually executes, which per Step 4 above
requires both this slice and a ternary-operator slice to land first.
Not confirmed until an actual boot attempt runs past both gaps.
