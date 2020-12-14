#version 330
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 1) in vec2 vertex_uv0;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 surface_normal;
out vec3 surface_position;
out vec2 uv0;

void main() {
   gl_Position 		= proj*view*model*vec4(vertex_position, 1.0);
   surface_position = (model*vec4(vertex_position, 1.0)).xyz;
   surface_normal	= transpose(inverse(mat3(model)))*vertex_normal;
   uv0 = vertex_uv0;
}
