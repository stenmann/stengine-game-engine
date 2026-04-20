---@meta

-------------------------Debug-------------------------

---@class Debug
Debug = {}

---Logs a message to the terminal
---@param message string Message to log to the console
function Debug.Log(message) end

-------------------------Actor-------------------------

---@class Actor
Actor = {}

---Gets the name of an Actor
---@return string
function Actor:GetName() end

---Gets the ID of an Actor
---@return integer
function Actor:GetID() end

---Gets the reference to a Component by its key
---@param key string
---@return Component|nil
function Actor:GetComponentByKey(key) end

---Gets first component on an Actor with type, nil if no such Component present
---@param type string
---@return Component|nil
function Actor:GetComponent(type) end

---Gets all components on an Actor with type
---@return Component[]
function Actor:GetComponents() end

---Adds a new component with the provided type
---@param type string
function Actor:AddComponent(type) end

---Removes a Component on an Actor by Component reference
---@param component Component
function Actor:RemoveComponent(component) end

---Finds first Actor with name
---@param name string
---@return Actor
function Actor.Find(name) end

---Finds all Actors with name
---@return Actor[]
function Actor.FindAll() end

---Creates a new Actor from an existing template
---@param template string
---@return Actor
function Actor.Instantiate(template) end

---Destroy an Actor by reference
---@param actor Actor
function Actor.Destroy(actor) end


-------------------------Component-------------------------

---@class Component
---@field key string key of the Component
---@field actor Actor Reference to the owning actor
---@field enabled boolean Whether the component runs lifetime methods
---@field [string] any Used for member accesses specific to a component type
Component = {}

-------------------------Application-------------------------

---@class Application
Application = {}

---Gets the current frame number
---@return integer
function Application.GetFrame() end

---Ends the program immediatly
function Application.Quit() end

---Pauses for a set amount of time
---@param milliseconds number The number of miliseconds to delay for
function Application.Sleep(milliseconds) end

---Opens a url in the default browser
---@param url string url of the website to open
function Application.OpenURL(url) end

-------------------------Input-------------------------

---@class Input
Input = {}

---Checks whether a key is actively pressed
---@param keycode string Name of the key to check
function Input.GetKey(keycode) end

---Checks whether a key was pressed down this frame
---@param keycode string Name of the key to check
function Input.GetKeyDown(keycode) end

---Checks whether a key was released this frame
---@param keycode string Name of the key to check
function Input.GetKeyUp(keycode) end

---Gets the current position of the mouse on the screen
---@return table position Table containing 'x' and 'y' fields
function Input.GetMousePosition() end

---Checks whether a given mouse button is actively pressed<br>
---1: left<br>
---2: middle<br>
---3: right
---@param button_num integer number of the button to check 
function Input.GetMouseButton(button_num) end

---Checks whether a given mouse button was pressed down this frame<br>
---1: left<br>
---2: middle<br>
---3: right
---@param button_num integer number of the button to check 
function Input.GetMouseButtonDown(button_num) end

---Checks whether a given mouse button was released this frame<br>
---1: left<br>
---2: middle<br>
---3: right
---@param button_num integer number of the button to check 
function Input.GetMouseButtonUp(button_num) end

---Gets the change in scroll wheel since last frame
---@return number
function Input.GetMouseScrollDelta() end

---Hides the cursor when over the game menu
function Input.HideCursor() end

---Unhides the cursor when over the game menu
function Input.ShowCursor() end

-------------------------Text-------------------------

---@class Text
Text = {}

---Draws text into a position of the screen
---@param str_content string text to draw to the screen
---@param x number x position in pixels
---@param y number y position in pixels
---@param font_name string name of the font to use
---@param font_size integer size of text to draw
---@param r integer r color channel between 0-255
---@param g integer g color channel between 0-255
---@param b integer b color channel between 0-255
---@param a integer alpha channel between 0-255
function Text.Draw(str_content, x, y, font_name, font_size, r, g, b, a) end

-------------------------Audio-------------------------

---@class Audio
Audio = {}

---Play an audio file on a specific channel
---@param channel integer channel to play the audio on
---@param clip_name string name of the clip to play
---@param does_loop boolean whether the clip should repeat on finish
function Audio.Play(channel, clip_name, does_loop) end

---Immedietly end any audio clip playing on a specific channel
---@param channel integer channel to halt audio on
function Audio.Halt(channel) end

---Adjust the volume of a specific channel
---@param channel integer channel to modify the volume of
---@param volume number value for volume (0-128)
function Audio.SetVolume(channel, volume) end

-------------------------Image-------------------------

---@class Image
Image = {}

---Draw an Image as a UI element on the screen
---@param image_name string Name of the image to draw (without .png extension)
---@param x integer x position of element, in pixels
---@param y integer y position of element, in pixels
function Image.DrawUI(image_name, x, y) end

---Draw an Image as a UI element on the screen with additional options
---@param image_name string Name of the image to draw (without .png extension)
---@param x integer x position of element, in pixels
---@param y integer y position of element, in pixels
---@param r integer r color channel between 0-255
---@param g integer g color channel between 0-255
---@param b integer b color channel between 0-255
---@param a integer alpha channel between 0-255
---@param sorting_order integer Defines the layer for the draw command
function Image.DrawUIEx(image_name, x, y, r, g, b, a, sorting_order) end

---Draw an Image in world space
---@param image_name string Name of the image to draw (without .png extension)
---@param x number x position of element, in world units
---@param y number y position of element, in world units
function Image.Draw(image_name, x, y) end

---Draw an Image in world space, with additional options
---@param image_name string Name of the image to draw (without .png extension)
---@param x number x position of element, in world units
---@param y number y position of element, in world units
---@param rotation_degrees number Rotation of the image, in degrees
---@param scale_x number Scale of the image's along the x axis
---@param scale_y number Scale of the image's along the y axis
---@param pivot_x number The pivot of the image on the x axis, normalized to (0-1)
---@param pivot_y number The pivot of the image on the x axis, normalized to (0-1)
---@param r integer r color channel between 0-255
---@param g integer g color channel between 0-255
---@param b integer b color channel between 0-255
---@param a integer alpha channel between 0-255
---@param sorting_order integer Defines the layer for the draw command
function Image.DrawEx(image_name, x, y, rotation_degrees, scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order) end

---Draws a single pixel onto the screen
---@param x integer x position of the pixel
---@param y integer y position of the pixel
---@param r integer r color channel between 0-255
---@param g integer g color channel between 0-255
---@param b integer b color channel between 0-255
---@param a integer alpha channel between 0-255
function Image.DrawPixel(x, y, r, g, b, a) end

-------------------------Camera-------------------------

---@class Camera
Camera = {}

---Sets the position of the camera to a point in world space
---@param x number x position in world units
---@param y number y position in world units
function Camera.SetPosition(x, y) end

---Gets the current x position of the Camera
---@return number
function Camera.GetPositionX() end

---Gets the current y position of the Camera
---@return number
function Camera.GetPositionY() end

---Sets the zoom factor of the Camera
---@param zoom_factor number new zoom factor
function Camera.SetZoom(zoom_factor) end

---Gets the current zoom factor of the Camera
---@return number
function Camera.GetZoom() end

-------------------------Scene-------------------------

---@class Scene
Scene = {}

---Queues a scene to load at the beginning of the next frame
---@param scene_name string name of the scene to load
function Scene.Load(scene_name) end

---Gets the name of the currently active scene
---@return string 
function Scene.GetCurrent() end

---Marks an Actor not to be destroyed during a scene change
---@param actor Actor actor to mark as DontDestroy
function Scene.DontDestroy(actor) end