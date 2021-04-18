#shader vertex
#version 330 core

// our pixel location
layout(location = 0) in vec4 position;
// our texture location.  Notice it's location = 1
// this is based on our stride from layout, as well as which location in the array buffer
layout(location = 1) in vec2 texCoord;

// link out of shader to next stage
out vec2 v_TexCoord;

void main()
{
	gl_Position = position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

// link into shader from previous stage
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
	//color = u_Color;
};