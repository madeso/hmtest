void main()
{
	gl_Position = ftransform();
	float dist = gl_Position.z / 20.0;
	gl_BackColor = gl_FrontColor = vec4(dist, dist, dist, 1.0);
} 
