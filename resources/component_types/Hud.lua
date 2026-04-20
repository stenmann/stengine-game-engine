Hud = {
OnStart = function(self)
    self.player = Actor.Find("player"):GetComponent("Player")
end,

OnLateUpdate = function(self)
    local lives = self.player.Health

    if not GameStarted then 
            return
        end

    if lives <= 0 then return end
    Image.DrawUI("health" .. lives,0,0)
end

}