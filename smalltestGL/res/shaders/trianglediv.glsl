#shader vertex
#version 330 core 

layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aBeta;


//R^4 geometry:
//uniform vec3 beta1;
//uniform vec3 beta2;
uniform vec4 a1;
uniform vec4 a2;
uniform vec3 beta1;
uniform vec3 beta2;
uniform mat4 Lambda1;
uniform mat4 Lambda1INV;
uniform mat4 Lambda2;
uniform mat4 Lambda2INV;
uniform float u_time;

//3D geometry:
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUT{
    vec4 a_1;
    vec4 a_2;
    vec3 b_1;
    vec3 b_2;
    mat4 L1;
    mat4 L1INV;
    mat4 L2;
    mat4 L2INV;
    float time;
    mat4 ProjViewModel;

    //unveränderte Eingangsgeometrie
    vec4 in_pos;

}vs_out;



void main() 
{
    vs_out.a_1 = a1;
    vs_out.a_2 = a2;
    vs_out.L1 = Lambda1;
    vs_out.L1INV = Lambda1INV;
    vs_out.L2 = Lambda2;
    vs_out.L2INV = Lambda2INV;
    vs_out.time = u_time;
    vs_out.ProjViewModel = projection * view * model;

    vs_out.in_pos = vec4(aPos.x, aPos.y, aPos.z, 1);
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);

};











#shader geometry
#version 330 core

//max vertices = (n+1)+2*sum(1,n)  sum(1,n) = (n/2)*(n+1)  10->121 20->441
layout(triangles) in;
layout(triangle_strip, max_vertices = 121 ) out; 


in VS_OUT {
    vec4 a_1;
    vec4 a_2;
    vec3 b_1;
    vec3 b_2;
    mat4 L1;
    mat4 L1INV;
    mat4 L2;
    mat4 L2INV;
    float time;
    mat4 ProjViewModel;

    //unveränderte Eingangsgeometrie
    vec4 in_pos;

}gs_in[];

out vec3 TexCoord;

//vec4 v0 = gl_PositionIn[0];
//vec4 v01 = (gl_PositionIn[1] - gl_PositionIn[0]);
//vec4 v02 = (gl_PositionIn[2] - gl_PositionIn[0]);

vec4 v0 = gs_in[0].in_pos;
vec4 v1 = gs_in[1].in_pos;
vec4 v2 = gs_in[2].in_pos;

vec4 v01 = (v1 - v0);
vec4 v02 = (v2 - v0);
vec4 v12 = (v2 - v1);



vec4 getRTP(vec4 pos) { //get only relativisticly transformed position

    vec4 xsobs = vec4(gs_in[0].time, 0, 0, 0); //Beoabachter im Beobachter System
    
    vec4 xssp = vec4(0, pos.x, pos.y, pos.z); //Ereignis im Gegenstandssystem

    vec4 xssobs = gs_in[0].L2INV * (gs_in[0].L1 * xsobs + gs_in[0].a_1 - gs_in[0].a_2); //Observer im Gegenstandssystem

    xssp[0] = xssobs[0] - (sqrt(pow((xssp[1] - xssobs[1]), 2) + pow((xssp[2] - xssobs[2]), 2) + pow((xssp[3] - xssobs[3]), 2))); //Schnittpunkt mit Weltlinie

    vec4 xsp = gs_in[0].L1INV * (gs_in[0].L2 * xssp + gs_in[0].a_2 - gs_in[0].a_1); //Ereignis im Beobachter System

    

    vec4 outVec = vec4(xsp[1], xsp[2], xsp[3], 1.0); //3d projection
    
    return outVec;

}


