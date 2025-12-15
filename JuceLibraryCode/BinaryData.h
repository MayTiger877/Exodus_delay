/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Knob_svg;
    const int            Knob_svgSize = 1205;

    extern const char*   bg_svg;
    const int            bg_svgSize = 165035;

    extern const char*   TileBG_svg;
    const int            TileBG_svgSize = 773;

    extern const char*   green_Slider_svg;
    const int            green_Slider_svgSize = 225;

    extern const char*   orange_Slider_svg;
    const int            orange_Slider_svgSize = 225;

    extern const char*   purple_Slider_svg;
    const int            purple_Slider_svgSize = 225;

    extern const char*   Red_Slider_svg;
    const int            Red_Slider_svgSize = 225;

    extern const char*   DistKnob_svg;
    const int            DistKnob_svgSize = 624;

    extern const char*   yellow_Slider_svg;
    const int            yellow_Slider_svgSize = 225;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 9;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
