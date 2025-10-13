all: sys

sys:
	make -f Makefile.$@
local:
	make -f Makefile.$@
clean: clean.local clean.sys

run.sys:
	make -f Makefile.sys run
run.local:
	make -f Makefile.local run
clean.local:
	make -f Makefile.sys clean
clean.sys:
	make -f Makefile.local clean

