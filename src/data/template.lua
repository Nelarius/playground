


-- this gets called just after the component is assigned
function activate()
    print( entity:hasTransform() )
end

-- this gets called in the update loop
function update( dt )
    -- comment
end

--  this gets called just before the component is removed
function deactivate()
    print("Script gonna hang up now!")
end
