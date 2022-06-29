#version 450

layout(location = 0) out vec4 f_FragColor;

layout(push_constant) uniform pushConstants
{
  layout(offset = 64) vec3 pc_Col;
};

void main()
{
  f_FragColor = vec4(pc_Col, 1.0);
}
