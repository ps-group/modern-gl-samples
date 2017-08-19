#version 130
in vec2 aPosition;
in vec2 aTexCoord;
out vec2 vTexCoord;
uniform mat4 uProjectionMatrix;
uniform mat4 uWorldMatrix;

void main()
{
    vTexCoord = aTexCoord;
    gl_Position = uProjectionMatrix * uWorldMatrix * vec4(aPosition, 0.0, 1.0);
}
