#version 120

uniform sampler2D tex;
varying vec2 texture_coordinate;
void main (void) {
    
    gl_FragColor = texture2D(tex, texture_coordinate.st);
    
}


