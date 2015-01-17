cbuffer ShaderConstants : register(b0) {
	float4 g_PolygonSize : packoffset(c0);
};

struct PixelShaderInput {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR0;
};

bool InsideEllipse(float x, float y){
	bool inside = false;
	float calc = (pow(x - (g_PolygonSize.x / 2.0f), 2.0f) / pow((g_PolygonSize.x/2.0f), 2.0f)) + 
		(pow(y - (g_PolygonSize.y/2.0f), 2.0f) / pow((g_PolygonSize.y/2.0f), 2.0f));
	if (calc <= 1.0f)
		inside = !inside;
	return inside;
}

float4 DrawRectAntiAlias(float x, float y, PixelShaderInput input){
	float alpha = 0.0f;
	float4 color = input.color;

	if (((x >= 4 && x <= 6) || (x >= g_PolygonSize.x - 6 && x <= g_PolygonSize.x - 4)) && 
				(y >= 4 && y <= g_PolygonSize.y - 6)
			|| ((y >= 4 && y <= 30) || (y >= g_PolygonSize.y - 6 && y <= g_PolygonSize.y - 4)) && 
				(x >= 4 && x <= g_PolygonSize.x - 4)){
		
		color = float4(0,0,0,1);
		if (y >= 4 && y <= 30){
			alpha = (g_PolygonSize.x - x) / g_PolygonSize.x;
			color = float4(0,0,0,1);
		}
	}else{
		if (x <= 4 /*&& (y >= 7 && y <= g_PolygonSize.y - 7)*/){ //7
		alpha = x / 4;
		color = float4(input.color.rgb, alpha);
	}else{
		if (x >= g_PolygonSize.x - 4 /*&& (y >= 7 && y <= g_PolygonSize.y - 7)*/){
			alpha = (g_PolygonSize.x - x) / 4;
			color = float4(input.color.rgb, alpha);
		}else{
			if (y <= 4){
				alpha = y / 4;
				color = float4(input.color.rgb, alpha);
			}else{
				if (y >= g_PolygonSize.y - 4){
					alpha = (g_PolygonSize.y - y) / 4;
					color = float4(input.color.rgb, alpha);
				}
			}
		}
	}
	}
	return color;
}
		

float4 DrawRoundedAntiAlias(float x, float y, PixelShaderInput input){
	float4 color = input.color;
	float alpha = 0.0f;
	if (x <= 8){
		alpha = x / 8;
		color = float4(input.color.rgb, alpha);
	}else{
		if (x >= g_PolygonSize.x - 8){
			alpha = (g_PolygonSize.x - x) / 8;
			color = float4(input.color.rgb, alpha);
		}else{
			if (y <= 8){
				alpha = y / 8;
				color = float4(input.color.rgb, alpha);
			}else{
				if (y >= g_PolygonSize.y - 8){
					alpha = (g_PolygonSize.y - y) / 8;
					color = float4(input.color.rgb, alpha);
				}
			}
		}
	}
	return color;
}

float4 DrawRoundedBorder(float x, float y, PixelShaderInput input){
	float4 color = DrawRoundedAntiAlias(x, y, input);
	float calc = 1.1f;

	if (y < (20.0f)){
		if (x < 20.0f) //top-left
			calc = (pow(x - (20.0f), 2.0f) / pow((20.0f), 2.0f)) + 
				(pow(y - (20.0f), 2.0f) / pow((20), 2.0f));
			if (g_PolygonSize.x - x <= 20) //top-right
				calc = (pow((g_PolygonSize.x - x) - (20.0f), 2.0f) / pow((20.0f), 2.0f)) + 
					(pow(y - (20.0f), 2.0f) / pow((20), 2.0f));
	}else{
		if (y > (g_PolygonSize.y - 20.0f)){
			if (x < 20.0f) //bottom-left
				calc = (pow(x - (20.0f), 2.0f) / pow((20.0f), 2.0f)) + 
					(pow((g_PolygonSize.y - y) - 20.0f, 2.0f) / pow(20.0f, 2.0f));
			if (x > g_PolygonSize.x - 20.0f) //bottom-right
				calc = (pow((g_PolygonSize.x - x) - 20.0f, 2.0f) / pow((20.0f), 2.0f)) + 
					(pow((g_PolygonSize.y - y) - 20.0f, 2.0f) / pow(20.0f, 2.0f));
		}
	}

	float alpha = (1.0f - calc) * 1.4285f;
	if (calc > 0.30f && calc <= 1.0f){
			color = float4(input.color.rgb, alpha); //TODO BORDER BACKGROUND HERE WITH AA
		}else{
			if (calc <= 0.30f){
				color = input.color;
			}else{
				if (calc != 1.1f)
					color = float4(0,0,0,0);
			}
		}
	return color;
}

float4 RoundRectBorder(float x, float y, PixelShaderInput input){
	float4 color = DrawRectAntiAlias(x, y, input);
	return color;
}

// shader entry point
float4 PixelMain(in PixelShaderInput input) : SV_TARGET {
	float2 position = input.texCoord.xy * g_PolygonSize.xy;

	if (g_PolygonSize.w == 1.0f){
		/*if (InsideEllipse(position.x, position.y))
			return input.color;
		else
			return float4(0,0,0,0);*/
		return DrawRoundedBorder(position.x, position.y, input);
	}else{
			
			return RoundRectBorder(position.x, position.y, input);

		//return DrawRoundedBorder(position.x, position.y, input);
		}
}