/*
 * Shader.cpp
 *
 *  Created on: 27.08.2016
 *      Author: wera
 */

#include "Shader.h"

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <string.h>

#include <cassert>
#include <fstream>


#include "../maths/Matrix4.h"
#include "../maths/Vector4.h"



using namespace opengl_utility;


typedef unsigned int uint;


	//CShaderProgram::CShaderProgram()
	//{
	//	SetDefaults();
	//}
	//
	//CShaderProgram::~CShaderProgram()
	//{
	//}
	//
	//CShaderProgram::operator GLuint ()
	//{
	//	return Program;
	//}
	//
	//bool CShaderProgram::Load(char *VertexShaderFileName, char *FragmentShaderFileName)
	//{
	//	bool Error = false;
	//
	//	Destroy();
	//
	//	Error |= ((VertexShader = LoadShader(VertexShaderFileName, GL_VERTEX_SHADER)) == 0);
	//	Error |= ((FragmentShader = LoadShader(FragmentShaderFileName, GL_FRAGMENT_SHADER)) == 0);
	//
	//	if(Error)
	//	{
	//		Destroy();
	//		return false;
	//	}
	//
	//	Program = glCreateProgram();
	//	glAttachShader(Program, VertexShader);
	//	glAttachShader(Program, FragmentShader);
	//	glLinkProgram(Program);
	//
	//	int LinkStatus;
	//	glGetProgramiv(Program, GL_LINK_STATUS, &LinkStatus);
	//
	//	if(LinkStatus == GL_FALSE)
	//	{
	//		//ErrorLog.Append("Error linking program (%s, %s)!\r\n", VertexShaderFileName, FragmentShaderFileName);
	//
	//		int InfoLogLength = 0;
	//		glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	//
	//		if(InfoLogLength > 0)
	//		{
	//			char *InfoLog = new char[InfoLogLength];
	//			int CharsWritten  = 0;
	//			glGetProgramInfoLog(Program, InfoLogLength, &CharsWritten, InfoLog);
	//			//ErrorLog.Append(InfoLog);
	//			delete [] InfoLog;
	//		}
	//
	//		Destroy();
	//
	//		return false;
	//	}
	//
	//	return true;
	//}
	//
	//void CShaderProgram::Destroy()
	//{
	//	glDetachShader(Program, VertexShader);
	//	glDetachShader(Program, FragmentShader);
	//
	//	glDeleteShader(VertexShader);
	//	glDeleteShader(FragmentShader);
	//
	//	glDeleteProgram(Program);
	//
	//	delete [] UniformLocations;
	//	delete [] AttribLocations;
	//
	//	SetDefaults();
	//}
	//
	//GLuint CShaderProgram::LoadShader(char *FileName, GLenum Type)
	//{
	//	std::string DirectoryFileName = FileName;
	//
	//	FILE *File;
	//
	//	if((File = fopen( DirectoryFileName.c_str() , "rb")) == NULL)
	//	{
	//		//ErrorLog.Append("Error loading file " + DirectoryFileName + "!\r\n");
	//		return 0;
	//	}
	//
	//	fseek(File, 0, SEEK_END);
	//	long Size = ftell(File);
	//	fseek(File, 0, SEEK_SET);
	//	char *Source = new char[Size + 1];
	//	fread(Source, 1, Size, File);
	//	fclose(File);
	//	Source[Size] = 0;
	//
	//	GLuint Shader = glCreateShader(Type);
	//
	//	glShaderSource(Shader, 1, (const char**)&Source, NULL);
	//	delete [] Source;
	//	glCompileShader(Shader);
	//
	//	int CompileStatus;
	//	glGetShaderiv(Shader, GL_COMPILE_STATUS, &CompileStatus);
	//
	//	if(CompileStatus == GL_FALSE)
	//	{
	//		//ErrorLog.Append("Error compiling shader %s!\r\n", FileName);
	//
	//		int InfoLogLength = 0;
	//		glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	//
	//		if(InfoLogLength > 0)
	//		{
	//			char *InfoLog = new char[InfoLogLength];
	//			int CharsWritten  = 0;
	//			glGetShaderInfoLog(Shader, InfoLogLength, &CharsWritten, InfoLog);
	//			//ErrorLog.Append(InfoLog);
	//			delete [] InfoLog;
	//		}
	//
	//		glDeleteShader(Shader);
	//
	//		return 0;
	//	}
	//
	//	return Shader;
	//}
	//
	//void CShaderProgram::SetDefaults()
	//{
	//	VertexShader = 0;
	//	FragmentShader = 0;
	//
	//	Program = 0;
	//
	//	UniformLocations = NULL;
	//	AttribLocations = NULL;
	//}


// Constructor
Shader::Shader() : mProgramObjectID(0)
{

}

// Constructor with arguments
Shader::Shader(const std::string vertexShaderFilename, const std::string fragmentShaderFilename)
    : mProgramObjectID(0)
{

    // Create the shader
    create(vertexShaderFilename, fragmentShaderFilename);
}

// Destructor
Shader::~Shader()
{

}

