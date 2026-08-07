<!-- Regenerate with: tree --dirsfirst -I 'deadsouls' . > aethermud-directory.md
     (append a blank line, then echo "Generated on $(date)" >> the file).
     -I 'deadsouls' excludes zip-files/deadsouls/, the one extracted
     third-party reference copy under zip-files/ (5,137 files, more than
     4x the rest of the repo combined) - it would otherwise drown this
     snapshot. The six zip-files/*.zip archives themselves are not
     expanded by tree regardless and still show as single-line entries.
     Add further -I excludes here if another vendored tree ever gets
     extracted into the repo; do not blindly rerun without checking. -->
.
├── docs
│   ├── reference
│   │   ├── information.md
│   │   ├── invocations.md
│   │   ├── LLM_BREADCRUMB.md
│   │   ├── README.md
│   │   └── reference-chart.md
│   ├── admin-thurtea-checklist.md
│   ├── ASSESSMENT.md
│   ├── DEVELOPMENT.md
│   ├── INSTALL.md
│   ├── legacy-riftsmud-reference.md
│   ├── lib-cleanup-manifest.md
│   ├── LLM_BREADCRUMB.md
│   ├── mercenary-groups-plan.md
│   ├── playtest-checklist.md
│   ├── playtest-checklist-players.md
│   ├── playtest-checklist-wizards.md
│   ├── playtest-roster.md
│   ├── PUBLISHING.md
│   ├── README.md
│   ├── RUNNING.md
│   ├── SHARING.md
│   ├── starting-equipment-audit.md
│   ├── STYLE.md
│   └── zone-expansion-plan.md
├── internal
│   ├── install-aethermud.md
│   ├── LLM_BREADCRUMB.md
│   ├── README.md
│   ├── report.md
│   ├── riftsmud-memories.md
│   ├── tomorrow.md
│   └── TRANSFER-CHECKLIST.md
├── nightmare3_fluffos_v2
│   ├── attic
│   │   ├── cmds
│   │   │   ├── database
│   │   │   │   └── soul.db
│   │   │   ├── mortal
│   │   │   │   └── _start.c
│   │   │   ├── skills
│   │   │   │   ├── _absorb.c
│   │   │   │   ├── _annihilate.c
│   │   │   │   ├── _blast.c
│   │   │   │   ├── _bless.c
│   │   │   │   ├── _boost.c
│   │   │   │   └── _warshout.c
│   │   │   └── soul
│   │   │       └── atmos.c
│   │   ├── domains
│   │   │   ├── adm
│   │   │   │   └── vehicle_test
│   │   │   │       ├── vt_east.c
│   │   │   │       ├── vt_ex5_pad.c
│   │   │   │       ├── vt_main.c
│   │   │   │       ├── vt_north.c
│   │   │   │       └── vt_south.c
│   │   │   ├── Examples
│   │   │   │   ├── doc
│   │   │   │   ├── etc
│   │   │   │   │   ├── chest.c
│   │   │   │   │   ├── flu.c
│   │   │   │   │   ├── match.c
│   │   │   │   │   └── torch.c
│   │   │   │   ├── room
│   │   │   │   │   ├── search.c
│   │   │   │   │   ├── simple.c
│   │   │   │   │   └── simple_search.c
│   │   │   │   ├── virtual
│   │   │   │   │   ├── exaA
│   │   │   │   │   │   └── daemon.c
│   │   │   │   │   ├── exaB
│   │   │   │   │   │   ├── daemon.c
│   │   │   │   │   │   ├── defines.h
│   │   │   │   │   │   ├── descs.df
│   │   │   │   │   │   ├── exceptions.c
│   │   │   │   │   │   ├── exits.db
│   │   │   │   │   │   └── terrs.db
│   │   │   │   │   ├── exaC
│   │   │   │   │   ├── exaA_server.c
│   │   │   │   │   ├── exaB_server.c
│   │   │   │   │   ├── explanation.t
│   │   │   │   │   ├── README
│   │   │   │   │   ├── start.c
│   │   │   │   │   └── virtual
│   │   │   │   └── weapon
│   │   │   │       ├── curse_sword.c
│   │   │   │       └── skill_sword.c
│   │   │   └── Praxis
│   │   │       ├── attic
│   │   │       │   └── class_change.c
│   │   │       ├── cemetary
│   │   │       │   └── mon
│   │   │       │       ├── ghost9.c
│   │   │       │       └── ghosta.c
│   │   │       ├── obj
│   │   │       │   ├── misc
│   │   │       │   │   └── stone_pile.c
│   │   │       │   └── mon
│   │   │       │       ├── christmas_bunny.c
│   │   │       │       ├── easter_bunny.c
│   │   │       │       ├── execution_d.c
│   │   │       │       └── goblin_fighter.c
│   │   │       ├── standardOld
│   │   │       │   ├── adv_inner.c
│   │   │       │   ├── adv_main.c
│   │   │       │   ├── alley1.c
│   │   │       │   ├── alley2.c
│   │   │       │   ├── app_room.c
│   │   │       │   ├── arch.c
│   │   │       │   ├── bank.c
│   │   │       │   ├── bank.c~
│   │   │       │   ├── bank_vault.c
│   │   │       │   ├── booth.c
│   │   │       │   ├── branches.c
│   │   │       │   ├── building.c
│   │   │       │   ├── cache.c
│   │   │       │   ├── chapel.c
│   │   │       │   ├── cleric_hall.c
│   │   │       │   ├── cleric_join.c
│   │   │       │   ├── cleric_vote.c
│   │   │       │   ├── council.c
│   │   │       │   ├── council_hall.c
│   │   │       │   ├── council_vote.c
│   │   │       │   ├── court_room.c
│   │   │       │   ├── crypt.c
│   │   │       │   ├── dump.c
│   │   │       │   ├── east_road1.c
│   │   │       │   ├── east_road2.c
│   │   │       │   ├── east_road3.c
│   │   │       │   ├── e_boc_la1.c
│   │   │       │   ├── e_boc_la2.c
│   │   │       │   ├── e_boc_la3.c
│   │   │       │   ├── farm.c
│   │   │       │   ├── fighter_hall.c
│   │   │       │   ├── fighter_join.c
│   │   │       │   ├── fighter_vote.c
│   │   │       │   ├── forest1.c
│   │   │       │   ├── forest2.c
│   │   │       │   ├── forest3.c
│   │   │       │   ├── freezer.c
│   │   │       │   ├── hall2.c
│   │   │       │   ├── hall3.c
│   │   │       │   ├── hall4.c
│   │   │       │   ├── hall5.c
│   │   │       │   ├── hall.c
│   │   │       │   ├── highway1.c
│   │   │       │   ├── highway2.c
│   │   │       │   ├── highway3.bak
│   │   │       │   ├── highway3.c
│   │   │       │   ├── hm_chamber.c
│   │   │       │   ├── hole.c
│   │   │       │   ├── hospital.c
│   │   │       │   ├── hotel.c
│   │   │       │   ├── house.c
│   │   │       │   ├── idle_storage.c
│   │   │       │   ├── idle_supply.c
│   │   │       │   ├── immortal_hall.c
│   │   │       │   ├── inn101.c
│   │   │       │   ├── inn102.c
│   │   │       │   ├── inn103.c
│   │   │       │   ├── inn104.c
│   │   │       │   ├── inn105.c
│   │   │       │   ├── inn106.c
│   │   │       │   ├── inn107.c
│   │   │       │   ├── inn108.c
│   │   │       │   ├── inn109.c
│   │   │       │   ├── inn110.c
│   │   │       │   ├── jungle.c
│   │   │       │   ├── kataan_hall.c
│   │   │       │   ├── kataan_join.c
│   │   │       │   ├── kataan_vote.c
│   │   │       │   ├── law.c
│   │   │       │   ├── library.c
│   │   │       │   ├── locked.c
│   │   │       │   ├── lpc_inner.c
│   │   │       │   ├── lpmud_room.c
│   │   │       │   ├── mage_hall.bak
│   │   │       │   ├── mage_hall.c
│   │   │       │   ├── mage_join.c
│   │   │       │   ├── mage_vote.c
│   │   │       │   ├── medium.c
│   │   │       │   ├── monastery.c
│   │   │       │   ├── monk_hall.c
│   │   │       │   ├── monk_join.c
│   │   │       │   ├── monk_vote.c
│   │   │       │   ├── mudlib.c
│   │   │       │   ├── n_centre1.c
│   │   │       │   ├── n_centre2.c
│   │   │       │   ├── ombud_hall.c
│   │   │       │   ├── party.c
│   │   │       │   ├── pass1.c
│   │   │       │   ├── pass2.c
│   │   │       │   ├── pass3.c
│   │   │       │   ├── pier1.c
│   │   │       │   ├── pit.c
│   │   │       │   ├── planning_room.c
│   │   │       │   ├── post.c
│   │   │       │   ├── project_room.c
│   │   │       │   ├── pub.c
│   │   │       │   ├── quest_room.c
│   │   │       │   ├── rain_forest.c
│   │   │       │   ├── realty.c
│   │   │       │   ├── restaurant.c
│   │   │       │   ├── rogue_hall.c
│   │   │       │   ├── rogue_join.c
│   │   │       │   ├── rogue_vote.c
│   │   │       │   ├── roots.c
│   │   │       │   ├── sage_room.c
│   │   │       │   ├── s_centre1.c
│   │   │       │   ├── s_centre2.c
│   │   │       │   ├── s_centre3.c
│   │   │       │   ├── s_centre4.c
│   │   │       │   ├── setter.c
│   │   │       │   ├── sewer.c
│   │   │       │   ├── sewer_ent.c
│   │   │       │   ├── sheriff.c
│   │   │       │   ├── spider_pit.c
│   │   │       │   ├── square.c
│   │   │       │   ├── square.c~
│   │   │       │   ├── stairs.c
│   │   │       │   ├── storage.c
│   │   │       │   ├── sun1.c
│   │   │       │   ├── sun2.c
│   │   │       │   ├── supply2.c
│   │   │       │   ├── supply.c
│   │   │       │   ├── torture_room.c
│   │   │       │   ├── town_hall.c
│   │   │       │   ├── trunk.c
│   │   │       │   ├── unnamed1.c
│   │   │       │   ├── void.c
│   │   │       │   ├── wall.c
│   │   │       │   ├── w_boc_la1.c
│   │   │       │   ├── w_boc_la2.c
│   │   │       │   ├── w_boc_la3.c
│   │   │       │   ├── west_road1.c
│   │   │       │   ├── west_road2.c
│   │   │       │   ├── west_road3.c
│   │   │       │   ├── wild1.c
│   │   │       │   ├── wild2.backup
│   │   │       │   ├── wild2.c
│   │   │       │   └── yard.c
│   │   │       ├── council_vote.c
│   │   │       ├── idle_storage.c
│   │   │       └── project_room.c
│   │   ├── tmp
│   │   │   └── test_load.c
│   │   ├── LLM_BREADCRUMB.md
│   │   └── README.md
│   ├── bin
│   │   ├── addr_server
│   │   ├── driver
│   │   ├── LLM_BREADCRUMB.md
│   │   ├── mudos.cfg
│   │   ├── mudos.cfg.example
│   │   ├── mudos.cfg.win32
│   │   ├── portbind
│   │   └── README.md
│   ├── fluffos-2.9-ds2.08
│   │   ├── ChangeLog.old
│   │   │   ├── 0.8.gz
│   │   │   ├── 0.9-0.9.13.gz
│   │   │   ├── 0.9.14.gz
│   │   │   ├── 0.9.15.gz
│   │   │   ├── 0.9.16.gz
│   │   │   ├── 0.9.17.gz
│   │   │   ├── 0.9.18.gz
│   │   │   ├── 0.9.19.gz
│   │   │   └── ChangeLog.v20.gz
│   │   ├── compat
│   │   │   ├── simuls
│   │   │   │   ├── apply.c
│   │   │   │   ├── #break_string.c#
│   │   │   │   ├── break_string.c
│   │   │   │   ├── dump_socket_status.c
│   │   │   │   ├── parse_command.c
│   │   │   │   ├── process_string.c
│   │   │   │   └── tail.c
│   │   │   ├── fixstatic
│   │   │   └── parser_error.c
│   │   ├── include
│   │   │   ├── function.h
│   │   │   ├── localtime.h
│   │   │   ├── origin.h
│   │   │   ├── parser_error.h
│   │   │   ├── runtime_config.h
│   │   │   ├── semantic.cache
│   │   │   ├── socket_err.h
│   │   │   └── type.h
│   │   ├── obj
│   │   │   ├── add_action.o
│   │   │   ├── addr_server.o
│   │   │   ├── applies_table.o
│   │   │   ├── array.o
│   │   │   ├── avltree.o
│   │   │   ├── backend.o
│   │   │   ├── binaries.o
│   │   │   ├── buffer.o
│   │   │   ├── call_out.o
│   │   │   ├── class.o
│   │   │   ├── comm.o
│   │   │   ├── compiler.o
│   │   │   ├── contrib.o
│   │   │   ├── crc32.o
│   │   │   ├── crypt.o
│   │   │   ├── debug.o
│   │   │   ├── develop.o
│   │   │   ├── disassembler.o
│   │   │   ├── dumpstat.o
│   │   │   ├── edit_source.o
│   │   │   ├── ed.o
│   │   │   ├── efuns_main.o
│   │   │   ├── efuns_port.o
│   │   │   ├── eoperators.o
│   │   │   ├── eval.o
│   │   │   ├── file.o
│   │   │   ├── fliconv.o
│   │   │   ├── function.o
│   │   │   ├── generate.o
│   │   │   ├── grammar.tab.o
│   │   │   ├── hash.o
│   │   │   ├── icode.o
│   │   │   ├── interpret.o
│   │   │   ├── lex.o
│   │   │   ├── main.o
│   │   │   ├── make_func.tab.o
│   │   │   ├── malloc.o
│   │   │   ├── mallocwrapper.o
│   │   │   ├── mapping.o
│   │   │   ├── master.o
│   │   │   ├── math.o
│   │   │   ├── matrix.o
│   │   │   ├── md.o
│   │   │   ├── mudlib_stats.o
│   │   │   ├── object.o
│   │   │   ├── otable.o
│   │   │   ├── parse.o
│   │   │   ├── parser.o
│   │   │   ├── portbind.o
│   │   │   ├── port.o
│   │   │   ├── program.o
│   │   │   ├── qsort.o
│   │   │   ├── rc.o
│   │   │   ├── reclaim.o
│   │   │   ├── regexp.o
│   │   │   ├── replace_program.o
│   │   │   ├── scratchpad.o
│   │   │   ├── simulate.o
│   │   │   ├── simul_efun.o
│   │   │   ├── socket_ctrl.o
│   │   │   ├── socket_efuns.o
│   │   │   ├── socket_err.o
│   │   │   ├── sockets.o
│   │   │   ├── sprintf.o
│   │   │   ├── stralloc.o
│   │   │   ├── trees.o
│   │   │   └── uvalarm.o
│   │   ├── packages
│   │   │   ├── async.c
│   │   │   ├── async.h
│   │   │   ├── async_spec.c
│   │   │   ├── compress.c
│   │   │   ├── compress_spec.c
│   │   │   ├── contrib.c
│   │   │   ├── contrib_spec.c
│   │   │   ├── db.c
│   │   │   ├── db.h
│   │   │   ├── db_spec.c
│   │   │   ├── develop.c
│   │   │   ├── develop_spec.c
│   │   │   ├── dwlib.c
│   │   │   ├── dwlib_spec.c
│   │   │   ├── external.c
│   │   │   ├── external_spec.c
│   │   │   ├── GNUmakefile
│   │   │   ├── GNUmakefile.pre
│   │   │   ├── Makefile
│   │   │   ├── Makefile.master
│   │   │   ├── Makefile.pre
│   │   │   ├── math.c
│   │   │   ├── math_spec.c
│   │   │   ├── matrix.c
│   │   │   ├── matrix.h
│   │   │   ├── matrix_spec.c
│   │   │   ├── mudlib_stats.c
│   │   │   ├── mudlib_stats.h
│   │   │   ├── mudlib_stats_spec.c
│   │   │   ├── packages
│   │   │   ├── packages.a
│   │   │   ├── parser.c
│   │   │   ├── parser.h
│   │   │   ├── parser_spec.c
│   │   │   ├── readme
│   │   │   ├── sockets.c
│   │   │   ├── sockets_spec.c
│   │   │   ├── uids.c
│   │   │   ├── uids.h
│   │   │   └── uids_spec.c
│   │   ├── testsuite
│   │   │   ├── clone
│   │   │   │   ├── login.c
│   │   │   │   ├── readme
│   │   │   │   └── user.c
│   │   │   ├── command
│   │   │   │   ├── codefor.c
│   │   │   │   ├── dest.c
│   │   │   │   ├── ed.c
│   │   │   │   ├── eval.c
│   │   │   │   ├── quit.c
│   │   │   │   ├── readme
│   │   │   │   ├── rm.c
│   │   │   │   ├── say.c
│   │   │   │   ├── shutdown.c
│   │   │   │   ├── speed.c
│   │   │   │   ├── tests.c
│   │   │   │   ├── update.c
│   │   │   │   └── who.c
│   │   │   ├── data
│   │   │   │   └── readme
│   │   │   ├── etc
│   │   │   │   ├── config.test
│   │   │   │   └── motd
│   │   │   ├── include
│   │   │   │   ├── command.h
│   │   │   │   ├── config.h
│   │   │   │   ├── globals.h
│   │   │   │   ├── lpctypes.h
│   │   │   │   ├── readme
│   │   │   │   └── tests.h
│   │   │   ├── inherit
│   │   │   │   ├── master
│   │   │   │   │   └── valid.c
│   │   │   │   ├── base.c
│   │   │   │   ├── clean_up.c
│   │   │   │   ├── readme
│   │   │   │   └── tests.c
│   │   │   ├── log
│   │   │   │   ├── author_stats
│   │   │   │   ├── domain_stats
│   │   │   │   └── readme
│   │   │   ├── single
│   │   │   │   ├── tests
│   │   │   │   │   ├── compiler
│   │   │   │   │   │   ├── fail
│   │   │   │   │   │   │   ├── arr_index.c
│   │   │   │   │   │   │   ├── bad_break.c
│   │   │   │   │   │   │   ├── bad_continue.c
│   │   │   │   │   │   │   ├── bad_foreach.c
│   │   │   │   │   │   │   ├── bad_for_init.c
│   │   │   │   │   │   │   ├── bad_init.c
│   │   │   │   │   │   │   ├── bad_local_init2.c
│   │   │   │   │   │   │   ├── bad_local_init.c
│   │   │   │   │   │   │   ├── bad_param2.c
│   │   │   │   │   │   │   ├── bad_param.c
│   │   │   │   │   │   │   ├── bad_return2.c
│   │   │   │   │   │   │   ├── bad_return.c
│   │   │   │   │   │   │   ├── bad_type_init.c
│   │   │   │   │   │   │   ├── class_redef.c
│   │   │   │   │   │   │   ├── div_zero3.c
│   │   │   │   │   │   │   ├── div_zero.c
│   │   │   │   │   │   │   ├── eof_in_comment.c
│   │   │   │   │   │   │   ├── eof_in_string.c
│   │   │   │   │   │   │   ├── local_redef2.c
│   │   │   │   │   │   │   ├── local_redef.c
│   │   │   │   │   │   │   ├── lvar_func2.c
│   │   │   │   │   │   │   ├── lvar_func.c
│   │   │   │   │   │   │   ├── missing_endif.c
│   │   │   │   │   │   │   ├── missing_type.c
│   │   │   │   │   │   │   ├── mixed_case.c
│   │   │   │   │   │   │   ├── mod_zero.c
│   │   │   │   │   │   │   ├── no_member.c
│   │   │   │   │   │   │   ├── no_ret_type.c
│   │   │   │   │   │   │   ├── no_such_class.c
│   │   │   │   │   │   │   ├── predef.c
│   │   │   │   │   │   │   ├── redef_function.c
│   │   │   │   │   │   │   ├── static_local.c
│   │   │   │   │   │   │   ├── string_case.c
│   │   │   │   │   │   │   ├── two_default.c
│   │   │   │   │   │   │   ├── undef_class.c
│   │   │   │   │   │   │   ├── var_to_nonvar.c
│   │   │   │   │   │   │   ├── void_arg2.c
│   │   │   │   │   │   │   ├── void_arg.c
│   │   │   │   │   │   │   ├── void_for_var.c
│   │   │   │   │   │   │   ├── void_global.c
│   │   │   │   │   │   │   ├── void_local.c
│   │   │   │   │   │   │   └── void_member.c
│   │   │   │   │   │   ├── at_block.c
│   │   │   │   │   │   ├── constant_expr.c
│   │   │   │   │   │   └── succeed.c
│   │   │   │   │   ├── efuns
│   │   │   │   │   │   ├── add_action.c
│   │   │   │   │   │   ├── add_light.c
│   │   │   │   │   │   ├── all_inventory.c
│   │   │   │   │   │   ├── allocate_buffer.c
│   │   │   │   │   │   ├── allocate.c
│   │   │   │   │   │   ├── allocate_mapping.c
│   │   │   │   │   │   ├── badshad.c
│   │   │   │   │   │   ├── bind.c
│   │   │   │   │   │   ├── break_string.c
│   │   │   │   │   │   ├── cache_stats.c
│   │   │   │   │   │   ├── call_other.c
│   │   │   │   │   │   ├── call_out.c
│   │   │   │   │   │   ├── call_stack.c
│   │   │   │   │   │   ├── capitalize.c
│   │   │   │   │   │   ├── children.c
│   │   │   │   │   │   ├── clear_bit.c
│   │   │   │   │   │   ├── clone_object.c
│   │   │   │   │   │   ├── clonep.c
│   │   │   │   │   │   ├── command.c
│   │   │   │   │   │   ├── cp.c
│   │   │   │   │   │   ├── crc32.c
│   │   │   │   │   │   ├── ctime.c
│   │   │   │   │   │   ├── deep_inherit_list.c
│   │   │   │   │   │   ├── deep_inventory.c
│   │   │   │   │   │   ├── destruct.c
│   │   │   │   │   │   ├── disable_commands.c
│   │   │   │   │   │   ├── dumpallobj.c
│   │   │   │   │   │   ├── ed.c
│   │   │   │   │   │   ├── enable_wizard.c
│   │   │   │   │   │   ├── environment.c
│   │   │   │   │   │   ├── error.c
│   │   │   │   │   │   ├── exec.c
│   │   │   │   │   │   ├── explode.c
│   │   │   │   │   │   ├── file_name.c
│   │   │   │   │   │   ├── file_size.c
│   │   │   │   │   │   ├── filter.c
│   │   │   │   │   │   ├── find_call_out.c
│   │   │   │   │   │   ├── find_living.c
│   │   │   │   │   │   ├── find_object.c
│   │   │   │   │   │   ├── find_player.c
│   │   │   │   │   │   ├── function_exists.c
│   │   │   │   │   │   ├── functionp.c
│   │   │   │   │   │   ├── function_profile.c
│   │   │   │   │   │   ├── generate_source.c
│   │   │   │   │   │   ├── get_char.c
│   │   │   │   │   │   ├── get_config.c
│   │   │   │   │   │   ├── get_dir.c
│   │   │   │   │   │   ├── goodshad.c
│   │   │   │   │   │   ├── has_errors.c
│   │   │   │   │   │   ├── implode.c
│   │   │   │   │   │   ├── in_edit.c
│   │   │   │   │   │   ├── inh0.c
│   │   │   │   │   │   ├── inh1.c
│   │   │   │   │   │   ├── inh2.c
│   │   │   │   │   │   ├── inherit_list.c
│   │   │   │   │   │   ├── inherits.c
│   │   │   │   │   │   ├── input_to.c
│   │   │   │   │   │   ├── interactive.c
│   │   │   │   │   │   ├── intp.c
│   │   │   │   │   │   ├── keys.c
│   │   │   │   │   │   ├── light.c
│   │   │   │   │   │   ├── living.c
│   │   │   │   │   │   ├── livings.c
│   │   │   │   │   │   ├── lower_case.c
│   │   │   │   │   │   ├── malloc_status.c
│   │   │   │   │   │   ├── map.c
│   │   │   │   │   │   ├── map_delete.c
│   │   │   │   │   │   ├── mapp.c
│   │   │   │   │   │   ├── master.c
│   │   │   │   │   │   ├── match_path.c
│   │   │   │   │   │   ├── member_array.c
│   │   │   │   │   │   ├── message.c
│   │   │   │   │   │   ├── mkdir.c
│   │   │   │   │   │   ├── move_object.c
│   │   │   │   │   │   ├── mud_status.c
│   │   │   │   │   │   ├── notify_fail.c
│   │   │   │   │   │   ├── nullp.c
│   │   │   │   │   │   ├── objectp.c
│   │   │   │   │   │   ├── opcprof.c
│   │   │   │   │   │   ├── origin.c
│   │   │   │   │   │   ├── pointerp.c
│   │   │   │   │   │   ├── present.c
│   │   │   │   │   │   ├── previous_object.c
│   │   │   │   │   │   ├── printf.c
│   │   │   │   │   │   ├── query_ed_mode.c
│   │   │   │   │   │   ├── query_idle.c
│   │   │   │   │   │   ├── query_ip_name.c
│   │   │   │   │   │   ├── query_ip_number.c
│   │   │   │   │   │   ├── query_load_average.c
│   │   │   │   │   │   ├── query_privs.c
│   │   │   │   │   │   ├── query_snoop.c
│   │   │   │   │   │   ├── query_snooping.c
│   │   │   │   │   │   ├── query_verb.c
│   │   │   │   │   │   ├── random.c
│   │   │   │   │   │   ├── read_buffer.c
│   │   │   │   │   │   ├── read_bytes.c
│   │   │   │   │   │   ├── read_file.c
│   │   │   │   │   │   ├── readme
│   │   │   │   │   │   ├── receive.c
│   │   │   │   │   │   ├── reg_assoc.c
│   │   │   │   │   │   ├── regexp.c
│   │   │   │   │   │   ├── remove_action.c
│   │   │   │   │   │   ├── remove_call_out.c
│   │   │   │   │   │   ├── rename.c
│   │   │   │   │   │   ├── replace_string.c
│   │   │   │   │   │   ├── restore_object.c
│   │   │   │   │   │   ├── restore_variable.c
│   │   │   │   │   │   ├── rm.c
│   │   │   │   │   │   ├── rmdir.c
│   │   │   │   │   │   ├── save_object.c
│   │   │   │   │   │   ├── save_variable.c
│   │   │   │   │   │   ├── say.c
│   │   │   │   │   │   ├── set_bit.c
│   │   │   │   │   │   ├── set_debug_level.c
│   │   │   │   │   │   ├── set_heart_beat.c
│   │   │   │   │   │   ├── set_hide.c
│   │   │   │   │   │   ├── set_light.c
│   │   │   │   │   │   ├── set_living_name.c
│   │   │   │   │   │   ├── set_privs.c
│   │   │   │   │   │   ├── shadow.c
│   │   │   │   │   │   ├── shutdown.c
│   │   │   │   │   │   ├── sizeof.c
│   │   │   │   │   │   ├── snoop.c
│   │   │   │   │   │   ├── sprintf.c
│   │   │   │   │   │   ├── sscanf.c
│   │   │   │   │   │   ├── talker.c
│   │   │   │   │   │   ├── to_float.c
│   │   │   │   │   │   ├── to_int.c
│   │   │   │   │   │   ├── unloaded.c
│   │   │   │   │   │   ├── users.c
│   │   │   │   │   │   └── values.c
│   │   │   │   │   └── operators
│   │   │   │   │       └── range.c
│   │   │   │   ├── inh.c
│   │   │   │   ├── master.c
│   │   │   │   ├── readme
│   │   │   │   ├── simul_efun.c
│   │   │   │   └── void.c
│   │   │   ├── tmp
│   │   │   │   └── bar
│   │   │   ├── u
│   │   │   │   └── readme
│   │   │   ├── readme
│   │   │   └── test_control.c
│   │   ├── tmp
│   │   │   └── insttest
│   │   ├── Win32
│   │   │   ├── ar.c
│   │   │   ├── configure.h
│   │   │   ├── cp.bat
│   │   │   ├── mv.bat
│   │   │   ├── rm.bat
│   │   │   └── touch.c
│   │   ├── windows
│   │   │   ├── addr_server.mak
│   │   │   ├── clean.bat
│   │   │   ├── configure.h
│   │   │   ├── edit_source.mak
│   │   │   ├── MudOS.mak
│   │   │   ├── README.win32
│   │   │   ├── step1.bat
│   │   │   ├── stepdbg.bat
│   │   │   └── steprel.bat
│   │   ├── 1.out
│   │   ├── add_action.c
│   │   ├── add_action.h
│   │   ├── addr_server
│   │   ├── addr_server.c
│   │   ├── addr_server.h
│   │   ├── applies
│   │   ├── applies.h
│   │   ├── applies_table.c
│   │   ├── arch.h
│   │   ├── array.c
│   │   ├── array.h
│   │   ├── Artistic.README
│   │   ├── avltree.c
│   │   ├── avltree.h
│   │   ├── backend.c
│   │   ├── backend.h
│   │   ├── binaries.c
│   │   ├── binaries.h
│   │   ├── bsdmalloc.c
│   │   ├── bsdmalloc.h
│   │   ├── buffer.c
│   │   ├── buffer.h
│   │   ├── Bugs
│   │   ├── build.MudOS
│   │   ├── buildMudOS.bat
│   │   ├── call_out.c
│   │   ├── call_out.h
│   │   ├── cc.h
│   │   ├── ChangeLog
│   │   ├── ChangeLog.alpha
│   │   ├── ChangeLog.beta
│   │   ├── ChangeLog.fluffos
│   │   ├── ChangeLog.fluffos-2.x
│   │   ├── class.c
│   │   ├── class.h
│   │   ├── comm.c
│   │   ├── comm.h
│   │   ├── compiler.c
│   │   ├── compiler.h
│   │   ├── comptest
│   │   ├── comptest.c
│   │   ├── Config.example
│   │   ├── config.h
│   │   ├── configuration
│   │   ├── configure
│   │   ├── configure.h
│   │   ├── Copyright
│   │   ├── crc32.c
│   │   ├── crc32.h
│   │   ├── crctab.h
│   │   ├── Credits.LPmud
│   │   ├── Credits.MudOS
│   │   ├── crypt.c
│   │   ├── crypt.h
│   │   ├── debug.c
│   │   ├── debug.h
│   │   ├── debugmalloc.c
│   │   ├── debugmalloc.h
│   │   ├── Dependencies
│   │   ├── diff2.txt
│   │   ├── disassembler.c
│   │   ├── driver
│   │   ├── driver.old
│   │   ├── dumpstat.c
│   │   ├── dumpstat.h
│   │   ├── ed.c
│   │   ├── ed.h
│   │   ├── edit_source
│   │   ├── edit_source.c
│   │   ├── edit_source.h
│   │   ├── efunctions.h
│   │   ├── efun_defs.c
│   │   ├── efun_protos.h
│   │   ├── efuns_incl.h
│   │   ├── efuns_main.c
│   │   ├── efuns_main.h
│   │   ├── efuns_port.c
│   │   ├── eoperators.c
│   │   ├── eoperators.h
│   │   ├── eval.c
│   │   ├── eval.h
│   │   ├── Exit_Codes
│   │   ├── file.c
│   │   ├── file.h
│   │   ├── file_incl.h
│   │   ├── files
│   │   ├── fliconv.c
│   │   ├── fliconv.h
│   │   ├── func_spec.c
│   │   ├── func_spec.cpp
│   │   ├── function.c
│   │   ├── function.h
│   │   ├── generate.c
│   │   ├── generate.h
│   │   ├── GNUmakefile
│   │   ├── GNUmakefile.debug
│   │   ├── GNUmakefile.in
│   │   ├── GNUmakefile.keep
│   │   ├── gnumalloc.c
│   │   ├── gnumalloc.h
│   │   ├── grammar.tab.c
│   │   ├── grammar.tab.h
│   │   ├── grammar.y
│   │   ├── grammar.y.pre
│   │   ├── hash.c
│   │   ├── hash.h
│   │   ├── icode.c
│   │   ├── icode.h
│   │   ├── Ideas
│   │   ├── INSTALL
│   │   ├── insttest
│   │   ├── interpret.c
│   │   ├── interpret.h
│   │   ├── IT_CRASHED
│   │   ├── lex.c
│   │   ├── lex.h
│   │   ├── lint.h
│   │   ├── LLM_BREADCRUMB.md
│   │   ├── local_options
│   │   ├── local_options.ds
│   │   ├── local_options.dw
│   │   ├── local_options.dwclone
│   │   ├── local_options.dw_orig
│   │   ├── local_options.generic
│   │   ├── local_options.lima
│   │   ├── local_options.lpuni
│   │   ├── local_options.merentha
│   │   ├── local_options.nm3
│   │   ├── local_options.skylib
│   │   ├── local_options.test
│   │   ├── local_options.tmi2
│   │   ├── lpc.h
│   │   ├── lpc_incl.h
│   │   ├── macros.h
│   │   ├── main.c
│   │   ├── main.h
│   │   ├── Makefile
│   │   ├── Makefile.in
│   │   ├── Makefile.in.pre
│   │   ├── make_func.h
│   │   ├── make_func.tab.c
│   │   ├── make_func.y
│   │   ├── make_malloc.c
│   │   ├── malloc.c
│   │   ├── malloc.h
│   │   ├── malloc_incl.h
│   │   ├── mallocwrapper.c
│   │   ├── mapping.c
│   │   ├── mapping.h
│   │   ├── master.c
│   │   ├── master.h
│   │   ├── md.c
│   │   ├── md.h
│   │   ├── mmalloc.c
│   │   ├── my_malloc.h
│   │   ├── network_incl.h
│   │   ├── NMakefile.in
│   │   ├── NMakefile.in.pre
│   │   ├── object.c
│   │   ├── object.h
│   │   ├── opc.h
│   │   ├── opcodes.h
│   │   ├── op_spec.c
│   │   ├── option_defs.c
│   │   ├── options.h
│   │   ├── options_incl.h
│   │   ├── origin.h
│   │   ├── otable.c
│   │   ├── otable.h
│   │   ├── parse.c
│   │   ├── parse.h
│   │   ├── parse_trees
│   │   ├── patchlevel.h
│   │   ├── Performance
│   │   ├── plainwrapper.c
│   │   ├── Platforms
│   │   ├── portability.h
│   │   ├── portbind
│   │   ├── portbind.c
│   │   ├── port.c
│   │   ├── port.h
│   │   ├── preprocess.c
│   │   ├── preprocess.h
│   │   ├── program.c
│   │   ├── program.h
│   │   ├── qsort.c
│   │   ├── qsort.h
│   │   ├── rc.c
│   │   ├── rc.h
│   │   ├── README
│   │   ├── README.md
│   │   ├── reclaim.c
│   │   ├── reclaim.h
│   │   ├── regexp.c
│   │   ├── regexp.h
│   │   ├── replace_program.c
│   │   ├── replace_program.h
│   │   ├── scratchpad.c
│   │   ├── scratchpad.h
│   │   ├── simulate.c
│   │   ├── simulate.h
│   │   ├── simul_efun.c
│   │   ├── simul_efun.h
│   │   ├── smalloc.c
│   │   ├── smalloc.h
│   │   ├── socket_ctrl.c
│   │   ├── socket_ctrl.h
│   │   ├── socket_efuns.c
│   │   ├── socket_efuns.h
│   │   ├── socket_err.c
│   │   ├── socket_err.h
│   │   ├── spec.h
│   │   ├── sprintf.c
│   │   ├── sprintf.h
│   │   ├── std.h
│   │   ├── std_incl.h
│   │   ├── stralloc.c
│   │   ├── stralloc.h
│   │   ├── strfuncs.c
│   │   ├── sysmalloc.c
│   │   ├── system_libs
│   │   ├── telnet.h
│   │   ├── trees.c
│   │   ├── trees.h
│   │   ├── uvalarm.c
│   │   ├── uvalarm.h
│   │   ├── Version
│   │   ├── wrappedmalloc.c
│   │   ├── wrappedmalloc.h
│   │   └── y.tab.c
│   ├── lib
│   │   ├── adm
│   │   │   └── tmp
│   │   ├── cmds
│   │   │   ├── adm
│   │   │   │   ├── _accountapproval.c
│   │   │   │   ├── _advance.c
│   │   │   │   ├── _archshout.c
│   │   │   │   ├── _bench.c
│   │   │   │   ├── _broadcast.c
│   │   │   │   ├── _cache.c
│   │   │   │   ├── _channel.c
│   │   │   │   ├── _commands.c
│   │   │   │   ├── _cref.c
│   │   │   │   ├── _dumpallobj.c
│   │   │   │   ├── _end.c
│   │   │   │   ├── _fdinfo.c
│   │   │   │   ├── _fixdemote.c
│   │   │   │   ├── _force.c
│   │   │   │   ├── _give.c
│   │   │   │   ├── _goto.c
│   │   │   │   ├── _grantrace.c
│   │   │   │   ├── _grantskills.c
│   │   │   │   ├── _haven.c
│   │   │   │   ├── _makechar.c
│   │   │   │   ├── _makewiz.c
│   │   │   │   ├── _monitor.c
│   │   │   │   ├── _opcprof.c
│   │   │   │   ├── _playerwipe.c
│   │   │   │   ├── _repairchar.c
│   │   │   │   ├── _setfaction.c
│   │   │   │   ├── _setocc.c
│   │   │   │   ├── _setrcc.c
│   │   │   │   ├── _setrole.c
│   │   │   │   ├── _setskill.c
│   │   │   │   ├── _sever.c
│   │   │   │   ├── _stat.c
│   │   │   │   ├── _trans.c
│   │   │   │   └── _warmboot.c
│   │   │   ├── ambassador
│   │   │   │   ├── _earmuffs.c
│   │   │   │   ├── _goto.c
│   │   │   │   ├── _home.c
│   │   │   │   ├── _man.c
│   │   │   │   ├── _people.c
│   │   │   │   ├── _set.c
│   │   │   │   └── _unset.c
│   │   │   ├── creator
│   │   │   │   ├── _ac.c
│   │   │   │   ├── _acheck.c
│   │   │   │   ├── _banish.c
│   │   │   │   ├── _callouts.c
│   │   │   │   ├── _classblock.c
│   │   │   │   ├── _clean.c
│   │   │   │   ├── _clone.c
│   │   │   │   ├── _codeblock.c
│   │   │   │   ├── _dest.c
│   │   │   │   ├── _echo.c
│   │   │   │   ├── _echoto.c
│   │   │   │   ├── _elog.c
│   │   │   │   ├── _events.c
│   │   │   │   ├── _format.c
│   │   │   │   ├── _invis.c
│   │   │   │   ├── _langs.c
│   │   │   │   ├── _localcmds.c
│   │   │   │   ├── _malloc.c
│   │   │   │   ├── _mraces.c
│   │   │   │   ├── _mstatus.c
│   │   │   │   ├── _netstat.c
│   │   │   │   ├── _popd.c
│   │   │   │   ├── _pupdate.c
│   │   │   │   ├── _pushd.c
│   │   │   │   ├── _qcs.c
│   │   │   │   ├── _qcsdirs.c
│   │   │   │   ├── _qcs_item.c
│   │   │   │   ├── _qcs_npc.c
│   │   │   │   ├── _qcs_room.c
│   │   │   │   ├── _rehash.c
│   │   │   │   ├── _replog.c
│   │   │   │   ├── _reset.c
│   │   │   │   ├── _roomcheck.c
│   │   │   │   ├── _scan.c
│   │   │   │   ├── _sethome.c
│   │   │   │   ├── _stat.c
│   │   │   │   ├── _tellblock.c
│   │   │   │   ├── _trans.c
│   │   │   │   ├── _unbundle.c
│   │   │   │   ├── _unref.c
│   │   │   │   ├── _uptime.c
│   │   │   │   ├── _vis.c
│   │   │   │   ├── _wc.c
│   │   │   │   ├── _wcheck.c
│   │   │   │   ├── _wizheal.c
│   │   │   │   └── _wizshout.c
│   │   │   ├── database
│   │   │   │   └── where.db
│   │   │   ├── guild
│   │   │   ├── hm
│   │   │   │   ├── _cloak.c
│   │   │   │   ├── _create.c
│   │   │   │   ├── _divorce.c
│   │   │   │   ├── _mark.c
│   │   │   │   ├── _marry.c
│   │   │   │   ├── _message.c
│   │   │   │   ├── _review.c
│   │   │   │   ├── _start.c
│   │   │   │   ├── _teleport.c
│   │   │   │   └── _title.c
│   │   │   ├── mortal
│   │   │   │   ├── _abilities.c
│   │   │   │   ├── _advance.c
│   │   │   │   ├── _armorofithan.c
│   │   │   │   ├── _assassinate.c
│   │   │   │   ├── _assassination.c
│   │   │   │   ├── _assist.c
│   │   │   │   ├── _autododge.c
│   │   │   │   ├── _autoparry.c
│   │   │   │   ├── _bandage.c
│   │   │   │   ├── _biography.c
│   │   │   │   ├── _bionics.c
│   │   │   │   ├── _body.c
│   │   │   │   ├── _breath.c
│   │   │   │   ├── _brief.c
│   │   │   │   ├── _bug.c
│   │   │   │   ├── _call.c
│   │   │   │   ├── _card.c
│   │   │   │   ├── _cast.c
│   │   │   │   ├── _chat.c
│   │   │   │   ├── _clan.c
│   │   │   │   ├── _cls.c
│   │   │   │   ├── _color.c
│   │   │   │   ├── _colorize.c
│   │   │   │   ├── _combat.c
│   │   │   │   ├── _commands.c
│   │   │   │   ├── _converse.c
│   │   │   │   ├── _credits.c
│   │   │   │   ├── _customize.c
│   │   │   │   ├── _date.c
│   │   │   │   ├── _describe.c
│   │   │   │   ├── _dig.c
│   │   │   │   ├── _dodge.c
│   │   │   │   ├── _drive.c
│   │   │   │   ├── _drop.c
│   │   │   │   ├── _echo.c
│   │   │   │   ├── _eject.c
│   │   │   │   ├── _emote.c
│   │   │   │   ├── _empathy.c
│   │   │   │   ├── _eq.c
│   │   │   │   ├── _equipment.c
│   │   │   │   ├── _examine.c
│   │   │   │   ├── _exchange.c
│   │   │   │   ├── _exp.c
│   │   │   │   ├── _experience.c
│   │   │   │   ├── _face.c
│   │   │   │   ├── _faction.c
│   │   │   │   ├── _faq.c
│   │   │   │   ├── _flee.c
│   │   │   │   ├── _fly.c
│   │   │   │   ├── _follow.c
│   │   │   │   ├── _forget.c
│   │   │   │   ├── _forward.c
│   │   │   │   ├── _get.c
│   │   │   │   ├── _give.c
│   │   │   │   ├── _greet.c
│   │   │   │   ├── _guild.c
│   │   │   │   ├── _harass.c
│   │   │   │   ├── _help.c
│   │   │   │   ├── _hide.c
│   │   │   │   ├── _idea.c
│   │   │   │   ├── _idle.c
│   │   │   │   ├── _improve.c
│   │   │   │   ├── _introduce.c
│   │   │   │   ├── _inventory.c
│   │   │   │   ├── _kill.c
│   │   │   │   ├── _kills.c
│   │   │   │   ├── _known.c
│   │   │   │   ├── _language.c
│   │   │   │   ├── _languages.c
│   │   │   │   ├── _lastkiller.c
│   │   │   │   ├── _left.c
│   │   │   │   ├── _lineecho.c
│   │   │   │   ├── _lines.c
│   │   │   │   ├── _look.c
│   │   │   │   ├── _magicnet.c
│   │   │   │   ├── _mail.c
│   │   │   │   ├── _meditate.c
│   │   │   │   ├── _metamorph.c
│   │   │   │   ├── _mindblock.c
│   │   │   │   ├── _money.c
│   │   │   │   ├── _mudlist.c
│   │   │   │   ├── _mudparty.c
│   │   │   │   ├── _news.c
│   │   │   │   ├── _nextreboot.c
│   │   │   │   ├── _objectread.c
│   │   │   │   ├── _ooc.c
│   │   │   │   ├── _parry.c
│   │   │   │   ├── _party.c
│   │   │   │   ├── _password.c
│   │   │   │   ├── _pemote.c
│   │   │   │   ├── _picture.c
│   │   │   │   ├── _pilot.c
│   │   │   │   ├── _position.c
│   │   │   │   ├── _posting.c
│   │   │   │   ├── _pray.c
│   │   │   │   ├── _prompt.c
│   │   │   │   ├── _prowl.c
│   │   │   │   ├── _psi.c
│   │   │   │   ├── _psihelp.c
│   │   │   │   ├── _psionics.c
│   │   │   │   ├── _psisword.c
│   │   │   │   ├── _pskills.c
│   │   │   │   ├── _push.c
│   │   │   │   ├── _quests.c
│   │   │   │   ├── _radio.c
│   │   │   │   ├── _readaura.c
│   │   │   │   ├── _reload.c
│   │   │   │   ├── _remember.c
│   │   │   │   ├── _remoteview.c
│   │   │   │   ├── _reputation.c
│   │   │   │   ├── _request.c
│   │   │   │   ├── _rest.c
│   │   │   │   ├── _retrieve.c
│   │   │   │   ├── _rift.c
│   │   │   │   ├── _right.c
│   │   │   │   ├── _roll.c
│   │   │   │   ├── _save.c
│   │   │   │   ├── _saving_throws.c
│   │   │   │   ├── _say.c
│   │   │   │   ├── _sbar.c
│   │   │   │   ├── _score.c
│   │   │   │   ├── _senseevil.c
│   │   │   │   ├── _setenv.c
│   │   │   │   ├── _setrespawn.c
│   │   │   │   ├── _shout.c
│   │   │   │   ├── _sirname.c
│   │   │   │   ├── _sites.c
│   │   │   │   ├── _skillrequest.c
│   │   │   │   ├── _skills.c
│   │   │   │   ├── _slave.c
│   │   │   │   ├── _sneak.c
│   │   │   │   ├── _snoopable.c
│   │   │   │   ├── _speak.c
│   │   │   │   ├── _spell.c
│   │   │   │   ├── _spells.c
│   │   │   │   ├── _sskills.c
│   │   │   │   ├── _stance.c
│   │   │   │   ├── _stats.c
│   │   │   │   ├── _status.c
│   │   │   │   ├── _stop.c
│   │   │   │   ├── _store.c
│   │   │   │   ├── _suicide.c
│   │   │   │   ├── _take.c
│   │   │   │   ├── _tattoo.c
│   │   │   │   ├── _telekinesis.c
│   │   │   │   ├── _telemechanics.c
│   │   │   │   ├── _telepathy.c
│   │   │   │   ├── _testcolor.c
│   │   │   │   ├── _tongues.c
│   │   │   │   ├── _touch.c
│   │   │   │   ├── _trade.c
│   │   │   │   ├── _treat.c
│   │   │   │   ├── _typo.c
│   │   │   │   ├── _unload.c
│   │   │   │   ├── _users.c
│   │   │   │   ├── _version.c
│   │   │   │   ├── _wake.c
│   │   │   │   ├── _wanted.c
│   │   │   │   ├── _watchradio.c
│   │   │   │   ├── _where.c
│   │   │   │   ├── _whimpy.c
│   │   │   │   ├── _whisper.c
│   │   │   │   ├── _who.c
│   │   │   │   ├── _wimpy.c
│   │   │   │   ├── _wimpydir.c
│   │   │   │   ├── _wiz.c
│   │   │   │   ├── _wizchat.c
│   │   │   │   ├── _workroom.c
│   │   │   │   ├── _worn.c
│   │   │   │   ├── _xp.c
│   │   │   │   ├── _yell.c
│   │   │   │   └── _zero.c
│   │   │   ├── qcs
│   │   │   │   ├── _about.c
│   │   │   │   ├── _add.c
│   │   │   │   ├── _create.c
│   │   │   │   ├── _delete.c
│   │   │   │   ├── _modify.c
│   │   │   │   └── _more.c
│   │   │   ├── race
│   │   │   ├── skills
│   │   │   │   ├── _aim.c
│   │   │   │   ├── _analyze.c
│   │   │   │   ├── _aura.c
│   │   │   │   ├── _backstab.c
│   │   │   │   ├── _balefire.c
│   │   │   │   ├── _bolt.c
│   │   │   │   ├── _buffer.c
│   │   │   │   ├── _bump.c
│   │   │   │   ├── _burn.c
│   │   │   │   ├── _chilltouch.c
│   │   │   │   ├── _compare.c
│   │   │   │   ├── _corrupt.c
│   │   │   │   ├── _cure.c
│   │   │   │   ├── _dark.c
│   │   │   │   ├── _demonfire.c
│   │   │   │   ├── _detect.c
│   │   │   │   ├── _deus.c
│   │   │   │   ├── _discern.c
│   │   │   │   ├── _dispell.c
│   │   │   │   ├── _drain.c
│   │   │   │   ├── _ears.c
│   │   │   │   ├── _enchant.c
│   │   │   │   ├── _fear.c
│   │   │   │   ├── _feed.c
│   │   │   │   ├── _fireball.c
│   │   │   │   ├── _forgive.c
│   │   │   │   ├── _freeze.c
│   │   │   │   ├── _heal.c
│   │   │   │   ├── _hold.c
│   │   │   │   ├── _keep.c
│   │   │   │   ├── _light.c
│   │   │   │   ├── _limbs.c
│   │   │   │   ├── _lockpick.c
│   │   │   │   ├── _lore.c
│   │   │   │   ├── _mend.c
│   │   │   │   ├── _mheal.c
│   │   │   │   ├── _missile.c
│   │   │   │   ├── _mist.c
│   │   │   │   ├── _offer.c
│   │   │   │   ├── _order.c
│   │   │   │   ├── _palm.c
│   │   │   │   ├── _paralyze.c
│   │   │   │   ├── _parry.c
│   │   │   │   ├── _peace.c
│   │   │   │   ├── _peek.c
│   │   │   │   ├── _pick.c
│   │   │   │   ├── _protect.c
│   │   │   │   ├── _purify.c
│   │   │   │   ├── _raise.c
│   │   │   │   ├── _rapture.c
│   │   │   │   ├── _replace.c
│   │   │   │   ├── _rot.c
│   │   │   │   ├── _scry.c
│   │   │   │   ├── _sense.c
│   │   │   │   ├── _shield.c
│   │   │   │   ├── _shock.c
│   │   │   │   ├── _sneak.c
│   │   │   │   ├── _steal.c
│   │   │   │   ├── _stealth.c
│   │   │   │   └── _turn.c
│   │   │   ├── HOWTO.md
│   │   │   ├── LLM_BREADCRUMB.md
│   │   │   ├── README.md
│   │   │   └── WHY.md
│   │   ├── daemon
│   │   │   ├── cfg
│   │   │   │   ├── mon_races
│   │   │   │   │   ├── arachnid
│   │   │   │   │   ├── centaur
│   │   │   │   │   ├── dragon
│   │   │   │   │   ├── equine
│   │   │   │   │   ├── fowl
│   │   │   │   │   ├── insectoid
│   │   │   │   │   ├── insectoid-winged
│   │   │   │   │   ├── merperson
│   │   │   │   │   └── quadruped
│   │   │   │   ├── races
│   │   │   │   │   ├── artrell
│   │   │   │   │   ├── atlantean
│   │   │   │   │   ├── dwarf
│   │   │   │   │   ├── elf
│   │   │   │   │   ├── faerie
│   │   │   │   │   ├── giant
│   │   │   │   │   ├── gnome
│   │   │   │   │   ├── half-elf
│   │   │   │   │   ├── halfling
│   │   │   │   │   ├── half-orc
│   │   │   │   │   ├── hobbit
│   │   │   │   │   ├── human
│   │   │   │   │   ├── kender
│   │   │   │   │   ├── klingon
│   │   │   │   │   ├── nymph
│   │   │   │   │   ├── ogre
│   │   │   │   │   ├── orc
│   │   │   │   │   └── satyr
│   │   │   │   ├── languages.cfg
│   │   │   │   └── races.cfg
│   │   │   ├── include
│   │   │   │   └── intermud.h
│   │   │   ├── save
│   │   │   │   ├── accounts
│   │   │   │   │   └── aethverifya.o
│   │   │   │   ├── projects
│   │   │   │   ├── votes
│   │   │   │   ├── banish.o
│   │   │   │   ├── economy.o
│   │   │   │   ├── events.o
│   │   │   │   ├── faction_d.o
│   │   │   │   ├── intermud.o
│   │   │   │   ├── network.o
│   │   │   │   └── player_list.o
│   │   │   ├── services
│   │   │   │   ├── auth.c
│   │   │   │   ├── channel.c
│   │   │   │   ├── emoteto.c
│   │   │   │   ├── error.c
│   │   │   │   ├── finger.c
│   │   │   │   ├── locate.c
│   │   │   │   ├── tell.c
│   │   │   │   ├── ucache.c
│   │   │   │   └── who.c
│   │   │   ├── soul
│   │   │   │   ├── a.l
│   │   │   │   ├── b.l
│   │   │   │   ├── c.l
│   │   │   │   ├── d.l
│   │   │   │   ├── e.l
│   │   │   │   ├── f.l
│   │   │   │   ├── g.l
│   │   │   │   ├── h.l
│   │   │   │   ├── i.l
│   │   │   │   ├── j.l
│   │   │   │   ├── k.l
│   │   │   │   ├── l.l
│   │   │   │   ├── m.l
│   │   │   │   ├── n.l
│   │   │   │   ├── p.l
│   │   │   │   ├── q.l
│   │   │   │   ├── r.l
│   │   │   │   ├── s.l
│   │   │   │   ├── t.l
│   │   │   │   ├── w.l
│   │   │   │   ├── y.l
│   │   │   │   └── z.l
│   │   │   ├── addiction_d.c
│   │   │   ├── alchemist.c
│   │   │   ├── apprentice_d.c
│   │   │   ├── astronomy.c
│   │   │   ├── banish.c
│   │   │   ├── bionic_d.c
│   │   │   ├── chat.c
│   │   │   ├── command.c
│   │   │   ├── donation.c
│   │   │   ├── economy.c
│   │   │   ├── estates.c
│   │   │   ├── estates.h
│   │   │   ├── ex5_d.c
│   │   │   ├── faction_d.c
│   │   │   ├── guild_d.c
│   │   │   ├── help.c
│   │   │   ├── help.h
│   │   │   ├── http.c
│   │   │   ├── http.h
│   │   │   ├── intermud.c
│   │   │   ├── language_d.c
│   │   │   ├── languages.c
│   │   │   ├── mail_d.c
│   │   │   ├── multi.c
│   │   │   ├── network.c
│   │   │   ├── news.c
│   │   │   ├── occ.c
│   │   │   ├── party.c
│   │   │   ├── project.c
│   │   │   ├── quest_d.c
│   │   │   ├── race.c
│   │   │   ├── refs.c
│   │   │   ├── rifts.c
│   │   │   ├── rifts_combat.c
│   │   │   ├── rifts_psionics_d.c
│   │   │   ├── rifts_skills.c
│   │   │   ├── rifts_spells_d.c
│   │   │   ├── rifts_start_d.c
│   │   │   ├── services.c
│   │   │   ├── services.old
│   │   │   ├── skill_request_d.c
│   │   │   ├── skills.c
│   │   │   ├── soul.c
│   │   │   ├── tattoo_d.c
│   │   │   ├── terminal.c
│   │   │   ├── timezones.c
│   │   │   ├── unique_items.c
│   │   │   ├── voting.c
│   │   │   └── wanted_d.c
│   │   ├── doc
│   │   │   ├── applies
│   │   │   │   ├── author_file
│   │   │   │   ├── catch_tell
│   │   │   │   ├── clean_up
│   │   │   │   ├── compile_object
│   │   │   │   ├── connect
│   │   │   │   ├── crash
│   │   │   │   ├── create
│   │   │   │   ├── creator_file
│   │   │   │   ├── destruct_env_of
│   │   │   │   ├── domain_file
│   │   │   │   ├── epilog
│   │   │   │   ├── error_handler
│   │   │   │   ├── flag
│   │   │   │   ├── get_bb_uid
│   │   │   │   ├── get_root_uid
│   │   │   │   ├── get_save_file_name
│   │   │   │   ├── id
│   │   │   │   ├── __INIT
│   │   │   │   ├── init
│   │   │   │   ├── log_error
│   │   │   │   ├── logon
│   │   │   │   ├── make_path_absolute
│   │   │   │   ├── move
│   │   │   │   ├── net_dead
│   │   │   │   ├── object_name
│   │   │   │   ├── preload
│   │   │   │   ├── privs_file
│   │   │   │   ├── process_input
│   │   │   │   ├── receive_message
│   │   │   │   ├── receive_snoop
│   │   │   │   ├── reset
│   │   │   │   ├── retrieve_ed_setup
│   │   │   │   ├── save_ed_setup
│   │   │   │   ├── slow_shutdown
│   │   │   │   ├── telnet_suboption
│   │   │   │   ├── valid_asm
│   │   │   │   ├── valid_compile_to_c
│   │   │   │   ├── valid_hide
│   │   │   │   ├── valid_link
│   │   │   │   ├── valid_object
│   │   │   │   ├── valid_override
│   │   │   │   ├── valid_read
│   │   │   │   ├── valid_save_binary
│   │   │   │   ├── valid_seteuid
│   │   │   │   ├── valid_shadow
│   │   │   │   ├── valid_socket
│   │   │   │   ├── valid_write
│   │   │   │   └── write_prompt
│   │   │   ├── approval
│   │   │   │   ├── balance
│   │   │   │   │   ├── bal.armour
│   │   │   │   │   ├── bal.ep
│   │   │   │   │   ├── bal.guilds
│   │   │   │   │   ├── bal.high_mortals
│   │   │   │   │   ├── bal.monsters
│   │   │   │   │   ├── bal.novice
│   │   │   │   │   ├── bal.shop.pub.armoury
│   │   │   │   │   ├── bal.weapons
│   │   │   │   │   └── README
│   │   │   │   ├── QC
│   │   │   │   │   ├── check.doc
│   │   │   │   │   ├── ideas
│   │   │   │   │   ├── qc.code
│   │   │   │   │   └── qc.realm
│   │   │   │   └── README
│   │   │   ├── build
│   │   │   │   ├── armours
│   │   │   │   │   └── tutorial
│   │   │   │   ├── economy
│   │   │   │   │   ├── pubs
│   │   │   │   │   └── shops
│   │   │   │   ├── etc
│   │   │   │   │   ├── containers
│   │   │   │   │   ├── guilds
│   │   │   │   │   ├── Infections
│   │   │   │   │   ├── light
│   │   │   │   │   ├── messages
│   │   │   │   │   ├── poisoning
│   │   │   │   │   ├── properties
│   │   │   │   │   ├── quests
│   │   │   │   │   ├── quest_template
│   │   │   │   │   └── vendor_types
│   │   │   │   ├── monster
│   │   │   │   │   ├── languages
│   │   │   │   │   ├── races
│   │   │   │   │   └── tutorial
│   │   │   │   ├── room
│   │   │   │   │   ├── Advanced
│   │   │   │   │   ├── Basic
│   │   │   │   │   ├── doors
│   │   │   │   │   ├── fishing
│   │   │   │   │   ├── FunctionList
│   │   │   │   │   ├── Intermediate
│   │   │   │   │   └── RoomLighting
│   │   │   │   ├── virtual
│   │   │   │   │   ├── 3d_virtual_rooms
│   │   │   │   │   ├── virtual_docs
│   │   │   │   │   └── virtual_setup
│   │   │   │   ├── weapon
│   │   │   │   │   └── tutorial
│   │   │   │   ├── approval
│   │   │   │   ├── building
│   │   │   │   └── GettingStarted
│   │   │   ├── classes
│   │   │   │   ├── 1
│   │   │   │   ├── 2
│   │   │   │   ├── 3
│   │   │   │   ├── 4
│   │   │   │   ├── 5
│   │   │   │   └── 6
│   │   │   ├── efun
│   │   │   │   ├── acos
│   │   │   │   ├── add_action
│   │   │   │   ├── all_inventory
│   │   │   │   ├── allocate
│   │   │   │   ├── allocate_buffer
│   │   │   │   ├── allocate_mapping
│   │   │   │   ├── asin
│   │   │   │   ├── atan
│   │   │   │   ├── author_stats
│   │   │   │   ├── break_string
│   │   │   │   ├── bufferp
│   │   │   │   ├── cache_stats
│   │   │   │   ├── call_other
│   │   │   │   ├── call_out
│   │   │   │   ├── call_out_info
│   │   │   │   ├── capitalize
│   │   │   │   ├── catch
│   │   │   │   ├── ceil
│   │   │   │   ├── children
│   │   │   │   ├── clear_bit
│   │   │   │   ├── clone_object
│   │   │   │   ├── clonep
│   │   │   │   ├── command
│   │   │   │   ├── commands
│   │   │   │   ├── cos
│   │   │   │   ├── cp
│   │   │   │   ├── crc32
│   │   │   │   ├── crypt
│   │   │   │   ├── ctime
│   │   │   │   ├── debug_info
│   │   │   │   ├── debugmalloc
│   │   │   │   ├── deep_inherit_list
│   │   │   │   ├── deep_inventory
│   │   │   │   ├── destruct
│   │   │   │   ├── disable_commands
│   │   │   │   ├── disable_wizard
│   │   │   │   ├── domain_stats
│   │   │   │   ├── dumpallobj
│   │   │   │   ├── dump_file_descriptors
│   │   │   │   ├── dump_prog
│   │   │   │   ├── dump_socket_status
│   │   │   │   ├── each
│   │   │   │   ├── ed
│   │   │   │   ├── enable_commands
│   │   │   │   ├── enable_wizard
│   │   │   │   ├── environment
│   │   │   │   ├── error
│   │   │   │   ├── errorp
│   │   │   │   ├── exec
│   │   │   │   ├── exp
│   │   │   │   ├── explode
│   │   │   │   ├── export_uid
│   │   │   │   ├── file_name
│   │   │   │   ├── file_size
│   │   │   │   ├── filter_array
│   │   │   │   ├── find_call_out
│   │   │   │   ├── find_living
│   │   │   │   ├── find_object
│   │   │   │   ├── find_player
│   │   │   │   ├── first_inventory
│   │   │   │   ├── floatp
│   │   │   │   ├── floor
│   │   │   │   ├── function_exists
│   │   │   │   ├── functionp
│   │   │   │   ├── function_profile
│   │   │   │   ├── generate_source
│   │   │   │   ├── get_char
│   │   │   │   ├── get_config
│   │   │   │   ├── get_dir
│   │   │   │   ├── geteuid
│   │   │   │   ├── getuid
│   │   │   │   ├── implode
│   │   │   │   ├── in_edit
│   │   │   │   ├── inherit_list
│   │   │   │   ├── inherits
│   │   │   │   ├── in_input
│   │   │   │   ├── input_to
│   │   │   │   ├── interactive
│   │   │   │   ├── intp
│   │   │   │   ├── keys
│   │   │   │   ├── link
│   │   │   │   ├── living
│   │   │   │   ├── livings
│   │   │   │   ├── localtime
│   │   │   │   ├── log
│   │   │   │   ├── lower_case
│   │   │   │   ├── malloc_status
│   │   │   │   ├── map_array
│   │   │   │   ├── map_delete
│   │   │   │   ├── mapp
│   │   │   │   ├── master
│   │   │   │   ├── match_path
│   │   │   │   ├── member_array
│   │   │   │   ├── memory_info
│   │   │   │   ├── message
│   │   │   │   ├── mkdir
│   │   │   │   ├── moncontrol
│   │   │   │   ├── move_object
│   │   │   │   ├── mud_status
│   │   │   │   ├── new
│   │   │   │   ├── next_inventory
│   │   │   │   ├── notify_fail
│   │   │   │   ├── nullp
│   │   │   │   ├── objectp
│   │   │   │   ├── objects
│   │   │   │   ├── opcprof
│   │   │   │   ├── origin
│   │   │   │   ├── parse_command
│   │   │   │   ├── pointerp
│   │   │   │   ├── pow
│   │   │   │   ├── present
│   │   │   │   ├── previous_object
│   │   │   │   ├── printf
│   │   │   │   ├── process_string
│   │   │   │   ├── process_value
│   │   │   │   ├── query_heart_beat
│   │   │   │   ├── query_host_name
│   │   │   │   ├── query_idle
│   │   │   │   ├── query_ip_name
│   │   │   │   ├── query_ip_number
│   │   │   │   ├── query_load_average
│   │   │   │   ├── query_privs
│   │   │   │   ├── query_shadowing
│   │   │   │   ├── query_snoop
│   │   │   │   ├── query_snooping
│   │   │   │   ├── query_verb
│   │   │   │   ├── random
│   │   │   │   ├── read_buffer
│   │   │   │   ├── read_bytes
│   │   │   │   ├── read_file
│   │   │   │   ├── receive
│   │   │   │   ├── reclaim_objects
│   │   │   │   ├── refs
│   │   │   │   ├── regexp
│   │   │   │   ├── reload_object
│   │   │   │   ├── remove_action
│   │   │   │   ├── remove_call_out
│   │   │   │   ├── rename
│   │   │   │   ├── replace_program
│   │   │   │   ├── replace_string
│   │   │   │   ├── resolve
│   │   │   │   ├── restore_object
│   │   │   │   ├── rm
│   │   │   │   ├── rmdir
│   │   │   │   ├── rusage
│   │   │   │   ├── save_object
│   │   │   │   ├── say
│   │   │   │   ├── set_author
│   │   │   │   ├── set_bit
│   │   │   │   ├── set_debug_level
│   │   │   │   ├── seteuid
│   │   │   │   ├── set_eval_limit
│   │   │   │   ├── set_heart_beat
│   │   │   │   ├── set_hide
│   │   │   │   ├── set_light
│   │   │   │   ├── set_living_name
│   │   │   │   ├── set_malloc_mask
│   │   │   │   ├── set_privs
│   │   │   │   ├── set_reset
│   │   │   │   ├── shadow
│   │   │   │   ├── shout
│   │   │   │   ├── shutdown
│   │   │   │   ├── sin
│   │   │   │   ├── sizeof
│   │   │   │   ├── snoop
│   │   │   │   ├── socket_accept
│   │   │   │   ├── socket_acquire
│   │   │   │   ├── socket_address
│   │   │   │   ├── socket_bind
│   │   │   │   ├── socket_close
│   │   │   │   ├── socket_connect
│   │   │   │   ├── socket_create
│   │   │   │   ├── socket_error
│   │   │   │   ├── socket_listen
│   │   │   │   ├── socket_release
│   │   │   │   ├── socket_write
│   │   │   │   ├── sort_array
│   │   │   │   ├── sprintf
│   │   │   │   ├── sqrt
│   │   │   │   ├── sscanf
│   │   │   │   ├── stat
│   │   │   │   ├── strcmp
│   │   │   │   ├── stringp
│   │   │   │   ├── strlen
│   │   │   │   ├── strsrch
│   │   │   │   ├── swap
│   │   │   │   ├── tail
│   │   │   │   ├── tan
│   │   │   │   ├── tell_object
│   │   │   │   ├── tell_room
│   │   │   │   ├── test_bit
│   │   │   │   ├── this_object
│   │   │   │   ├── this_player
│   │   │   │   ├── throw
│   │   │   │   ├── time
│   │   │   │   ├── time_expression
│   │   │   │   ├── to_float
│   │   │   │   ├── to_int
│   │   │   │   ├── trace
│   │   │   │   ├── traceprefix
│   │   │   │   ├── undefinedp
│   │   │   │   ├── unique_array
│   │   │   │   ├── uptime
│   │   │   │   ├── userp
│   │   │   │   ├── users
│   │   │   │   ├── values
│   │   │   │   ├── virtualp
│   │   │   │   ├── wizardp
│   │   │   │   ├── write
│   │   │   │   ├── write_buffer
│   │   │   │   ├── write_bytes
│   │   │   │   └── write_file
│   │   │   ├── etc
│   │   │   │   ├── defines
│   │   │   │   │   ├── __ARCH__
│   │   │   │   │   ├── __DIR__
│   │   │   │   │   ├── HAS_CACHE_STATS
│   │   │   │   │   ├── HAS_DEBUG_LEVEL
│   │   │   │   │   ├── HAS_DEBUGMALLOC
│   │   │   │   │   ├── HAS_EACH
│   │   │   │   │   ├── HAS_ED
│   │   │   │   │   ├── HAS_LIGHT
│   │   │   │   │   ├── HAS_MATH
│   │   │   │   │   ├── HAS_MATRIX
│   │   │   │   │   ├── HAS_MUDLIB_ERROR_HANDLER
│   │   │   │   │   ├── HAS_MUDLIB_STATS
│   │   │   │   │   ├── HAS_OPCPROF
│   │   │   │   │   ├── HAS_PRINTF
│   │   │   │   │   ├── HAS_PRIVS
│   │   │   │   │   ├── HAS_RUSAGE
│   │   │   │   │   ├── HAS_SHADOWS
│   │   │   │   │   ├── HAS_SOCKETS
│   │   │   │   │   ├── LPC3
│   │   │   │   │   ├── MUD_NAME
│   │   │   │   │   ├── MUDOS
│   │   │   │   │   ├── __PORT__
│   │   │   │   │   ├── SAVE_EXTENSION
│   │   │   │   │   ├── USE_EUID
│   │   │   │   │   └── __VERSION__
│   │   │   │   └── Credits.DriverDocs
│   │   │   ├── help
│   │   │   │   ├── ambassador
│   │   │   │   ├── creator
│   │   │   │   │   ├── build
│   │   │   │   │   ├── debughints
│   │   │   │   │   ├── debugmacro
│   │   │   │   │   ├── ed
│   │   │   │   │   ├── mmsg
│   │   │   │   │   ├── postal
│   │   │   │   │   ├── qcs
│   │   │   │   │   └── start
│   │   │   │   ├── hm
│   │   │   │   │   └── estates
│   │   │   │   ├── user
│   │   │   │   │   ├── aberrant
│   │   │   │   │   ├── abilities
│   │   │   │   │   ├── account
│   │   │   │   │   ├── advance
│   │   │   │   │   ├── aethermud
│   │   │   │   │   ├── airwarlock
│   │   │   │   │   ├── algorfrostgiant
│   │   │   │   │   ├── alias
│   │   │   │   │   ├── alignment
│   │   │   │   │   ├── anarchist
│   │   │   │   │   ├── ansi
│   │   │   │   │   ├── anthropology
│   │   │   │   │   ├── appraisegoods
│   │   │   │   │   ├── archaeology
│   │   │   │   │   ├── arches
│   │   │   │   │   ├── armorofithan
│   │   │   │   │   ├── art
│   │   │   │   │   ├── artrell
│   │   │   │   │   ├── assassination
│   │   │   │   │   ├── assist
│   │   │   │   │   ├── astronomy
│   │   │   │   │   ├── atlanteannomad
│   │   │   │   │   ├── atlanteanslave
│   │   │   │   │   ├── autododge
│   │   │   │   │   ├── autoparry
│   │   │   │   │   ├── bandage
│   │   │   │   │   ├── barter
│   │   │   │   │   ├── basilisk
│   │   │   │   │   ├── bearman
│   │   │   │   │   ├── beg
│   │   │   │   │   ├── biology
│   │   │   │   │   ├── bionics
│   │   │   │   │   ├── blackmarket
│   │   │   │   │   ├── bodyfixer
│   │   │   │   │   ├── bogie
│   │   │   │   │   ├── borg
│   │   │   │   │   ├── bot
│   │   │   │   │   ├── botany
│   │   │   │   │   ├── bots
│   │   │   │   │   ├── bountyhunter
│   │   │   │   │   ├── breath
│   │   │   │   │   ├── brief
│   │   │   │   │   ├── brownie
│   │   │   │   │   ├── bug
│   │   │   │   │   ├── burster
│   │   │   │   │   ├── card
│   │   │   │   │   ├── cardsharp
│   │   │   │   │   ├── cast
│   │   │   │   │   ├── changeling
│   │   │   │   │   ├── channels
│   │   │   │   │   ├── chat
│   │   │   │   │   ├── cityrat
│   │   │   │   │   ├── clan
│   │   │   │   │   ├── classes
│   │   │   │   │   ├── cleric
│   │   │   │   │   ├── coalitionsoldier
│   │   │   │   │   ├── color
│   │   │   │   │   ├── colorize
│   │   │   │   │   ├── combat
│   │   │   │   │   ├── commands
│   │   │   │   │   ├── commonfaerie
│   │   │   │   │   ├── commonpixie
│   │   │   │   │   ├── communication
│   │   │   │   │   ├── computerrepair
│   │   │   │   │   ├── concept
│   │   │   │   │   ├── concept1
│   │   │   │   │   ├── concept2
│   │   │   │   │   ├── consent
│   │   │   │   │   ├── conservator
│   │   │   │   │   ├── converse
│   │   │   │   │   ├── cook
│   │   │   │   │   ├── coyle
│   │   │   │   │   ├── crazy
│   │   │   │   │   ├── csdeadboy
│   │   │   │   │   ├── csgrunt
│   │   │   │   │   ├── csmilitaryspecialist
│   │   │   │   │   ├── csranger
│   │   │   │   │   ├── cssamasrpapilot
│   │   │   │   │   ├── cstechnicalofficer
│   │   │   │   │   ├── currency
│   │   │   │   │   ├── customize
│   │   │   │   │   ├── cyberdoc
│   │   │   │   │   ├── cyberknight
│   │   │   │   │   ├── dancing
│   │   │   │   │   ├── days
│   │   │   │   │   ├── death
│   │   │   │   │   ├── deception
│   │   │   │   │   ├── delphijuicer
│   │   │   │   │   ├── describe
│   │   │   │   │   ├── description
│   │   │   │   │   ├── detectpsionics
│   │   │   │   │   ├── diabolic
│   │   │   │   │   ├── disclaimer
│   │   │   │   │   ├── disposition
│   │   │   │   │   ├── dodge
│   │   │   │   │   ├── dogboy
│   │   │   │   │   ├── drop
│   │   │   │   │   ├── dwarf
│   │   │   │   │   ├── echo
│   │   │   │   │   ├── eject
│   │   │   │   │   ├── electricalengineer
│   │   │   │   │   ├── elf
│   │   │   │   │   ├── emote
│   │   │   │   │   ├── empathy
│   │   │   │   │   ├── eq
│   │   │   │   │   ├── equinoid
│   │   │   │   │   ├── escapeartist
│   │   │   │   │   ├── experience
│   │   │   │   │   ├── face
│   │   │   │   │   ├── faction
│   │   │   │   │   ├── faerie
│   │   │   │   │   ├── faq
│   │   │   │   │   ├── fighter
│   │   │   │   │   ├── firedragon
│   │   │   │   │   ├── firstaid
│   │   │   │   │   ├── flee
│   │   │   │   │   ├── fly
│   │   │   │   │   ├── forger
│   │   │   │   │   ├── forget
│   │   │   │   │   ├── forward
│   │   │   │   │   ├── freelancespy
│   │   │   │   │   ├── frostpixie
│   │   │   │   │   ├── gargoyle
│   │   │   │   │   ├── geology
│   │   │   │   │   ├── get
│   │   │   │   │   ├── getting-started
│   │   │   │   │   ├── giant
│   │   │   │   │   ├── giftedgypsy
│   │   │   │   │   ├── give
│   │   │   │   │   ├── glitterboypilot
│   │   │   │   │   ├── gnome
│   │   │   │   │   ├── goal
│   │   │   │   │   ├── goblin
│   │   │   │   │   ├── greathorneddragon
│   │   │   │   │   ├── greenwoodfaerie
│   │   │   │   │   ├── greet
│   │   │   │   │   ├── guild
│   │   │   │   │   ├── gunfighter
│   │   │   │   │   ├── gurgoyle
│   │   │   │   │   ├── half-elf
│   │   │   │   │   ├── halfling
│   │   │   │   │   ├── half-orc
│   │   │   │   │   ├── harassment
│   │   │   │   │   ├── headhunter
│   │   │   │   │   ├── hide
│   │   │   │   │   ├── history
│   │   │   │   │   ├── hobbit
│   │   │   │   │   ├── holisticmedicine
│   │   │   │   │   ├── horror
│   │   │   │   │   ├── human
│   │   │   │   │   ├── hyperionjuicer
│   │   │   │   │   ├── icedragon
│   │   │   │   │   ├── idea
│   │   │   │   │   ├── idling
│   │   │   │   │   ├── improve
│   │   │   │   │   ├── index
│   │   │   │   │   ├── informationsharing
│   │   │   │   │   ├── introduce
│   │   │   │   │   ├── isspeacekeeper
│   │   │   │   │   ├── issspecter
│   │   │   │   │   ├── items
│   │   │   │   │   ├── jotan
│   │   │   │   │   ├── juicer
│   │   │   │   │   ├── kankoran
│   │   │   │   │   ├── kataan
│   │   │   │   │   ├── kender
│   │   │   │   │   ├── kill
│   │   │   │   │   ├── kittanifieldmechanic
│   │   │   │   │   ├── kittaniwarrior
│   │   │   │   │   ├── klingon
│   │   │   │   │   ├── knight
│   │   │   │   │   ├── knighteurope
│   │   │   │   │   ├── known
│   │   │   │   │   ├── languages
│   │   │   │   │   ├── left
│   │   │   │   │   ├── levels
│   │   │   │   │   ├── leylinerifter
│   │   │   │   │   ├── leylinewalker
│   │   │   │   │   ├── lineecho
│   │   │   │   │   ├── linerows
│   │   │   │   │   ├── lock
│   │   │   │   │   ├── look
│   │   │   │   │   ├── mage
│   │   │   │   │   ├── magic
│   │   │   │   │   ├── mail
│   │   │   │   │   ├── map
│   │   │   │   │   ├── marriage
│   │   │   │   │   ├── masterassassin
│   │   │   │   │   ├── maximan
│   │   │   │   │   ├── medicaldoctor
│   │   │   │   │   ├── meditate
│   │   │   │   │   ├── mentors
│   │   │   │   │   ├── mercenary
│   │   │   │   │   ├── mercs
│   │   │   │   │   ├── mercsoldier
│   │   │   │   │   ├── metamorph
│   │   │   │   │   ├── mindblock
│   │   │   │   │   ├── mindmelter
│   │   │   │   │   ├── misc
│   │   │   │   │   ├── miscreant
│   │   │   │   │   ├── monk
│   │   │   │   │   ├── months
│   │   │   │   │   ├── moons
│   │   │   │   │   ├── movement
│   │   │   │   │   ├── multi-playing
│   │   │   │   │   ├── mystic
│   │   │   │   │   ├── names
│   │   │   │   │   ├── necromancer
│   │   │   │   │   ├── negapsychic
│   │   │   │   │   ├── news
│   │   │   │   │   ├── ngrmechanic
│   │   │   │   │   ├── ngrsoldier
│   │   │   │   │   ├── nightelvesfaerie
│   │   │   │   │   ├── nimrofiregiant
│   │   │   │   │   ├── ninjajuicer
│   │   │   │   │   ├── ntsetprotector
│   │   │   │   │   ├── nymph
│   │   │   │   │   ├── objectread
│   │   │   │   │   ├── occs
│   │   │   │   │   ├── ogre
│   │   │   │   │   ├── ooc
│   │   │   │   │   ├── operator
│   │   │   │   │   ├── opticsystems
│   │   │   │   │   ├── orc
│   │   │   │   │   ├── palmer
│   │   │   │   │   ├── palmer2
│   │   │   │   │   ├── palming
│   │   │   │   │   ├── paramedic
│   │   │   │   │   ├── parry
│   │   │   │   │   ├── party
│   │   │   │   │   ├── password
│   │   │   │   │   ├── pathology
│   │   │   │   │   ├── pemote
│   │   │   │   │   ├── picture
│   │   │   │   │   ├── pirate
│   │   │   │   │   ├── playinstrument
│   │   │   │   │   ├── pogtal
│   │   │   │   │   ├── position
│   │   │   │   │   ├── post
│   │   │   │   │   ├── postal
│   │   │   │   │   ├── preservefood
│   │   │   │   │   ├── principled
│   │   │   │   │   ├── privacy
│   │   │   │   │   ├── professionalthief
│   │   │   │   │   ├── prompt
│   │   │   │   │   ├── prowl
│   │   │   │   │   ├── psi
│   │   │   │   │   ├── psihelp
│   │   │   │   │   ├── psionics
│   │   │   │   │   ├── psisword
│   │   │   │   │   ├── pskills
│   │   │   │   │   ├── puertoangel
│   │   │   │   │   ├── quests
│   │   │   │   │   ├── quit
│   │   │   │   │   ├── races
│   │   │   │   │   ├── radio
│   │   │   │   │   ├── rahu-man
│   │   │   │   │   ├── ratling
│   │   │   │   │   ├── readaura
│   │   │   │   │   ├── recognizegems
│   │   │   │   │   ├── recognizeweaponquality
│   │   │   │   │   ├── religion
│   │   │   │   │   ├── reload
│   │   │   │   │   ├── remember
│   │   │   │   │   ├── remoteview
│   │   │   │   │   ├── remove
│   │   │   │   │   ├── reply
│   │   │   │   │   ├── reporting
│   │   │   │   │   ├── reputation
│   │   │   │   │   ├── rest
│   │   │   │   │   ├── retrieve
│   │   │   │   │   ├── riftsmud
│   │   │   │   │   ├── right
│   │   │   │   │   ├── robotelectronics
│   │   │   │   │   ├── robotpilot
│   │   │   │   │   ├── rogue
│   │   │   │   │   ├── roguescholar
│   │   │   │   │   ├── roguescientist
│   │   │   │   │   ├── roleplaying
│   │   │   │   │   ├── royalknight
│   │   │   │   │   ├── rptitle
│   │   │   │   │   ├── rules
│   │   │   │   │   ├── sailor
│   │   │   │   │   ├── satyr
│   │   │   │   │   ├── save
│   │   │   │   │   ├── savingthrows
│   │   │   │   │   ├── say
│   │   │   │   │   ├── sbar
│   │   │   │   │   ├── score
│   │   │   │   │   ├── scrupulous
│   │   │   │   │   ├── scruvy
│   │   │   │   │   ├── search
│   │   │   │   │   ├── secondaryvampire
│   │   │   │   │   ├── seduction
│   │   │   │   │   ├── senseevil
│   │   │   │   │   ├── sentiment
│   │   │   │   │   ├── sentinel
│   │   │   │   │   ├── sew
│   │   │   │   │   ├── shaman
│   │   │   │   │   ├── shifter
│   │   │   │   │   ├── shop
│   │   │   │   │   ├── shout
│   │   │   │   │   ├── silverbellsfaerie
│   │   │   │   │   ├── singing
│   │   │   │   │   ├── sirname
│   │   │   │   │   ├── skillrequest
│   │   │   │   │   ├── skills
│   │   │   │   │   ├── slave
│   │   │   │   │   ├── smuggler
│   │   │   │   │   ├── specialforces
│   │   │   │   │   ├── spells
│   │   │   │   │   ├── sskills
│   │   │   │   │   ├── staff
│   │   │   │   │   ├── stance
│   │   │   │   │   ├── startingequipment
│   │   │   │   │   ├── stats
│   │   │   │   │   ├── stop
│   │   │   │   │   ├── store
│   │   │   │   │   ├── suicide
│   │   │   │   │   ├── sunajassassin
│   │   │   │   │   ├── surveillancesystems
│   │   │   │   │   ├── syntax
│   │   │   │   │   ├── take
│   │   │   │   │   ├── tattoo
│   │   │   │   │   ├── tattooedman
│   │   │   │   │   ├── tattoowarrior
│   │   │   │   │   ├── technowizard
│   │   │   │   │   ├── telekinesis
│   │   │   │   │   ├── telemechanics
│   │   │   │   │   ├── telepathy
│   │   │   │   │   ├── tell
│   │   │   │   │   ├── terminal
│   │   │   │   │   ├── think
│   │   │   │   │   ├── threestrikes
│   │   │   │   │   ├── thunderlizarddragon
│   │   │   │   │   ├── timezones
│   │   │   │   │   ├── tips
│   │   │   │   │   ├── titan
│   │   │   │   │   ├── touch
│   │   │   │   │   ├── trade
│   │   │   │   │   ├── trapdetection
│   │   │   │   │   ├── treesprite
│   │   │   │   │   ├── tribalwarrior
│   │   │   │   │   ├── trigger
│   │   │   │   │   ├── triggers
│   │   │   │   │   ├── troll
│   │   │   │   │   ├── typo
│   │   │   │   │   ├── unload
│   │   │   │   │   ├── unprincipled
│   │   │   │   │   ├── unwield
│   │   │   │   │   ├── vagabond
│   │   │   │   │   ├── vehicles
│   │   │   │   │   ├── ventriloquism
│   │   │   │   │   ├── view
│   │   │   │   │   ├── wake
│   │   │   │   │   ├── watersprite
│   │   │   │   │   ├── weapon
│   │   │   │   │   ├── weapons
│   │   │   │   │   ├── wear
│   │   │   │   │   ├── werebear
│   │   │   │   │   ├── weretiger
│   │   │   │   │   ├── werewolf
│   │   │   │   │   ├── whisper
│   │   │   │   │   ├── who
│   │   │   │   │   ├── wield
│   │   │   │   │   ├── wildernessscout
│   │   │   │   │   ├── wildvampire
│   │   │   │   │   ├── wimpy
│   │   │   │   │   ├── withdraw
│   │   │   │   │   ├── wizardship
│   │   │   │   │   ├── wizardship2
│   │   │   │   │   ├── worn
│   │   │   │   │   ├── writing
│   │   │   │   │   └── www
│   │   │   │   ├── wiz
│   │   │   │   │   ├── admin
│   │   │   │   │   ├── apprentice
│   │   │   │   │   ├── call
│   │   │   │   │   ├── coding
│   │   │   │   │   ├── domain
│   │   │   │   │   ├── kit
│   │   │   │   │   ├── makewiz
│   │   │   │   │   ├── roleplay
│   │   │   │   │   ├── setrole
│   │   │   │   │   ├── staff
│   │   │   │   │   ├── update
│   │   │   │   │   └── warmboot
│   │   │   │   └── death
│   │   │   ├── law
│   │   │   │   ├── code_ownership
│   │   │   │   ├── laws.immortal
│   │   │   │   ├── MULTIPLE_CHARACTERS
│   │   │   │   ├── playerkilling_objects
│   │   │   │   ├── wizards_second
│   │   │   │   └── wiztools
│   │   │   ├── lfun
│   │   │   │   ├── add_enter.3
│   │   │   │   ├── add_enter.l
│   │   │   │   ├── add_exit
│   │   │   │   ├── add_hp
│   │   │   │   ├── add_invis_exit
│   │   │   │   ├── add_limb
│   │   │   │   ├── add_mp
│   │   │   │   ├── add_skill
│   │   │   │   ├── add_sp
│   │   │   │   ├── add_wielding_limb
│   │   │   │   ├── catch_tell
│   │   │   │   ├── check_on_limb
│   │   │   │   ├── clean_up
│   │   │   │   ├── convert.sh
│   │   │   │   ├── do_damage
│   │   │   │   ├── equip_armour_to_limb
│   │   │   │   ├── equip_weapon_to_limb
│   │   │   │   ├── extra_look
│   │   │   │   ├── init_limb_data
│   │   │   │   ├── invis_exit
│   │   │   │   ├── man2ascii.sh
│   │   │   │   ├── more
│   │   │   │   ├── move
│   │   │   │   ├── notify_kill
│   │   │   │   ├── query_ac
│   │   │   │   ├── query_all_skills
│   │   │   │   ├── query_armour
│   │   │   │   ├── query_auto_load
│   │   │   │   ├── query_fingers
│   │   │   │   ├── query_is_limb
│   │   │   │   ├── query_limbs
│   │   │   │   ├── query_max_hp
│   │   │   │   ├── query_max_mp
│   │   │   │   ├── query_max_sp
│   │   │   │   ├── query_property
│   │   │   │   ├── query_reference
│   │   │   │   ├── query_weapon
│   │   │   │   ├── query_wielding_limbs
│   │   │   │   ├── remove
│   │   │   │   ├── remove_exit
│   │   │   │   ├── remove_invis_exit
│   │   │   │   ├── remove_limb
│   │   │   │   ├── return_limb
│   │   │   │   ├── return_target_limb
│   │   │   │   ├── set_ac
│   │   │   │   ├── set_exits
│   │   │   │   ├── set_fingers
│   │   │   │   ├── set_hp
│   │   │   │   ├── set_invis_exits
│   │   │   │   ├── set_max_hp
│   │   │   │   ├── set_max_mp
│   │   │   │   ├── set_max_sp
│   │   │   │   ├── set_mp
│   │   │   │   ├── set_overall_ac
│   │   │   │   ├── set_skill
│   │   │   │   ├── set_sp
│   │   │   │   ├── set_stats
│   │   │   │   ├── set_wielding_limbs
│   │   │   │   └── skip_obvious
│   │   │   ├── library
│   │   │   │   ├── changing_monsters
│   │   │   │   ├── escape_codes
│   │   │   │   ├── guilds
│   │   │   │   ├── high_mortal
│   │   │   │   ├── immortality
│   │   │   │   ├── Information
│   │   │   │   ├── intermud_rules
│   │   │   │   ├── multiple_characters
│   │   │   │   ├── name_changes
│   │   │   │   ├── offensive_acts
│   │   │   │   ├── organization
│   │   │   │   ├── player_killing_clarification
│   │   │   │   ├── players
│   │   │   │   ├── rules
│   │   │   │   ├── sexual_harrassment
│   │   │   │   ├── snooping
│   │   │   │   ├── spirit_of_the_game
│   │   │   │   └── suiciding
│   │   │   ├── lpc
│   │   │   │   ├── advanced
│   │   │   │   ├── basic
│   │   │   │   │   ├── chapter1
│   │   │   │   │   ├── chapter2
│   │   │   │   │   ├── chapter3
│   │   │   │   │   ├── chapter4
│   │   │   │   │   ├── chapter5
│   │   │   │   │   ├── chapter6
│   │   │   │   │   ├── chapter7
│   │   │   │   │   ├── chapter8
│   │   │   │   │   ├── Contents
│   │   │   │   │   └── Introduction
│   │   │   │   ├── data_types
│   │   │   │   │   ├── function
│   │   │   │   │   ├── int
│   │   │   │   │   ├── mapping
│   │   │   │   │   └── string
│   │   │   │   ├── etc
│   │   │   │   │   └── error_msgs
│   │   │   │   ├── intermediate
│   │   │   │   │   ├── chapter1
│   │   │   │   │   ├── chapter2
│   │   │   │   │   ├── chapter3
│   │   │   │   │   ├── chapter4
│   │   │   │   │   ├── chapter5
│   │   │   │   │   ├── chapter6
│   │   │   │   │   ├── chapter7
│   │   │   │   │   ├── Contents
│   │   │   │   │   └── Copyright
│   │   │   │   └── types
│   │   │   │       ├── array.2d
│   │   │   │       ├── buffer
│   │   │   │       ├── float
│   │   │   │       ├── function
│   │   │   │       ├── general
│   │   │   │       └── mappings
│   │   │   ├── mudlib
│   │   │   │   ├── features
│   │   │   │   │   ├── login
│   │   │   │   │   ├── network
│   │   │   │   │   ├── reboots
│   │   │   │   │   └── rooms
│   │   │   │   ├── Agreement
│   │   │   │   ├── Credits
│   │   │   │   ├── DirectoryStructure
│   │   │   │   ├── GivingAccess
│   │   │   │   ├── MudLocking
│   │   │   │   ├── Security
│   │   │   │   ├── Start
│   │   │   │   └── Troubleshooting
│   │   │   ├── SimulEfun
│   │   │   │   ├── absolute_path
│   │   │   │   ├── absolute_value
│   │   │   │   ├── add_sky_event
│   │   │   │   ├── alignment_ok
│   │   │   │   ├── alignment_string
│   │   │   │   ├── ambassadorp
│   │   │   │   ├── architecture
│   │   │   │   ├── archp
│   │   │   │   ├── arrange_string
│   │   │   │   ├── base_name
│   │   │   │   ├── cardinal
│   │   │   │   ├── center
│   │   │   │   ├── consolidate
│   │   │   │   ├── convert_name
│   │   │   │   ├── copy
│   │   │   │   ├── currency_inflation
│   │   │   │   ├── currency_mass
│   │   │   │   ├── currency_rate
│   │   │   │   ├── currency_value
│   │   │   │   ├── date
│   │   │   │   ├── day
│   │   │   │   ├── destruct
│   │   │   │   ├── distinct_array
│   │   │   │   ├── domain
│   │   │   │   ├── domain_exists
│   │   │   │   ├── domain_master
│   │   │   │   ├── effective_light
│   │   │   │   ├── event
│   │   │   │   ├── event_pending
│   │   │   │   ├── exclude_array
│   │   │   │   ├── exec
│   │   │   │   ├── file_exists
│   │   │   │   ├── file_privs
│   │   │   │   ├── format_page
│   │   │   │   ├── format_string
│   │   │   │   ├── get_object
│   │   │   │   ├── hiddenp
│   │   │   │   ├── high_mortalp
│   │   │   │   ├── hour
│   │   │   │   ├── identify
│   │   │   │   ├── leaderp
│   │   │   │   ├── livings
│   │   │   │   ├── load_object
│   │   │   │   ├── log_file
│   │   │   │   ├── member_group
│   │   │   │   ├── minutes
│   │   │   │   ├── month
│   │   │   │   ├── moon_light
│   │   │   │   ├── mud_currencies
│   │   │   │   ├── mudlib
│   │   │   │   ├── mudlib_version
│   │   │   │   ├── mud_name
│   │   │   │   ├── nominative
│   │   │   │   ├── objective
│   │   │   │   ├── ordinal
│   │   │   │   ├── path_file
│   │   │   │   ├── percent
│   │   │   │   ├── pluralize
│   │   │   │   ├── possessive
│   │   │   │   ├── possessive_noun
│   │   │   │   ├── query_host_port
│   │   │   │   ├── query_night
│   │   │   │   ├── query_snoop
│   │   │   │   ├── query_snooping
│   │   │   │   ├── read_database
│   │   │   │   ├── reflexive
│   │   │   │   ├── remove_sky_event
│   │   │   │   ├── say
│   │   │   │   ├── season
│   │   │   │   ├── set_eval_limit
│   │   │   │   ├── set_privs
│   │   │   │   ├── shout
│   │   │   │   ├── shutdown
│   │   │   │   ├── snoop
│   │   │   │   ├── strip_colours
│   │   │   │   ├── tell_object
│   │   │   │   ├── tell_room
│   │   │   │   ├── total_light
│   │   │   │   ├── translate
│   │   │   │   ├── unguarded
│   │   │   │   ├── user_exists
│   │   │   │   ├── user_path
│   │   │   │   ├── version
│   │   │   │   ├── wrap
│   │   │   │   ├── write
│   │   │   │   └── year
│   │   │   ├── TestPlans
│   │   │   │   └── PlayerAbilities
│   │   │   └── README
│   │   ├── domains
│   │   │   ├── adm
│   │   │   │   ├── wiz_tools
│   │   │   │   │   ├── admin_cheat_sheet.c
│   │   │   │   │   ├── apprentice_kit.c
│   │   │   │   │   ├── card_format_d.c
│   │   │   │   │   ├── creation_review_menu_d.c
│   │   │   │   │   ├── demotion_menu_d.c
│   │   │   │   │   ├── dominion_menu_d.c
│   │   │   │   │   ├── mailbox.c
│   │   │   │   │   ├── mailbox_welcome.c
│   │   │   │   │   ├── ring_of_dominion.c
│   │   │   │   │   ├── rp_skill_menu_d.c
│   │   │   │   │   ├── rp_skill_tool.c
│   │   │   │   │   ├── skill_slip.c
│   │   │   │   │   ├── staff_board.c
│   │   │   │   │   ├── staff_of_creation.c
│   │   │   │   │   ├── staff_of_demotion.c
│   │   │   │   │   ├── staff_of_dominion.c
│   │   │   │   │   ├── staff_supplies_chest.c
│   │   │   │   │   ├── tanstaafl_base.c
│   │   │   │   │   ├── tattoo_gun.c
│   │   │   │   │   ├── tattoo_menu_d.c
│   │   │   │   │   └── wiz_reference_book.c
│   │   │   │   └── adm.c
│   │   │   ├── ChiTown
│   │   │   │   ├── areas
│   │   │   │   │   ├── bandit_camp.c
│   │   │   │   │   ├── burb_chop_shop.c
│   │   │   │   │   ├── burb_dogboy_checkpoint.c
│   │   │   │   │   ├── burb_fence_stall.c
│   │   │   │   │   ├── burb_flophouse.c
│   │   │   │   │   ├── burb_gang_corner.c
│   │   │   │   │   ├── burb_night_market.c
│   │   │   │   │   ├── burb_preacher_square.c
│   │   │   │   │   ├── burb_refugee_camp.c
│   │   │   │   │   ├── burb_rooftops.c
│   │   │   │   │   ├── burb_smuggler_crawl.c
│   │   │   │   │   ├── burb_soup_kitchen.c
│   │   │   │   │   ├── burb_tenements.c
│   │   │   │   │   ├── burb_wall_shadow.c
│   │   │   │   │   ├── burb_water_pump.c
│   │   │   │   │   ├── chitown_alley.c
│   │   │   │   │   ├── chitown_black_market.c
│   │   │   │   │   ├── chitown_boulevard.c
│   │   │   │   │   ├── chitown_burb_row.c
│   │   │   │   │   ├── chitown_burbs.c
│   │   │   │   │   ├── chitown_burb_shanty.c
│   │   │   │   │   ├── chitown_clinic.c
│   │   │   │   │   ├── chitown_dead_end_junction.c
│   │   │   │   │   ├── chitown_gate.c
│   │   │   │   │   ├── chitown_hydra_lair.c
│   │   │   │   │   ├── chitown_hydra_treasure.c
│   │   │   │   │   ├── chitown_lower_market.c
│   │   │   │   │   ├── chitown_maintenance_tunnel.c
│   │   │   │   │   ├── chitown_market.c
│   │   │   │   │   ├── chitown_officer_row.c
│   │   │   │   │   ├── chitown_sewer_entrance.c
│   │   │   │   │   ├── chitown_sewer_overflow.c
│   │   │   │   │   ├── chitown_sewer_run.c
│   │   │   │   │   ├── chitown_start.c
│   │   │   │   │   ├── cs_apartment_lobby.c
│   │   │   │   │   ├── cs_armory.c
│   │   │   │   │   ├── cs_barracks.c
│   │   │   │   │   ├── cs_civilian_arms.c
│   │   │   │   │   ├── cs_command_plaza.c
│   │   │   │   │   ├── cs_dogboy_kennels.c
│   │   │   │   │   ├── cs_iss_checkpoint.c
│   │   │   │   │   ├── cs_outfitter_shop.c
│   │   │   │   │   ├── cs_propaganda_hall.c
│   │   │   │   │   ├── cs_psi_stalker_post.c
│   │   │   │   │   ├── cs_ration_depot.c
│   │   │   │   │   ├── cs_recruitment.c
│   │   │   │   │   ├── cs_residential.c
│   │   │   │   │   ├── cs_samas_hangar.c
│   │   │   │   │   ├── cs_secure_wing.c
│   │   │   │   │   ├── cs_service_hatch.c
│   │   │   │   │   ├── cs_shopping_arcade.c
│   │   │   │   │   └── illinois_road.c
│   │   │   │   ├── equipment
│   │   │   │   │   ├── chitown_contraband.c
│   │   │   │   │   ├── cs_dog_tag.c
│   │   │   │   │   └── fake_cs_id.c
│   │   │   │   ├── monsters
│   │   │   │   │   ├── bandit.c
│   │   │   │   │   ├── burb_dog_boy.c
│   │   │   │   │   ├── burb_thug.c
│   │   │   │   │   ├── chitown_fence.c
│   │   │   │   │   ├── chitown_ganger.c
│   │   │   │   │   ├── chitown_tunnel_rat.c
│   │   │   │   │   └── cs_juicer.c
│   │   │   │   └── npcs
│   │   │   │       ├── burb_mechanic.c
│   │   │   │       ├── burb_preacher.c
│   │   │   │       ├── burb_refugee.c
│   │   │   │       ├── burb_soup_cook.c
│   │   │   │       ├── chitown_doctor.c
│   │   │   │       ├── contraband_dealer.c
│   │   │   │       ├── cs_arms_clerk.c
│   │   │   │       ├── cs_citizen.c
│   │   │   │       ├── cs_dog_boy.c
│   │   │   │       ├── cs_grunt.c
│   │   │   │       ├── cs_iss_clerk.c
│   │   │   │       ├── cs_outfitter.c
│   │   │   │       ├── cs_patrol.c
│   │   │   │       ├── cs_psi_stalker.c
│   │   │   │       ├── cs_quartermaster.c
│   │   │   │       ├── cs_ration_clerk.c
│   │   │   │       ├── cs_recruiter.c
│   │   │   │       ├── cs_rift_liaison.c
│   │   │   │       ├── cs_samas_pilot.c
│   │   │   │       ├── cs_sergeant.c
│   │   │   │       └── dead_boy_guard.c
│   │   │   ├── DemonPlane
│   │   │   │   ├── areas
│   │   │   │   │   ├── demon_sliver_corridor.c
│   │   │   │   │   ├── demon_sliver_depths.c
│   │   │   │   │   ├── demon_sliver_fissure.c
│   │   │   │   │   ├── demon_sliver_gate.c
│   │   │   │   │   ├── demon_sliver_nest.c
│   │   │   │   │   ├── demon_sliver_pit.c
│   │   │   │   │   ├── demon_sliver_scar.c
│   │   │   │   │   ├── demon_sliver_shrine.c
│   │   │   │   │   └── demon_sliver_wastes.c
│   │   │   │   ├── equipment
│   │   │   │   │   ├── demon_ichor_vial.c
│   │   │   │   │   └── rift_shard_amulet.c
│   │   │   │   ├── monsters
│   │   │   │   │   ├── demon_sliver_alpha.c
│   │   │   │   │   ├── demon_sliver_hunter.c
│   │   │   │   │   └── demon_sliver_stalker.c
│   │   │   │   └── npcs
│   │   │   │       └── ley_researcher.c
│   │   │   ├── Horton
│   │   │   │   ├── areas
│   │   │   │   │   ├── horton_abandoned_farmstead.c
│   │   │   │   │   ├── horton_ancient_grove.c
│   │   │   │   │   ├── horton_bandit_camp.c
│   │   │   │   │   ├── horton_cornfield_ruins.c
│   │   │   │   │   ├── horton_creek_crossing.c
│   │   │   │   │   ├── horton_forest_deep.c
│   │   │   │   │   ├── horton_forest_edge.c
│   │   │   │   │   ├── horton_hermit_cabin.c
│   │   │   │   │   ├── horton_hunting_blind.c
│   │   │   │   │   ├── horton_inn.c
│   │   │   │   │   ├── horton_ley_glade.c
│   │   │   │   │   ├── horton_logging_road.c
│   │   │   │   │   ├── horton_main_street.c
│   │   │   │   │   ├── horton_marsh_edge.c
│   │   │   │   │   ├── horton_outskirts.c
│   │   │   │   │   ├── horton_overgrown_orchard.c
│   │   │   │   │   ├── horton_pine_hollow.c
│   │   │   │   │   ├── horton_quarry_floor.c
│   │   │   │   │   ├── horton_quarry_rim.c
│   │   │   │   │   ├── horton_rail_spur.c
│   │   │   │   │   ├── horton_scrub_ridge.c
│   │   │   │   │   ├── horton_station.c
│   │   │   │   │   ├── horton_store.c
│   │   │   │   │   ├── horton_wilderness_trail.c
│   │   │   │   │   ├── horton_wolf_den.c
│   │   │   │   │   ├── horton_wrecked_convoy.c
│   │   │   │   │   └── hovertrain.c
│   │   │   │   ├── equipment
│   │   │   │   │   ├── basic_medkit.c
│   │   │   │   │   └── salvaged_armor_plate.c
│   │   │   │   ├── monsters
│   │   │   │   │   ├── feral_boar.c
│   │   │   │   │   ├── horton_militia_guard.c
│   │   │   │   │   ├── horton_scavenger.c
│   │   │   │   │   ├── quarry_lurker.c
│   │   │   │   │   ├── trail_bandit.c
│   │   │   │   │   └── wild_dog_pack.c
│   │   │   │   └── npcs
│   │   │   │       ├── horton_hermit.c
│   │   │   │       ├── horton_innkeeper.c
│   │   │   │       └── horton_shopkeeper.c
│   │   │   ├── Lazlo
│   │   │   │   ├── areas
│   │   │   │   │   ├── lazlo_gate.c
│   │   │   │   │   ├── lazlo_ley_shrine.c
│   │   │   │   │   ├── lazlo_library.c
│   │   │   │   │   ├── lazlo_magic_quarter.c
│   │   │   │   │   ├── lazlo_market.c
│   │   │   │   │   ├── lazlo_shoreline.c
│   │   │   │   │   └── lazlo_square.c
│   │   │   │   ├── monsters
│   │   │   │   │   ├── lazlo_feral_dog.c
│   │   │   │   │   └── ley_line_instructor.c
│   │   │   │   ├── npcs
│   │   │   │   │   ├── lazlo_gate_guard.c
│   │   │   │   │   ├── lazlo_ley_priest.c
│   │   │   │   │   ├── lazlo_mage_vendor.c
│   │   │   │   │   └── lazlo_scholar.c
│   │   │   │   └── lazlo_welcome.c
│   │   │   ├── LoneStar
│   │   │   │   ├── areas
│   │   │   │   │   ├── lone_star_armory.c
│   │   │   │   │   ├── lone_star_barracks.c
│   │   │   │   │   ├── lone_star_bunkroom.c
│   │   │   │   │   ├── lone_star_canteen.c
│   │   │   │   │   ├── lone_star_cold_storage.c
│   │   │   │   │   ├── lone_star_containment.c
│   │   │   │   │   ├── lone_star_culvert_mouth.c
│   │   │   │   │   ├── lone_star_escape_tunnel.c
│   │   │   │   │   ├── lone_star_gate.c
│   │   │   │   │   ├── lone_star_gene_vault.c
│   │   │   │   │   ├── lone_star_incubation.c
│   │   │   │   │   ├── lone_star_kennels.c
│   │   │   │   │   ├── lone_star_kennel_yard.c
│   │   │   │   │   ├── lone_star_lab.c
│   │   │   │   │   ├── lone_star_motorpool.c
│   │   │   │   │   ├── lone_star_observation.c
│   │   │   │   │   ├── lone_star_perimeter.c
│   │   │   │   │   ├── lone_star_plaza.c
│   │   │   │   │   ├── lone_star_sump.c
│   │   │   │   │   ├── lone_star_supply.c
│   │   │   │   │   ├── lone_star_support_row.c
│   │   │   │   │   ├── lone_star_training_ring.c
│   │   │   │   │   └── lone_star_tunnel_junction.c
│   │   │   │   ├── equipment
│   │   │   │   ├── monsters
│   │   │   │   │   ├── ls_dog_boy_pup.c
│   │   │   │   │   ├── ls_failed_specimen.c
│   │   │   │   │   └── ls_mutant_subject.c
│   │   │   │   └── npcs
│   │   │   │       ├── coalition_scientist.c
│   │   │   │       ├── cs_dog_boy_guard.c
│   │   │   │       ├── ls_canteen_keeper.c
│   │   │   │       ├── ls_civilian_contractor.c
│   │   │   │       ├── ls_kennel_master.c
│   │   │   │       └── ls_supply_officer.c
│   │   │   ├── NewCamelot
│   │   │   │   ├── areas
│   │   │   │   │   ├── academy_dormitory.c
│   │   │   │   │   ├── academy_lecture_hall.c
│   │   │   │   │   ├── academy_library.c
│   │   │   │   │   ├── academy_quad.c
│   │   │   │   │   ├── academy_summoning.c
│   │   │   │   │   ├── black_forest.c
│   │   │   │   │   ├── blacksmith_forge.c
│   │   │   │   │   ├── camelot_armory.c
│   │   │   │   │   ├── camelot_garden.c
│   │   │   │   │   ├── camelot_gate.c
│   │   │   │   │   ├── camelot_square.c
│   │   │   │   │   ├── camelot_tavern.c
│   │   │   │   │   ├── camelot_wizard_tower.c
│   │   │   │   │   ├── castle_armory_royal.c
│   │   │   │   │   ├── castle_barracks.c
│   │   │   │   │   ├── castle_courtyard.c
│   │   │   │   │   ├── castle_dungeon.c
│   │   │   │   │   ├── castle_gatehouse.c
│   │   │   │   │   ├── castle_great_hall.c
│   │   │   │   │   ├── castle_throne_room.c
│   │   │   │   │   ├── castle_tower_top.c
│   │   │   │   │   ├── castle_war_room.c
│   │   │   │   │   ├── chapel_of_light.c
│   │   │   │   │   ├── europe_road.c
│   │   │   │   │   ├── forest_lake.c
│   │   │   │   │   ├── forest_lake_far_shore.c
│   │   │   │   │   ├── market_street1.c
│   │   │   │   │   ├── market_street2.c
│   │   │   │   │   ├── newcamelot_start.c
│   │   │   │   │   ├── stables.c
│   │   │   │   │   └── training_grounds.c
│   │   │   │   ├── equipment
│   │   │   │   │   ├── camelot_longsword.c
│   │   │   │   │   ├── chain_mail.c
│   │   │   │   │   ├── dragonfire_lance.c
│   │   │   │   │   ├── knight_shield.c
│   │   │   │   │   ├── nexus_stone.c
│   │   │   │   │   ├── short_sword.c
│   │   │   │   │   └── spell_tome.c
│   │   │   │   └── npcs
│   │   │   │       ├── academy_headmaster.c
│   │   │   │       ├── academy_student.c
│   │   │   │       ├── camelot_armorer.c
│   │   │   │       ├── camelot_blacksmith.c
│   │   │   │       ├── camelot_mage.c
│   │   │   │       ├── camelot_squire.c
│   │   │   │       ├── chapel_priestess.c
│   │   │   │       ├── dragon_knight.c
│   │   │   │       ├── dungeon_prisoner.c
│   │   │   │       ├── king_arrthuu.c
│   │   │   │       ├── lesser_fire_elemental.c
│   │   │   │       ├── mrrlyn.c
│   │   │   │       ├── pendragon_barkeep.c
│   │   │   │       ├── quartermaster_knight.c
│   │   │   │       ├── royal_knight_guard.c
│   │   │   │       ├── stable_hand.c
│   │   │   │       └── young_dragon.c
│   │   │   ├── NGR
│   │   │   │   ├── areas
│   │   │   │   │   ├── ngr_biergarten.c
│   │   │   │   │   ├── ngr_checkpoint.c
│   │   │   │   │   ├── ngr_frontline_trench.c
│   │   │   │   │   ├── NGR_garrison.c
│   │   │   │   │   ├── NGR_gate.c
│   │   │   │   │   ├── NGR_market_district.c
│   │   │   │   │   ├── ngr_no_mans_land.c
│   │   │   │   │   ├── ngr_patrol_road.c
│   │   │   │   │   ├── ngr_plaza.c
│   │   │   │   │   ├── ngr_residential.c
│   │   │   │   │   ├── NGR_triax_depot.c
│   │   │   │   │   ├── ngr_triax_showroom.c
│   │   │   │   │   ├── ngr_underground_bunker.c
│   │   │   │   │   └── ngr_watchtower.c
│   │   │   │   ├── equipment
│   │   │   │   │   └── triax_helmet.c
│   │   │   │   └── npcs
│   │   │   │       ├── gargoyle_raider.c
│   │   │   │       ├── ngr_arms_dealer.c
│   │   │   │       ├── ngr_barkeep.c
│   │   │   │       ├── ngr_border_guard.c
│   │   │   │       ├── ngr_civilian.c
│   │   │   │       ├── ngr_elite_soldier.c
│   │   │   │       ├── ngr_gate_guard.c
│   │   │   │       ├── ngr_patrol_soldier.c
│   │   │   │       ├── ngr_sniper.c
│   │   │   │       └── triax_rep.c
│   │   │   ├── Praxis
│   │   │   │   ├── adm
│   │   │   │   │   ├── access.c
│   │   │   │   │   ├── audit_equipment.txt
│   │   │   │   │   ├── audit_npcs.txt
│   │   │   │   │   ├── audit_occs.txt
│   │   │   │   │   ├── audit_psionics.txt
│   │   │   │   │   ├── audit_races.txt
│   │   │   │   │   ├── audit_skills.txt
│   │   │   │   │   ├── audit_spells.txt
│   │   │   │   │   ├── gap_report.txt
│   │   │   │   │   ├── master.c
│   │   │   │   │   └── master_gap_report.txt
│   │   │   │   ├── areas
│   │   │   │   │   ├── splynn
│   │   │   │   │   │   ├── alvurron
│   │   │   │   │   │   │   ├── alvurron_blood_pit.c
│   │   │   │   │   │   │   ├── alvurron_deep_rift.c
│   │   │   │   │   │   │   ├── alvurron_gate.c
│   │   │   │   │   │   │   ├── alvurron_inner_district.c
│   │   │   │   │   │   │   ├── alvurron_market.c
│   │   │   │   │   │   │   ├── alvurron_quarter.c
│   │   │   │   │   │   │   ├── alvurron_rift_chamber.c
│   │   │   │   │   │   │   ├── alvurron_roost.c
│   │   │   │   │   │   │   ├── alvurron_slave_pens.c
│   │   │   │   │   │   │   ├── alvurron_spire.c
│   │   │   │   │   │   │   ├── alvurron_spire_top.c
│   │   │   │   │   │   │   ├── alvurron_square.c
│   │   │   │   │   │   │   ├── alvurron_temple.c
│   │   │   │   │   │   │   ├── alvurron_undermarket.c
│   │   │   │   │   │   │   └── alvurron_warrens.c
│   │   │   │   │   │   ├── ocean
│   │   │   │   │   │   │   ├── ocean_rift_deep.c
│   │   │   │   │   │   │   ├── ocean_rift_floor.c
│   │   │   │   │   │   │   └── ocean_rift_shallows.c
│   │   │   │   │   │   ├── catacomb_chamber.c
│   │   │   │   │   │   ├── catacomb_entrance.c
│   │   │   │   │   │   ├── catacomb_hall.c
│   │   │   │   │   │   ├── catacomb_tomb.c
│   │   │   │   │   │   ├── ocean_rift.c
│   │   │   │   │   │   ├── preserve_blind.c
│   │   │   │   │   │   ├── preserve_bonefield.c
│   │   │   │   │   │   ├── preserve_cagepit.c
│   │   │   │   │   │   ├── preserve_cagerow.c
│   │   │   │   │   │   ├── preserve_catwalk.c
│   │   │   │   │   │   ├── preserve_den.c
│   │   │   │   │   │   ├── preserve_hive.c
│   │   │   │   │   │   ├── preserve_hivecore.c
│   │   │   │   │   │   ├── preserve_hivedeep.c
│   │   │   │   │   │   ├── preserve_hollow.c
│   │   │   │   │   │   ├── preserve_nest.c
│   │   │   │   │   │   ├── preserve_nook.c
│   │   │   │   │   │   ├── preserve_reeds.c
│   │   │   │   │   │   ├── preserve_ridge.c
│   │   │   │   │   │   ├── preserve_saddle.c
│   │   │   │   │   │   ├── preserve_snag.c
│   │   │   │   │   │   ├── preserve_snareline.c
│   │   │   │   │   │   ├── preserve_sunken.c
│   │   │   │   │   │   ├── preserve_switchback.c
│   │   │   │   │   │   ├── preserve_thicket.c
│   │   │   │   │   │   ├── preserve_thornbrake.c
│   │   │   │   │   │   ├── preserve_trail.c
│   │   │   │   │   │   ├── preserve_waterhole.c
│   │   │   │   │   │   ├── rockys_bar_splynn.c
│   │   │   │   │   │   ├── shadow_slave.c
│   │   │   │   │   │   ├── slave_pens.c
│   │   │   │   │   │   ├── splynn_back_alley.c
│   │   │   │   │   │   ├── splynn_black_market.c
│   │   │   │   │   │   ├── splynn_clan_hall.c
│   │   │   │   │   │   ├── splynn_docks.c
│   │   │   │   │   │   ├── splynn_east_bazaar.c
│   │   │   │   │   │   ├── splynn_entry.c
│   │   │   │   │   │   ├── splynn_forest_edge.c
│   │   │   │   │   │   ├── splynn_guild_row.c
│   │   │   │   │   │   ├── splynn_market.c
│   │   │   │   │   │   ├── splynn_pits.c
│   │   │   │   │   │   ├── splynn_preserves.c
│   │   │   │   │   │   ├── splynn_south_road.c
│   │   │   │   │   │   ├── splynn_underground.c
│   │   │   │   │   │   └── splynn_west_road.c
│   │   │   │   │   ├── back_alley.c
│   │   │   │   │   ├── coalition_checkpoint.c
│   │   │   │   │   ├── collapsed_building.c
│   │   │   │   │   ├── cs_territory.c
│   │   │   │   │   ├── cutters_shop.c
│   │   │   │   │   ├── deserter_hideout.c
│   │   │   │   │   ├── general_store.c
│   │   │   │   │   ├── highway_ruins.c
│   │   │   │   │   ├── hydra_lair.c
│   │   │   │   │   ├── hydra_treasure.c
│   │   │   │   │   ├── long_road.c
│   │   │   │   │   ├── market_street.c
│   │   │   │   │   ├── monument_square.c
│   │   │   │   │   ├── nexus_approach.c
│   │   │   │   │   ├── nexus_dimensional_tear.c
│   │   │   │   │   ├── old_highway.c
│   │   │   │   │   ├── riffys_bar.c
│   │   │   │   │   ├── rockys_bar.c
│   │   │   │   │   ├── test_zone.c
│   │   │   │   │   └── weapons_shop.c
│   │   │   │   ├── attic
│   │   │   │   │   └── tmp_hos
│   │   │   │   ├── cemetary
│   │   │   │   │   ├── mon
│   │   │   │   │   │   ├── ghost3.c
│   │   │   │   │   │   ├── ghost4.c
│   │   │   │   │   │   ├── ghost5.c
│   │   │   │   │   │   ├── ghost6.c
│   │   │   │   │   │   ├── ghost8.c
│   │   │   │   │   │   └── ghost.c
│   │   │   │   │   ├── grave_yard2.c
│   │   │   │   │   ├── grave_yard3.c
│   │   │   │   │   ├── grave_yard4.c
│   │   │   │   │   ├── grave_yard5.c
│   │   │   │   │   ├── grave_yard6.c
│   │   │   │   │   ├── grave_yard7.c
│   │   │   │   │   ├── grave_yard8.c
│   │   │   │   │   └── grave_yard.c
│   │   │   │   ├── data
│   │   │   │   │   ├── booth_question
│   │   │   │   │   ├── booth_votes.o
│   │   │   │   │   ├── cleric_crypt.o
│   │   │   │   │   ├── properties.o
│   │   │   │   │   ├── properties.t
│   │   │   │   │   └── wall.o
│   │   │   │   ├── death
│   │   │   │   │   └── death_room.c
│   │   │   │   ├── equipment
│   │   │   │   │   ├── ammo
│   │   │   │   │   │   ├── e_clip_long.c
│   │   │   │   │   │   └── e_clip_standard.c
│   │   │   │   │   ├── cybernetics
│   │   │   │   │   │   ├── biocomp_implant.c
│   │   │   │   │   │   ├── clock_calendar.c
│   │   │   │   │   │   ├── cyber_arm_left.c
│   │   │   │   │   │   ├── cyber_armor.c
│   │   │   │   │   │   ├── cyber_arm_right.c
│   │   │   │   │   │   ├── cyber_ear.c
│   │   │   │   │   │   ├── cyber_eye_optic.c
│   │   │   │   │   │   ├── cyber_legs.c
│   │   │   │   │   │   ├── cyber_lung.c
│   │   │   │   │   │   ├── gyro_compass_implant.c
│   │   │   │   │   │   ├── headjack.c
│   │   │   │   │   │   ├── irmss_kit.c
│   │   │   │   │   │   ├── language_translator.c
│   │   │   │   │   │   ├── multi_optic_eye.c
│   │   │   │   │   │   ├── neural_net_implant.c
│   │   │   │   │   │   ├── oxygen_cell.c
│   │   │   │   │   │   ├── radio_implant.c
│   │   │   │   │   │   └── targeting_eye.c
│   │   │   │   │   ├── grenades
│   │   │   │   │   │   ├── frag_grenade.c
│   │   │   │   │   │   ├── plasma_grenade.c
│   │   │   │   │   │   └── smoke_grenade.c
│   │   │   │   │   ├── magic_items
│   │   │   │   │   │   ├── healing_charm.c
│   │   │   │   │   │   └── rune_dagger.c
│   │   │   │   │   ├── power_armor
│   │   │   │   │   │   ├── CS_Terror_Trooper.c
│   │   │   │   │   │   ├── GB_Glitter_Boy.c
│   │   │   │   │   │   ├── kittani_power_armor.c
│   │   │   │   │   │   ├── NG_EX10_Samson.c
│   │   │   │   │   │   └── PA-06A_SAMAS.c
│   │   │   │   │   ├── splynn
│   │   │   │   │   │   ├── alien_hide.c
│   │   │   │   │   │   ├── exotic_energy_clip.c
│   │   │   │   │   │   ├── net_gun.c
│   │   │   │   │   │   ├── plasma_pistol.c
│   │   │   │   │   │   ├── predator_armor.c
│   │   │   │   │   │   ├── psi_sword_hilt.c
│   │   │   │   │   │   └── splynn_map.c
│   │   │   │   │   ├── vehicles
│   │   │   │   │   │   └── ng_wastelander.c
│   │   │   │   │   ├── alvurron_key.c
│   │   │   │   │   ├── armor_talisman.c
│   │   │   │   │   ├── atm_card.c
│   │   │   │   │   ├── backpack.c
│   │   │   │   │   ├── bandit_patch.c
│   │   │   │   │   ├── bone_dust.c
│   │   │   │   │   ├── boom_gun.c
│   │   │   │   │   ├── c12_laser_rifle.c
│   │   │   │   │   ├── c18_pistol.c
│   │   │   │   │   ├── chain_mail.c
│   │   │   │   │   ├── coalition_grunt_armor.c
│   │   │   │   │   ├── computer_pad.c
│   │   │   │   │   ├── cp30_laser.c
│   │   │   │   │   ├── cp40_laser.c
│   │   │   │   │   ├── dead_boy_armor.c
│   │   │   │   │   ├── e_clip.c
│   │   │   │   │   ├── environmental_body_armor.c
│   │   │   │   │   ├── faction_amulet.c
│   │   │   │   │   ├── field_journal.c
│   │   │   │   │   ├── first_aid_kit.c
│   │   │   │   │   ├── flame_hilt.c
│   │   │   │   │   ├── ghostly_katana.c
│   │   │   │   │   ├── handcuffs.c
│   │   │   │   │   ├── hover_cycle.c
│   │   │   │   │   ├── hunting_rifle.c
│   │   │   │   │   ├── id_card.c
│   │   │   │   │   ├── kittani_laser.c
│   │   │   │   │   ├── knife.c
│   │   │   │   │   ├── leather_glove.c
│   │   │   │   │   ├── leather_jacket.c
│   │   │   │   │   ├── locked_chest.c
│   │   │   │   │   ├── medical_bag.c
│   │   │   │   │   ├── military_radio.c
│   │   │   │   │   ├── ngr_body_armor.c
│   │   │   │   │   ├── ngr_rail_gun.c
│   │   │   │   │   ├── obsidian_blade.c
│   │   │   │   │   ├── personal_radio.c
│   │   │   │   │   ├── ppe_battery.c
│   │   │   │   │   ├── purified_water.c
│   │   │   │   │   ├── rations.c
│   │   │   │   │   ├── rebreather.c
│   │   │   │   │   ├── ritual_components.c
│   │   │   │   │   ├── rope.c
│   │   │   │   │   ├── salvage_item.c
│   │   │   │   │   ├── science_kit.c
│   │   │   │   │   ├── short_sword.c
│   │   │   │   │   ├── shovel.c
│   │   │   │   │   ├── spell_components.c
│   │   │   │   │   ├── supply_crate.c
│   │   │   │   │   ├── sword_of_atlantis.c
│   │   │   │   │   ├── tool_kit.c
│   │   │   │   │   ├── trained_hawk.c
│   │   │   │   │   ├── tx5_particle_rifle.c
│   │   │   │   │   ├── vibro_knife.c
│   │   │   │   │   └── vibro_sword.c
│   │   │   │   ├── monsters
│   │   │   │   │   ├── alvurron_champion.c
│   │   │   │   │   ├── alvurron_dealer.c
│   │   │   │   │   ├── alvurron_enforcer.c
│   │   │   │   │   ├── alvurron_overlord_boss.c
│   │   │   │   │   ├── alvurron_overseer.c
│   │   │   │   │   ├── alvurron_rift_wisp.c
│   │   │   │   │   ├── alvurron_trader.c
│   │   │   │   │   ├── alvurron_watcher.c
│   │   │   │   │   ├── atlantean_guardian.c
│   │   │   │   │   ├── brodkil.c
│   │   │   │   │   ├── brodkil_demon.c
│   │   │   │   │   ├── catacomb_wight.c
│   │   │   │   │   ├── clan_recruiter.c
│   │   │   │   │   ├── coalition_dead_boy.c
│   │   │   │   │   ├── coalition_grunt.c
│   │   │   │   │   ├── cutter.c
│   │   │   │   │   ├── d_bee_wanderer.c
│   │   │   │   │   ├── deevil_scout.c
│   │   │   │   │   ├── deserter.c
│   │   │   │   │   ├── gargoyle.c
│   │   │   │   │   ├── gargoyle_raider.c
│   │   │   │   │   ├── general_vendor.c
│   │   │   │   │   ├── highway_bandit.c
│   │   │   │   │   ├── hound_demon.c
│   │   │   │   │   ├── hydra.c
│   │   │   │   │   ├── jungle_stalker.c
│   │   │   │   │   ├── kittani_merchant.c
│   │   │   │   │   ├── kittani_overseer.c
│   │   │   │   │   ├── kittani_warrior.c
│   │   │   │   │   ├── kydian_overlord.c
│   │   │   │   │   ├── magic_bush.c
│   │   │   │   │   ├── minor_demon.c
│   │   │   │   │   ├── moxim.c
│   │   │   │   │   ├── ocean_creature.c
│   │   │   │   │   ├── ocean_leviathan.c
│   │   │   │   │   ├── plains_wolf.c
│   │   │   │   │   ├── preserve_beast.c
│   │   │   │   │   ├── preserve_captive.c
│   │   │   │   │   ├── rat.c
│   │   │   │   │   ├── rocky_barkeep.c
│   │   │   │   │   ├── sea_serpent.c
│   │   │   │   │   ├── splugorth_minion.c
│   │   │   │   │   ├── splynn_clip_vendor.c
│   │   │   │   │   ├── splynn_slave.c
│   │   │   │   │   ├── weapons_vendor.c
│   │   │   │   │   ├── wild_vampire.c
│   │   │   │   │   └── xiticix_warrior.c
│   │   │   │   ├── mountains
│   │   │   │   │   ├── chamber1.c
│   │   │   │   │   ├── chamber2.c
│   │   │   │   │   ├── chamber3.c
│   │   │   │   │   ├── chamber4.c
│   │   │   │   │   ├── entrance.c
│   │   │   │   │   ├── temp.c
│   │   │   │   │   ├── tunnel1.c
│   │   │   │   │   ├── tunnel2.c
│   │   │   │   │   └── tunnel3.c
│   │   │   │   ├── npcs
│   │   │   │   │   ├── black_market_vendor.c
│   │   │   │   │   ├── cyber_doc_medic.c
│   │   │   │   │   ├── falconer.c
│   │   │   │   │   ├── sal.c
│   │   │   │   │   ├── tattoo_master.c
│   │   │   │   │   └── thurtea_hawk.c
│   │   │   │   ├── obj
│   │   │   │   │   ├── armor
│   │   │   │   │   │   └── helm.c
│   │   │   │   │   ├── magic
│   │   │   │   │   │   ├── ball.c
│   │   │   │   │   │   └── invis.c
│   │   │   │   │   ├── misc
│   │   │   │   │   │   ├── app_board.c
│   │   │   │   │   │   ├── bag.c
│   │   │   │   │   │   ├── chest.c
│   │   │   │   │   │   ├── cold.c
│   │   │   │   │   │   ├── deed.c
│   │   │   │   │   │   ├── donation.c
│   │   │   │   │   │   ├── easter_egg.c
│   │   │   │   │   │   ├── fishing_pole.c
│   │   │   │   │   │   ├── gallows.c
│   │   │   │   │   │   ├── handcuffs.c
│   │   │   │   │   │   ├── hood.c
│   │   │   │   │   │   ├── match.c
│   │   │   │   │   │   ├── order.c
│   │   │   │   │   │   ├── pedestal.c
│   │   │   │   │   │   ├── shovel.c
│   │   │   │   │   │   ├── stone.c
│   │   │   │   │   │   ├── torch.c
│   │   │   │   │   │   ├── vial.c
│   │   │   │   │   │   └── watchtower.c
│   │   │   │   │   ├── mon
│   │   │   │   │   │   ├── armageddon.c
│   │   │   │   │   │   ├── atmos.c
│   │   │   │   │   │   ├── balrog.c
│   │   │   │   │   │   ├── beggar.c
│   │   │   │   │   │   ├── guard.c
│   │   │   │   │   │   ├── horace.c
│   │   │   │   │   │   ├── knight.c
│   │   │   │   │   │   ├── lars.c
│   │   │   │   │   │   ├── mora.c
│   │   │   │   │   │   ├── orc_shaman.c
│   │   │   │   │   │   ├── police.c
│   │   │   │   │   │   ├── receptionist.c
│   │   │   │   │   │   ├── rift_survivor.c
│   │   │   │   │   │   ├── spider.c
│   │   │   │   │   │   ├── unity.c
│   │   │   │   │   │   ├── veteran_arbiter.c
│   │   │   │   │   │   └── waitress.c
│   │   │   │   │   ├── weapon
│   │   │   │   │   │   ├── bow.c
│   │   │   │   │   │   ├── dagger.c
│   │   │   │   │   │   ├── orc_slayer.c
│   │   │   │   │   │   └── sword.c
│   │   │   │   │   └── bounty_board.c
│   │   │   │   ├── orc_valley
│   │   │   │   │   ├── chamber1.c
│   │   │   │   │   ├── chamber2.c
│   │   │   │   │   ├── guard.c
│   │   │   │   │   ├── open.c
│   │   │   │   │   ├── passage1.c
│   │   │   │   │   ├── passage2.c
│   │   │   │   │   ├── shaman.c
│   │   │   │   │   └── treasure.c
│   │   │   │   ├── quests
│   │   │   │   │   ├── chaucer.c
│   │   │   │   │   ├── cs_deserter.c
│   │   │   │   │   ├── dragon.c
│   │   │   │   │   ├── evil.c
│   │   │   │   │   ├── hellfire.c
│   │   │   │   │   ├── honor.c
│   │   │   │   │   ├── imp_crown.c
│   │   │   │   │   ├── lost_supplies.c
│   │   │   │   │   ├── orcslayer.c
│   │   │   │   │   ├── pyr_quest.c
│   │   │   │   │   ├── rocky_rumor.c
│   │   │   │   │   ├── the_blight_of_the_bog.c
│   │   │   │   │   └── zemoch.c
│   │   │   │   ├── vehicles
│   │   │   │   │   ├── ex5
│   │   │   │   │   │   ├── ex5_cargo.c
│   │   │   │   │   │   ├── ex5_cockpit.c
│   │   │   │   │   │   ├── ex5_crew_quarters.c
│   │   │   │   │   │   ├── ex5_forward_bay.c
│   │   │   │   │   │   ├── ex5_hull.c
│   │   │   │   │   │   └── ex5_portside.c
│   │   │   │   │   └── mountaineer
│   │   │   │   │       ├── mtn_bunks.c
│   │   │   │   │       ├── mtn_cab.c
│   │   │   │   │       ├── mtn_cabin.c
│   │   │   │   │       └── mtn_cargo.c
│   │   │   │   ├── adv_inner.c
│   │   │   │   ├── adv_main.c
│   │   │   │   ├── alley1.c
│   │   │   │   ├── alley2.c
│   │   │   │   ├── app_room.c
│   │   │   │   ├── arch.c
│   │   │   │   ├── bank.c
│   │   │   │   ├── bank_vault.c
│   │   │   │   ├── booth.c
│   │   │   │   ├── branches.c
│   │   │   │   ├── building.c
│   │   │   │   ├── cache.c
│   │   │   │   ├── ChangeLog
│   │   │   │   ├── chapel.c
│   │   │   │   ├── cleric_hall.c
│   │   │   │   ├── cleric_join.c
│   │   │   │   ├── cleric_vote.c
│   │   │   │   ├── commands
│   │   │   │   ├── council.c
│   │   │   │   ├── council_hall.c
│   │   │   │   ├── court_room.c
│   │   │   │   ├── crypt.c
│   │   │   │   ├── dump.c
│   │   │   │   ├── east_road1.c
│   │   │   │   ├── east_road2.c
│   │   │   │   ├── east_road3.c
│   │   │   │   ├── e_boc_la1.c
│   │   │   │   ├── e_boc_la2.c
│   │   │   │   ├── e_boc_la3.c
│   │   │   │   ├── farm.c
│   │   │   │   ├── fighter_hall.c
│   │   │   │   ├── fighter_join.c
│   │   │   │   ├── fighter_vote.c
│   │   │   │   ├── forest1.c
│   │   │   │   ├── forest2.c
│   │   │   │   ├── forest3.c
│   │   │   │   ├── freezer.c
│   │   │   │   ├── hall2.c
│   │   │   │   ├── hall3.c
│   │   │   │   ├── hall4.c
│   │   │   │   ├── hall5.c
│   │   │   │   ├── hall.c
│   │   │   │   ├── highway1.c
│   │   │   │   ├── highway2.c
│   │   │   │   ├── highway3.c
│   │   │   │   ├── hm_chamber.c
│   │   │   │   ├── hole.c
│   │   │   │   ├── hospital.c
│   │   │   │   ├── hotel.c
│   │   │   │   ├── hotelroom101.c
│   │   │   │   ├── hotelroom102.c
│   │   │   │   ├── hotelroom103.c
│   │   │   │   ├── hotelroom104.c
│   │   │   │   ├── hotelroom105.c
│   │   │   │   ├── hotelroom106.c
│   │   │   │   ├── hotelroom107.c
│   │   │   │   ├── hotelroom108.c
│   │   │   │   ├── hotelroom109.c
│   │   │   │   ├── hotelroom110.c
│   │   │   │   ├── house.c
│   │   │   │   ├── idle_supply.c
│   │   │   │   ├── immortal_hall.c
│   │   │   │   ├── inn101.c
│   │   │   │   ├── inn102.c
│   │   │   │   ├── inn103.c
│   │   │   │   ├── inn104.c
│   │   │   │   ├── inn105.c
│   │   │   │   ├── inn106.c
│   │   │   │   ├── inn107.c
│   │   │   │   ├── inn108.c
│   │   │   │   ├── inn109.c
│   │   │   │   ├── inn110.c
│   │   │   │   ├── jungle.c
│   │   │   │   ├── kataan_hall.c
│   │   │   │   ├── kataan_join.c
│   │   │   │   ├── kataan_vote.c
│   │   │   │   ├── legacy_audit_archive.txt
│   │   │   │   ├── library.c
│   │   │   │   ├── locked.c
│   │   │   │   ├── lpc_inner.c
│   │   │   │   ├── lpmud_room.c
│   │   │   │   ├── mage_hall.c
│   │   │   │   ├── mage_join.c
│   │   │   │   ├── mage_vote.c
│   │   │   │   ├── medium.c
│   │   │   │   ├── monastery.c
│   │   │   │   ├── monk_hall.c
│   │   │   │   ├── monk_join.c
│   │   │   │   ├── monk_vote.c
│   │   │   │   ├── mudlib.c
│   │   │   │   ├── n_centre1.c
│   │   │   │   ├── n_centre2.c
│   │   │   │   ├── ombud_hall.c
│   │   │   │   ├── party.c
│   │   │   │   ├── pass1.c
│   │   │   │   ├── pass2.c
│   │   │   │   ├── pass3.c
│   │   │   │   ├── pier1.c
│   │   │   │   ├── pit.c
│   │   │   │   ├── planning_room.c
│   │   │   │   ├── post.c
│   │   │   │   ├── pub.c
│   │   │   │   ├── quest_room.c
│   │   │   │   ├── rain_forest.c
│   │   │   │   ├── realty.c
│   │   │   │   ├── restaurant.c
│   │   │   │   ├── rifts_welcome.c
│   │   │   │   ├── rogue_hall.c
│   │   │   │   ├── rogue_join.c
│   │   │   │   ├── rogue_vote.c
│   │   │   │   ├── roots.c
│   │   │   │   ├── sage_room.c
│   │   │   │   ├── s_centre1.c
│   │   │   │   ├── s_centre2.c
│   │   │   │   ├── s_centre3.c
│   │   │   │   ├── s_centre4.c
│   │   │   │   ├── setter.c
│   │   │   │   ├── sewer.c
│   │   │   │   ├── sewer_ent.c
│   │   │   │   ├── sheriff.c
│   │   │   │   ├── spider_pit.c
│   │   │   │   ├── square.c
│   │   │   │   ├── stairs.c
│   │   │   │   ├── storage.c
│   │   │   │   ├── sun1.c
│   │   │   │   ├── sun2.c
│   │   │   │   ├── supply2.c
│   │   │   │   ├── supply.c
│   │   │   │   ├── torture_room.c
│   │   │   │   ├── town_hall.c
│   │   │   │   ├── trunk.c
│   │   │   │   ├── unnamed1.c
│   │   │   │   ├── void.c
│   │   │   │   ├── wall.c
│   │   │   │   ├── w_boc_la1.c
│   │   │   │   ├── w_boc_la2.c
│   │   │   │   ├── w_boc_la3.c
│   │   │   │   ├── west_road1.c
│   │   │   │   ├── west_road2.c
│   │   │   │   ├── west_road3.c
│   │   │   │   ├── wild1.c
│   │   │   │   ├── wild2.c
│   │   │   │   └── yard.c
│   │   │   ├── PuertoAngel
│   │   │   │   ├── areas
│   │   │   │   │   ├── pa_beach.c
│   │   │   │   │   ├── pa_cantina.c
│   │   │   │   │   ├── pa_church.c
│   │   │   │   │   ├── pa_harbor.c
│   │   │   │   │   ├── pa_main_street.c
│   │   │   │   │   ├── pa_outskirts.c
│   │   │   │   │   └── pa_residential.c
│   │   │   │   ├── equipment
│   │   │   │   │   ├── holy_water_vial.c
│   │   │   │   │   ├── silver_cross.c
│   │   │   │   │   ├── silver_dagger.c
│   │   │   │   │   └── wooden_stake.c
│   │   │   │   └── npcs
│   │   │   │       ├── pa_barkeep.c
│   │   │   │       ├── pa_dockmaster.c
│   │   │   │       ├── pa_hunter.c
│   │   │   │       └── pa_secondary_vampire.c
│   │   │   ├── Tolkeen
│   │   │   │   ├── areas
│   │   │   │   │   ├── tolkeen_approach.c
│   │   │   │   │   ├── tolkeen_armory.c
│   │   │   │   │   ├── tolkeen_barracks.c
│   │   │   │   │   ├── tolkeen_clinic.c
│   │   │   │   │   ├── tolkeen_component_alley.c
│   │   │   │   │   ├── tolkeen_crypt_depths.c
│   │   │   │   │   ├── tolkeen_crypt_entrance.c
│   │   │   │   │   ├── tolkeen_crypt_hall.c
│   │   │   │   │   ├── tolkeen_crypt_vault.c
│   │   │   │   │   ├── tolkeen_crypt_ward.c
│   │   │   │   │   ├── tolkeen_cs_trench.c
│   │   │   │   │   ├── tolkeen_gate.c
│   │   │   │   │   ├── tolkeen_inn.c
│   │   │   │   │   ├── tolkeen_inn_rooms.c
│   │   │   │   │   ├── tolkeen_library.c
│   │   │   │   │   ├── tolkeen_mage_quarter.c
│   │   │   │   │   ├── tolkeen_market.c
│   │   │   │   │   ├── tolkeen_no_mans_land.c
│   │   │   │   │   ├── tolkeen_refugee_yard.c
│   │   │   │   │   ├── tolkeen_ruined_suburb.c
│   │   │   │   │   ├── tolkeen_scorched_field.c
│   │   │   │   │   ├── tolkeen_siege_edge.c
│   │   │   │   │   ├── tolkeen_square.c
│   │   │   │   │   ├── tolkeen_tower_ne.c
│   │   │   │   │   ├── tolkeen_tower_nw.c
│   │   │   │   │   ├── tolkeen_tw_workshop.c
│   │   │   │   │   ├── tolkeen_wall_e.c
│   │   │   │   │   ├── tolkeen_wall_n.c
│   │   │   │   │   └── tolkeen_wall_w.c
│   │   │   │   ├── equipment
│   │   │   │   │   └── ward_charm.c
│   │   │   │   ├── monsters
│   │   │   │   │   ├── crypt_shade.c
│   │   │   │   │   ├── cs_siege_scout.c
│   │   │   │   │   └── tolkeen_ward_golem.c
│   │   │   │   └── npcs
│   │   │   │       ├── tolkeen_gate_guard.c
│   │   │   │       ├── tolkeen_innkeeper.c
│   │   │   │       ├── tolkeen_mage.c
│   │   │   │       ├── tolkeen_militia.c
│   │   │   │       ├── tolkeen_refugee.c
│   │   │   │       └── tolkeen_tw_vendor.c
│   │   │   ├── wizards
│   │   │   │   ├── thurtea_test
│   │   │   │   │   ├── archivist.c
│   │   │   │   │   ├── canvas_shirt.c
│   │   │   │   │   ├── canvas_trousers.c
│   │   │   │   │   ├── field_cap.c
│   │   │   │   │   ├── LLM_BREADCRUMB.md
│   │   │   │   │   ├── quartermaster.c
│   │   │   │   │   ├── README.md
│   │   │   │   │   ├── rift_crawler.c
│   │   │   │   │   ├── signet_ring.c
│   │   │   │   │   ├── sparring_drone.c
│   │   │   │   │   ├── test_arena.c
│   │   │   │   │   ├── test_hall.c
│   │   │   │   │   ├── travel_boots.c
│   │   │   │   │   ├── utility_belt.c
│   │   │   │   │   └── work_gloves.c
│   │   │   │   ├── admin_chamber.c
│   │   │   │   ├── coding_workshop.c
│   │   │   │   ├── combat_test.c
│   │   │   │   ├── domain_office.c
│   │   │   │   ├── hallway.c
│   │   │   │   ├── lounge.c
│   │   │   │   ├── lounge_eclip_vendor.c
│   │   │   │   ├── lounge_pet_trader.c
│   │   │   │   ├── lounge_rifle_vendor.c
│   │   │   │   ├── lounge_sword_vendor.c
│   │   │   │   ├── mdc_training_dummy.c
│   │   │   │   ├── restore_crystal.c
│   │   │   │   ├── rift_opener.c
│   │   │   │   ├── rp_study.c
│   │   │   │   └── training_dummy.c
│   │   │   ├── HOWTO.md
│   │   │   ├── LLM_BREADCRUMB.md
│   │   │   ├── README.md
│   │   │   └── WHY.md
│   │   ├── estates
│   │   ├── ftp
│   │   ├── include
│   │   │   ├── chat.h
│   │   │   └── vehicle.h
│   │   ├── log
│   │   │   ├── adm
│   │   │   │   ├── calls
│   │   │   │   ├── eval
│   │   │   │   ├── makewiz
│   │   │   │   ├── playerwipes
│   │   │   │   ├── setrole
│   │   │   │   └── warmboot
│   │   │   ├── debug
│   │   │   ├── errors
│   │   │   │   ├── adm
│   │   │   │   ├── chitown
│   │   │   │   ├── cmds
│   │   │   │   ├── daemon
│   │   │   │   ├── Examples
│   │   │   │   ├── LoneStar
│   │   │   │   ├── obj
│   │   │   │   ├── Praxis
│   │   │   │   ├── secure
│   │   │   │   └── std
│   │   │   ├── open
│   │   │   │   ├── adm
│   │   │   │   │   └── staff_promotions
│   │   │   │   ├── rp_wizard
│   │   │   │   │   └── tattoos
│   │   │   │   ├── faction
│   │   │   │   ├── illegal
│   │   │   │   ├── stats
│   │   │   │   └── stats.old
│   │   │   ├── personal
│   │   │   ├── reports
│   │   │   │   └── ping_q
│   │   │   ├── rp_wizard
│   │   │   │   ├── clan
│   │   │   │   └── tattoos
│   │   │   ├── secure
│   │   │   ├── watch
│   │   │   │   └── logon
│   │   │   ├── access
│   │   │   ├── access.old
│   │   │   ├── advance
│   │   │   ├── author_stats
│   │   │   ├── bank
│   │   │   ├── catch
│   │   │   ├── combat_round
│   │   │   ├── crashes
│   │   │   ├── domain_stats
│   │   │   ├── enter
│   │   │   ├── faction
│   │   │   ├── game_log
│   │   │   ├── httpd
│   │   │   ├── intermud
│   │   │   ├── mudlist_packet
│   │   │   ├── mudlist_packet.old
│   │   │   ├── new_players
│   │   │   ├── runtime
│   │   │   ├── shutdowns
│   │   │   ├── start
│   │   │   ├── stats
│   │   │   └── ws-bridge.log
│   │   ├── news
│   │   │   ├── arch
│   │   │   ├── child
│   │   │   ├── cleric
│   │   │   ├── faq
│   │   │   ├── fighter
│   │   │   ├── high_mortal
│   │   │   ├── immortal
│   │   │   ├── kataan
│   │   │   ├── locked
│   │   │   ├── mage
│   │   │   ├── monk
│   │   │   ├── news
│   │   │   ├── registration
│   │   │   ├── rogue
│   │   │   ├── welcome
│   │   │   └── wizard
│   │   ├── obj
│   │   │   ├── items
│   │   │   │   └── atlantean_text.c
│   │   │   ├── tattoo_weapons
│   │   │   │   ├── flaming_arrow.c
│   │   │   │   ├── flaming_sword.c
│   │   │   │   ├── tattoo_battle_axe.c
│   │   │   │   ├── tattoo_dagger.c
│   │   │   │   └── tattoo_spear.c
│   │   │   ├── credcard.c
│   │   │   └── credit_chip.c
│   │   ├── realms
│   │   │   ├── splynncryth
│   │   │   │   ├── area
│   │   │   │   │   ├── armor
│   │   │   │   │   ├── npc
│   │   │   │   │   ├── room
│   │   │   │   │   └── weap
│   │   │   │   └── workroom.c
│   │   │   ├── thurtea
│   │   │   │   ├── adm
│   │   │   │   │   └── access.c
│   │   │   │   ├── area
│   │   │   │   │   ├── armor
│   │   │   │   │   │   └── testarmor.c
│   │   │   │   │   ├── npc
│   │   │   │   │   │   └── bandit.c
│   │   │   │   │   ├── room
│   │   │   │   │   │   └── testroom.c
│   │   │   │   │   └── weap
│   │   │   │   │       └── testsword.c
│   │   │   │   └── workroom.c
│   │   │   ├── HOWTO.md
│   │   │   ├── LLM_BREADCRUMB.md
│   │   │   ├── README.md
│   │   │   └── WHY.md
│   │   ├── save
│   │   ├── secure
│   │   │   ├── cfg
│   │   │   │   ├── aliases.cfg
│   │   │   │   ├── groups.cfg
│   │   │   │   ├── groups.cfg.example
│   │   │   │   ├── mudos.cfg
│   │   │   │   ├── preload.cfg
│   │   │   │   ├── read.cfg
│   │   │   │   ├── socket.cfg
│   │   │   │   └── write.cfg
│   │   │   ├── cmds
│   │   │   │   ├── adm
│   │   │   │   │   ├── _addguest.c
│   │   │   │   │   ├── _allow.c
│   │   │   │   │   ├── _arrest.c
│   │   │   │   │   ├── _currency.c
│   │   │   │   │   ├── _economy.c
│   │   │   │   │   ├── _elections.c
│   │   │   │   │   ├── _grant.c
│   │   │   │   │   ├── _lawlog.c
│   │   │   │   │   ├── _link.c
│   │   │   │   │   ├── _register.c
│   │   │   │   │   ├── _removeguest.c
│   │   │   │   │   ├── _revoke.c
│   │   │   │   │   ├── _rid.c
│   │   │   │   │   ├── _setreboot.c
│   │   │   │   │   ├── _shutdown.c
│   │   │   │   │   ├── _snoopers.c
│   │   │   │   │   ├── _tempban.c
│   │   │   │   │   ├── _unallow.c
│   │   │   │   │   ├── _unbanish.c
│   │   │   │   │   ├── _unlink.c
│   │   │   │   │   ├── _unregister.c
│   │   │   │   │   ├── _unwatch.c
│   │   │   │   │   ├── _watch.c
│   │   │   │   │   ├── _whoallowed.c
│   │   │   │   │   ├── _whobanished.c
│   │   │   │   │   ├── _whoguests.c
│   │   │   │   │   ├── _whoregistered.c
│   │   │   │   │   ├── _whowatched.c
│   │   │   │   │   ├── _wizard.c
│   │   │   │   │   └── _xmote.c
│   │   │   │   ├── ambassador
│   │   │   │   │   ├── _cat.c
│   │   │   │   │   ├── _cd.c
│   │   │   │   │   ├── _cp.c
│   │   │   │   │   ├── _diff.c
│   │   │   │   │   ├── _ed.c
│   │   │   │   │   ├── _gauge.c
│   │   │   │   │   ├── _grep.c
│   │   │   │   │   ├── _head.c
│   │   │   │   │   ├── _load.c
│   │   │   │   │   ├── _ls.c
│   │   │   │   │   ├── _more.c
│   │   │   │   │   ├── _pwd.c
│   │   │   │   │   ├── _sc.c
│   │   │   │   │   ├── _ss.c
│   │   │   │   │   └── _tail.c
│   │   │   │   ├── creator
│   │   │   │   │   ├── _call.c
│   │   │   │   │   ├── _changelog.c
│   │   │   │   │   ├── _clone.c
│   │   │   │   │   ├── _dbxframe.c
│   │   │   │   │   ├── _dbxwhere.c
│   │   │   │   │   ├── _domains.c
│   │   │   │   │   ├── _dref.c
│   │   │   │   │   ├── _du.c
│   │   │   │   │   ├── _eref.c
│   │   │   │   │   ├── _eval.c
│   │   │   │   │   ├── _force.c
│   │   │   │   │   ├── _grant.c
│   │   │   │   │   ├── _imcp.c
│   │   │   │   │   ├── _locate.c
│   │   │   │   │   ├── _log.c
│   │   │   │   │   ├── _mkdir.c
│   │   │   │   │   ├── _move.c
│   │   │   │   │   ├── _mv.c
│   │   │   │   │   ├── _realms.c
│   │   │   │   │   ├── _ref.c
│   │   │   │   │   ├── _refs.c
│   │   │   │   │   ├── _rm.c
│   │   │   │   │   ├── _rmdir.c
│   │   │   │   │   ├── _snoop.c
│   │   │   │   │   ├── _sponsor.c
│   │   │   │   │   ├── _tref.c
│   │   │   │   │   ├── _update.c
│   │   │   │   │   └── _wizz.c
│   │   │   │   └── mortal
│   │   │   │       ├── _bug.c
│   │   │   │       ├── _chfn.c
│   │   │   │       ├── _finger.c
│   │   │   │       ├── _idea.c
│   │   │   │       ├── _mudidea.c
│   │   │   │       ├── _passwd.c
│   │   │   │       ├── _peer.c
│   │   │   │       ├── _praise.c
│   │   │   │       ├── _reply.c
│   │   │   │       ├── _rwho.c
│   │   │   │       ├── _suicide.c
│   │   │   │       ├── _tell.c
│   │   │   │       └── _typo.c
│   │   │   ├── daemon
│   │   │   │   ├── account_d.c
│   │   │   │   ├── advance.c
│   │   │   │   ├── bank.c
│   │   │   │   ├── bboard.c
│   │   │   │   ├── bboard.h
│   │   │   │   ├── chat.c
│   │   │   │   ├── events.c
│   │   │   │   ├── finger.c
│   │   │   │   ├── folders.c
│   │   │   │   ├── folders.h
│   │   │   │   ├── letters.c
│   │   │   │   ├── letters.h
│   │   │   │   ├── localpost.c
│   │   │   │   ├── localpost.h
│   │   │   │   ├── master.c
│   │   │   │   ├── master.h
│   │   │   │   ├── mcp_d.c
│   │   │   │   ├── mcp_d.h
│   │   │   │   ├── options.c
│   │   │   │   ├── options.h
│   │   │   │   ├── player.c
│   │   │   │   ├── politics.c
│   │   │   │   ├── remotepost.c
│   │   │   │   ├── remotepost.h
│   │   │   │   ├── shutdown.c
│   │   │   │   ├── shutdown.h
│   │   │   │   ├── users.c
│   │   │   │   ├── users.h
│   │   │   │   └── wiztools.c
│   │   │   ├── etc
│   │   │   │   ├── approval
│   │   │   │   ├── elections
│   │   │   │   ├── mudlib
│   │   │   │   └── quests
│   │   │   ├── include
│   │   │   │   ├── astronomy.h
│   │   │   │   ├── balance.h
│   │   │   │   ├── bank.h
│   │   │   │   ├── bboard.h
│   │   │   │   ├── cfg.h
│   │   │   │   ├── clock.h
│   │   │   │   ├── commands.h
│   │   │   │   ├── config.h
│   │   │   │   ├── council.h
│   │   │   │   ├── daemons.h
│   │   │   │   ├── databases.h
│   │   │   │   ├── debug2.h
│   │   │   │   ├── debug.h
│   │   │   │   ├── deputies.h
│   │   │   │   ├── dirs.h
│   │   │   │   ├── dummy.h
│   │   │   │   ├── files.h
│   │   │   │   ├── flags.h
│   │   │   │   ├── global.h
│   │   │   │   ├── iips.h
│   │   │   │   ├── living.h
│   │   │   │   ├── localtime.h
│   │   │   │   ├── materials.h
│   │   │   │   ├── money.h
│   │   │   │   ├── move.h
│   │   │   │   ├── ncommands.h
│   │   │   │   ├── network.h
│   │   │   │   ├── news.h
│   │   │   │   ├── object.h
│   │   │   │   ├── objects.h
│   │   │   │   ├── old_items.h
│   │   │   │   ├── old_room.h
│   │   │   │   ├── old_weapon.h
│   │   │   │   ├── options.h
│   │   │   │   ├── parse_com.h
│   │   │   │   ├── party.h
│   │   │   │   ├── post.h
│   │   │   │   ├── privs.h
│   │   │   │   ├── quests.h
│   │   │   │   ├── rooms.h
│   │   │   │   ├── save.h
│   │   │   │   ├── security.h
│   │   │   │   ├── simul_efun.h
│   │   │   │   ├── socket.h
│   │   │   │   ├── soul.h
│   │   │   │   ├── std.h
│   │   │   │   ├── stdprops.h
│   │   │   │   ├── tell.h
│   │   │   │   ├── user.h
│   │   │   │   ├── voting.h
│   │   │   │   └── writef.h
│   │   │   ├── save
│   │   │   │   ├── apprentice
│   │   │   │   │   ├── players
│   │   │   │   │   └── submissions
│   │   │   │   ├── binary
│   │   │   │   ├── boards
│   │   │   │   ├── daemon
│   │   │   │   │   └── politics.o
│   │   │   │   ├── daemons
│   │   │   │   ├── letters
│   │   │   │   ├── login_accounts
│   │   │   │   │   ├── LLM_BREADCRUMB.md
│   │   │   │   │   └── README.md
│   │   │   │   ├── postal
│   │   │   │   │   ├── a
│   │   │   │   │   ├── c
│   │   │   │   │   │   └── cabe
│   │   │   │   │   │       └── postalrc.o
│   │   │   │   │   ├── d
│   │   │   │   │   │   └── dragonchar
│   │   │   │   │   │       └── postalrc.o
│   │   │   │   │   ├── f
│   │   │   │   │   │   └── fear
│   │   │   │   │   │       └── postalrc.o
│   │   │   │   │   ├── l
│   │   │   │   │   │   └── leywalker
│   │   │   │   │   │       └── postalrc.o
│   │   │   │   │   ├── m
│   │   │   │   │   │   └── merctest
│   │   │   │   │   │       └── postalrc.o
│   │   │   │   │   ├── p
│   │   │   │   │   │   ├── percivil
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── psistalker
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── ptatlana
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── ptatlanb
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── ptcknight
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── ptcoder
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── ptdacon
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── ptdragona
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── ptdragonb
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── ptrpwiz
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── ptsmoake
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── pttitana
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── pttitanb
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── ptvampa
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   └── ptvampb
│   │   │   │   │   │       └── postalrc.o
│   │   │   │   │   ├── s
│   │   │   │   │   │   └── splynncryth
│   │   │   │   │   │       └── postalrc.o
│   │   │   │   │   ├── t
│   │   │   │   │   │   └── thurtea
│   │   │   │   │   │       └── postalrc.o
│   │   │   │   │   ├── v
│   │   │   │   │   │   ├── verifier
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── vzzsplynnh
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   ├── vzzsplynnk
│   │   │   │   │   │   │   └── postalrc.o
│   │   │   │   │   │   └── vzzupdater
│   │   │   │   │   │       └── postalrc.o
│   │   │   │   │   └── z
│   │   │   │   │       ├── ztestfive
│   │   │   │   │       │   └── postalrc.o
│   │   │   │   │       ├── ztestfour
│   │   │   │   │       │   └── postalrc.o
│   │   │   │   │       ├── ztestrr
│   │   │   │   │       │   └── postalrc.o
│   │   │   │   │       ├── ztestthree
│   │   │   │   │       │   └── postalrc.o
│   │   │   │   │       ├── ztesttwo
│   │   │   │   │       │   └── postalrc.o
│   │   │   │   │       └── zztest
│   │   │   │   │           └── postalrc.o
│   │   │   │   ├── skill_grants
│   │   │   │   ├── skill_requests
│   │   │   │   ├── tmp
│   │   │   │   ├── users
│   │   │   │   │   ├── a
│   │   │   │   │   ├── d
│   │   │   │   │   ├── f
│   │   │   │   │   ├── h
│   │   │   │   │   ├── m
│   │   │   │   │   ├── p
│   │   │   │   │   ├── q
│   │   │   │   │   ├── s
│   │   │   │   │   ├── t
│   │   │   │   │   │   └── thurtea.o
│   │   │   │   │   ├── v
│   │   │   │   │   │   ├── vzzsplynnk.o
│   │   │   │   │   │   └── vzzupdater.o
│   │   │   │   │   └── z
│   │   │   │   └── votes
│   │   │   ├── SimulEfun
│   │   │   │   ├── absolute_path.c
│   │   │   │   ├── absolute_value.c
│   │   │   │   ├── alignment.c
│   │   │   │   ├── base_name.c
│   │   │   │   ├── communications.c
│   │   │   │   ├── convert_name.c
│   │   │   │   ├── copy.c
│   │   │   │   ├── creator_file.c
│   │   │   │   ├── distinct_array.c
│   │   │   │   ├── domains.c
│   │   │   │   ├── economy.c
│   │   │   │   ├── english.c
│   │   │   │   ├── events.c
│   │   │   │   ├── exclude_array.c
│   │   │   │   ├── files.c
│   │   │   │   ├── format_page.c
│   │   │   │   ├── format_string.c
│   │   │   │   ├── get_object.c
│   │   │   │   ├── identify.c
│   │   │   │   ├── idle.c
│   │   │   │   ├── interact.c
│   │   │   │   ├── interface.c
│   │   │   │   ├── light.c
│   │   │   │   ├── load_object.c
│   │   │   │   ├── log_file.c
│   │   │   │   ├── magic_class.c
│   │   │   │   ├── misc.c
│   │   │   │   ├── mud_info.c
│   │   │   │   ├── ordinal.c
│   │   │   │   ├── parse_objects.c
│   │   │   │   ├── path_file.c
│   │   │   │   ├── percent.c
│   │   │   │   ├── personal_log.c
│   │   │   │   ├── pointers.c
│   │   │   │   ├── read_database.c
│   │   │   │   ├── security.c
│   │   │   │   ├── SimulEfun.c
│   │   │   │   ├── SimulEfun.h
│   │   │   │   ├── strings.c
│   │   │   │   ├── substr.c
│   │   │   │   ├── time.c
│   │   │   │   ├── to_object.c
│   │   │   │   ├── translate.c
│   │   │   │   ├── user_exists.c
│   │   │   │   ├── user_path.c
│   │   │   │   ├── visible.c
│   │   │   │   ├── wiz_role.c
│   │   │   │   └── wrap.c
│   │   │   ├── std
│   │   │   │   ├── client.c
│   │   │   │   ├── daemon.c
│   │   │   │   ├── login.c
│   │   │   │   ├── login.cnf
│   │   │   │   ├── login.nm33
│   │   │   │   ├── post.c
│   │   │   │   └── post.h
│   │   │   ├── tmp
│   │   │   ├── HOWTO.md
│   │   │   ├── LLM_BREADCRUMB.md
│   │   │   ├── README.md
│   │   │   └── WHY.md
│   │   ├── std
│   │   │   ├── hm
│   │   │   │   ├── hm_room.c
│   │   │   │   └── room_maker.c
│   │   │   ├── living
│   │   │   │   ├── body.c
│   │   │   │   ├── combat.c
│   │   │   │   ├── env.c
│   │   │   │   ├── follow.c
│   │   │   │   ├── messages.c
│   │   │   │   └── skills.c
│   │   │   ├── obj
│   │   │   │   ├── templates
│   │   │   │   │   ├── qcs_armor.c
│   │   │   │   │   ├── qcs_npc.c
│   │   │   │   │   ├── qcs_room.c
│   │   │   │   │   └── qcs_weapon.c
│   │   │   │   ├── aura_orb.c
│   │   │   │   ├── body_part.c
│   │   │   │   ├── brand.c
│   │   │   │   ├── coins.c
│   │   │   │   ├── corpse.c
│   │   │   │   ├── dark_orb.c
│   │   │   │   ├── deed.c
│   │   │   │   ├── dim_pocket_bag.c
│   │   │   │   ├── ears_orb.c
│   │   │   │   ├── fresh_blood.c
│   │   │   │   ├── light_orb.c
│   │   │   │   ├── magic_net.c
│   │   │   │   ├── player_list.c
│   │   │   │   ├── rifts_familiar.c
│   │   │   │   ├── RoomCreator.c
│   │   │   │   ├── torch.c
│   │   │   │   ├── undead.c
│   │   │   │   ├── wed_ring.c
│   │   │   │   └── workroom.c
│   │   │   ├── room
│   │   │   │   ├── exits.c
│   │   │   │   ├── exits.h
│   │   │   │   ├── items.c
│   │   │   │   └── senses.c
│   │   │   ├── user
│   │   │   │   ├── autosave.c
│   │   │   │   ├── autosave.h
│   │   │   │   ├── editor.c
│   │   │   │   ├── editor.h
│   │   │   │   ├── files.c
│   │   │   │   ├── files.h
│   │   │   │   ├── more.c
│   │   │   │   ├── more.h
│   │   │   │   ├── nmsh.c
│   │   │   │   ├── nmsh.h
│   │   │   │   └── refs.c
│   │   │   ├── virtual
│   │   │   │   └── compile.c
│   │   │   ├── access.c
│   │   │   ├── armour.c
│   │   │   ├── barkeep.c
│   │   │   ├── bboard.c
│   │   │   ├── bboard.h
│   │   │   ├── blue_rift_portal.c
│   │   │   ├── clean_up.c
│   │   │   ├── clean_up.h
│   │   │   ├── container.c
│   │   │   ├── container.h
│   │   │   ├── daemon.c
│   │   │   ├── deputy.c
│   │   │   ├── door.c
│   │   │   ├── drink.c
│   │   │   ├── drink.h
│   │   │   ├── estate.c
│   │   │   ├── food.c
│   │   │   ├── food.h
│   │   │   ├── germ.c
│   │   │   ├── germ.h
│   │   │   ├── guild.c
│   │   │   ├── HOWTO.md
│   │   │   ├── light.c
│   │   │   ├── light.h
│   │   │   ├── living.c
│   │   │   ├── LLM_BREADCRUMB.md
│   │   │   ├── money.c
│   │   │   ├── monster.c
│   │   │   ├── Object.c
│   │   │   ├── Object.h
│   │   │   ├── pier.c
│   │   │   ├── poison.c
│   │   │   ├── prop_logic.c
│   │   │   ├── quest_ob.c
│   │   │   ├── README.md
│   │   │   ├── realtor.c
│   │   │   ├── rift_portal.c
│   │   │   ├── rifts_ambient.c
│   │   │   ├── rifts_corpse.c
│   │   │   ├── rifts_npc.c
│   │   │   ├── rifts_pet.c
│   │   │   ├── rifts_radio.c
│   │   │   ├── rifts_vehicle.c
│   │   │   ├── rifts_vendor.c
│   │   │   ├── room.c
│   │   │   ├── room.h
│   │   │   ├── sign.c
│   │   │   ├── storage.c
│   │   │   ├── storage.h
│   │   │   ├── test.c
│   │   │   ├── user.c
│   │   │   ├── vault.c
│   │   │   ├── vendor.c
│   │   │   ├── virtual.c
│   │   │   ├── vote_login.h
│   │   │   ├── vote_room.c
│   │   │   ├── weapon.c
│   │   │   └── WHY.md
│   │   ├── tmp
│   │   │   └── bb
│   │   ├── www
│   │   │   ├── errors
│   │   │   │   ├── access.html
│   │   │   │   ├── badcmd.html
│   │   │   │   ├── badgate.html
│   │   │   │   └── notfound.html
│   │   │   ├── gateways
│   │   │   │   ├── efuns.c
│   │   │   │   ├── finger.c
│   │   │   │   ├── HomePages.c
│   │   │   │   ├── mudlist.c
│   │   │   │   ├── SimulEfuns.c
│   │   │   │   └── who.c
│   │   │   └── index.html
│   │   ├── LLM_BREADCRUMB.md
│   │   └── README.md
│   ├── win32
│   │   ├── driver.exe
│   │   ├── LLM_BREADCRUMB.md
│   │   ├── mud.bat
│   │   └── README.md
│   ├── INSTALL
│   ├── LLM_BREADCRUMB.md
│   ├── README.md
│   └── RiftsMUD Memories.txt
├── scripts
│   ├── check-setup.sh
│   ├── init-local-config.sh
│   ├── LLM_BREADCRUMB.md
│   └── README.md
├── staff-handbook
│   ├── ch01-server.md
│   ├── ch02-gotchas.md
│   ├── ch03-staff-ranks.md
│   ├── ch04-wizard-tools.md
│   ├── ch05-chargen-flow.md
│   ├── ch06-races.md
│   ├── ch07-occs.md
│   ├── ch08-mdc-sdc.md
│   ├── ch09-combat.md
│   ├── ch10-skills.md
│   ├── ch11-spells.md
│   ├── ch12-psionics.md
│   ├── ch13-ppe-isp.md
│   ├── ch14-alignments.md
│   ├── ch15-insanities.md
│   ├── ch16-builder.md
│   ├── HOWTO.md
│   ├── index.md
│   ├── LLM_BREADCRUMB.md
│   ├── README.md
│   └── WHY.md
├── tools
│   ├── __pycache__
│   ├── LLM_BREADCRUMB.md
│   ├── migrate-static.pl
│   ├── playtest_create_chars.py
│   ├── README.md
│   ├── section8_verify3.py
│   ├── section8_verify4.py
│   ├── section8_verify.py
│   └── spell_shortcut_verify.py
├── www
│   ├── assets
│   │   ├── aethermud-favicon.ico
│   │   ├── AetherMUD.jpeg
│   │   ├── android-chrome-192x192.png
│   │   ├── android-chrome-512x512.png
│   │   ├── apple-touch-icon.png
│   │   ├── favicon-16x16.png
│   │   ├── favicon-32x32.png
│   │   ├── favicon.ico
│   │   ├── invocations.md
│   │   ├── LLM_BREADCRUMB.md
│   │   └── README.md
│   ├── css
│   │   ├── LLM_BREADCRUMB.md
│   │   ├── README.md
│   │   └── site.css
│   ├── js
│   │   ├── LLM_BREADCRUMB.md
│   │   └── README.md
│   ├── admin-commands.html
│   ├── admin.html
│   ├── apprentice.html
│   ├── chargen.html
│   ├── classes.html
│   ├── coding.html
│   ├── commands.html
│   ├── domain.html
│   ├── HOWTO.md
│   ├── index.html
│   ├── LLM_BREADCRUMB.md
│   ├── play.html
│   ├── qcs.html
│   ├── races.html
│   ├── README.md
│   ├── roadmap.html
│   ├── roleplay.html
│   ├── staff.html
│   ├── staff-toolkit.html
│   ├── WHY.md
│   └── world.html
├── zip-files
│   ├── deadsouls.zip
│   ├── ldmud.zip
│   ├── lil.zip
│   ├── lima.zip
│   ├── lpuni.zip
│   └── nightmare3.zip
├── aethermud.code-workspace
├── aethermud-directory.md
├── backups-prompt.md
├── CLAUDE.md
├── commit.sh
├── dead_souls_alpha.zip
├── HOWTO.md
├── LLM_BREADCRUMB.md
├── mud.sh
├── README.md
└── WHY.md

346 directories, 3658 files

Generated on Tue 28 Jul 2026 07:18:01 PM ADT
