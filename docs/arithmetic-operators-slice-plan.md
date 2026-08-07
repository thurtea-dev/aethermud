# Arithmetic operators feature slice: plan

Status: planning only, no implementation yet.

Scope: add real binary subtraction (`-`), multiplication (`*`),
division (`/`), and modulo (`%`), plus unary negation (`-x`), as
genuine expression operators. Confirmed necessary by the blocking line
identified by the eighth boot attempt, `secure/daemon/master.c:152`:

```c
write("("+(t/60)+"."+(t%60)+")\n");
```

## Step 1 findings: collision check

Confirmed by reading the current source before proposing anything new
(full detail already reported separately, summarized here for the
plan record):

- `-` is consumed by the parser in exactly one place, `parsePostfix()`'s
  `checkText("->")`. The lexer already tokenizes a lone `-` correctly
  today (falls through to a plain `Symbol("-")` whenever not followed
  by `>`), so no lexer change is needed for `-` itself. Negative
  literals (`-1`) do not work today: confirmed, not assumed, by tracing
  that `lexNumber()` only starts on a digit, so `-1` always tokenizes
  as two separate tokens, and nothing currently consumes a leading `-`
  in expression position.
- `*` is consumed in exactly three places, all in statement/declaration
  preambles (`parseVarDeclStatement`, `parseParamList`, `parseFunction`),
  always immediately after a type keyword and before an identifier,
  never inside the expression-parsing chain
  (`parseAdditive`/`parseUnary`/`parsePostfix`/`parsePrimary`). No
  ambiguity exists between the array-marker use and a future
  multiplication operator: they live in structurally disjoint call
  paths in this recursive-descent parser, disambiguated by which
  function is executing, not by lookahead.

## Step 2 findings: confirmed real usage

Scanned `master.c`, `master.h`, and the full preprocessed compilation
unit (all eight included headers expanded: `config.h`, `objects.h`,
`rooms.h`, `cfg.h`, `privs.h`, `dirs.h`, `save.h`, `daemons.h`), using a
comment/string/char-literal-aware scan so sprintf format specifiers,
path text inside string literals, and `->` are correctly excluded from
the arithmetic counts. Preprocessed-unit totals matched the
master.c-plus-master.h totals exactly, confirming no included header
contributes anything extra.

| Category | Count |
|---|---|
| `-` as `->` (already handled) | 13 |
| `-` as `--` decrement (distinct operator, out of scope) | 4 sites |
| `-` as unary negation (`-1`, `-2`, always literal) | 17 sites |
| `-` as binary subtraction | 1 site (`t = time() - t;`, line 151) |
| `*` as array-type marker (already handled) | 28 |
| `*` as binary multiplication | 0 |
| `/` as division | 1 site (`t/60`, line 152) |
| `%` as modulo | 1 site (`t%60`, line 152) |

Unary negation is the dominant confirmed need, well ahead of binary
subtraction. Multiplication has zero confirmed sites in this file, but
is still included below rather than left out: building `-`/`/`/`%`
without `*` would leave the arithmetic precedence level partial for no
real savings (the VM opcode already exists unused, see below), the
same reasoning already used for non-empty mapping literals in an
earlier slice despite only the empty form being observed at the time.

### Known fallout: while(i--) will start throwing a different error

The four `while(i--)` sites do not parse today (a bare `-` is
unconsumed anywhere, so the statement fails at the following `)` with
`expected ';' in expression statement`). After this slice, they will
still fail, but differently: `parseAdditive()` will consume the first
`-` as binary subtraction, then call `parseUnary()` for the right
operand, which will consume the second `-` as unary negation and then
recurse into `parseUnary()` again looking for that negation's operand,
landing on the loop's closing `)` -- which `parsePrimary()` has no case
for, so it throws `parse error: expected expression at line N (got
")")`. Both before and after this slice `while(i--)` fails to parse,
just with a different message, since `--` (increment/decrement) is a
distinct, separate operator this slice does not implement. Flagging
this now so it is not mistaken for a regression when the next boot
attempt runs: it is the same known gap surfacing through a different
error message, not a new one.

## Lexer changes (Lexer.cpp)

1. Add `c == '/'` and `c == '%'` to the character whitelist in
   `tokenize()`'s dispatch, the same one-line-per-character addition
   already used for `+`, `|`, `&`.

