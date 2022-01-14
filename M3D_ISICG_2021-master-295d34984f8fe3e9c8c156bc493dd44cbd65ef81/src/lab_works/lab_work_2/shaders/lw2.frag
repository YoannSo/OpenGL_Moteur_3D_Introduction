#version 450

	layout( location = 0) out vec4 fragColor;
	in vec3 colorToFragment;
	uniform float uLuminosity; // on recupere la luminosité
void main() {
	// on affecte donc la couleur grace au vbo qu'on a recuperer au vertex, et on multiplie par la luminosité
	fragColor= vec4(colorToFragment.x*uLuminosity,colorToFragment.y*uLuminosity,colorToFragment.z*uLuminosity,1.f);
}
