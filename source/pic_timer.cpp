#include "pic_timer.h"
#include "screen.h"
#include "isr.h"
#include "registers.h"
#include "process.h"

extern Screen screen;
extern PicTimer picTimer;
extern Sheduler sheduler;

PicTimer:: PicTimer(int frequency)
{
	tick_ = 0;
	 
	//int divisor = 1193180 / frequency;
	register_irq_handler(0, pic_timer_callback);

    __asm__(" \
		movb $0x36, %al; \
		outb $0x43;		\
		");
		
// 37, 0x59 - 		
	__asm__(" \
		movb $37, %al; \
		outb $0x40;		\
		");
	
	__asm__(" \
		movb $0x5D, %al; \
		outb $0x40;		\
		");
		
}

PicTimer:: PicTimer()
{
	screen.printString("PicTimer constructor, default\n");	
	register_irq_handler(0, pic_timer_callback);
}

void PicTimer:: init()
{
	screen.printString("PicTimer init\n"); 
	tick_ = 0;
	 
	//int divisor = 1193180 / frequency;
	register_irq_handler(0, pic_timer_callback);

    __asm__(" \
		movb $0x36, %al; \
		outb $0x43;		\
		");
		
// 37, 0x59 - 		
	__asm__(" \
		movb $37, %al; \
		outb $0x40;		\
		");
	
	__asm__(" \
		movb $0x5D, %al; \
		outb $0x40;		\
		");
}
extern "C" void pic_timer_callback(registers regs)
{
	/*
	if(picTimer.tick_ % 2 == 0)
		screen.setTextColor(0, COLOR_LIGHT_RED);
	else
		screen.setTextColor(0, COLOR_LIGHT_RED);
	screen.printString("timer ");
	screen.printInt(picTimer.tick_);
	screen.printString("\n");*/
	picTimer.tick_++;
	//if(picTimer.tick_ % 5 == 0)
	sheduler.switchTask();
}
