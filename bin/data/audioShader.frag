//precision highp float;

uniform sampler2D mytex;


varying float colX;
varying float colY;
void main(void)
 {
//    vec2 texCoord = gl_FragCoord.xy / resolution.xy;
//    texCoord.x *= 2.0;
//    texCoord.x = 1.0- abs(1.0-texCoord.x);
//    texCoord.y+= .25;
//    texCoord.y *= 2.0;
//   // gl_FragColor = vec4(col,col,col,1);
//    vec4 tempColor = texture2D(tex0,texCoord.st);
//    //gl_FragColor = texture2D(tex0,texCoord.st);
//    gl_FragColor = vec4(0.2+1.5*tempColor.rgb,tempColor.r/2.0 +0.5);
     vec4 color = texture2D(mytex,vec2(colX*255.0,colY*255.0));
     gl_FragColor = color ;//+ vec4(colX,colY,0.0,1.0)/2.0;///color;//color;//
}    


