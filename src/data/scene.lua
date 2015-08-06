
print("Now creating scene.")

--[[
    All entities must be placed in the entities variable
    
    Each entity is an anonymous table entry
]]
entities = {
    {
        script = "data/template.lua",
        transform = {
            position = pg.Vector3f( 0.0, 0.0, 0.0 )
            scale = pg.Vector3f( 1.0, 1.0, 1.0 )
        },
        camera = {
            fov =           1.15,
            near_plane =    0.1,
            far_plane =     1000.0,
            perspective =   true,
            active =        true
        }
    }
}

