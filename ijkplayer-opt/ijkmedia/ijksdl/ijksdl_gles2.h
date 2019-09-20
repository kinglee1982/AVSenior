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

#ifndef IJKSDL__IJKSDL_GLES2_H
#define IJKSDL__IJKSDL_GLES2_H

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif

typedef struct SDL_VoutOverlay SDL_VoutOverlay;

/*
 * Common
 */

#ifdef DEBUG
#define IJK_GLES2_checkError_TRACE(op)
#define IJK_GLES2_checkError_DEBUG(op)
#else
#define IJK_GLES2_checkError_TRACE(op) IJK_GLES2_checkError(op) 
#define IJK_GLES2_checkError_DEBUG(op) IJK_GLES2_checkError(op)
#endif

#define GLES_MARKUP_RATIO_TYPE 0x1111
#define GLES_MARKUP_PARTSCALE_TYPE 0x2222
#define GLES_FLOAT_BYTE_SIZE 4
#define GLES_VERTEX_COORD_NUM 3
#define GLES_RECT_POINTS_COORD_NUM (GLES_VERTEX_COORD_NUM * 5)
//for Fragment Shader Type 0x0--0xf
#define GLES_FS_TYPE_NORMAL      0xF
#define GLES_FS_TYPE_GRAY        0x1
#define GLES_FS_TYPE_SINGLE_C    0x2  //color
#define GLES_FS_TYPE_PSEUDO      0x3  //type:maybe need type
#define GLES_FS_TYPE_AUX_FOCUS   0x4  //color:stroke color
#define GLES_FS_TYPE_3DLUT       0x5  //filepath
#define GLES_FS_TYPE_ZEBRA_S     0x6  //ratio : brightness ratio
#define GLES_FS_TYPE_NUMBER     (GLES_FS_TYPE_ZEBRA_S + 1)
//end Fragment Shader Type

//for line markup Type 0x00--0xf0
#define GLES_MARKUP_TYPE_NORMAL      0xF0
#define GLES_MARKUP_TYPE_WIREFRAME   0x10 //centerX,centerY,ratio,color,lineW,type(wireFrameRatio)
#define GLES_MARKUP_TYPE_RATIO       0x20 //ratio,color,lineW ,type(for alphaOutside)
#define GLES_MARKUP_TYPE_PARTSCALE    0x30 //centerX,centerY,type(partZoomType),ratio(Scaling ratio)
#define GLES_MARKUP_TYPE_NUMBER      ((GLES_MARKUP_TYPE_PARTSCALE >> 4) + 1)
//end line markup Type

//for part markup Type 0x000--0xf00
#define GLES_PARTMARKUP_TYPE_NORMAL       0xF00
#define GLES_PARTMARKUP_TYPE_CENTERFLAG   0x100 //type
#define GLES_PARTMARKUP_TYPE_NUMBER       ((GLES_PARTMARKUP_TYPE_CENTERFLAG >> 8) + 1)
//end part markup Type

//for analysis Type 0x0000--0xf000
#define GLES_ANALYSIS_TYPE_NORMAL       0xF000
#define GLES_ANALYSIS_TYPE_B_TABLE     0x1000 //brightness table
#define GLES_ANALYSIS_TYPE_SCOPEBOX    0x2000 //
#define GLES_ANALYSIS_TYPE_NUMBER       ((GLES_ANALYSIS_TYPE_SCOPEBOX >> 12) + 1)
//end analysis Type

//for center flag Type 0-3
#define GLES_CENTER_FLAG_BIGPLUS        0
#define GLES_CENTER_FLAG_SMALLPLUS      1
#define GLES_CENTER_FLAG_BIGCIRCLE      2 
#define GLES_CENTER_FLAG_SMALLCIRCLE    3
//end center flag Type


typedef struct GLES2_Draw_Type_t{
	unsigned short drawType;
	unsigned char centerX;//0-100 percent
	unsigned char centerY;//0-100 percent
	unsigned char lineWidth; //pixel
	unsigned char cFlagType; //center flag draw type + or .0-3
	unsigned char partZoomType; //large, medium and small 0-2
	unsigned char pseudoType; //pseudo type 0-2
	unsigned char alphaOutside; //0x00-0xff
	unsigned char brightLimit; //0x00-0xff  for zebra-stripe
	float wireFrameRatio;//frame scale
	float partZoomRatio;//part scale
	float whRatio;// w / h
	int argbFrame;//color for line frame
	int argb;//color
	char filePath[128];//3d lut filepath
	GLfloat rectVertexs[GLES_RECT_POINTS_COORD_NUM];
}GLES2_Draw_Type;

void IJK_GLES2_printString(const char *name, GLenum s);
void IJK_GLES2_checkError(const char *op);

GLuint IJK_GLES2_loadShader(GLenum shader_type, const char *shader_source);

/*
 * Renderer
 */
#define IJK_GLES2_MAX_PLANE 3
typedef struct IJK_GLES2_Renderer IJK_GLES2_Renderer;

IJK_GLES2_Renderer *IJK_GLES2_Renderer_create(SDL_VoutOverlay *overlay);
void      IJK_GLES2_Renderer_reset(IJK_GLES2_Renderer *renderer);
void      IJK_GLES2_Renderer_free(IJK_GLES2_Renderer *renderer);
void      IJK_GLES2_Renderer_freeP(IJK_GLES2_Renderer **renderer);

GLboolean IJK_GLES2_Renderer_setupGLES();
GLboolean IJK_GLES2_Renderer_isValid(IJK_GLES2_Renderer *renderer);
GLboolean IJK_GLES2_Renderer_isFormat(IJK_GLES2_Renderer *renderer, int format);
GLboolean IJK_GLES2_Renderer_use(IJK_GLES2_Renderer *renderer);
GLboolean IJK_GLES2_Renderer_renderOverlay(IJK_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay);

#define IJK_GLES2_GRAVITY_RESIZE                (0) // Stretch to fill view bounds.
#define IJK_GLES2_GRAVITY_RESIZE_ASPECT         (1) // Preserve aspect ratio; fit within view bounds.
#define IJK_GLES2_GRAVITY_RESIZE_ASPECT_FILL    (2) // Preserve aspect ratio; fill view bounds.
GLboolean IJK_GLES2_Renderer_setGravity(IJK_GLES2_Renderer *renderer, int gravity, GLsizei view_width, GLsizei view_height);
void IJK_GLES2_Renderer_changeFShader(IJK_GLES2_Renderer *renderer, const char *fragment_shader_source);
void IJK_GLES2_Renderer_SetFilter(IJK_GLES2_Renderer *renderer,int cmd,int type,
	int centerX,int centerY,float ratio,int color,int lineW,const char *filePath);
#endif