2. No ambiguity with the existing comment logic. `tokenize()`'s main
   loop calls `skipWhitespaceAndComments()` once at the top of every
   iteration, before the character-classification dispatch ever runs.
   That function already fully consumes `//...` and `/* ... */` in a
   loop before returning. So by the time the dispatch checks `c == '/'`,
   any `/` that could have started a comment has already been consumed
   upstream; a `/` that survives to the dispatch is guaranteed not to
   be a comment start. No two-character lookahead is needed in
   `lexSymbol()` for `/` (there is no `/=` in scope), it becomes a
   plain single-character `Symbol("/")` the same way `+` already is.

3. `%` needs no special handling either: it is never used for anything
   else at the lexer level (sprintf format specifiers like `%s` are
   just bytes inside an already-lexed string literal, `lexString()`
   does not inspect them). Plain single-character `Symbol("%")`.

4. `-` needs no lexer change at all, per the Step 1 findings above: it
   already tokenizes correctly today.

## Parser changes (Parser.cpp, Parser.hpp)

Precedence, standard C-family: multiplicative (`*`, `/`, `%`) binds
tighter than additive (`+`, `-`), which binds tighter than everything
above it in the existing chain, and looser than unary. Inserted
between `parseAdditive` and `parseUnary`:

```
parseExpr -> parseLogicalOr -> parseLogicalAnd -> parseEquality
          -> parseComparison -> parseAdditive -> parseMultiplicative
          -> parseUnary -> parsePostfix -> parsePrimary
```

1. New `parseMultiplicative()`, same loop-while-operator-matches shape
   already used throughout this precedence chain:
   ```cpp
   AstPtr Parser::parseMultiplicative() {
       AstPtr left = parseUnary();

       while (checkText("*") || checkText("/") || checkText("%")) {
           std::string opText = advance().text;
           BinOp op = (opText == "*") ? BinOp::Mul
                    : (opText == "/") ? BinOp::Div
                    : BinOp::Mod;

           auto right = parseUnary();
           auto bin = std::make_unique<BinaryExpr>();
           bin->op = op;
           bin->left = std::move(left);
           bin->right = std::move(right);
           left = std::move(bin);
       }

       return left;
   }
   ```

2. `parseAdditive()`'s two calls to its operand parser change from
   `parseUnary()` to `parseMultiplicative()`, and its loop condition
   gains `-`:
   ```cpp
   AstPtr Parser::parseAdditive() {
       AstPtr left = parseMultiplicative();

       while (checkText("+") || checkText("-")) {
           std::string opText = advance().text;
           BinOp op = (opText == "+") ? BinOp::Add : BinOp::Sub;

           auto right = parseMultiplicative();
           auto bin = std::make_unique<BinaryExpr>();
           bin->op = op;
           bin->left = std::move(left);
           bin->right = std::move(right);
           left = std::move(bin);
       }

       return left;
   }
   ```

3. `parseUnary()` gains a second prefix case for `-`, alongside the
   existing `!` case, reusing the `UnaryExpr` node from the
   logical-operators slice with a new `UnaryOp::Neg` member:
   ```cpp
   AstPtr Parser::parseUnary() {
       if (checkText("!")) {
           advance();
           auto operand = parseUnary();
           auto notExpr = std::make_unique<UnaryExpr>();
           notExpr->op = UnaryOp::Not;
           notExpr->operand = std::move(operand);
           return notExpr;
       }
       if (checkText("-")) {
           advance();
           auto operand = parseUnary(); // right-associative
           auto negExpr = std::make_unique<UnaryExpr>();
           negExpr->op = UnaryOp::Neg;
           negExpr->operand = std::move(operand);
           return negExpr;
       }
       return parsePostfix();
   }
   ```
   This covers both confirmed usage shapes: a negative literal
   (`-1`) is `UnaryExpr(Neg, IntLiteral(1))`, and the same mechanism
   generalizes correctly to `-x` or `-(a+b)` even though no such site
   was found in master.c today, the same reasoning already applied to
   `!`'s right-associative chaining in the logical-operators slice.

4. `Parser.hpp` declares `parseMultiplicative` alongside the existing
   precedence-chain methods. `parseUnary` is already declared from the
   logical-operators slice, no new declaration needed for it.

## AST changes (Ast.hpp)

1. `BinOp` gains `Sub`, `Mul`, `Div`, `Mod`:
   ```cpp
   enum class BinOp { Eq, Neq, Lt, Lte, Gt, Gte, Add, Or, And, Sub, Mul, Div, Mod };
   ```
   No new node type: reuses `BinaryExpr` exactly as `Add`/`Or`/`And`
   already do.

