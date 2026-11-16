#include <stdio.h>
#include "game.h"
#include "terminal.h"

int main(void)
{
    int choice = 0;

    setup_terminal();

    while (choice != 3)
    {
        clear_screen();

        printf("==============================\n");
        printf("        PONG ARENA\n");
        printf("==============================\n\n");

        printf("1. Player vs Player\n");
        printf("2. Player vs AI\n");
        printf("3. Exit\n\n");

        printf("Select an option: ");

        choice = get_key();

        switch (choice)
        {
            case '1':
                play_game(0);
                break;

            case '2':
                play_game(1);
                break;

            case '3':
                break;

            default:
                break;
        }
    }

    restore_terminal();

    printf("\nThanks for playing Pong Arena!\n");

    return 0;
}
