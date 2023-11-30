//-----------------------------------------------------------------------------
// VS #0
void Pos_VS
(
	in float4 iPos : POSITION,

	out float4 oPos : POSITION,

	uniform float4x4 uProjectionMtx
)
{
	// Compute output position.
	oPos = mul(uProjectionMtx, iPos);
}

//-----------------------------------------------------------------------------
// VS #1
void PosColor_VS
(
	in float4 iPos : POSITION,
	in float4 iColor : COLOR,

	out float4 oPos : POSITION,
	out float4 oColor : COLOR,

	uniform float4x4 uProjectionMtx
)
{
	// Compute output position.
	oPos = mul(uProjectionMtx, iPos);

	// Pass color.
	oColor = iColor.bgra;
}

//-----------------------------------------------------------------------------
// VS #2
void PosTex0_VS
(
	in float4 iPos : POSITION,
	in float2 iTex0 : TEXCOORD0,

	out float4 oPos : POSITION,
	out float2 oTex0 : TEXCOORD0,

	uniform float4x4 uProjectionMtx
)
{
	// Compute output position.
	oPos = mul(uProjectionMtx, iPos);

	// Pass texcoord 0.
	oTex0 = iTex0;
}

//-----------------------------------------------------------------------------
// VS #3
void PosColorCoverage_VS
(
	in float4 iPos : POSITION,
	in float4 iColor : COLOR,
	in float iCoverage : TEXCOORD2,

	out float4 oPos : POSITION,
	out float4 oColor : COLOR,
	out float oCoverage : TEXCOORD1,

	uniform float4x4 uProjectionMtx
)
{
	// Compute output position.
	oPos = mul(uProjectionMtx, iPos);

	// Pass color.
	oColor = iColor.bgra;

	// Pass coverage.
	oCoverage = iCoverage;
}

//-----------------------------------------------------------------------------
// VS #4
void PosTex0Coverage_VS
(
	in float4 iPos : POSITION,
	in float2 iTex0 : TEXCOORD0,
	in float iCoverage : TEXCOORD2,

	out float4 oPos : POSITION,
	out float2 oTex0 : TEXCOORD0,
	out float oCoverage : TEXCOORD1,

	uniform float4x4 uProjectionMtx
)
{
	// Compute output position.
	oPos = mul(uProjectionMtx, iPos);

	// Pass texcoord 0.
	oTex0 = iTex0;

	// Pass coverage.
	oCoverage = iCoverage;
}

//-----------------------------------------------------------------------------
// VS #5
void PosColorTex1_VS
(
	in float4 iPos : POSITION,
	in float4 iColor : COLOR,
	in float2 iTex1 : TEXCOORD1,

	out float4 oPos : POSITION,
	out float4 oColor : COLOR,
	out float2 oTex1 : TEXCOORD1,

	uniform float4x4 uProjectionMtx
)
{
	// Compute output position.
	oPos = mul(uProjectionMtx, iPos);

	// Pass color.
	oColor = iColor.bgra;

	// Pass texcoord 1.
	oTex1 = iTex1;
}

//-----------------------------------------------------------------------------
// VS #6
void PosTex0Tex1_VS
(
	in float4 iPos : POSITION,
	in float4 iTex0 : TEXCOORD0,
	in float2 iTex1 : TEXCOORD1,

	out float4 oPos : POSITION,
	out float4 oTex0 : TEXCOORD0,
	out float2 oTex1 : TEXCOORD1,

	uniform float4x4 uProjectionMtx
)
{
	// Compute output position.
	oPos = mul(uProjectionMtx, iPos);

	// Pass texcoord 0.
	oTex0 = iTex0;

	// Pass texcoord 1.
	oTex1 = iTex1;
}

//-----------------------------------------------------------------------------
float4 samplePMA(uniform sampler2D uTexture, in float2 iTex0)
{
	float4 lTexel = tex2D(uTexture, iTex0);

	lTexel.rgb *= lTexel.w;

	return lTexel;
}

