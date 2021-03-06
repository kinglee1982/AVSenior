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
	uniform   lowp  sampler2D lut_sampler;

	uniform   ivec2 cunstom_cmd_type;
	uniform   vec4 cunstom_Params;
	uniform   vec4 cunstom_Params_plus;
	uniform   vec4 cunstom_Colors;
	uniform   vec2 cunstom_factor;

	vec3 getFalseColor(float y)
	{
        float num = (219.0 / 255.00);
		if (y <= 0.02 * num){
			return vec3(0.690,0.169,0.623);
		}else if (y <= 0.08 * num){
			return vec3(0.090,0.396,0.588);
		}else if (y <= 0.15 * num){
			return vec3(0.102,0.513,0.659);
		}else if (y <= 0.24 * num){
			return vec3(0.259,0.635,0.658);
		}else if (y <= 0.43 * num){
			return vec3(0.517,0.517,0.517);
		}else if (y <= 0.47 * num){
			return vec3(0.396,0.729,0.255);
		}else if (y <= 0.54 * num){
			return vec3(0.619,0.619,0.619);
		}else if (y <= 0.58 * num){
			return vec3(0.980,0.498,0.447);
		}else if (y <= 0.76 * num){
			return vec3(0.819,0.819,0.819);
		}else if (y <= 0.85 * num){
			return vec3(0.945,0.910,0.588);
		}else if (y <= 0.92 * num){
			return vec3(1.0,1.0,0.0);
		}else if (y <= 0.98 * num){
			return vec3(1.0,0.588,0.0);
		}else if (y <= 1.0 * num){
			return vec3(0.996,0.0,0.0);
		}else{
			return vec3(0.0,0.0,0.0);
		}
	}

	vec4 outSideAlpha(vec3 inColor,float aratio)
	{
		float l = cunstom_Params.x;
		float t = cunstom_Params.y;
		float r = cunstom_Params.z;
		float b = cunstom_Params.w;
		float x = vv2_Texcoord.x;
		float y = vv2_Texcoord.y;
		if (x <= l){
			return vec4(inColor * aratio, aratio);
		}
		if (x >= r){
			return vec4(inColor * aratio, aratio);
		}
		if (y >= b){
			return vec4(inColor * aratio, aratio);
		}
		if (y <= t){
			return vec4(inColor * aratio, aratio);
		}
		return vec4(inColor,1.0);
	}

	vec4 inSideAlpha(vec3 inColor,float aratio)
	{
		float l = cunstom_Params.x;
		float r = cunstom_Params.y;
		float t = cunstom_Params.z;
		float b = cunstom_Params.w;
		float x = vv2_Texcoord.x;
		float y = vv2_Texcoord.y;
		if (x > l){
			if (x < r){
				if (y > t){
					if (y < b){
						return vec4(inColor * aratio, aratio);
					}
				}
			}
		}
		return vec4(inColor,1.0);
	}
#if 0
	vec3 magnifier(vec3 yuv)
	{
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
		return yuv;
	}
#else
	vec3 magnifier(vec3 yuv)
	{
		float x = vv2_Texcoord.x;
		float y = vv2_Texcoord.y;
		float l = cunstom_Params.x;
		float r = cunstom_Params.y;
		float t = cunstom_Params.z;
		float b = cunstom_Params.w;
		
		float ratio = cunstom_Params_plus.x;
		float offsetX = cunstom_Params_plus.y;
		float offsetY = cunstom_Params_plus.z;
		if (x > l){
			if (x < r){
				if (y > t){
					if (y < b){
						float xp = x / ratio + offsetX;
						float yp = y / ratio + offsetY;
						yuv.x = (texture2D(us2_SamplerX, vec2(xp, yp)).r - (16.0 / 255.0));
    					yuv.y = (texture2D(us2_SamplerY, vec2(xp, yp)).r - 0.5);
    					yuv.z = (texture2D(us2_SamplerZ, vec2(xp, yp)).r - 0.5);
					}
				}
			}
		}
		return yuv;
	}
