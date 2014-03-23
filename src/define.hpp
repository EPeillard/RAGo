/**
 * This file only contains define values such as the goban size for example
 *
 */

///This value indicate the margin (in pixels) accepted during the corners detections between
///the corners getted in the @function init() function
///and the center of the circle detected in the @function detection()
#define MARGIN_MARKERS_CALIB 2

///Goban size (number of columns/rows)
///It should be a variable to be able to use differents sizes of goban
#define GOBAN_SIZE 19

///Max number of corners
#define CORNER_NUMBER 4

///Height of the camera capture in pixels
#define CAMERA_HEIGHT 720
///Height of the camera capture in pixels
#define CAMERA_WIDTH 1280

///Height of the projector image in pixels
#define PROJECTOR_HEIGHT 768
///Height of the projector image in pixels
#define PROJECTOR_WIDTH 1024

///Projector mod drawing the corners and borders of the goban detected
#define PROJ_MOD_BORDERS 0
///Projector mod unused
#define PROJ_MOD_1 1
///Projector mod drawing the detection point
#define PROJ_MOD_DETECTION 2
///Projector mod drawing the stone played by a terminal player
#define PROJ_MOD_STONE 3
///Projector mod drawing the clock
#define PROJ_MOD_CLOCK 4

#define FULL_VG_HEIGHT 2100
#define FULL_VG_WIDTH 2400
#define VG_WIDTH 1800
#define VG_HEIGHT 1800
#define FULL_VG_PADDING 100
#define VG_PADDING 50

#define STONE_SIZE 40

#define CLOCK_CENTER_X 2200
#define CLOCK_CENTER_Y 1050
#define CIRCLE_CENTER_Y 525
#define CLOCK_SIZE 150
#define CIRCLE_WIDTH 20

#define RATIO_G_VG 100
#define STONE_BORDER_SIZE 45

#define PLAYER_NONE 0
#define PLAYER_WHITE 1
#define PLAYER_BLACK 2

#define CLOCK_PLAYER_CENTER_X 1975
#define CLOCK_TIME_CENTER_X 2125
#define CLOCK_1_CENTER_Y 1500
#define CLOCK_2_CENTER_Y 1700
#define CLOCK_PLAYER_WIDTH 150
#define CLOCK_PLAYER_HEIGHT 100
#define CLOCK_TIME_WIDTH 150
#define CLOCK_TIME_HEIGHT 100


