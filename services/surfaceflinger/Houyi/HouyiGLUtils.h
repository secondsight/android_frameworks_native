//
//  HouyiGLUtils.h
//  Houyi
//
//  Created by Lance Mao on 14/2/13.
//  Copyright (c) 2013 Lance Mao. All rights reserved.
//

#ifndef Houyi_HouyiGLUtils_h
#define Houyi_HouyiGLUtils_h

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cutils/log.h>

namespace Houyi
{
    class GLUtils
    {
    public:
        static int loadShader(const char* pVertexSource, const char* pFragmentSource);
        static GLuint loadShader(GLenum shaderType, const char* pSource);
    };
}

#endif
