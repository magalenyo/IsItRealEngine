#pragma once
class Scene
{
public:
	Scene();
	~Scene();

private:
	unsigned int vao;
	unsigned int fbo;
	unsigned int texture;
	//float quadVertices[] = {
	//	// positions   // texCoords
	//	-1.0f,  1.0f,  0.0f, 1.0f,
	//	-1.0f, -1.0f,  0.0f, 0.0f,
	//	 1.0f, -1.0f,  1.0f, 0.0f,

	//	-1.0f,  1.0f,  0.0f, 1.0f,
	//	 1.0f, -1.0f,  1.0f, 0.0f,
	//	 1.0f,  1.0f,  1.0f, 1.0f
	//};

	void CreateVAO();
	void CreateFBO();
	void CreateTexture();
	void DeleteBuffer();


};

