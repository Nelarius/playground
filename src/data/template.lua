


-- this gets called just after the component is assigned
function activate()
end

vz = -1.0

-- this gets called in the update loop
function update( dt )
    if entity:hasTransform() then
        print( entity.transform.position.z + vz * dt )
        print(entity.transform.position.z)
        entity.transform.position.z = -10.0
    end
end

--  this gets called just before the component is removed
function deactivate()
    print("Script gonna hang up now!")
end
