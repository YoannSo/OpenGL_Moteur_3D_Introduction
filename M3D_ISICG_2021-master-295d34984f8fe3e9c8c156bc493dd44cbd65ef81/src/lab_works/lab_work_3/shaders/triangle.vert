#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexColor;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 uProjectionMatrix;

// MATRIX M V P => P*V*M pour aovir la position

out vec3 color;

void main()
{
	color = aVertexColor;

	gl_Position =uProjectionMatrix* viewMatrix*modelMatrix*vec4( aVertexPosition, 1.f );
}
