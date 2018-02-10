obj-m += rmkm.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement

build:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

test:
	@echo TODO

help:
	@echo "Available make targets:"
	@echo "   * build"
	@echo "   * clean"
	@echo "   * test"

.PHONY: build clean test
