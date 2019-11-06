/*
 * Copyright (c) 2016 Bilibili
 * copyright (c) 2016 Zhang Rui <bbcallen@gmail.com>
 *
 * This file is part of ijkPlayer.
 *
 * ijkPlayer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * ijkPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ijkPlayer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "internal.h"
static void IJK_GLES2_Draw_Custom_Graph(IJK_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay);
static GLfloat *IJK_GLES2_Draw_FrameVertexs(IJK_GLES2_Renderer *renderer,float whratio,float centerX,float centerY);
static void IJK_GLES2_printProgramInfo(GLuint program)
{
    if (!program)
        return;

    GLint info_len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
    if (!info_len) {
        ALOGE("[GLES2][Program] empty info\n");
        return;
    }

    char    buf_stack[32];
    char   *buf_heap = NULL;
    char   *buf      = buf_stack;
    GLsizei buf_len  = sizeof(buf_stack) - 1;
    if (info_len > sizeof(buf_stack)) {
        buf_heap = (char*) malloc(info_len + 1);
        if (buf_heap) {
            buf     = buf_heap;
            buf_len = info_len;
        }
    }

    glGetProgramInfoLog(program, buf_len, NULL, buf);
    ALOGE("[GLES2][Program] error %s\n", buf);

    if (buf_heap)
        free(buf_heap);
}

static void IJK_GLES2_Display_reset(IJK_GLES2_Renderer *renderer)
{
	if (!renderer || !renderer->display_program)
        return;
	if (renderer->display_vertex_shader)
        glDeleteShader(renderer->display_vertex_shader);
    if (renderer->display_fragment_shader)
        glDeleteShader(renderer->display_fragment_shader);
    if (renderer->display_program)
        glDeleteProgram(renderer->display_program);

    renderer->display_vertex_shader   = 0;
    renderer->display_fragment_shader = 0;
    renderer->display_program         = 0;
}

void IJK_GLES2_Renderer_reset(IJK_GLES2_Renderer *renderer)
{
    if (!renderer)
        return;

    if (renderer->vertex_shader)
        glDeleteShader(renderer->vertex_shader);
    if (renderer->fragment_shader)
        glDeleteShader(renderer->fragment_shader);
    if (renderer->program)
        glDeleteProgram(renderer->program);

    renderer->vertex_shader   = 0;
    renderer->fragment_shader = 0;
    renderer->program         = 0;

    for (int i = 0; i < IJK_GLES2_MAX_PLANE; ++i) {
        if (renderer->plane_textures[i]) {
            glDeleteTextures(1, &renderer->plane_textures[i]);
            renderer->plane_textures[i] = 0;
        }
    }
	if (renderer->lut_textures[0]) {
		glDeleteTextures(1, &renderer->lut_textures[0]);
        renderer->lut_textures[0] = 0;
	}
	IJK_GLES2_Display_reset(renderer);
}

void IJK_GLES2_Renderer_free(IJK_GLES2_Renderer *renderer)
{
    if (!renderer)
        return;

    if (renderer->func_destroy)
        renderer->func_destroy(renderer);

#if 0
    if (renderer->vertex_shader)    ALOGW("[GLES2] renderer: vertex_shader not deleted.\n");
    if (renderer->fragment_shader)  ALOGW("[GLES2] renderer: fragment_shader not deleted.\n");
    if (renderer->program)          ALOGW("[GLES2] renderer: program not deleted.\n");

    for (int i = 0; i < IJK_GLES2_MAX_PLANE; ++i) {
        if (renderer->plane_textures[i])
            ALOGW("[GLES2] renderer: plane texture[%d] not deleted.\n", i);
    }
#endif

    free(renderer);
}

void IJK_GLES2_Renderer_freeP(IJK_GLES2_Renderer **renderer)
{
    if (!renderer || !*renderer)
        return;

    IJK_GLES2_Renderer_free(*renderer);
    *renderer = NULL;
}

static void IJK_GLES2_Display_create(IJK_GLES2_Renderer *renderer)
{

    renderer->display_vertex_shader = IJK_GLES2_loadShader(GL_VERTEX_SHADER, IJK_GLES2_getVertexShader_display());
    if (!renderer->display_vertex_shader)
        goto fail;

    renderer->display_fragment_shader = IJK_GLES2_loadShader(GL_FRAGMENT_SHADER, IJK_GLES2_getFragmentShader_display());
    if (!renderer->display_fragment_shader)
        goto fail;

    renderer->display_program = glCreateProgram();                          IJK_GLES2_checkError("glCreateProgram");
    if (!renderer->display_program)
        goto fail;

    glAttachShader(renderer->display_program, renderer->display_vertex_shader);     IJK_GLES2_checkError("glAttachShader(vertex)");
    glAttachShader(renderer->display_program, renderer->display_fragment_shader);   IJK_GLES2_checkError("glAttachShader(fragment)");
    glLinkProgram(renderer->display_program);                               IJK_GLES2_checkError("glLinkProgram");
    GLint link_status = GL_FALSE;
    glGetProgramiv(renderer->display_program, GL_LINK_STATUS, &link_status);
    if (!link_status)
        goto fail;


    renderer->display_position   = glGetAttribLocation(renderer->display_program, "display_position");                IJK_GLES2_checkError_TRACE("glGetAttribLocation(display_position)");
    renderer->display_mvp        = glGetUniformLocation(renderer->display_program, "display_mvp");    IJK_GLES2_checkError_TRACE("glGetUniformLocation(display_mvp)");
    renderer->point_size        = glGetUniformLocation(renderer->display_program, "point_size");    IJK_GLES2_checkError_TRACE("glGetUniformLocation(point_size)");
	renderer->display_color = glGetUniformLocation(renderer->display_program, "display_color");            IJK_GLES2_checkError_TRACE("glGetUniformLocation(display_color)");
    return;
fail:
    if (renderer->display_vertex_shader)
        glDeleteShader(renderer->display_vertex_shader);
    if (renderer->display_fragment_shader)
        glDeleteShader(renderer->display_fragment_shader);
    if (renderer->display_program)
        glDeleteProgram(renderer->display_program);
    return;
}

IJK_GLES2_Renderer *IJK_GLES2_Renderer_create_base(const char *fragment_shader_source)
{
    assert(fragment_shader_source);

    IJK_GLES2_Renderer *renderer = (IJK_GLES2_Renderer *)calloc(1, sizeof(IJK_GLES2_Renderer));
    if (!renderer)
        goto fail;

    renderer->vertex_shader = IJK_GLES2_loadShader(GL_VERTEX_SHADER, IJK_GLES2_getVertexShader_default());
    if (!renderer->vertex_shader)
        goto fail;

    renderer->fragment_shader = IJK_GLES2_loadShader(GL_FRAGMENT_SHADER, fragment_shader_source);
    if (!renderer->fragment_shader)
        goto fail;

    renderer->program = glCreateProgram();                          IJK_GLES2_checkError("glCreateProgram");
    if (!renderer->program)
        goto fail;

    glAttachShader(renderer->program, renderer->vertex_shader);     IJK_GLES2_checkError("glAttachShader(vertex)");
    glAttachShader(renderer->program, renderer->fragment_shader);   IJK_GLES2_checkError("glAttachShader(fragment)");
    glLinkProgram(renderer->program);                               IJK_GLES2_checkError("glLinkProgram");
    GLint link_status = GL_FALSE;
    glGetProgramiv(renderer->program, GL_LINK_STATUS, &link_status);
    if (!link_status)
        goto fail;


    renderer->av4_position   = glGetAttribLocation(renderer->program, "av4_Position");                IJK_GLES2_checkError_TRACE("glGetAttribLocation(av4_Position)");
    renderer->av2_texcoord   = glGetAttribLocation(renderer->program, "av2_Texcoord");                IJK_GLES2_checkError_TRACE("glGetAttribLocation(av2_Texcoord)");
    renderer->um4_mvp        = glGetUniformLocation(renderer->program, "um4_ModelViewProjection");    IJK_GLES2_checkError_TRACE("glGetUniformLocation(um4_ModelViewProjection)");
	IJK_GLES2_Display_create(renderer);
    return renderer;

fail:

    if (renderer && renderer->program)
        IJK_GLES2_printProgramInfo(renderer->program);

    IJK_GLES2_Renderer_free(renderer);
    return NULL;
}


IJK_GLES2_Renderer *IJK_GLES2_Renderer_create(SDL_VoutOverlay *overlay)
{
    if (!overlay)
        return NULL;

    IJK_GLES2_printString("Version", GL_VERSION);
    IJK_GLES2_printString("Vendor", GL_VENDOR);
    IJK_GLES2_printString("Renderer", GL_RENDERER);
    IJK_GLES2_printString("Extensions", GL_EXTENSIONS);

    IJK_GLES2_Renderer *renderer = NULL;
    switch (overlay->format) {
        case SDL_FCC_RV16:      renderer = IJK_GLES2_Renderer_create_rgb565(); break;
        case SDL_FCC_RV24:      renderer = IJK_GLES2_Renderer_create_rgb888(); break;
        case SDL_FCC_RV32:      renderer = IJK_GLES2_Renderer_create_rgbx8888(); break;
#ifdef __APPLE__
        case SDL_FCC_NV12:      renderer = IJK_GLES2_Renderer_create_yuv420sp(); break;
        case SDL_FCC__VTB:      renderer = IJK_GLES2_Renderer_create_yuv420sp_vtb(overlay); break;
#endif
        case SDL_FCC_YV12:      renderer = IJK_GLES2_Renderer_create_yuv420p(); break;
        case SDL_FCC_I420:      renderer = IJK_GLES2_Renderer_create_yuv420p(); break;
        case SDL_FCC_I444P10LE: renderer = IJK_GLES2_Renderer_create_yuv444p10le(); break;
        default:
            ALOGE("[GLES2] unknown format %4s(%d)\n", (char *)&overlay->format, overlay->format);
            return NULL;
    }
	
	memset(&renderer->cur_draw_t,0x00,sizeof(GLES2_Draw_Type));
	renderer->cur_draw_t.drawType = 0xFFFF;
	for (int i = 0;i < 15;i += 3){
		renderer->cur_draw_t.rectVertexs[i] = (i == 3 || i == 6) ? 1.0f : -1.0f;
		renderer->cur_draw_t.rectVertexs[i + 1] = (i == 6 || i == 9) ? -1.0f : 1.0f;
		renderer->cur_draw_t.rectVertexs[i + 2] = 0.0f;
	}
	
    renderer->format = overlay->format;
    return renderer;
}

GLboolean IJK_GLES2_Renderer_isValid(IJK_GLES2_Renderer *renderer)
{
    return renderer && renderer->program ? GL_TRUE : GL_FALSE;
}

GLboolean IJK_GLES2_Renderer_isFormat(IJK_GLES2_Renderer *renderer, int format)
{
    if (!IJK_GLES2_Renderer_isValid(renderer))
        return GL_FALSE;

    return renderer->format == format ? GL_TRUE : GL_FALSE;
}

/*
 * Per-Context routine
 */
