#pragma once

#include <windows.h>
#include <fstream>

#define CONFIG_FOLDER std::string("c:/config-system/")

class c_config_system {
private:
    // int and bools
    inline BOOL WritePrivateProfileInt( LPCSTR lpAppName, LPCSTR lpKeyName, int nInteger, LPCSTR lpFileName ) {
        char lpString[1024];
        sprintf_s( lpString, sizeof( lpString ), "%d", nInteger );
        return WritePrivateProfileStringA( lpAppName, lpKeyName, lpString, lpFileName );
    }

    // float
    inline BOOL WritePrivateProfileFloat( LPCSTR lpAppName, LPCSTR lpKeyName, float nInteger, LPCSTR lpFileName ) {
        char lpString[1024];
        sprintf_s( lpString, sizeof( lpString ), "%f", nInteger );
        return WritePrivateProfileStringA( lpAppName, lpKeyName, lpString, lpFileName );
    }

    // float
    inline float GetPrivateProfileFloat( LPCSTR lpAppName, LPCSTR lpKeyName, FLOAT flDefault, LPCSTR lpFileName ) {
        char szData[32];
        GetPrivateProfileStringA( lpAppName, lpKeyName, std::to_string( flDefault ).c_str( ), szData, 32, lpFileName );
        return ( float )atof( szData );
    }

public:
    inline BOOL create_folder( ) {
        if ( std::filesystem::exists( CONFIG_FOLDER ) && std::filesystem::is_directory( CONFIG_FOLDER ) ) {
            std::println( "[c_config_system] Folder Already Exists. (GOOD)" );
        }
        else {
            BOOL result = CreateDirectoryA( CONFIG_FOLDER.c_str( ), 0 );
            if ( result == FALSE ) [[unlikely]] {
                std::println( "[c_config_system] Failed To Create Config Folder." );
                return result;
            }
            return result;
        }
    }

