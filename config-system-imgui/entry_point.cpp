#ifndef ENTRY_POINT_CPP
#define ENTRY_POINT_CPP

#include <impl/includes.h>

#pragma comment(lib,"d3d11.lib")

std::int32_t main(
	const int argc,
	char** argv
) {
	if ( g_config->create_folder( ) == 0 ) {
		return std::getchar( );
	}

	overlay::c_overlay overlay = {};

	if ( !overlay.create_window( ) ) {
		std::println( "[c_overlay::create_window] Returned FALSE" );
		overlay.clean_up( );
		return std::getchar( );
	}

	if ( !overlay.create_d3d11_device( ) ) {
		std::println( "[c_overlay::create_d3d11_device] Returned FALSE" );
		overlay.clean_up( );
		return std::getchar( );
	}

	overlay.render_loop( );

	return std::getchar( );
}

#endif // !gaurd