GLboolean IJK_GLES2_Renderer_setupGLES()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);       IJK_GLES2_checkError_TRACE("glClearColor");
    glEnable(GL_CULL_FACE);                     IJK_GLES2_checkError_TRACE("glEnable(GL_CULL_FACE)");
    glCullFace(GL_BACK);                        IJK_GLES2_checkError_TRACE("glCullFace");
    glDisable(GL_DEPTH_TEST);

    return GL_TRUE;
}

static void IJK_GLES2_Renderer_Vertices_reset(IJK_GLES2_Renderer *renderer)
{
    renderer->vertices[0] = -1.0f;
    renderer->vertices[1] = -1.0f;
    renderer->vertices[2] =  1.0f;
    renderer->vertices[3] = -1.0f;
    renderer->vertices[4] = -1.0f;
    renderer->vertices[5] =  1.0f;
    renderer->vertices[6] =  1.0f;
    renderer->vertices[7] =  1.0f;
}

static void IJK_GLES2_Renderer_Vertices_apply(IJK_GLES2_Renderer *renderer)
{
    switch (renderer->gravity) {
        case IJK_GLES2_GRAVITY_RESIZE_ASPECT:
            break;
        case IJK_GLES2_GRAVITY_RESIZE_ASPECT_FILL:
            break;
        case IJK_GLES2_GRAVITY_RESIZE:
            IJK_GLES2_Renderer_Vertices_reset(renderer);
            return;
        default:
            ALOGE("[GLES2] unknown gravity %d\n", renderer->gravity);
            IJK_GLES2_Renderer_Vertices_reset(renderer);
            return;
    }

    if (renderer->layer_width <= 0 ||
        renderer->layer_height <= 0 ||
        renderer->frame_width <= 0 ||
        renderer->frame_height <= 0)
    {
        ALOGE("[GLES2] invalid width/height for gravity aspect\n");
        IJK_GLES2_Renderer_Vertices_reset(renderer);
        return;
    }

    float width     = renderer->frame_width;
    float height    = renderer->frame_height;

    if (renderer->frame_sar_num > 0 && renderer->frame_sar_den > 0) {
        width = width * renderer->frame_sar_num / renderer->frame_sar_den;
    }

    const float dW  = (float)renderer->layer_width	/ width;
    const float dH  = (float)renderer->layer_height / height;
    float dd        = 1.0f;
    float nW        = 1.0f;
    float nH        = 1.0f;

    switch (renderer->gravity) {
        case IJK_GLES2_GRAVITY_RESIZE_ASPECT_FILL:  dd = FFMAX(dW, dH); break;
        case IJK_GLES2_GRAVITY_RESIZE_ASPECT:       dd = FFMIN(dW, dH); break;
    }

    nW = (width  * dd / (float)renderer->layer_width);
    nH = (height * dd / (float)renderer->layer_height);

    renderer->vertices[0] = - nW;
    renderer->vertices[1] = - nH;
    renderer->vertices[2] =   nW;
    renderer->vertices[3] = - nH;
    renderer->vertices[4] = - nW;
    renderer->vertices[5] =   nH;
    renderer->vertices[6] =   nW;
    renderer->vertices[7] =   nH;
}

