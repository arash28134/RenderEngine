#include "programs/TreeProgram.h"

#include "WorldConfig.h"

#include <iostream>

const std::string Engine::TreeProgram::PROGRAM_NAME = "TreeProgram";

const unsigned long long Engine::TreeProgram::SHADOW_MAP = 0x01;
const unsigned long long Engine::TreeProgram::WIRE_MODE = 0x02;

Engine::TreeProgram::TreeProgram(std::string name, unsigned long long params)
	:Program(name, params)
{
	vShaderFile = "shaders/vegetation/tree/tree.vert";
	fShaderFile = "shaders/vegetation/tree/tree.frag";
	gShaderFile = "shaders/vegetation/tree/tree.geom";
}

Engine::TreeProgram::TreeProgram(const TreeProgram & other)
	:Program(other)
{
	uModelViewProj = other.uModelViewProj;
	uModelView = other.uModelView;
	uNormal = other.uNormal;
	uLightDepthMat = other.uLightDepthMat;
	uGridUV = other.uGridUV;
	uAmplitude = other.uAmplitude;
	uFrecuency = other.uFrecuency;
	uScale = other.uScale;
	uOctaves = other.uOctaves;
	uLightDir = other.uLightDir;
	uWaterLevel = other.uWaterLevel;

	uInPos = other.uInPos;
	uInColor = other.uInColor;
	uInNormal = other.uInNormal;
	uInEmissive = other.uInEmissive;
}

void Engine::TreeProgram::initialize()
{
	std::string config = "";

	if (parameters & Engine::TreeProgram::SHADOW_MAP)
	{
		config += "#define SHADOW_MAP";
	}

	if (parameters & Engine::TreeProgram::WIRE_MODE)
	{
		config += "#define WIRE_MODE";
	}

	vShader = loadShader(vShaderFile, GL_VERTEX_SHADER, config);
	gShader = loadShader(gShaderFile, GL_GEOMETRY_SHADER, config);
	fShader = loadShader(fShaderFile, GL_FRAGMENT_SHADER, config);

	glProgram = glCreateProgram();

	glAttachShader(glProgram, vShader);
	glAttachShader(glProgram, gShader);
	glAttachShader(glProgram, fShader);

	glLinkProgram(glProgram);

	int linked;
	glGetProgramiv(glProgram, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(glProgram, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(glProgram, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}

	configureProgram();
}

void Engine::TreeProgram::configureProgram()
{
	uModelViewProj = glGetUniformLocation(glProgram, "modelViewProj");
	uModelView = glGetUniformLocation(glProgram, "modelView");
	uNormal = glGetUniformLocation(glProgram, "normal");
	uGridUV = glGetUniformLocation(glProgram, "tileUV");
	uAmplitude = glGetUniformLocation(glProgram, "amplitude");
	uFrecuency = glGetUniformLocation(glProgram, "frecuency");
	uScale = glGetUniformLocation(glProgram, "scale");
	uOctaves = glGetUniformLocation(glProgram, "octaves");
	uLightDepthMat = glGetUniformLocation(glProgram, "lightDepthMat");
	uLightDir = glGetUniformLocation(glProgram, "lightDir");
	uWaterLevel = glGetUniformLocation(glProgram, "waterHeight");

	uInPos = glGetAttribLocation(glProgram, "inPos");
	uInColor = glGetAttribLocation(glProgram, "inColor");
	uInNormal = glGetAttribLocation(glProgram, "inNormal");
	uInEmissive = glGetAttribLocation(glProgram, "inEmissive");
}

void Engine::TreeProgram::configureMeshBuffers(Mesh * mesh)
{
	glBindVertexArray(mesh->vao);

	if (uInPos != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertices);
		glVertexAttribPointer(uInPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uInPos);
	}

	if (uInColor != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboColors);
		glVertexAttribPointer(uInColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uInColor);
	}

	if (uInNormal != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboNormals);
		glVertexAttribPointer(uInNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uInNormal);
	}

	if (uInEmissive != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboEmission);
		glVertexAttribPointer(uInEmissive, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uInEmissive);
	}
}

void Engine::TreeProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	glm::mat4 modelView = view * obj->getModelMatrix();
	glm::mat4 modelViewProj = proj * modelView;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	glUniformMatrix4fv(uModelViewProj, 1, GL_FALSE, &(modelViewProj[0][0]));
	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &(modelView[0][0]));
	glUniformMatrix4fv(uNormal, 1, GL_FALSE, &(normal[0][0]));

	glUniform1f(uAmplitude, Engine::Settings::terrainAmplitude);
	glUniform1f(uFrecuency, Engine::Settings::terrainFrecuency);
	glUniform1f(uScale, Engine::Settings::terrainScale);
	glUniform1i(uOctaves, Engine::Settings::terrainOctaves);
	glUniform1f(uWaterLevel, Engine::Settings::waterHeight);
}

void Engine::TreeProgram::setUniformTileUV(float u, float v)
{
	glUniform2f(uGridUV, u, v);
}

void Engine::TreeProgram::setUniformLightDepthMat(const glm::mat4 & ldm)
{
	glUniformMatrix4fv(uLightDepthMat, 1, GL_FALSE, &(ldm[0][0]));
}

void Engine::TreeProgram::setUniformLightDir(const glm::vec3 & ld)
{
	glUniform3fv(uLightDir, 1, &ld[0]);
}

void Engine::TreeProgram::setUniformDepthMap(Engine::TextureInstance * ti)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ti->getTexture()->getTextureId());
	glUniform1i(uDepthMap, 0);
}

void Engine::TreeProgram::destroy()
{
	glDetachShader(glProgram, gShader);
	glDeleteShader(gShader);

	Engine::Program::destroy();
}
// ===========================================================================================


Engine::Program * Engine::TreeProgramFactory::createProgram(unsigned long long params)
{
	Engine::TreeProgram * tp = new Engine::TreeProgram(Engine::TreeProgram::PROGRAM_NAME, params);
	tp->initialize();
	return tp;
}