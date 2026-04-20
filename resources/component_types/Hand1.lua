Hand1 = {
    hp = 10,
    max_hp = 10,
    state = "idle",
    state_timer = 0,
    target_y = 0,
    move_speed = 0.03,
    charge_speed = 0.15,
    start_x = 6, 
    charging = false,
    stunned = false,
    stun_pending = false,
    lastAttack = "charge_in",

    OnStart = function(self)
        self.transform = self.actor:GetComponent("Transform")
        self.target_y = self.transform.y
        self.start_x = self.transform.x
        self.boss = Actor.Find("boss"):GetComponent("Boss")
        self.hand2 = Actor.Find("hand2"):GetComponent("Hand2")
    end,

    OnUpdate = function(self)
        if not GameStarted then return end

        if self.stun_pending and self.state == "idle" then
            self.stun_pending = false
            self:ApplyStun()
        end

        if self.stunned then
            self.stun_timer = self.stun_timer - 1
            if self.stun_timer <= 0 then
                self:Recover()
            end
            return
        end

        self.state_timer = self.state_timer + 1

        if self.state == "idle" then
            self:Idle()
            self:SideToSideMovement()
        elseif self.state == "charge_in" then
            self:ChargeIn()
        elseif self.state == "charge_out" then
            self:ChargeOut()
        elseif self.state == "laser" then
            self:LaserAttack()
        end
    end,

    Idle = function(self)
        if self.boss.hp > 50 then
            if self.state_timer >= 180 and self.hand2.charging == false  then
            self:SetState("charge_in")
            end
        end

        if self.boss.hp <= 50 then 
        if self.state_timer >= 180 and self.hand2.charging == false and self.lastAttack == "laser" then
            self:SetState("charge_in")
            self.lastAttack = "charge_in"
        elseif self.state_timer >= 180 and self.hand2.charging == false and self.lastAttack == "charge_in" then
            self:SetState("laser")
            self.lastAttack = "laser"
        end
        end
    end,

    ChargeIn = function(self)
        self.charging = true
        self.transform.x = self.transform.x - self.charge_speed

        if self.transform.x <= -1 then
            self:SetState("charge_out")
        end
    end,

    ChargeOut = function(self)
        self.transform.x = self.transform.x + self.charge_speed

        if self.transform.x >= 6 then
            self:SetState("idle")
            self.charging = false
        end
    end,

    SetState = function(self, new_state)
        self.state = new_state
        self.state_timer = 0
    end,

    SideToSideMovement = function(self)
        if math.abs(self.transform.y - self.target_y) < 0.1 then
            self.target_y = math.random(-30, 45) / 10.0
        end
        if self.transform.y < self.target_y then
            self.transform.y = self.transform.y + self.move_speed
        else
            self.transform.y = self.transform.y - self.move_speed
        end
    end,
LaserAttack = function(self)
    if self.state_timer == 1 then
        self.laser_actor = nil
        self.laser_ready = false
        self.laser_sweeping = false

        if self.transform.y >= 0 then
            self.laser_target_y = 4.5
            self.laser_direction = 1
            self.sweep_end_y = -4.5
            self.sweep_direction = -1
        else
            self.laser_target_y = -4.5
            self.laser_direction = -1
            self.sweep_end_y = 4.5
            self.sweep_direction = 1
        end
        return
    end

    if not self.laser_ready then
        if math.abs(self.transform.y - self.laser_target_y) > 0.1 then
            self.transform.y = self.transform.y + (0.08 * self.laser_direction)
        else
            self.transform.y = self.laser_target_y

            self.laser_actor = Actor.Instantiate("laser")
            self.laser_t = self.laser_actor:GetComponent("Transform")
            self.laser_hb = self.laser_actor:GetComponent("LaserHitbox")
            self.laser_hb.active = true
            self.laser_t.x = 0
            self.laser_t.y = self.transform.y

            self.laser_ready = true
            self.laser_sweeping = true
        end

    elseif self.laser_sweeping then
        self.laser_hb.active = true
        self.transform.y = self.transform.y + (0.06 * self.sweep_direction)
        self.laser_t.x = 0
        self.laser_t.y = self.transform.y

        local past_end = (self.sweep_direction == 1 and self.transform.y >= self.sweep_end_y)
                      or (self.sweep_direction == -1 and self.transform.y <= self.sweep_end_y)

        if past_end then
            self.laser_hb.active = false
            Actor.Destroy(self.laser_actor)
            self.laser_actor = nil
            self.laser_ready = false
            self.laser_sweeping = false
            self:SetState("idle")
        end
    end
end,


    ApplyStun = function(self)
        self.stunned = true
        self.stun_timer = 720
        self.hp = self.max_hp 
        self:SetState("idle")
    end,

    Recover = function(self)
        self.stunned = false
        self.hp = self.max_hp 
        self.charging = false
        self:SetState("idle")
    end,

   TakeDamage = function(self, amount)
        self.hp = self.hp - amount
        if self.hp <= 0 then
            if self.state == "charge_in" or self.state == "charge_out" then
                self.stun_pending = true  
            else
                self:ApplyStun()
            end
        end
    end,
}