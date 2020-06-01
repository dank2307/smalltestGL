#shader vertex
#version 330 core 

layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aBeta;


//R^4 geometry:
//uniform vec3 beta1;
//uniform vec3 beta2;
uniform vec4 a1;
uniform vec4 a2;
uniform mat4 Lambda1;
uniform mat4 Lambda1INV;
uniform mat4 Lambda2;
uniform mat4 Lambda2INV;
uniform float u_time;



//3D geometry:
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec4 vertexColor;
out vec3 texCoord;

vec4 tPos;
vec3 spaceVec;


void main() 
{
   
    texCoord = aPos;
    //4d transformations:
    vec4 xsobs = vec4(u_time, 0, 0, 0); //Beoabachter im Beobachter System
    vec4 xssp = vec4(0, aPos.x, aPos.y, aPos.z); //Ereignis im Gegenstandssystem
    
    
    vec4 xssobs = Lambda2INV * (Lambda1 * xsobs + a1 - a2); //Observer im Gegenstandssystem
    
    xssp[0] = xssobs[0] - (sqrt( pow((xssp[1] - xssobs[1]),2) + pow((xssp[2] - xssobs[2]),2) + pow((xssp[3] - xssobs[3]),2) )); //Schnittpunkt mit Weltlinie
    
    vec4 xsp = Lambda1INV * (Lambda2 * xssp + a2 - a1);



    //3d grafik
   
    spaceVec = vec3(xsp[1], xsp[2], xsp[3]);
    gl_Position = projection*view*model*vec4(spaceVec, 1.0);
    
    //für Farbe
    tPos = gl_Position;

    vertexColor = vec4( (tPos.z+3)/10, (tPos.z + 3) / 10, (tPos.z + 3) / 5, 1.0f);
};




#shader geometry
#version 330 core


layout(triangles) in;
layout(triangle_strip, max_vertices = 121 ) out; 


vec4 v0 = gl_PositionIn[0];
vec4 v01 = (gl_PositionIn[1] - gl_PositionIn[0]);
vec4 v02 = (gl_PositionIn[2] - gl_PositionIn[0]);


void produceVertex(float s, float t)
{
    gl_Position = v0 + s * v01 + t * v02;
    EmitVertex();
}

void doDivision() {

    int i, j;

    
    int numLayers = 10;
    
    float ds = 1.0 / float(numLayers);
    float dt = 1.0 / float(numLayers);

    float t = 0.0;
    for (i = 0; i < numLayers; i++) {
        
        float s = 0.0;
        
        for (j = 0; j < numLayers - i; j++) {
            produceVertex(s, t);
            produceVertex(s, t + dt);
            s += ds;
        }
        produceVertex(s, t);
        t += dt;
        EndPrimitive();
    }

}


void main()
{
    doDivision();
}





#shader fragment
#version 330 core 


out vec4 FragColor;
in vec4 vertexColor;
in vec3 texCoord;


void main() 
{
    float a = 0.6f;
    float freq = 10;
    FragColor =( 0.3 * vec4(sin(freq * texCoord.x), sin(freq * texCoord.x), sin(freq * texCoord.x), 0)
        + 0.3 * vec4(sin(freq * texCoord.y), sin(freq * texCoord.y), sin(freq * texCoord.y), 0)
        + 0.3 * vec4(sin(freq * texCoord.z), sin(freq * texCoord.z), sin(freq * texCoord.z), 0)
        + vec4(a+1.0f, a, a, 1));

};