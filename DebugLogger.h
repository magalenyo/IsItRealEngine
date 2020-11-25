#pragma once

#include "Globals.h"
#include "GL/glew.h"

// This class prepares filters the error from GLEW and logs it
class DebugLogger {

public:

	// This function calls the LOG function with the formatted error from GLEW
	static void __stdcall GLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		const char* tmp_source = "", * tmp_type = "", * tmp_severity = "";
		switch (source) {
		case GL_DEBUG_SOURCE_API: tmp_source = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: tmp_source = "Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: tmp_source = "Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: tmp_source = "Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION: tmp_source = "Application"; break;
		case GL_DEBUG_SOURCE_OTHER: tmp_source = "Other"; break;
		};
		switch (type) {
		case GL_DEBUG_TYPE_ERROR: tmp_type = "Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tmp_type = "Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: tmp_type = "Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY: tmp_type = "Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: tmp_type = "Performance"; break;
		case GL_DEBUG_TYPE_MARKER: tmp_type = "Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP: tmp_type = "Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP: tmp_type = "Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER: tmp_type = "Other"; break;
		};
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH: tmp_severity = "high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: tmp_severity = "medium"; break;
		case GL_DEBUG_SEVERITY_LOW: tmp_severity = "low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
		};

		//if (!isIdExcluded(id)) {
			LOG("<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);
		//}
	}

	// This function calls the LOG function with the step from ASSIMP
	static void AssimpErrorFunction(const char* msg, char* userData)
	{
		LOG(msg);
	}

private:
	static const int GL_ID_NOTIFICATION_USINGVIDEOMEMORY = 131185;		// The id of the "Buffer object X (bound to GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations"

	// returns true if idToScan is one of the IDS to exclude (not show) in the log
	static bool isIdExcluded(int idToScan) {
		bool result = false;
		if (idToScan == GL_ID_NOTIFICATION_USINGVIDEOMEMORY) {
			result = true;
		}
		return result;
	}
};