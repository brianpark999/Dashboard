/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace DashData
{
    extern const char*   DashboardHUBonlycropped_png;
    const int            DashboardHUBonlycropped_pngSize = 449919;

    extern const char*   DashboardHUBonly_png;
    const int            DashboardHUBonly_pngSize = 500231;

    extern const char*   HUBBG_png;
    const int            HUBBG_pngSize = 1525275;

    extern const char*   Logo_ENHANCIA_Round_png;
    const int            Logo_ENHANCIA_Round_pngSize = 30589;

    extern const char*   upload_png;
    const int            upload_pngSize = 729;

    extern const char*   Ahmet_Altun__HalisGRBold_otf;
    const int            Ahmet_Altun__HalisGRBold_otfSize = 57952;

    extern const char*   Ahmet_Altun__HalisGRLight_otf;
    const int            Ahmet_Altun__HalisGRLight_otfSize = 46352;

    extern const char*   Ahmet_Altun__HalisGRRegular_otf;
    const int            Ahmet_Altun__HalisGRRegular_otfSize = 58528;

    extern const char*   NOOADemiSerifDEMO_ttf;
    const int            NOOADemiSerifDEMO_ttfSize = 18432;

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