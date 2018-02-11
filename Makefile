ccflags-y := -std=gnu99 -Wno-declaration-after-statement

obj-m += rmkm.o
rmkm-objs := ./src/rmkm.o ./src/int_parser.o ./src/median_calc.o

build:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

test:
	@echo "Note: Run 'dmesg -w' in a separate terminal to see results."
	sudo insmod rmkm.ko
	sudo chmod o+rw /dev/median
	cat /dev/median
	echo "3 1 2" > /dev/median
	cat /dev/median
	echo "11 15" > /dev/median
	cat /dev/median
	echo "15 15" > /dev/median
	cat /dev/median
	echo "0 0 0 0" > /dev/median
	cat /dev/median
	sudo rmmod rmkm.ko

help:
	@echo "Available make targets:"
	@echo "   * build"
	@echo "   * clean"
	@echo "   * test"

.PHONY: build clean test
