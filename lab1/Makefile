KVERS = $(shell uname -r)

MY_CFLAGS += -march=native
# MY_CFLAGS += -g

# Kernel modules
obj-m += MeltdownKernel.o

build: kernel_modules

kernel_modules:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules

clean:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean
	rm -rf CacheTime ExceptionHandling MeltdownExperiment MeltdownExperimentAsm MeltdownExperimentNext UsertoKernel FlushReload

CacheTime:
	gcc $(MY_CFLAGS) CacheTime.c -o CacheTime
	./CacheTime

FlushReload:
	gcc $(MY_CFLAGS) FlushReload.c -o FlushReload
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

replace5:
	sed 's/0xfb61b000/0x$(shell sudo dmesg | grep 'secret data address' | sed 's/:/ /g' | awk '{print $$NF}')/' MeltdownAttack.c > MeltdownAttack2.c

UsertoKernel: replace
	gcc $(MY_CFLAGS) UsertoKernel2.c -o UsertoKernel
	./UsertoKernel

ExceptionHandling:
	gcc $(MY_CFLAGS) ExceptionHandling.c -o ExceptionHandling
	./ExceptionHandling

MeltdownExperiment: replace2
	gcc $(MY_CFLAGS) MeltdownExperiment2.c -o MeltdownExperiment
	./MeltdownExperiment

MeltdownExperimentNext: replace3
	gcc $(MY_CFLAGS) MeltdownExperimentNext2.c -o MeltdownExperimentNext
	./MeltdownExperimentNext

MeltdownExperimentAsm: replace4
	gcc $(MY_CFLAGS) MeltdownExperimentAsm2.c -o MeltdownExperimentAsm
	./MeltdownExperimentAsm

MeltdownAttack: replace5
	gcc $(MY_CFLAGS) MeltdownAttack2.c -o MeltdownAttack
	# ./MeltdownAttack

attack: MeltdownAttack
	./MeltdownAttack

remove:
	sudo rmmod MeltdownKernel

.PHONY: CacheTime FlushReload UsertoKernel ExceptionHandling MeltdownExperiment MeltdownExperimentNext MeltdownExperimentAsm MeltdownAttack
