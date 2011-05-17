varying float diffuse;
varying vec3 ambience;
varying vec3 specular;

uniform vec3 bg_colour;
uniform vec3 light_position;

void main()
{
   gl_Position = gl_ModelViewMatrix * gl_Vertex;

   vec3 light = vec3(1, 1, 5);
   vec3 light_dir = (gl_ModelViewMatrix * vec4(light, 0.0));
   vec3 normal = gl_NormalMatrix * gl_Normal;

   vec3 ambient_colour = bg_colour;
   vec3 eye_to_vertex = normalize(-gl_Position);
   vec3 half_vector = normalize(eye_to_vertex + light_dir);

   float shininess = 50;
   float ambient_intensity = 0.5;
   float diffuse_intensity = 0.5;
   float specular_intensity = pow(max(dot(half_vector, normalize(normal)), 0), shininess);

   ambience = ambient_colour * ambient_intensity;

   diffuse = max(dot(normalize(normal), normalize(light_dir)), 0) * diffuse_intensity;
   specular = specular_intensity * ambient_colour;

   gl_Position = gl_ProjectionMatrix * gl_Position;
}
