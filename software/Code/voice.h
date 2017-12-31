#ifndef __VOICE_H__
#define __VOICE_H__

#include "stm32f10x.h"

struct __voice_file {
	u32 file_len;
	u32 file_pos;
};

void play_voice(struct __voice_file v);

#endif
