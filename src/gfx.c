#include "gfx.h"

#include "tga.h"
#include "time.h"
#include "debug.h"

#define _GFX_MAX_VERTICES 1048576

typedef void *(*wglCreateContextAttribsARB_t)(void*,void*,int32_t*);

typedef uint32_t GLenum;
typedef uint8_t GLboolean;
typedef uint32_t GLbitfield;
typedef int8_t GLbyte;
typedef int16_t GLshort;
typedef int32_t GLint;
typedef int32_t GLsizei;
typedef uint8_t GLubyte;
typedef uint16_t GLushort;
typedef uint32_t GLuint;
typedef ptrdiff_t GLsizeiptr, GLintptr;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
typedef char GLchar;

#define GL_RENDERER                        0x1F01
#define GL_VERSION                         0x1F02
#define GL_COLOR_BUFFER_BIT                0x00004000
#define GL_ARRAY_BUFFER                    0x8892
#define GL_NO_ERROR                        0
#define GL_DYNAMIC_DRAW                    0x88E8
#define GL_MAP_READ_BIT                    0x0001
#define GL_MAP_WRITE_BIT                   0x0002
#define GL_MAP_COHERENT_BIT                0x0080
#define GL_MAP_PERSISTENT_BIT              0x0040
#define GL_VERTEX_SHADER                   0x8B31
#define GL_FRAGMENT_SHADER                 0x8B30
#define GL_COMPILE_STATUS                  0x8B81
#define GL_LINK_STATUS                     0x8B82
#define GL_FLOAT                           0x1406
#define GL_TRIANGLES                       0x0004
#define GL_TEXTURE_2D                      0x0DE1
#define GL_RGBA8                           0x8058
#define GL_MAX_TEXTURE_SIZE                0x0D33
#define GL_RGBA                            0x1908
#define GL_UNSIGNED_BYTE                   0x1401
#define GL_TEXTURE0                        0x84C0
#define GL_TEXTURE_MAG_FILTER              0x2800
#define GL_TEXTURE_MIN_FILTER              0x2801
#define GL_NEAREST                         0x2600
#define GL_LINEAR                          0x2601
#define GL_BLEND                           0x0BE2
#define GL_SRC_ALPHA                       0x0302
#define GL_ONE_MINUS_SRC_ALPHA             0x0303
#define GL_NUM_EXTENSIONS                  0x821D
#define GL_EXTENSIONS                      0x1F03
#define WGL_CONTEXT_MAJOR_VERSION_ARB      0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB      0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB       0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB   0x00000001
#define WGL_CONTEXT_FLAGS_ARB              0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB          0x00000001

