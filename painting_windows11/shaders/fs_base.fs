#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D Texture0;
uniform vec4 Color;

void main() 
{
   vec4 tmpColor = texture(Texture0, texCoord);

   if (tmpColor.a < 0.05)
        discard;   
   if(Color.r < 0)
      tmpColor.r = tmpColor.g = tmpColor.b = 0.2126 * tmpColor.r + 0.7152 * tmpColor.g + 0.0722 * tmpColor.b;
   else
      tmpColor = tmpColor*Color;
   FragColor = tmpColor;
}