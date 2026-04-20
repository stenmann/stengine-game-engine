---@meta

-------------------------ParticleSystem-------------------------

---@class ParticleSystem : Component
---@field x number
---@field y number
---@field frames_between_bursts integer frame delay between bursts
---@field burst_quantity integer quantity of particles in a burst
---@field start_scale_min number min beginning scale of particles
---@field start_scale_max number max beginning scale of particles
---@field rotation_min number min rotation scale of particles, in degrees
---@field rotation_max number max rotation scale of particles, in degrees
---@field start_color_r number r color channel for initial color, between 0-255
---@field start_color_g number g color channel for initial color, between 0-255
---@field start_color_b number b color channel for initial color, between 0-255
---@field start_color_a number alpha channel for initial color, between 0-255
---@field emit_radius_min number min radius for a particle to spawn from system
---@field emit_radius_max number max radius for a particle to spawn from system
---@field emit_angle_min number min angle for a particle to spawn around system
---@field emit_angle_max number max angle for a particle to spawn around system
---@field image string name of the image to use for particles
---@field sorting_order integer Defines the layer for the draw command
---@field duration_frames integer lifetime of the particles in frames
---@field start_speed_min number min start speed for particles
---@field start_speed_max number max start speed for particles
---@field rotation_speed_min number min rotation speed for particles, in degrees
---@field rotation_speed_max number max rotation speed for particles, in degrees
---@field gravity_scale_x number gravity scale in the x axis
---@field gravity_scale_y number gravity scale in the y axis
---@field drag_factor number drag factor for velocity
---@field angular_drag_factor number drag factor for angular velocity
---@field end_scale number optional ending scale for particles
---@field end_color_r number optional r channel for ending color
---@field end_color_g number optional g channel for ending color
---@field end_color_b number optional b channel for ending color
---@field end_color_a number optional alpha for ending color
ParticleSystem = {}

---Start playing the particle system
function ParticleSystem:Play() end

---Stop the particle system
function ParticleSystem:Stop() end

---Emit a burst immediately
function ParticleSystem:Burst() end