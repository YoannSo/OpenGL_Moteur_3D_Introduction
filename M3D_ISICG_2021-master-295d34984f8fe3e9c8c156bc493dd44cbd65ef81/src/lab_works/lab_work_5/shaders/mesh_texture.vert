#version 450

// tous nos attribut dans le vao
layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;

// nos variable qui vont etre utilise pour le fragment shader
out vec3 normalizedNormal; // vecteur normal normalisé
out vec3 vertexPosOut; // vecteur position
out vec3 lightPos; // position de la lumiere
out vec2 VertexTexCoords;

// les uniform qu'on recupere
uniform mat4 uMVMatrix; // MODEL VIEW MATRIX
uniform mat4 uMVPMatrix; // MODEL VIEW PROJECTION MATRIX
uniform mat4 uViewMatrix ; // VIEW MATRIX
uniform mat4 uModelMatrix  ; // MODEL MATRIX
uniform mat3 uNormalMatrix; // NORMAL MATRIX
uniform vec3 cameraPosition; // Position de la camera

void main()
{	
	VertexTexCoords=aVertexTexCoords; // pour envoyer au fragment shader


	// mem fonctionnement
	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );

	normalizedNormal = normalize( uNormalMatrix * aVertexNormal ); 

	vertexPosOut = vec3( uMVMatrix*vec4(aVertexPosition, 1.f) );
	lightPos = vec3(uViewMatrix * vec4(cameraPosition, 1.0)); 
	
	vec3 Li = normalize( - vertexPosOut );
    if( dot(Li, normalizedNormal) < 0 ){
        normalizedNormal *= -1;
    }
}
