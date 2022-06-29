#version 450

layout(location = 0) in vec3 a_Pos;

layout(location = 0) out vec3 v_Col;

layout(push_constant) uniform constants
{
  layout(offset = 0) mat4 pc_W2V;
};

void main()
{
  vec4 tempVec = pc_W2V * vec4(a_Pos, 1.0);
  gl_Position = tempVec;
  
  float multiplier = min(5 * tempVec.z, 1.0);
  v_Col = vec3(multiplier, multiplier, multiplier);
}
