#include "hal.h"

#define NR_KEYS 18

enum {KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS};

/* Only the following keys are used in NEMU-PAL. */
static const int keycode_array[] = {
	K_UP, K_DOWN, K_LEFT, K_RIGHT, K_ESCAPE,
	K_RETURN, K_SPACE, K_PAGEUP, K_PAGEDOWN, K_r,
	K_a, K_d, K_e, K_w, K_q,
	K_s, K_f, K_p
};

static int key_state[NR_KEYS];


static inline int getkeyindex(int code)
{
	code=code>=0x80?code-0x80:code;
	int i=0;
	for(;i<NR_KEYS;i++)
		if(code==keycode_array[i])
			return i;
	return -1;
}

void
keyboard_event(void) {
	/* TODO: Fetch the scancode and update the key states. */
	// assert(0);
	// 
	int code = in_byte(0x60);
						//0x60
	int index = getkeyindex(code);
	if(index==-1)
		return ;
	/*
	你应该从数字逻辑电路实验中认识到和扫描码相关的内容了: 
	当按下一个键的时候, 键盘控制器将会发送该键的通码(make code); 当
	释放一个键的时候, 键盘控制器将会发送该键的断码(break code),
	 其中断码的值为通码的值+0x80. 需要注意的是, 断码仅在键被释放的时候才发送,
	  因此你应该用"收到断码"来作为键被释放的检测条件, 
	  而不是用"没收到通码"作为检测条件.
	 */
	if (code < 0x80) 
	{ 
	// press
	    switch(key_state[index]){
			case KEY_STATE_EMPTY:key_state[index] = KEY_STATE_PRESS;break;
			case KEY_STATE_PRESS:key_state[index] = KEY_STATE_WAIT_RELEASE;break;
			case KEY_STATE_WAIT_RELEASE:key_state[index] = KEY_STATE_WAIT_RELEASE;break;
			case KEY_STATE_RELEASE:key_state[index] = KEY_STATE_PRESS;break;
			default:assert(0);
		}
	}
	else 
	{ 

	// release
		switch(key_state[index]){
			case KEY_STATE_EMPTY:key_state[index] = KEY_STATE_RELEASE;break;
			case KEY_STATE_PRESS:key_state[index] = KEY_STATE_RELEASE;break;
			case KEY_STATE_WAIT_RELEASE:key_state[index] = KEY_STATE_RELEASE;break;
			// case KEY_STATE_RELEASE:key_state[index] = KEY_STATE_PRESS;break;
			default:assert(0);
		}
	}
}

static inline int
get_keycode(int index) {
	assert(index >= 0 && index < NR_KEYS);
	return keycode_array[index];
}

static inline int
query_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	return key_state[index];
}

static inline void
release_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_WAIT_RELEASE;
}

static inline void
clear_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_EMPTY;
}

bool 
process_keys(void (*key_press_callback)(int), void (*key_release_callback)(int)) {
	cli();
	/* TODO: Traverse the key states. Find a key just pressed or released.
	 * If a pressed key is found, call `key_press_callback' with the keycode.
	 * If a released key is found, call `key_release_callback' with the keycode.
	 * If any such key is found, the function return true.
	 * If no such key is found, the function return false.
	 * Remember to enable interrupts before returning from the function.
	 */

	// assert(0);
	int i=0;
	for(;i<NR_KEYS;i++)
	{
		if(key_state[i]==KEY_STATE_PRESS)
		{
			key_state[i]=KEY_STATE_WAIT_RELEASE;
			key_press_callback(get_keycode(i));
			sti();
			return true;
		}
		else if(key_state[i]==KEY_STATE_RELEASE)
		{
			key_state[i]=KEY_STATE_EMPTY;
			key_release_callback(get_keycode(i));
			//becasue we think it has been proceed;
			sti();
			return true;
		}
	}
	sti();
	return false;
}
