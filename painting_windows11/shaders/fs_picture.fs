#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D Texture0;
uniform float Bright;
uniform vec4 Color;

void main() 
{
   vec4 tmpColor = texture(Texture0, texCoord)*Color;
   if(tmpColor.a < 0.05)
        discard;   
   FragColor = vec4(tmpColor.rgb * bright, 1);
}