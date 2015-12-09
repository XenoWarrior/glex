#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <memory>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <math.h>

#include "common.h"
#include "GameAssetManager.h"
#include "CubeAsset.h"
#include "GameAsset.h"

/**
 * GameWorld allows us to separate the management of the game world from the
 * nuts and bolts of game loop initialisation.  The GameWorld currently has
 * a very simplified scene graph consisiting of a single GameAssetManager.
 */
class GameWorld {
	public:
		/**
		* We thread the ApplicationMode through the GameWorld ss we want to read it
		* in from the user.  Threading the state through the various function calls
		* is preferable (in this case) to having some kind of global state.
		*/
		GameWorld(ApplicationMode);

		/**
		* Calling Draw() will draw the entire world.
		*/
		void Draw();

		/**
		* Game camera controller handler
		*/
		void CameraController(int keycode);
		void DoAction(int a);

	private:
		std::shared_ptr<GameAssetManager> asset_manager;

		GLfloat camera_speed = 0.1;

		GLfloat camera_x = 0.0;
		GLfloat camera_y = 0.0;

		glm::vec3 position = glm::vec3(0, 0, -3);
		glm::vec3 z_direction;
		glm::vec3 x_direction;
};

#endif // GAMEWORLD_H
