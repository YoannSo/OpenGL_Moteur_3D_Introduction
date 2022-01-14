#version 450

layout( location = 0 ) out vec4 fragColor;

layout( binding = 1 ) uniform sampler2D uDiffuseMap; // deux Sampler pour faire marcher nos texture
layout( binding = 2 ) uniform sampler2D uSpecularMap;

in vec3 normalizedNormal;
in vec3 vertexPosOut;
in vec2 VertexTexCoords;
in vec3 LightPos;

uniform vec3 ambientColor;
uniform vec3 difusColor;
uniform vec3 speculaireColor;
uniform vec3 cameraPosition;
uniform float shininess;

// nouvelle variable uniform qui vont nous dire si on a certainne map
uniform bool _hasDiffuseMap;
uniform bool _hasSpecularMap;



void main(){
	vec3 surfaceColorForDifus=difusColor;
	vec3 surfaceColorForSpecular=speculaireColor;

	// si on a certainne map alors on les utilises
	if(_hasDiffuseMap){
		surfaceColorForDifus=  vec3 (texture(uDiffuseMap,VertexTexCoords));
	}
	if(_hasSpecularMap){
		surfaceColorForSpecular=  vec3 (texture(uSpecularMap,VertexTexCoords));
	}
	
	// meme fonctionnement qu'avant

	vec3 Li = normalize( LightPos - vertexPosOut );
	vec3 H = reflect(-Li, normalizedNormal);
	
	vec3 lum_ambient = ambientColor;
	vec3 lum_difus = max ( dot( normalizedNormal ,  Li ), 0.f ) * surfaceColorForDifus;
	vec3 lum_speculaire = pow( max( dot( normalize( - vertexPosOut), H), 0.f), 1000)* surfaceColorForSpecular;


	vec3 finalColor=lum_ambient+lum_difus+lum_speculaire;

	fragColor = vec4( finalColor, 1.f );
}
