#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/gl.h>

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <optional>

#include "Mesh.h"
#include "Pipeline.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "Camera.h"
#include "Scene.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct AppState{
    SDL_Window* windowPtr = nullptr;
    SDL_GLContext context = nullptr;

    Scene scene;

    std::optional<Camera> camera;
    std::optional<Renderer> renderer;
};

static int windowWidth = 800, windowHeight = 600;

static VertexLayout rainbowTriangle{
    5 * sizeof(float),
    {
        { 0, 3, GL_FLOAT, GL_FALSE, (void*)0 },
        { 1, 2, GL_FLOAT, GL_FALSE, (void*)(3* sizeof(float)) },
        // { 2, 2, GL_FLOAT, GL_FALSE, (void*)(6* sizeof(float)) }
    }
};

static std::vector<float> vertices = {
    // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
};
static std::vector<unsigned int> indices = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

const Fustrum cameraFustrum = {
    glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f
};

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
    
    AppState *as = new AppState{};
    if (!as) {
        return SDL_APP_FAILURE;
    } else {
        *appstate = as;
    }    

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

    as->windowPtr = SDL_CreateWindow( "Hello World", windowWidth, windowHeight,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if ( !as->windowPtr ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Fail to create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    as->context = SDL_GL_CreateContext( as->windowPtr );
    
    if ( !as->context ){
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Fail to create GLContext: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!gladLoadGL( (GLADloadfunc)SDL_GL_GetProcAddress )){
        SDL_LogError( SDL_LOG_CATEGORY_VIDEO, "Failed to initialize GLAD: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    

    // TODO : should be in some kind of renderer class 
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);  
    glViewport(0, 0, windowWidth, windowHeight );
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // usually this will be autometically after context creation.
    // seem to be true after tested
    // SDL_GL_MakeCurrent( mainWindow, mainContext );
    
    // Enable Vsync
    // TODO : read ts too
    SDL_GL_SetSwapInterval(1); // 1 = on, 0 = off, -1 = adaptive 

    // note that we're translating the scene in the reverse direction of where we want to move
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    as->camera.emplace(
        glm::vec3(0.0, 0.0, 3.0),
        glm::vec3(0.0, 0.0, 0.0), 
        cameraFustrum
    );

    as->renderer.emplace();
    
    RenderObject renderObject;
    renderObject.mesh = new Mesh( vertices, indices, rainbowTriangle );
    renderObject.pipeline = new Pipeline("shaders/vertex.glsl", "shaders/fragment.glsl" );
    renderObject.texture = new Texture( "images/container.jpg", textureParam );
    renderObject.inWorldState = {
        glm::vec3(0.0f),
        glm::vec3(-55.0f, 0.0f, 0.0f),
        glm::vec3(1.0f)
    };

    as->scene.objects.push_back(renderObject);

    return SDL_APP_CONTINUE;
}

void handleKeyPress(){
    const bool *key_states = SDL_GetKeyboardState(NULL);
    int direction = 0;


    if (key_states[SDL_SCANCODE_W]) {
        direction += 1; 
    } 

    if (key_states[SDL_SCANCODE_S]) {
        direction += -1;  /* pressed what would be "S" on a US QWERTY keyboard. Move backward! */
    }

    if (key_states[SDL_SCANCODE_A]) {
        direction += -1;  /* pressed what would be "S" on a US QWERTY keyboard. Move backward! */
    }

    if (key_states[SDL_SCANCODE_D]) {
        direction += -1;  /* pressed what would be "S" on a US QWERTY keyboard. Move backward! */
    }

    if (key_states[SDL_SCANCODE_SPACE]) {
        direction += -1;  /* pressed what would be "S" on a US QWERTY keyboard. Move backward! */
    }

    if (key_states[SDL_SCANCODE_D]) {
        direction += -1;  /* pressed what would be "S" on a US QWERTY keyboard. Move backward! */
    }

    if (key_states[SDL_SCANCODE_LCTRL] || key_states[SDL_SCANCODE_RCTRL] ) {
        direction += -1;  /* pressed what would be "S" on a US QWERTY keyboard. Move backward! */
    }

    if (key_states[SDL_SCANCODE_ESCAPE]) {
        direction += -1;  /* pressed what would be "S" on a US QWERTY keyboard. Move backward! */
    }
}

void update( const Camera& camera, Uint64 deltaTime_ns ){
    return;
}

SDL_AppResult SDL_AppIterate(void *appstate){

    AppState* as = static_cast<AppState*>( appstate );

    static Uint64 frameCount{};
    static Uint64 lastTick_ns{};
    static Uint64 pastTick_ns{};
    Uint64 currentTick_ns = SDL_GetTicksNS();
    Uint64 deltaTime_ns = currentTick_ns - pastTick_ns;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // update( as->camera, deltaTime_ns );

    as->renderer->render( *as->camera, as->scene );
    // we have to swap the buffer to present
    SDL_GL_SwapWindow( as->windowPtr );

    if ( currentTick_ns - lastTick_ns > 999999999) {
        lastTick_ns = currentTick_ns;
        // SDL_snprintf(debug_string, sizeof(debug_string), "%" SDL_PRIu64 " fps", frameCount);
        std::cout << frameCount << '\n';
        frameCount = 0;
    }
    pastTick_ns = currentTick_ns;
    frameCount += 1;
    Uint64 elapsed = SDL_GetTicksNS() - currentTick_ns;
    if (elapsed < 999999) {
        SDL_DelayNS(999999 - elapsed);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    AppState* as = static_cast<AppState*>( appstate );

    static int numKeys;
    static const bool *keys = SDL_GetKeyboardState(&numKeys);

    std::string output = "";
    switch( event->type ){
        
        case SDL_EVENT_QUIT:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "EVENT QUIT " );

            return SDL_APP_SUCCESS;

        // case SDL_EVENT_KEY_DOWN:
        //     if ( keys[ SDL_SCANCODE_SPACE ] ){
        //         output += " space";
        //     }
        //     if ( keys[ SDL_SCANCODE_W ] ){
        //        output += " w";
        //     }


        case SDL_EVENT_MOUSE_MOTION:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "APPEVENT mouse %f, %f", event->motion.x, event->motion.y  );
            
            break;
        case SDL_EVENT_WINDOW_RESIZED:

            SDL_GetWindowSizeInPixels( as->windowPtr, &windowWidth, &windowHeight );
            glViewport(0, 0, windowWidth, windowHeight);
            // SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "   (%d %d) ", windowWidth, windowHeight );
            
            break;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){
    
    SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Quitting..... " );
    AppState* as = static_cast<AppState*>( appstate );

    as->renderer.reset();
    as->camera.reset();

    // Destroy scene-owned GL resources here too.
    as->scene = Scene{};
    SDL_GL_DestroyContext(as->context);
    SDL_DestroyWindow( as->windowPtr );
    delete as;
    return;
}