vec4 getTP(vec4 pos) { //get transformed position

    vec4 xsobs = vec4(gs_in[0].time, 0, 0, 0); //Beoabachter im Beobachter System
    
    vec4 xssp = vec4(0, pos.x, pos.y, pos.z); //Ereignis im Gegenstandssystem

    vec4 xssobs = gs_in[0].L2INV * (gs_in[0].L1 * xsobs + gs_in[0].a_1 - gs_in[0].a_2); //Observer im Gegenstandssystem

    xssp[0] = xssobs[0] - (sqrt(pow((xssp[1] - xssobs[1]), 2) + pow((xssp[2] - xssobs[2]), 2) + pow((xssp[3] - xssobs[3]), 2))); //Schnittpunkt mit Weltlinie

    vec4 xsp = gs_in[0].L1INV * (gs_in[0].L2 * xssp + gs_in[0].a_2 - gs_in[0].a_1); //Ereignis im Beobachter System

    

    vec4 outVec = gs_in[0].ProjViewModel * vec4(xsp[1], xsp[2], xsp[3], 1.0); //3d projection
    
    return outVec;

}

void getVertex(float s, float t){

    vec4 newpos = v0 + s * v01 + t * v02;
    TexCoord = vec3(newpos.xyz);
    gl_Position = getTP(newpos);
    EmitVertex();

}

void doDivisionTri(int numLayers){

    int i, j;

    
    //int numLayers = 10;
    
    float ds = 1.0 / float(numLayers);
    float dt = 1.0 / float(numLayers);

    float t = 0.0;
    
    for (i = 0; i < numLayers; i++){
        
        float s = 0.0;
        
        for (j = 0; j < numLayers - i; j++) {
            getVertex(s, t);
            getVertex(s, t + dt);
            s += ds;
        }

        getVertex(s, t);
        t += dt;
        EndPrimitive();
    }

}




void main(){

    
    vec4 va;
    vec4 vb;

    vec4 viipp;

    float dist;
    float tolerance = 0.002;
    float tostep = 0.003;

    int maxLayers = 10;
    int neededlayers = 2;

    for(int i=0; i<3; i++){
        
        int ipp = i++;
        if(ipp==3) ipp = 0;
        
        //Mitte der transformierten Punkte:
        va = getRTP( gs_in[i].in_pos) + 0.5 *(  getRTP(gs_in[ipp].in_pos) - getRTP(gs_in[i].in_pos) );
        
        //Transformierte Mitte:
        vb = getRTP( gs_in[i].in_pos + 0.5* ( gs_in[ipp].in_pos - gs_in[i].in_pos )  );

        //Verbindungsvektor zwischen den transformierten Punkten:
        viipp = getRTP(gs_in[ipp].in_pos) - getRTP(gs_in[i].in_pos);

        dist = length(va -vb)/length(viipp);

        while(dist > (tolerance+float(neededlayers)*tostep)){
            if(neededlayers>(maxLayers-1)) break;

            neededlayers++;
            //tostep += 0.005;
        }

        //Wenn maximale Teilung aufgrund einer Seite -> skip
        if(neededlayers >= maxLayers) break;

    }
    

    doDivisionTri(neededlayers);
    //doDivisionTri(10);
}











#shader fragment
#version 330 core


in vec3 TexCoord;
out vec4 FragColor;



float checkerboard(vec2 tc) {
    vec4 bgcol = vec4(tc, 0, 1);

    float xpatFreq = 5.0;
    float ypatFraq = 5.0;
    float baseVal = 0.5;
    float stepVal = 0.2;

    bool xpat = (mod(tc.x * xpatFreq * 0.5, 1.0) < 0.5);
    bool ypat = (mod(tc.y * ypatFraq * 0.5, 1.0) < 0.5);

    float val = (xpat ^^ ypat) ? baseVal - stepVal : baseVal + stepVal;
    return val;

}


void main(){

    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    FragColor = vec4(checkerboard(vec2(TexCoord.x, TexCoord.y)), checkerboard(vec2(TexCoord.x, TexCoord.y)), checkerboard(vec2(TexCoord.z, TexCoord.x)), 1);

};