2. `UnaryOp` gains `Neg`:
   ```cpp
   enum class UnaryOp { Not, Neg };
   ```
   No new node type: reuses `UnaryExpr` exactly as `Not` already does.

## CodeGen changes (CodeGen.cpp)

1. `emitBinaryExpr()`'s existing switch gains four cases, reusing the
   already-declared opcodes confirmed below, the same pattern already
   used for `Add`:
   ```cpp
   case BinOp::Sub: op = OpCode::Sub; break;
   case BinOp::Mul: op = OpCode::Mul; break;
   case BinOp::Div: op = OpCode::Div; break;
   case BinOp::Mod: op = OpCode::Mod; break;
   ```
   `Sub`/`Mul`/`Div`/`Mod` are not `Or`/`And`, so they are unaffected
   by `emitExpr()`'s existing short-circuit routing check and fall
   through to `emitBinaryExpr()` normally, alongside `Eq`/`Lt`/`Add`.

2. `emitExpr()`'s existing `UnaryExpr` branch, added in the
   logical-operators slice, gains a second case for `UnaryOp::Neg`.
   Unary negation is desugared into the existing `Sub` opcode instead
   of a new dedicated opcode, reusing it the same way the plan reuses
   `Sub`/`Mul`/`Div`/`Mod` themselves rather than inventing new
   surface:
   ```cpp
   if (auto* un = dynamic_cast<const UnaryExpr*>(&expr)) {
       if (un->op == UnaryOp::Not) {
           emitExpr(*un->operand);
           out_->code.push_back(Instruction{OpCode::Not, 0, 0});
       } else { // UnaryOp::Neg
           out_->code.push_back(Instruction{OpCode::PushInt, 0, 0});
           emitExpr(*un->operand);
           out_->code.push_back(Instruction{OpCode::Sub, 0, 0});
       }
       return;
   }
   ```
   This also means unary negation automatically inherits `Sub`'s own
   numeric type-checking and division-by-zero-irrelevant-here behavior
   for free: `-x` where `x` is not numeric fails with the exact same
   `LpcRuntimeError` `Sub` already produces for a non-numeric operand,
   with no separate error path to keep consistent.

## VM changes (Bytecode.hpp, VM.cpp)

Confirmed by reading the current source before proposing anything new:
`Bytecode.hpp` already declares `Sub, Mul, Div, Mod` in the `OpCode`
enum, immediately after `Add`. Grepped both `CodeGen.cpp` and `VM.cpp`
for any reference to these four names: zero hits in either file. They
are genuine declared-but-unimplemented stubs, exactly like
`MakeArray`/`Index`/`Dup` were before their respective slices: never
emitted, never handled, falling to `VM.cpp`'s `default` case and
throwing `NotImplementedError` if ever reached. This slice reuses all
four exactly as declared, no `Bytecode.hpp` change needed.

New grouped case in `VM.cpp`'s execution switch, mirroring the
existing `Lt`/`Lte`/`Gt`/`Gte` grouping shape (one shared block, an
inner switch to pick the specific operation), rather than four
separately duplicated blocks:

```cpp
case OpCode::Sub:
case OpCode::Mul:
case OpCode::Div:
case OpCode::Mod: {
    if (localStack.size() < 2) {
        throw LpcRuntimeError("arithmetic: stack underflow");
    }
    Value rhs = localStack.back(); localStack.pop_back();
    Value lhs = localStack.back(); localStack.pop_back();

    bool eitherDouble = std::holds_alternative<double>(lhs.data) ||
                         std::holds_alternative<double>(rhs.data);

    double lv, rv;
    if (std::holds_alternative<int64_t>(lhs.data)) lv = static_cast<double>(std::get<int64_t>(lhs.data));
    else if (std::holds_alternative<double>(lhs.data)) lv = std::get<double>(lhs.data);
    else throw LpcRuntimeError("arithmetic: left operand is not numeric");

    if (std::holds_alternative<int64_t>(rhs.data)) rv = static_cast<double>(std::get<int64_t>(rhs.data));
    else if (std::holds_alternative<double>(rhs.data)) rv = std::get<double>(rhs.data);
    else throw LpcRuntimeError("arithmetic: right operand is not numeric");

    if ((instr.op == OpCode::Div || instr.op == OpCode::Mod) && rv == 0.0) {
        throw LpcRuntimeError(instr.op == OpCode::Div
            ? "Div: division by zero"
            : "Mod: modulo by zero");
    }

    double result;
    switch (instr.op) {
        case OpCode::Sub: result = lv - rv; break;
        case OpCode::Mul: result = lv * rv; break;
        case OpCode::Div: result = lv / rv; break;
        case OpCode::Mod: result = std::fmod(lv, rv); break;
        default: break;
    }

    if (eitherDouble) {
        localStack.emplace_back(Value(result));
    } else {
        localStack.emplace_back(Value(static_cast<int64_t>(result)));
    }
    ++ip;
    break;
}
```

