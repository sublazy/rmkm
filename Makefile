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
	@echo "Note: Run 'dmesg -w' in a separate terminal to see results."
	sudo insmod rmkm.ko
	sudo chmod o+rw /dev/median
	cat /dev/median
	cat /dev/median
	echo "1 3 6 7" > /dev/median
	cat /dev/median
	@make --no-print-directory reload
	echo "1 2 3" > /dev/median
	cat /dev/median
	@make --no-print-directory reload
	echo "1 3 3 5" > /dev/median
	cat /dev/median
	@make --no-print-directory reload
	echo "1 1 7 7 7 8 9" > /dev/median
	cat /dev/median
	@make --no-print-directory reload
	echo "1" > /dev/median
	cat /dev/median
	echo "2" > /dev/median
	cat /dev/median
	echo "3" > /dev/median
	cat /dev/median
	echo "4" > /dev/median
	cat /dev/median
	sudo rmmod rmkm.ko

help:
	@echo "Available make targets:"
	@echo "   * build"
	@echo "   * clean"
	@echo "   * test"

.PHONY: build clean test
