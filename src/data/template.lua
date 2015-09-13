

-- this gets called just after the component is assigned
function activate()
end

-- accumulated time
t = 0.0
-- angular valocity
av = 0.5
r = 8.0

-- this gets called in the update loop
function update( dt )
    if entity:hasTransform() then
        t = t + dt
        entity.transform.position = pg.Vector3f( r*math.cos( t*av ), 0.0, r*math.sin( t*av ) )
    end
end

--  this gets called just before the component is removed
function deactivate()
    print("Script gonna hang up now!")
end
