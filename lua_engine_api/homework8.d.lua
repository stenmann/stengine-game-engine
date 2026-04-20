---@meta

-------------------------Vector2-------------------------

---@class Vector2
---@field x number
---@field y number
Vector2 = {}

---@return Vector2 
function Vector2(x, y) end

function Vector2:Normalize() end

---@return number 
function Vector2:Length() end

-------------------------Rigidbody-------------------------

---@class Rigidbody : Component
---@field x number
---@field y number
---@field rotation number
---@field gravity_scale number
Rigidbody = {}

---Apply a force to the center of the Rigidbody
---@param force Vector2 force to apply
function Rigidbody:AddForce(force) end

---Set the position of the rigidbody
---@param position Vector2
function Rigidbody:SetPosition(position) end

---Set the rotation of the rigidbody (degrees, clockwise)
---@param degrees number
function Rigidbody:SetRotation(degrees) end

---Set the velocity of the rigidbody
---@param velocity Vector2
function Rigidbody:SetVelocity(velocity) end

---Set the angular velocity (degrees per second, clockwise)
---@param degrees number
function Rigidbody:SetAngularVelocity(degrees) end

---Set the gravity scale of the rigidbody
---@param scale number
function Rigidbody:SetGravityScale(scale) end

---Rotate so the local "up" direction matches the given direction
---@param direction Vector2
function Rigidbody:SetUpDirection(direction) end

---Rotate so the local "right" direction matches the given direction
---@param direction Vector2
function Rigidbody:SetRightDirection(direction) end

---Get the position of the Rigidbody
---@return Vector2
function Rigidbody:GetPosition() end

---Get the rotation of the Rigidbody
---@return number
function Rigidbody:GetRotation() end

---Get the current linear velocity
---@return Vector2
function Rigidbody:GetVelocity() end

---Get the angular velocity (degrees per second)
---@return number
function Rigidbody:GetAngularVelocity() end

---Get the gravity scale
---@return number
function Rigidbody:GetGravityScale() end

---Get the normalized "up" direction vector (0,-1 when no rotation)
---@return Vector2
function Rigidbody:GetUpDirection() end

---Get the normalized "right" direction vector (1,0 when no rotation)
---@return Vector2
function Rigidbody:GetRightDirection() end

-------------------------HitResult-------------------------

---@class HitResult
---@field actor Actor
---@field point Vector2
---@field normal Vector2
---@field is_trigger boolean
Rigidbody = {}

-------------------------Physics-------------------------

---@class Physics
Physics = {}

---Returns the nearest hit, returning nil if nothing was hit
---@param position Vector2
---@param direction Vector2
---@param distance number
---@return HitResult|nil
function Physics.Raycast(position, direction, distance) end

---Returns all hits, in increasing order of distance
---@param position Vector2
---@param direction Vector2
---@param distance number
---@return HitResult[]
function Physics.RaycastAll(position, direction, distance) end

-------------------------Event-------------------------

---@class Event
Event = {}

---Publish an Event for any subscribers to receive
---@param event_type string Name of the type of event to publish to
---@param event_object any Object for subscribers to receive
function Event.Publish(event_type, event_object) end

---Subscribe to receive Published event after the end of the current frame
---@param event_type string Name of the type of event to subscribe to
---@param component Component Component that the event function belongs to
---@param event_function fun(self: Component, event_object: any) Function subscribe to the event 
function Event.Subscribe(event_type, component, event_function) end

---Unsubscribe from a subscribed event after the end of the current frame
---@param event_type string Name of the type of event to unsubscribe from
---@param component Component Component that the event function belongs to
---@param event_function fun(self: Component, event_object: any) Function unsubscribe from the event 
function Event.Unsubscribe(event_type, component, event_function) end
