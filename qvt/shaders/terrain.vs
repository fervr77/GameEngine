#version 450 core
layout(location=0) in vec3 aPos;


out vec2 mapCoord;

uniform vec3 cameraPosition;
uniform mat4 transform;
uniform mat4 localMatrix;
uniform int lod;
uniform vec2 index;
uniform float scaleY;
uniform float gap;
uniform vec2 location;
uniform int lod_morph_area[8];
uniform sampler2D heightmap;


float morphLatitude(vec2 position)
{
    vec2 frac = position - location;

    if(index==vec2(0,0))
    {
        float morph = frac.x - frac.y;
        if(morph>0)
        {
            return morph;
        }

        
    }

     if(index==vec2(1,0))
    {
        float morph = gap - frac.x - frac.y;
        if(morph>0)
        {
            return morph;
        }

        
    }

    if(index==vec2(0,1))
    {
        float morph = frac.x - frac.y - gap;
        if(morph>0)
        {
            return -morph;
        }

        
    }


    if(index==vec2(1,1))
    {
        float morph = frac.y - frac.x;
        if(morph>0)
        {
            return -morph;
        }

        
    }



}


float morphLongitude(vec2 position)
{
    vec2 frac = position - location;

    if(index==vec2(0,0))
    {
        float morph = frac.y - frac.x;
        if(morph>0)
        {
            return -morph;
        }

        
    }

     if(index==vec2(1,0))
    {
        float morph = frac.y - (gap-frac.x);
        if(morph>0)
        {
            return morph;
        }

        
    }

    if(index==vec2(0,1))
    {
        float morph = gap - frac.y - frac.x;
        if(morph>0)
        {
            return -morph;
        }

        
    }


    if(index==vec2(1,1))
    {
        float morph = frac.x - frac.y;
        if(morph>0)
        {
            return morph;
        }

        
    }



}


vec2 morph(vec2 localPosition,int morph_area)
{
    vec2 morphing = vec2(0,0);

    vec2 fixPointLatitude = vec2(0,0);
    vec2 fixPointLongitude = vec2(0,0);
    float distLatitude;
    float distLongitude;


    if(index==vec2(0,0))
    {
        fixPointLatitude = location + vec2(gap,0);
        fixPointLongitude = location + vec2(0,gap);
    }

    else if(index==vec2(1,0))
    {
        fixPointLatitude = location;
        fixPointLongitude = location + vec2(gap,gap);
    }

    else if(index==vec2(0,1))
    {
        fixPointLatitude = location + vec2(gap,gap);
        fixPointLongitude = location;
    }
    else if(index==vec2(1,1))
    {
        fixPointLatitude = location + vec2(0,gap);
        fixPointLongitude = location + vec2(gap,0);
    }

    float planarFactor = 0;
    if(cameraPosition.y > abs(scaleY))
    {
        planarFactor = 1.0f;
    }
    else
    {
        planarFactor = cameraPosition.y/abs(scaleY);
    }

    distLatitude = length(cameraPosition - (transform*vec4(fixPointLatitude.x,planarFactor,fixPointLatitude.y,1)).xyz);
    distLongitude = length(cameraPosition - (transform*vec4(fixPointLongitude.x,planarFactor,fixPointLongitude.y,1)).xyz);

    if(distLatitude> morph_area)
        morphing.x = morphLatitude(localPosition.xy);
    if(distLongitude>morph_area)
        morphing.y = morphLongitude(localPosition.xy);

    return morphing;
}



void main(void)
{
    vec3 localPosition = (localMatrix*vec4(aPos,1.0f)).xyz;

    if(lod>0)
    {
        vec2 result = morph(localPosition.xz,lod_morph_area[lod-1]);
        localPosition += vec3(result.x,0,result.y);

    }

    float height = texture(heightmap,localPosition.xz).r;

    gl_Position = transform*vec4(localPosition.x,height,localPosition.z,1.0f);

    mapCoord = localPosition.xz;

}