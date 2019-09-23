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

#include "ijksdl/gles2/internal.h"

static const char g_shader[] = IJK_GLES_STRING(
    precision highp float;
    varying   highp vec2 vv2_Texcoord;
	varying   highp vec4 gPosition;
    uniform         mat3 um3_ColorConversion;
    uniform   lowp  sampler2D us2_SamplerX;
    uniform   lowp  sampler2D us2_SamplerY;
    uniform   lowp  sampler2D us2_SamplerZ;

	uniform   ivec2 cunstom_cmd_type;
	uniform   vec4 cunstom_Params;
	uniform   vec4 cunstom_Params_plus;
	uniform   vec4 cunstom_Colors;
    void main()
    {
        mediump vec3 yuv;
        lowp    vec3 rgb;
		float fcolor;
		int fcmd = cunstom_cmd_type.x;
		int ftype = cunstom_cmd_type.y;
		float aratio = cunstom_Colors.a;

		yuv.x = (texture2D(us2_SamplerX, vv2_Texcoord).r - (16.0 / 255.0));
        yuv.y = (texture2D(us2_SamplerY, vv2_Texcoord).r - 0.5);
        yuv.z = (texture2D(us2_SamplerZ, vv2_Texcoord).r - 0.5);
		if (ftype == 0x2222){
			float centerX = cunstom_Params.x;
			float centerY = cunstom_Params.y;
			float ratio = cunstom_Params.z;
			float offsetX = cunstom_Params_plus.y;
			float offsetY = cunstom_Params_plus.z;
			vec2  center = vec2(centerX, centerY);
			float dis = distance(vec2(gPosition.x, gPosition.y/cunstom_Params_plus.x), center);
			if (dis < cunstom_Params.w){
				float x = vv2_Texcoord.x / ratio + offsetX;
				float y = vv2_Texcoord.y / ratio + offsetY;
				yuv.x = (texture2D(us2_SamplerX, vec2(x, y)).r - (16.0 / 255.0));
        		yuv.y = (texture2D(us2_SamplerY, vec2(x, y)).r - 0.5);
        		yuv.z = (texture2D(us2_SamplerZ, vec2(x, y)).r - 0.5);
			}
		}
        rgb = um3_ColorConversion * yuv;
		vec3 color = rgb;
		
		if (fcmd == 0xF){
			
		}else if(fcmd == 0x1){
        	fcolor = rgb.r * 0.299 + rgb.g * 0.587 + rgb.b * 0.114;
        	color = vec3(fcolor,fcolor,fcolor);
		}else if (fcmd == 0x2){
			vec3 crgb = vec3(cunstom_Colors.r,cunstom_Colors.g,cunstom_Colors.b);
        	color = crgb * rgb;
		}else if (fcmd == 0x3){
        	fcolor = rgb.r * 0.299 + rgb.g * 0.587 + rgb.b * 0.114;
			color = vec3(abs(0.0 - fcolor),abs(0.5 - fcolor),abs(1.0 - fcolor));
		}else if (fcmd == 0x4){
        	
		}else if (fcmd == 0x5){
        	
		}else if (fcmd == 0x6){
        	
		}
		gl_FragColor = vec4(color, 1);
		if (ftype == 0x1111){
			if (cunstom_Params.x != 0.0f){
				float wratio = cunstom_Params.x;
				if (vv2_Texcoord.x <= wratio){
					gl_FragColor = vec4(color * aratio, aratio);
				}
				if (vv2_Texcoord.x >= 1.0f - wratio){
					gl_FragColor = vec4(color * aratio, aratio);
				}
			}
			if (cunstom_Params.y != 0.0f){
				float hratio = cunstom_Params.y;
				if (vv2_Texcoord.y >= 1.0f - hratio){
					gl_FragColor = vec4(color * aratio, aratio);
				}
				if (vv2_Texcoord.y <= hratio){
					gl_FragColor = vec4(color * aratio, aratio);
				}
			}
		}
    }
);

const char *IJK_GLES2_getFragmentShader_yuv420p()
{
	return g_shader;
}
