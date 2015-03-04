in vec4 position;

uniform mat4 _mvProj;
uniform mat4 _mv;

out vec3 vEyeSpacePos;

void main(void) {
    vec4 v = position;
    gl_Position = _mvProj * v;
    vEyeSpacePos = vec3(_mv * v);
} 