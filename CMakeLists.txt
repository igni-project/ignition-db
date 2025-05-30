cmake_minimum_required(VERSION 3.15...4.0)

project(
	ignition
	VERSION 0.1
	LANGUAGES C
)

add_executable(
	ignition
	src/main.c
	src/socket.c
	src/cmd/cmdline.c
)

find_library(LIBSUP sup)



