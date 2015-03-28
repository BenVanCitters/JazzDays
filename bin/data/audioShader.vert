attribute vec4 position;

uniform float time;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
attribute float texCoordX;
attribute float texCoordY;
varying float colX;
varying float colY;
void main()
{
	vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;//projectionMatrix * modelViewMatrix * position;
//	pos += vec4(0,audioLevel,0,0);
//	col = time;
	//pos.y = pos.y + 2 * audioLevel;
    colX = texCoordX;
    colY = texCoordY;
	gl_Position = pos;
}
