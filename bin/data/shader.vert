varying vec3 vPos;
void main() {
    
    vPos = gl_Vertex.xyz;
    
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}