#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <glad/glad.h>
#include<GLFW/glfw3.h>
class Shader
{
private:
	std::string _name = "";
	std::string _vShader = "";
	std::string _fShader = "";
	GLuint _shaderProgram = -1; //---------ho cambiato il nome

public:

	const static unsigned int maxLightsNumber = 3;

	Shader() {};
	//Material(const Material& material){}; // senza parentesi {} da errore
	~Shader() {

	}
	void processShader();
	void setName(std::string name);
	std::string& getName();
	void setVShader(std::string vShader);
	const std::string& getVShader();
	void setFShader(std::string fShader);
	const std::string& getFShader();
	GLuint getShaderProgram();

	bool buildMaterial(const std::string& vShaderPath, const std::string& fShaderPath);
	bool compile(const std::string& shaderCode, const GLenum& shaderType, GLuint& shader);
	bool link(const GLuint& vShader, const GLuint& fShader);

	void clean();
	bool readShader(const std::string& path, std::string& shader);
	bool buildMaterial();

	void draw();

};

