# ignition-db

Ignition-db is a program that displays incoming SUP messages in human readable format.

## Usage

Only one argument is non-optional: the socket domain. Append `-d` or `--domain` to the command arguments to specify the socket domain. For example: `ignition-db -d/run/user/$UID/ignition-db`.

Ignition-db shall create a new UNIX domain socket at the location given by `--domain`.

