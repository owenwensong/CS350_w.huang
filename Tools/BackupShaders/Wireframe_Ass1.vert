#version 450

layout(location = 0) in vec3 a_Pos;

layout(push_constant) uniform constants
{
  layout(offset = 0) mat4 pc_W2V;
};

void main()
{
  gl_Position = pc_W2V * vec4(a_Pos, 1.0);
}
