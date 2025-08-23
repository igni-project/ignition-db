# ignition-db

Ignition is a program that displays incoming SUP messages in human readable format.

## Usage

Only one argument is non-optional: the socket domain. Append `-d` or `--domain` to the command arguments to specify the socket domain. For example: `ignition -d/run/user/$UID/ignition`.

Ignition shall create a new UNIX domain socket at the location given by `--domain`.

