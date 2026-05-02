#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_tex_coord;
layout(location = 2) in vec3 in_normal;

layout(location = 0) out vec2 out_tex_coord;
layout(location = 1) out vec3 out_normal;

layout(set = 0, binding = 2) uniform PerFrameData
{
    mat4 projection;
    mat4 view;
} u_per_frame_data;

layout(push_constant) uniform PerObjectData
{
    mat4 model;
} u_per_object_data;

void main()
{
    gl_Position = u_per_frame_data.projection * u_per_frame_data.view * u_per_object_data.model * vec4(in_position, 1.0);
    out_tex_coord = in_tex_coord;
    out_normal = in_normal;
}