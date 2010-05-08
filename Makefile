# This Makefile is just a wrapper around our fancy new build script, but
# feel free to continue using make to build the project, it will work
# just fine.

# New source files no longer go here! Add entries to the client_srcs and
# server_srcs tables as necessary in "sources.lua". The correct Makefile
# syntax will be generated automatically!

all: debug

debug:
	./build debug

release:
	./build release

debug-client:
	./build debug client

debug-server:
	./build debug server

release-client:
	./build release client

release-server:
	./build release server

clean:
	./build clean

help:
	@echo "Run \"make debug\" or \"make release\" to build debug or release versions of the"
	@echo "client and server. You can also build a specific binary by running something"
	@echo "like \"make debug-client\" or \"make release-server\". If you have any questions,"
	@echo "ask Bob at rsomers@calpoly.edu!"
