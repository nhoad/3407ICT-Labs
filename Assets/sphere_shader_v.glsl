varying vec3 N, L, P;

uniform vec3 bg_colour;

void main()
{
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   P = gl_Position;
   N = gl_NormalMatrix * gl_Normal;

   //gl_TexCoord[0] = gl_MultiTexCoord0;

   // this looks wrong, but I'm actually using the normals to map my textures to strangely shaped objects.
   gl_TexCoord[0] = vec4(gl_Normal, 1);

   vec4 V = gl_ModelViewMatrix * gl_Vertex;

   L = vec3(5, 5, 5);
}
