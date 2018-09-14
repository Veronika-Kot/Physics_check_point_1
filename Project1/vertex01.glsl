uniform mat4 MVP; // 4 by 4 matrix

//anything with size 1 to 4. those vars are arbitrary 
attribute vec3 vCol; //rgb
attribute vec3 vPos; //xyz vec2 for xy only
//if atributes are not enabled it will get to origin location 000 or black color

//Going OUT to the next stage; going to fragmentShader
varying vec3 color; 

void main()
{
    gl_Position = MVP * vec4(5.0 * vPos, 1.0); //changed from vec4 bc vPos has 3 values now

    color = vCol; //take color which comes in and assigned it to the color, which goes to next change
   
   //We can pass data such colors and/or coordinates in the shaders too
   //make in black color.rgb *= 0.0001f; 
   /* color.r *= 0.0001f;
   color.g *= 0.0001f;
   color.b *= 0.0001f;
   
   color.r += 1.0f;
   color.g += 0.0f;
   color.b += 0.0f; */
}
