# Character literals and string indexing feature slice: plan

Status: planning only, no implementation yet.

Scope: add LPC character literal syntax (`'x'`), and extend string
indexing (`str[i]`) to actually work at the VM level for read access.
Both are needed together to clear the blocking line identified by the
seventh boot attempt, `secure/daemon/master.c:65`:

```c
if(!lines[i] || lines[i] == "" || lines[i][0] == '#') continue;
```

`lines[i][0]` already parses today (`IndexExpr` and the `Index` opcode
exist since the arrays/mappings slice), but the VM's `Index` case only
handles `Array`/`Mapping` targets, throwing for anything else,
including a `std::string`. `'#'` does not lex at all today, `'` is not
in the lexer's whitelist. Neither gap is useful without the other:
fixing only the literal would still leave `lines[i][0]` throwing at
runtime, fixing only string indexing would still leave `'#'`
unlexable. Confirmed by reading the current source before proposing
anything new (see the sections below).

## Step 2 findings: what master.c and master.h actually use

Searched both files (`grep -n "'"`) plus the fully preprocessed
compilation unit (`cpp -I secure/include -x c secure/daemon/master.c`,
which inlines every `#include`d header master.c pulls in: `config.h`,
`objects.h`, `rooms.h`, `cfg.h`, `privs.h`, `dirs.h`, `save.h`,
`daemons.h`, plus `master.h` itself) to make sure nothing arrives only
through an included header. The preprocessed scan found the exact same
set, just at shifted line numbers, confirming no included header adds
anything beyond what is already in master.c/master.h directly.
`master.h` itself has no single-quote occurrences at all.

Nine real character-literal sites, ten literal occurrences, in
master.c (line numbers are the real source file, not preprocessed):

| Line | Code | Literal | Comparison |
|------|------|---------|------------|
| 65   | `lines[i][0] == '#'` | `'#'` | `==` |
| 114  | `lines[i][0] == '#'` | `'#'` | `==` |
| 162  | `items[i][0] == '#'` | `'#'` | `==` |
| 171  | `file[0] != '/'` | `'/'` | `!=` |
| 182  | `file[0] != '/'` | `'/'` | `!=` |
| 399  | `file[0] != '/'` | `'/'` | `!=` |
| 526  | `str[0] != '/'` | `'/'` | `!=` |
| 534  | `str[0] != '/'` | `'/'` | `!=` |
| 594  | `str[0] < 'a' \|\| str[0] > 'z'` | `'a'`, `'z'` | `<`, `>` |

Every one of these is a comparison against `x[0]`, the first
character of a string. None is a multi-character index, none is an
assignment target (`string[i] = 'x'` does not appear anywhere in
master.c or master.h). This confirms string indexing as an lvalue is
genuinely unused here, not just unexamined, so it is explicitly scoped
out of this slice below rather than built speculatively.

Two false positives worth naming explicitly, so they are not mistaken
for additional character-literal gaps:
- Line 462: `sprintf("'%s' at %s", ...)`. The `'` characters here are
  literal bytes inside an already-lexed double-quoted string, not
  character-literal tokens. `lexString()` already consumes arbitrary
  bytes (including `'`) up to the closing `"` with no gap.
- Line 306: `... a fresh account's first character, ...`. This `'` is
  an English possessive apostrophe inside a `/* ... */` block comment.
  Comments are stripped by `skipWhitespaceAndComments()` before any
  token dispatch runs, so this is not reachable by the lexer at all.

