ProjectileP = {
    destroyed = false,

    OnStart = function(self)
        self.transform = self.actor:GetComponent("Transform")
        self.dv = self.actor:GetComponent("DirectionalVelocity")
        table.insert(All_projectiles, self)
    end,

    OnLateUpdate = function(self)
        if self.transform.x > 10 or self.transform.x < -10 or
           self.transform.y > 10 or self.transform.y < -10 then
            self.destroyed = true
        end

        if self.destroyed then
            self:OnDestroy()  -- clean up table first
            Actor.Destroy(self.actor)
        end
    end,

    OnDestroy = function(self)
        for i, projectile in ipairs(All_projectiles) do
            if projectile == self then
                table.remove(All_projectiles, i)
                break
            end
        end
    end
}