//-----------------------------------------------------------------------------
float2 radialTexcoords(uniform float4 uRadialGrad[2], in float2 iTex0)
{
	float fTemp1 = ((uRadialGrad[1].y * iTex0.x) - (uRadialGrad[1].z * iTex0.y));
	float2 vTemp2;

	vTemp2.x = (((uRadialGrad[0].x * iTex0.x) + (uRadialGrad[0].y * iTex0.y)) + (uRadialGrad[0].z * sqrt(((((uRadialGrad[0].w * iTex0.x) * iTex0.x) + ((uRadialGrad[1].x * iTex0.y) * iTex0.y)) - (fTemp1 * fTemp1)))));
	vTemp2.y = uRadialGrad[1].w;

	return vTemp2;
}

//-----------------------------------------------------------------------------
// FS #0
void RGBA_FS
(
	out float4 oColor : COLOR,

	uniform float4 uColor
)
{
	// Pass uniform input color.
	oColor = uColor.bgra;
}

//-----------------------------------------------------------------------------
// FS #1
void MASK_FS
(
	out float4 oColor : COLOR
)
{
	// Pass const mask color.
	oColor = float4(1.0, 1.0, 1.0, 1.0);
}

//-----------------------------------------------------------------------------
// FS #2
void PathSolid_FS
(
	in float4 iColor : COLOR,

	out float4 oColor : COLOR
)
{
	// Pass input color.
	oColor = iColor;
}

//-----------------------------------------------------------------------------
// FS #3
void PathLinear_FS
(
	in float2 iTex0 : TEXCOORD0,

	out float4 oColor : COLOR,

	uniform float uOpacity,
	uniform sampler2D uRamps : register(s0)
)
{
	// Compute output color.
	oColor = uOpacity * tex2D(uRamps, iTex0);
}

//-----------------------------------------------------------------------------
// FS #4
void PathRadial_FS
(
	in float2 iTex0 : TEXCOORD0,

	out float4 oColor : COLOR,

	uniform float4 uRadialGrad[2],
	uniform float uOpacity,
	uniform sampler2D uRamps : register(s0)
)
{
	float2 vRadialTexcoords = radialTexcoords(uRadialGrad, iTex0);

	// Compute output color.
	oColor = uOpacity * tex2D(uRamps, vRadialTexcoords);
}

//-----------------------------------------------------------------------------
// FS #5
void PathPattern_FS
(
	in float2 iTex0 : TEXCOORD0,

	out float4 oColor : COLOR,

	uniform float uOpacity,
	uniform sampler2D uPattern : register(s0)
)
{
	// Compute output color.
	oColor = uOpacity * samplePMA(uPattern, iTex0);
}

//-----------------------------------------------------------------------------
// FS #6
void PathAASolid_FS
(
	in float4 iColor : COLOR,
	in float iCoverage : TEXCOORD1,

	out float4 oColor : COLOR
)
{
	// Compute output color.
	oColor = iCoverage * iColor;
}

//-----------------------------------------------------------------------------
// FS #7
void PathAALinear_FS
(
	in float2 iTex0 : TEXCOORD0,
	in float iCoverage : TEXCOORD1,

	out float4 oColor : COLOR,

	uniform float uOpacity,
	uniform sampler2D uRamps : register(s0)
)
{
	// Compute output color.
	oColor = (uOpacity * iCoverage) * tex2D(uRamps, iTex0);
}

//-----------------------------------------------------------------------------
// FS #8
void PathAARadial_FS
(
	in float2 iTex0 : TEXCOORD0,
	in float iCoverage : TEXCOORD1,

	out float4 oColor : COLOR,

	uniform float4 uRadialGrad[2],
	uniform float uOpacity,
	uniform sampler2D uRamps : register(s0)
)
{
	float2 vRadialTexcoords = radialTexcoords(uRadialGrad, iTex0);

	// Compute output color.
	oColor = (uOpacity * iCoverage) * tex2D(uRamps, vRadialTexcoords);
}

//-----------------------------------------------------------------------------
// FS #9
void PathAAPattern_FS
(
	in float2 iTex0 : TEXCOORD0,
	in float iCoverage : TEXCOORD1,

	out float4 oColor : COLOR,

	uniform float uOpacity,
	uniform sampler2D uPattern : register(s0)
)
{
	// Compute output color.
	oColor = (uOpacity * iCoverage) * samplePMA(uPattern, iTex0);
}

