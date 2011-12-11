varying vec3 vPos;
varying vec3 normal;

void main() {
    vec3 eyePos = vec3(0.1,0.1,0.9);
    
    
    vec3 nN = normalize(normal);  

    float f = dot(nN, eyePos);
        
    gl_FragColor = vec4(vec3(1.0,1.0,1.0),1.0) * f;
}