#version 330 core
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D Texture0;
uniform vec4 Color;
uniform bool isTextured;
uniform float repeticao;
void main()
{
    vec4 base = isTextured ? texture(Texture0, TexCoord * repeticao) : Color;
    float luz = 0.55 + 0.45 * max(dot(normalize(Normal), normalize(vec3(-0.4, 1.0, 0.7))), 0.0);
    FragColor = vec4(base.rgb * luz, base.a);
}
