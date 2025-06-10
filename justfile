default:
    @just --list

upload:
	pio run -t upload

test:
    pio test

build:
    pio run

compileCommands:
    rm compile_commands.json ; pio run -t compiledb -v

serial-monitor:
    pio device monitor

u: upload

m: serial-monitor

b: build

um: u m
