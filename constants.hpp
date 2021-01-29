#ifndef RZ_CONSTANTS_H
#define RZ_CONSTANTS_H

const int TILE_SIZE = 48;

const bool VSYNC = true;
const bool FULLSCREEN = false;

const int WINDOW_WIDTH = 2560;
const int WINDOW_HEIGHT = 1440;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int LEVEL_WIDTH = 64 * TILE_SIZE;
const int LEVEL_HEIGHT = SCREEN_HEIGHT + TILE_SIZE;

const float TRANS_X = float(WINDOW_WIDTH - SCREEN_WIDTH) / 2.0f;
const float TRANS_Y = float(WINDOW_HEIGHT - SCREEN_HEIGHT) / 2.0f;
const float SCALE_X = 1.0f / ((float)WINDOW_WIDTH / (float)SCREEN_WIDTH);
const float SCALE_Y = 1.0f / ((float)WINDOW_HEIGHT / (float)SCREEN_HEIGHT);

#endif
