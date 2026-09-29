#pragma
#include <glm/glm.hpp>

class Scene;
class Mesh;
class Camera;
class Texture;


class Editor {
public:
	int selected = -1;
	float throwSpeed = 12.0f;
	glm::vec3 throwColor = glm::vec3(1.0f, 0.5f, 0.31f);
	int throwTex = 0;

	void draw(Scene& scene, const char* texNames[], int texCount);
	void spawn(Scene& scene, Camera& cam, Mesh& cube, Texture* textures[]);

};