// Create the shader
bool Shader::create(const std::string vertexShaderFilename,
                    const std::string fragmentShaderFilename)
{

    // Set the shader filenames
    mFilenameVertexShader = vertexShaderFilename;
    mFilenameFragmentShader = fragmentShaderFilename;

    // Check that the needed OpenGL extensions are available
    bool isExtensionOK = checkOpenGLExtensions();
    if (!isExtensionOK)
    {
       //cerr << "Error : Impossible to use GLSL vertex or fragment shaders on this platform" << endl;
       assert(false);
       return false;
    }

    // Delete the current shader
    destroy();

    assert(!vertexShaderFilename.empty() && !fragmentShaderFilename.empty());

    // ------------------- Load the vertex shader ------------------- //
    GLuint vertexShaderID;
    std::ifstream fileVertexShader;
    fileVertexShader.open(vertexShaderFilename.c_str(), std::ios::binary);
    assert(fileVertexShader.is_open());

    if (fileVertexShader.is_open()) {

        // Get the size of the file
        fileVertexShader.seekg(0, std::ios::end);
        uint fileSize = (uint) (fileVertexShader.tellg());
        assert(fileSize != 0);

        // Read the file
        fileVertexShader.seekg(std::ios::beg);
        char* bufferVertexShader = new char[fileSize + 1];
        fileVertexShader.read(bufferVertexShader, fileSize);
        fileVertexShader.close();
        bufferVertexShader[fileSize] = '\0';

        // Create the OpenGL vertex shader and compile it
        vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        assert(vertexShaderID != 0);
        glShaderSource(vertexShaderID, 1, (const char **) (&bufferVertexShader), NULL);
        glCompileShader(vertexShaderID);
        delete[] bufferVertexShader;

        // Get the compilation information
        int compiled;
        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compiled);

        // If the compilation failed
        if (compiled == 0) {

            // Get the log of the compilation
            int lengthLog;
            glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &lengthLog);
            char* str = new char[lengthLog];
            glGetShaderInfoLog(vertexShaderID, lengthLog, NULL, str);

            // Display the log of the compilation
            std::cerr << "Vertex Shader Error : " << str << std::endl;
            delete[] str;
            assert(false);
            return false;
        }
    }
    else {
        std::cerr << "Error : Impossible to open the vertex shader file " <<
                     vertexShaderFilename << std::endl;
        assert(false);
        return false;
    }

    // ------------------- Load the fragment shader ------------------- //
    GLuint fragmentShaderID;
    std::ifstream fileFragmentShader;
    fileFragmentShader.open(fragmentShaderFilename.c_str(), std::ios::binary);
    assert(fileFragmentShader.is_open());

    if (fileFragmentShader.is_open()) {

        // Get the size of the file
        fileFragmentShader.seekg(0, std::ios::end);
        uint fileSize = (uint) (fileFragmentShader.tellg());
        assert(fileSize != 0);

        // Read the file
        fileFragmentShader.seekg(std::ios::beg);
        char* bufferFragmentShader = new char[fileSize + 1];
        fileFragmentShader.read(bufferFragmentShader, fileSize);
        fileFragmentShader.close();
        bufferFragmentShader[fileSize] = '\0';

        // Create the OpenGL fragment shader and compile it
        fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        assert(fragmentShaderID != 0);
        glShaderSource(fragmentShaderID, 1, (const char **) (&bufferFragmentShader), NULL);
        glCompileShader(fragmentShaderID);
        delete[] bufferFragmentShader;

        // Get the compilation information
        int compiled;
        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compiled);

        // If the compilation failed
        if (compiled == 0) {

            // Get the log of the compilation
            int lengthLog;
            glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &lengthLog);
            char* str = new char[lengthLog];
            glGetShaderInfoLog(fragmentShaderID, lengthLog, NULL, str);

            // Display the log of the compilation
            std::cerr << "Fragment Shader Error : " << str << std::endl;
            delete[] str;
            assert(false);
            return false;
        }
    }
    else {
        std::cerr << "Error : Impossible to open the fragment shader file " <<
                     fragmentShaderFilename << std::endl;
        assert(false);
        return false;
    }

    // Create the shader program and attach the shaders
    mProgramObjectID = glCreateProgram();
    assert(mProgramObjectID);
    glAttachShader(mProgramObjectID, vertexShaderID);
    glAttachShader(mProgramObjectID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    // Try to link the program
    glLinkProgram(mProgramObjectID);
    int linked;
    glGetProgramiv(mProgramObjectID, GL_LINK_STATUS, &linked);
    if (!linked) {
        int logLength;
        glGetProgramiv(mProgramObjectID, GL_INFO_LOG_LENGTH, &logLength);
        char* strLog = new char[logLength];
        glGetProgramInfoLog(mProgramObjectID, logLength, NULL, strLog);
//        cerr << "Linker Error in vertex shader " << vertexShaderFilename <<
//                " or in fragment shader " << fragmentShaderFilename << " : " << strLog << endl;
        delete[] strLog;
        destroy();
        assert(false);
    }

    return true;
}

// Set a vector 4 uniform value to this shader (be careful if the uniform is not
// used in the shader, the compiler will remove it, then when you will try
// to set it, an assert will occur)
void Shader::setUniformValue(const std::string& variableName, const Vector4& v) const
{
    assert(mProgramObjectID != 0);
    glUniform4f(getUniformLocation(variableName), v.x, v.y, v.z, v.w);
}


// Set a 4x4 matrix uniform value to this shader (be careful if the uniform is not
// used in the shader, the compiler will remove it, then when you will try
// to set it, an assert will occur)
void Shader::setUniformValue(const std::string& variableName, const Matrix4& matrix , bool transpose ) const
{
	assert(mProgramObjectID != 0);
	GLfloat mat[16];
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			mat[i*4 + j] = matrix.m[i][j];
		}
	}
	glUniformMatrix4fv(getUniformLocation(variableName), 1, transpose , mat);
}


