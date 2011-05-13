varying vec4 position;
varying vec3 normal;
varying float diffuse;

void main()
{
   vec3 n = normal;
   vec3 light = vec3(5);

   gl_FragColor = diffuse;
}
