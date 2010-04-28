#include "console.h"
#include "graphics.h"

int Console::_inputPos = 0;
char Console::_input[CONSOLE_MAX_INPUT] = {219, '\0'};

Console::Console()
{
    _titleFont = NULL;
    _consoleFont = NULL;
    _level = CONSOLE_INFO;
    _captured = false;
}

void Console::init()
{
    // load the fonts
    _titleFont = new OGLFT::Translucent("resources/fonts/sui_generis_free.ttf", 12);
    if (_titleFont->isValid())
    {
        _titleFont->setForegroundColor(1.0, 1.0, 1.0, 1.0);
    }
    else
    {
        fprintf(stderr, "WARNING: Failed to load debugging console title font!\n");
    }
    _consoleFont = new OGLFT::Monochrome("resources/fonts/terminal.ttf", 9);
    if (!_consoleFont->isValid())
    {
        fprintf(stderr, "WARNING: Failed to load debugging console monospace font!\n");
    }

    info("Initialized debugging console.");
}

void Console::draw()
{
    char buffer[25];

    // draw the translucent background
    glBegin(GL_QUADS);
        glColor4f(0.0, 0.0, 0.0, 0.5);
        glVertex2i(CONSOLE_LEFT, global::height - CONSOLE_TOP);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH, global::height - CONSOLE_TOP);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH, global::height - CONSOLE_TOP - CONSOLE_HEIGHT);
        glVertex2i(CONSOLE_LEFT, global::height - CONSOLE_TOP - CONSOLE_HEIGHT);
    glEnd();

    // draw the header
    glBegin(GL_QUADS);
        if (!_captured)
        {
            glColor4f(0.0, 0.0, 0.0, 0.5);
        }
        else
        {
            glColor4f(1.0, 0.0, 0.0, 0.5);
        }
        glVertex2i(CONSOLE_LEFT, global::height - CONSOLE_TOP);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH, global::height - CONSOLE_TOP);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH, global::height - CONSOLE_TOP - 25);
        glVertex2i(CONSOLE_LEFT, global::height - CONSOLE_TOP - 25);
    glEnd();

    // if our fonts didn't load, kinda no point in going further...
    if (!_titleFont->isValid() || !_consoleFont->isValid()) return;

    // draw the title
    _titleFont->setHorizontalJustification(OGLFT::Face::LEFT);
    _titleFont->draw(CONSOLE_LEFT + 6, global::height - CONSOLE_TOP - 25 + 6, "Debug Console");

    // draw the svn revision number
    snprintf(buffer, 25, "Rev #%s", SVN_REV);
    _titleFont->setHorizontalJustification(OGLFT::Face::CENTER);
    _titleFont->draw(CONSOLE_LEFT + (CONSOLE_WIDTH / 2), global::height - CONSOLE_TOP - 25 + 6, buffer);

    // draw the update loop time
    snprintf(buffer, 25, "%d ms", _updateTime);
    _titleFont->setHorizontalJustification(OGLFT::Face::RIGHT);
    _titleFont->draw(CONSOLE_LEFT + CONSOLE_WIDTH - 6 - 100, global::height - CONSOLE_TOP - 25 + 6, buffer);
    
    // draw the fps
    snprintf(buffer, 25, "%d fps", global::fps);
    _titleFont->setHorizontalJustification(OGLFT::Face::RIGHT);
    _titleFont->draw(CONSOLE_LEFT + CONSOLE_WIDTH - 6, global::height - CONSOLE_TOP - 25 + 6, buffer);

    // print each line of console output
    int line_num = 0;
    for (std::list<console_line_t>::iterator it = _lines.begin(); it != _lines.end(); it++)
    {
        switch (it->type)
        {
            case CONSOLE_COMMAND:
                _consoleFont->setForegroundColor(0.0, 1.0, 0.0, 1.0);
                break;

            case CONSOLE_ERROR:
                _consoleFont->setForegroundColor(1.0, 0.4, 0.4, 1.0);
                break;

            case CONSOLE_WARNING:
                _consoleFont->setForegroundColor(1.0, 1.0, 0.0, 1.0);
                break;

            case CONSOLE_INFO:
            default:
                _consoleFont->setForegroundColor(1.0, 1.0, 1.0, 1.0);
                break;
        }

        _consoleFont->draw(CONSOLE_LEFT + 6, global::height - CONSOLE_TOP - 25 - ((line_num + 1) * 12), it->line);
        line_num++;
    }
    
    // draw the footer
    glBegin(GL_QUADS);
        if (!_captured)
        {
            glColor4f(0.0, 0.0, 0.0, 0.5);
        }
        else
        {
            glColor4f(1.0, 0.0, 0.0, 0.5);
        }
        glVertex2i(CONSOLE_LEFT, global::height - CONSOLE_TOP - CONSOLE_HEIGHT + 15);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH, global::height - CONSOLE_TOP - CONSOLE_HEIGHT + 15);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH, global::height - CONSOLE_TOP - CONSOLE_HEIGHT);
        glVertex2i(CONSOLE_LEFT, global::height - CONSOLE_TOP - CONSOLE_HEIGHT);
    glEnd();

    // draw the prompt
    _consoleFont->setForegroundColor(1.0, 1.0, 1.0, 1.0);
    _consoleFont->draw(CONSOLE_LEFT + 6, global::height - CONSOLE_TOP - CONSOLE_HEIGHT + 4, ">");
    _consoleFont->draw(CONSOLE_LEFT + 20, global::height - CONSOLE_TOP - CONSOLE_HEIGHT + 4, _input);

}

