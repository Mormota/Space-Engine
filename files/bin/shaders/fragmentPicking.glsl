#version 330 core

out vec4 color;

in vec4 pColor;

void main(){

    color = vec4(pColor);

}