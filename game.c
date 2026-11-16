#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "terminal.h"

// Reset ball position
void reset_ball(Ball *ball, int direction)
{
    ball->x = WIDTH / 2.0f;
    ball->y = HEIGHT / 2.0f;

    ball->dx = direction * 0.65f;

    ball->dy = (rand() % 2 == 0) ? 0.35f : -0.35f;
}

// Draw game screen
void draw_game(Ball ball, Paddle left, Paddle right,
               int ai_mode)
{
    clear_screen();

    printf("PLAYER 1: %d", left.score);

    if (ai_mode)
        printf("                         AI: %d\n", right.score);
    else
        printf("                    PLAYER 2: %d\n", right.score);

    printf("+");

    for (int i = 0; i < WIDTH; i++)
        printf("-");

    printf("+\n");

    for (int y = 0; y < HEIGHT; y++)
    {
        printf("|");

        for (int x = 0; x < WIDTH; x++)
        {
            char pixel = ' ';

            // Center line
            if (x == WIDTH / 2 && y % 2 == 0)
                pixel = ':';

            // Left paddle
            if (x == 1 &&
                y >= left.y &&
                y < left.y + PADDLE_SIZE)
                pixel = '|';

            // Right paddle
            if (x == WIDTH - 2 &&
                y >= right.y &&
                y < right.y + PADDLE_SIZE)
                pixel = '|';

            // Ball
            if ((int)(ball.x + 0.5f) == x &&
                (int)(ball.y + 0.5f) == y)
                pixel = 'O';

            printf("%c", pixel);
        }

        printf("|\n");
    }

    printf("+");

    for (int i = 0; i < WIDTH; i++)
        printf("-");

    printf("+\n");

    printf("\nW/S = Player 1");

    if (!ai_mode)
        printf("    I/K = Player 2");

    printf("    Q = Quit\n");

    fflush(stdout);
}

// Move AI paddle
void update_ai(Paddle *ai, Ball ball)
{
    int target = (int)ball.y - PADDLE_SIZE / 2;

    if (ball.dx > 0)
    {
        if (ai->y + PADDLE_SIZE / 2 < target)
            ai->y++;
        else if (ai->y + PADDLE_SIZE / 2 > target)
            ai->y--;
    }

    if (ai->y < 0)
        ai->y = 0;

    if (ai->y > HEIGHT - PADDLE_SIZE)
        ai->y = HEIGHT - PADDLE_SIZE;
}

// Ball movement and collision
void move_ball(Ball *ball, Paddle *left, Paddle *right)
{
    ball->x += ball->dx;
    ball->y += ball->dy;

    // Top and bottom walls
    if (ball->y <= 0 || ball->y >= HEIGHT - 1)
    {
        ball->dy = -ball->dy;
    }

    // Left paddle collision
    if (ball->dx < 0 &&
        ball->x <= 2 &&
        ball->y >= left->y &&
        ball->y < left->y + PADDLE_SIZE)
    {
        ball->dx = -ball->dx;

        float hit =
            (ball->y - left->y) / (float)PADDLE_SIZE;

        ball->dy = (hit - 0.5f) * 1.2f;
    }

    // Right paddle collision
    if (ball->dx > 0 &&
        ball->x >= WIDTH - 3 &&
        ball->y >= right->y &&
        ball->y < right->y + PADDLE_SIZE)
    {
        ball->dx = -ball->dx;

        float hit =
            (ball->y - right->y) / (float)PADDLE_SIZE;

        ball->dy = (hit - 0.5f) * 1.2f;
    }
}

// Main game loop
void play_game(int ai_mode)
{
    Paddle left = { HEIGHT / 2 - 2, 0 };
    Paddle right = { HEIGHT / 2 - 2, 0 };

    Ball ball;

    reset_ball(&ball, 1);

    int quit_game = 0;

    while (left.score < WIN_SCORE &&
           right.score < WIN_SCORE &&
           !quit_game)
    {
        int key = get_key();

        // Player 1 controls
        if (key == 'w' || key == 'W')
            left.y--;

        if (key == 's' || key == 'S')
            left.y++;

        // Player 2 controls
        if (!ai_mode)
        {
            if (key == 'i' || key == 'I')
                right.y--;

            if (key == 'k' || key == 'K')
                right.y++;
        }

        // Quit game
        if (key == 'q' || key == 'Q')
        {
            quit_game = 1;
            break;
        }

        // Keep paddles inside field
        if (left.y < 0)
            left.y = 0;

        if (left.y > HEIGHT - PADDLE_SIZE)
            left.y = HEIGHT - PADDLE_SIZE;

        if (right.y < 0)
            right.y = 0;

        if (right.y > HEIGHT - PADDLE_SIZE)
            right.y = HEIGHT - PADDLE_SIZE;

        // AI movement
        if (ai_mode)
            update_ai(&right, ball);

        // Move ball
        move_ball(&ball, &left, &right);

        // Player 2 scores
        if (ball.x < 0)
        {
            right.score++;

            reset_ball(&ball, 1);

            sleep_ms(500);
        }

        // Player 1 scores
        if (ball.x >= WIDTH)
        {
            left.score++;

            reset_ball(&ball, -1);

            sleep_ms(500);
        }

        // Draw updated screen
        draw_game(ball, left, right, ai_mode);

        sleep_ms(35);
    }

    if (quit_game)
        return;

    // Game over screen
    clear_screen();

    printf("\n==============================\n");
    printf("          GAME OVER!\n");
    printf("==============================\n\n");

    if (left.score > right.score)
    {
        printf("PLAYER 1 WINS!\n");
    }
    else if (ai_mode)
    {
        printf("AI WINS!\n");
    }
    else
    {
        printf("PLAYER 2 WINS!\n");
    }

    printf("\nFinal Score: %d - %d\n",
           left.score, right.score);

    printf("\nPress any key to return to menu...");

    fflush(stdout);

    while (!get_key())
        sleep_ms(20);
}
