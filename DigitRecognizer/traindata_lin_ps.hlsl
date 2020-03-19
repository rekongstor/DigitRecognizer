
/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
    float4 col = shaderTexture.Sample(SampleType, input.tex);
    float val = col.r;
    return float4(1.f - val, val, 0.3f - val * 0.3f, 1.f);
    //return float4(input.tex.x, input.tex.y, 0.f, 1.f);
}
