#include <ShaderLibrary.h>

void ShaderLibrary::AddShader(const GLchar* vertexPath, const GLchar* fragmentPath, ShaderLibrary::SHADER_TYPE type)
{
	GLuint Program;
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (std::ifstream::failure e)
	{
		std::cout << "ERROR:SHADER:FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// Shader Program
	GLuint programm = glCreateProgram();
	glAttachShader(programm, vertex);
	glAttachShader(programm, fragment);
	glLinkProgram(programm);

	// Print linking errors if any
	glGetProgramiv(programm, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programm, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}

	m_shaders.insert(std::pair<ShaderLibrary::SHADER_TYPE, GLuint>(type, programm));

	// Delete the shaders as they're linked into our program now and no
	//longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void ShaderLibrary::Use(ShaderLibrary::SHADER_TYPE type)
{
	// Find in the map the matching shader type.
	// TODO: Maps arent guaranteed to haev one value
	glUseProgram(std::find_if(m_shaders.cbegin(), m_shaders.cend(), [type](const auto& m) { return m.first == type; })->second);
}

GLuint ShaderLibrary::GetProgramID(ShaderLibrary::SHADER_TYPE type)
{
	// TODO: Maps arent guaranteed to haev one value
	return std::find_if(m_shaders.cbegin(), m_shaders.cend(), [type](const auto& m) { return m.first == type; })->second;
}