No escape sequences (`'\n'`, `'\t'`, etc.) appear anywhere in either
file: a targeted search for a literal backslash immediately after an
opening quote (`'\`) returned nothing, and the ten confirmed literals
above are all single printable ASCII characters (`#`, `/`, `a`, `z`).
So master.c itself does not require escape support. The lexer design
below adds it anyway (see the Lexer section) since it costs almost
nothing on top of the plain-character case (mirrors `lexString()`'s
existing escape table almost exactly) and mirrors the reasoning
already used for non-empty mapping literals in the arrays/mappings
slice plan: build the generically-correct version of a construct when
it is nearly free, rather than the narrowest version that only covers
today's grep hits and immediately needs revisiting.

One additional, distinct gap found while searching, explicitly out of
scope for this slice: range/slice indexing, `x[0..0]`. Found at seven
real call sites (lines 215, 317, 318, 505, 593, 595, 601; an eighth at
line 608 is commented out), always in the same
`DIR_USERS+"/"+name[0..0]` shape (a common LPC idiom: shard player
save files by the first letter of their name). This is a materially
different feature from single-index string reads: it uses a `..`
range operator (not currently lexed, `.` is not in the whitellist
either), needs its own AST node (a range has two ends, `IndexExpr` has
one), and returns a `String` substring rather than an `int` character
code the way `x[0]` does. It is not folded into this slice for the
same reason `sscanf()` was excluded from the arrays/mappings slice and
`sscanf()`/range-indexing both remain excluded here: unrelated parsing
complexity that deserves its own scoped slice rather than growing this
one. Given how many sites use it, it is likely to become the next wall
shortly after this slice lands, similar to how `||`/`&&` was
immediately followed by this slice's blocker on the very same line.

## Lexer changes (Lexer.cpp, Lexer.hpp)

1. New private method `lexChar()`, mirroring the shape of the existing
   `lexString()`:
   ```cpp
   Token Lexer::lexChar() {
       int startLine = line_;
       advance(); // consume opening '

       if (atEnd()) {
           throw LpcRuntimeError("unterminated character literal at line " +
                                  std::to_string(startLine));
       }

       char c = advance();
       int64_t code;
       if (c == '\\' && !atEnd()) {
           char esc = advance();
           switch (esc) {
               case 'n': code = '\n'; break;
               case 't': code = '\t'; break;
               case '\'': code = '\''; break;
               case '"': code = '"'; break;
               case '\\': code = '\\'; break;
               default: code = static_cast<unsigned char>(esc); break;
           }
       } else {
           code = static_cast<unsigned char>(c);
       }

       if (atEnd() || peek() != '\'') {
           throw LpcRuntimeError(
               "character literal must contain exactly one character, at line " +
               std::to_string(startLine));
       }
       advance(); // consume closing '

       return Token{TokenType::Number, std::to_string(code), startLine};
   }
   ```
   The escape table matches `lexString()`'s existing cases exactly
   (`n`, `t`, `"`, `\\`) plus one addition, `\'`, needed to represent a
   literal single quote character (`'\''`), which `lexString()` has no
   equivalent need for since `"` already terminates strings on an
   unescaped `"`, not `'`.

2. `tokenize()`'s main dispatch gains a branch for `c == '\''`,
   alongside the existing `c == '"'` branch that calls `lexString()`:
   ```cpp
   } else if (c == '\'') {
       tokens.push_back(lexChar());
   }
   ```
   This is a dedicated multi-character lexing routine, the same
   pattern `"` already uses, not an addition to the plain
   single-character symbol whitelist. There is no meaningful "bare
   `'`" token the way a lone `-` or `|` still has a fallback meaning;
   a malformed character literal is a real lexer error, matching how
   `lexString()` already throws on an unterminated string rather than
   silently falling through.

3. `Lexer.hpp` declares `lexChar()` alongside the existing
   `lexString()`/`lexNumber()` declarations. No `Lexer.hpp` changes
   are needed beyond that declaration; no new `TokenType` is added
   (see the AST/Parser section below for why).

## AST/Parser changes: reusing IntLiteral, not a new node

`IntLiteral` already exists and is exactly the right shape:
```cpp
struct IntLiteral : AstNode {
    int64_t value = 0;
};
```
Real LPC character literals are int values, `'#'` and `35` are the
same value at the language level, there is no separate character type
anywhere else in this VM's `Value` variant, and there is no character-
specific behavior anywhere downstream (`CodeGen`, `VM`) that would
ever need to distinguish "this int came from a char literal" from
"this int came from a digit sequence". So `lexChar()` above does the
ASCII/byte value conversion once, at lex time, and emits a plain
`TokenType::Number` token holding that value's decimal text, exactly
the token type `lexNumber()` already produces. `parsePrimary()`'s
existing `TokenType::Number` branch:
```cpp
if (check(TokenType::Number)) {
    auto lit = std::make_unique<IntLiteral>();
    lit->value = std::stoll(advance().text);
    return lit;
}
```
already handles it with zero changes. This means the entire
AST/Parser/CodeGen path for character literals needs no new node, no
new `Parser.cpp`/`Parser.hpp` code, and no new `CodeGen.cpp` branch:
the existing `IntLiteral` -> `OpCode::PushInt` pipeline, already
exercised by every plain integer literal in this codebase, covers it
completely. This mirrors the same reasoning already used for `||`/
`&&` reusing `BinaryExpr` instead of new dedicated node types: add a
new AST node only when the construct's runtime behavior genuinely
differs from something that already exists, not by default.

The alternative considered and rejected: a dedicated `CharLiteral`
node plus a new `TokenType::Char`, with the int conversion happening
in the parser instead of the lexer. This would work too, but adds a
node, a token type, and a parser branch that all do exactly what
`IntLiteral` already does, for no behavioral difference. Rejected as
pure duplication.

## VM changes (VM.cpp): extending the existing Index opcode

`CodeGen.cpp` needs no changes at all for string indexing. Its
`IndexExpr` branch already emits `target`, then `index`, then
`OpCode::Index` unconditionally, the same as it already does for
arrays and mappings; it has never inspected the target's runtime type,
that dispatch happens entirely inside the VM's `Index` case. So this
slice's only VM-level change is one new branch in that existing
`switch`/`if` chain in `VM.cpp`'s `case OpCode::Index`, alongside the
existing `Array` and `Mapping` branches, before the final
`else { throw LpcRuntimeError(...) }`:

```cpp
} else if (auto* str = std::get_if<std::string>(&targetVal.data)) {
    if (!std::holds_alternative<int64_t>(indexVal.data)) {
        throw LpcRuntimeError("Index: string index must be an integer");
    }
    int64_t i = std::get<int64_t>(indexVal.data);
    if (i < 0 || static_cast<size_t>(i) >= str->size()) {
        throw LpcRuntimeError("Index: string index out of bounds");
    }
    unsigned char ch = static_cast<unsigned char>((*str)[static_cast<size_t>(i)]);
    localStack.push_back(Value(static_cast<int64_t>(ch)));
}
```

