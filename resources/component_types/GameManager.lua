GameManager = {

    	OnStart = function(self)
            if All_projectiles == nil then
			    All_projectiles = {}
		    end
            if All_projectiles_Boss == nil then
                All_projectiles_Boss = {}
            end

            self.boss = Actor.Find("boss"):GetComponent("Boss")
            self.player = Actor.Find("player"):GetComponent("Player")
        end,

        OnUpdate = function(self)
            if self.boss.hp <= 50 then
                self.phase = 2
            end

            if self.player.Health <= 0 then
                GameStarted = false
                Scene.Load("gameplay")
            end

            if self.boss.hp <= 0 then
                GameStarted = false
                Scene.Load("gameplay")
            end

        end 



}