#version 450

layout(location = 0) in vec3 v_Pos;
layout(location = 1) in vec3 v_Nml;

layout(location = 0) out vec4 f_FragColor;

layout(push_constant) uniform pushConstants
{
  layout(offset = 64) vec3 pc_LocalCamPos;
  layout(offset = 80) vec3 pc_LightCol;
};

const float c_Gamma = 0.444444; // 1 / 2.5
const float c_AmbientStrength = 0.0625;

void main()
{
  vec3 lightDir = normalize(pc_LocalCamPos - v_Pos);
  vec3 camDir = normalize(v_Pos - pc_LocalCamPos);
  
  float specularAmt = max( 0, dot(v_Nml, normalize( lightDir - camDir )));
  
  f_FragColor = vec4(pow(vec3(c_AmbientStrength) + pc_LightCol * specularAmt, vec3(c_Gamma)), 1);
}
