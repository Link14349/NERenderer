#ifndef __SHADER_H_
#define __SHADER_H_

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	Shader(const char* vsfn, const char* fsfn) {
		std::ifstream vsfile, fsfile;
		vsfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fsfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vsfile.open(vsfn);
			fsfile.open(fsfn);
		} catch (std::ifstream::failure e)
		{
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		std::ostringstream vsss, fsss;
		vsss << vsfile.rdbuf();
		fsss << fsfile.rdbuf();
		vsfile.close();
		fsfile.close();
		std::string fsc(fsss.str()), vsc(vsss.str());
		const char* fss = fsc.c_str();
		const char* vss = vsc.c_str();
		char infoLog[512];

		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vss, NULL);
		glCompileShader(vs);
		// check for shader compile errors
		glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vs, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			return;
		}
		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fss, NULL);
		glCompileShader(fs);
		// check for shader compile errors
		glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fs, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			return;
		}
		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vs);
		glAttachShader(shaderProgram, fs);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			return;
		}
		glDeleteShader(vs);
		glDeleteShader(fs);
	}
	int ifSuccess() const { return success; }
	void use() const {
		glUseProgram(shaderProgram);
	}
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }
    void setFloat2(const std::string &name, float a, float b) const {
        glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), a, b);
    }
    void setFloat3(const std::string &name, float a, float b, float c) const {
        glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), a, b, c);
    }
    void setFloat4(const std::string &name, float a, float b, float c, float d) const {
        glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), a, b, c, d);
    }
    void setMatrix4fv(const std::string &name, const glm::mat4& mat4) const {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
    }
    ~Shader() {
		glDeleteProgram(shaderProgram);
	}
private:
	unsigned int shaderProgram;
	int success;
};

#endif