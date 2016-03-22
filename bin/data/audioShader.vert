#version 120
varying vec2 texture_coordinate;
void main()
{
    
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    texture_coordinate = vec2(gl_MultiTexCoord0);
}