#endif
	vec3 zebra(vec3 inColor,float y,float division)
	{
		if (y >= division){
			float tx = vv2_Texcoord.x;
			float ty = vv2_Texcoord.y;
			float v = tx + ty;
			//0.002 for line width ;0.0070 for line space
			//v % 0.020
            if ((v - 0.0070 * floor(v / 0.0070)) <= 0.002){
                return vec3(0.3,0.3,0.3);
            }
		}
		return inColor;
	}

	vec3 auxfocus(vec3 inColor,vec3 linecolor,float lineAlpha)
	{  
		vec2 v_directTexCoord[8];
		float YValues[8];

		float wfactor = cunstom_factor.x;
		float hfactor = cunstom_factor.y;
		float rfactor = cunstom_Params_plus.w;
		mediump vec2 widthStep = vec2(wfactor, 0.0);
    	mediump vec2 heightStep = vec2(0.0, hfactor);
    	mediump vec2 widthHeightStep = vec2(wfactor, hfactor);
    	mediump vec2 widthNegativeHeightStep = vec2(wfactor, -hfactor);
		
		v_directTexCoord[0] = vv2_Texcoord - widthStep;//left
    	v_directTexCoord[1] = vv2_Texcoord + widthStep;//right
    	v_directTexCoord[2] = vv2_Texcoord + heightStep;//top
    	v_directTexCoord[3] = vv2_Texcoord - heightStep;//bottom
    	v_directTexCoord[4] = vv2_Texcoord - widthHeightStep;//left top
    	v_directTexCoord[5] = vv2_Texcoord + widthNegativeHeightStep ;//right top
    	v_directTexCoord[6] = vv2_Texcoord - widthNegativeHeightStep;//left bottom
    	v_directTexCoord[7] = vv2_Texcoord + widthHeightStep;//right bottom

		YValues[0] = (texture2D(us2_SamplerX, v_directTexCoord[0]).r - (16.0 / 255.0));//left
     	YValues[1] = (texture2D(us2_SamplerX, v_directTexCoord[1]).r - (16.0 / 255.0));//right
     	YValues[2] = (texture2D(us2_SamplerX, v_directTexCoord[2]).r - (16.0 / 255.0));//top
     	YValues[3] = (texture2D(us2_SamplerX, v_directTexCoord[3]).r - (16.0 / 255.0));//bottom
     	YValues[4] = (texture2D(us2_SamplerX, v_directTexCoord[4]).r - (16.0 / 255.0));//left top
     	YValues[5] = (texture2D(us2_SamplerX, v_directTexCoord[5]).r - (16.0 / 255.0));//right top
     	YValues[6] = (texture2D(us2_SamplerX, v_directTexCoord[6]).r - (16.0 / 255.0));//left bottom
     	YValues[7] = (texture2D(us2_SamplerX, v_directTexCoord[7]).r - (16.0 / 255.0));//right bottom

		float hColor = YValues[0] * -2.0 + YValues[1] * 2.0 +
                    YValues[2] * 0.0 + YValues[3] * 0.0 +
                    YValues[4] * -1.0 + YValues[5] * 1.0 +
                    YValues[6] * -1.0 + YValues[7] * 1.0;

     	float vColor = YValues[0] * 0.0 + YValues[1] * 0.0 +
                    YValues[2] * -2.0 + YValues[3] * 2.0 +
                    YValues[4] * -1.0 + YValues[5] * -1.0 +
                    YValues[6] * 1.0 + YValues[7] * 1.0;

     	float finalColor = length(vec2(hColor, vColor));
		if (finalColor >= rfactor){
			vec4 fcolor = mix(vec4(inColor,1.0), vec4(linecolor,1.0), lineAlpha);
			return fcolor.rgb;
		}
		return inColor;
	}

	vec3 lut3dMapping(vec3 rgb,float size)
	{
		if (size <= 1.0)return rgb;
		float r = rgb.r;
		float g = rgb.g;
		float b = rgb.b;
		if (b > 0.99) b = 0.99;
	    float blueColor = b * (size - 1.0);
		float fw = cunstom_factor.x;
		float fh = cunstom_factor.y;
		float oneW = fw / 8.0;
		float oneh = 0.0;
		float mod = size - 8.0 * (floor(size / 8.0));
		float yline = 0.0;
		if (mod == 0.0){
			yline = size / 8.0;
		}else{
			yline = size / 8.0 + 1.0;	
		}
		oneh = fh / yline;
		float xOff = 1.0 / 8.0;
		float yOff = 1.0 / yline;
	    vec2 quad;
	    quad.y = floor(floor(blueColor) / 8.0);
	    quad.x = floor(blueColor) - (quad.y * 8.0);

		float rp = 0.5/fw + ((xOff - 1.0/fw) * r);
		float gp = 0.5/fh + ((yOff - 1.0/fh) * g);
	    vec2 texPos;
	    texPos.x = (quad.x * xOff) + rp;
	    texPos.y = (quad.y * yOff) + gp;
	    
	    vec4 newColor = texture2D(lut_sampler, texPos);
	    vec4 fcolor = mix(vec4(rgb,1.0), vec4(newColor.rgb, 1.0), 1.0);
		return fcolor.rgb;
	}

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
			yuv = magnifier(yuv);
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
			color = getFalseColor(yuv.x);
		}else if (fcmd == 0x4){
			vec3 crgb = vec3(cunstom_Colors.r,cunstom_Colors.g,cunstom_Colors.b);
			color = auxfocus(color,crgb,cunstom_Colors.a);
		}else if (fcmd == 0x5){
        	color = lut3dMapping(color,cunstom_Params_plus.w);
        	//color = texture2D(lut_sampler, vv2_Texcoord).rgb;
		}else if (fcmd == 0x6){
        	color = zebra(color,yuv.x,cunstom_Params_plus.w);
		}
		gl_FragColor = vec4(color, 1);
		if (ftype == 0x1111){
			gl_FragColor = outSideAlpha(color,aratio);
		}
		if (ftype == 0x3333){
			gl_FragColor = inSideAlpha(color,cunstom_Params_plus.x);
		}
    }
);

const char *IJK_GLES2_getFragmentShader_yuv420p()
{
	return g_shader;
}
