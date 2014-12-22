in vec4 position;
in vec3 normal;
in vec2 uv1;

uniform mat4 _mvProj;
uniform mat4 _mv;
uniform mat4 _m;
uniform mat4 _lightMat;
uniform mat3 _norm;

out vec3 vEyeSpacePos;
out vec2 vUv;
out vec3 vNormal;
out vec4 vShadowMapCoord;
out vec3 vEcPosition;

void main(void) {
    vec4 v = position;
    gl_Position = _mvProj * v;
    vEcPosition = (_mv * v).xyz;
    vUv = uv1;
    vNormal = normalize(_norm * normal);
    vShadowMapCoord = _lightMat  * v;
    vEyeSpacePos = vec3(_mv * v);
} 