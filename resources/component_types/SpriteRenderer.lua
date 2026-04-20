SpriteRenderer = {

    image = "",
  OnStart = function(self)
    self.t = self.actor:GetComponent("Transform")
  end,

  OnUpdate = function(self)
    Image.Draw(self.image,self.t.x,self.t.y)
  end
}