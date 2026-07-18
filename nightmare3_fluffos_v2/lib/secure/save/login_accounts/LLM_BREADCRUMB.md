# LLM breadcrumb: login_accounts

1. Read `/home/thurtea/aethermud/CLAUDE.md` for project rules.
2. Read `README.md` in this directory.
3. Parent: `../LLM_BREADCRUMB.md` (or `secure/save` docs).
4. Related: `/secure/daemon/account_d.c`, `/secure/std/login.c`.

## Gotchas

- Not the bank account directory (`/secure/save/accounts/`).
- Password hashes live here; never expose paths to mortals.
- Max 5 characters per account (`MAX_ACCOUNT_CHARS` in account_d).
