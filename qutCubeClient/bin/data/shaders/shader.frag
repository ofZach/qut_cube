precision highp float;

uniform sampler2D tex0; // line image
uniform sampler2D tex1; // rock image
uniform float scale;

varying vec2 texCoordVarying;


void main()
{
    
    vec2 st = texCoordVarying.st;
    
    vec2 stOrig = st;
    
    
    // figure out where are from the center and use scale in some way;
//    
//    st -= vec2(0.5, 0.5);
//    
//    float angle = atan(st.y, st.x);
//    float dist = length(st);
//    dist += scale;
//    while (dist < 0.4) dist += 0.1;
//    while (dist > 0.5) dist -= 0.1;
//    
//    st.x = dist * cos(angle);
//    st.y = dist * sin(angle);
//    
//    
//    st += vec2(0.5, 0.5);
    
    
    //st *= vec2(0.9, 0.9);  // zooms in on the rock texture
    vec4 colorRock = texture2D(tex1, st);
    
    // 5 = how much displacement, try 50 or 100, etc.  always use decimal nums (10.0, etc)
    vec2 st2 = stOrig + vec2( (colorRock.x - 0.5) * 10.0, (colorRock.z - 0.5) * 10.0) * vec2( 1.0 / 1024.0, 1.0 / 768.0);
    vec4 color2 = texture2D(tex0, st2);
    
    color2 += vec4(0.3);
    
    vec4 color3 = texture2D(tex0, stOrig);
    color3 += vec4(0.3);
    vec4 colorMix = color2 * color3;
	gl_FragColor = vec4(colorMix.xyz, 1.0);
}