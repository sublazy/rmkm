ccflags-y := -std=gnu99 -Wno-declaration-after-statement

obj-m += rmkm.o
rmkm-objs := ./src/rmkm.o ./src/int_parser.o ./src/median_calc.o

build:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

reload:
	 @echo "reloading rmkm module"
	 @sudo rmmod rmkm.ko
	 @sudo insmod rmkm.ko
	 @sudo chmod o+rw /dev/median

test:
	@cd tests; bats basic.bats

help:
	@echo "Available make targets:"
	@echo "   * build"
	@echo "   * clean"
	@echo "   * test"

.PHONY: build clean test
