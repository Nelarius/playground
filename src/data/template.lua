

-- this gets called just after the component is assigned
function activate()
    print("Hello, this is script speaking!")
end

-- this gets called in the update loop
function update( dt )
end

--  this gets called just before the component is removed
function deactivate()
    print("Script gonna hang up now!")
end
