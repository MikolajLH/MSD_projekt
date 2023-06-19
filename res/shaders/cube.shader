#shader vertex
#version 440

layout(location = 0) in vec4 position;
// layout(location = 1) in vec4 col;
layout(std430, binding = 2) buffer layoutName2
{
    vec4 colors_SSBO[];
};
// layout(location = 2) in int index_arr;
layout(std430, binding = 3) buffer layoutName
{
    int data_SSBO[];
};
out vec4 out_col;

// uniform gsamplerBuffer u_tbo_tex;
uniform mat4 u_MVP;
uniform ivec3 dimensions;

void main()
{
    vec4 pos = position;
    // int index =  gl_InstanceID;
    int index = data_SSBO[gl_InstanceID];
    vec4 col = colors_SSBO[index];
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
    // if (index_arr == gl_InstanceID + 1){
        // out_col.r = float(data_SSBO[gl_InstanceID]) / 1000000000.0;
        // out_col.g = float(index_arr) / 10000.0;
        // out_col.a = 0.1;
    // }
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