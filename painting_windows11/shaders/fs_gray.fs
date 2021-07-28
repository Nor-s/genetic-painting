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
   float average = 0.2126 * tmpColor.r + 0.7152 * tmpColor.g + 0.0722 * tmpColor.b;
   tmpColor.r = average;
   FragColor = vec4(tmpColor.rrr * bright, 1);
}