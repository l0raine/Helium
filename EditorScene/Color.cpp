#include "EditorScenePch.h"
#include "Color.h"

using namespace Helium;
using namespace Helium::Editor;

void Editor::Color::UnpackColor( uint32_t packed, uint32_t& a, uint32_t& r, uint32_t& g, uint32_t& b )
{
	a = ( packed >> 24 ) & 0xff;
	r = ( packed >> 16) & 0xff;
	g = ( packed >> 8 ) & 0xff;
	b = ( packed & 0xff );
}

Helium::Color Editor::Color::BlendColor( Helium::Color color1, Helium::Color color2, float32_t weight )
{
	uint32_t a1, a2, r1, r2, g1, g2, b1, b2;

	Editor::Color::UnpackColor( color1.GetArgb(), a1, r1, g1, b1 );
	Editor::Color::UnpackColor( color2.GetArgb(), a2, r2, g2, b2 );

	return Helium::Color(
		static_cast< uint8_t >( static_cast< float32_t >( r1 ) + static_cast< float32_t >( r2 - r1 ) * weight + 0.5f ),
		static_cast< uint8_t >( static_cast< float32_t >( g1 ) + static_cast< float32_t >( g2 - g1 ) * weight + 0.5f ),
		static_cast< uint8_t >( static_cast< float32_t >( b1 ) + static_cast< float32_t >( b2 - b1 ) * weight + 0.5f ),
		static_cast< uint8_t >( static_cast< float32_t >( a1 ) + static_cast< float32_t >( a2 - a1 ) * weight + 0.5f ) );
}

