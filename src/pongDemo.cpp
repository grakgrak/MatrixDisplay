/*
 * Pong
 * Original Code from https://github.com/rparrett/pongclock
 *
 */

// Demo only - not playable

#include "Arduino.h"
#include "matrix.h"

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB

int16_t h = 32;
int16_t w = 64;

int16_t paddle_h = 5;
int16_t paddle_w = 1;

int16_t lpaddle_x = 0;
int16_t rpaddle_x = w - paddle_w;

int16_t lpaddle_y = 0;
int16_t rpaddle_y = h - paddle_h;

int16_t lpaddle_d = 1;
int16_t rpaddle_d = -1;

int16_t lpaddle_ball_t = w - w / 4;
int16_t rpaddle_ball_t = w / 4;

int16_t target_y = 0;

int16_t ball_x = 2;
int16_t ball_y = 2;
int16_t oldball_x = 2;
int16_t oldball_y = 2;

int16_t ball_dx = 1;
int16_t ball_dy = 1;

int16_t ball_w = 2;
int16_t ball_h = 2;

int16_t dashline_h = 2;
int16_t dashline_w = 1;
int16_t dashline_n = h / dashline_h;
int16_t dashline_x = w / 2 - 1;
int16_t dashline_y = dashline_h / 2;

int16_t lscore = 12;
int16_t rscore = 4;

void midline() {
  matrix.drawFastVLine(32, 0, 32, RGB(0,0,127)); // push dash pixels
}

void lpaddle() {
  
//   if (lpaddle_d == 1) {
//     tft.fillRect(lpaddle_x, lpaddle_y, paddle_w, 1, BLACK);
//   } 
//   else if (lpaddle_d == -1) {
//     tft.fillRect(lpaddle_x, lpaddle_y + paddle_h - 1, paddle_w, 1, BLACK);
//   }

  lpaddle_y = lpaddle_y + lpaddle_d;

  if (ball_dx == 1) lpaddle_d = 0;
  else {
    if (lpaddle_y + paddle_h / 2 == target_y) lpaddle_d = 0;
    else if (lpaddle_y + paddle_h / 2 > target_y) lpaddle_d = -1;
    else lpaddle_d = 1;
  }

  if (lpaddle_y + paddle_h >= h && lpaddle_d == 1) lpaddle_d = 0;
  else if (lpaddle_y <= 0 && lpaddle_d == -1) lpaddle_d = 0;

  matrix.writeFastVLine(lpaddle_x, lpaddle_y, paddle_h, WHITE);
}

void rpaddle() {
  
//   if (rpaddle_d == 1) {
//     tft.fillRect(rpaddle_x, rpaddle_y, paddle_w, 1, BLACK);
//   } 
//   else if (rpaddle_d == -1) {
//     tft.fillRect(rpaddle_x, rpaddle_y + paddle_h - 1, paddle_w, 1, BLACK);
//   }

  rpaddle_y = rpaddle_y + rpaddle_d;

  if (ball_dx == -1) rpaddle_d = 0;
  else {
    if (rpaddle_y + paddle_h / 2 == target_y) rpaddle_d = 0;
    else if (rpaddle_y + paddle_h / 2 > target_y) rpaddle_d = -1;
    else rpaddle_d = 1;
  }

  if (rpaddle_y + paddle_h >= h && rpaddle_d == 1) rpaddle_d = 0;
  else if (rpaddle_y <= 0 && rpaddle_d == -1) rpaddle_d = 0;

  matrix.writeFastVLine(rpaddle_x, rpaddle_y, paddle_h, WHITE);
}

void calc_target_y() {
  int16_t target_x;
  int16_t reflections;
  int16_t y;

  if (ball_dx == 1) {
    target_x = w - ball_w;
  } 
  else {
    target_x = -1 * (w - ball_w);
  }

  y = abs(target_x * (ball_dy / ball_dx) + ball_y);

  reflections = floor(y / h);

  if (reflections % 2 == 0) {
    target_y = y % h;
  } 
  else {
    target_y = h - (y % h);
  }
}

void ball() {
  ball_x = ball_x + ball_dx;
  ball_y = ball_y + ball_dy;

  if (ball_dx == -1 && ball_x == paddle_w && ball_y + ball_h >= lpaddle_y && ball_y <= lpaddle_y + paddle_h) {
    ball_dx = ball_dx * -1;
    calc_target_y(); 
  } 
  else if (ball_dx == 1 && ball_x + ball_w == w - paddle_w && ball_y + ball_h >= rpaddle_y && ball_y <= rpaddle_y + paddle_h) {
    ball_dx = ball_dx * -1;
    calc_target_y();
  } 

  if (ball_y > h - ball_w || ball_y < 0) {
    ball_dy = ball_dy * -1;
    ball_y += ball_dy; // Keep in bounds
  }

  matrix.drawRect(ball_x, ball_y, ball_w, ball_h, WHITE);

  oldball_x = ball_x;
  oldball_y = ball_y;
}

void initgame() {
  lpaddle_y = random(0, h - paddle_h);
  rpaddle_y = random(0, h - paddle_h);

  // ball is placed on the center of the left paddle
  ball_y = lpaddle_y + (paddle_h / 2);
  
  calc_target_y();
}

void render() {
    vTaskDelay(50);

    matrix.startWrite();
    matrix.black();

    lpaddle();
    rpaddle();

    midline();

    ball();

    matrix.endWrite();
}

void pongDemo()
{
    initgame();

    matrix.setTextColor(RGB(127,127,127));

    for(int i = 0; i < 2000; i++)
        render();
}