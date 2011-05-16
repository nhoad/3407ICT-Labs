varying float diffuse;
varying vec3 ambience;
varying vec3 specular;

void main()
{
   gl_FragColor.xyz = ambience + diffuse + specular;
}
