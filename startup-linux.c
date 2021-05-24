#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#include <string.h>
#include <getopt.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "graphics-gl.h"

// Open a library, using multiple fallback paths
#define dlopenfb(dl, ...) \
{ \
    const char *fname[] = { __VA_ARGS__ }; \
    for (int i = 0; i < sizeof(fname) / sizeof(*fname); i++) \
    { \
        dl = dlopen(fname[i], RTLD_LAZY); \
        if (dl != NULL) \
            break; \
    } \
}

void
error(const char *s, ...)
{
    va_list va;
    va_start(va, s);
    fprintf(stderr, "error: ");
    vfprintf(stderr, s, va);
    fputc('\n', stderr);
    va_end(va);
    exit(EXIT_FAILURE);
}

// OpenGL on Linux unfortunately requires GLX, which is dependent on
// X11. Otherwise, I'd just use XCB.
static void
setupGL()
{
    void *dlGL;
    dlopenfb(dlGL, "/usr/local/lib/libGL.so", "/usr/lib/libGL.so",
             "/lib/libGL.so", "./libGL.so");
    if (dlGL == NULL)
        error("failed to open libGL.so!");
    // Load the OpenGL functions used by the engine
    {
        FUNCTIONLOADER loaders[] =
        {
            { (void *) dlsym, dlGL }
        };
        loadGLFunctions(sizeof(loaders) / sizeof(*loaders), loaders);
    }
    printf("got glDrawElements at 0x%p\n", glDrawElements);
    dlclose(dlGL);
}

int
main(int argc, char *argv[])
{
    Display *display;
    Screen *screen;
    Window window, parentWindow;
    const char *displayName;
    int screenId;
    displayName = getenv("DISPLAY");
    display = XOpenDisplay(displayName);
    if (display == NULL && displayName != NULL)
        display = XOpenDisplay(NULL);
    if (display == NULL)
        error("failed to open X11 display");
    screen = DefaultScreenOfDisplay(display);
    screenId = DefaultScreen(display);
    parentWindow = RootWindowOfScreen(screen);
// These options are only enabled for the debug build. They're
// used to allow the game to be embedded into the editor.
#ifdef DEBUG
    int rPort, sPort;
nextopt:
    switch (getopt(argc, argv, "he:s:r:"))
    {
        case 'e': {        // embed in window by id
            parentWindow = (Window) strtol(optarg, NULL, 0);
            goto nextopt;
        }
        case 'h':
            puts("-e ID   - embed into window with ID\n"
                 "-r PORT - receive events on PORT\n"
                 "-s PORT - send events on PORT");
            exit(EXIT_SUCCESS);
        case 'r':          // receive events on port #
            rPort = atoi(optarg);
            goto nextopt;
        case 's':          // send events on port #
            sPort = atoi(optarg);
            goto nextopt;
    }
#endif
    window = XCreateSimpleWindow
    (
        display,                        // display
        parentWindow,                   // parent window
        0,                              // x position
        0,                              // y position
        320,                            // width
        240,                            // height
        1,
        BlackPixel(display, screenId),
        WhitePixel(display, screenId)
    );
    XSelectInput
    (
        display,
        window,
        KeyPressMask | KeyReleaseMask | KeymapStateMask
    );
    XClearWindow(display, window);
    XMapRaised(display, window);
    XEvent ev;
    KeySym keysym = 0;
    char str[25] = {0};
    int len, running = 1;
    while (running)
    {
        XNextEvent(display, &ev);
        switch (ev.type)
        {
            case Expose:
            {
                break;
            }
            case KeymapNotify:
            {
                XRefreshKeyboardMapping(&ev.xmapping);
                break;
            }
            case KeyPress:
            {
                len = XLookupString(&ev.xkey, str, 25, &keysym, NULL);
                if (isprint(*str) && !isspace(*str))
                    printf("key pressed: %s - %i\n", str, len);
                else
                    printf("key pressed: 0x%02X - %i\n", *str, len);
                if (keysym == XK_Escape)
                    running = 0;
                break;
            }
            case KeyRelease:
            {
                len = XLookupString(&ev.xkey, str, 25, &keysym, NULL);
                if (isprint(*str) && !isspace(*str))
                    printf("key released: %s - %i\n", str, len);
                else
                    printf("key released: 0x%02X - %i\n", *str, len);
            }
        }
    }
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}
