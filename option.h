#ifndef OPTION_H
#define OPTION_H

// Get number of elements with cool macro hack . . .
enum class Option
{
	BEGIN = __LINE__,
	OPEN_FILE = 0,
	SAVE_FILE,
	UNDO,
	REDO,
	HELP,
	CREATE_RECTANGLE,
	CREATE_CIRCLE,
	CREATE_TRIANGLE,
	MOVE,
	SUB_Z,
	ADD_Z,
	RESIZE_ADD,
	RESIZE_SUB,
	DELETE,
	COLOR_OUTLINE,
	COLOR_FILL,
	SET_COLOR_RED,
	SET_COLOR_GREEN,
	SET_COLOR_BLUE,
	SET_COLOR_YELLOW,
	SET_COLOR_MAGENTA,
	SET_COLOR_BLACK,
	SET_COLOR_WHITE,
	SIZE = __LINE__ - BEGIN -1
};

#endif // OPTION_H