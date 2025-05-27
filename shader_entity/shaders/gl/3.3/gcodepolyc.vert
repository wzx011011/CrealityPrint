#version 150 core
/* gcodepolyc.vs */

in vec3 vertexPosition;
in vec3 vertexNormal;
in float vertexStep;
in int vertexRole;
in int vertexFlag;

out vec4 color;
out vec2 intensity;
out float step;

uniform mat4 model_matrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

/* light */
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

vec2 lightIntensity(vec3 position, vec3 _normal)
{
	vec2 intensity;
	float NdotL = max(dot(_normal, LIGHT_TOP_DIR), 0.0);
	intensity.x = INTENSITY_AMBIENT + NdotL * LIGHT_TOP_DIFFUSE;
	intensity.y = LIGHT_TOP_SPECULAR * pow(max(dot(-normalize(position), reflect(LIGHT_TOP_DIR, _normal)), 0.0), LIGHT_TOP_SHININESS);

	NdotL = max(dot(_normal, LIGHT_FRONT_DIR), 0.0);
	intensity.x += NdotL * LIGHT_FRONT_DIFFUSE;
	
	return intensity;
}
/* light end */

/* color */
uniform vec4 palette[101];
uniform float roleState[20];

vec4 getBaseColor()
{
	float alpha = roleState[vertexRole];
	return vec4(palette[vertexFlag].rgb, alpha);
}
/* color end */

void main( void )
{	
	step = vertexStep;
    color = getBaseColor();

	vec3 eye_normal = normalize(vertexNormal);
	vec4 pos = viewMatrix * model_matrix * vec4(vertexPosition, 1.0);
    intensity = lightIntensity(vec3(pos), eye_normal);
    gl_Position = projectionMatrix * pos;
}
