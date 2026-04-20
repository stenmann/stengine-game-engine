EnemyCollide = {
    OnStart = function(self)
        self.boss = Actor.Find("boss"):GetComponent("Boss")
        self.boss_t = Actor.Find("boss"):GetComponent("Transform")
        self.hand1 = Actor.Find("hand1"):GetComponent("Hand1")
        self.hand1_t = Actor.Find("hand1"):GetComponent("Transform")
        self.hand2 = Actor.Find("hand2"):GetComponent("Hand2")
        self.hand2_t = Actor.Find("hand2"):GetComponent("Transform")
    end,

    CheckCollision = function(self, pos1, pos2, radius)
        local dx = math.abs(pos1.x - pos2.x)
        local dy = math.abs(pos1.y - pos2.y)
        return dx < radius and dy < radius
    end,

OnUpdate = function(self)
    for i = #All_projectiles, 1, -1 do
        local projectile = All_projectiles[i]

        -- skip already destroyed projectiles
        if not projectile.destroyed then
            local pt = projectile.transform
            local pos = {x = pt.x, y = pt.y}

            if self:CheckCollision(pos, {x = self.boss_t.x, y = self.boss_t.y}, 1.2) then
                self.boss.hp = self.boss.hp - 1
                --Debug.Log("boss hit ".. self.boss.hp)
                projectile.destroyed = true
            end

            if self:CheckCollision(pos, {x = self.hand1_t.x, y = self.hand1_t.y}, 1.2) then
                self.hand1:TakeDamage(1)
                projectile.destroyed = true
            end

            if self:CheckCollision(pos, {x = self.hand2_t.x, y = self.hand2_t.y}, 1.2) then
                self.hand2:TakeDamage(1)
                projectile.destroyed = true
            end
        end
    end
end
}