#define _GFX_GLAPI \
    _X1(GLenum,         glGetError,                void) \
    _X1(const GLubyte*, glGetString,               GLenum) \
    _X1(void,           glClear,                   GLbitfield) \
    _X4(void,           glClearColor,              GLclampf, GLclampf, GLclampf, GLclampf) \
    _X4(void,           glViewport,                GLint, GLint, GLsizei, GLsizei) \
    _X2(void,           glGenBuffers,              GLsizei, GLuint*) \
    _X2(void,           glBindBuffer,              GLenum, GLuint) \
    _X2(void,           glDeleteBuffers,           GLsizei, const GLuint*) \
    _X4(void,           glBufferStorage,           GLenum, GLsizeiptr, const void*, GLbitfield) \
    _X4(void*,          glMapBufferRange,          GLenum, GLintptr, GLsizeiptr, GLbitfield) \
    _X1(GLuint,         glCreateShader,            GLenum) \
    _X1(GLuint,         glCreateProgram,           void) \
    _X4(void,           glShaderSource,            GLuint, GLsizei, const GLchar**, const GLint*) \
    _X1(void,           glCompileShader,           GLuint) \
    _X3(void,           glGetShaderiv,             GLuint, GLenum, GLint*) \
    _X4(void,           glGetShaderInfoLog,        GLuint, GLsizei, GLsizei*, GLchar*) \
    _X2(void,           glAttachShader,            GLuint, GLuint) \
    _X1(void,           glLinkProgram,             GLuint) \
    _X3(void,           glGetProgramiv,            GLuint, GLenum, GLint*) \
    _X4(void,           glGetProgramInfoLog,       GLuint, GLsizei, GLsizei*, GLchar*) \
    _X1(void,           glUseProgram,              GLuint) \
    _X1(void,           glDeleteShader,            GLuint) \
    _X1(void,           glDeleteProgram,           GLuint) \
    _X2(void,           glGenVertexArrays,         GLsizei, GLuint*) \
    _X1(void,           glBindVertexArray,         GLuint) \
    _X2(void,           glDeleteVertexArrays,      GLsizei, GLuint*) \
    _X6(void,           glVertexAttribPointer,     GLuint, GLint, GLenum, GLboolean, GLsizei, const void*) \
    _X1(void,           glEnableVertexAttribArray, GLuint) \
    _X3(void,           glDrawArrays,              GLenum, GLint, GLsizei) \
    _X2(GLint,          glGetUniformLocation,      GLuint, const GLchar*) \
    _X3(void,           glUniform2f,               GLint, GLfloat, GLfloat) \
    _X2(void,           glGenTextures,             GLsizei, GLuint*) \
    _X2(void,           glDeleteTextures,          GLsizei, GLuint*) \
    _X2(void,           glBindTexture,             GLenum, GLuint) \
    _X9(void,           glTexImage2D,              GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*) \
    _X2(void,           glGetIntegerv,             GLenum, GLint*) \
    _X1(void,           glActiveTexture,           GLenum) \
    _X3(void,           glTexParameteri,           GLenum, GLenum, GLint) \
    _X1(void,           glEnable,                  GLenum) \
    _X2(void,           glBlendFunc,               GLenum, GLenum) \
    _X5(void,           glGetTexImage,             GLenum, GLint, GLenum, GLenum, void*) \
    _X2(const GLubyte*, glGetStringi,              GLenum, GLuint) \
    _X9(void,           glTexSubImage2D,           GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void*)

static const char *_s_gfx_vs_source =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aXY;\n"
    "layout (location = 1) in vec2 aUV;\n"
    "out vec2 vUV;\n"
    "uniform vec2 uSize;\n"
    "void main() {\n"
    "    gl_Position = vec4(aXY.x / uSize.x * 2 - 1, (1 - aXY.y / uSize.y) * 2 - 1, 0, 1.0);\n"
    "    vUV = aUV;\n"
    "}";

static const char *_s_gfx_fs_source =
    "#version 330 core\n"
    "in vec2 vUV;"
    "out vec4 oCol;\n"
    "uniform sampler2D uTex;\n"
    "void main() {\n"
    "    oCol = texture(uTex, vUV);\n"
    "}";

static struct {
    HDC hdc;
    PIXELFORMATDESCRIPTOR pfd;
    HGLRC hgl;
    HMODULE opengl32dll;
    vec2_t size;
    GLuint vbo;
    GLuint vao;
    gfx_vertex_t *vmem;
    uint32_t vcnt;
    GLuint vs;
    GLuint fs;
    GLuint sp;
    GLint uSize;
    GLuint tex;
} _s_gfx;

static struct {
    #define _X1(_rtype, _name, _a1) _rtype (*_name)(_a1);
    #define _X2(_rtype, _name, _a1, _a2) _rtype (*_name)(_a1, _a2);
    #define _X3(_rtype, _name, _a1, _a2, _a3) _rtype (*_name)(_a1, _a2, _a3);
    #define _X4(_rtype, _name, _a1, _a2, _a3, _a4) _rtype (*_name)(_a1, _a2, _a3, _a4);
    #define _X5(_rtype, _name, _a1, _a2, _a3, _a4, _a5) _rtype (*_name)(_a1, _a2, _a3, _a4, _a5);
    #define _X6(_rtype, _name, _a1, _a2, _a3, _a4, _a5, _a6) _rtype (*_name)(_a1, _a2, _a3, _a4, _a5, _a6);
    #define _X9(_rtype, _name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9) _rtype (*_name)(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
    _GFX_GLAPI
    #undef _X1
    #undef _X2
    #undef _X3
    #undef _X4
    #undef _X5
    #undef _X6
    #undef _X9
} _s_gfx_glapi;

