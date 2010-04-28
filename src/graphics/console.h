#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <GL/glut.h>
#include <list>
#include <vector>
#include "../system/global.h"
#include "../system/io.h"
#include "OGLFT.h"

// REQUIRES SPECIAL BUILD RULE IN MAKEFILE!!!
#include "../../svnrev.h"

#define CONSOLE_WIDTH 650
#define CONSOLE_HEIGHT 320
#define CONSOLE_TOP 10
#define CONSOLE_LEFT 10

#define CONSOLE_UPDATE_SAMPLES 10

#define CONSOLE_INFO 0
#define CONSOLE_WARNING 1
#define CONSOLE_ERROR 2
#define CONSOLE_COMMAND 3

#define CONSOLE_MAX_LINES 23
#define CONSOLE_MAX_INPUT 78
#define CONSOLE_MAX_ARGS 10

typedef struct console_line_struct {
    int type;
    const char *line;
} console_line_t;

typedef struct console_cmd_struct {
    const char *name;
    void (*func)(int argc, char *argv[]);
} console_cmd_t;

class Console {
public:
    Console();

    void init();
    void draw();
    void update(int ms);

    void info(const char *format, ...);
    void warning(const char *format, ...);
    void error(const char *format, ...);

    void setLogLevel(int level) { _level = level; }

    void capture();
    void release();
    bool isCaptured() const { return _captured; }

    void process(const char *cmd);
    void registerCmd(const char *cmd, void (*func)(int argc, char *argv[]));

    static void key_down(unsigned char key, int x, int y);
    static void key_up(unsigned char key, int x, int y);
    static void special_key_down(int skey, int x, int y);
    static void special_key_up(int skey, int x, int y);

    static int _inputPos;
    static char _input[CONSOLE_MAX_INPUT];

private:
    OGLFT::Translucent *_titleFont;
    OGLFT::Monochrome *_consoleFont;
    int _updateTime;
    std::list<console_line_t> _lines;
    int _level;
    bool _captured;
    void command(const char *format, ...);
    std::vector<console_cmd_t> _cmds;
};

#endif
