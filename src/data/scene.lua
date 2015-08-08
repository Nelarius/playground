
print("Now creating scene.")

--[[
    All entities must be placed in the entities variable
    
    Each entity is an anonymous table entry
]]
entities = {
    {
        script = "data/template.lua",
        transform = {
            position = pg.Vector3f( 0.0, 0.0, 8.0 ),
            scale = pg.Vector3f( 1.0, 1.0, 1.0 )
        },
        camera = {
            fov =           1.15,
            nearPlane =    0.1,
            farPlane =     1000.0,
            perspective =   true,
            active =        true
        }
    },
    
    {
        transform = {
            position = pg.Vector3f( 0.0, 0.0, 0.0 ),
            scale = pg.Vector3f( 1.0, 1.0, 1.0 )
        },
        renderable = {
            model = "data/cow.obj",
            ambient = {
                color = pg.Vector3f( 0.5, 0.5, 0.1 )
            }
        }
    }
}

