#include <sb7.h>
#include <vmath.h>

class my_applicaiton : public sb7::application
{
public:
	GLuint compile_shaders(void) {
		GLuint vertex_shader;
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);

		GLuint fragment_shader;
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* vertex_shader_source[] = {
			"#version 430 core\n"
			"layout (location = 0) in vec4 offset\n"
			"void main(void)\n"
			"{\n"
			"    const vec4 vertices[12] = vec4[12](vec4(0.25, -0.25, 0.5, 1.0)\n"
			"									    vec4(-0.25, 0.25, 0.5, 1.0)\n"
			"                                       vec4(0.25, 0.25, 0.5, 1.0))\n"									    
			"    gl_Position = vertices[gl_VertexID] + offset;\n"
			"}\n"
		};


		const GLchar* fragment_shader_source[] = {
			"#version 430 core\n"
			"out vec4 color;\n"
			"void main(void)\n"
			"{\n"
			"    color = vec4(0.53, 0.81, 0.92, 1.0);\n"
			"}\n"
		};

		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(vertex_shader);
		glCompileShader(fragment_shader);
		GLuint program;
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		return program;
	}
	virtual void startup()
	{
		rendering_program = compile_shaders();
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
	}
	virtual void shutdown() {
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
	}
	virtual void render(double currentTime) {
		GLfloat red[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, red);
		glUseProgram(rendering_program);
		GLfloat attrib[] = { (float)sin(currentTime) * 0.5f, (float)cos(currentTime) * 0.5f, 0.0f, 0.0f };
		glVertexAttrib4fv(0, attrib);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	GLuint rendering_program;
	GLuint vertex_array_object;
};
DECLARE_MAIN(my_applicaiton)