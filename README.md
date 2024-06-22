# Shopping List (GEODE MOD)

Geode-exclusive mod that I programmed for the game Geometry Dash, publicly available in Geode 3.0.0 (Modloader to the game), where you can see the amount of Items you have bought (or haven't bought yet) from the five available shops in the game.

## Settings

* **Garage Button**: Removes the button of the mod from the Garage layer (Icon kit) so you can only see it in the Shops. This option is enabled by default.
* **Disable Checkmark**: Icons aren't greyed out and displayed without the checkmark, if you want to see all the designs. This option is disabled by default.

## Developer Commentary

* The mod hooks two layers in the game, GarageLayer (Icon Kit) and the ShopsLayer (Shops)
* This mod doesn't buy things for the player, just reads the stats of what they have
* In the Icon Kit, the mod adds a button in the "Shards-menu" node
* Most of the buttons, menus and sprites have an ID
* Portion of the code is commented, but not all
* Requires "Node-IDs" (v1.12.0 or above)

## To-do List

* Select/Deselect all and sorting options
* Add the paths of power and their costs
* MacOS compatibility
* Extra details