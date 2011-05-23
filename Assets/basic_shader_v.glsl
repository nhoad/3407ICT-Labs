varying vec3 N, L;

uniform vec3 bg_colour;

void main()
{

   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   N = gl_NormalMatrix * gl_Normal;

   //gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_TexCoord[0] = vec4(gl_Normal, 1);

   vec4 V = gl_ModelViewMatrix * gl_Vertex;

   vec3 light_pos = vec3(1, 1, 5);
   L = light_pos - V.xyz;
}
