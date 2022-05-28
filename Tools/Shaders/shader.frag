#version 450

layout(location = 0) in vec3 v_Col;

layout(location = 0) out vec4 f_FragColor;

layout (set = 1, binding = 0) uniform u0v3
{
  vec3 u_ColMul;
};

void main()
{
  f_FragColor = vec4(v_Col.x * u_ColMul.x, v_Col.y * u_ColMul.y, v_Col.z * u_ColMul.z, 1.0);
}
