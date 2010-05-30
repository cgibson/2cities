-- Listing of the source files needed to build the client and server
-- Note this is a listing of the cpp files, not the object files

-- You do not need to specify the /src/ directory here. It's implied.
-- Just list the subdirectory, followed by the file's name. For example:
--
--      "graphics/console.cpp",
--
-- (note the trailing comma!)

-- By the way, double dashes are comments in Lua

client_srcs = {
    "audio/SoundManager.cpp",
    "graphics/hud.cpp",
    "graphics/shader.cpp",
    "graphics/tex.cpp",
    "graphics/console.cpp",
    "graphics/graphics.cpp",
    "graphics/renderer.cpp",
    "graphics/Lighting.cpp",
    "graphics/modelhandler.cpp",
    "graphics/Material.cpp",
    "graphics/OGLFT.cpp",
    "graphics/UIControl.cpp",
    "graphics/UILabel.cpp",
    "graphics/GameUI.cpp",
    "graphics/camera.cpp",
    "graphics/freelookcamera.cpp",
    "graphics/orbitalcamera.cpp",
    "graphics/skybox.cpp",
    "graphics/skyscraper.cpp",
    "helper/Vector.cpp",
	"helper/Point.cpp",
    "helper/GLSL_helper.cpp",
    "helper/FBOHelper.cpp",
    "helper/luahelper.cpp",
    "helper/texloader.cpp",
    "math/quaternion.cpp",
    "network/Network.cpp",
	"network/NetworkSystem.cpp",
    "network/NetworkPrivate.cpp",
    "network/NetworkClient.cpp",
    "network/NetworkServer.cpp",
	"network/NetworkManager.cpp",
    "physics/Physics.cpp",
    "physics/PhysicsBody.cpp",
    "scene/WorldObject.cpp",
    "scene/CustomObject.cpp",
    "scene/ComplexObject.cpp",
    "scene/factory.cpp",
    "scene/loader.cpp",
    "scene/model.cpp",
    "scene/ModelManager.cpp",
    "scene/BlackHole.cpp",
    "state/BuildState.cpp",
    "state/CarnageState.cpp",
    "state/InGameState.cpp",
    "state/StateManager.cpp",
    "system/gl_helper.cpp",
    "system/global.cpp",
    "system/io.cpp",
    "system/main.cpp"
}

server_srcs = {
    "helper/Vector.cpp",
    "math/quaternion.cpp",
    "network/Network.cpp",
    "network/NetworkSystem.cpp",
    "network/NetworkServer.cpp",
	"network/NetworkManager.cpp",
    "physics/Physics.cpp",
    "physics/PhysicsBody.cpp",
    "scene/WorldObject.cpp",
    "scene/factory.cpp",
    "scene/BlackHole.cpp",
    "state/BuildState.cpp",
    "state/CarnageState.cpp",
    "state/InGameState.cpp",
    "state/StateManager.cpp",
    "system/global.cpp",
    "system/io.cpp",
    "system/mainServer.cpp",
    "scene/CustomObject.cpp",
    "helper/Point.cpp"
}
