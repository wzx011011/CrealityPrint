#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in int vertexFlag;

out vec3 gnormal;
out vec3 worldPosition;
out vec4 color;
out vec2 intensity;

uniform vec3 offset;
uniform vec4 palette[17];
uniform int state;
uniform vec4 stateGain[4];

uniform mat4 model_matrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

/* 光照参数 */
// 环境光 
vec3 LIGHT_TOP_DIR = vec3(-0.4574957, 0.4574957, 0.7624929);	
float LIGHT_TOP_DIFFUSE = 0.48;
float LIGHT_TOP_SPECULAR  = 0.15;
float LIGHT_TOP_SHININESS = 10.0;
// 反射光 
vec3 LIGHT_FRONT_DIR = vec3(0.6985074, 0.1397015, 0.5985074);
float LIGHT_FRONT_DIFFUSE = 0.54;
float LIGHT_FRONT_SPECULAR = 0.0;
float LIGHT_FRONT_SHININESS = 5.0;
vec3 LIGHT_BACK_DIR = vec3(0.1397015, 0.6985074,0.6985074);
float LIGHT_BACK_DIFFUSE = 0.18;
float INTENSITY_AMBIENT  =  0.3;

vec2 lightIntensity(vec3 normal, vec3 position)
{
	vec2 intensity;

	float NdotL = max(dot(normal, LIGHT_TOP_DIR), 0.0);

	intensity.x = INTENSITY_AMBIENT + NdotL * LIGHT_TOP_DIFFUSE;
	intensity.y = LIGHT_TOP_SPECULAR * pow(max(dot(-normalize(position), reflect(LIGHT_TOP_DIR, normal)), 0.0), LIGHT_TOP_SHININESS);

	NdotL = max(dot(normal, LIGHT_FRONT_DIR), 0.0);
	intensity.x += NdotL * LIGHT_FRONT_DIFFUSE;
	
	return intensity;
}	

void main( void )
{
    color   = palette[vertexFlag] * stateGain[state];

    vec3 vert    = vertexPosition + offset;

    mat4 modelViewMatrix = viewMatrix * model_matrix;
	
	mat4 normalMatrix = transpose(inverse(modelViewMatrix));
	vec3 normal = normalize(mat3(normalMatrix) * vertexNormal);
	
	mat4 ggnmatrix = transpose(inverse(model_matrix));
	gnormal = normalize(mat3(ggnmatrix) * vertexNormal);

    vec4 pos = vec4(vert, 1.0);
    worldPosition = vec3(model_matrix * pos);

    intensity = lightIntensity(normal, worldPosition.xyz);

    gl_Position = projectionMatrix * modelViewMatrix * pos;
}