void Console::update(int ms)
{
    static int updateTimeCtr = 0;
    static int updateTimes[CONSOLE_UPDATE_SAMPLES]; // average over many samples

    // use a circular buffer to average update times over CONSOLE_UPDATE_SAMPLES updates
    updateTimes[updateTimeCtr] = ms;
    updateTimeCtr++;
    if (updateTimeCtr >= CONSOLE_UPDATE_SAMPLES)
    {
        int sum = 0;
        for (int i = 0; i < CONSOLE_UPDATE_SAMPLES; i++)
        {
            sum += updateTimes[i];
        }
        _updateTime = sum / CONSOLE_UPDATE_SAMPLES;
        updateTimeCtr = 0;
    }
}

void Console::info(const char *format, ...)
{
    // only write if log level permits it
    if (_level > CONSOLE_INFO) return;

    va_list ap;
    va_start(ap, format);

    // allocate space for the line
    char *buffer = (char *)malloc(sizeof(char) * 81);

    // do the print into the buffer
    vsnprintf(buffer, 81, format, ap);

    // create the line type
    console_line_t line;
    line.type = CONSOLE_INFO;
    line.line = buffer;

    // add it to the end of the list
    _lines.push_back(line);

    // pop off the front if it's full
    if (_lines.size() > CONSOLE_MAX_LINES)
    {
        free((void *)(_lines.front().line));
        _lines.pop_front();
    }

    va_end(ap);
}

void Console::warning(const char *format, ...)
{
    // only write if log level permits it
    if (_level > CONSOLE_WARNING) return;

    va_list ap;
    va_start(ap, format);

    // allocate space for the line
    char *buffer = (char *)malloc(sizeof(char) * 81);

    // do the print into the buffer
    vsnprintf(buffer, 81, format, ap);

    // create the line type
    console_line_t line;
    line.type = CONSOLE_WARNING;
    line.line = buffer;

    // add it to the end of the list
    _lines.push_back(line);

    // pop off the front if it's full
    if (_lines.size() > CONSOLE_MAX_LINES)
    {
        free((void *)(_lines.front().line));
        _lines.pop_front();
    }

    va_end(ap);
}

void Console::error(const char *format, ...)
{
    // only write if log level permits it
    if (_level > CONSOLE_ERROR) return;

    va_list ap;
    va_start(ap, format);

    // allocate space for the line
    char *buffer = (char *)malloc(sizeof(char) * 81);

    // do the print into the buffer
    vsnprintf(buffer, 81, format, ap);

    // create the line type
    console_line_t line;
    line.type = CONSOLE_ERROR;
    line.line = buffer;

    // add it to the end of the list
    _lines.push_back(line);

    // pop off the front if it's full
    if (_lines.size() > CONSOLE_MAX_LINES)
    {
        free((void *)(_lines.front().line));
        _lines.pop_front();
    }

    va_end(ap);
}

void Console::command(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    // allocate space for the line
    char *buffer = (char *)malloc(sizeof(char) * 81);

    // do the print into the buffer
    vsnprintf(buffer, 81, format, ap);

    // create the line type
    console_line_t line;
    line.type = CONSOLE_COMMAND;
    line.line = buffer;

    // add it to the end of the list
    _lines.push_back(line);

    // pop off the front if it's full
    if (_lines.size() > CONSOLE_MAX_LINES)
    {
        free((void *)(_lines.front().line));
        _lines.pop_front();
    }

    va_end(ap);
}

void Console::capture()
{
    _captured = true;

    // steal glut keyboard callbacks
    glutKeyboardFunc(&Console::key_down);
    glutKeyboardUpFunc(&Console::key_up);
    glutSpecialFunc(&Console::special_key_down);
    glutSpecialUpFunc(&Console::special_key_up);
}

