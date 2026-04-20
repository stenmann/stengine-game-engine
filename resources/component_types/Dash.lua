Dash = {

dash_cooldown = 0,
dash_duration = 0,
dash_vel_x = 0,
dash_vel_y = 0,
dash_speed = 0.5,

    OnStart = function(self)
        self.t = self.actor:GetComponent("Transform")
        self.player = Actor.Find("player"):GetComponent("Player")
    end,

    OnUpdate = function(self)
    if self.dash_duration > 0 then
        self.t.x = self.t.x + self.dash_vel_x
        self.t.y = self.t.y + self.dash_vel_y
        self.dash_duration = self.dash_duration - 1
        self.player.IsDashing = true

        if self.dash_duration == 0 then
            self.player.IsDashing = false
        end

    elseif Input.GetKeyDown("space") and self.dash_cooldown == 0 then
        local dx = 0
        local dy = 0

        if Input.GetKey("right") then dx = self.dash_speed end
        if Input.GetKey("left") then dx = -self.dash_speed end
        if Input.GetKey("up") then dy = -self.dash_speed end
        if Input.GetKey("down") then dy = self.dash_speed end

    if dx ~= 0 and dy ~= 0 then
        dx = dx * 0.7071
        dy = dy * 0.7071
    end
        self.dash_vel_x = dx
        self.dash_vel_y = dy
        self.dash_duration = 4   
        self.dash_cooldown = 45
        self.player.IsDashing = true
    end

    if self.dash_cooldown > 0 then
        self.dash_cooldown = self.dash_cooldown - 1
    end
end

}