#ifndef HOUYI_SHADERS
#define HOUYI_SHADERS

#define HOUYI_SHADER_COUNT 1

const char* vshaders[1] = {
"attribute vec4 aPosition;\n"
"attribute vec2 aTexcoord;\n"
"\n"
"uniform mat4 matWorld;\n"
"uniform mat4 matEye;\n"
"uniform mat4 matProjection;\n"
"\n"
"varying vec2 vTexcoord;\n"
"\n"
"void main() {\n"
"    vTexcoord.st = aTexcoord.st;\n"
"    vec4 pos = vec4(aPosition.xy, 0.0, 1.0);\n"
"    gl_Position = matProjection * matEye * matWorld * pos;\n"
"}\n"
};

const char* pshaders[1] = {
"#extension GL_OES_EGL_image_external : require\n"
"\n"
"precision mediump float;\n"
"\n"
"uniform samplerExternalOES tex;\n"
"uniform int uUseOR;\n"
"uniform mat4 matTex;\n"
"\n"
"varying vec2 vTexcoord;\n"
"\n"
"vec2 HmdWarp(vec2 in01)\n"
"{\n"
"	vec4 HmdWarpParam = vec4(2.5, 0.5, 0.25, 0.05);\n"
"    vec2 lensCenter = vec2(0.5, 0.5);\n"
"    vec2 theta = (in01 - lensCenter) * 2.0;\n"
"    float rSq = theta.x * theta.x + theta.y * theta.y;\n"
"    vec2 rvector = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq + HmdWarpParam.z * rSq * rSq + HmdWarpParam.w * rSq * rSq * rSq);\n"
"    return lensCenter + 0.15 * rvector;\n"
"}\n"
"\n"
"void main()\n"
"{\n"
"   vec4 tcTrans = matTex * vec4(vTexcoord, 1.0, 1.0);\n"
"	vec2 tc = vec2(0.0, 0.0);"
"   if (uUseOR == 1) {\n"
"       tc = HmdWarp(tcTrans.xy);\n"
"   } else {\n"
"       tc = tcTrans.xy;\n"
"   }\n"
"	if (tc.s < 0.0 || tc.s > 1.0 || tc.t < 0.0 || tc.t > 1.0) {\n"
"		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
"	} else {\n"
"		gl_FragColor = texture2D(tex, tc); \n"
"	}\n"
"}\n"
};

#endif