static GLuint _gfx_compile_shader(GLenum type, LPCSTR name, const GLchar *src);
static GLuint _gfx_create_program(GLuint vs, GLuint fs);

void gfx_init(HWND hwnd) {
    int pf;
    HGLRC tmpgl;
    wglCreateContextAttribsARB_t wglCreateContextAttribsARB;
    int32_t attribs[9];

    _s_gfx.hdc = GetDC(hwnd);
    ASSERT(_s_gfx.hdc);

    _s_gfx.pfd.nSize = sizeof(_s_gfx.pfd);
    _s_gfx.pfd.nVersion = 1;
    _s_gfx.pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    _s_gfx.pfd.iPixelType = PFD_TYPE_RGBA;
    _s_gfx.pfd.iLayerType = PFD_MAIN_PLANE;
    pf = ChoosePixelFormat(_s_gfx.hdc, &_s_gfx.pfd);
    DescribePixelFormat(_s_gfx.hdc, pf, sizeof(_s_gfx.pfd), &_s_gfx.pfd);
    SetPixelFormat(_s_gfx.hdc, pf, &_s_gfx.pfd);
    tmpgl = wglCreateContext(_s_gfx.hdc);
    ASSERT(tmpgl);
    wglMakeCurrent(_s_gfx.hdc, tmpgl);

    wglCreateContextAttribsARB = (wglCreateContextAttribsARB_t)wglGetProcAddress("wglCreateContextAttribsARB");
    wglMakeCurrent(0, 0);
    wglDeleteContext(tmpgl);
    attribs[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
    attribs[1] = 4;
    attribs[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
    attribs[3] = 4;
    attribs[4] = WGL_CONTEXT_PROFILE_MASK_ARB;
    attribs[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
    attribs[6] = WGL_CONTEXT_FLAGS_ARB;
    attribs[7] = WGL_CONTEXT_DEBUG_BIT_ARB;
    attribs[8] = 0;
    _s_gfx.hgl = wglCreateContextAttribsARB(_s_gfx.hdc, 0, attribs);
    ASSERT(_s_gfx.hgl);
    wglMakeCurrent(_s_gfx.hdc, _s_gfx.hgl);

    _s_gfx.opengl32dll = LoadLibraryW(L"opengl32.dll");
    ASSERT(_s_gfx.opengl32dll);

    #define _X1(_rtype, _name, _a1) do { _s_gfx_glapi._name = (_rtype(*)(_a1))wglGetProcAddress(#_name); if (!_s_gfx_glapi._name) { _s_gfx_glapi._name = (_rtype(*)(_a1))GetProcAddress(_s_gfx.opengl32dll, #_name); } ASSERT(_s_gfx_glapi._name); } while (0);
    #define _X2(_rtype, _name, _a1, _a2) do { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2))wglGetProcAddress(#_name); if (!_s_gfx_glapi._name) { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2))GetProcAddress(_s_gfx.opengl32dll, #_name); } ASSERT(_s_gfx_glapi._name); } while (0);
    #define _X3(_rtype, _name, _a1, _a2, _a3) do { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2, _a3))wglGetProcAddress(#_name); if (!_s_gfx_glapi._name) { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2, _a3))GetProcAddress(_s_gfx.opengl32dll, #_name); } ASSERT(_s_gfx_glapi._name); } while (0);
    #define _X4(_rtype, _name, _a1, _a2, _a3, _a4) do { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2, _a3, _a4))wglGetProcAddress(#_name); if (!_s_gfx_glapi._name) { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2, _a3, _a4))GetProcAddress(_s_gfx.opengl32dll, #_name); } ASSERT(_s_gfx_glapi._name); } while (0);
    #define _X5(_rtype, _name, _a1, _a2, _a3, _a4, _a5) do { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2, _a3, _a4, _a5))wglGetProcAddress(#_name); if (!_s_gfx_glapi._name) { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2, _a3, _a4, _a5))GetProcAddress(_s_gfx.opengl32dll, #_name); } ASSERT(_s_gfx_glapi._name); } while (0);
    #define _X6(_rtype, _name, _a1, _a2, _a3, _a4, _a5, _a6) do { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2, _a3, _a4, _a5, _a6))wglGetProcAddress(#_name); if (!_s_gfx_glapi._name) { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2, _a3, _a4, _a5, _a6))GetProcAddress(_s_gfx.opengl32dll, #_name); } ASSERT(_s_gfx_glapi._name); } while (0);
    #define _X9(_rtype, _name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9) do { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9))wglGetProcAddress(#_name); if (!_s_gfx_glapi._name) { _s_gfx_glapi._name = (_rtype(*)(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9))GetProcAddress(_s_gfx.opengl32dll, #_name); } ASSERT(_s_gfx_glapi._name); } while (0);
    _GFX_GLAPI
    #undef _X1
    #undef _X2
    #undef _X3
    #undef _X4
    #undef _X5
    #undef _X6
    #undef _X9

    _s_gfx_glapi.glGenBuffers(1, &_s_gfx.vbo);
    _s_gfx_glapi.glBindBuffer(GL_ARRAY_BUFFER, _s_gfx.vbo);
    _s_gfx_glapi.glBufferStorage(
        GL_ARRAY_BUFFER,
        sizeof(gfx_vertex_t) * _GFX_MAX_VERTICES,
        0,
        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT
    );
    ASSERT(_s_gfx_glapi.glGetError() == GL_NO_ERROR);

    _s_gfx_glapi.glGenVertexArrays(1, &_s_gfx.vao);
    _s_gfx_glapi.glBindVertexArray(_s_gfx.vao);
    ASSERT(_s_gfx_glapi.glGetError() == GL_NO_ERROR);
    _s_gfx_glapi.glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 4, NULL);
    _s_gfx_glapi.glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    _s_gfx_glapi.glEnableVertexAttribArray(0);
    _s_gfx_glapi.glEnableVertexAttribArray(1);

    _s_gfx.vmem = _s_gfx_glapi.glMapBufferRange(
        GL_ARRAY_BUFFER,
        0,
        sizeof(gfx_vertex_t) * _GFX_MAX_VERTICES,
        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT
    );
    ASSERT(_s_gfx.vmem);

    _s_gfx.vs = _gfx_compile_shader(GL_VERTEX_SHADER, "vertex", _s_gfx_vs_source);
    _s_gfx.fs = _gfx_compile_shader(GL_FRAGMENT_SHADER, "fragment", _s_gfx_fs_source);
    _s_gfx.sp = _gfx_create_program(_s_gfx.vs, _s_gfx.fs);
    ASSERT((_s_gfx.uSize = _s_gfx_glapi.glGetUniformLocation(_s_gfx.sp, "uSize")) != -1);
    _s_gfx_glapi.glUseProgram(_s_gfx.sp);

    ASSERT(_s_gfx_glapi.glGetError() == GL_NO_ERROR);

    _s_gfx_glapi.glGenTextures(1, &_s_gfx.tex);
    _s_gfx_glapi.glBindTexture(GL_TEXTURE_2D, _s_gfx.tex);
    _s_gfx_glapi.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    _s_gfx_glapi.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _s_gfx_glapi.glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        GFX_TEXTURE_SIZE,
        GFX_TEXTURE_SIZE,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        0
    );
    ASSERT(_s_gfx_glapi.glGetError() == GL_NO_ERROR);
    _s_gfx_glapi.glActiveTexture(GL_TEXTURE0);

    _s_gfx_glapi.glEnable(GL_BLEND);
    _s_gfx_glapi.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    _s_gfx_glapi.glClearColor(1, 1, 1, 0);
    gfx_set_size(1, 1);

    debug_log(
        DEBUG_INFO,
        L"using OpenGL %hs on %hs",
        _s_gfx_glapi.glGetString(GL_VERSION),
        _s_gfx_glapi.glGetString(GL_RENDERER)
    );
}

void gfx_cleanup(void) {
    _s_gfx_glapi.glDeleteTextures(1, &_s_gfx.tex);
    _s_gfx_glapi.glDeleteShader(_s_gfx.vs);
    _s_gfx_glapi.glDeleteShader(_s_gfx.fs);
    _s_gfx_glapi.glDeleteProgram(_s_gfx.sp);
    _s_gfx_glapi.glDeleteBuffers(1, &_s_gfx.vbo);
    _s_gfx_glapi.glDeleteVertexArrays(1, &_s_gfx.vao);
    wglMakeCurrent(0, 0);
    wglDeleteContext(_s_gfx.hgl);
    FreeLibrary(_s_gfx.opengl32dll);
}

void gfx_set_size(int w, int h) {
    _s_gfx.size.x = (float)w;
    _s_gfx.size.y = (float)h;
    _s_gfx_glapi.glViewport(0, 0, w, h);
    _s_gfx_glapi.glUniform2f(_s_gfx.uSize, _s_gfx.size.x, _s_gfx.size.y);
}

void gfx_clear(void) {
    _s_gfx.vcnt = 0;
    _s_gfx_glapi.glClear(GL_COLOR_BUFFER_BIT);
}

void gfx_draw(const gfx_vertex_t *vertices, uint32_t count) {
    uint32_t i;
    ASSERT(_s_gfx.vcnt + count < _GFX_MAX_VERTICES);
    for (i = 0; i < count; i++) {
        _s_gfx.vmem[_s_gfx.vcnt + i] = vertices[i];
    }
    _s_gfx.vcnt += count;
}

void gfx_present(void) {
    _s_gfx_glapi.glUseProgram(_s_gfx.sp);
    _s_gfx_glapi.glBindBuffer(GL_ARRAY_BUFFER, _s_gfx.vbo);
    _s_gfx_glapi.glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(gfx_vertex_t), NULL);
    _s_gfx_glapi.glEnableVertexAttribArray(0);
    _s_gfx_glapi.glBindVertexArray(_s_gfx.vao);
    _s_gfx_glapi.glDrawArrays(GL_TRIANGLES, 0, _s_gfx.vcnt);
    SwapBuffers(_s_gfx.hdc);
}

