enum KeyboardState{
	RELEASED,
	BUTTON_0,
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
};

void KeyboardInit(void);
enum KeyboardState eKeyboardRead(void);
