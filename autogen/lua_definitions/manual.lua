-------------
-- globals --
-------------

--- @type MarioState[]
--- Array of MarioStates, from 0 to MAX_PLAYERS - 1
--- - Uses the local index, which is different between every player
--- - Index 0 always refers to the local player
gMarioStates = {}

--- @type NetworkPlayer[]
--- Array of NetworkPlayers, from 0 to MAX_PLAYERS - 1
--- - Uses the local index, which is different between every player
--- - Index 0 always refers to the local player
gNetworkPlayers = {}

--- @type Mod[]
--- Array of all mods loaded, starting from 0
--- - All mods are loaded in the same order for every player
--- - Index 0 is the first mod in the list (the top of the mod list)
gActiveMods = {}

--- @type Character[]
gCharacters = {}

--- @type Controller[]
gControllers = {}

--- @type GlobalTextures
gTextures = {}

--- @type GlobalObjectAnimations
gObjectAnimations = {}

--- @type GlobalObjectCollisionData
gGlobalObjectCollisionData = {}

--- @type PaintingValues
gPaintingValues = {}

--- @alias SyncTable table

--- @type SyncTable
--- Any keys added and modified to this table will be synced among everyone.
--- - This shouldn't be used to sync player-specific values; Use gPlayerSyncTable for that
--- - Note: Does not support tables as keys
gGlobalSyncTable = {}

--- @type SyncTable[]
--- An array of sync tables. Any change to any sync tables will be synced to everyone else.
--- - This array takes in a local index, however it automatically translates to the global index
--- - Note: Does not support tables as keys
gPlayerSyncTable = {}

--- @type LevelValues
gLevelValues = {}

--- @type BehaviorValues
gBehaviorValues = {}

--- @type FirstPersonCamera
--- The struct that contains the values for the first person camera
gFirstPersonCamera = {}

--- @type LakituState
--- The primary struct that controls the camera
--- - Local player only
gLakituState = {}

--- @type ServerSettings
gServerSettings = {}

--- @type NametagsSettings
gNametagsSettings = {}

-----------
-- hooks --
-----------

--- @param behaviorId BehaviorId | integer?  The behavior id of the object to modify. Pass in as `nil` to create a custom object
--- @param objectList ObjectList | integer Object list
--- @param replaceBehavior boolean Whether or not to completely replace the behavior
--- @param initFunction? fun(obj:Object) Run on object creation
--- @param loopFunction? fun(obj:Object) Run every frame
--- @param behaviorName? string Optional
--- @return BehaviorId BehaviorId Use if creating a custom object, otherwise can be ignored
--- Modify an object's behavior or create a new custom object
function hook_behavior(behaviorId, objectList, replaceBehavior, initFunction, loopFunction, behaviorName)
    -- ...
end

--- @param command string The command to run. Should be easy to type
--- @param description string Should describe what the command does and how to use it
--- @param func fun(msg:string): boolean Run upon activating the command. Return `true` to confirm the command has succeeded
--- @return nil
function hook_chat_command(command, description, func)
    -- ...
end

--- @param command string The command to change the description of
--- @param description string The description to change to
function update_chat_command_description(command, description)
    -- ...
end

--- @param hookEventType LuaHookedEventType When a function should run
--- @param func fun(...: any): any The function to run
--- Different hooks can pass in different parameters and have different return values. Be sure to read the hooks guide for more information.
function hook_event(hookEventType, func)
    -- ...
end

--- @class ActionTable
    --- @field every_frame fun(m:MarioState):integer?
    --- @field gravity fun(m:MarioState):integer?

--- @param actionId integer The action to replace
--- @param funcOrFuncTable fun(m:MarioState):integer? | ActionTable The new behavior of the action
--- @param interactionType? InteractionFlag Optional; The flag that determines how the action interacts with other objects
--- If a function table is used, it must be in the form of `{ act_hook = [func], ... }`. Current action hooks include:
--- - every_frame
--- - gravity
function hook_mario_action(actionId, funcOrFuncTable, interactionType)
    -- ...
end

--- @param syncTable SyncTable Must be the gGlobalSyncTable or gPlayerSyncTable[] or one of their child tables
--- @param field string Field name
--- @param tag any An additional parameter
--- @param func fun(tag:any, oldVal:any, newVal:any) Run when the specified field has been changed
function hook_on_sync_table_change(syncTable, field, tag, func)
    -- ...
end

---------------
-- functions --
---------------

--- @param t number Angle
--- @return number
function sins(t)
    -- ...
end

--- @param t number Angle
--- @return number
function coss(t)
    -- ...
end

--- @param y number
--- @param x number
--- @return integer
function atan2s(y, x)
    -- ...
end

--- @param objFieldTable table<any, "u32"|"s32"|"f32">
--- @return nil
--- Keys must start with `o` and values must be `"u32"`, `"s32"`, or `"f32"`
function define_custom_obj_fields(objFieldTable)
    -- ...
end

