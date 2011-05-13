varying vec4 position;
varying vec3 normal;

void main()
{
   vec3 n = normal;
   vec3 light = vec3(5);
   vec3 diffuse = max(dot(normalize(n), normalize(light)), 0);

   gl_FragColor = vec4(diffuse, 0.0);
}
