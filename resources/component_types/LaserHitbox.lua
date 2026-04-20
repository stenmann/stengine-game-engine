LaserHitbox = {
    active = false,
    owner = nil,  -- set by the hand that spawns it

    OnStart = function(self)
        self.transform = self.actor:GetComponent("Transform")
        self.player = Actor.Find("player"):GetComponent("Player")
        self.player_col = Actor.Find("player"):GetComponent("PlayerCollide")
    end,

OnUpdate = function(self)
    if not self.active then return end

    local dy = math.abs(self.transform.y - self.player.transform.y)
    if dy < 0.3 and self.player_col.invincible_frames == 0 and self.player.IsDashing == false then
        self.player.Health = self.player.Health - 1
        self.player_col.invincible_frames = 60
    end
end
}