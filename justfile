default:
  @just --list

upload:
	pio run -t upload

test:
    pio test

compile:
	pio run

compileCommands:
	pio run -t compiledb

serial-monitor:
    pio device monitor
