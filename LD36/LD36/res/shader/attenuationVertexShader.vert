#version 110 

varying vec4 vColour; //Pixel colour passed to fragment shader

varying float attenuation;
vec2 Point;

void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;

    float attenuation = 1.f / length(gl_Position - Point);
}