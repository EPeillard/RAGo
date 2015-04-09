/**
 * This file only contains define values such as the goban size for example
 *
 */

///This value indicate the margin (in pixels) accepted during the corners detections between
///the corners getted in the init() function
///and the center of the circle detected in the detection()
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

///Values used for the detection parameter
#define GRAY_SCALE 0.4

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
///Projector mod drawing a Aruco marker
#define PROJ_MOD_MARKER 5
///Projector mod drawing an Aruco marker filled board
#define PROJ_MOD_BOARD 6

///All of the following values are for the VirtualGoban

///Height of the full Virtual Goban
#define FULL_VG_HEIGHT 2100
///Width of the full Virtual Goban
#define FULL_VG_WIDTH 2400
///Width of the goban in the Virtual Goban
#define VG_WIDTH 1800
///Height of the goban in the Virtual Goban
#define VG_HEIGHT 1800
///Padding to make a margin between the goban and the border of the VirtualGoban
#define FULL_VG_PADDING 100
///Padding inside the goban to integrate the border of the phisical goban
#define VG_PADDING 50

///Stone size
#define STONE_SIZE 40

///Center of the clock
#define CLOCK_CENTER_X 2200
///Center of the clock
#define CLOCK_CENTER_Y 1050
///Center of the circles which signalise the hand is detected
#define CIRCLE_CENTER_Y 525
///Clock size
#define CLOCK_SIZE 150
///Width of the cicle
#define CIRCLE_WIDTH 20

///Ratio of the windows size for the marker size
#define RATIO_MARKER_SIZE 0.1

///Ratio between the Goban and the VirtualGoban
#define RATIO_G_VG 100
#define STONE_BORDER_SIZE 45

///Player void
#define PLAYER_NONE 0
///Player white
#define PLAYER_WHITE 1
///Player black
#define PLAYER_BLACK 2

///Localisation of the clocks
#define CLOCK_PLAYER_CENTER_X 1975
#define CLOCK_TIME_CENTER_X 2125
#define CLOCK_1_CENTER_Y 1500
#define CLOCK_2_CENTER_Y 1700
#define CLOCK_PLAYER_WIDTH 150
#define CLOCK_PLAYER_HEIGHT 100
#define CLOCK_TIME_WIDTH 150
#define CLOCK_TIME_HEIGHT 100

///Windows names
#define WINDOW_CAMERA "Vue caméra"
#define WINDOW_PROJECTOR "Affichage projecteur"
#define WINDOW_VERBOSE "Mode verbose"
#define WINDOW_VG "VirtualGoban"


