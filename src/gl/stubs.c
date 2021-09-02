#include "gl4es.h"
#include "attributes.h"
#include "../glx/hardext.h"
#include "debug.h"
#include "glstate.h"
#include "init.h"
#include "loader.h"
#include "logs.h"

#define STUB errorShim(GL_INVALID_VALUE);

NonAliasExportDecl(void,glClampColorARB,(GLenum target, GLenum clamp)){STUB}

void APIENTRY_GL4ES gl4es_glGetMinmaxParameteriv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);
}
AliasExport(void,glGetMinmaxParameteriv,,(GLenum target, GLenum pname, GLint *params));

void APIENTRY_GL4ES gl4es_glCopyColorTable(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);
}
AliasExport(void,glCopyColorTable,,(GLenum target, GLenum pname, GLint *params));
void APIENTRY_GL4ES gl4es_glGetColorTableParameterfv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);
}

AliasExport(void,glGetColorTableParameterfv,,(GLenum target, GLenum pname, GLint *params));
// glCopyConvolutionFilter2D

void APIENTRY_GL4ES gl4es_glCopyConvolutionFilter2D(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);
}

AliasExport(void,glCopyConvolutionFilter2D,,(GLenum target, GLenum pname, GLint *params));
// glGetColorTableParameteriv

void APIENTRY_GL4ES gl4es_glGetColorTableParameteriv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);
}
AliasExport(void,glGetColorTableParameteriv,,(GLenum target, GLenum pname, GLint *params));
// glConvolutionFilter1D
void APIENTRY_GL4ES gl4es_glConvolutionFilter1D(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);
}
AliasExport(void,glConvolutionFilter1D,,(GLenum target, GLenum pname, GLint *params));
// glMinmax

void APIENTRY_GL4ES gl4es_glMinmax(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glMinmax,,(GLenum target, GLenum pname, GLint *params));

// glConvolutionParameterf
void APIENTRY_GL4ES gl4es_glConvolutionParameterf(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glConvolutionParameterf,,(GLenum target, GLenum pname, GLint *params));

// glGetMinmaxParameterfv
void APIENTRY_GL4ES gl4es_glGetMinmaxParameterfv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glGetMinmaxParameterfv,,(GLenum target, GLenum pname, GLint *params));

// glGetColorTable

void APIENTRY_GL4ES gl4es_glGetColorTable(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glGetColorTable,,(GLenum target, GLenum pname, GLint *params));

// glGetMinmax

void APIENTRY_GL4ES gl4es_glGetMinmax(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glGetMinmax,,(GLenum target, GLenum pname, GLint *params));

// glGetHistogram


void APIENTRY_GL4ES gl4es_glGetHistogram(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glGetHistogram,,(GLenum target, GLenum pname, GLint *params));

// glResetMinmax


void APIENTRY_GL4ES gl4es_glResetMinmax(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glResetMinmax,,(GLenum target, GLenum pname, GLint *params));

// glColorTableParameterfv


void APIENTRY_GL4ES gl4es_glColorTableParameterfv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glColorTableParameterfv,,(GLenum target, GLenum pname, GLint *params));

// glConvolutionParameteriv

void APIENTRY_GL4ES gl4es_glConvolutionParameteriv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glConvolutionParameteriv,,(GLenum target, GLenum pname, GLint *params));

// glGetHistogramParameteriv

void APIENTRY_GL4ES gl4es_glGetHistogramParameteriv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glGetHistogramParameteriv,,(GLenum target, GLenum pname, GLint *params));

// glGetHistogramParameterfv


void APIENTRY_GL4ES gl4es_glGetHistogramParameterfv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glGetHistogramParameterfv,,(GLenum target, GLenum pname, GLint *params));

// glHistogram

void APIENTRY_GL4ES gl4es_glHistogram(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glHistogram,,(GLenum target, GLenum pname, GLint *params));

// glColorTableParameteriv

void APIENTRY_GL4ES gl4es_glColorTableParameteriv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glColorTableParameteriv,,(GLenum target, GLenum pname, GLint *params));

// glGetConvolutionParameterfv

void APIENTRY_GL4ES gl4es_glGetConvolutionParameterfv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glGetConvolutionParameterfv,,(GLenum target, GLenum pname, GLint *params));

// glGetConvolutionParameteriv

void APIENTRY_GL4ES gl4es_glGetConvolutionParameteriv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glGetConvolutionParameteriv,,(GLenum target, GLenum pname, GLint *params));

// glSeparableFilter2D

void APIENTRY_GL4ES gl4es_glSeparableFilter2D(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glSeparableFilter2D,,(GLenum target, GLenum pname, GLint *params));

// glCopyConvolutionFilter1D

void APIENTRY_GL4ES gl4es_glCopyConvolutionFilter1D(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glCopyConvolutionFilter1D,,(GLenum target, GLenum pname, GLint *params));

// glColorSubTable

void APIENTRY_GL4ES gl4es_glColorSubTable(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glColorSubTable,,(GLenum target, GLenum pname, GLint *params));

// glGetConvolutionFilter

void APIENTRY_GL4ES gl4es_glGetConvolutionFilter(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glGetConvolutionFilter,,(GLenum target, GLenum pname, GLint *params));

// glResetHistogram
void APIENTRY_GL4ES gl4es_glResetHistogram(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glResetHistogram,,(GLenum target, GLenum pname, GLint *params));

// glConvolutionFilter2D

void APIENTRY_GL4ES gl4es_glConvolutionFilter2D(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glConvolutionFilter2D,,(GLenum target, GLenum pname, GLint *params));

// glConvolutionParameteri

void APIENTRY_GL4ES gl4es_glConvolutionParameteri(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glConvolutionParameteri,,(GLenum target, GLenum pname, GLint *params));

// glGetSeparableFilter

void APIENTRY_GL4ES gl4es_glGetSeparableFilter(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glGetSeparableFilter,,(GLenum target, GLenum pname, GLint *params));

// glCopyColorSubTable

void APIENTRY_GL4ES gl4es_glCopyColorSubTable(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glCopyColorSubTable,,(GLenum target, GLenum pname, GLint *params));

// glConvolutionParameterfv

void APIENTRY_GL4ES gl4es_glConvolutionParameterfv(GLenum target, GLenum pname, GLint *params) {
  errorShim(GL_INVALID_ENUM);

}
AliasExport(void,glConvolutionParameterfv,,(GLenum target, GLenum pname, GLint *params));
