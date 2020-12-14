#version 330

in vec3 surface_normal;
in vec3 surface_position;
in vec2 uv0;

uniform vec3 light_dir;
uniform vec3 view_pos;
uniform vec3 ambient_color;
uniform float Ks;
uniform float Kd;
uniform float shininess;
uniform vec3 light_color;

uniform sampler2D diffuse_texture;

out vec4 outColor;

void main() {

	vec3 diffuse_color = texture2D(diffuse_texture, uv0).xyz;
	vec3 nlight_dir = normalize(light_dir);
	float NL = max(dot(normalize(surface_normal), nlight_dir), 0.0);
	
	vec3 ambient = ambient_color * diffuse_color;
	if (NL > 0) {	// if in front of light
		vec3 R = reflect(nlight_dir, normalize(surface_normal));
		vec3 V = normalize(view_pos - surface_position);
		float VR = max(dot(V, R), 0.0);
		vec3 specular = Ks * light_color * pow(VR, shininess);
		vec3 diffuse = Kd * diffuse_color * light_color * NL;
		
		outColor = normalize(vec4(ambient + diffuse + specular, 1.0));
	}
	else{	// is black
		outColor = normalize(vec4(ambient, 1.0));
	}
}