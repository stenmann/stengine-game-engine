Player = {
    IsDashing = false,
    Health = 6,
    OnStart = function(self)
        self.transform = self.actor:GetComponent("Transform")
        player = self  -- global reference
    end,

    OnUpdate = function(self)
    end
}