ccflags-y := -std=gnu99 -Wno-declaration-after-statement

obj-m += rmkm.o
rmkm-objs := ./src/rmkm.o ./src/int_parser.o

build:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

test:
	@echo "Note: Run 'dmesg -w' in a separate terminal to see results."
	sudo insmod rmkm.ko
	sudo chmod o+rw /dev/median
	cat /dev/median
	echo "1 2 3" > /dev/median
	cat /dev/median
	echo -e " 10 \t12 \n\v\t  13   " > /dev/median
	echo -e "    01234567 10023     " > /dev/median
	echo -e " 2000000001 " > /dev/median
	cat /dev/median
	cat /dev/median
	sudo rmmod rmkm.ko

help:
	@echo "Available make targets:"
	@echo "   * build"
	@echo "   * clean"
	@echo "   * test"

.PHONY: build clean test
