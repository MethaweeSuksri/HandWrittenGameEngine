#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/gl.h>

#include <iostream>
#include <string>
#include <stdexcept>

static SDL_Window *mainWindow = nullptr;
static SDL_GLContext mainContext = nullptr; 

static int windowWidth = 800, windowHeight = 600;

bool SetGLAttribute( int majorVersion = 3, int minorVersion = 3 ){
    try {
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minorVersion );
        
        // TODO : look at this in detail later
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
        SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
        SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
        return true;
    }
    catch( const std::runtime_error& e ){
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Fail to set GL Attribute");
        return false;
    }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv){

    // Set Priority for all category 
    SDL_SetLogPriority( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG );

    if ( !SDL_Init( SDL_INIT_VIDEO ) ){
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Fail to initialize SDL video subsystem: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if( SetGLAttribute( 4, 5 ) ){
        int majorVersion, minorVersion;
        SDL_GL_GetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersion);
        SDL_GL_GetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, &minorVersion);
        SDL_Log( "OpenGL context Major: %d Minor: %d", majorVersion, minorVersion );
    }
    else{
        return SDL_APP_FAILURE;
    }

    mainWindow = SDL_CreateWindow( "Hello World", windowWidth, windowHeight,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if ( !mainWindow ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Fail to create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    mainContext = SDL_GL_CreateContext( mainWindow );
    
    if ( !mainContext ){
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Fail to create GLContext: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // TODO : usually this will be autometically after context creation. but do test this
    SDL_GL_MakeCurrent( mainWindow, mainContext );
    
    // Enable Vsync
    // TODO : read ts too
    SDL_GL_SetSwapInterval(1); // 1 = on, 0 = off, -1 = adaptive 

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate){

    // we have to swap the buffer to present
    SDL_GL_SwapWindow( mainWindow );

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    static int numKeys;
    static const bool *keys = SDL_GetKeyboardState(&numKeys);

    std::string output = "";
    switch( event->type ){
        

        case SDL_EVENT_KEY_DOWN:
            if ( keys[ SDL_SCANCODE_SPACE ] ){
                output += " space";
            }
            if ( keys[ SDL_SCANCODE_W ] ){
               output += " w";
            }
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "%s", output.c_str()  );
            // SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "APPEVENT mouse" );
            break;
        case SDL_EVENT_MOUSE_MOTION:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "APPEVENT mouse %f, %f", event->motion.x, event->motion.y  );
            
            break;
        case SDL_EVENT_WINDOW_RESIZED:

            SDL_GetWindowSize( mainWindow, &windowWidth, &windowHeight );
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "   (%d %d) ", windowWidth, windowHeight );
            
            break;
        case SDL_EVENT_QUIT:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "EVENT QUIT " );

            return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){

    SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Quitting..... " );

    SDL_GL_DestroyContext( mainContext );
    SDL_DestroyWindow( mainWindow ); 
    return;
}