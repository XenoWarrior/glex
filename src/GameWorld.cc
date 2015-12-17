#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode)
{
	// Creates a CubeAsset in the asset manager
	asset_manager = std::make_shared<GameAssetManager>(mode);

	// Starting point for cube (0,0,0)
	std::shared_ptr<CubeAsset> tmp_obj = std::make_shared<CubeAsset>(glm::vec3(0.0, 0.0, 0.0));
	asset_manager->AddAsset(tmp_obj);
}

/**
 * Handles drawing of the camera and game world assets
 */
void GameWorld::Draw()
{
	// Update the facing direction
	UpdateFacingDirection();

	// Resetting camera position X
	if(camera_x >= 6.280)
		camera_x = 0.1;
	if(camera_x <= 0)
		camera_x = 6.280;

	// Resetting camera position Y
	if(camera_y >= 1.5f)
		camera_y = 1.5;
	if(camera_y <= -1.5f)
		camera_y = -1.5;

	glm::vec3 direction(
		cos(camera_y) * sin(camera_x),
		sin(camera_y),
		cos(camera_y) * cos(camera_x)
	);

	glm::vec3 move_direction(
		cos(camera_y) * sin(camera_x),
		0,
		cos(camera_y) * cos(camera_x)
	);
	z_direction = move_direction;

	x_direction = glm::vec3(
		sin(camera_x - 3.14f/2.0f),
		0,
		cos(camera_x - 3.14f/2.0f)
	);

	glm::vec3 vup = glm::cross(x_direction, direction);
	glm::mat4 cam_proj = glm::perspective(75.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	cam_view = glm::lookAt(
		position,
		position + direction,
		vup
	);
	glm::mat4 cam_mod(1.0f);

	glUniformMatrix4fv(0, 1, GL_FALSE, &cam_proj[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &cam_view[0][0]);
	glUniformMatrix4fv(2, 1, GL_FALSE, &cam_mod[0][0]);

	// Calls draw from the asset manager
	asset_manager->Draw();

	// Draws our grid
	DrawGrid();
}

/**
 * Handles action codes passed from main
 */
void GameWorld::DoAction(int a)
{
	if(a == 1)
	{
		offset_pos = GetOffset();
		std::shared_ptr<CubeAsset> new_cube = std::make_shared<CubeAsset>(glm::vec3(0.0f + int(round(position.x)) + offset_pos.x, 0.0f + int(round(position.y)) + offset_pos.y, 0.0f + int(round(position.z)) + offset_pos.z)); // Cube to make
		asset_manager->AddAsset(new_cube);
	}
	if(a == 2)
	{
		offset_pos = GetOffset();
		asset_manager->RemoveAsset(position, offset_pos);
	}
	if(a == 3)
	{
		asset_manager->RemoveAll();
		CreateShape("sphere", rand() % 19 + 1);
	}
	if(a == 4)
	{
		asset_manager->RemoveAll();
		CreateShape("cube", rand() % 19 + 1);
	}
}

/**
* Handles the keyboard inputs passed from main.
*/
void GameWorld::CameraController(int k)
{
	/* Slow collision checker
		std::vector<std::shared_ptr<CubeAsset>> asset_list = asset_manager->GetAssets();
		for(int i = 0; i < asset_list.size(); i++)
		{
			CubeAsset c = *asset_list[i];
			if(CheckCollision(c.GetVec3()))
			{
				std::cout << "[P]Detected collision with cube at: (X: " << position.x << ", Y: " << position.y << ", Z: " << position.z << ")" << std::endl;
				std::cout << "[C]Detected collision with cube at: (X: " << c.GetVec3().x << ", Y: " << c.GetVec3().y << ", Z: " << c.GetVec3().z << ")" << std::endl;
			}
		}
	*/

	// For W A S D
	if(k == 1)
		position += z_direction * camera_speed;
	if(k == 2)
		position -= x_direction * camera_speed;
	if(k == 3)
		position -= z_direction * camera_speed;
	if(k == 4)
		position += x_direction * camera_speed;

	// I K 
	if(k == 5)
		camera_y += 0.5f * camera_speed;
	if(k == 7)
		camera_y -= 0.5f * camera_speed;

	// J L
	if(k == 6)
		camera_x += 0.5f * camera_speed;
	if(k == 8)
		camera_x -= 0.5f * camera_speed;

	// For R F
	if(k == 9)
		position.y += 0.5f * camera_speed;
	if(k == 10)
		position.y -= 0.5f * camera_speed;

}

/**
 * Handles the mouse input for the camera
 */
void GameWorld::MoveCamera(glm::vec2 motion, glm::vec2 display)
{
	camera_x += ((display.x/2) - motion.x) * camera_speed / 100.0f;
	camera_y += ((display.y/2) - motion.y) * camera_speed / 100.0f;
}

/**
 * Calculates the camera facing direction
 */
void GameWorld::UpdateFacingDirection()
{
	if(camera_x <= point && camera_x >= 0.0 && f_pos != "N")
	{
		f_pos = "N";
		return;
	}
	if(camera_x >= point && camera_x <= point*3 && f_pos != "NW")
	{
		f_pos = "NW";
		return;
	}
	if(camera_x >= point*3 && camera_x <= point*5 && f_pos != "W")
	{
		f_pos = "W";
		return;
	}
	if(camera_x >= point*5 && camera_x <= point*7 && f_pos != "SW")
	{
		f_pos = "SW";
		return;
	}
	if(camera_x >= point*7 && camera_x <= point*9 && f_pos != "S")
	{
		f_pos = "S";
		return;
	}
	if(camera_x >= point*9 && camera_x <= point*11 && f_pos != "SE")
	{
		f_pos = "SE";
		return;
	}
	if(camera_x >= point*11 && camera_x <= point*13 && f_pos != "E")
	{
		f_pos = "E";
		return;
	}
	if(camera_x >= point*13 && camera_x <= point*15 && f_pos != "NE")
	{
		f_pos = "NE";
		return;
	}
	if(camera_x >= point*15 && camera_x <= point*17 && f_pos != "N")
	{
		f_pos = "N";
		return;
	}
}

/**
 * Returns an offset for the space in front of the camera
 * and also calculates block placement based on the camera angle
 */
glm::vec3 GameWorld::GetOffset()
{
	int x = 0, y = 0, z = 0, s = 0;
	float p =	 0.0f;

	if(f_pos == "N")
	{
		if(camera_x < 1)
			p = camera_x;
		else
			p = camera_x - point*16;
		
		s = int(floor(p * 10));
		z += block_dist;
		x += s;
	}
	if(f_pos == "E")
	{
		s = int(floor((point*12 - camera_x) * 10));
		x -= block_dist;
		z -= s;	
	}
	if(f_pos == "S")
	{
		s = int(floor((point*8 - camera_x) * 10));
		z -= block_dist;
		x += s;
	}
	if(f_pos == "W")
	{
		s = int(floor((point*4 - camera_x) * 10));
		x += block_dist;
		z += s;
	}

	if(f_pos == "NE")
	{
		z += block_dist;
		x -= block_dist;
	}
	if(f_pos == "SE")
	{
		z -= block_dist;
		x -= block_dist;
	}
	if(f_pos == "SW")
	{
		z -= block_dist;
		x += block_dist;
	}
	if(f_pos == "NW")
	{
		z += block_dist;
		x += block_dist;
	}

	s = int(camera_y * 10);
	if(s > block_dist)
	{
		s = block_dist;
	}
	if(s < (0 - block_dist))
	{
		s = (0 - block_dist);
	}
	y += s;

	if(x > block_dist)
		x = block_dist;
	if(y > block_dist)
		y = block_dist;
	if(z > block_dist)
		z = block_dist;

	if(x < 0 - block_dist)
		x = 0 - block_dist;
	if(y < 0 - block_dist)
		y = 0 - block_dist;
	if(z < 0 - block_dist)
		z = 0 - block_dist;

	return glm::vec3(x, y, z);
}

/**
 * Draws a grid in 3D space
 */
void GameWorld::DrawGrid()
{
	glm::vec3 offset_pos = GetOffset();

	glBegin(GL_QUADS);
	// Bottom
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.2f, int(round(position.y) + offset_pos.y) - 0.52f, int(round(position.z) + offset_pos.z) + 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.2f, int(round(position.y) + offset_pos.y) - 0.52f, int(round(position.z) + offset_pos.z) + 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.2f, int(round(position.y) + offset_pos.y) - 0.52f, int(round(position.z) + offset_pos.z) - 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.2f, int(round(position.y) + offset_pos.y) - 0.52f, int(round(position.z) + offset_pos.z) - 0.2f);
	
	// Top
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.2f, int(round(position.y) + offset_pos.y) + 0.52f, int(round(position.z) + offset_pos.z) + 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.2f, int(round(position.y) + offset_pos.y) + 0.52f, int(round(position.z) + offset_pos.z) + 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.2f, int(round(position.y) + offset_pos.y) + 0.52f, int(round(position.z) + offset_pos.z) - 0.2f);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.2f, int(round(position.y) + offset_pos.y) + 0.52f, int(round(position.z) + offset_pos.z) - 0.2f);

	// Front
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.125f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) - 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.125f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) - 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.125f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) - 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.125f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) - 0.52);

	// Back
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.125f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) + 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.125f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) + 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.125f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) + 0.52);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.125f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) + 0.52);

	// Left
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.52f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) + 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.52f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) - 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.52f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) - 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) + 0.52f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) + 0.125f);
	
	// Right
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.52f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) + 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.52f, int(round(position.y) + offset_pos.y) - 0.125f, int(round(position.z) + offset_pos.z) - 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.52f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) - 0.125f);
	glVertex3f(int(round(position.x) + offset_pos.x) - 0.52f, int(round(position.y) + offset_pos.y) + 0.125f, int(round(position.z) + offset_pos.z) + 0.125f);

	glEnd();

	for(float i = -20; i <= 20; i += 0.5)
	{
		glBegin(GL_LINES);
		glVertex3f(-20, -0.5, i);
		glVertex3f(20, -0.5, i);
		glVertex3f(i, -0.5, -20);
		glVertex3f(i, -0.5, 20);
		glEnd();
	}
}

