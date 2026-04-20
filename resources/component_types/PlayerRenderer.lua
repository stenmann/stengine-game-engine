PlayerRenderer = {

    image = "",
  OnStart = function(self)
    self.t = self.actor:GetComponent("Transform")
  end,

  OnUpdate = function(self)
    --Image.Draw(self.image,self.t.x,self.t.y)
    Image.DrawEx(self.image,self.t.x,self.t.y,self.t.rotation_degrees,self.t.scale_x,self.t.scale_y,0.5, 0.5, 255, 255, 255, 255, 1)
  end
}