


-- this gets called just after the component is assigned
function activate()
    if entity:hasTransform() then
        transform.position = pg.Vector3f( 10.0, 10.0, 10.0 )
    end
end

-- this gets called in the update loop
function update( dt )
    -- comment
end

--  this gets called just before the component is removed
function deactivate()
    print("Script gonna hang up now!")
end
