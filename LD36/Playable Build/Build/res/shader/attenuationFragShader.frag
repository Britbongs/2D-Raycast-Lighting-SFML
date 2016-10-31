#version 110

varying vec4 vertColor;
uniform sampler2D texture;
uniform vec2 point;
float lightIntensity;

float calculateAttenuation() 
{ 
    //TODO: move values to C++ and store in header to allow changes
    float constAtten = 0.0f; 
    float linearAtten = 0.2f; 
    float quadraticAtten = 0.002f;
    
    float distance = length(gl_FragCoord.xy - point);
    float distanceSquared = distance * distance;
    float attenVal = 200.f;
    // return clamp(1.f / (constAtten + (linearAtten * distance) + (quadraticAtten * distanceSquared)), 0.0f, 1.0f);
    return clamp ((1.f / (distance + distanceSquared)) * attenVal , 0.0, 1.f);
}

void main(void)
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec4 col = vec4(vertColor);
    
    col.xyz += pixel.xyz;
    col.w *= calculateAttenuation();
    col.w + pixel.w;
    
    gl_FragColor = col;
}
