--[[
    All entities must be placed in the entities variable
    
    Each entity is an anonymous table entry
]]
entities = {
    {
        script = "data/template.lua",
        transform = {
            position = pg.Vector3f( 0.0, 0.0, 8.0 ),
            rotation = pg.Quaternion( 0.0, 0.0, 0.0, 1.0 ),
            scale = pg.Vector3f( 1.0, 1.0, 1.0 )
        },
        camera = {
            fov =           1.32,
            nearPlane =    0.1,
            farPlane =     1000.0,
            perspective =   true,
            active =        true
        }
    },
    
    {
        transform = {
            position = pg.Vector3f( 0.0, 0.0, 0.0 ),
            rotation = pg.Quaternion( 0.0, 0.0, 0.0, 1.0 ),
            scale = pg.Vector3f( 0.001, 0.01, 0.001 )
        },
        renderable = {
            model = "data/cube.dae",
            specular = {
                shininess = 80.0,
                specularColor = pg.Vector3f( 1.0, 1.0, 1.0 ),
                ambientColor = pg.Vector3f( 0.941, 0.455, 0.804 )
            }
        },        
        script = "data/RotateSecond.lua"
    },
    
    {
        transform = {
            position = pg.Vector3f( 0.0, 0.0, 0.0 ),
            rotation = pg.Quaternion( 0.0, 0.0, 0.0, 1.0 ),
            scale = pg.Vector3f( 0.0015, 0.008, 0.001 )
        },
        renderable = {
            model = "data/cube.dae",
            specular = {
                shininess = 80.0,
                specularColor = pg.Vector3f( 1.0, 1.0, 1.0 ),
                ambientColor = pg.Vector3f( 0.941, 0.455, 0.804 )
            }
        },        
        script = "data/RotateMinute.lua"
    },
    
    {
        transform = {
            position = pg.Vector3f( 0.0, 0.0, 0.0 ),
            rotation = pg.Quaternion( 0.0, 0.0, 0.0, 1.0 ),
            scale = pg.Vector3f( 0.0019, 0.005, 0.001 )
        },
        renderable = {
            model = "data/cube.dae",
            specular = {
                shininess = 80.0,
                specularColor = pg.Vector3f( 1.0, 1.0, 1.0 ),
                ambientColor = pg.Vector3f( 0.941, 0.455, 0.804 )
            }
        },        
        script = "data/RotateHour.lua"
    }
}