This mirrors `Add`'s existing numeric branch exactly (same
`eitherDouble` promotion pattern, same int64_t/double extraction
shape, same cast-back-to-int64_t-if-neither-was-double convention),
per the instruction to match what was done for `Add`. Unlike `Add`,
there is no string or array branch here: `Add`'s string concatenation
and array concatenation are meaningful because `+` genuinely has those
LPC semantics; `-`/`*`/`/`/`%` do not, LPC does not define subtracting
two strings or multiplying two arrays, so numeric-only with a clear
`LpcRuntimeError` for anything else is the correct behavior, not a
narrower version of `Add`'s. Division and modulo by zero are checked
once, after both operands are already confirmed numeric, covering both
integer and float right-hand operands with a single `rv == 0.0` check
against the already-promoted double value.

`Mod` uses `std::fmod` rather than integer `%` directly, since the
shared code path already works in `double` space the same way `Add`'s
numeric branch does; this is the direct analog of using `+` in double
space for addition, not a separate design decision. For the confirmed
real usage (`t%60` where `t` is an int-valued timestamp difference),
this produces the same result as integer modulo.

## Test plan (mirrors the prior slices)

Unit tests in `test_lexer.cpp`:
- Lex `/` and `%` each as their own single-character `Symbol` token,
  and confirm `//` and `/* ... */` still fully consume as comments
  (regression check: adding `/` to the whitelist must not break
  existing comment handling).
- Parse `t/60` and `t%60` and assert `BinOp::Div`/`BinOp::Mod`
  respectively.
- Parse `-1` and assert the result is a `UnaryExpr` with `op ==
  UnaryOp::Neg` wrapping an `IntLiteral` with `value == 1`, confirming
  no literal-only special case leaked in, the general unary path is
  what actually handles it.
- Parse `a - b * c` and assert the outer node is `BinOp::Sub` whose
  right operand is itself a `BinOp::Mul`, confirming multiplicative
  binds tighter than additive.
- Codegen `t/60` and `t%60` and assert the instruction stream contains
  `OpCode::Div` and `OpCode::Mod` respectively.
- VM-level execution tests: `10 - 3` returns `7`; `4 * 5` returns
  `20`; `17 / 5` returns `3` (integer division); `17 % 5` returns `2`;
  `-1` alone returns `-1`; a non-numeric operand to each of
  `-`/`*`/`/`/`%` throws `LpcRuntimeError`; `5 / 0` and `5 % 0` both
  throw `LpcRuntimeError` rather than crashing or returning a bogus
  value.
- A VM-level test reproducing the real blocking line's arithmetic
  directly: given `t = 125`, `t/60 == 2` and `t%60 == 5`.

`mudlib_stub` live-verification object: a function reproducing the
real blocking line's shape,
`write("("+(t/60)+"."+(t%60)+")\n");`-equivalent, with a fixed `t`
value and an expected printed string, confirming string concatenation
(already implemented) composes correctly with the new arithmetic
opcodes end to end through an actual `write()` call. Same live-boot
verification style as the prior four slices: temporarily wire the
object into `master.c`'s `create()`, boot against `mudlib_stub`,
capture the transcript, then revert `master.c`.

## Sequencing after this slice

Confirmed still out of scope, unchanged from prior slices' notes:
`sscanf()`, range/slice indexing (`x[0..0]`), and now also
increment/decrement (`--`, and by extension `++`, neither found
independently but `--` is confirmed present at 4 sites and `++` is a
reasonable guess for elsewhere in the mudlib given the symmetry). Any
of these, or some other still-unseen construct, could be the next
wall; given `while(i--)`'s fallout described above, `--` is a
reasonable candidate for what the ninth boot attempt hits, but this is
not confirmed until that attempt actually runs.