This matches LPC semantics: `string[i]` yields the character at
position `i` as an int (its byte/ASCII value), not a one-character
substring, consistent with how `'#'` is itself an int after the lexer
change above, so `lines[i][0] == '#'` becomes an `int64_t` vs
`int64_t` comparison end to end, handled by the existing `Eq`/`Neq`
case and `valuesEqual()` with no changes needed there either. The
bounds check follows the exact same shape and error-message pattern
the `Array` branch immediately above it already uses
(`"Index: array index out of bounds"` -> `"Index: string index out of
bounds"`), and the integer-index type check mirrors
`"Index: array index must be an integer"` the same way.

## String indexing as an assignment target: confirmed out of scope

`IndexAssignStmt`/`OpCode::IndexAssign` already exist from the
arrays/mappings slice, and could in principle be extended the same
way (mutate the byte at position `i` in place). This is explicitly
not part of this slice: the Step 2 search above found zero occurrences
of `string[i] = ...` anywhere in master.c or master.h, only read-context
comparisons. Building it now would be speculative, the same reasoning
already applied to bitwise `|`/`&` in the logical-operators slice and
to non-empty array indexing assignment targets that never appeared.
If a future boot attempt or mudlib file surfaces a real need, it is a
small, self-contained follow-up: extend `IndexAssign`'s existing
`Array`/`Mapping` branches with a `std::string` case that also needs
to handle the fact that `std::string` is stored by value in the
`Value` variant, not by `shared_ptr` the way `Array`/`Mapping` are, so
in-place mutation through a copied `Value` would need its own design
pass rather than reusing the shared_ptr-aliasing behavior `IndexAssign`
currently relies on for arrays and mappings. Noting that distinction
here so it is not assumed to be a trivial copy-paste if picked up
later.

## Test plan (mirrors the prior three slices)

Unit tests in `test_lexer.cpp`:
- Lex `'#'`, `'/'`, `'a'`, `'z'` (the exact values found in master.c)
  and assert each produces a single `TokenType::Number` token whose
  text is the correct decimal ASCII value (`'#'` -> `"35"`, `'/'` ->
  `"47"`, `'a'` -> `"97"`, `'z'` -> `"122"`).
- Lex an escaped case not present in master.c but covered by the
  design, e.g. `'\n'`, and assert it produces `TokenType::Number`
  `"10"`, confirming the escape table works even though nothing in
  master.c currently exercises it.
- Assert a malformed character literal (`''` empty, or `'ab'` two
  characters) throws, matching the "exactly one character" LPC rule.
- Parse `x == '#'` and assert the right-hand side of the resulting
  `BinaryExpr` is an `IntLiteral` with `value == 35`, confirming no
  new AST node leaks into the tree, reuse of `IntLiteral` is real, not
  just true at the token level.
- A VM-level execution test: build a small `probe()` function
  equivalent to the real blocking line's shape, using a string local
  compared against `[0]` and a character literal, for both the match
  and no-match cases, e.g.
  `string s; s = "#comment"; return s[0] == '#';` returning `1`, and
  `s = "hello"; return s[0] == '#';` returning `0`.
- A VM-level bounds test: indexing an empty string at `0` throws
  `LpcRuntimeError`, matching the existing empty-array behavior, not a
  silent wrong answer.

`mudlib_stub` live-verification object: extend the existing
`guard_check.c`-style pattern (or a new sibling file) with a function
reproducing the real blocking line as closely as this slice now
allows, this time using an actual string and `[0]` instead of the
array-of-fields stand-in the logical-operators slice used (that
stand-in existed specifically because character literals and string
indexing did not exist yet; this slice removes the reason for it).
Something shaped like:
```c
void create() {
    mixed *lines;
    int i;

    lines = ({ "sword", "", "#comment" });
    i = 0;
    while (i < sizeof(lines)) {
        if (!lines[i] || lines[i] == "" || lines[i][0] == '#') {
            write("skip\n");
        } else {
            write("keep: " + lines[i] + "\n");
        }
        i = i + 1;
    }
}
```
Same live-boot verification style as the prior three slices:
temporarily wire this object into `master.c`'s `create()`, boot
against `mudlib_stub`, capture the transcript, then revert `master.c`.

## Sequencing after this slice

Two things confirmed still out of scope, unchanged from prior slices'
notes: `sscanf()` (arrays/mappings slice) and now also range/slice
indexing (`x[0..0]`, found above at seven call sites). Whichever of
these, or some other still-unseen construct, appears earliest in
master.c's token stream becomes the next wall; given range indexing's
`..`/`.` tokens are not handled by any existing whitelist addition
from this or prior slices, and given how early some of its seven call
sites appear in the file (line 215, well before several of this
slice's own remaining `'/'` sites), it is a reasonable guess for what
the eighth boot attempt will hit, but this is not confirmed until that
attempt actually runs.
