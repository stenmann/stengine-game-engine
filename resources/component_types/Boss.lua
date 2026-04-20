Boss = {
    hp = 100,
    state = "idle",
    state_timer = 0,
    target_x = 0,       
    move_speed = 0.03,
    lastAttack = "shootSpread",
    phase = 1,

    OnStart = function(self)
        self.transform = self.actor:GetComponent("Transform")
        self.target_x = self.transform.x
    end,

    OnUpdate = function(self)
        if not GameStarted then 
            return
        end

        self:SideToSideMovement() 

        self.state_timer = self.state_timer + 1

        if self.state == "idle" then
            self:Idle()
        elseif self.state == "shootBurst" then
            self:ShootTracking()
        elseif self.state == "shootSpread" then
            self:ShootFan()
        end
    end,

    Idle = function(self)
        if self.state_timer >= 120 and self.lastAttack == "shootSpread" then
            self:SetState("shootBurst")
            self.lastAttack = "shootBurst"
        end
        if self.state_timer >= 120 and self.lastAttack == "shootBurst" then
            self:SetState("shootSpread")
            self.lastAttack = "shootSpread"
        end 
    end,

    ShootFan = function(self)
        self:SpawnBulletWithAngle(15)
        self:SpawnBulletWithAngle(30)
        self:SpawnBulletWithAngle(45)
        self:SpawnBulletWithAngle(60)
        self:SpawnBulletWithAngle(75)
        self:SpawnBulletWithAngle(90)
        self:SpawnBulletWithAngle(105)
        self:SpawnBulletWithAngle(120)
        self:SpawnBulletWithAngle(135)
        self:SpawnBulletWithAngle(150)
        self:SpawnBulletWithAngle(165)
        self:SetState("idle")

    end,
    ShootTracking = function(self)
        if self.state_timer % 20 == 0 then
            self:SpawnBulletTracking()
        end
        if self.state_timer >= 80 then
            self:SetState("idle")
        end
    end,

    SetState = function(self, new_state)
        self.state = new_state
        self.state_timer = 0 
    end,

    TakeDamage = function(self, amount)
        self.hp = self.hp - amount
        if self.hp <= 0 then
            self:SetState("dead")
        end
    end,

    SpawnBulletTracking = function(self)
        local bullet = Actor.Instantiate("bossBullet")
        local bt = bullet:GetComponent("Transform")
        local dv = bullet:GetComponent("DirectionalVelocity")
        bt.x = self.transform.x
        bt.y = self.transform.y
        local dx = player.transform.x - self.transform.x
        local dy = player.transform.y - self.transform.y
        local len = math.sqrt(dx*dx + dy*dy)
        dv.x_vel = (dx/len) * 0.15
        dv.y_vel = (dy/len) * 0.15
    end,

    SpawnBulletWithAngle = function(self,angle_degrees)
         local bullet = Actor.Instantiate("bossBullet")
        local bt = bullet:GetComponent("Transform")
        local dv = bullet:GetComponent("DirectionalVelocity")
        bt.x = self.transform.x
        bt.y = self.transform.y
        local radians = math.rad(angle_degrees)
        local speed = 0.15
        dv.x_vel = math.cos(radians) * speed
        dv.y_vel = math.sin(radians) * speed
    end, 

    SideToSideMovement = function(self)
        if math.abs(self.transform.x - self.target_x) < 0.1 then
            self.target_x = math.random(-30, 30) / 10.0
        end

        if self.transform.x < self.target_x then
            self.transform.x = self.transform.x + self.move_speed
        else
            self.transform.x = self.transform.x - self.move_speed
        end
    end,
}