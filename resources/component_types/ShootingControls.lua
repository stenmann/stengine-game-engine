ShootingControls = {

    speed = 0.1,
    fire_cooldown = 0,

    OnStart = function(self)
        self.t = self.actor:GetComponent("Transform")
    end,

    OnUpdate = function(self)
    if not GameStarted then 
        return
    end
    -- handle turning
    if Input.GetKey("d") then self.t.rotation_degrees = 90 end
    if Input.GetKey("a") then self.t.rotation_degrees = 270 end
    if Input.GetKey("w") then self.t.rotation_degrees = 0 end
    if Input.GetKey("s") then self.t.rotation_degrees = 180 end

    -- tick cooldown once per frame
    if self.fire_cooldown > 0 then
        self.fire_cooldown = self.fire_cooldown - 1
    end

    -- shoot if any direction held and cooldown ready
    local shooting = Input.GetKey("d") or Input.GetKey("a") or 
                     Input.GetKey("w") or Input.GetKey("s")

    if shooting and self.fire_cooldown == 0 then
        self:SpawnBullet()
        self.fire_cooldown = 25
    end
end,

 SpawnBullet = function(self)
    local bullet = Actor.Instantiate("playerBullet")
    local bt = bullet:GetComponent("Transform")
    local dv = bullet:GetComponent("DirectionalVelocity")
    local bulletspeed = 0.2

    bt.x = self.t.x
    bt.y = self.t.y

    if Input.GetKey("d") then
        dv.x_vel = bulletspeed
        dv.y_vel = 0
    elseif Input.GetKey("a") then
        dv.x_vel = -bulletspeed
        dv.y_vel = 0
    elseif Input.GetKey("w") then
        dv.x_vel = 0
        dv.y_vel = -bulletspeed
        bt.rotation_degrees = 270
    elseif Input.GetKey("s") then
        dv.x_vel = 0
        dv.y_vel = bulletspeed
        bt.rotation_degrees = 90
    end
end
}
    

        

    