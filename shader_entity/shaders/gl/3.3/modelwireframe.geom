#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 gnormal;
    vec3 worldPosition;
    vec4 color;
    vec2 intensity;
    vec4 pos;
} gs_in[];

out vec3 gnormal;
out vec3 worldPosition;
out vec4 color;
out vec2 intensity;
out vec3 barycentric;

void combindVertex(int index)
{
    gnormal = gs_in[index].gnormal;
    worldPosition = gs_in[index].worldPosition;
    color = gs_in[index].color;
    intensity = gs_in[index].intensity;
    gl_Position = gs_in[index].pos;

    EmitVertex();
}

void main() 
{
    barycentric = vec3(1.0, 0.0, 0.0);
    combindVertex(0);
    
    barycentric = vec3(0.0, 1.0, 0.0);
    combindVertex(1);

    barycentric = vec3(0.0, 0.0, 1.0);
    combindVertex(2);

    EndPrimitive();
	
}