void Console::release()
{
    _captured = false;

    // restore glut keyboard callbacks
    glutKeyboardFunc(&io::key_down);
    glutKeyboardUpFunc(&io::key_up);
    glutSpecialFunc(&io::special_key_down);
    glutSpecialUpFunc(&io::special_key_up);
}

void Console::process(const char *cmd)
{
    char buffer[CONSOLE_MAX_INPUT];

    // first things first, strip off any leading spaces
    bool only_spaces = true;
    int i = 0;
    int j = 0;
    for (i = 0; i < strlen(cmd); i++)
    {
        if (isspace(cmd[i]) && only_spaces) continue;
        buffer[j] = cmd[i];
        j++;
        only_spaces = false;
    }
    buffer[j] = '\0';

    // is it empty? (command was just whitespace)
    if (strlen(buffer) == 0) return;

    // echo it back to the console
    command(buffer);

    // if the first character is a '$' hand it to the shell
    // for processing
    // TODO
    
    // parse it into its argc and argv
    int argc = 0;
    char **argv = (char **)malloc(sizeof(char *) * CONSOLE_MAX_ARGS);
    for (i = 0; i < CONSOLE_MAX_ARGS; i++)
    {
        argv[i] = (char *)malloc(sizeof(char) * CONSOLE_MAX_INPUT);
        argv[i][0] = '\0';
    }
    int buffer_pos = 0;
    int argv_pos = 0;
    bool eating_spaces = false;
    while (strlen(buffer + buffer_pos) > 0)
    {
        if (!isspace(buffer[buffer_pos]))
        {
            argv[argc][argv_pos] = buffer[buffer_pos];
            argv_pos++;
            eating_spaces = false;
        }
        else
        {
            if (!eating_spaces)
            {
                argv[argc][argv_pos] = '\0';
                argc++;
                eating_spaces = true;
                argv_pos = 0;
            }
        }
        buffer_pos++;
    }
    argv[argc][argv_pos] = '\0';
    if (!isspace(buffer[strlen(buffer) - 1])) argc++;

    // search through our list of functions for one with a matching name
    bool found = false;
    for (i = 0; i < _cmds.size(); i++)
    {
        if (strcmp(_cmds[i].name, argv[0]) == 0)
        {
            // match! call the function!
            _cmds[i].func(argc, (char **)argv);
            found = true;
            break;
        }
    }
    if (!found) error("%s: command not found", argv[0]);

    for (i = 0; i < CONSOLE_MAX_ARGS; i++)
    {
        free(argv[i]);
    }
    free(argv);
}

void Console::registerCmd(const char *name, void (*func)(int argc, char *argv[]))
{
    // override the command if it already exists
    bool found = false;
    for (int i = 0; i < _cmds.size(); i++)
    {
        if (strcmp(_cmds[i].name, name) == 0)
        {
            // match! override the function!
            _cmds[i].func = func;
            found = true;
            break;
        }
    }

    if (!found)
    {
        // add it
        console_cmd_t cmd;
        cmd.name = name;
        cmd.func = func;
        _cmds.push_back(cmd);
    }
}

void Console::key_down(unsigned char key, int x, int y)
{
    // check for release key
    if (key == '`')
    {
        gfx::hud.console.release();
        gfx::hud.showConsole(false);
        return;
    }
   
    // check for special keys
    switch (key)
    {
        case 8: // backspace
            if (strlen(Console::_input) > 1)
            {
                Console::_inputPos--;
                Console::_input[Console::_inputPos] = 219; // block cursor
                Console::_input[Console::_inputPos + 1] = '\0';
            }
            return;
            break;

        case 13: // enter
            if (strlen(Console::_input) > 1)
            {
                Console::_input[Console::_inputPos] = '\0';
                gfx::hud.console.process(Console::_input);
                
                Console::_inputPos = 0;
                Console::_input[Console::_inputPos] = 219; // block cursor
                Console::_input[Console::_inputPos + 1] = '\0';
            }
            return;
            break;

        case 127: // delete

            break;
    }

    if (strlen(Console::_input) < CONSOLE_MAX_INPUT - 2)
    {
        Console::_input[Console::_inputPos] = key;
        Console::_inputPos++;
        Console::_input[Console::_inputPos] = 219; // block cursor
        Console::_input[Console::_inputPos + 1] = '\0';
    }
}

void Console::key_up(unsigned char key, int x, int y)
{
    // dont care, just need to override the default
}

void Console::special_key_down(int skey, int x, int y)
{
    // dont care, just need to override the default
}

void Console::special_key_up(int skey, int x, int y)
{
    // dont care, just need to override the default
}
