varying vec3 n;
varying vec3 v;
varying float dist;

uniform sampler2D spotTexture;

void main (void)
{
	vec3 N = normalize(n);
	vec3 L = normalize(gl_LightSource[0].position.xyz - v); 
	vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)
	vec3 R = normalize(-reflect(L,N)); 
	float t = (gl_LightSource[0].constantAttenuation +
					gl_LightSource[0].linearAttenuation * dist +
					gl_LightSource[0].quadraticAttenuation * dist * dist);
	//t = max(t, 0.001);
	float att = 0.0f;

	//calculate Diffuse Term:
	vec4 Idiff;

	// calculate Specular Term:
	vec4 Ispec;

	float NdotL = dot(N,L);
	if( NdotL > 0.0 ) {
		float spotEffect = dot(normalize(gl_LightSource[0].spotDirection), 
						normalize(-L));

		if( spotEffect > gl_LightSource[0].spotCosCutoff ) {
			spotEffect = pow(spotEffect, gl_LightSource[0].spotExponent);
			att = spotEffect / t;
			//calculate Diffuse Term:
			Idiff = gl_FrontLightProduct[0].diffuse * NdotL;
			vec2 loc = (v.xy/v.z);
			loc *= gl_LightSource[0].spotCosCutoff * -1.50;
			loc += 0.5;
			Idiff *= texture2D(spotTexture, loc);

			// calculate Specular Term:
			Ispec = gl_FrontLightProduct[0].specular 
							  * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
		}
	}

	// write Total Color:
	gl_FragColor = att * (Idiff + Ispec);
}