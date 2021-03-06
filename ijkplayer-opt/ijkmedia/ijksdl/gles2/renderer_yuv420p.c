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

#include "internal.h"

static GLboolean yuv420p_use(IJK_GLES2_Renderer *renderer)
{
    //ALOGI("use render yuv420p\n");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glUseProgram(renderer->program);            IJK_GLES2_checkError_TRACE("glUseProgram");

    if (0 == renderer->plane_textures[0])
        glGenTextures(3, renderer->plane_textures);

	if (0 == renderer->lut_textures[0])
        glGenTextures(1, renderer->lut_textures);

	int i = 0;
    for (; i < 3 + 1; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
		if (i < 3)
        	glBindTexture(GL_TEXTURE_2D, renderer->plane_textures[i]);
		else
			glBindTexture(GL_TEXTURE_2D, renderer->lut_textures[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (i < 3)
        	glUniform1i(renderer->us2_sampler[i], i);
		else
			glUniform1i(renderer->lut_sampler, i);
    }

    glUniformMatrix3fv(renderer->um3_color_conversion, 1, GL_FALSE, IJK_GLES2_getColorMatrix_bt709());
    return GL_TRUE;
}

static GLsizei yuv420p_getBufferWidth(IJK_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay)
{
    if (!overlay)
        return 0;

    return overlay->pitches[0] / 1;
}
static GLboolean yuv420p_uploadTexture(IJK_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay)
{
    if (!renderer || !overlay)
        return GL_FALSE;

          int     planes[3]    = { 0, 1, 2 };
    const GLsizei widths[3]    = { overlay->pitches[0], overlay->pitches[1], overlay->pitches[2] };
    const GLsizei heights[3]   = { overlay->h,          overlay->h / 2,      overlay->h / 2 };
    const GLubyte *pixels[3]   = { overlay->pixels[0],  overlay->pixels[1],  overlay->pixels[2] };

    switch (overlay->format) {
        case SDL_FCC_I420:
            break;
        case SDL_FCC_YV12:
            planes[1] = 2;
            planes[2] = 1;
            break;
        default:
            ALOGE("[yuv420p] unexpected format %x\n", overlay->format);
            return GL_FALSE;
    }

    for (int i = 0; i < 3; ++i) {
        int plane = planes[i];

        glBindTexture(GL_TEXTURE_2D, renderer->plane_textures[i]);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_LUMINANCE,
                     widths[plane],
                     heights[plane],
                     0,
                     GL_LUMINANCE,
                     GL_UNSIGNED_BYTE,
                     pixels[plane]);
    }
	if (renderer->cur_draw_t.lutSize > 0){
		glBindTexture(GL_TEXTURE_2D, renderer->lut_textures[0]);

		int hLine = renderer->cur_draw_t.lutSize / 8 + (renderer->cur_draw_t.lutSize % 8 == 0 ? 0 : 1);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     renderer->cur_draw_t.lutSize * 8,
                     renderer->cur_draw_t.lutSize * hLine,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     renderer->cur_draw_t.lutDatas);
	}
    return GL_TRUE;
}

IJK_GLES2_Renderer *IJK_GLES2_Renderer_create_yuv420p()
{
    ALOGI("create render yuv420p\n");
    IJK_GLES2_Renderer *renderer = IJK_GLES2_Renderer_create_base(IJK_GLES2_getFragmentShader_yuv420p());
    if (!renderer)
        goto fail;

    renderer->us2_sampler[0] = glGetUniformLocation(renderer->program, "us2_SamplerX"); IJK_GLES2_checkError_TRACE("glGetUniformLocation(us2_SamplerX)");
    renderer->us2_sampler[1] = glGetUniformLocation(renderer->program, "us2_SamplerY"); IJK_GLES2_checkError_TRACE("glGetUniformLocation(us2_SamplerY)");
    renderer->us2_sampler[2] = glGetUniformLocation(renderer->program, "us2_SamplerZ"); IJK_GLES2_checkError_TRACE("glGetUniformLocation(us2_SamplerZ)");
    renderer->lut_sampler = glGetUniformLocation(renderer->program, "lut_sampler"); IJK_GLES2_checkError_TRACE("glGetUniformLocation(lut_sampler)");
	
    renderer->um3_color_conversion = glGetUniformLocation(renderer->program, "um3_ColorConversion"); IJK_GLES2_checkError_TRACE("glGetUniformLocation(um3_ColorConversionMatrix)");
	renderer->cunstom_cmd_type = glGetUniformLocation(renderer->program, "cunstom_cmd_type");            IJK_GLES2_checkError_TRACE("glGetUniformLocation(cunstom_cmd_type)");
	renderer->cunstom_Params = glGetUniformLocation(renderer->program, "cunstom_Params");            IJK_GLES2_checkError_TRACE("glGetUniformLocation(cunstom_Params)");
	renderer->cunstom_Params_plus = glGetUniformLocation(renderer->program, "cunstom_Params_plus"); 		   IJK_GLES2_checkError_TRACE("glGetUniformLocation(cunstom_Params_plus)");
	renderer->cunstom_factor = glGetUniformLocation(renderer->program, "cunstom_factor"); 		   IJK_GLES2_checkError_TRACE("glGetUniformLocation(cunstom_factor)");
	renderer->cunstom_Colors = glGetUniformLocation(renderer->program, "cunstom_Colors");            IJK_GLES2_checkError_TRACE("glGetUniformLocation(cunstom_Colors)");

    renderer->func_use            = yuv420p_use;
    renderer->func_getBufferWidth = yuv420p_getBufferWidth;
    renderer->func_uploadTexture  = yuv420p_uploadTexture;

    return renderer;
fail:
    IJK_GLES2_Renderer_free(renderer);
    return NULL;
}
