

-- this gets called just after the component is assigned
function activate()
end

-- accumulated time
t = 0.0
-- angular velocity
av = 0.5
r = 8.0

-- this gets called in the update loop
function update( dt )
    if entity:hasTransform() then
        t = t + dt
        q = pg.Quaternion( 0.0, 0.0, math.sin( -dt*10.0 ), math.cos( -dt*10.0 ) )
        entity.transform.rotation = pg.Quaternion( 0.0, 0.0, math.sin( -t*0.5 ), math.cos( -t*0.5 ) )
    end
end

--  this gets called just before the component is removed
function deactivate()
    print("Script gonna hang up now!")
end
