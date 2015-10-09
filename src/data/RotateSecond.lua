

-- this gets called just after the component is assigned
function activate()
end

pi = 3.1415927
t = 0.0

-- this gets called in the update loop
function update( dt )
    if entity:hasTransform() then
        t = t + dt
        local angle = 2*pi*t / 60.0
        entity.transform.rotation = pg.Quaternion( 0.0, 0.0, math.sin( -angle ), math.cos( -angle ) )
    end
end

--  this gets called just before the component is removed
function deactivate()
    print("Script gonna hang up now!")
end
