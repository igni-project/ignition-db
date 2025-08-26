cmake_minimum_required(VERSION 3.15...4.0)

project(
	ignition-db
	VERSION 0.2.2
	LANGUAGES C
)

add_executable(
	ignition-db
	src/main.c
	src/cmd/cmdline.c
	src/world/env.c
	src/debug/print.c
	src/format/sup.c
)

find_library(LIBSUP sup)

target_link_libraries(ignition-db ${LIBSUP})

install(TARGETS ignition-db)
include(CPack)

