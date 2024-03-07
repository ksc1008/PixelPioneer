#include "../Graphics//graphics.h"
#include "../Graphics/texturepack.h"

int main(int argc, char** argv) {
	initiateGL();
	TexturepackRepository::getInstance()->addTexturepack(*testManifest());
	openWindow();

}