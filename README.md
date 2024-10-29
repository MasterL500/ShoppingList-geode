# SHOPPING LIST (Geode Mod)

This is a Geode-exclusive mod that I developed for the game Geometry Dash. It's publicly available in Geode 3.0.0 (the game's mod loader), and it allows you to view how many items you have purchased—or haven't yet—from the game's five shops.

## Settings

* **Garage Button**: Removes the button of the Mod from the Garage Layer (Icon kit), so you can only access it inside the Shops. This option is enabled by default.
* **Disable Checkmark**: Icons aren't greyed out and displayed without a checkmark, if you want to see the designs. This option is enabled by default.

## Developer Commentary

* The mod hooks two layers in the game, GarageLayer (Icon Kit) and the ShopsLayer (Shops)
* This mod doesn't buy things for the player, just reads the stats of what they have
* In the Icon Kit, the mod adds a button in the "Shards-menu" node
* Most of the buttons, menus and sprites have an ID
* Portion of the code is commented, but not all
* Requires "Node-IDs" (v1.12.0 or above)

## To-do List

* ~~Select/Deselect all and sorting options~~ COMPLETED
* Add the paths of power and their costs
* MacOS compatibility
* Extra details

# Resources
* [Geode SDK Documentation](https://docs.geode-sdk.org/)
* [Geode SDK Source Code](https://github.com/geode-sdk/geode/)
* [Geode CLI](https://github.com/geode-sdk/cli)
* [Bindings](https://github.com/geode-sdk/bindings/)
* [Dev Tools](https://github.com/geode-sdk/DevTools)