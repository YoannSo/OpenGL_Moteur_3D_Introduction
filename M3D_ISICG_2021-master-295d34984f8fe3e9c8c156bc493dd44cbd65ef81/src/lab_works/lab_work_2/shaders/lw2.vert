#version 450
		layout( location = 0) in vec2 aVertexPosition;
		layout(location=1) in vec3 colorVBO;
		uniform float uTranslationX; // variable qui va nous permettre la translation 
		out vec3 colorToFragment; // variable qu'on va envoyer au fragment shader pour la couleur

void main() {

	 colorToFragment=colorVBO;
	gl_Position= vec4(aVertexPosition.x+uTranslationX,aVertexPosition.y,0.f , 1.f); // creation de la position en fonction de la translation en x
}
