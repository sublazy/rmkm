ALGO ?= heap

ccflags-y := -std=gnu99 -Wno-declaration-after-statement

obj-m += rmkm.o
rmkm-objs := src/rmkm.o src/int_parser.o src/heap.o src/utils.o

ifeq ($(ALGO),heap)
rmkm-objs += src/median_calc_2.o
else
rmkm-objs += src/median_calc.o
endif

build:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

insmod:
	 @sudo insmod rmkm.ko
	 @sudo chmod o+rw /dev/median

rmmod:
	 @sudo rmmod rmkm.ko

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
	@echo "   * insmod"
	@echo "   * rmmod"
	@echo "   * reload"

.PHONY: build clean test
