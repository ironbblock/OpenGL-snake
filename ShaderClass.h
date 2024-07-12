#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
class Shader {
public:
	unsigned int ID;
	Shader(const char* vPath, const char* fPath) {
		string vCode,fCode;
		ifstream vFile,fFile;
		stringstream vSS,fSS;
		vFile.open(vPath);
		fFile.open(fPath);
		vSS << vFile.rdbuf();
		fSS << fFile.rdbuf();
		vFile.close();
		fFile.close();
		vCode = vSS.str();
		fCode = fSS.str();
		const char* vShaderCode = vCode.c_str();
		const char* fShaderCode = fCode.c_str();
		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	void use(){
		glUseProgram(ID);
	}
};
#endif