static void IJK_GLES2_Renderer_Vertices_reloadVertex(IJK_GLES2_Renderer *renderer)
{
    glVertexAttribPointer(renderer->av4_position, 2, GL_FLOAT, GL_FALSE, 0, renderer->vertices);    IJK_GLES2_checkError_TRACE("glVertexAttribPointer(av4_position)");
    glEnableVertexAttribArray(renderer->av4_position);                                      IJK_GLES2_checkError_TRACE("glEnableVertexAttribArray(av4_position)");
}

#define IJK_GLES2_GRAVITY_MIN                   (0)
#define IJK_GLES2_GRAVITY_RESIZE                (0) // Stretch to fill layer bounds.
#define IJK_GLES2_GRAVITY_RESIZE_ASPECT         (1) // Preserve aspect ratio; fit within layer bounds.
#define IJK_GLES2_GRAVITY_RESIZE_ASPECT_FILL    (2) // Preserve aspect ratio; fill layer bounds.
#define IJK_GLES2_GRAVITY_MAX                   (2)

GLboolean IJK_GLES2_Renderer_setGravity(IJK_GLES2_Renderer *renderer, int gravity, GLsizei layer_width, GLsizei layer_height)
{
    if (renderer->gravity != gravity && gravity >= IJK_GLES2_GRAVITY_MIN && gravity <= IJK_GLES2_GRAVITY_MAX)
        renderer->vertices_changed = 1;
    else if (renderer->layer_width != layer_width)
        renderer->vertices_changed = 1;
    else if (renderer->layer_height != layer_height)
        renderer->vertices_changed = 1;
    else
        return GL_TRUE;

    renderer->gravity      = gravity;
    renderer->layer_width  = layer_width;
    renderer->layer_height = layer_height;
    return GL_TRUE;
}

static void IJK_GLES2_Renderer_TexCoords_reset(IJK_GLES2_Renderer *renderer)
{
    renderer->texcoords[0] = 0.0f;
    renderer->texcoords[1] = 1.0f;
    renderer->texcoords[2] = 1.0f;
    renderer->texcoords[3] = 1.0f;
    renderer->texcoords[4] = 0.0f;
    renderer->texcoords[5] = 0.0f;
    renderer->texcoords[6] = 1.0f;
    renderer->texcoords[7] = 0.0f;
}

static void IJK_GLES2_Renderer_TexCoords_cropRight(IJK_GLES2_Renderer *renderer, GLfloat cropRight)
{
    ALOGE("IJK_GLES2_Renderer_TexCoords_cropRight\n");
    renderer->texcoords[0] = 0.0f;
    renderer->texcoords[1] = 1.0f;
    renderer->texcoords[2] = 1.0f - cropRight;
    renderer->texcoords[3] = 1.0f;
    renderer->texcoords[4] = 0.0f;
    renderer->texcoords[5] = 0.0f;
    renderer->texcoords[6] = 1.0f - cropRight;
    renderer->texcoords[7] = 0.0f;
}

static void IJK_GLES2_Renderer_TexCoords_reloadVertex(IJK_GLES2_Renderer *renderer)
{
    glVertexAttribPointer(renderer->av2_texcoord, 2, GL_FLOAT, GL_FALSE, 0, renderer->texcoords);   IJK_GLES2_checkError_TRACE("glVertexAttribPointer(av2_texcoord)");
    glEnableVertexAttribArray(renderer->av2_texcoord);                                              IJK_GLES2_checkError_TRACE("glEnableVertexAttribArray(av2_texcoord)");
}

