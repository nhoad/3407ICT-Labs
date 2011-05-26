varying vec3 N, L, P;

uniform sampler2D tex;

void main()
{
   vec3 tex_diffuse = texture2D(tex, gl_TexCoord[0].st).xyz;

   const float shininess = 64.0;

   // normalise our normal and light positions
   vec3 NN = normalize(N);
   vec3 NL = normalize(L);

   // calculate the diffuse lighting
   float diffuse = max(0.0, dot(NN, NL)) * 0.5;

   // get our eye to vertex
   vec3 EtoV = normalize(-P);

   // calculate the half vector
   vec3 NH = normalize(EtoV + L);

   vec3 specular = pow(max(0.0, dot(NN, NH)), shininess);

   // set color to the diffuse component
   gl_FragColor = diffuse;

   // add the specular component
   gl_FragColor.rgb += specular;

   // add texture component
   gl_FragColor.rgb += tex_diffuse;
}
