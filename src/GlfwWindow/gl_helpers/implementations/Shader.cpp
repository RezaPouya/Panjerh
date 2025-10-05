#include "../Shader.h"



Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	/*std::ifstream vShaderFile;
	std::ifstream fShaderFile;*/

	auto vertexCode = Shader::ReadShaderFile(vertexPath);
	auto fragmentCode = Shader::ReadShaderFile(fragmentPath);

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	GLuint vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Active()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

std::string Shader::ReadShaderFile(const std::string  filePath) {
	// Check if file exists first
	if (!std::filesystem::exists(filePath)) {
		auto abPath = std::filesystem::absolute(filePath);
		throw std::runtime_error("File not found: " + filePath + "\n absolute file path: " + abPath.string());
	}

	std::ifstream in(filePath, std::ios::binary | std::ios::ate);
	if (!in) {
		throw std::runtime_error("Cannot open file: " + static_cast<std::string>(filePath));
	}

	in.seekg(0, std::ios::end); // go to end of file to Read file size
	std::string content(in.tellg(), '\0'); // read file size 
	in.seekg(0, std::ios::beg); // return to start of file 
	in.read(content.data(), content.size());

	return content;
}


void Shader::checkCompileErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

const unsigned int Shader::GetId() const {
	return ID;
}