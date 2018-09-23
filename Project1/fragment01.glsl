varying vec3 color;
uniform vec3 objectColor;
uniform bool useVertexColour;

void main()
{
  
 if( useVertexColour )
 {
   gl_FragColor = vec4(color, 1.0);
 }
 else {
	gl_FragColor = vec4(objectColor, 1.0);
  }
	
	
	//gl_FragColor.r = 1.0f;

	//gl_FragColor.gb = 0.0f;

}