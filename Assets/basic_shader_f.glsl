varying vec3 N, L;

uniform sampler2D tex;

void main()
{
   vec3 tex_diffuse = texture2D(tex, gl_TexCoord[0].st).xyz;

   const float specular_intensity = 128.0;

   vec3 NN = normalize(N);

   vec3 NL = normalize(L);
   vec3 NH = normalize(NL + vec3(0.0, 0.0, 1.0));

   float NdotL = max(0.0, dot(NN, NL));

   gl_FragColor.rgb = vec3(0.1, 0.2, 0.4) * NdotL;

   if (NdotL > 0.0)
      gl_FragColor.rgb += pow(max(0.0, dot(NN, NH)), specular_intensity);
}