--- @param object Object Object to sync
--- @param standardSync boolean Automatically syncs common fields and syncs with distance. If `false`, all syncing must be done with `network_send_object`.
--- @param fieldTable table<string> The fields to sync
--- @return nil
--- All synced fields must start with `o` and there should not be any keys, just values
function network_init_object(object, standardSync, fieldTable)
    -- ...
end

--- @param object Object Object to sync
--- @param reliable boolean Whether or not the game should try to resend the packet in case it gets lost, good for important packets
--- @return nil
--- Sends a sync packet to sync up the object with everyone else
function network_send_object(object, reliable)
    -- ...
end

--- @param reliable boolean Whether or not the game should try to resend the packet in case its lost, good for important packets
--- @param dataTable table Table of values to be included in the packet
--- @return nil
--- `dataTable` can only contain strings, integers, numbers, booleans, and nil
function network_send(reliable, dataTable)
    -- ...
end

--- @param toLocalIndex integer The local index to send the packet to
--- @param reliable boolean Whether or not the game should try to resend the packet in case its lost, good for important packets
--- @param dataTable table Table of values to be included in the packet
--- @return nil
--- `dataTable` can only contain strings, integers, numbers, booleans, and nil
function network_send_to(toLocalIndex, reliable, dataTable)
    -- ...
end

--- @param textureName string
--- @return TextureInfo
--- Gets the `TextureInfo` of a texture by name
--- - Note: This also works with vanilla textures
function get_texture_info(textureName)
    -- ...
end

--- @param texInfo TextureInfo
--- @param x number
--- @param y number
--- @param scaleW number
--- @param scaleH number
--- @return nil
--- Renders a texture to the screen
function djui_hud_render_texture(texInfo, x, y, scaleW, scaleH)
    -- ...
end

--- @param texInfo TextureInfo
--- @param x number
--- @param y number
--- @param scaleW number
--- @param scaleH number
--- @param tileX number
--- @param tileY number
--- @param tileW number
--- @param tileH number
--- @return nil
--- Renders a tile of a texture to the screen
function djui_hud_render_texture_tile(texInfo, x, y, scaleW, scaleH, tileX, tileY, tileW, tileH)
    -- ...
end

--- @param texInfo TextureInfo
--- @param prevX number
--- @param prevY number
--- @param prevScaleW number
--- @param prevScaleH number
--- @param x number
--- @param y number
--- @param scaleW number
--- @param scaleH number
--- @return nil
--- Renders an interpolated texture to the screen
function djui_hud_render_texture_interpolated(texInfo, prevX, prevY, prevScaleW, prevScaleH, x, y, scaleW, scaleH)
    -- ...
end

--- @param texInfo TextureInfo
--- @param prevX number
--- @param prevY number
--- @param prevScaleW number
--- @param prevScaleH number
--- @param x number
--- @param y number
--- @param scaleW number
--- @param scaleH number
--- @param tileX number
--- @param tileY number
--- @param tileW number
--- @param tileH number
--- @return nil
--- Renders an interpolated tile of a texture to the screen
function djui_hud_render_texture_tile_interpolated(texInfo, prevX, prevY, prevScaleW, prevScaleH, x, y, scaleW, scaleH, tileX, tileY, tileW, tileH)
    -- ...
end

--- @param textureName string
--- @param overrideTexInfo TextureInfo
--- @return nil
--- Overrides a texture with a custom `TextureInfo`
--- * textureName must be the codename of a vanilla texture, you can find these in files such as `texture.inc.c`s
--- * overrideTexInfo can be any TextureInfo
function texture_override_set(textureName, overrideTexInfo)
    -- ...
end

--- @param textureName string
--- @return nil
--- Resets an overridden texture
function texture_override_reset(textureName)
    -- ...
end

--- @class bhvData
--- @field behavior BehaviorId
--- @field behaviorArg integer

--- @param levelNum LevelNum | integer
--- @param func fun(areaIndex:number, bhvData:bhvData, macroBhvIds:BehaviorId[], macroBhvArgs:integer[])
--- @return nil
--- When `func` is called, arguments are filled depending on the level command:
--- - `AREA` command: only `areaIndex` is filled. It's a number.
--- - `OBJECT` command: only `bhvData` is filled. `bhvData` is a table with two fields: `behavior` and `behaviorArg`.
--- - `MACRO` command: only `macroBhvIds` and `macroBhvArgs` are filled. `macrobhvIds` is a list of behavior ids. `macroBhvArgs` is a list of behavior params. Both lists have the same size and start at index 0.
function level_script_parse(levelNum, func)
    -- ...
end

--- @param name string
--- @param flags integer
--- @param animYTransDivisor integer
--- @param startFrame integer
--- @param loopStart integer
--- @param loopEnd integer
--- @param values table
--- @param index table
--- @return nil
--- Registers an animation that can be used in objects if `smlua_anim_util_set_animation` is called
function smlua_anim_util_register_animation(name, flags, animYTransDivisor, startFrame, loopStart, loopEnd, values, index)
    -- ...
end

--- @param message string The message to log
--- @param level? ConsoleMessageLevel Optional; Determines whether the message should appear as info, a warning or an error.
--- @return nil
--- Logs a message to the in-game console
function log_to_console(message, level)
    -- ...
end