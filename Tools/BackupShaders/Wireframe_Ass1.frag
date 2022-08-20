#version 450

layout(location = 0) out vec4 f_FragColor;

layout (set = 1, binding = 0) uniform u0v3
{
  vec3 u_ColMul;
};

void main()
{
  f_FragColor = vec4(u_ColMul, 1.0);
}
