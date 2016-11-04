#version 110 

varying vec4 vertColor; //Pixel colour passed to fragment shader 

void main(void)
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
    
    vertColor = vec4(gl_Color);
}
