#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <bitset>

#include "constants.h"

/* Application Configurations */

#define APP_TITLE "Cellular Automata"

// This should be equal to the number of individual image defines
//
#define NUM_TOOL_IMAGES 8

#define IMGPTH_TLNEXT    "bitmaps/next.bmp"
#define IMGPTH_TLPLAY    "bitmaps/play.bmp"
#define IMGPTH_TLSTOP    "bitmaps/stop.bmp"
#define IMGPTH_TLRESTART "bitmaps/restart.bmp"
#define IMGPTH_TLMVUP    "bitmaps/move_up.bmp"
#define IMGPTH_TLMVRIGHT "bitmaps/move_right.bmp"
#define IMGPTH_TLMVDOWN  "bitmaps/move_down.bmp"
#define IMGPTH_TLMVLEFT  "bitmaps/move_left.bmp"

/* Timer Configurations */

#define TIMER_INTERVAL 250

/* Cellular Automaton Configurations */

#define CA_CELLS_X 128
#define CA_CELLS_Y 64

#define CA_CELLS_NUM (CA_CELLS_X * CA_CELLS_Y)

/* Cellular Automaton Initialization Configurations */

#define CAINIT_RATIO 10
#define CAINIT_RATIO_AXIS 0

#define CAINIT_SRAND_RESEED_MOD 64

/* Canvas Configurations */

#define CANVAS_CELL_PIXELS_X 10
#define CANVAS_CELL_PIXELS_Y 10

#define CANVAS_GRID_PIXELS_X 1
#define CANVAS_GRID_PIXELS_Y 1

#define CANVAS_GRID_COLOR       wxColour(0x00,0x00,0x00)
#define CANVAS_CELL_ALIVE_COLOR wxColour(0x00,0x00,0x00)
#define CANVAS_CELL_DEAD_COLOR  wxColour(0xFF,0xFF,0xFF)

/* Derivative Macros */

	#define CAINIT_RATIO_MOD (CAINIT_RATIO + 1)

	#define CANVAS_CELL_WITH_GRID_PIXELS_X (CANVAS_CELL_PIXELS_X + CANVAS_GRID_PIXELS_X)
	#define CANVAS_CELL_WITH_GRID_PIXELS_Y (CANVAS_CELL_PIXELS_Y + CANVAS_GRID_PIXELS_Y)

	// The added 4 pixels reflects the area cut off at the right and bottom edges
	// by the simple border
	#define CANVAS_SIZE_X (CANVAS_CELL_WITH_GRID_PIXELS_X) * (CA_CELLS_X - 1) + CANVAS_CELL_PIXELS_X + 4
	#define CANVAS_SIZE_Y (CANVAS_CELL_WITH_GRID_PIXELS_Y) * (CA_CELLS_Y - 1) + CANVAS_CELL_PIXELS_Y + 4

/* Compile-Time Definitions */


#endif // DEFS_H_INCLUDED
