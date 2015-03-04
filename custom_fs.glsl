in vec3 vEyeSpacePos;

out vec4 fragColor;


void main(void)
{
    float dist = -vEyeSpacePos.z;
    fragColor = vec4(dist,dist,dist, 1.0);
}
 