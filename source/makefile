# Main makefile
OBJS = loader.o memory_management_s.o process.o process_s.o kernel_main.o gdt_entry.o idt_entry.o interrupt.o load_idt.o isr.o paging_management.o page.o memory_management.o kernel_heap.o user_space_main.o pic_timer.o screen.o array.o linked_list.o common.o fs.o initrd_fs.o tss_entry.o load_tss.o system_calls.o user_space_system_calls.o icxxabi.o
CFLAGS = -fno-stack-protector -g
GAS_CFLAGS = -march=i686 --32 -g

OPTS = -fno-exceptions -Iinclude -I. -fno-rtti -march=i686 -m32

all: linker.ld $(OBJS)
		g++ -T linker.ld -o kernel.bin $(OBJS) $(OPTS) -nostdlib -static -lstdc++ -Wl,--build-id=none

loader.o: loader.S
		as -o loader.o loader.S $(GAS_CFLAGS)

process_s.o: process.S
		as -o process_s.o process.S $(GAS_CFLAGS)

memory_management_s.o: memory_management.S 
		as -o memory_management_s.o memory_management.S $(GAS_CFLAGS)

kernel_main.o: kernel_main.cpp
		g++ $(OPTS) -c -o kernel_main.o  kernel_main.cpp $(CFLAGS)
		
gdt_entry.o: gdt_entry.cpp gdt_entry.h
		g++ $(OPTS) -c -o gdt_entry.o  gdt_entry.cpp $(CFLAGS)
		
screen.o: screen.cpp screen.h
		g++ $(OPTS) -c -o screen.o screen.cpp $(CFLAGS)
		
idt_entry.o: idt_entry.cpp idt_entry.h	
		g++ $(OPTS) -c -o idt_entry.o  idt_entry.cpp $(CFLAGS)

isr.o: isr.cpp isr.h		
		g++ $(OPTS) -c -o isr.o  isr.cpp $(CFLAGS)

load_idt.o: load_idt.S
		as -o load_idt.o load_idt.S $(GAS_CFLAGS)

interrupt.o: interrupt.S
		as -o interrupt.o interrupt.S $(GAS_CFLAGS)

user_space_main.o:	user_space_main.cpp
		g++ $(OPTS) -c -o user_space_main.o user_space_main.cpp $(CFLAGS)
		
pic_timer.o: pic_timer.cpp
		g++ $(OPTS) -c -o pic_timer.o pic_timer.cpp $(CFLAGS)
		
paging_management.o: paging_management.cpp
		g++ $(OPTS) -c -o paging_management.o paging_management.cpp $(CFLAGS)

page.o: page.cpp
		g++ $(OPTS) -c -o page.o page.cpp $(CFLAGS)
		
memory_management.o: memory_management.cpp
		g++ $(OPTS) -c -o memory_management.o memory_management.cpp $(CFLAGS)
				
kernel_heap.o: kernel_heap.cpp
		g++ $(OPTS) -c -o kernel_heap.o kernel_heap.cpp $(CFLAGS)

process.o: process.cpp process.h
		g++ $(OPTS) -c -o process.o process.cpp $(CFLAGS)
		
# include
common.o: utils/common.cpp include/common.h
		g++ $(OPTS) -c -o common.o utils/common.cpp $(CFLAGS)
		
array.o: utils/array.cpp include/array.h
		g++ $(OPTS) -c -o array.o utils/array.cpp $(CFLAGS)
		
linked_list.o: utils/linked_list.cpp include/linked_list.h
		g++ $(OPTS) -c -o linked_list.o utils/linked_list.cpp $(CFLAGS)

# vfs
fs.o:	vfs/fs.cpp vfs/fs.h
		g++ $(OPTS) -c -o fs.o vfs/fs.cpp $(CFLAGS)

initrd_fs.o: vfs/initrd_fs.cpp vfs/initrd_fs.h
		g++ $(OPTS) -c -o initrd_fs.o vfs/initrd_fs.cpp $(CFLAGS)
				
#
system_calls.o: system_calls.cpp system_calls.h
		g++ $(OPTS) -c -o system_calls.o system_calls.cpp $(CFLAGS)

user_space_system_calls.o: user_space_system_calls.cpp user_space_system_calls.h
		g++ $(OPTS) -c -o user_space_system_calls.o user_space_system_calls.cpp $(CFLAGS)
				
tss_entry.o: tss_entry.cpp tss_entry.h
		g++ $(OPTS) -c -o tss_entry.o tss_entry.cpp
		
load_tss.o: load_tss.S
		as -o load_tss.o load_tss.S $(GAS_CFLAGS)
		
icxxabi.o: icxxabi.cpp
		g++ $(OPTS) -c -o icxxabi.o icxxabi.cpp $(CFLAGS)
		
clean:
		rm -f *.o *.bin