void gfx_update_texture(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const  uint32_t *pixels) {
    _s_gfx_glapi.glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    ASSERT(_s_gfx_glapi.glGetError() == GL_NO_ERROR);
}

void gfx_dump_texture_tga(LPCWSTR path) {
    void *p;
    p = VirtualAlloc(0, GFX_TEXTURE_SIZE * GFX_TEXTURE_SIZE * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    ASSERT(p);
    _s_gfx_glapi.glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, p);
    ASSERT(_s_gfx_glapi.glGetError() == GL_NO_ERROR);
    tga_save(path, GFX_TEXTURE_SIZE, GFX_TEXTURE_SIZE, p);
    VirtualFree(p, GFX_TEXTURE_SIZE * GFX_TEXTURE_SIZE * 4, MEM_DECOMMIT | MEM_RELEASE);
}

GLuint _gfx_compile_shader(GLenum type, LPCSTR name, const GLchar *src) {
    GLuint s;
    int ok;
    char log[1024];
    ASSERT(s = _s_gfx_glapi.glCreateShader(type));
    _s_gfx_glapi.glShaderSource(s, 1, &src, 0);
    _s_gfx_glapi.glCompileShader(s);
    _s_gfx_glapi.glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        _s_gfx_glapi.glGetShaderInfoLog(s, sizeof(log), 0, log);
        debug_log(DEBUG_ERROR, L"failed to compile %hs shader:\n%hs%hs", name, log, src);
        debug_crash(L"Failed to compile %hs shader.", name);
    }
    return s;
}

GLuint _gfx_create_program(GLuint vs, GLuint fs) {
    GLuint sp;
    int ok;
    char log[1024];
    ASSERT(sp = _s_gfx_glapi.glCreateProgram());
    _s_gfx_glapi.glAttachShader(sp, vs);
    _s_gfx_glapi.glAttachShader(sp, fs);
    _s_gfx_glapi.glLinkProgram(sp);
    _s_gfx_glapi.glGetProgramiv(sp, GL_LINK_STATUS, &ok);
    if (!ok) {
        _s_gfx_glapi.glGetProgramInfoLog(sp, sizeof(log), 0, log);
        debug_log(DEBUG_ERROR, L"failed to link shader program:\n%hs", log);
        debug_crash(L"Failed to link shader program.");
    }
    return sp;
}
