#version 450

layout( location = 0 ) out vec4 fragColor;

// on recupere nos normal et nos positions
in vec3 normalizedNormal;
in vec3 vertexPosOut;
in vec3 LightPos;

// on recupere toutes nos variable pour calculer la lumiere
uniform vec3 ambientColor;
uniform vec3 difusColor;
uniform vec3 speculaireColor;
uniform float shininess;


void main(){
	
	// deux variable qui vont nous servir, Li va etre la direction de la lumiere, et H la direction de reflection
	vec3 Li = normalize( LightPos - vertexPosOut );	//vecteur de la lumière émise
	vec3 H = reflect(-Li, normalizedNormal);
	
	vec3 lum_ambient = ambientColor; // l'eclairage ambient serra siplement ambientColor

	//calcul grace a https://opengl.developpez.com/
	vec3 lum_difus= max ( dot( normalizedNormal ,  Li ), 0.f ) * difusColor;
	vec3 lum_speculaire = pow( max( dot( normalize( - vertexPosOut), H), 0.f), shininess)* speculaireColor;


	vec3 finalColor=lum_ambient+lum_difus+lum_speculaire; // on fait la somme des 3 lumieres pour avoir celle finale

	fragColor = vec4( finalColor, 1.f );
}
