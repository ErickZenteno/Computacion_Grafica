#version 330 core

in vec3 our_color;
in vec3 our_normal;
in vec3 fragPos;

struct PositionlLight{
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};
out vec4 color;
uniform PositionlLight light;
uniform vec3 viewPos;


void main(){
	vec3 ambient = light.ambient * our_color;
	vec3 normal=normalize(our_normal);
	vec3 ligthDir=normalize(light.position - fragPos);
	float diff=max(dot(ligthDir, normal), 0.0);
	vec3 diffuse=diff * light.diffuse * our_color;
	vec3 r=reflect(-ligthDir,normal);
	vec3 viewDir=normalize(viewPos-fragPos);
	float spec=pow(max(dot(r,viewDir),0.0),32.0);
	vec3 specular = spec * light.specular * our_color;
	vec3 result = ambient + diffuse;
	color = vec4(result, 1.0);

}