static void IJK_GLES2_Display_use(IJK_GLES2_Renderer *renderer)
{
    if (!renderer)
        return;

	glUseProgram(renderer->display_program);            IJK_GLES2_checkError_TRACE("glUseProgram");
    IJK_GLES_Matrix modelViewProj;
    IJK_GLES2_loadOrtho(&modelViewProj, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(renderer->display_mvp, 1, GL_FALSE, modelViewProj.m);                    IJK_GLES2_checkError_TRACE("glUniformMatrix4fv(display_mvp)");
    return;
}

static GLfloat *IJK_GLES2_Draw_RectVertexs(IJK_GLES2_Renderer *renderer,float centerX,float centerY,float ratio)
{
	static GLfloat vertexs[15] = {0.0f};
	int fw = (int)(renderer->frame_width * renderer->texcoords[2]); //for screen crop
	float y = fw * ratio / 2.0f / (renderer->frame_height * 1.0f);
	if (centerX - ratio <= -1.0f)centerX = -1.0 + ratio;
	if (centerX + ratio >= 1.0f)centerX = 1.0 - ratio;
	if (centerY + y >= 1.0f)centerY = 1.0f - y;
	if (centerY - y <= -1.0f)centerY = -1.0f + y;

	vertexs[0] = centerX - ratio;vertexs[1] = centerY + y;
	vertexs[3] = centerX + ratio;vertexs[4] = centerY + y;
	vertexs[6] = centerX + ratio;vertexs[7] = centerY - y;
	vertexs[9] = centerX - ratio;vertexs[10] = centerY - y;
	vertexs[12] = centerX - ratio;vertexs[13] = centerY + y;
	return vertexs;
}

static void IJK_GLES2_Set_Custom(IJK_GLES2_Renderer *renderer)
{
	int type = (renderer->cur_draw_t.drawType & 0xF) == GLES_FS_TYPE_PSEUDO ? 
		renderer->cur_draw_t.pseudoType : renderer->cur_draw_t.brightLimit;
	float arg1 = 0.0f,arg2 = 0.0f,arg3 = 0.0f,arg4 = 0.0f;
	float plusarg1 = 0.0f,plusarg2 = 0.0f,plusarg3 = 0.0f,plusarg4 = 0.0f;
	float wfactor = 0.0f,hfactor = 0.0f;
	if ((renderer->cur_draw_t.drawType & 0x00F0) == GLES_MARKUP_TYPE_RATIO){
		int fw = renderer->frame_width;
		int fh = renderer->frame_height;
		float whratio = renderer->cur_draw_t.whRatio;
		float orginWHRatio = fw * 1.0f / fh;
		float woffsetRatio = whratio <= orginWHRatio ? (fw - fh * whratio) / fw : 0.0f;
		float hoffsetRatio = whratio <= orginWHRatio ? 0.0f : (fh - fw / whratio) / fh;
		arg1 = woffsetRatio / 2.0f;//left
		arg2 = hoffsetRatio / 2.0f;//top
		arg3 = 1.0f - arg1;//right
		arg4 = 1.0f - arg2;//bottom
		type = GLES_MARKUP_RATIO_TYPE;
	}else if ((renderer->cur_draw_t.drawType & 0x00F0) == GLES_MARKUP_TYPE_WIREFRAME){
		float centerX = (renderer->cur_draw_t.centerX / 100.0f - 0.5f) * 2;
		float centerY = ( 0.5f - renderer->cur_draw_t.centerY / 100.0f) * 2;
		float whratio = renderer->cur_draw_t.whRatio;
		GLfloat *idex = IJK_GLES2_Draw_FrameVertexs(renderer,whratio,centerX,centerY);
		arg1 = (idex[0] + 1.0f) / 2.0f;//left
		arg2 = (1.0f - idex[1]) / 2.0f;//top
		arg3 = (idex[6] + 1.0f) / 2.0f;//right
		arg4 = (1.0f - idex[7]) / 2.0f;//bottom
		type = GLES_MARKUP_RATIO_TYPE;
	}else if ((renderer->cur_draw_t.drawType & 0x00F0) == GLES_MARKUP_TYPE_PARTSCALE){
	#if 0
		arg1 = (-0.5f + renderer->cur_draw_t.centerX / 100.0f) * 2.0f;
		arg2 = (0.5f - renderer->cur_draw_t.centerY / 100.0f) * 2.0f;
		arg3 = renderer->cur_draw_t.partZoomScale;
		arg4 = renderer->cur_draw_t.partZoomRatio;

		plusarg1 = renderer->frame_width * 1.0f / renderer->frame_height;
		plusarg2 = ((arg1 + 1.0f) / 2.0f) / (arg3 == 3.0f ? 1.5f : (arg3 == 4.0f ? 1.325f : 2.0f));
		plusarg3 = ((-arg2 + 1.0f) / 2.0f) / (arg3 == 3.0f ? 1.5f : (arg3 == 4.0f ? 1.325f : 2.0f));
	#else
		float centerX = (renderer->cur_draw_t.centerX / 100.0f - 0.5f) * 2.0f;
		float centerY = (0.5f - renderer->cur_draw_t.centerY / 100.0f) * 2.0f;

		float ratio = renderer->cur_draw_t.partZoomRatio == 0.1f ? 0.3f : (renderer->cur_draw_t.partZoomRatio == 0.2f ? 0.4f : 0.5f);
		GLfloat *idex = IJK_GLES2_Draw_RectVertexs(renderer,centerX,centerY,ratio);
		arg1 = (idex[0] + 1.0f) / 2.0f;
		arg2 = ( 1.0f + idex[3]) / 2.0f;
		arg3 = (1.0f - idex[1]) / 2.0f;
		arg4 = (1.0f - idex[7]) / 2.0f;

		plusarg1 = renderer->cur_draw_t.partZoomScale;
		plusarg2 = ((centerX + 1.0f) / 2.0f) / (plusarg1 == 3.0f ? 1.5f : (plusarg1 == 4.0f ? 1.325f : 2.0f));
		plusarg3 = ((-centerY + 1.0f) / 2.0f) / (plusarg1 == 3.0f ? 1.5f : (plusarg1 == 4.0f ? 1.325f : 2.0f));
	#endif
		type = GLES_MARKUP_PARTSCALE_TYPE;
	}else if ((renderer->cur_draw_t.drawType & 0x00F0) == GLES_MARKUP_TYPE_B_TABLE || 
		(renderer->cur_draw_t.drawType & 0x00F0) == GLES_MARKUP_TYPE_SCOPEBOX){
		float centerX = (renderer->cur_draw_t.centerX / 100.0f - 0.5f) * 2.0f;
		float centerY = (0.5f - renderer->cur_draw_t.centerY / 100.0f) * 2.0f;
		GLfloat *idex = IJK_GLES2_Draw_RectVertexs(renderer,centerX,centerY,0.5f);
		arg1 = (idex[0] + 1.0f) / 2.0f;
		arg2 = ( 1.0f + idex[3]) / 2.0f;
		arg3 = (1.0f - idex[1]) / 2.0f;
		arg4 = (1.0f - idex[7]) / 2.0f;

		plusarg1 = renderer->cur_draw_t.alphabscope / 100.0f;
		type = GLES_IN_ALPHA_TYPE;
	}
	if ((renderer->cur_draw_t.drawType & 0x000F) == GLES_FS_TYPE_ZEBRA_S ||
		(renderer->cur_draw_t.drawType & 0x000F) == GLES_FS_TYPE_AUX_FOCUS){
		plusarg4 = renderer->cur_draw_t.brightLimit / 100.0f;
		float auxflw = renderer->cur_draw_t.auxfocuslinewidth * 1.0f;
		wfactor = auxflw / renderer->frame_width;
		hfactor = auxflw / renderer->frame_height;
	}else if ((renderer->cur_draw_t.drawType & 0x000F) == GLES_FS_TYPE_3DLUT){
		plusarg4 = renderer->cur_draw_t.lutSize * 1.0f;
		wfactor = 1.0f * renderer->frame_width;
		hfactor = 1.0f * renderer->frame_height;
	}
	glUniform2i(renderer->cunstom_cmd_type,renderer->cur_draw_t.drawType & 0xF,type);
	glUniform4f(renderer->cunstom_Params,arg1,arg2,arg3,arg4);
	glUniform4f(renderer->cunstom_Params_plus,plusarg1,plusarg2,plusarg3,plusarg4);
	glUniform2f(renderer->cunstom_factor,wfactor,hfactor);
	
	int argb = renderer->cur_draw_t.argb;
	float alpha = (renderer->cur_draw_t.drawType & 0x00F0) == GLES_MARKUP_TYPE_RATIO || 
		(renderer->cur_draw_t.drawType & 0x00F0) == GLES_MARKUP_TYPE_WIREFRAME ? 
		renderer->cur_draw_t.alphaOutside / 100.0f : 0.0f;
	if ((renderer->cur_draw_t.drawType & 0x000F) == GLES_FS_TYPE_AUX_FOCUS){
		alpha = ((renderer->cur_draw_t.argb >> 24) & 0xFF) / 255.0f;
	}
	glUniform4f(renderer->cunstom_Colors,((argb >> 16) & 0xFF) / 255.0f,
		((argb >> 8) & 0xFF) / 255.0f,(argb & 0xFF) / 255.0f,alpha);
}
/*
 * Per-Renderer routine
 */
GLboolean IJK_GLES2_Renderer_use(IJK_GLES2_Renderer *renderer,bool needReset)
{
    if (!renderer)
        return GL_FALSE;

    assert(renderer->func_use);
    if (!renderer->func_use(renderer))
        return GL_FALSE;

    IJK_GLES_Matrix modelViewProj;
    IJK_GLES2_loadOrtho(&modelViewProj, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(renderer->um4_mvp, 1, GL_FALSE, modelViewProj.m);                    IJK_GLES2_checkError_TRACE("glUniformMatrix4fv(um4_mvp)");

	if (needReset)
    	IJK_GLES2_Renderer_TexCoords_reset(renderer);
    IJK_GLES2_Renderer_TexCoords_reloadVertex(renderer);
	
	if (needReset)
    	IJK_GLES2_Renderer_Vertices_reset(renderer);
    IJK_GLES2_Renderer_Vertices_reloadVertex(renderer);

	IJK_GLES2_Set_Custom(renderer);
    return GL_TRUE;
}

/*
 * Per-Frame routine
 */
GLboolean IJK_GLES2_Renderer_renderOverlay(IJK_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay)
{
    if (!renderer || !renderer->func_uploadTexture)
        return GL_FALSE;
//can't reset Vertices or TexCoords,maybe has changed
	IJK_GLES2_Renderer_use(renderer,false);
    glClear(GL_COLOR_BUFFER_BIT);               IJK_GLES2_checkError_TRACE("glClear");

    GLsizei visible_width  = renderer->frame_width;
    GLsizei visible_height = renderer->frame_height;
    if (overlay) {
        visible_width  = overlay->w;
        visible_height = overlay->h;
        if (renderer->frame_width   != visible_width    ||
            renderer->frame_height  != visible_height   ||
            renderer->frame_sar_num != overlay->sar_num ||
            renderer->frame_sar_den != overlay->sar_den) {

            renderer->frame_width   = visible_width;
            renderer->frame_height  = visible_height;
            renderer->frame_sar_num = overlay->sar_num;
            renderer->frame_sar_den = overlay->sar_den;

            renderer->vertices_changed = 1;
        }

        renderer->last_buffer_width = renderer->func_getBufferWidth(renderer, overlay);

        if (!renderer->func_uploadTexture(renderer, overlay))
            return GL_FALSE;
    } else {
        // NULL overlay means force reload vertice
        renderer->vertices_changed = 1;
    }

    GLsizei buffer_width = renderer->last_buffer_width;
    if (renderer->vertices_changed ||
        (buffer_width > 0 &&
         buffer_width > visible_width &&
         buffer_width != renderer->buffer_width &&
         visible_width != renderer->visible_width)){

        renderer->vertices_changed = 0;

        IJK_GLES2_Renderer_Vertices_apply(renderer);
        IJK_GLES2_Renderer_Vertices_reloadVertex(renderer);

        renderer->buffer_width  = buffer_width;
        renderer->visible_width = visible_width;

        GLsizei padding_pixels     = buffer_width - visible_width;
        GLfloat padding_normalized = ((GLfloat)padding_pixels) / buffer_width;

        IJK_GLES2_Renderer_TexCoords_reset(renderer);
        IJK_GLES2_Renderer_TexCoords_cropRight(renderer, padding_normalized);
        IJK_GLES2_Renderer_TexCoords_reloadVertex(renderer);
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);      IJK_GLES2_checkError_TRACE("glDrawArrays");

	IJK_GLES2_Draw_Custom_Graph(renderer,overlay);
    return GL_TRUE;
}

static GLfloat *IJK_GLES2_Draw_FrameVertexs(IJK_GLES2_Renderer *renderer,float whratio,float centerX,float centerY)
{
	int lineMarkupType = renderer->cur_draw_t.drawType & 0x00F0;
	static GLfloat vertexs[GLES_RECT_POINTS_COORD_NUM] = {0.0f};
	GLfloat *originVertexs = renderer->cur_draw_t.rectVertexs;
	int fw = (int)(renderer->frame_width * renderer->texcoords[2]); //for screen crop
	//int fw = renderer->frame_width;
	int fh = renderer->frame_height;
	if (whratio <= 0 || fw == 0 || fh == 0)return originVertexs;
	
	float orginWHRatio = fw * 1.0f / fh;
	float woffsetRatio = whratio <= orginWHRatio ? (fw - fh * whratio) / fw : 0.0f;
	float hoffsetRatio = whratio <= orginWHRatio ? 0.0f : (fh - fw / whratio) / fh;
	if (lineMarkupType == GLES_MARKUP_TYPE_WIREFRAME){
		float x = -0.5f + woffsetRatio / 2.0f + centerX;
		float y = 0.5f - hoffsetRatio / 2.0f + centerY;
		float xE = x + (1.0f - woffsetRatio);
		float yE = y - (1.0f - hoffsetRatio);
		if (xE > 1.0f){
			xE = 1.0f;
			x = xE - (1.0f - woffsetRatio);
		}
		if (yE < -1.0f){
			yE = -1.0f;
			y = yE + (1.0f - hoffsetRatio);
		}
		x = x < -1.0f ? -1.0f : x;
		y = y > 1.0f ? 1.0f : y;

		float offsetx = 0.0f,offsety = 0.0f;
		if (whratio <= orginWHRatio){
			offsety = renderer->cur_draw_t.wireFrameRatio - 0.5f; 
			offsetx = offsety / orginWHRatio * whratio; 
		}else{
			offsetx = renderer->cur_draw_t.wireFrameRatio - 0.5f; 
			offsety = offsetx * orginWHRatio / whratio; 
		}
		x -= offsetx;
		y += offsety;
		xE += offsetx;
		yE -= offsety;
		
		vertexs[0] = x;vertexs[1] = y;
		vertexs[3] = xE;vertexs[4] = y;
		vertexs[6] = xE;vertexs[7] = yE;
		vertexs[9] = x;vertexs[10] = yE;
		vertexs[12] = x;vertexs[13] = y;
	}else{
		for (int i = 0;i < 15;i += 3){
			vertexs[i] = (i == 3 || i == 6) ? 1.0f - woffsetRatio : -1.0f + woffsetRatio;
			vertexs[i + 1] = (i == 6 || i == 9) ? -1.0f + hoffsetRatio : 1.0f - hoffsetRatio;
			vertexs[i + 2] = 0.0f;
		}
	}
	return vertexs;
}

static GLfloat *IJK_GLES2_Luma_Vertexs(IJK_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay,int type,int loop)
{
	float centerX = (renderer->cur_draw_t.centerX / 100.0f - 0.5f) * 2.0f;
	float centerY = (0.5f - renderer->cur_draw_t.centerY / 100.0f) * 2.0f;
	GLfloat *idex = IJK_GLES2_Draw_RectVertexs(renderer,centerX,centerY,0.5f);
	GLubyte *ybytes = overlay->pixels[0];
	int ySize = renderer->frame_width * renderer->frame_height;
	float yoffset = idex[1] - idex[7];
	int max = 0;
	if (type == GLES_MARKUP_TYPE_B_TABLE){
		int yNumbers[255] = {0};
		for (int i=0;i<ySize;i++){
			yNumbers[ybytes[i]]++;
		}
		for (int i=0;i<255;i++){
			if (yNumbers[i] > max)max = yNumbers[i];
		}
		float yOffs[255] = {0.0};
		for (int i=0;i<255;i++){
			yOffs[i] = yNumbers[i] * yoffset / max;
		}

		memset(&renderer->cur_draw_t.lumaVertexs[0],0x00,sizeof(renderer->cur_draw_t.lumaVertexs));
		for (int i=0;i<255 * 2;i += 2){
			renderer->cur_draw_t.lumaVertexs[i * 2] = idex[0] + (i / 2) / 255.0f;
			renderer->cur_draw_t.lumaVertexs[i * 2 + 1] = idex[7];
			
			renderer->cur_draw_t.lumaVertexs[(i + 1) * 2] = idex[0] + (i / 2) / 255.0f;
			renderer->cur_draw_t.lumaVertexs[(i + 1) * 2 + 1] = idex[7] + yOffs[i/2];
		}
		return renderer->cur_draw_t.lumaVertexs;
	}else{
		memset(&renderer->cur_draw_t.oscVertexs[0],0x00,sizeof(renderer->cur_draw_t.oscVertexs));
		int hstart = loop * OSC_HEIGHT_STEP;
		int hend = hstart + OSC_HEIGHT_STEP;
		hstart = hstart < renderer->frame_height ? hstart : renderer->frame_height;
		hend = hend < renderer->frame_height ? hend : renderer->frame_height;
		if (hend <= hstart)return NULL;
		for (int h = hstart;h < hend;h++){
			int yStart = (h - hstart) * renderer->frame_width;
			for (int i=0;i<renderer->frame_width;i++){
				float yoffV = ybytes[i + h * renderer->frame_width] * yoffset / 255.0f;
				renderer->cur_draw_t.oscVertexs[(yStart + i) * 2] = idex[0] + i * 1.0f / renderer->frame_width;
				renderer->cur_draw_t.oscVertexs[(yStart + i) * 2 + 1] = idex[7] + yoffV;
			}
		}
		return renderer->cur_draw_t.oscVertexs;
	}
	
}

static void IJK_GLES2_Draw_Custom_Graph(IJK_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay)
{
	int lineMarkupType = renderer->cur_draw_t.drawType & 0x00F0;
	int partMarkupType = renderer->cur_draw_t.drawType & 0x0F00;

	if (lineMarkupType == GLES_MARKUP_TYPE_WIREFRAME ||
		lineMarkupType == GLES_MARKUP_TYPE_RATIO ||
		lineMarkupType == GLES_MARKUP_TYPE_PARTSCALE ||
		lineMarkupType == GLES_MARKUP_TYPE_B_TABLE ||
		lineMarkupType == GLES_MARKUP_TYPE_SCOPEBOX ||
		partMarkupType == GLES_PARTMARKUP_TYPE_CENTERFLAG)
	{
		IJK_GLES2_Display_use(renderer);
		if (lineMarkupType == GLES_MARKUP_TYPE_WIREFRAME || 
			lineMarkupType == GLES_MARKUP_TYPE_PARTSCALE ||
			lineMarkupType == GLES_MARKUP_TYPE_RATIO){
			float centerX = (lineMarkupType == GLES_MARKUP_TYPE_RATIO ? 50 : renderer->cur_draw_t.centerX) / 100.0f;
			float centerY = (lineMarkupType == GLES_MARKUP_TYPE_RATIO ? 50 : renderer->cur_draw_t.centerY) / 100.0f;
			centerX = (centerX - 0.5f) * 2;
			centerY = ( 0.5f - centerY) * 2;
			float whratio = renderer->cur_draw_t.whRatio;
			float lineW = renderer->cur_draw_t.lineWidth > 0 ? renderer->cur_draw_t.lineWidth * 1.0f : 1.0f;
			GLfloat *vertexs = NULL;
			if (lineMarkupType == GLES_MARKUP_TYPE_PARTSCALE){
				float ratio = renderer->cur_draw_t.partZoomRatio == 0.1f ? 0.3f : (renderer->cur_draw_t.partZoomRatio == 0.2f ? 0.4f : 0.5f);
				vertexs = IJK_GLES2_Draw_RectVertexs(renderer,centerX,centerY,ratio);
			}else{
				vertexs = IJK_GLES2_Draw_FrameVertexs(renderer,whratio,centerX,centerY);
			}
			glVertexAttribPointer(renderer->display_position, 3, GL_FLOAT, GL_FALSE, 12, vertexs);
			glEnableVertexAttribArray(renderer->display_position);
			glLineWidth(lineW);
			int argb = renderer->cur_draw_t.argbFrame;
			glUniform4f(renderer->display_color,((argb >> 16) & 0xFF) / 255.0f,
				((argb >> 8) & 0xFF) / 255.0f,(argb & 0xFF) / 255.0f,((argb >> 24) & 0xFF) / 255.0f);
			glDrawArrays(GL_LINE_STRIP, 0, 5); 
			glDisableVertexAttribArray(renderer->display_position);
		}else if (lineMarkupType == GLES_MARKUP_TYPE_B_TABLE ||
					lineMarkupType == GLES_MARKUP_TYPE_SCOPEBOX){
			int cycleNumber = lineMarkupType == GLES_MARKUP_TYPE_B_TABLE ? 1 : renderer->frame_height;
			for (int loop = 0;loop < cycleNumber;loop ++){
				if (overlay == NULL)break;
				GLfloat *vertexs = IJK_GLES2_Luma_Vertexs(renderer,overlay,lineMarkupType,loop);
				if (vertexs == NULL)break;
				glVertexAttribPointer(renderer->display_position, 2, GL_FLOAT, GL_FALSE, 8, vertexs);
				glEnableVertexAttribArray(renderer->display_position);
				glUniform4f(renderer->display_color,1.0f,1.0f,1.0f,1.0f);
				if (lineMarkupType == GLES_MARKUP_TYPE_B_TABLE){
					glLineWidth(2.0f);
					glDrawArrays(GL_LINES, 0, 255 * 2); 
				}else{
					int hstart = loop * OSC_HEIGHT_STEP;
					int hend = hstart + OSC_HEIGHT_STEP;
					hstart = hstart < renderer->frame_height ? hstart : renderer->frame_height;
					hend = hend < renderer->frame_height ? hend : renderer->frame_height;
 					if (hend > hstart){
						glUniform1f(renderer->point_size,renderer->cur_draw_t.scopePointSize);
 						glDrawArrays(GL_POINTS, 0, (hend - hstart) * renderer->frame_width); 
					}
				}
			}
			glDisableVertexAttribArray(renderer->display_position);
		}
		if (partMarkupType == GLES_PARTMARKUP_TYPE_CENTERFLAG){
			float orginWHRatio = renderer->frame_width * 1.0f / renderer->frame_height;
			if (renderer->cur_draw_t.cFlagType <= 1){
				float multi = renderer->cur_draw_t.cFlagType == 0 ? 1.0f : 0.5f;
				float ratio = 0.16f;
				float vertics[] = {-ratio / orginWHRatio * multi,0.0f,0.0f,
					ratio / orginWHRatio * multi,0.0f,0.0f,
					0.0f,ratio * multi,0.0f,
					0.0f,-ratio * multi,0.0f};
				glVertexAttribPointer(renderer->display_position, 3, GL_FLOAT, GL_FALSE, 12, vertics);
				glEnableVertexAttribArray(renderer->display_position);
				glLineWidth(renderer->cur_draw_t.cFlagType == 0 ? 10 : 5);
				int argb = renderer->cur_draw_t.argbFrame;
				glUniform4f(renderer->display_color,((argb >> 16) & 0xFF) / 255.0f,
					((argb >> 8) & 0xFF) / 255.0f,(argb & 0xFF) / 255.0f,((argb >> 24) & 0xFF) / 255.0f);
				glDrawArrays(GL_LINES, 0, 5); 
				glDisableVertexAttribArray(renderer->display_position);
			}else{
				glEnableVertexAttribArray(renderer->display_position);
				int argb = renderer->cur_draw_t.argbFrame;
				glUniform4f(renderer->display_color,((argb >> 16) & 0xFF) / 255.0f,
					((argb >> 8) & 0xFF) / 255.0f,(argb & 0xFF) / 255.0f,((argb >> 24) & 0xFF) / 255.0f);

			#define VERTEX_DATA_NUM 360
			#define PI 3.14159265
				float whratio = renderer->frame_width * 1.0f / renderer->frame_height;
				float size = renderer->cur_draw_t.cFlagType == 2 ? 0.08f : 0.05f;
				float radian = (float) (2 * PI / VERTEX_DATA_NUM);
				GLfloat *circleVertex = renderer->cur_draw_t.circleVertexs;
				circleVertex[0] = 0.0f;
        		circleVertex[1] = 0.0f;
				for (int i = 0; i < VERTEX_DATA_NUM; i++) {
           			circleVertex[2 * i + 2] = (float) (size * cos(radian * i)) / whratio;
            		circleVertex[2 * i + 1 + 2] = (float) (size * sin(radian * i));
        		}
				circleVertex[VERTEX_DATA_NUM * 2 + 2] = (float) (size * cos(radian)) / whratio;
        		circleVertex[VERTEX_DATA_NUM * 2 + 1 + 2] = (float) (size * sin(radian));
				glVertexAttribPointer(renderer->display_position, 2, GL_FLOAT, GL_FALSE, 8, circleVertex);
				glDrawArrays(GL_TRIANGLE_FAN, 0, VERTEX_DATA_NUM + 2);
				glDisableVertexAttribArray(renderer->display_position);
			}
		}
	}
}

static inline int av_isspace(int c)
{
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' ||
           c == '\v';
}
static int skip_line(const char *p)
{
    while (*p && av_isspace(*p))
        p++;
    return !*p || *p == '#';
}
#define NEXT_LINE(loop_cond) do {                           \
    if (!fgets(line, sizeof(line), f)) {                   \
		ALOGE("GLES_FS_TYPE_3DLUT Parse_cube Unexpected EOF\n");\
        return -2;                         \
    }                                                       \
} while (loop_cond)
static int IJK_GLES2_Renderer_Parse_cube(IJK_GLES2_Renderer *renderer, FILE *f)
{
	unsigned char *lutDataAddr = renderer->cur_draw_t.lutDatas;
    char line[LUT_MAX_LINE_SIZE];
    float min[3] = {0.0f, 0.0f, 0.0f};
    float max[3] = {1.0f, 1.0f, 1.0f};
	float rgb[3];
	int lutSize = 0;
    while (fgets(line, sizeof(line), f)) {
        if (!strncmp(line, "LUT_3D_SIZE ", 12)) {
            int i, j, k;
            const int size = strtol(line + 12, NULL, 0);
			lutSize = size;
			int S = size;
			int S2 = S * S;
			int W = S * 8;
			int idx,x,y,n = 0;
			int index;

            if (size < 2 || size > LUT_MAX_LEVEL) {
                ALOGE("GLES_FS_TYPE_3DLUT Parse_cube invalid 3D LUT size !\n");
                return -1;
            }
            for (k = 0; k < size; k++) {
                for (j = 0; j < size; j++) {
                    for (i = 0; i < size; i++) {
						idx = k *size *size + j * size + i;
						n = idx / S2;
						x = i;
						y = (idx % S2) / S + n / 8 * S;
						index = y * W + (n % 8) * S + x;
                        do {
try_again:
                            NEXT_LINE(0);
                            if (!strncmp(line, "DOMAIN_", 7)) {
                                float *vals = NULL;
                                if      (!strncmp(line + 7, "MIN ", 4)) vals = min;
                                else if (!strncmp(line + 7, "MAX ", 4)) vals = max;
                                if (!vals){
									ALOGE("GLES_FS_TYPE_3DLUT Parse_cube invalid DATA (DOMAIN_)!\n");
                                    return -3;
                                }
                                sscanf(line + 11, "%f %f %f", vals, vals + 1, vals + 2);
                                goto try_again;
                            }
                        } while (skip_line(line));
                        if (sscanf(line, "%f %f %f", &rgb[0], &rgb[1], &rgb[2]) != 3){
							ALOGE("GLES_FS_TYPE_3DLUT Parse_cube invalid DATA (RGB)!\n");
                            return -3;
                        }
                        rgb[0] *= max[0] - min[0];
                        rgb[1] *= max[1] - min[1];
                        rgb[2] *= max[2] - min[2];
						//ALOGE("%f %f %f\n",rgb[0],rgb[1],rgb[2]);
						lutDataAddr[index * 3] = (unsigned char)(rgb[0] * 255);
						lutDataAddr[index * 3 + 1] = (unsigned char)(rgb[1] * 255);
						lutDataAddr[index * 3 + 2] = (unsigned char)(rgb[2] * 255);
                    }
                }
            }
            break;
        }
    }
	renderer->cur_draw_t.lutSize = lutSize;
    return 0;
}

static void *parese_cube_thread(void *arg)
{
	IJK_GLES2_Renderer *renderer = (IJK_GLES2_Renderer *)arg;
    FILE *f = fopen(renderer->cur_draw_t.lutFilePath, "r");
	if (!f) {
		goto exit;
	}
	IJK_GLES2_Renderer_Parse_cube(renderer,f);
exit:
    pthread_exit(NULL);
    return(NULL);
}

void IJK_GLES2_Renderer_SetFilter(IJK_GLES2_Renderer *renderer,int cmd,int type,
	int centerX,int centerY,float ratio,int color,int lineW,const char *filePath)
{
	int fsType = cmd & 0x000F;
	int lineMarkupType = cmd & 0x00F0;
	int partMarkupType = cmd & 0x0F00;
    if ((fsType == 0xF) || (fsType > 0 && fsType < GLES_FS_TYPE_NUMBER)){
		int lutNewFile = 0;
		switch(fsType){
			case GLES_FS_TYPE_NORMAL:
			case GLES_FS_TYPE_GRAY:
				break;
			case GLES_FS_TYPE_SINGLE_C:
				renderer->cur_draw_t.argb = color;
				break;
			case GLES_FS_TYPE_PSEUDO:
				renderer->cur_draw_t.pseudoType = type;
				break;
			case GLES_FS_TYPE_AUX_FOCUS:
				renderer->cur_draw_t.argb = color;
				renderer->cur_draw_t.brightLimit = 100 - (unsigned char)ratio;
				renderer->cur_draw_t.auxfocuslinewidth = lineW;
				break;
			case GLES_FS_TYPE_3DLUT:
				if (strcmp(renderer->cur_draw_t.lutFilePath,filePath) != 0){
					lutNewFile = 1;
				}
				sprintf(renderer->cur_draw_t.lutFilePath,"%s",filePath);
				break;
			case GLES_FS_TYPE_ZEBRA_S:
				renderer->cur_draw_t.brightLimit = (unsigned char)ratio;
				break;
		}
		if (lutNewFile){
			FILE *f = fopen(filePath, "r");
		    if (!f) {
				ALOGE("GLES_FS_TYPE_3DLUT file %s not found !\n",filePath);
				fsType = renderer->cur_draw_t.drawType & 0xF;
		    }else{
		    	renderer->cur_draw_t.lutSize = 0;
		    	fclose(f);
				pthread_t tidp;
		    	if ((pthread_create(&tidp, NULL, parese_cube_thread, (void*)renderer)) != 0){
					ALOGE("GLES_FS_TYPE_3DLUT pthread_create error !\n");
					fsType = renderer->cur_draw_t.drawType & 0xF;
		    	}
		    }
		}else if (fsType != GLES_FS_TYPE_3DLUT){
			memset(renderer->cur_draw_t.lutFilePath,0x00,sizeof(renderer->cur_draw_t.lutFilePath));
			renderer->cur_draw_t.lutSize = 0;
		}
		renderer->cur_draw_t.drawType &= 0xFFF0;
		renderer->cur_draw_t.drawType |= fsType;
    }
	if ((lineMarkupType == 0xF0) || ((lineMarkupType >> 4) > 0 && (lineMarkupType >> 4) < GLES_MARKUP_TYPE_NUMBER)){
		switch(lineMarkupType){
			case GLES_MARKUP_TYPE_NORMAL:
				break;
			case GLES_MARKUP_TYPE_WIREFRAME:
				renderer->cur_draw_t.centerX = centerX;
				renderer->cur_draw_t.centerY = centerY;
				renderer->cur_draw_t.whRatio = ratio;
				renderer->cur_draw_t.argbFrame = color;
				renderer->cur_draw_t.lineWidth = lineW;
				renderer->cur_draw_t.wireFrameRatio = (type & 0xFF) / 100.0f;
				renderer->cur_draw_t.alphaOutside = (unsigned char)(type >> 16);
				break;
			case GLES_MARKUP_TYPE_RATIO:
				renderer->cur_draw_t.alphaOutside = type;
				renderer->cur_draw_t.whRatio = ratio;
				renderer->cur_draw_t.argbFrame = color;
				renderer->cur_draw_t.lineWidth = lineW;
				break;
			case GLES_MARKUP_TYPE_PARTSCALE:
				renderer->cur_draw_t.centerX = centerX;
				renderer->cur_draw_t.centerY = centerY;
				renderer->cur_draw_t.partZoomRatio = type / 10.0f;
				renderer->cur_draw_t.partZoomScale = ratio;
				renderer->cur_draw_t.argbFrame = color;
				renderer->cur_draw_t.lineWidth = lineW;
				break;
			case GLES_MARKUP_TYPE_B_TABLE:
			case GLES_MARKUP_TYPE_SCOPEBOX:
				renderer->cur_draw_t.centerX = centerX;
				renderer->cur_draw_t.centerY = centerY;
				renderer->cur_draw_t.scopePointSize = (lineW > 0 && lineW < 5) ? (float)lineW : 1.0f;
				renderer->cur_draw_t.alphabscope = (unsigned char)ratio;
				break;
				
		}
		renderer->cur_draw_t.drawType &= 0xFF0F;
		renderer->cur_draw_t.drawType |= lineMarkupType;
    }
	if ((partMarkupType == 0xF00) || ((partMarkupType >> 8) > 0 && (partMarkupType >> 8) < GLES_PARTMARKUP_TYPE_NUMBER)){
		switch(partMarkupType){
			case GLES_PARTMARKUP_TYPE_NORMAL:
				break;
			case GLES_PARTMARKUP_TYPE_CENTERFLAG:
				if (type >= GLES_CENTER_FLAG_BIGPLUS && type <= GLES_CENTER_FLAG_SMALLCIRCLE){
					renderer->cur_draw_t.cFlagType = type;
					renderer->cur_draw_t.argbFrame = color;
				}
				break;
		}
		renderer->cur_draw_t.drawType &= 0xF0FF;
		renderer->cur_draw_t.drawType |= partMarkupType;
    }

//DEBUG
	ALOGE("[Filter] ---------------------------------------[Filter]\n");
	ALOGE(" ---------------- frame w:h = %d:%d--------\n",renderer->frame_width,renderer->frame_height);
	ALOGE(" ---------------- drawType = 0x%x--------\n",renderer->cur_draw_t.drawType);
	ALOGE(" ---------------- centerX = %d--------\n",renderer->cur_draw_t.centerX);
	ALOGE(" ---------------- centerY = %d--------\n",renderer->cur_draw_t.centerY);
	ALOGE(" ---------------- lineWidth = %d--------\n",renderer->cur_draw_t.lineWidth);
	ALOGE(" ---------------- cFlagType = %d--------\n",renderer->cur_draw_t.cFlagType);
	ALOGE(" ---------------- scopePointSize = %f--------\n",renderer->cur_draw_t.scopePointSize);
	ALOGE(" ---------------- partZoomRatio = %f--------\n",renderer->cur_draw_t.partZoomRatio);
	ALOGE(" ---------------- pseudoType = %d--------\n",renderer->cur_draw_t.pseudoType);
	ALOGE(" ---------------- alphaOutside = %d--------\n",renderer->cur_draw_t.alphaOutside);
	ALOGE(" ---------------- brightLimit = %d--------\n",renderer->cur_draw_t.brightLimit);
	ALOGE(" ---------------- partZoomScale = %f--------\n",renderer->cur_draw_t.partZoomScale);
	ALOGE(" ---------------- whRatio = %f--------\n",renderer->cur_draw_t.whRatio);
	ALOGE(" ---------------- argbFrame = 0x%x--------\n",renderer->cur_draw_t.argbFrame);
	ALOGE(" ---------------- argb = 0x%x--------\n",renderer->cur_draw_t.argb);
	ALOGE(" ---------------- lutFilePath = %s--------\n",renderer->cur_draw_t.lutFilePath);
	ALOGE("[Filter] ---------------------------------------[Filter]\n");
}

