varying float diffuse;
varying vec3 ambience;
varying vec3 specular;

uniform sampler2D tex;

void main()
{
   vec3 tex_diffuse = texture2D(tex, gl_TexCoord[0].st).xyz;

   gl_FragColor.xyz = (ambience * tex_diffuse) + (diffuse * tex_diffuse) + specular;
}
