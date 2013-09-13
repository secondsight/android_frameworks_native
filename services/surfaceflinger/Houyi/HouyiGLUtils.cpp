#include "HouyiGLUtils.h"

namespace Houyi
{
    int GLUtils::loadShader(const char* pVertexSource, const char* pFragmentSource)
    {
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
        if (vertexShader)
        {
            ALOGI ("Creating program. vertexShader created\n");
        }
        else
        {
            ALOGE ("create vertex shader failed\n");
            return 0;
        }

        GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
        if (pixelShader)
        {
            ALOGI ("Creating program. fragmentShader created\n");
        }
        else
        {
            ALOGE ("create fragment shader failed\n");
            return 0;
        }

        GLuint program = glCreateProgram();
        if (program)
        {
            glAttachShader(program, vertexShader);
            glAttachShader(program, pixelShader);
            glLinkProgram(program);
            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE)
            {
                GLint bufLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
                if (bufLength)
                {
                    char* buf = (char*) malloc(bufLength);
                    if (buf)
                    {
                        glGetProgramInfoLog(program, bufLength, NULL, buf);
                        ALOGE("Could not link program:\n%s\n", buf);
                        free(buf);
                    }
                }
                glDeleteProgram(program);
                program = 0;
            }
        } ALOGI ("Program created successfully\n");
        return program;
    }

    GLuint GLUtils::loadShader(GLenum shaderType, const char* pSource)
    {
        GLuint shader = glCreateShader(shaderType);
        if (shader)
        {
            glShaderSource(shader, 1, &pSource, NULL);
            glCompileShader(shader);
            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (!compiled)
            {
                GLint infoLen = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen)
                {
                    char* buf = (char*) malloc(infoLen);
                    if (buf)
                    {
                        glGetShaderInfoLog(shader, infoLen, NULL, buf);
                        ALOGE("Could not compile shader %d:\n%s\n",
                                shaderType, buf);
                        free(buf);
                    }
                    glDeleteShader(shader);
                    shader = 0;
                }
            }
        }
        return shader;
    }
}