/**
 * Generates a shape
 */
void GameWorld::CreateShape(std::string shape, int size)
{
	if(shape == "sphere")
	{
		for (int z = 0; z < size; z++)
		{
			for (int y = 0; y < size; y++)
			{
				for (int x = 0; x < size; x++)
				{
					if (sqrt((float) (x-size/2)*(x-size/2) + (y-size/2)*(y-size/2) + (z-size/2)*(z-size/2)) <= size/2)
					{
						asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.0+x, 0.0+y, 0.0+z)));
					}
				}
			}
		}
		return;
	}
	else if(shape == "cube")
	{
		for (int z = 0; z < size; z++)
		{
			for (int y = 0; y < size; y++)
			{
				for (int x = 0; x < size; x++)
				{
					asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.0+x, 0.0+y, 0.0+z)));
				}
			}
		}
		return;
	}
}


/**
 * Returns true if the position passed is already filled with a block
 */
bool GameWorld::CheckCollision(glm::vec3 point)
{
	if(point == glm::vec3(int(position.x), int(position.y), int(position.z)))
	{
		return true;
	}
	
	return false;
}

/**
 * Changes block distance
 */
void GameWorld::ChangeBlockDist(int i)
{
	block_dist += i;

	// Check distance
	if(block_dist < 1)
	{
		block_dist = 1;
	}
	if(block_dist > 10)
	{
		block_dist = 10;
	}
}