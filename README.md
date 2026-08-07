# AetherMUD

AetherMUD is a custom C++ LPC driver.

The driver is written from scratch against the
FluffOS 2.9-ds2.08 reference driver's semantics (grammar, compiler, and
runtime behavior), with every language feature implemented and tested
against the actual mudlib rather than a synthetic test bed.

Current status: the driver boots the real mudlib through account creation
via live raw-socket testing. See docs/STATUS.md for the full running log of
what is implemented, what is confirmed working, and what remains a known
stub or scope limitation.

## Build instructions

    cmake -B driver/build -S driver
    cmake --build driver/build
    driver/build/lpcdriver etc/driver.cfg
