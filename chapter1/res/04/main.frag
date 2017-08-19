#version 130
in vec2 vTexCoord;
uniform sampler2D uColormap;

void main()
{
    gl_FragColor = texture2D(uColormap, vTexCoord);
}