const Helium::Color Editor::Color::SNOW( 255, 250, 250, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::GHOSTWHITE( 248, 248, 255, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::WHITESMOKE( 245, 245, 245, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::GAINSBORO( 220, 220, 220, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::FLORALWHITE( 255, 250, 240, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::OLDLACE( 253, 245, 230, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LINEN( 250, 240, 230, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::ANTIQUEWHITE( 250, 235, 215, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::PAPAYAWHIP( 255, 239, 213, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::BLANCHEDALMOND( 255, 235, 205, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::BISQUE( 255, 228, 196, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::PEACHPUFF( 255, 218, 185, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::NAVAJOWHITE( 255, 222, 173, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MOCCASIN( 255, 228, 181, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::CORNSILK( 255, 248, 220, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::IVORY( 255, 255, 240, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LEMONCHIFFON( 255, 250, 205, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SEASHELL( 255, 245, 238, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::HONEYDEW( 240, 255, 240, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MINTCREAM( 245, 255, 250, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::AZURE( 240, 255, 255, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::ALICEBLUE( 240, 248, 255, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LAVENDER( 230, 230, 250, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LAVENDERBLUSH( 255, 240, 245, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MISTYROSE( 255, 228, 225, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::WHITE( 255, 255, 255, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::BLACK( 0, 0, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SILVER( 192, 192, 192, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKSLATEGRAY( 47, 79, 79, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKSLATEGREY( 47, 79, 79, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DIMGRAY( 105, 105, 105, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DIMGREY( 105, 105, 105, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SLATEGRAY( 112, 128, 144, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SLATEGREY( 112, 128, 144, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTSLATEGRAY( 119, 136, 153, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTSLATEGREY( 119, 136, 153, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::GRAY( 190, 190, 190, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::GREY( 190, 190, 190, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTGREY( 211, 211, 211, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTGRAY( 211, 211, 211, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MIDNIGHTBLUE( 25, 25, 112, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::NAVY( 0, 0, 128, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::NAVYBLUE( 0, 0, 128, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::CORNFLOWERBLUE( 100, 149, 237, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKSLATEBLUE( 72, 61, 139, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SLATEBLUE( 106, 90, 205, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MEDIUMSLATEBLUE( 123, 104, 238, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTSLATEBLUE( 132, 112, 255, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MEDIUMBLUE( 0, 0, 205, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::ROYALBLUE( 65, 105, 225, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::BLUE( 0, 0, 255, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DODGERBLUE( 30, 144, 255, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DEEPSKYBLUE( 0, 191, 255, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SKYBLUE( 135, 206, 235, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTSKYBLUE( 135, 206, 250, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::STEELBLUE( 70, 130, 180, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTSTEELBLUE( 176, 196, 222, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTBLUE( 173, 216, 230, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::POWDERBLUE( 176, 224, 230, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::PALETURQUOISE( 175, 238, 238, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKTURQUOISE( 0, 206, 209, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MEDIUMTURQUOISE( 72, 209, 204, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::TURQUOISE( 64, 224, 208, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::CYAN( 0, 255, 255, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTCYAN( 224, 255, 255, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::CADETBLUE( 95, 158, 160, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MEDIUMAQUAMARINE( 102, 205, 170, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::AQUAMARINE( 127, 255, 212, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKGREEN( 0, 100, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKOLIVEGREEN( 85, 107, 47, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKSEAGREEN( 143, 188, 143, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SEAGREEN( 46, 139, 87, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MEDIUMSEAGREEN( 60, 179, 113, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTSEAGREEN( 32, 178, 170, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::PALEGREEN( 152, 251, 152, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SPRINGGREEN( 0, 235, 127, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LAWNGREEN( 124, 252, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::GREEN( 0, 255, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::CHARTREUSE( 127, 255, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MEDIUMSPRINGGREEN( 0, 250, 154, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::GREENYELLOW( 173, 255, 47, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIMEGREEN( 50, 205, 50, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::YELLOWGREEN( 154, 205, 50, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::FORESTGREEN( 44, 149, 44, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::OLIVEDRAB( 107, 142, 35, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKKHAKI( 189, 183, 107, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::KHAKI( 240, 230, 140, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::PALEGOLDENROD( 238, 232, 170, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTGOLDENRODYELLOW( 250, 250, 210, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTYELLOW( 255, 255, 224, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::YELLOW( 255, 255, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::GOLD( 255, 215, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTGOLDENROD( 238, 221, 130, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::GOLDENROD( 218, 165, 32, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKGOLDENROD( 184, 134, 11, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::ROSYBROWN( 188, 143, 143, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::INDIAN( 205, 92, 92, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::INDIANRED( 205, 92, 92, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SADDLEBROWN( 139, 69, 19, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SIENNA( 160, 82, 45, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::PERU( 205, 133, 63, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::BURLYWOOD( 222, 184, 135, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::BEIGE( 245, 245, 220, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::WHEAT( 245, 222, 179, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SANDYBROWN( 244, 164, 96, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::TAN( 210, 180, 140, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::CHOCOLATE( 210, 105, 30, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::FIREBRICK( 178, 34, 34, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::BROWN( 165, 42, 42, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKSALMON( 233, 150, 122, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::SALMON( 250, 128, 114, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTSALMON( 255, 160, 122, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::ORANGE( 255, 165, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKORANGE( 255, 140, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::CORAL( 255, 127, 80, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTCORAL( 240, 128, 128, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::TOMATO( 255, 99, 71, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::ORANGERED( 255, 69, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::RED( 255, 0, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTRED( 255, 127, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::HOTPINK( 255, 105, 180, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DEEPPINK( 255, 20, 147, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::PINK( 255, 192, 203, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTPINK( 255, 182, 193, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::PALEVIOLETRED( 219, 112, 147, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MAROON( 176, 48, 96, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MEDIUMVIOLETRED( 199, 21, 133, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::VIOLETRED( 208, 32, 144, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MAGENTA( 255, 0, 255, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::VIOLET( 238, 130, 238, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::PLUM( 221, 160, 221, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::ORCHID( 218, 112, 214, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MEDIUMORCHID( 186, 85, 211, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKORCHID( 153, 50, 204, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKVIOLET( 148, 0, 211, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::BLUEVIOLET( 138, 43, 226, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::PURPLE( 160, 32, 240, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::MEDIUMPURPLE( 147, 112, 219, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::THISTLE( 216, 191, 216, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKGREY( 169, 169, 169, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKGRAY( 169, 169, 169, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKBLUE( 0, 0, 139, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKCYAN( 0, 139, 139, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKMAGENTA( 139, 0, 139, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::DARKRED( 139, 0, 0, static_cast< uint8_t >( 255 ) );
const Helium::Color Editor::Color::LIGHTGREEN( 144, 238, 144, static_cast< uint8_t >( 255 ) );