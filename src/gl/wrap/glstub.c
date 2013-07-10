#include "stub.h"

#define STUB(def)\
def {\
    char *debug = getenv("LIBGL_DEBUG");\
    if (debug && strcmp(debug, "1") == 0)\
        printf("stub: %s;\n", #def);\
}

STUB(void glFogCoordd(GLdouble coord))
STUB(void glFogCoordf(GLfloat coord))
STUB(void glFogCoorddv(const GLdouble *coord))
STUB(void glFogCoordfv(const GLfloat *coord))

#ifdef USE_ES2
STUB(void glFogf(GLenum pname, GLfloat param));
STUB(void glFogfv(GLenum pname, const GLfloat *params));
STUB(void glLoadIdentity());
STUB(void glLoadMatrixf(const GLfloat *m));
STUB(void glMultMatrixf(const GLfloat *m));
STUB(void glMatrixMode(GLenum mode));
STUB(void glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearVal, GLfloat farVal));
STUB(void glPopMatrix());
STUB(void glPushMatrix());
STUB(void glScalef(GLfloat x, GLfloat y, GLfloat z));
STUB(void glTranslatef(GLfloat x, GLfloat y, GLfloat z));
STUB(void glFrustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far));
STUB(void glGetClipPlanef(GLenum plane, GLfloat *equation));
STUB(void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz));
STUB(void glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat r, GLfloat q, GLfloat t));
STUB(void glEnableClientState(GLenum state));
STUB(void glDisableClientState(GLenum state));
#endif

// STUB(void glMultiTexCoord());
// STUB(void glVertexAttrib());
// STUB(void glEvalCoord());
// STUB(void glEvalPoint());
STUB(GLint glRenderMode(GLenum mode))
STUB(void glArrayElement(GLint i))
#ifndef USE_ES2
STUB(void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha))
#endif
STUB(void glBlendEquationSeparatei(GLuint buf, GLenum modeRGB, GLenum modeAlpha))
#ifndef USE_ES2
STUB(void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha))
#endif
STUB(void glBlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha))
STUB(void glColorMaterial(GLenum face, GLenum mode))
STUB(void glCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type))
STUB(void glDrawBuffer(GLenum mode))
STUB(void glEdgeFlag(GLboolean flag))
STUB(void glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid * img))
STUB(void glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params))
STUB(void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params))
STUB(void glIndexf(GLfloat c))
STUB(void glInitNames())
STUB(void glLoadName(GLuint name))
STUB(void glPixelTransferf(GLenum pname, GLfloat param))
STUB(void glPixelTransferi(GLenum pname, GLint param))
STUB(void glPixelZoom(GLfloat xfactor, GLfloat yfactor))
STUB(void glPolygonMode(GLenum face, GLenum mode))
STUB(void glPolygonStipple(const GLubyte *mask))
STUB(void glPopName())
STUB(void glPushName())
STUB(void glReadBuffer(GLenum mode))
STUB(void glSecondaryColor3f(GLfloat r, GLfloat g, GLfloat b))
STUB(void glTexImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *data))

// glSelectBuffer: http://www.lighthouse3d.com/opengl/picking/index.php?color1
STUB(void glSelectBuffer(GLsizei size, GLuint *buffer))

// mesh functions
STUB(void glEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2));
STUB(void glMapGrid2d(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2));
STUB(void glMap2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points));

#undef STUB