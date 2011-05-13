varying vec4 position;
varying vec3 normal;
varying float diffuse;

void main()
{
   vec3 light = vec3(3, 5, 1);
   vec3 light_dir = gl_ModelViewMatrix * vec4(light, 0.0);

   normal = gl_NormalMatrix * gl_Normal;

   position = gl_Vertex;
   gl_Position = gl_ModelViewProjectionMatrix * position;

   diffuse = max(dot(normalize(normal), normalize(light)), 0) * 0.5;

}
