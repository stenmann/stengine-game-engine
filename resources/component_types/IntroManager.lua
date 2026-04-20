IntroManager = {
    introComplete = false,
    entering = false,
    font_size = 100,
    title_x = 190,
    title_y = 100,

    OnStart = function(self)
        local player = Actor.Find("player")
        local boss = Actor.Find("boss")
        local hand1 = Actor.Find("hand1")
        local hand2 = Actor.Find("hand2")

        self.player_t = player:GetComponent("Transform")
        self.boss_t = boss:GetComponent("Transform")
        self.hand1_t = hand1:GetComponent("Transform")
        self.hand2_t = hand2:GetComponent("Transform")

    end,

OnUpdate = function(self)
    local frameCount = Application.GetFrame()

    if self.introComplete == false then
        Text.Draw("Stars of Issac", self.title_x, self.title_y, "SuperPixel", self.font_size, 60, 208, 112, 255)
        if frameCount % 90 < 45 then
            Text.Draw("Push Space to Start", 400, 500, "SuperPixel", 45, 255, 255, 255, 255)
        end
        if Input.GetKeyDown("space") then
            self.introComplete = true
            self.entering = true
            -- GameStarted stays false until animation finishes
        end

    elseif self.introComplete == true then
        if self.font_size > 0 then
            Text.Draw("Stars of Issac", self.title_x, self.title_y, "SuperPixel", self.font_size, 60, 208, 112, 255)
            self.font_size = self.font_size - 5
            self.title_x = self.title_x + 27
            self.title_y = self.title_y + 5
        end
    end

    if self.entering then
        if self.player_t.y > 0 then
            self.player_t.y = self.player_t.y - 0.05
            if self.boss_t.y < -4 then
                self.boss_t.y = self.boss_t.y + 0.025
            end
            if self.hand1_t.x > 6 then
                self.hand1_t.x = self.hand1_t.x - 0.05
            end
            if self.hand2_t.x < -6 then
                self.hand2_t.x = self.hand2_t.x + 0.05
            end
        else
            self.player_t.y = 0
            --self.boss_t.y = -4
            --self.hand1_t.x = 6
            --self.hand2_t.x = -6
            self.entering = false
            GameStarted = true  -- set here so enemies don't move during intro
        end
    end
end
}