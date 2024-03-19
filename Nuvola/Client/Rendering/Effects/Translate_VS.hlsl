float3 origin;

VSOutput main(float4 position : SV_Position)
{
    VSOutput vout;

    vout.PositionPS = mul(position - origin, WorldViewProj);
    vout.PositionPS.z = vout.PositionPS.w; // Draw on far plane
    vout.TexCoord = position.xyz;

    return vout;
}