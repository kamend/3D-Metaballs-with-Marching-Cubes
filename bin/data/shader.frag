varying vec3 vPos;

void main() {
    vec3 eyePos = vec3(100.0,0.0,100.0);
     
    float f = length(vPos - eyePos);
    
    f /= 1000.0;
    
    gl_FragColor = vec4(vec3(1.0,0.0,1.0)*f,0.3);
}