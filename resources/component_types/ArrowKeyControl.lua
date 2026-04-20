ArrowKeyControl = {
    speed = 0.06,

    OnStart = function(self)
        self.t = self.actor:GetComponent("Transform")
    end,

    OnUpdate = function(self)
        if not GameStarted then 
            return
        end

        if Input.GetKey("right") then 
            self.t.x = self.t.x + self.speed
            --self.t.rotation_degrees = 90
        end

        if Input.GetKey("left") then 
            self.t.x = self.t.x - self.speed
            --self.t.rotation_degrees = 270
        end
        if Input.GetKey("up") then 
            self.t.y = self.t.y - self.speed
            --self.t.rotation_degrees = 0;
        end
        if Input.GetKey("down") then 
            self.t.y = self.t.y + self.speed
            --self.t.rotation_degrees = 180
        end

        

    end
    

        

    
}