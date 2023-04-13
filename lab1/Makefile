KVERS = $(shell uname -r)

CFLAGS := -g -march=native

# Kernel modules
obj-m += MeltdownKernel.o

build: kernel_modules

kernel_modules:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules

clean:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean
	rm -rf CacheTime ExceptionHandling MeltdownExperiment MeltdownExperimentAsm MeltdownExperimentNext UsertoKernel FlushReload

CacheTime:
	gcc $(CFLAGS) CacheTime.c -o CacheTime
	./CacheTime

FlushReload:
	gcc $(CFLAGS) FlushReload.c -o FlushReload
	./FlushReload

run: build
	sudo insmod MeltdownKernel.ko

address:
	sudo dmesg | grep 'secret data address'

replace:
	sed 's/0x1234/0x$(shell sudo dmesg | grep 'secret data address' | sed 's/:/ /g' | awk '{print $$NF}')/' UsertoKernel.c > UsertoKernel2.c

replace2:
	sed 's/0xfb61b000/0x$(shell sudo dmesg | grep 'secret data address' | sed 's/:/ /g' | awk '{print $$NF}')/' MeltdownExperiment.c > MeltdownExperiment2.c

replace3:
	sed 's/0xfb61b000/0x$(shell sudo dmesg | grep 'secret data address' | sed 's/:/ /g' | awk '{print $$NF}')/' MeltdownExperimentNext.c > MeltdownExperimentNext2.c

replace4:
	sed 's/0xfb61b000/0x$(shell sudo dmesg | grep 'secret data address' | sed 's/:/ /g' | awk '{print $$NF}')/' MeltdownExperimentAsm.c > MeltdownExperimentAsm2.c

UsertoKernel: replace
	gcc $(CFLAGS) UsertoKernel2.c -o UsertoKernel
	./UsertoKernel

ExceptionHandling:
	gcc $(CFLAGS) ExceptionHandling.c -o ExceptionHandling
	./ExceptionHandling

MeltdownExperiment: replace2
	gcc $(CFLAGS) MeltdownExperiment2.c -o MeltdownExperiment
	./MeltdownExperiment

MeltdownExperimentNext: replace3
	gcc $(CFLAGS) MeltdownExperimentNext2.c -o MeltdownExperimentNext
	./MeltdownExperimentNext

MeltdownExperimentAsm: replace4
	gcc $(CFLAGS) MeltdownExperimentAsm2.c -o MeltdownExperimentAsm
	./MeltdownExperimentAsm

MeltdownAttack:
	gcc $(CFLAGS) MeltdownAttack.c -o MeltdownAttack
	./MeltdownAttack

remove:
	sudo rmmod MeltdownKernel

.PHONY: CacheTime FlushReload UsertoKernel ExceptionHandling MeltdownExperiment MeltdownExperimentNext MeltdownExperimentAsm MeltdownAttack