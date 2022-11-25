#include "Shader.h"

void Shader::setName(std::string name) {
    _name = name;
}

std::string& Shader::getName() {
    return _name;
}

void Shader::setVShader(std::string pathvShader) {
    _vShader = pathvShader;
}

const std::string& Shader::getVShader() {
    return _vShader;
}

void Shader::setFShader(std::string pathfShader) {
    _fShader = pathfShader;
}

const std::string& Shader::getFShader() {
    return _fShader;
}


GLuint& Shader::getShaderProgram() {
    return _shaderProgram;
}

bool Shader::readShader(const std::string& path, std::string& shader) {
    bool rv = true;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shader = shaderStream.str();
    }

    catch (std::ifstream::failure error) {
        rv = false;
        std::cout << "ERRORE DI APERTURA FILE DELLO SHADER ->" << shader; // AGGIUNGERE QUALE SHADER
    }
    return rv;

}

bool Shader::buildMaterial()
{

    bool rv = true;

    std::ifstream f;


    glUseProgram(0); //lo metto perchè ignoro tutto ciò che è precedente, 

                     //tutte le chiamate opengl fatte qui non possono rischiare di riferirsi a un program vecchio (lo 0 è come se fosse "NULL")

    f.open(getVShader());
    bool vShaderExist = f.good();
    f.close();

    f.open(getFShader());
    bool fShaderExist = f.good();
    f.close();


    if (vShaderExist && fShaderExist) {

        rv = buildMaterial(getVShader(), getFShader());
    }
    else {
        std::cout << "NON ESISTONO GLI SHADER DI " << _name << std::endl;
        rv = false;
    }


    return rv;
}


bool Shader::buildMaterial(const std::string& vShaderPath, const std::string& fShaderPath)
{
    bool rv = false;
    if (readShader(vShaderPath, _vShader) && readShader(fShaderPath, _fShader)) {
        GLuint vShader;
        GLuint fShader;
        if (compile(getVShader(), GL_VERTEX_SHADER, vShader) && compile(getFShader(), GL_FRAGMENT_SHADER, fShader)) {
            if (link(vShader, fShader)) {

                glUseProgram(_shaderProgram); // momentaneo
                glDeleteShader(vShader);
                glDeleteShader(fShader);
                rv = true;
            }
        }
    }
    return rv;
}


bool Shader::compile(const std::string& shaderCode, const GLenum& shaderType, GLuint& shader) {
    bool rv = true;
    const GLchar* shaderCodeChar = shaderCode.c_str();

    GLchar infoLog[512];
    GLint success;

    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCodeChar, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl; // non è solo fragment, quello è da cambiare
        rv = false;
    }
    return rv;
}


bool Shader::link(const GLuint& vShader, const GLuint& fShader) {
    bool rv = true;

    GLchar infoLog[512];
    GLint success;

    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, vShader);
    glAttachShader(_shaderProgram, fShader);
    glLinkProgram(_shaderProgram);

    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        rv = false;
    }
    return rv;
}

void Shader::draw() {
    glUseProgram(_shaderProgram);
}

void Shader::processShader() {
    
    setVShader("color.vert");
    setFShader("color.frag");
    /*std::cout << "path1 " << bg.getVShader();
    std::cout << "path2 " << bg.getFShader();*/

    buildMaterial();
    std::cout << "Vertex shader " << getVShader() << std::endl;
    std::cout << "Fragment shader " << getFShader() << std::endl;

}

