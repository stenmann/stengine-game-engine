PlayerCollide = {
    invincible_frames = 0,
    OnStart = function(self)
        self.player = Actor.Find("player"):GetComponent("Player")
        self.player_t = Actor.Find("player"):GetComponent("Transform")
        self.boss_t = Actor.Find("boss"):GetComponent("Transform")
        self.hand1_t = Actor.Find("hand1"):GetComponent("Transform")
        self.hand2_t = Actor.Find("hand2"):GetComponent("Transform")
     
    end,

    CheckCollision = function(self, pos1, pos2, radius)
        local dx = math.abs(pos1.x - pos2.x)
        local dy = math.abs(pos1.y - pos2.y)
        return dx < radius and dy < radius
    end,

OnUpdate = function(self)
     if self.invincible_frames > 0 then
            self.invincible_frames = self.invincible_frames - 1
        end
    for i = #All_projectiles_Boss, 1, -1 do
        local projectile = All_projectiles_Boss[i]

        -- skip already destroyed projectiles
        if not projectile.destroyed then
            local pt = projectile.transform
            local pos = {x = pt.x, y = pt.y}

            if self:CheckCollision(pos, {x = self.player_t.x, y = self.player_t.y}, .4) and self.invincible_frames == 0 and self.player.IsDashing == false then
                self.player.Health = self.player.Health - 1
                projectile.destroyed = true
                self.invincible_frames = 60
            end

        end
    end

    if self:CheckCollision({x = self.boss_t.x, y = self.boss_t.y}, {x = self.player_t.x, y = self.player_t.y}, 1.5) and self.invincible_frames == 0 and self.player.IsDashing == false then
                self.player.Health = self.player.Health - 1
                self.invincible_frames = 60
            end
    
    if self:CheckCollision({x = self.hand1_t.x, y = self.hand1_t.y}, {x = self.player_t.x, y = self.player_t.y}, 1.5) and self.invincible_frames == 0 and self.player.IsDashing == false then
                self.player.Health = self.player.Health - 1
                self.invincible_frames = 60
            end
    
    if self:CheckCollision({x = self.hand2_t.x, y = self.hand2_t.y}, {x = self.player_t.x, y = self.player_t.y}, 1.5) and self.invincible_frames == 0 and self.player.IsDashing == false then
                self.player.Health = self.player.Health - 1
                self.invincible_frames = 60
            end

    
end
}