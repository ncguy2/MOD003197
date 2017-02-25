#version 330
in vec2 TexCoords;
out vec4 FinalColour;

uniform float timeScale = 10.0;
uniform float cloudScale = 0.5;
uniform float skyCover = 0.6;
uniform float softness = 0.2;
uniform float brightness = 1.0;
uniform int noiseOctaves = 8;
uniform float curlStrain = 3.0;
uniform float cover = 0.5;

uniform float time;
uniform vec2 screenRes;

float noise(vec2 uv);

float saturate(float num) {
    return clamp(num,0.0,1.0);
}

float hash(float n) { return fract(sin(n) * 1e4); }
float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }

vec2 rotate(vec2 uv) {
    uv = uv + noise(uv*0.2)*0.005;
    float rot = curlStrain;
    float sinRot=sin(rot);
    float cosRot=cos(rot);
    mat2 rotMat = mat2(cosRot,-sinRot,sinRot,cosRot);
    return uv * rotMat;
}

float fbm (vec2 uv) {
    float rot = 1.57;
    float sinRot=sin(rot);
    float cosRot=cos(rot);
    float f = 0.0;
    float total = 0.0;
    float mul = 0.5;
    mat2 rotMat = mat2(cosRot,-sinRot,sinRot,cosRot);

    for(int i = 0;i < noiseOctaves;i++) {
        f += noise(uv+time*0.00015*timeScale*(1.0-mul))*mul;
        total += mul;
        uv *= 3.0;
        uv=rotate(uv);
        mul *= 0.5;
    }
    return f/total;
}

void main() {
    vec2 texelPos = TexCoords / screenRes;
    vec2 uv = TexCoords / (40000.0 * cloudScale);
    float bright = brightness * (1.8 - cover);

    float col1 = fbm(uv -  0.5 * time * 0.00004 * timeScale);
    float col2 = fbm(uv - 10.5 * time * 0.00002 * timeScale);

    float clouds1 = smoothstep(1.0 - cover, min((1.0 - cover) + softness * 2.0, 1.0), col1);
    float clouds2 = smoothstep(1.0 - cover, min((1.0 - cover) + softness, 1.0), col2);

    float cloudsFormComb = saturate(clouds1 + clouds2);

    vec4 skyCol = vec4(0, 0, 0, 1);
    float cloudCol = saturate(saturate(1.0 - pow(col1, 1.0) * 0.2) * bright);
    vec4 clouds1Col = vec4(cloudCol, cloudCol, cloudCol, 1.0);
    vec4 clouds2Col = mix(clouds1Col, skyCol, 0.25);
    vec4 cloudsCombCol = mix(clouds1Col, clouds2Col, saturate(clouds1 - clouds2));

    FinalColour = mix(skyCol, cloudsCombCol, cloudsFormComb);
}

float noise(vec2 uv) {
    vec2 i = floor(uv);
    vec2 f = fract(uv);

    // Four corners in 2D of a tile
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    // Simple 2D lerp using smoothstep envelope between the values.
    // return vec3(mix(mix(a, b, smoothstep(0.0, 1.0, f.x)),
    //          mix(c, d, smoothstep(0.0, 1.0, f.x)),
    //          smoothstep(0.0, 1.0, f.y)));

    // Same code, with the clamps in smoothstep and common subexpressions
    // optimized away.
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}