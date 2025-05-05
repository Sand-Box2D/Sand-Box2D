/// @brief Platform-specific module of the renderer backend. See class `Renderer`.

#pragma once

#include <memory>
#include <cstdint>

/// @brief Declaration of platform-specific Renderer data.
struct RendererSpecific;

/// @brief Mode of the renderer window (fullscreen or not or etc.).
enum RendererMode
{
    RR_MODE_FULLSCREEN_SOFT = 0,    // Renderer opens a window that's fullscreen. Provided resolution is ignored.
    RR_MODE_FULLSCREEN_HARD,        // Renderer opens in real fullscreen. You can force resolution if needed.
    RR_MODE_WINDOW                  // Renderer opens a window instead. Its resolution must be defined.
};

/// @brief Various params to set for the renderer environment such as resolution.
struct RendererParams
{
    /// @brief Value that returns real window size when multiplied with game resolution.
    ///
    /// E.g. `GAME_RES * scale = WINDOW_SIZE`
    /// => `{1280:720} * 1.5 = {1920:1080}`
    float scale = 1;

    /// @brief Desired mode of the renderer window.
    ///
    /// (See enum RendererMode).
    RendererMode renderer_mode = RR_MODE_FULLSCREEN_SOFT;

    /// @brief Desired size of the physical renderer window in pixels.
    ///
    /// - If you go to soft fullscreen, leave 0 as renderer will occupy the whole screen anyway.
    ///
    /// - If you go to hard fullscreen, leaving 0 means taking the screen resolution, but you can force another.
    ///
    /// - If you go to window, you have to define a resolution.
    ///
    /// The game resolution will get smaller if you provide a scale >1.
    int width = 0, height = 0;
};

/// @brief Little struct representing a color with an alpha channel.
struct RendererColor
{
    const unsigned char red = (char)0;      // Quantity of the red color (0-255).
    const unsigned char green = (char)0;    // Quantity of the green color (0-255).
    const unsigned char blue = (char)0;     // Quantity of the blue color (0-255).
    const unsigned char alpha = (char)0xFF; // Alpha channel (opacity) (0-255).
};

/**
 * @brief Renderer of the game.
 * 
 * It initializes the window and its renderer, lets you get/set resolution and,
 * most importantly, provides you the basic rendering functions
 * such as "render this texture there with this angle".
 * 
 * It could be implemented differently on different platforms.
 * For now, only SDL2 renderer is implemented.
 * 
 * Refer to the current platform source .cpp file to know which implementation this is.
 * 
 * When created, Renderer does nothing. To init the graphics environment, call `init()`
 * passing the `rendererParams` struct if you need to overwrite default params
 * (that is, software fullscreen window. You pass the struct to launch in a window for example).
 */
class Renderer
{
public:
    Renderer();
    ~Renderer();

    /// @brief Init the graphical environment (create a window and its renderer).
    /// @param rendererParams RendererParams to create a window/renderer with.
    /// @return True if could init correctly. False if not.
    bool init(RendererParams rendererParams = RendererParams());

    /// @brief Set the new renderer params when need to update.
    ///
    /// It won't init the renderer. Call `init()` instead.
    /// @param rendererParams New RendererParams.
    void setParams(RendererParams rendererParams);

    /// @brief Get current width of the game env in pixels.
    ///
    /// Game window might be bigger though. Refer to `getScale()`.
    ///
    /// Still, you need to consider *this* resolution when rendering.
    /// @return 0 if didn't yet init renderer.
    int getWidth();

    /// @brief Get current height of the game env in pixels.
    ///
    /// Game window might be bigger though. Refer to `getScale()`.
    ///
    /// Still, you need to consider *this* resolution when rendering.
    /// @return 0 if didn't yet init renderer.
    int getHeight();

    /// @brief Get a value that gives the physical size of the renderer window when
    /// multiplied with the game env size (`getWidth()` and `getHeight()`).
    /// @return 1 if didn't yet init renderer.
    float getScale();

    /// @brief Get time between two renders in milliseconds.
    ///
    /// ATTENTION: First few calls, function will return a wrong value.
    ///
    /// Its return value will update at every `render()` call.
    /// @return 0 if didn't yet init renderer.
    double getDelta();

    /// @brief Get rendered frames count since the renderer init.
    /// @return 0 if didn't yet init renderer.
    unsigned long int getFrames();

    /// @brief Get link to the window object specific to an implementation.
    /// @tparam TWindow Type of the pointer to the window object that could be different
    /// on different Window platforms such as SDL2 or GLFW.
    /// @return Pointer to the specific window object.
    template<typename TWindow>
    TWindow getWindow();

    /// @brief Get link to the renderer object specific to an implementation.
    /// @tparam TRenderer Type of the pointer to the renderer object that could be different
    /// on different Renderer platforms such as SDL2 Renderer or OpenGL.
    /// @return Pointer to the specific renderer object.
    template<typename TRenderer>
    TRenderer getRenderer();

    /// @brief Fill the whole screen with a single color.
    /// @param color Desired color of the screen (background even).
    void clearScreen(RendererColor color);

    /// @brief Swap the renderer buffer and show everything that was rendered.
    ///
    /// Also it refreshes the delta time value you can access via `getDelta()`.
    void render();

private:
    /// @brief Pointer to platform-specific Renderer data.
    std::shared_ptr<RendererSpecific> mp_Specific;

    /// @brief Is the renderer inited and ready to use?
    bool m_IsInited = false;

    /// @brief Quantity of frames that were rendered since the renderer init.
    unsigned long int m_Frames = 0;

    /// @brief Current size of the physical renderer window.
    ///
    /// 0 by default when not inited.
    ///
    /// Note `m_Scale`.
    int m_WindowWidth = 0, m_WindowHeight = 0;

    /// @brief Current size of the game environment.
    ///
    /// 0 by default when not inited.
    ///
    /// Note `m_Scale`.
    int m_GameWidth = 0, m_GameHeight = 0;

    /// @brief Value that returns real window size when multiplied with game resolution.
    ///
    /// Default value is 1.
    ///
    /// E.g. `GAME_RES * scale = WINDOW_SIZE`
    /// => `{1280:720} * 1.5 = {1920:1080}`
    float m_Scale = 1;

    /// @brief Value used to calculate the delta time.
    uint64_t m_PerfNow = 0, m_PerfLast = 0;

    /// @brief Current value of the time between two renders in milliseconds.
    double m_Delta = 0;
};
