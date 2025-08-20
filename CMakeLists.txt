cmake_minimum_required(VERSION 3.15...4.0)

project(
	ignition
	VERSION 0.2
	LANGUAGES C
)

add_executable(
	ignition
	src/main.c
	src/ipc.c
	src/cmd/cmdline.c
	src/render/gfxenv.c
	src/debug/print.c
	src/format/sup.c
)

find_library(LIBSUP sup)

target_link_libraries(ignition ${LIBSUP})

install(TARGETS ignition)
include(CPack)

