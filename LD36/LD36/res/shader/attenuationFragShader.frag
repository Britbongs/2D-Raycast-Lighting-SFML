#version 110
uniform sampler2D texture;

//Shader parameters
uniform vec2 point;
uniform float attenuationConstant;
void main()
{

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    float bug = 0.0;

    float distance = 0.0;

    distance = length(point - gl_FragCoord.xy);
    
    //distance *= distance; //ENABLE FOR QUADRATIC FALLOFF
    //distance *= (distance*distance); //ENABLE FOR CUBIC DROP OFF
    
    vec4 Col; 
    Col.x = gl_Color.x;
    Col.y = gl_Color.y;
    Col.z = gl_Color.z;
    Col.w = gl_Color.w;
    Col.w = (1.f / distance) * attenuationConstant;

    gl_FragColor = Col;
}
