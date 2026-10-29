#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/gl.h>

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include "Mesh.h"
#include "Pipeline.h"
#include "Texture.h"

static SDL_Window *mainWindow = nullptr;
static SDL_GLContext mainContext = nullptr;

static int windowWidth = 800, windowHeight = 600;

// static std::vector<float> vertices = {
//      0.5f,   0.5f,  0.0f,  // top right
//      0.5f,  -0.5f,  0.0f,  // bottom right
//     -0.5f,  -0.5f,  0.0f,  // bottom left
//     -0.5f,   0.5f,  0.0f   // top left 
// };

// static std::vector<float> vertices = {
//     // positions         // colors
//      0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
//     -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
//      0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
// };

static std::vector<float> vertices = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

static std::vector<unsigned int> indices = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle 
};

static VertexLayout rainbowTriangle{
    8 * sizeof(float),
    {
        { 0, 3, GL_FLOAT, GL_FALSE, (void*)0 },
        { 1, 3, GL_FLOAT, GL_FALSE, (void*)(3* sizeof(float)) },
        { 2, 2, GL_FLOAT, GL_FALSE, (void*)(6* sizeof(float)) }
    }
};

// static std::vector<float> texCoords = {
//     0.0f, 0.0f,  // lower-left corner  
//     1.0f, 0.0f,  // lower-right corner
//     0.5f, 1.0f   // top-center corner
// };

static Pipeline* mainPipeline;

Mesh* myMesh;

Texture* texture;

static std::vector<TextureParameter> textureParam{
     {GL_TEXTURE_WRAP_S, GL_REPEAT},// set texture wrapping to GL_REPEAT (default wrapping method)
    {GL_TEXTURE_WRAP_T, GL_REPEAT},
    {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
    {GL_TEXTURE_MAG_FILTER, GL_LINEAR}
};

bool setGLAttribute( int majorVersion = 3, int minorVersion = 3 ){
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
        SDL_LogError( SDL_LOG_CATEGORY_VIDEO, "Fail to set GL Attribute");
        return false;
    }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv){

    // Set Priority for all category 
    SDL_SetLogPriority( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG );
    SDL_SetLogPriority( SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG );
    SDL_SetLogPriority( SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_DEBUG );

    if ( !SDL_Init( SDL_INIT_VIDEO ) ){
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Fail to initialize SDL video subsystem: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if( setGLAttribute( 4, 5 ) ){
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

    if (!gladLoadGL( (GLADloadfunc)SDL_GL_GetProcAddress )){
        SDL_LogError( SDL_LOG_CATEGORY_VIDEO, "Failed to initialize GLAD: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, windowWidth, windowHeight );
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // usually this will be autometically after context creation.
    // seem to be true after tested
    // SDL_GL_MakeCurrent( mainWindow, mainContext );
    
    // Enable Vsync
    // TODO : read ts too
    SDL_GL_SetSwapInterval(1); // 1 = on, 0 = off, -1 = adaptive 

    myMesh = new Mesh( vertices, indices, rainbowTriangle );

    texture = new Texture( "images/container.jpg", textureParam );

    mainPipeline = new Pipeline("shaders/vertex.glsl", "shaders/fragment.glsl" );

    mainPipeline->bind();


    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate){

    glClear(GL_COLOR_BUFFER_BIT);
    texture->bind();
    myMesh->drawMesh( GL_TRIANGLES );

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

            SDL_GetWindowSizeInPixels( mainWindow, &windowWidth, &windowHeight );
            glViewport(0, 0, windowWidth, windowHeight);
            // SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "   (%d %d) ", windowWidth, windowHeight );
            
            break;
        case SDL_EVENT_QUIT:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "EVENT QUIT " );

            return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){

    SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Quitting..... " );

    delete myMesh;

    SDL_GL_DestroyContext( mainContext );
    SDL_DestroyWindow( mainWindow ); 
    return;
}