//-----------------------------------------------------------------------------
// FS #10
void ImagePaintOpacitySolid_FS
(
	in float4 iColor : COLOR,
	in float2 iTex1 : TEXCOORD1,

	out float4 oColor : COLOR,

	uniform sampler2D uImage : register(s1)
)
{
	// Compute output color.
	oColor = iColor.w * tex2D(uImage, iTex1);
}

//-----------------------------------------------------------------------------
// FS #11
void ImagePaintOpacityLinear_FS
(
	in float2 iTex0 : TEXCOORD0,
	in float2 iTex1 : TEXCOORD1,

	out float4 oColor : COLOR,

	uniform float uOpacity,
	uniform sampler2D uRamps : register(s0),
	uniform sampler2D uImage : register(s1)
)
{
	// Compute output color.
	oColor = tex2D(uImage, iTex1) * (uOpacity * tex2D(uRamps, iTex0).w);
}

//-----------------------------------------------------------------------------
// FS #12
void ImagePaintOpacityRadial_FS
(
	in float2 iTex0 : TEXCOORD0,
	in float2 iTex1 : TEXCOORD1,

	out float4 oColor : COLOR,

	uniform float4 uRadialGrad[2],
	uniform float uOpacity,
	uniform sampler2D uRamps : register(s0),
	uniform sampler2D uImage : register(s1)
)
{
	float2 vRadialTexcoords = radialTexcoords(uRadialGrad, iTex0);

	// Compute output color.
	oColor = tex2D(uImage, iTex1) * (uOpacity * tex2D(uRamps, vRadialTexcoords).w);
}

//-----------------------------------------------------------------------------
// FS #13
void ImagePaintOpacityPattern_FS
(
	in float2 iTex0 : TEXCOORD0,
	in float2 iTex1 : TEXCOORD1,

	out float4 oColor : COLOR,

	uniform float uOpacity,
	uniform sampler2D uPattern : register(s0),
	uniform sampler2D uImage : register(s1)
)
{
	// Compute output color.
	oColor = tex2D(uImage, iTex1) * (uOpacity * tex2D(uPattern, iTex0).w);
}

//-----------------------------------------------------------------------------
// FS #14
void TextSolid_FS
(
	in float4 iColor : COLOR,
	in float2 iTex1 : TEXCOORD1,

	out float4 oColor : COLOR,

	uniform sampler2D uGlyphs : register(s1)
)
{
	// Compute output color.
	oColor = tex2D(uGlyphs, iTex1).x * iColor;
}

//-----------------------------------------------------------------------------
// FS #15
void TextLinear_FS
(
	in float2 iTex0 : TEXCOORD0,
	in float2 iTex1 : TEXCOORD1,

	out float4 oColor : COLOR,

	uniform float uOpacity,
	uniform sampler2D uRamps : register(s0),
	uniform sampler2D uGlyphs : register(s1)
)
{
	// Compute output color.
	oColor = (tex2D(uGlyphs, iTex1).x * uOpacity) * tex2D(uRamps, iTex0);
}

//-----------------------------------------------------------------------------
// FS #16
void TextRadial_FS
(
	in float2 iTex0 : TEXCOORD0,
	in float2 iTex1 : TEXCOORD1,

	out float4 oColor : COLOR,

	uniform float4 uRadialGrad[2],
	uniform float uOpacity,
	uniform sampler2D uRamps : register(s0),
	uniform sampler2D uGlyphs : register(s1)
)
{
	float2 vRadialTexcoords = radialTexcoords(uRadialGrad, iTex0);

	// Compute output color.
	oColor = (tex2D(uGlyphs, iTex1).x * uOpacity) * tex2D(uRamps, vRadialTexcoords);
}

//-----------------------------------------------------------------------------
// FS #17
void TextPattern_FS
(
	in float2 iTex0 : TEXCOORD0,
	in float2 iTex1 : TEXCOORD1,

	out float4 oColor : COLOR,

	uniform float uOpacity,
	uniform sampler2D uPattern : register(s0),
	uniform sampler2D uGlyphs : register(s1)
)
{
	// Compute output color.
	oColor = (tex2D(uGlyphs, iTex1).x * uOpacity) * samplePMA(uPattern, iTex0);
}
