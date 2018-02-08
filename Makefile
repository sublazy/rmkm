help:
	@echo "Available make targets:"
	@echo "   * build"
	@echo "   * clean"
	@echo "   * test"

obj-m += rmkm.o

build:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

test:
	@echo TODO

.PHONY: build clean test