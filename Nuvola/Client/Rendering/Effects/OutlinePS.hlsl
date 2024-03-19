struct PixelInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

float4 pixel_main(PixelInput input) : SV_Target
{
    // Sample the depth and normal buffers
    float depth = tex2D(DepthSampler, position.xy).r;
    float3 normal = normalize(tex2D(NormalSampler, position.xy).xyz);

    // Calculate the Sobel gradients
    float3 gradientX = float3(
        tex2D(GradientSampler, position.xy - float2(1, 0)).r,
        tex2D(GradientSampler, position.xy).r,
        tex2D(GradientSampler, position.xy + float2(1, 0)).r
    );

    float3 gradientY = float3(
        tex2D(GradientSampler, position.xy - float2(0, 1)).r,
        tex2D(GradientSampler, position.xy).r,
        tex2D(GradientSampler, position.xy + float2(0, 1)).r
    );

    // Calculate the magnitude of the gradient
    float gradientMagnitude = length(gradientX) + length(gradientY);

    // Set the threshold for edge detection (adjust as needed)
    float edgeThreshold = 0.1;

    // Determine if the current pixel is an edge
    float edgeFactor = (gradientMagnitude > edgeThreshold) ? 1.0 : 0.0;

    // Set an outline color
    float4 outlineColor = float4(0.0, 0.0, 0.0, 1.0); // Black outline

    // Blend the outline color with the original color based on the edge factor
    float4 finalColor = lerp(OriginalColor, outlineColor, edgeFactor);

    // Return the final color
    return finalColor;
}