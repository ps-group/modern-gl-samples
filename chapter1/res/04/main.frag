#version 130
in vec2 vTexCoord;
uniform sampler2D uColormap;

void main()
{
    gl_FragColor = vec4(0.1 * vTexCoord.x, 0.1 * vTexCoord.y, 0, 0) + texture2D(uColormap, vTexCoord);
}
