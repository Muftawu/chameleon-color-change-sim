#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int width = 800;
int height = 800;
const char *title = "Chameleon Color Change Simulation";

#define COLOR_ALPHA 100
#define CHAMELEON_COLOR CLITERAL(Color) { 120, 120, 20, COLOR_ALPHA }
#define TARGET_COLOR CLITERAL(Color) { 10, 250, 255, COLOR_ALPHA }

#define MIN_PID_VALUE 0
#define MAX_PID_VALUE 255

int ENABLE_MOTION = 0;
int FAST_TRANSFORM = 0;
int PRINT_OUTPUT = 0;

// (10, 10, 255, 100) => 0.66, 0.039, 0.0029

#define Kp 0.66
#define Ki 0.039
#define Kd 0.0029

int proportional_r, proportional_g, proportional_b;
int integral_r, integral_g, integral_b;
int derivative_r, derivative_g, derivative_b;

int previous_proportional_r, previous_proportional_g, previous_proportional_b;

typedef struct Chameleon {
  float x, y;
  float radius;
  float speed_x, speed_y;
  Color color;
} Chameleon;

void render_chameleon(Chameleon cam) {
  DrawCircle(cam.x, cam.y, cam.radius, cam.color);
}

int Clamp(int min, int max, int value) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

int main(int argc, char *argv[]) {

  for (int i = 0; i < argc; i++) {
    printf("%d => %s \n", i, argv[i]);
    if (strcmp(argv[i], "motion") == 0) ENABLE_MOTION = 1;
    if (strcmp(argv[i], "output") == 0) PRINT_OUTPUT = 1;
    if (strcmp(argv[i], "fast") == 0) FAST_TRANSFORM = 1;
  }

  Chameleon cam;
  cam.x = 0.4 * width;
  cam.y = height / 2.0;
  cam.radius = 80.0f;
  cam.speed_x = 20.0;
  cam.speed_y = 20.0;
  cam.color = CHAMELEON_COLOR;

  InitWindow(width, height, title);
  float frameTime;

  while (!WindowShouldClose()) {
    if (!FAST_TRANSFORM) frameTime = GetFrameTime(); else frameTime = 0.013;

    if (ENABLE_MOTION) {
      cam.x += cam.speed_x * frameTime;
      cam.y += cam.speed_y * frameTime;
      if (cam.x - cam.radius <= 0 || cam.x + cam.radius >= width) cam.speed_x *= -1.0005;
      if (cam.y - cam.radius <= 0 || cam.y + cam.radius >= height) cam.speed_y *= -1.0005;
    }

    proportional_r = TARGET_COLOR.r - cam.color.r;
    proportional_g = TARGET_COLOR.g - cam.color.g;
    proportional_b = TARGET_COLOR.b - cam.color.b;

    integral_r += proportional_r * frameTime;
    integral_g += proportional_g * frameTime;
    integral_b += proportional_b * frameTime;

    if (!FAST_TRANSFORM) {
      integral_r = Clamp(MIN_PID_VALUE, MAX_PID_VALUE, integral_r);
      integral_g = Clamp(MIN_PID_VALUE, MAX_PID_VALUE, integral_g);
      integral_b = Clamp(MIN_PID_VALUE, MAX_PID_VALUE, integral_b);
    }

    derivative_r = (proportional_r - previous_proportional_r) / frameTime;
    derivative_g = (proportional_g - previous_proportional_g) / frameTime;
    derivative_b = (proportional_b - previous_proportional_b) / frameTime;

    int pid_color_r = Kp * proportional_r + Ki * integral_r + Kd * derivative_r;
    int pid_color_g = Kp * proportional_g + Ki * integral_g + Kd * derivative_b;
    int pid_color_b = Kp * proportional_b + Ki * integral_b + Kd * derivative_b;

    if (!FAST_TRANSFORM) {
      cam.color.r = Clamp(MIN_PID_VALUE, MAX_PID_VALUE, (cam.color.r + pid_color_r * frameTime));
      cam.color.g = Clamp(MIN_PID_VALUE, MAX_PID_VALUE, (cam.color.g + pid_color_g * frameTime));
      cam.color.b = Clamp(MIN_PID_VALUE, MAX_PID_VALUE, (cam.color.b + pid_color_b * frameTime));
    } else {
      cam.color.r = pid_color_r;
      cam.color.g = pid_color_g;
      cam.color.b = pid_color_b;
    }

    previous_proportional_r = proportional_r;
    previous_proportional_g = proportional_g;
    previous_proportional_b = proportional_b;

    if (PRINT_OUTPUT)
      printf("chameleon {%d %d %d} <==> PID {%d %d %d} \n", cam.color.r, cam.color.g, cam.color.b, TARGET_COLOR.r, TARGET_COLOR.g, TARGET_COLOR.b);

    if (abs(cam.color.r - TARGET_COLOR.r) < 1 && abs(cam.color.g - TARGET_COLOR.g) < 1 && abs(cam.color.b - TARGET_COLOR.b) < 1) { 
        printf("...COLOR MATCHING DONE...\n");
        break;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    DrawCircle(0.8 * width, height / 2.0, 30.0, TARGET_COLOR);

    DrawText(title, 0.1 * width, 0.05 * height, 30, WHITE);
    render_chameleon(cam);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
