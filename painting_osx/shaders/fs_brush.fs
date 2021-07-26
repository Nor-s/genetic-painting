#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D texture0;

void main() 
{
   vec4 tmpColor = texture(texture0, texCoord);
   if(tmpColor.r < 0.1 && tmpColor.g < 0.1 && tmpColor.b < 0.1) {
      discard;
   }
   
   FragColor = tmpColor;
}