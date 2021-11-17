#include "registers.h"

class PicTimer
{
public:
	int tick_;
	PicTimer(int );
	PicTimer();
	void init();
};

extern "C" void pic_timer_callback(registers );
