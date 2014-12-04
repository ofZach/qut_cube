
uniform mat4 modelViewProjectionMatrix;

attribute vec4 position;
attribute vec4 color;
attribute vec4 normal;
attribute vec2 texcoord;

varying vec2 texCoordVarying;

void main()
{
	texCoordVarying = texcoord;
	gl_Position = modelViewProjectionMatrix * position;
}