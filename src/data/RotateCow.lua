

-- this gets called just after the component is assigned
function activate()
end

pi = 3.1415927
t = 0.0

function update( dt )
    t = t + dt
    if entity:hasTransform() then
        entity.transform.rotation = pg.Quaternion( 0.0, math.sin(-0.2*t), 0.0, math.cos(-0.2*t))
    end
end

--  this gets called just before the component is removed
function deactivate()
    print("Script gonna hang up now!")
end
