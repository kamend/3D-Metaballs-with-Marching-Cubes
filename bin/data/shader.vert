varying vec3 vPos;
varying vec3 normal;
void main() {
    
    normal = gl_NormalMatrix * gl_Normal;
   
    
    vPos = gl_Vertex.xyz;
    
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}