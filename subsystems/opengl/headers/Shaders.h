//
// Created by Guy on 25/02/2017.
//

#ifndef FIRESIM_SHADERS_H
#define FIRESIM_SHADERS_H

#define SHADER_ROOT std::string("assets/shaders")
#define TEXTURE_SHADER_VERT SHADER_ROOT + "/vert.glsl"
#define TEXTURE_SHADER_FRAG SHADER_ROOT + "/sprite.frag.glsl"

#define CLOUD_SHADER_VERT SHADER_ROOT + "/vert.glsl"
#define CLOUD_SHADER_FRAG SHADER_ROOT + "/cloud.frag.glsl"

#define EXTRACT_SHADER_VERT SHADER_ROOT + "/vert.glsl"
#define EXTRACT_SHADER_FRAG SHADER_ROOT + "/extract.frag.glsl"

#define BLUR_SHADER_VERT SHADER_ROOT + "/vert.glsl"
#define BLUR_SHADER_FRAG SHADER_ROOT + "/blur.frag.glsl"

#define SCREEN_SHADER_VERT SHADER_ROOT + "/vert.glsl"
#define SCREEN_SHADER_FRAG SHADER_ROOT + "/screen.frag.glsl"

#define TEXTURE_SHADER "shader.texture"
#define CLOUD_SHADER "shader.cloud"

#define EXTRACT_SHADER "shader.extract"
#define BLUR_SHADER "shader.blur"
#define SCREEN_SHADER "shader.screen"

#endif //FIRESIM_SHADERS_H