    inline void save( const char* filename ) {
        /* example code for mostly all value types */
        
        // int
        WritePrivateProfileInt( "Crosshair", "DrawCrosshair", settings::crosshair::draw_crosshair, filename );
        WritePrivateProfileInt( "Crosshair", "CrosshairOutline", settings::crosshair::crosshair_outline, filename );
        
        // float
        WritePrivateProfileFloat( "Crosshair", "CrosshairSize", settings::crosshair::crosshair_size, filename );
        WritePrivateProfileFloat( "Crosshair", "CrosshairOutlineThickness", settings::crosshair::crosshair_outline_thickness, filename );
        WritePrivateProfileFloat( "Crosshair", "CrosshairThickness", settings::crosshair::crosshair_thickness, filename );
        WritePrivateProfileFloat( "Crosshair", "CrosshairGap", settings::crosshair::crosshair_gap, filename );
        
        // int 
        WritePrivateProfileInt( "Crosshair", "DrawDot", settings::crosshair::draw_dot, filename );
        WritePrivateProfileInt( "Crosshair", "DotOutline", settings::crosshair::dot_outline, filename );

        // float
        WritePrivateProfileFloat( "Crosshair", "DotRadius", settings::crosshair::dot_radius, filename );
        WritePrivateProfileFloat( "Crosshair", "DotOutlineThickness", settings::crosshair::dot_outline_thickness, filename );

        /* imvec4 is an struct of x, y, z, w and there all float so you save them indevidualy you could make an wrapper function if want*/
        WritePrivateProfileFloat( "Crosshair", "CrosshairColorR", settings::crosshair::crosshair_color.x, filename );
        WritePrivateProfileFloat( "Crosshair", "CrosshairColorG", settings::crosshair::crosshair_color.y, filename );
        WritePrivateProfileFloat( "Crosshair", "CrosshairColorB", settings::crosshair::crosshair_color.z, filename );
        WritePrivateProfileFloat( "Crosshair", "CrosshairColorA", settings::crosshair::crosshair_color.w, filename );

        /* imvec4 is an struct of x, y, z, w and there all float so you save them indevidualy you could make an wrapper function if want*/
        WritePrivateProfileFloat( "Crosshair", "DotColorR", settings::crosshair::dot_color.x, filename );
        WritePrivateProfileFloat( "Crosshair", "DotColorG", settings::crosshair::dot_color.y, filename );
        WritePrivateProfileFloat( "Crosshair", "DotColorB", settings::crosshair::dot_color.z, filename );
        WritePrivateProfileFloat( "Crosshair", "DotColorA", settings::crosshair::dot_color.w, filename );
    }
    inline void load( const char* filename ) {
        /* example code for mostly all value types */
        
        // int
        settings::crosshair::draw_crosshair = GetPrivateProfileIntA( "Crosshair", "DrawCrosshair", settings::crosshair::draw_crosshair, filename );
        settings::crosshair::crosshair_outline = GetPrivateProfileIntA( "Crosshair", "CrosshairOutline", settings::crosshair::crosshair_outline, filename );

        // float
        settings::crosshair::crosshair_size = GetPrivateProfileFloat( "Crosshair", "CrosshairSize", settings::crosshair::crosshair_size, filename );
        settings::crosshair::crosshair_outline_thickness = GetPrivateProfileFloat( "Crosshair", "CrosshairOutlineThickness", settings::crosshair::crosshair_outline_thickness, filename );
        settings::crosshair::crosshair_thickness = GetPrivateProfileFloat( "Crosshair", "CrosshairThickness", settings::crosshair::crosshair_thickness, filename );
        settings::crosshair::crosshair_gap = GetPrivateProfileFloat( "Crosshair", "CrosshairGap", settings::crosshair::crosshair_gap, filename );

        // int
        settings::crosshair::draw_dot = GetPrivateProfileIntA( "Crosshair", "DrawDot", settings::crosshair::draw_dot, filename );
        settings::crosshair::dot_outline = GetPrivateProfileIntA( "Crosshair", "DotOutline", settings::crosshair::dot_outline, filename );

        // float
        settings::crosshair::dot_radius = GetPrivateProfileFloat( "Crosshair", "DotRadius", settings::crosshair::dot_radius, filename );
        settings::crosshair::dot_outline_thickness = GetPrivateProfileFloat( "Crosshair", "DotOutlineThickness", settings::crosshair::dot_outline_thickness, filename );

        /* imvec4 is an struct of x, y, z, w and there all float so you load them indevidualy you could make an wrapper function if want*/
        settings::crosshair::crosshair_color.x = GetPrivateProfileFloat( "Crosshair", "CrosshairColorR", settings::crosshair::crosshair_color.x, filename );
        settings::crosshair::crosshair_color.y = GetPrivateProfileFloat( "Crosshair", "CrosshairColorG", settings::crosshair::crosshair_color.y, filename );
        settings::crosshair::crosshair_color.z = GetPrivateProfileFloat( "Crosshair", "CrosshairColorB", settings::crosshair::crosshair_color.z, filename );
        settings::crosshair::crosshair_color.w = GetPrivateProfileFloat( "Crosshair", "CrosshairColorA", settings::crosshair::crosshair_color.w, filename );

        /* imvec4 is an struct of x, y, z, w and there all float so you load them indevidualy you could make an wrapper function if want*/
        settings::crosshair::dot_color.x = GetPrivateProfileFloat( "Crosshair", "DotColorR", settings::crosshair::dot_color.x, filename );
        settings::crosshair::dot_color.y = GetPrivateProfileFloat( "Crosshair", "DotColorG", settings::crosshair::dot_color.y, filename );
        settings::crosshair::dot_color.z = GetPrivateProfileFloat( "Crosshair", "DotColorB", settings::crosshair::dot_color.z, filename );
        settings::crosshair::dot_color.w = GetPrivateProfileFloat( "Crosshair", "DotColorA", settings::crosshair::dot_color.w, filename );
        
    }
};
inline auto g_config = std::make_shared<c_config_system>( ); // C++17 or later i think? 
// if C++17 or lower use this
//auto g_config = new c_config_system( );
// make sure to do 'delete[] g_config;' atexit/cleanup

std::string get_author_from_ini( const std::string& path ) {
    std::ifstream file( path );
    std::string line;
    while ( std::getline( file, line ) ) {
        if ( line.rfind( "author=", 0 ) == 0 ) {
            return line.substr( 7 );
        }
    }
    return "Unknown";
}