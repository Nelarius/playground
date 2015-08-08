


-- this gets called just after the component is assigned
function activate()
end

vz = -1.0
z = 5.0

-- this gets called in the update loop
function update( dt )
    if entity:hasTransform() then
        z = z + vz * dt
        entity.transform.position = pg.Vector3f( 0.0, 0.0, z )
    end
end

--  this gets called just before the component is removed
function deactivate()
    print("Script gonna hang up now!")
end
