default:
  @just --list

upload:
	pio run -t upload

compile:
	pio run

compileCommands:
	pio run -t compiledb

serial-monitor:
    pio device monitor
