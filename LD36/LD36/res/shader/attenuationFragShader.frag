#version 110

// varying float attenuation;
uniform sampler2D texture;

//Shader parameters
//uniform vec2 point;
uniform float lightIntensity;

void main()
{

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    //float bug = 100.0;

    //float distance = 0.0;

    //distance = length(point - gl_FragCoord.xy);
    //float atten; 
    
    //distance *= distance; //ENABLE FOR QUADRATIC FALLOFF
    //distance *= (distance*distance); //ENABLE FOR CUBIC DROP OFF
    
    // vec4 col; 
    
    // pixel.w *= 1.f / attenuation;
    
    // col = vec4(gl_Color);
    // col += pixel;
    // col.w *= attenuation;

    // if (col.w > 0.1f)
    // { 
    //     col.y *= bug;
    // }
    // col.w = ((1.f / distance) * pixel.w) * attenuationConstant;
    
     gl_FragColor = gl_Color * pixel;
}
