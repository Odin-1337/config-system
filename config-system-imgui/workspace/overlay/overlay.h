#pragma once

/* this is an shit code just saying... */

namespace overlay {
    LRESULT __stdcall wnd_proc(
        HWND hwnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam
    ) {
        if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, wParam, lParam ) )
            return true;

        switch ( msg )
        {
        case WM_CLOSE:
            DestroyWindow( hwnd );
            return 0;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
        }

        return DefWindowProcW( hwnd, msg, wParam, lParam );
    }

    class c_overlay {
    public:
        bool create_window( ) {
            ImGui_ImplWin32_EnableDpiAwareness( );
            this->m_window_scale = ImGui_ImplWin32_GetDpiScaleForMonitor( ::MonitorFromPoint( POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY ) );

            this->m_window_class = {
                sizeof( m_window_class ),
                CS_CLASSDC,
                wnd_proc,
                0L, 0L,
                GetModuleHandle( nullptr ),
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                L"ImGui Example",
                nullptr
            };

            ::RegisterClassExW( &m_window_class );
            this->m_window_handle = ::CreateWindowW(
                this->m_window_class.lpszClassName,
                L"Dear ImGui DirectX11 Example",
                WS_OVERLAPPEDWINDOW,
                100, 100, // pos
                ( int )( 1280 * this->m_window_scale ), // size x, width
                ( int )( 800 * this->m_window_scale ), // size y, height
                nullptr,
                nullptr,
                this->m_window_class.hInstance,
                nullptr
            );
            if ( this->m_window_handle == nullptr || !this->m_window_handle ) {
                std::println( "[c_overlay] Window handle is invalid, nullptr" );
                return false;
            }

            ::ShowWindow( this->m_window_handle, SW_SHOWDEFAULT );
            ::UpdateWindow( this->m_window_handle );
        }

        bool create_d3d11_device( )
        {
            DXGI_SWAP_CHAIN_DESC sd = {};
            ZeroMemory( &sd, sizeof( sd ) );
            sd.BufferCount = 2;
            sd.BufferDesc.Width = 0;
            sd.BufferDesc.Height = 0;
            sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.BufferDesc.RefreshRate.Numerator = 60;
            sd.BufferDesc.RefreshRate.Denominator = 1;
            sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.OutputWindow = this->m_window_handle;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.Windowed = TRUE;
            sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

            UINT device_flags = 0;
            D3D_FEATURE_LEVEL feature_level;
            const D3D_FEATURE_LEVEL feature_level_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
            HRESULT res = D3D11CreateDeviceAndSwapChain(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                device_flags,
                feature_level_array,
                2,
                D3D11_SDK_VERSION,
                &sd,
                &this->m_swap_chain,
                &this->m_device,
                &feature_level,
                &this->m_device_ctx
            );
            if ( res == DXGI_ERROR_UNSUPPORTED ) [[unlikely]] {
                std::println( "[c_overlay] DXGI Unsupported." );
                return false;
            }
            if ( res != S_OK ) [[unlikely]] {
                std::println( "[c_overlay] DXGI error: 0x{:08X}", static_cast< ULONG >( res ) );
                return false;
            }

            ID3D11Texture2D* back_buffer = {};
            this->m_swap_chain->GetBuffer( 0, IID_PPV_ARGS( &back_buffer ) );
            this->m_device->CreateRenderTargetView( back_buffer, nullptr, &this->m_render_view );
            back_buffer->Release( );

            IMGUI_CHECKVERSION( );
            ImGui::CreateContext( );
            ImGuiIO& io = ImGui::GetIO( ); ( void )io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            ImGui::StyleColorsDark( );

            // Setup scaling
            ImGuiStyle& style = ImGui::GetStyle( );
            style.ScaleAllSizes( this->m_window_scale );        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
            style.FontScaleDpi = this->m_window_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

            // Setup Platform/Renderer backends
            ImGui_ImplWin32_Init( this->m_window_handle );
            ImGui_ImplDX11_Init( this->m_device, this->m_device_ctx );

            // load fonts here
            io.Fonts->AddFontFromFileTTF( "c:\\Windows\\Fonts\\segoeui.ttf" );

            return true;
        }

        void render_loop( ) {
            MSG msg = {};
            while ( msg.message != WM_QUIT ) {
                if ( PeekMessageA( &msg, this->m_window_handle, 0, 0, PM_REMOVE ) ) {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }

                ImGui_ImplDX11_NewFrame( );
                ImGui_ImplWin32_NewFrame( );
                ImGui::NewFrame( );
                {
                    static char config_name[64] = "";
                    static int selected_config = -1;
                    std::vector<std::filesystem::directory_entry> configs;

                    ImGui::Text( "Config name:" );
                    ImGui::SameLine( );
                    ImGui::PushItemWidth( 200 );
                    ImGui::InputText( "##config_name", config_name, IM_ARRAYSIZE( config_name ) );
                    ImGui::PopItemWidth( );
                    ImGui::SameLine( );
                    if ( ImGui::Button( "Save" ) ) {
                        if ( !std::filesystem::exists( CONFIG_FOLDER ) && !std::filesystem::is_directory( CONFIG_FOLDER ) ) {
                            std::thread( [=]( ) { MessageBoxA( nullptr, std::format( "Config directory missing: {}", CONFIG_FOLDER ).c_str( ), "Config System", MB_OK | MB_ICONINFORMATION ); } ).detach( );
                        }
                        else if ( strlen( config_name ) == 0 ) {
                            std::thread( [=]( ) { MessageBoxA( nullptr, "Please enter a config name.", "Config System", MB_OK | MB_ICONERROR ); } ).detach( );
                        }
                        else {
                            std::string path = CONFIG_FOLDER + config_name + ".ini";
                            g_config->save( path.c_str( ) );
                            std::thread( [=]( ) { MessageBoxA( nullptr, "Config saved successfully!", "Config System", MB_OK | MB_ICONINFORMATION ); } ).detach( );
                        }
                    }

                    ImGui::Separator( );

                    if ( std::filesystem::exists( CONFIG_FOLDER ) && std::filesystem::is_directory( CONFIG_FOLDER ) ) {
                        for ( const auto& entry : std::filesystem::directory_iterator( CONFIG_FOLDER ) ) {
                            if ( entry.path( ).extension( ) == ".ini" )
                                configs.push_back( entry );
                        }

                        std::sort( configs.begin( ), configs.end( ), []( const auto& a, const auto& b ) {
                            return std::filesystem::last_write_time( a ) > std::filesystem::last_write_time( b );
                            } );

                        ImGui::BeginChild( "##config_list", ImVec2( ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y - 100 ), true );
                        for ( int i = 0; i < configs.size( ); ++i ) {
                            std::string name = configs[i].path( ).filename( ).string( );
                            bool is_selected = ( selected_config == i );
                            if ( ImGui::Selectable( name.c_str( ), is_selected ) ) {
                                selected_config = i;
                            }
                        }
                        ImGui::EndChild( );

                        if ( selected_config >= 0 && selected_config < configs.size( ) ) {
                            const auto& selected = configs[selected_config];
                            std::string name = selected.path( ).filename( ).string( );
                            std::string path = selected.path( ).string( );

                            ImGui::Text( "Selected Config: %s", name.c_str( ) );

                            std::time_t cftime = std::chrono::system_clock::to_time_t( 
                                std::chrono::clock_cast< std::chrono::system_clock >( std::filesystem::last_write_time( selected ) )
                            );
                            std::tm tm_buf;
                            localtime_s( &tm_buf, &cftime );

                            char time_buf[64];
                            strftime( time_buf, sizeof( time_buf ), "%Y-%m-%d %H:%M:%S", &tm_buf );
                            ImGui::Text( "Last Modified: %s", time_buf );

                            if ( ImGui::Button( "Load" ) ) {
                                g_config->load( path.c_str( ) );
                                std::thread( [=]( ) { MessageBoxA( nullptr, std::format( "Loaded config: {}", name ).c_str( ), "Config System", MB_OK | MB_ICONINFORMATION ); } ).detach( );
                            }

                            ImGui::SameLine( );

                            if ( ImGui::Button( "Delete" ) ) {
                                std::thread( [=]( ) { MessageBoxA( nullptr, std::format( "Deleted config: {}", name ).c_str( ), "Config System", MB_OK | MB_ICONINFORMATION ); } ).detach( );
                                std::filesystem::remove( path );
                                selected_config = -1;
                            }
                        }
                    }
                    else {
                        ImGui::TextColored( ImVec4( 1, 0, 0, 1 ), "Config directory missing: %s", CONFIG_FOLDER.c_str( ) );
                    }
                }
                ImGui::Render( );
                ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );
                const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
                this->m_device_ctx->OMSetRenderTargets( 1, &this->m_render_view, nullptr );
                this->m_device_ctx->ClearRenderTargetView( this->m_render_view, clear_color_with_alpha );
                ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

                // first param 0 = no vsync, 1 = vsync
                this->m_swap_chain->Present( 0, 0 );
            }


            ImGui_ImplDX11_Shutdown( );
            ImGui_ImplWin32_Shutdown( );
            ImGui::DestroyContext( );

            this->clean_up( );
        }

        void clean_up( ) { /* can remove logs its really not needed... */
            if ( this->m_render_view ) {
                this->m_render_view->Release( );
                this->m_render_view = nullptr;
                std::println( "[c_overlay::clean_up] Render Target View is Cleaned." );
            }
            if ( this->m_swap_chain ) {
                this->m_swap_chain->Release( );
                this->m_swap_chain = nullptr;
                std::println( "[c_overlay::clean_up] SwapChain is Cleaned." );
            }
            if ( this->m_device_ctx ) {
                this->m_device_ctx->Release( );
                this->m_device_ctx = nullptr;
                std::println( "[c_overlay::clean_up] Device Context is Cleaned." );
            }
            if ( this->m_device ) {
                this->m_device->Release( );
                this->m_device = nullptr;
                std::println( "[c_overlay::clean_up] Device is Cleaned." );
            }
            if ( this->m_window_handle != nullptr ) {
                ::DestroyWindow( this->m_window_handle );
                std::println( "[c_overlay::clean_up] Destroyed Window" );
            }
            if ( this->m_window_class.lpszClassName && this->m_window_class.hInstance ) {
                ::UnregisterClassW( this->m_window_class.lpszClassName, this->m_window_class.hInstance );
                std::println( "[c_overlay::clean_up] Unregistered Window Class" );
            }
        }
    private:
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_device_ctx = nullptr;
        IDXGISwapChain* m_swap_chain = nullptr;
        ID3D11RenderTargetView* m_render_view = nullptr;
        HWND m_window_handle = nullptr;
        WNDCLASSEXW m_window_class = {};
        float m_window_scale = 0.f;
    };
}