#shader vertex
#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 col;
out vec4 out_col;

// uniform gsamplerBuffer u_tbo_tex;
uniform mat4 u_MVP;
uniform ivec3 dimensions;

void main()
{
    vec4 pos = position;
    int index =  gl_InstanceID;
    int ix = index % dimensions.x;
    index /= dimensions.x;
    int iy = index % dimensions.y;
    index /= dimensions.y;
    int iz = index % dimensions.z;
    pos.x += float(ix * 1);
    pos.y += float(iy * 1);
    pos.z += float(iz * 1);
    gl_Position = u_MVP * pos * gl_InstanceID;
    out_col = col;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 out_col;

// uniform vec4 u_Color;

void main()
{
    color = out_col;
    if (color.a == 0.0f)
        discard;
};