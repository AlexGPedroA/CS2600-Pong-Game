#ifndef GAME_H
#define GAME_H

// Game dimensions
#define WIDTH 60
#define HEIGHT 20

// Game settings
#define PADDLE_SIZE 4
#define WIN_SCORE 5

// Ball structure
typedef struct
{
    float x;
    float y;

    float dx;
    float dy;

} Ball;

// Paddle structure
typedef struct
{
    int y;
    int score;

} Paddle;

// Game function declarations
void play_game(int ai_mode);

void draw_game(Ball ball, Paddle left, Paddle right,
               int ai_mode);

void move_ball(Ball *ball, Paddle *left, Paddle *right);

void update_ai(Paddle *ai, Ball ball);

void reset_ball(Ball *ball, int direction);

#endif
