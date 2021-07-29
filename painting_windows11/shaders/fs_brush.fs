#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D texture0;
uniform float bright;

void main() 
{
   vec4 tmpColor = texture(texture0, texCoord);
   if(tmpColor.a < 0.05)
        discard;   
   FragColor = vec4(tmpColor.rgb * bright, 0.6);
}