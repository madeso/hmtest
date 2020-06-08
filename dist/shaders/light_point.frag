varying vec3 n;
varying vec3 v;
varying float dist;

void main (void)
{
	vec3 N = normalize(n);
	vec3 L = normalize(gl_LightSource[0].position.xyz - v); 
	float att = 0.0;

	float NdotL = dot(N,L);//, 0.0);

	vec4 Idiff;
	vec4 Ispec;

	if( NdotL > 0.0 ) {
		vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)
		vec3 R = normalize(-reflect(L,N)); 
		float t = (gl_LightSource[0].constantAttenuation +
						gl_LightSource[0].linearAttenuation * dist +
						gl_LightSource[0].quadraticAttenuation * dist * dist);
		//t = max(t, 0.0001);
		att = 1.0 / t;
		//calculate Diffuse Term:
		Idiff = gl_FrontLightProduct[0].diffuse * NdotL;

		// calculate Specular Term:
		Ispec = gl_FrontLightProduct[0].specular 
						  * pow(max(dot(R,E),0.0),gl_FrontMaterial.shininess);
	}
	// write Total Color:
	gl_FragColor = att * (Idiff + Ispec);
}