# login_accounts

Login account save files for the multi-character account system.

Each account is stored as `/secure/save/login_accounts/<letter>/<account>.o`
and holds the account password plus a list of up to five character names.
Character bodies still live under `/secure/save/users/`.

Managed by `/secure/daemon/account_d.c`. Do not edit these files by hand.
