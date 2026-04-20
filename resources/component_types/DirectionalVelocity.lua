DirectionalVelocity = {

    x_vel = 0,
    y_vel = 0,

    OnStart = function(self)
self.t = self.actor:GetComponent("Transform")
end,

OnUpdate = function(self)
    self.t.x = self.t.x + self.x_vel
    self.t.y = self.t.y + self.y_vel
end

}