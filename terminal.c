#include <stdio.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

#ifndef _WIN32
static struct termios original_terminal;
static int original_flags;
#endif

// Configure terminal
void setup_terminal(void)
{
#ifdef _WIN32
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;

    if (GetConsoleMode(output, &mode))
    {
        SetConsoleMode(
            output,
            mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING
        );
    }
#else
    tcgetattr(STDIN_FILENO, &original_terminal);

    struct termios raw = original_terminal;

    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    original_flags = fcntl(STDIN_FILENO, F_GETFL, 0);

    fcntl(
        STDIN_FILENO,
        F_SETFL,
        original_flags | O_NONBLOCK
    );
#endif

    printf("\033[2J\033[?25l");
    fflush(stdout);
}

// Restore terminal settings
void restore_terminal(void)
{
    printf("\033[?25h\033[0m\n");
    fflush(stdout);

#ifndef _WIN32
    tcsetattr(
        STDIN_FILENO,
        TCSAFLUSH,
        &original_terminal
    );

    fcntl(
        STDIN_FILENO,
        F_SETFL,
        original_flags
    );
#endif
}

// Clear terminal screen
void clear_screen(void)
{
    printf("\033[H");
    fflush(stdout);
}

// Read keyboard input
int get_key(void)
{
#ifdef _WIN32

    if (_kbhit())
    {
        return _getch();
    }

    return 0;

#else

    unsigned char key;

    if (read(STDIN_FILENO, &key, 1) == 1)
    {
        return key;
    }

    return 0;

#endif
}

// Pause execution
void sleep_ms(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}
