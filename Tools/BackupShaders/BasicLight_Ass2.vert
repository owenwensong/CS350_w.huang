#version 450

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Nml;

layout(location = 0) out vec3 v_Pos;
layout(location = 1) out vec3 v_Nml;

layout(push_constant) uniform pushConstants
{
  layout(offset = 0) mat4 pc_W2V;
};

void main()
{
  gl_Position = pc_W2V * vec4(a_Pos, 1.0);
  
  v_Pos = a_Pos; // send local space coords to frag shader
  v_Nml = a_Nml; // send vertex normal to frag shader
  
  // since T and N are perpendicular and cross product produces
  // |A||B|sin(angle(A,B))n, where |A| == 1, |B| == 1, sin(A, B) == 1
  // therefore the vector is normalized already.
  // This of course assumes A and B are properly normalized at the loading phase
  // v_TBN = mat3(a_Tan, cross(a_Nml, a_Tan), a_Nml);
}
