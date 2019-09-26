#pragma once

class RGBA
{
	public: float red, green, blue, alpha;      

	public: RGBA();
	

	public: RGBA(int _red, int _green, int _blue, int _alpha);


	public: void SetRGBA8(int _red, int _green, int _blue, int _alpha);
};