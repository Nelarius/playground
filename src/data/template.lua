

-- this gets called just after the component is assigned
function activate()
end

-- accumulated time
t = 0.0
-- angular velocity
av = 0.5
r = 6.0

-- this gets called in the update loop
function update( dt )
    t = t + dt
    if entity:hasTransform() then
        entity.transform.position = pg.Vector3f( r*math.cos(t), 0.5, r*math.sin(t))
    end
end

--  this gets called just before the component is removed
function deactivate()
    print("Script gonna hang up now!")
end
