#include <stdio.h>

#include "graphics.h"
#include "graphics-gl.h"
#include "memory.h"

void
render_gl(void)
{
    glClearColor(0.4, 0.6, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    static int initialized = 0;
    static GLuint vCache;
    if (!initialized)
    {
        // Set up the vertex cache
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, vCache);
        glBufferData
        (
            GL_TRANSFORM_FEEDBACK_BUFFER,
            32 * sizeof(Vert),
            NULL,
            GL_DYNAMIC_COPY
        );
        initialized = 1;
    }

    GCmd *gCmd;
    switch (gCmd->w0 >> 24)
    {
        case G_NOOP:
            break;
        case G_LOADVERTS:
            glBufferSubData
            (
                GL_ARRAY_BUFFER,
                ((gCmd->w0 >> 12) & 0xFFF) * sizeof(Vert),
                ((gCmd->w0 & 0xFFF) + 1) * sizeof(Vert),
                virtualToPhysical(gCmd->w1)
            );
            glBeginTransformFeedback(GL_POINTS);
            //glDrawElements();
            glEndTransformFeedback();
            break;
        case G_DRAWTRIS:
            break;
        case G_END:
            break;
    }
}

#define LOAD(TYPE, NAME) \
    for (int i = 0; i < nLoaders && NAME == NULL; i++) \
        NAME = (TYPE) loader[i].load(loader[i].object, #NAME); \
    if (NAME == NULL) \
        fprintf(stderr, "error: failed to load " #NAME ".\n");

PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = NULL;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer = NULL;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = NULL;

PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = NULL;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = NULL;
PFNGLISRENDERBUFFERPROC glIsRenderbuffer = NULL;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = NULL;

PFNGLCLEARPROC glClear = NULL;
PFNGLCLEARCOLORPROC glClearColor = NULL;

PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLISPROGRAMPROC glIsProgram = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLDETACHSHADERPROC glDetachShader = NULL;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;

PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLISSHADERPROC glIsShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
PFNGLISVERTEXARRAYPROC glIsVertexArray = NULL;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;

PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLISBUFFERPROC glIsBuffer = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback = NULL;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback = NULL;

PFNGLDRAWELEMENTSPROC glDrawElements = NULL;

void
loadGLFunctions(int nLoaders, FUNCTIONLOADER loader[])
{
    // Load the framebuffer functions
    {
        LOAD(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
        LOAD(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers);
        LOAD(PFNGLISFRAMEBUFFERPROC, glIsFramebuffer);
        LOAD(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
        LOAD(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer);
        LOAD(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
    }
    // Load the renderbuffer functions
    {
        LOAD(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers);
        LOAD(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers);
        LOAD(PFNGLISRENDERBUFFERPROC, glIsRenderbuffer);
        LOAD(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer);
    }
    // Load general functions
    {
        LOAD(PFNGLCLEARPROC, glClear);
        LOAD(PFNGLCLEARCOLORPROC, glClearColor);
    }
    // Load program functions
    {
        LOAD(PFNGLCREATEPROGRAMPROC, glCreateProgram);
        LOAD(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
        LOAD(PFNGLISPROGRAMPROC, glIsProgram);
        LOAD(PFNGLATTACHSHADERPROC, glAttachShader);
        LOAD(PFNGLDETACHSHADERPROC, glDetachShader);
        LOAD(PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation);
        LOAD(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
        LOAD(PFNGLLINKPROGRAMPROC, glLinkProgram);
        LOAD(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
        LOAD(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
        LOAD(PFNGLUSEPROGRAMPROC, glUseProgram);
    }
    // Load shader functions
    {
        LOAD(PFNGLCREATESHADERPROC, glCreateShader);
        LOAD(PFNGLDELETESHADERPROC, glDeleteShader);
        LOAD(PFNGLISSHADERPROC, glIsShader);
        LOAD(PFNGLSHADERSOURCEPROC, glShaderSource);
        LOAD(PFNGLCOMPILESHADERPROC, glCompileShader);
        LOAD(PFNGLGETSHADERIVPROC, glGetShaderiv);
        LOAD(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
    }
    // Load vertex array functions
    {
        LOAD(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
        LOAD(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
        LOAD(PFNGLISVERTEXARRAYPROC, glIsVertexArray);
        LOAD(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
        LOAD(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
        LOAD(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
        LOAD(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
    }
    // Load buffer functions
    {
        LOAD(PFNGLGENBUFFERSPROC, glGenBuffers);
        LOAD(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
        LOAD(PFNGLISBUFFERPROC, glIsBuffer);
        LOAD(PFNGLBINDBUFFERPROC, glBindBuffer);
        LOAD(PFNGLBUFFERDATAPROC, glBufferData);
        LOAD(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
        LOAD(PFNGLBEGINTRANSFORMFEEDBACKPROC, glBeginTransformFeedback);
        LOAD(PFNGLENDTRANSFORMFEEDBACKPROC, glEndTransformFeedback);
    }
    // Load drawing functions
    {
        LOAD(PFNGLDRAWELEMENTSPROC, glDrawElements);
    }
}
