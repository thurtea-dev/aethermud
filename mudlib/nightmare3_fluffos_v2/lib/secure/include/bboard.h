/* NOTE (2026-07-27): this whole header is orphaned. Nothing includes
   <bboard.h>; both std/bboard.c and secure/daemon/bboard.c include their
   own local "bboard.h" (a quoted include resolves next to the .c file), so
   these macros are never read by anything.

   The path below used to say "/adm/save/boards/", a directory that does not
   exist on this lib. The real board save directory is DIR_BOARDS in
   secure/include/dirs.h, "/secure/save/boards", which is what
   secure/daemon/bboard.c actually reads and writes. Corrected here so the
   stale path cannot mislead anyone who finds this file; kept as a plain
   literal so the header stays self-contained. */
#define BBOARD_DIR 		"/secure/save/boards/"
#define BBOARD_EDIT		"/tmp/bb/"

#define BBOARD_OK		0
#define BAD_DATA		1
#define INVALID_MSG		2
#define ILLEGAL_ACCESS		3
#define EDIT_ABORT		4
